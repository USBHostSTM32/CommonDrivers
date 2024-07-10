#include "t818_drive_control.h"

typedef Button_StatusTypeDef (*button_init_func_t)(button_t*);

typedef struct {
	uint8_t index;
	button_init_func_t init_func;
} ButtonInitConfig;

static const ButtonInitConfig button_init_configs[BUTTON_COUNT] = {
		{ BUTTON_PADDLE_SHIFTER_LEFT, button_is_pressed_edge }, //USED
		{ BUTTON_PADDLE_SHIFTER_RIGHT, button_is_pressed_edge }, //USED
		{ BUTTON_DRINK, button_is_pressed_base }, { BUTTON_RADIO,
				button_is_pressed_base }, { BUTTON_ONE_PLUS,
				button_is_pressed_long }, { BUTTON_TEN_MINUS,
				button_is_pressed_long },
		{ BUTTON_SHA, button_is_pressed_level }, //USED
		{ BUTTON_OIL, button_is_pressed_long },
		{ BUTTON_PARKING, button_is_pressed_edge }, //USED
		{ BUTTON_NEUTRAL, button_is_pressed_edge }, //USED
		{ BUTTON_K1, button_is_pressed_level }, //USED
		{ BUTTON_K2, button_is_pressed_level }, //USED
		{ BUTTON_S1, button_is_pressed_edge }, { BUTTON_LEFT_SIDE_WHEEL_UP,
				button_is_pressed_edge }, { BUTTON_LEFT_SIDE_WHEEL_DOWN,
				button_is_pressed_edge }, { BUTTON_RIGHT_SIDE_WHEEL_UP,
				button_is_pressed_edge }, { BUTTON_RIGHT_SIDE_WHEEL_DOWN,
				button_is_pressed_base }, { BUTTON_GRIP_ANTICLOCKWISE,
				button_is_pressed_base }, { BUTTON_GRIP_CLOCKWISE,
				button_is_pressed_long }, { BUTTON_ENG_ANTICLOCKWISE,
				button_is_pressed_long }, { BUTTON_ENG_CLOCKWISE,
				button_is_pressed_level },
		{ BUTTON_22, button_is_pressed_level }, { BUTTON_23,
				button_is_pressed_edge },
		{ BUTTON_GRIP, button_is_pressed_edge }, { BUTTON_ENG,
				button_is_pressed_base } };

static T818DriveControl_StatusTypeDef t818_driving_commands_init(
		t818_driving_commands_t *t818_driving_commands) {
	T818DriveControl_StatusTypeDef status = T818_DC_ERROR;

	if (t818_driving_commands != NULL) {
		t818_driving_commands->wheel_steering_degree = 0.0f;
		t818_driving_commands->braking_module = 0.0f;
		t818_driving_commands->throttling_module = 0.0f;
		t818_driving_commands->clutching_module = 0.0f;

		Button_StatusTypeDef btn_status = BUTTON_OK;
		for (uint8_t i = 0; (i < BUTTON_COUNT) && (btn_status == BUTTON_OK);
				i++) {
			uint8_t index = button_init_configs[i].index;
			button_init_func_t init_func = button_init_configs[i].init_func;
			btn_status = button_init(&t818_driving_commands->buttons[index],
					init_func);
		}

		if (btn_status == BUTTON_OK) {
			t818_driving_commands->pad_arrow_position = DIRECTION_NONE;
			status = T818_DC_OK;
		}
	}

	return status;
}

T818DriveControl_StatusTypeDef t818_drive_control_init(
		t818_drive_control_t *t818_drive_control,
		const t818_drive_control_config_t *t818_config,
		HID_T818_Info_TypeDef *t818_info) {

	T818DriveControl_StatusTypeDef status = T818_DC_ERROR;

	if ((t818_drive_control != NULL) && (t818_config != NULL)
			&& (t818_info != NULL)) {
		t818_drive_control->state = WAITING_WHEEL_COFIGURATION;
		t818_drive_control->config = t818_config;
		t818_drive_control->t818_info = t818_info;
		if (t818_driving_commands_init(
				&t818_drive_control->t818_driving_commands) == T818_DC_OK) {
			status = T818_DC_OK;
		}
	}

	return status;
}

/**
 * @brief Converts raw wheel rotation to steering angle.
 *
 * This function converts the raw wheel rotation value to a steering angle
 * in the range of -30.0 to 30.0 degrees.
 *
 * @param[in] raw_wheel_rotation The raw wheel rotation value.
 * @return The steering angle in degrees.
 */
static inline float __convert_steering_angle(uint16_t raw_wheel_rotation) {
	return (60.0f / 65535.0f) * ((float) raw_wheel_rotation) - 30.0f;
}

/**
 * @brief Normalizes a value to a range between 0 and 1.
 *
 * This function normalizes the given value based on the provided maximum value.
 *
 * @param[in] value The value to be normalized.
 * @param[in] max_value The maximum possible value.
 * @return The normalized value.
 */
static inline float __normalize_value(uint16_t value, uint16_t max_value) {
	const float f_max_value = (float) max_value;
	float f_value = (float) value;

	if (f_value > f_max_value) {
		f_value = f_max_value;
	}
	return f_value / f_max_value;
}

/**
 * @brief Updates the T818 driving control commands.
 *
 * This function updates the driving commands for the T818 device based on the
 * current input values. It includes steering angle, braking, throttle, and clutch.
 *
 * @param[in,out] t818_drive_control Pointer to the T818 drive control structure.
 * @return Status of the drive control update.
 */
static inline T818DriveControl_StatusTypeDef __t818_drive_control_update(
		t818_drive_control_t *t818_drive_control) {
	T818DriveControl_StatusTypeDef status = T818_DC_ERROR;
	if (t818_drive_control != NULL) {
		__disable_irq();
		t818_drive_control->t818_driving_commands.wheel_steering_degree =
				__convert_steering_angle(
						t818_drive_control->t818_info->wheel_rotation);
		t818_drive_control->t818_driving_commands.braking_module = 1.0f
				- __normalize_value((t818_drive_control->t818_info->brake),
				T818_BRAKE_MAX);
		t818_drive_control->t818_driving_commands.throttling_module = 1.0f
				- __normalize_value((t818_drive_control->t818_info->throttle),
				T818_THROTTLE_MAX);
		t818_drive_control->t818_driving_commands.clutching_module = 1.0f
				- __normalize_value((t818_drive_control->t818_info->clutch),
				T818_CLUTCH_MAX);

		Button_StatusTypeDef btn_status = BUTTON_OK;
		for (uint8_t i = 0;
				(i < (uint8_t) BUTTON_COUNT) && (btn_status == BUTTON_OK);
				i++) {
			btn_status = button_update(
					&t818_drive_control->t818_driving_commands.buttons[i],
					t818_drive_control->t818_info->buttons[i]);
		}
		t818_drive_control->t818_driving_commands.pad_arrow_position =
				(DirectionalPadArrowPosition) t818_drive_control->t818_info->pad_arrow;
		__enable_irq();
		if (btn_status == BUTTON_OK) {
			status = T818_DC_OK;
		}
	}
	return status;
}

/**
 * @brief Checks if the wheel is linked.
 *
 * This function checks if the T818 wheel is linked by verifying the USB HID state.
 *
 * @param[in] t818_drive_control Pointer to the T818 drive control structure.
 * @return True if the wheel is linked, false otherwise.
 */
static inline bool8u __check_wheel_is_linked(
		t818_drive_control_t *t818_drive_control) {
	bool8u wheel_linked = CD_FALSE;

	if (t818_drive_control->config->t818_host_handle->pActiveClass != NULL) {
		const HID_HandleTypeDef *active_class =
				(HID_HandleTypeDef*) t818_drive_control->config->t818_host_handle->pActiveClass->pData;
		if ((active_class->state == USBH_HID_POLL)
				|| (active_class->state == USBH_HID_GET_DATA)) {
			wheel_linked = CD_TRUE;
		}
	}
	return wheel_linked;
}

/**
 * @brief Checks if the wheel is ready.
 *
 * This function checks if the T818 wheel is ready for use by ensuring all inputs
 * (brake, throttle, and clutch) are at their maximum values.
 *
 * @param[in] t818_drive_control Pointer to the T818 drive control structure.
 * @return True if the wheel is ready, false otherwise.
 */
static inline bool8u __check_wheel_is_ready(
		t818_drive_control_t *t818_drive_control) {
	uint8_t wheel_ready = CD_FALSE;

	if (__check_wheel_is_linked(t818_drive_control) == CD_TRUE) {
		if ((t818_drive_control->t818_info->brake == T818_BRAKE_MAX)
				&& (t818_drive_control->t818_info->throttle == T818_THROTTLE_MAX)
				&& (t818_drive_control->t818_info->clutch == T818_CLUTCH_MAX)) {
			wheel_ready = CD_TRUE;
		}
	}

	return wheel_ready;
}

T818DriveControl_StatusTypeDef t818_drive_control_step(
		t818_drive_control_t *t818_drive_control) {
	T818DriveControl_StatusTypeDef status = T818_DC_ERROR;
	if (t818_drive_control != NULL) {
		switch (t818_drive_control->state) {
		case WAITING_WHEEL_COFIGURATION:
			if (__check_wheel_is_ready(t818_drive_control) == CD_TRUE) {
				t818_drive_control->state = READING_WHEEL;
			} else {
				t818_drive_control->t818_driving_commands.braking_module =
						calculate_new_smoothed_value(
								t818_drive_control->t818_driving_commands.braking_module,
								T818_BRAKING_SET_POINT, T818_PEDAL_INCREMENT,
								T818_PEDAL_DECREMENT);
				t818_drive_control->t818_driving_commands.throttling_module = 0;
			}
			status = T818_DC_OK;
			break;
		case READING_WHEEL:
			if (__check_wheel_is_linked(t818_drive_control) == CD_TRUE) {
				status = __t818_drive_control_update(t818_drive_control);
			} else {
				t818_drive_control->state = WAITING_WHEEL_COFIGURATION;
				status = T818_DC_OK;
			}
			break;
		default:
			break;

		}

	}
	return status;
}
