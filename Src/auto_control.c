/*
 * AutoControl.c
 *
 *  Created on: Jun 24, 2024
 *      Author: vital
 */

#include <auto_control.h>

AutoControl_StatusTypeDef auto_control_init(auto_control_t *auto_control,
		t818_driving_commands_t *driving_commands) {
	AutoControl_StatusTypeDef status = AUTO_CONTROL_ERROR;

	if (auto_control != NULL && driving_commands != NULL) {
		auto_control->driving_commands = driving_commands;
		auto_control->self_driving = CD_FALSE;
		auto_control->advanced_mode = CD_FALSE;
		auto_control->state_control = CD_FALSE;
		auto_control->speed_mode = CD_FALSE;
		auto_control->right_light = CD_FALSE;
		auto_control->left_light = CD_FALSE;
		auto_control->front_light = CD_FALSE;
		auto_control->EBP = CD_TRUE;
		auto_control->mode_selection = AUTO_CONTROL_MODE_SELECTION_DIFFERENT;
		auto_control->gear_shift = AUTO_CONTROL_GEAR_SHIFT_PARK;
		auto_control->steering = 0;
		auto_control->braking = AUTO_CONTROL_MIN_BRAKING;
		auto_control->speed = AUTO_CONTROL_MIN_SPEED;
		auto_control->state = PARKING;

		status = AUTO_CONTROL_OK;
	}

	return status;

}

static inline auto_control_state __update_auto_control_state_parking(auto_control_t *auto_control){
	auto_control_state new_state;
	if (auto_control->driving_commands->buttons[AUTO_CONTROL_GEAR_UP_BUTTON].state == BUTTON_PRESSED) {
		new_state = RETRO;
	} else if (auto_control->driving_commands->buttons[AUTO_CONTROL_NEUTRAL_BUTTON].state == BUTTON_PRESSED) {
		new_state = NEUTRAL;
	} else {
		new_state = PARKING;
	}
	return new_state;
}

static inline auto_control_state __update_auto_control_state_retro(auto_control_t *auto_control){
	auto_control_state new_state;
	if ((auto_control->driving_commands->buttons[AUTO_CONTROL_GEAR_UP_BUTTON].state == BUTTON_PRESSED) ||
		(auto_control->driving_commands->buttons[AUTO_CONTROL_NEUTRAL_BUTTON].state == BUTTON_PRESSED)) {
		new_state = NEUTRAL;
	} else if (	(auto_control->driving_commands->buttons[AUTO_CONTROL_GEAR_DOWN_BUTTON].state == BUTTON_PRESSED) ||
				(auto_control->driving_commands->buttons[AUTO_CONTROL_PARKING_BUTTON].state == BUTTON_PRESSED)) {
		new_state = PARKING;
	} else {
		new_state = RETRO;
	}
	return new_state;
}
static inline auto_control_state __update_auto_control_state_neutral(auto_control_t *auto_control){
	auto_control_state new_state;
	if (auto_control->driving_commands->buttons[AUTO_CONTROL_GEAR_UP_BUTTON].state == BUTTON_PRESSED) {
		new_state = DRIVE;
	} else if (auto_control->driving_commands->buttons[AUTO_CONTROL_GEAR_DOWN_BUTTON].state == BUTTON_PRESSED) {
		new_state = RETRO;
	} else if (auto_control->driving_commands->buttons[AUTO_CONTROL_PARKING_BUTTON].state == BUTTON_PRESSED) {
		new_state = PARKING;
	} else {
		new_state = NEUTRAL;
	}
	return new_state;
}
static inline auto_control_state __update_auto_control_state_drive(auto_control_t *auto_control){
	auto_control_state new_state;
	if ((auto_control->driving_commands->buttons[AUTO_CONTROL_GEAR_DOWN_BUTTON].state == BUTTON_PRESSED) ||
		(auto_control->driving_commands->buttons[AUTO_CONTROL_NEUTRAL_BUTTON].state == BUTTON_PRESSED)) {
		new_state = NEUTRAL;
	} else if (auto_control->driving_commands->buttons[AUTO_CONTROL_PARKING_BUTTON].state == BUTTON_PRESSED) {
		new_state = PARKING;
	} else {
		new_state = DRIVE;
	}
	return new_state;
}


inline static void __basic_rules(auto_control_t *auto_control) {
	auto_control->self_driving = CD_TRUE;
	auto_control->advanced_mode = CD_FALSE;
	auto_control->state_control = CD_FALSE;
	auto_control->speed_mode = CD_FALSE;

	auto_control->right_light =
			auto_control->driving_commands->buttons[AUTO_CONTROL_RIGHT_LIGHT_BUTTON].state;
	auto_control->left_light =
			auto_control->driving_commands->buttons[AUTO_CONTROL_LEFT_LIGHT_BUTTON].state;
	auto_control->front_light =
			auto_control->driving_commands->buttons[AUTO_CONTROL_FRONT_LIGHT_BUTTON].state;

	auto_control->mode_selection = AUTO_CONTROL_MODE_SELECTION_DIFFERENT;

	auto_control->steering = (int16_t) roundf(
			map_value_float(
					auto_control->driving_commands->wheel_steering_degree,
					T818_MIN_STEERING_ANGLE, T818_MAX_STEERING_ANGLE,
					AUTO_CONTROL_MIN_STEERING, AUTO_CONTROL_MAX_STEERING));
}

inline static void __moving_rules(auto_control_t *auto_control) {
	uint16_t braking, speed;
	auto_control->EBP = CD_FALSE;
	braking = (uint16_t) roundf(auto_control->driving_commands->braking_module * AUTO_CONTROL_MAX_BRAKING);
	speed = (uint16_t) roundf(
			auto_control->driving_commands->throttling_module
					* AUTO_CONTROL_MAX_SPEED);

	if ((braking > AUTO_CONTROL_MIN_BRAKING)
			&& (speed > AUTO_CONTROL_MIN_SPEED)) {
		braking = AUTO_CONTROL_MAX_BRAKING;
		speed = AUTO_CONTROL_MIN_SPEED;
	}

	auto_control->braking = braking;
	auto_control->speed = speed;
}

inline static void __neutral_rules(auto_control_t *auto_control) {
	__basic_rules(auto_control);
	auto_control->EBP = CD_FALSE;
	auto_control->gear_shift = AUTO_CONTROL_GEAR_SHIFT_NEUTRAL;
	auto_control->braking = (uint16_t) roundf(auto_control->driving_commands->braking_module * AUTO_CONTROL_MAX_BRAKING);
	auto_control->speed = AUTO_CONTROL_MIN_SPEED;
}

inline static void __parking_rules(auto_control_t *auto_control) {
	__basic_rules(auto_control);
	auto_control->EBP = CD_TRUE;
	auto_control->gear_shift = AUTO_CONTROL_GEAR_SHIFT_DRIVE;
	auto_control->braking = AUTO_CONTROL_MAX_BRAKING;
	auto_control->speed = AUTO_CONTROL_MIN_SPEED;
}

inline static void __retro_rules(auto_control_t *auto_control) {
	__basic_rules(auto_control);
	__moving_rules(auto_control);
	auto_control->gear_shift = AUTO_CONTROL_GEAR_SHIFT_RETRO;
}

inline static void __drive_rules(auto_control_t *auto_control) {
	__basic_rules(auto_control);
	__moving_rules(auto_control);
	auto_control->gear_shift = AUTO_CONTROL_GEAR_SHIFT_DRIVE;
}

AutoControl_StatusTypeDef auto_control_step(auto_control_t *auto_control) {
	AutoControl_StatusTypeDef status = AUTO_CONTROL_ERROR;

	if (auto_control != NULL) {

		switch (auto_control->state) {
		case PARKING:
			__parking_rules(auto_control);
			auto_control->state = __update_auto_control_state_parking(auto_control);
			status = AUTO_CONTROL_OK;
			break;
		case RETRO:
			__retro_rules(auto_control);
			auto_control->state = __update_auto_control_state_retro(auto_control);
			status = AUTO_CONTROL_OK;
			break;
		case NEUTRAL:
			__neutral_rules(auto_control);
			auto_control->state = __update_auto_control_state_neutral(auto_control);
			status = AUTO_CONTROL_OK;
			break;
		case DRIVE:
			__drive_rules(auto_control);
			auto_control->state = __update_auto_control_state_drive(auto_control);
			status = AUTO_CONTROL_OK;
			break;
		default:
			break;

		}

		status = AUTO_CONTROL_OK;
	}

	return status;
}
