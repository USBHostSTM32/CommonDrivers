#include "t818_drive_control.h"

typedef Button_StatusTypeDef (*button_init_func_t)(button_t *);

typedef struct {
	uint8_t index;
	button_init_func_t init_func;
} ButtonInitConfig;

static const ButtonInitConfig button_init_configs[BUTTON_COUNT] = {
	{BUTTON_PADDLE_SHIFTER_LEFT, button_is_pressed_base},
	{BUTTON_PADDLE_SHIFTER_RIGHT, button_is_pressed_base},
	{BUTTON_DRINK, button_is_pressed_base},
	{BUTTON_RADIO, button_is_pressed_base},
	{BUTTON_ONE_PLUS, button_is_pressed_long},
	{BUTTON_TEN_MINUS, button_is_pressed_long},
	{BUTTON_SHA, button_is_pressed_long},
	{BUTTON_OIL, button_is_pressed_long},
	{BUTTON_PARKING, button_is_pressed_level},
	{BUTTON_NEUTRAL, button_is_pressed_level},
	{BUTTON_K1, button_is_pressed_level},
	{BUTTON_K2, button_is_pressed_level},
	{BUTTON_S1, button_is_pressed_edge},
	{BUTTON_LEFT_SIDE_WHEEL_UP, button_is_pressed_edge},
	{BUTTON_LEFT_SIDE_WHEEL_DOWN, button_is_pressed_edge},
	{BUTTON_RIGHT_SIDE_WHEEL_UP, button_is_pressed_edge},
	{BUTTON_RIGHT_SIDE_WHEEL_DOWN, button_is_pressed_base},
	{BUTTON_GRIP_ANTICLOCKWISE, button_is_pressed_base},
	{BUTTON_GRIP_CLOCKWISE, button_is_pressed_long},
	{BUTTON_ENG_ANTICLOCKWISE, button_is_pressed_long},
	{BUTTON_ENG_CLOCKWISE, button_is_pressed_level},
	{BUTTON_22, button_is_pressed_level},
	{BUTTON_23, button_is_pressed_edge},
	{BUTTON_GRIP, button_is_pressed_edge},
	{BUTTON_ENG, button_is_pressed_base}
};

static T818DriveControl_StatusTypeDef t818_driving_commands_init(t818_driving_commands_t *t818_driving_commands) {
	T818DriveControl_StatusTypeDef status = T818_DC_ERROR;

	if (t818_driving_commands != NULL) {
		t818_driving_commands->wheel_steering_degree = 0.0f;
		t818_driving_commands->braking_module = 0.0f;
		t818_driving_commands->throttling_module = 0.0f;
		t818_driving_commands->clutching_module = 0.0f;

		Button_StatusTypeDef btn_status = BUTTON_OK;
		for (uint8_t i = 0; (i < BUTTON_COUNT) && (btn_status == BUTTON_OK); i++) {
			uint8_t index = button_init_configs[i].index;
			button_init_func_t init_func = button_init_configs[i].init_func;
			btn_status = button_init(&t818_driving_commands->buttons[index], init_func);
		}

		if(btn_status == BUTTON_OK){
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

	if ((t818_drive_control != NULL) && (t818_config != NULL) && (t818_info != NULL)) {
		t818_drive_control->config = t818_config;
		t818_drive_control->t818_info = t818_info;
		if (t818_driving_commands_init(
				&t818_drive_control->t818_driving_commands) == T818_DC_OK) {
			status = T818_DC_OK;
		}
	}

	return status;
}

static inline float __convert_steering_angle(t818_drive_control_t *t818_drive_control)
{
	return (60.0f/65535.0f)*((float)t818_drive_control->t818_info->wheel_rotation)-30.0f;
}

static inline float __normalize_value(uint16_t value){
	return (1.0f/1023.0f)*((float)value);
}

T818DriveControl_StatusTypeDef t818_drive_control_update(t818_drive_control_t *t818_drive_control)
{
	T818DriveControl_StatusTypeDef status = T818_DC_ERROR;
	if (t818_drive_control != NULL) {
			if (USBH_HID_GetT818Info(t818_drive_control->config->t818_host_handle) == USBH_OK) {

				t818_drive_control->t818_driving_commands.wheel_steering_degree=__convert_steering_angle(t818_drive_control);
				t818_drive_control->t818_driving_commands.braking_module=__normalize_value(t818_drive_control->t818_info->brake);
				t818_drive_control->t818_driving_commands.throttling_module=__normalize_value(t818_drive_control->t818_info->throttle);
				t818_drive_control->t818_driving_commands.clutching_module=__normalize_value(t818_drive_control->t818_info->clutch);

				Button_StatusTypeDef btn_status = BUTTON_OK;
				for (uint8_t i = 0; (i < (uint8_t)BUTTON_COUNT) && (btn_status == BUTTON_OK); i++) {
					btn_status = button_update(&t818_drive_control->t818_driving_commands.buttons[i], t818_drive_control->t818_info->buttons[i]);
				}

				t818_drive_control->t818_driving_commands.pad_arrow_position=(DirectionalPadArrowPosition)t818_drive_control->t818_info->pad_arrow;

				if (btn_status == BUTTON_OK){
					status = T818_DC_OK;
				}
			}
		}
	return status;
}
