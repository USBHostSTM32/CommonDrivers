/*
 * AutoControl.h
 *
 *  Created on: Jun 24, 2024
 *      Author: vital
 */

#ifndef INC_AUTOCONTROL_H_
#define INC_AUTOCONTROL_H_

typedef enum {
	PARKING, RETRO, NEUTRAL, DRIVE
} auto_control_state;

typedef struct {
	button_t *right_light_button;
	button_t *left_light_button;
	button_t *front_light_button;
	button_t *gear_up_button;
	button_t *gear_down_button;
} auto_control_buttons_t;

typedef struct {
	uint8_t self_driving_enable :1;
	uint8_t advanced_mode :1;
	uint8_t state_control :1;
	uint8_t speed_mode :1;
	uint8_t right_light :1;
	uint8_t left_light :1;
	uint8_t front_light :1;
	uint8_t EBP :1;
	uint8_t mode_selection :4;
	uint8_t gear_shift :4;
	int16_t steering;
	uint16_t braking;
	uint16_t speed;

	auto_control_buttons_t *buttons;
	auto_control_state state;
} auto_control_t;

typedef uint8_t AutoControl_StatusTypeDef;

AutoControl_StatusTypeDef auto_control_buttons_init(button_t *right_light_button,
		button_t *left_light_button, button_t *front_light_button,
		button_t *gear_up_button, button_t *gear_down_button);

AutoControl_StatusTypeDef auto_control_init(auto_control_t *auto_control,
		auto_control_buttons_t *buttons);

AutoControl_StatusTypeDef auto_control_step(auto_control_t *auto_control);

//DEVI CAMBIARE METTENDO SOLO IL PUNTATORE A DRIVING COMMAND E FARE LE COSTANTI DEI BOTTONI COME HA DETTO ALE

#endif /* INC_AUTOCONTROL_H_ */
