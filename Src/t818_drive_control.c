#include <t818_drive_control.h>

T818DriveControl_StatusTypeDef t818_driving_commands_init(
		t818_driving_commands_t *t818_driving_commands) {
	T818DriveControl_StatusTypeDef status = T818_DC_ERROR;

	if (t818_driving_commands != NULL) {
		t818_driving_commands->wheel_steering_degree = 0.0f;
		t818_driving_commands->braking_module = 0.0f;
		t818_driving_commands->throttling_module = 0.0f;
		t818_driving_commands->clutching_module = 0.0f;

		if (button_init(&t818_driving_commands->paddle_shifter_left,
				button_is_pressed_base) == BUTTON_OK
				&& button_init(&t818_driving_commands->paddle_shifter_right,
						button_is_pressed_base) == BUTTON_OK
				&& button_init(&t818_driving_commands->drink,
						button_is_pressed_base) == BUTTON_OK
				&& button_init(&t818_driving_commands->radio,
						button_is_pressed_base) == BUTTON_OK
				&& button_init(&t818_driving_commands->one_plus,
						button_is_pressed_base) == BUTTON_OK
				&& button_init(&t818_driving_commands->ten_minus,
						button_is_pressed_base) == BUTTON_OK
				&& button_init(&t818_driving_commands->sha,
						button_is_pressed_base) == BUTTON_OK
				&& button_init(&t818_driving_commands->oil,
						button_is_pressed_base) == BUTTON_OK
				&& button_init(&t818_driving_commands->parking,
						button_is_pressed_base) == BUTTON_OK
				&& button_init(&t818_driving_commands->neutral,
						button_is_pressed_base) == BUTTON_OK
				&& button_init(&t818_driving_commands->k1,
						button_is_pressed_base) == BUTTON_OK
				&& button_init(&t818_driving_commands->k2,
						button_is_pressed_base) == BUTTON_OK
				&& button_init(&t818_driving_commands->s1,
						button_is_pressed_base) == BUTTON_OK
				&& button_init(&t818_driving_commands->left_side_wheel_up,
						button_is_pressed_base) == BUTTON_OK
				&& button_init(&t818_driving_commands->left_side_wheel_down,
						button_is_pressed_base) == BUTTON_OK
				&& button_init(&t818_driving_commands->right_side_wheel_up,
						button_is_pressed_base) == BUTTON_OK
				&& button_init(&t818_driving_commands->right_side_wheel_down,
						button_is_pressed_base) == BUTTON_OK
				&& button_init(&t818_driving_commands->grip_anticlockwise,
						button_is_pressed_base) == BUTTON_OK
				&& button_init(&t818_driving_commands->grip_clockwise,
						button_is_pressed_base) == BUTTON_OK
				&& button_init(&t818_driving_commands->eng_anticlockwise,
						button_is_pressed_base) == BUTTON_OK
				&& button_init(&t818_driving_commands->eng_clockwise,
						button_is_pressed_base) == BUTTON_OK
				&& button_init(&t818_driving_commands->button_22,
						button_is_pressed_base) == BUTTON_OK
				&& button_init(&t818_driving_commands->button_23,
						button_is_pressed_base) == BUTTON_OK
				&& button_init(&t818_driving_commands->grip,
						button_is_pressed_base) == BUTTON_OK
				&& button_init(&t818_driving_commands->eng,
						button_is_pressed_base) == BUTTON_OK) {
			status = T818_DC_OK;
		}

		t818_driving_commands->pad_arrow_position = DIRECTION_NONE;
	}

	return status;
}

T818DriveControl_StatusTypeDef t818_drive_control_init(
		t818_drive_control_t *t818_drive_control,
		const t818_drive_control_config_t *t818_config,
		HID_T818_Info_TypeDef *t818_info,
		t818_driving_commands_t *t818_driving_commands) {

	T818DriveControl_StatusTypeDef status = T818_DC_ERROR;

	if (t818_drive_control != NULL && t818_config != NULL && t818_info != NULL
			&& t818_driving_commands != NULL) {
		t818_drive_control->config = t818_config;
		t818_drive_control->t818_info = t818_info;
		t818_drive_control->t818_driving_commands = t818_driving_commands;
		status = T818_DC_OK;
	}

	return status;
}

//DA FINIRE
T818DriveControl_StatusTypeDef t818_drive_control_update(
		t818_drive_control_t *t818_drive_control) {

	T818DriveControl_StatusTypeDef status = T818_DC_ERROR;

	if (t818_drive_control != NULL) {
		if (USBH_HID_GetT818Info(t818_drive_control->config->t818_host_handle)
				== USBH_OK) {
			if (button_update(
					&t818_drive_control->t818_driving_commands->paddle_shifter_left,
					t818_drive_control->t818_info->paddle_shifter_left) == BUTTON_OK
					&& button_update(&t818_drive_control->t818_driving_commands->paddle_shifter_right,
							t818_drive_control->t818_info->paddle_shifter_right) == BUTTON_OK
					&& button_update(&t818_drive_control->t818_driving_commands->drink,
							t818_drive_control->t818_info->drink) == BUTTON_OK
					&& button_update(&t818_drive_control->t818_driving_commands->radio,
							t818_drive_control->t818_info->radio) == BUTTON_OK
					&& button_update(&t818_drive_control->t818_driving_commands->one_plus,
							t818_drive_control->t818_info->one_plus) == BUTTON_OK
					&& button_update(&t818_drive_control->t818_driving_commands->ten_minus,
							t818_drive_control->t818_info->ten_minus) == BUTTON_OK
					&& button_update(&t818_drive_control->t818_driving_commands->sha,
							t818_drive_control->t818_info->sha) == BUTTON_OK
					&& button_update(&t818_drive_control->t818_driving_commands->oil,
							t818_drive_control->t818_info->oil) == BUTTON_OK
					&& button_update(&t818_drive_control->t818_driving_commands->parking,
							t818_drive_control->t818_info->parking) == BUTTON_OK
					&& button_update(&t818_drive_control->t818_driving_commands->neutral,
							t818_drive_control->t818_info->neutral) == BUTTON_OK
					&& button_update(&t818_drive_control->t818_driving_commands->k1,
							t818_drive_control->t818_info->k1) == BUTTON_OK
					&& button_update(&t818_drive_control->t818_driving_commands->k2,
							t818_drive_control->t818_info->k2) == BUTTON_OK
					&& button_update(&t818_drive_control->t818_driving_commands->s1,
							t818_drive_control->t818_info->s1) == BUTTON_OK
					&& button_update(&t818_drive_control->t818_driving_commands->left_side_wheel_up,
							t818_drive_control->t818_info->left_side_wheel_up) == BUTTON_OK
					&& button_update(&t818_drive_control->t818_driving_commands->left_side_wheel_down,
							t818_drive_control->t818_info->left_side_wheel_down) == BUTTON_OK
					&& button_update(&t818_drive_control->t818_driving_commands->right_side_wheel_up,
							t818_drive_control->t818_info->right_side_wheel_up) == BUTTON_OK
					&& button_update(&t818_drive_control->t818_driving_commands->right_side_wheel_down,
							t818_drive_control->t818_info->right_side_wheel_down) == BUTTON_OK
					&& button_update(&t818_drive_control->t818_driving_commands->grip_anticlockwise,
							t818_drive_control->t818_info->grip_anticlockwise) == BUTTON_OK
					&& button_update(&t818_drive_control->t818_driving_commands->grip_clockwise,
							t818_drive_control->t818_info->grip_clockwise) == BUTTON_OK
					&& button_update(&t818_drive_control->t818_driving_commands->eng_anticlockwise,
							t818_drive_control->t818_info->eng_anticlockwise) == BUTTON_OK
					&& button_update(&t818_drive_control->t818_driving_commands->eng_clockwise,
							t818_drive_control->t818_info->eng_clockwise) == BUTTON_OK
					&& button_update(&t818_drive_control->t818_driving_commands->button_22,
							t818_drive_control->t818_info->button_22) == BUTTON_OK
					&& button_update(&t818_drive_control->t818_driving_commands->button_23,
							t818_drive_control->t818_info->button_23) == BUTTON_OK
					&& button_update(&t818_drive_control->t818_driving_commands->grip,
							t818_drive_control->t818_info->grip) == BUTTON_OK
					&& button_update(&t818_drive_control->t818_driving_commands->eng,
							t818_drive_control->t818_info->eng) == BUTTON_OK) {

				status = T818_DC_OK;
			}

		}
	}

	return status;
}

