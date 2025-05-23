/*
 * @file AutoControl.c
 * @brief Implementation of the Auto Control Interface
 *
 * This file provides the implementation of the functions declared in the
 * `auto_control.h` header file. It includes the logic for initializing the
 * Auto Control module and performing control steps based on input commands
 * from the `t818_drive_control.h` module.
 *
 * Created on: Jun 24, 2024
 * Authors: Alessio Guarini, Antonio Vitale
 */

#include "auto_control.h"

/**
 * @brief Checks if parking is enabled based on speed command.
 *
 * This function checks whether parking is enabled based on the provided speed feedback.
 * Parking could be enabled if the absolute value of the speed command is less than
 * a predefined threshold (`speed_threshold`).
 *
 * @param speed_feedback The speed feedback to be checked.
 * @return CD_TRUE if parking is enabled, CD_FALSE otherwise.
 */
static inline bool8u __check_parking_enable(int16_t speed_feedback) {
	static const int16_t speed_threshold = 10U; /**< Threshold for speed feedback to enable parking */
	bool8u ret = CD_FALSE;

	if ((speed_feedback < speed_threshold) && (speed_feedback > (-1 * speed_threshold))) {
		ret = CD_TRUE;
	}
	return ret;
}

/**
 * @brief Updates the Auto Control state in the PARKING state.
 *
 * This inline function updates the Auto Control state when the vehicle is in the
 * PARKING state based on the input commands.
 *
 * @param auto_control Pointer to the Auto Control instance.
 * @return The new state of the Auto Control.
 */
static inline auto_control_state __update_auto_control_state_parking(
		auto_control_t *auto_control) {
	auto_control_state new_state;
	if (auto_control->driving_commands->buttons[AUTO_CONTROL_GEAR_UP_BUTTON].state
			== BUTTON_PRESSED) {
		new_state = RETRO;
	} else if (auto_control->driving_commands->buttons[AUTO_CONTROL_NEUTRAL_BUTTON].state
			== BUTTON_PRESSED) {
		new_state = NEUTRAL;
	} else {
		new_state = PARKING;
	}
	return new_state;
}

/**
 * @brief Updates the Auto Control state in the RETRO state.
 *
 * This inline function updates the Auto Control state when the vehicle is in the
 * RETRO state based on the input commands.
 *
 * @param auto_control Pointer to the Auto Control instance.
 * @return The new state of the Auto Control.
 */
static inline auto_control_state __update_auto_control_state_retro(
		auto_control_t *auto_control) {
	auto_control_state new_state;
	if ((auto_control->driving_commands->buttons[AUTO_CONTROL_GEAR_UP_BUTTON].state
			== BUTTON_PRESSED)
			|| (auto_control->driving_commands->buttons[AUTO_CONTROL_NEUTRAL_BUTTON].state
					== BUTTON_PRESSED)) {
		new_state = NEUTRAL;
	} else if (((auto_control->driving_commands->buttons[AUTO_CONTROL_GEAR_DOWN_BUTTON].state
			== BUTTON_PRESSED)
			|| (auto_control->driving_commands->buttons[AUTO_CONTROL_PARKING_BUTTON].state
					== BUTTON_PRESSED))
			&& (__check_parking_enable(auto_control->auto_data_feedback->speed)
					== CD_TRUE)) {
		new_state = PARKING;
	} else {
		new_state = RETRO;
	}
	return new_state;
}

/**
 * @brief Updates the Auto Control state in the NEUTRAL state.
 *
 * This inline function updates the Auto Control state when the vehicle is in the
 * NEUTRAL state based on the input commands.
 *
 * @param auto_control Pointer to the Auto Control instance.
 * @return The new state of the Auto Control.
 */
static inline auto_control_state __update_auto_control_state_neutral(
		auto_control_t *auto_control) {
	auto_control_state new_state;
	if (auto_control->driving_commands->buttons[AUTO_CONTROL_GEAR_UP_BUTTON].state
			== BUTTON_PRESSED) {
		new_state = DRIVE;
	} else if (auto_control->driving_commands->buttons[AUTO_CONTROL_GEAR_DOWN_BUTTON].state
			== BUTTON_PRESSED) {
		new_state = RETRO;
	} else if ((auto_control->driving_commands->buttons[AUTO_CONTROL_PARKING_BUTTON].state
			== BUTTON_PRESSED)
			&& (__check_parking_enable(auto_control->auto_data_feedback->speed)
					== CD_TRUE)) {
		new_state = PARKING;
	} else {
		new_state = NEUTRAL;
	}
	return new_state;
}

/**
 * @brief Updates the Auto Control state in the DRIVE state.
 *
 * This inline function updates the Auto Control state when the vehicle is in the
 * DRIVE state based on the input commands.
 *
 * @param auto_control Pointer to the Auto Control instance.
 * @return The new state of the Auto Control.
 */
static inline auto_control_state __update_auto_control_state_drive(
		auto_control_t *auto_control) {
	const t818_driving_commands_t *drive_comm =
			(t818_driving_commands_t*) auto_control->driving_commands;
	auto_control_state new_state;
	if ((drive_comm->buttons[AUTO_CONTROL_GEAR_DOWN_BUTTON].state
			== BUTTON_PRESSED)
			|| (drive_comm->buttons[AUTO_CONTROL_NEUTRAL_BUTTON].state
					== BUTTON_PRESSED)) {
		new_state = NEUTRAL;
	} else if ((__check_parking_enable(auto_control->auto_data_feedback->speed)
			== CD_TRUE)
			&& drive_comm->buttons[AUTO_CONTROL_PARKING_BUTTON].state
					== BUTTON_PRESSED) {
		new_state = PARKING;
	} else {
		new_state = DRIVE;
	}
	return new_state;
}

/**
 * @brief Applies the basic rules for the Auto Control module.
 *
 * This inline function applies the basic control rules for the Auto Control module,
 * setting the necessary data fields based on the current driving commands.
 *
 * @param auto_control Pointer to the Auto Control instance.
 */
static inline void __basic_rules(auto_control_t *auto_control) {
	auto_control_data_t *const auto_data =
			(auto_control_data_t*) &(auto_control->auto_control_data);
	const t818_driving_commands_t *drive_comm =
			(t818_driving_commands_t*) auto_control->driving_commands;
	auto_data->self_driving = CD_TRUE;
	auto_data->advanced_mode = CD_FALSE;
	auto_data->state_control = CD_FALSE;
	auto_data->speed_mode = CD_FALSE;

	auto_data->right_light =
			drive_comm->buttons[AUTO_CONTROL_RIGHT_LIGHT_BUTTON].state;
	auto_data->left_light =
			drive_comm->buttons[AUTO_CONTROL_LEFT_LIGHT_BUTTON].state;
	auto_data->front_light =
			drive_comm->buttons[AUTO_CONTROL_FRONT_LIGHT_BUTTON].state;

	auto_data->mode_selection = AUTO_CONTROL_MODE_SELECTION_FIELD;

	auto_data->steering = (int16_t) roundf(
			map_value_float(drive_comm->wheel_steering_degree,
			T818_MIN_STEERING_ANGLE,
			T818_MAX_STEERING_ANGLE, AUTO_CONTROL_MIN_STEERING,
			AUTO_CONTROL_MAX_STEERING));
}

/*
 * @brief
 */
static inline uint16_t __calculate_speed(float current_speed, float set_point) {
	uint16_t speed_u16 = 0U;
	float speed = calculate_new_smoothed_value(current_speed,
			set_point * ((float)AUTO_CONTROL_MAX_SPEED),
			AUTO_CONTROL_SPEED_MAX_INCREMENT, AUTO_CONTROL_SPEED_MAX_DECREMENT);

	speed_u16 = (uint16_t) roundf(speed);
	if(speed_u16 > AUTO_CONTROL_MAX_SPEED){
		speed_u16 = AUTO_CONTROL_MAX_SPEED;
	}

	return speed_u16;
}

/*
 * @brief
 */
static inline uint16_t __calculate_braking(float braking_module) {
	uint16_t brake = (uint16_t) roundf(braking_module * ((float) AUTO_CONTROL_MAX_BRAKING));

	if(brake > AUTO_CONTROL_MAX_BRAKING){
		brake = AUTO_CONTROL_MAX_BRAKING;
	}

	return brake;
}

/**
 * @brief Applies the moving rules for the Auto Control module.
 *
 * This inline function applies the moving control rules for the Auto Control module,
 * setting the necessary data fields based on the current driving commands.
 *
 * @param auto_control Pointer to the Auto Control instance.
 */
static inline void __moving_rules(auto_control_t *auto_control) {
	uint16_t braking = __calculate_braking(
			auto_control->driving_commands->braking_module);
	uint16_t speed;
	auto_control->auto_control_data.EBP = CD_FALSE;

	if (braking > AUTO_CONTROL_MIN_BRAKING) {
		speed = AUTO_CONTROL_MIN_SPEED;
	} else {
		float current_speed = (float) auto_control->auto_control_data.speed;
		speed = __calculate_speed(current_speed,
				auto_control->driving_commands->throttling_module);
	}

	auto_control->auto_control_data.braking = braking;
	auto_control->auto_control_data.speed = speed;
}

/**
 * @brief Applies the neutral rules for the Auto Control module.
 *
 * This inline function applies the neutral control rules for the Auto Control module,
 * setting the necessary data fields based on the current driving commands.
 *
 * @param auto_control Pointer to the Auto Control instance.
 */
static inline void __neutral_rules(auto_control_t *auto_control) {
	__basic_rules(auto_control);
	auto_control->auto_control_data.EBP = CD_FALSE;
	auto_control->auto_control_data.gear_shift =
	AUTO_CONTROL_GEAR_SHIFT_NEUTRAL;
	auto_control->auto_control_data.braking = __calculate_braking(
			auto_control->driving_commands->braking_module);
}

/**
 * @brief Applies the parking rules for the Auto Control module.
 *
 * This inline function applies the parking control rules for the Auto Control module,
 * setting the necessary data fields based on the current driving commands.
 *
 * @param auto_control Pointer to the Auto Control instance.
 */
static inline void __parking_rules(auto_control_t *auto_control) {
	__basic_rules(auto_control);
	auto_control->auto_control_data.EBP = CD_TRUE;
	auto_control->auto_control_data.gear_shift =
	AUTO_CONTROL_GEAR_SHIFT_PARK;
	auto_control->auto_control_data.braking = AUTO_CONTROL_PARKING_BRAKING;
	auto_control->auto_control_data.speed = AUTO_CONTROL_MIN_SPEED;
}

/**
 * @brief Applies the retro rules for the Auto Control module.
 *
 * This inline function applies the retro control rules for the Auto Control module,
 * setting the necessary data fields based on the current driving commands.
 *
 * @param auto_control Pointer to the Auto Control instance.
 */
static inline void __retro_rules(auto_control_t *auto_control) {
	__basic_rules(auto_control);
	__moving_rules(auto_control);
	auto_control->auto_control_data.gear_shift = AUTO_CONTROL_GEAR_SHIFT_RETRO;
}

/**
 * @brief Applies the drive rules for the Auto Control module.
 *
 * This inline function applies the drive control rules for the Auto Control module,
 * setting the necessary data fields based on the current driving commands.
 *
 * @param auto_control Pointer to the Auto Control instance.
 */
static inline void __drive_rules(auto_control_t *auto_control) {
	__basic_rules(auto_control);
	__moving_rules(auto_control);
	auto_control->auto_control_data.gear_shift = AUTO_CONTROL_GEAR_SHIFT_DRIVE;
}

/**
 * @brief Initializes auto control data structure with default values.
 *
 * This function initializes the fields of the `auto_control_data_t` structure
 * with default values.
 *
 * @param auto_control_data Pointer to the auto control data structure to initialize.
 */
static inline void __auto_control_data_init(
		auto_control_data_t *auto_control_data) {
	auto_control_data->self_driving = CD_FALSE;
	auto_control_data->advanced_mode = CD_FALSE;
	auto_control_data->state_control = CD_FALSE;
	auto_control_data->speed_mode = CD_FALSE;
	auto_control_data->right_light = CD_FALSE;
	auto_control_data->left_light = CD_FALSE;
	auto_control_data->front_light = CD_FALSE;
	auto_control_data->EBP = CD_TRUE;
	auto_control_data->mode_selection =
	AUTO_CONTROL_MODE_SELECTION_FIELD;
	auto_control_data->gear_shift =
	AUTO_CONTROL_GEAR_SHIFT_PARK;
	auto_control_data->steering = 0;
	auto_control_data->braking = AUTO_CONTROL_MAX_BRAKING;
	auto_control_data->speed = AUTO_CONTROL_MIN_SPEED;
}


AutoControl_StatusTypeDef auto_control_init(auto_control_t *auto_control,
		t818_driving_commands_t *driving_commands, auto_data_feedback_t *auto_data_feedback) {
	AutoControl_StatusTypeDef status = AUTO_CONTROL_ERROR;

	if ((auto_control != NULL) && (driving_commands != NULL) && (auto_data_feedback != NULL)) {
		auto_control->driving_commands = driving_commands;
		__auto_control_data_init(&auto_control->auto_control_data);
		auto_control->auto_data_feedback=auto_data_feedback;
		auto_control->state = PARKING;

		status = AUTO_CONTROL_OK;
	}

	return status;
}

AutoControl_StatusTypeDef auto_control_step(auto_control_t *auto_control) {
	AutoControl_StatusTypeDef status = AUTO_CONTROL_ERROR;

	if (auto_control != NULL) {
		switch (auto_control->state) {
		case PARKING:
			__parking_rules(auto_control);
			auto_control->state = __update_auto_control_state_parking(
					auto_control);
			status = AUTO_CONTROL_OK;
			break;
		case RETRO:
			__retro_rules(auto_control);
			auto_control->state = __update_auto_control_state_retro(
					auto_control);
			status = AUTO_CONTROL_OK;
			break;
		case NEUTRAL:
			__neutral_rules(auto_control);
			auto_control->state = __update_auto_control_state_neutral(
					auto_control);
			status = AUTO_CONTROL_OK;
			break;
		case DRIVE:
			__drive_rules(auto_control);
			auto_control->state = __update_auto_control_state_drive(
					auto_control);
			status = AUTO_CONTROL_OK;
			break;
		default:
			break;
		}
	}

	return status;
}
