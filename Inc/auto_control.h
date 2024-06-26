/*
 * AutoControl.h
 *
 *  Created on: Jun 24, 2024
 *      Author: vital
 */

#ifndef INC_AUTO_CONTROL_H_
#define INC_AUTO_CONTROL_H_

#include "t818_drive_control.h"

#define AUTO_CONTROL_LEFT_LIGHT_BUTTON			(BUTTON_K1)
#define AUTO_CONTROL_RIGHT_LIGHT_BUTTON			(BUTTON_K2)
#define AUTO_CONTROL_FRONT_LIGHT_BUTTON			(BUTTON_SHA)
#define AUTO_CONTROL_GEAR_UP_BUTTON				(BUTTON_PADDLE_SHIFTER_RIGHT)
#define AUTO_CONTROL_GEAR_DOWN_BUTTON			(BUTTON_PADDLE_SHIFTER_LEFT)
#define AUTO_CONTROL_NEUTRAL_BUTTON				(BUTTON_NEUTRAL)
#define AUTO_CONTROL_PARKING_BUTTON				(BUTTON_PARKING)

#define AUTO_CONTROL_MODE_SELECTION_DIFFERENT	(1U)
#define AUTO_CONTROL_MODE_SELECTION_FIELD		(1U)
#define AUTO_CONTROL_MODE_SELECTION_SAME		(3U)

#define AUTO_CONTROL_GEAR_SHIFT_PARK			(1U)
#define AUTO_CONTROL_GEAR_SHIFT_DRIVE			(1U)
#define AUTO_CONTROL_GEAR_SHIFT_RETRO			(3U)
#define AUTO_CONTROL_GEAR_SHIFT_NEUTRAL			(2U)

#define AUTO_CONTROL_MAX_SPEED					(1024U)
#define AUTO_CONTROL_MIN_SPEED					(0U)

#define AUTO_CONTROL_MAX_BRAKING				(1024U)
#define AUTO_CONTROL_MIN_BRAKING				(0U)

#define AUTO_CONTROL_MAX_STEERING				(1024)
#define AUTO_CONTROL_MIN_STEERING				(-1024)

typedef uint8_t AutoControl_StatusTypeDef;

#define AUTO_CONTROL_OK         				((AutoControl_StatusTypeDef) 0U)

#define AUTO_CONTROL_ERROR     			 		((AutoControl_StatusTypeDef) 1U)


typedef enum {
	PARKING, RETRO, NEUTRAL, DRIVE
} auto_control_state;

typedef struct {
	bool self_driving :1;
	bool advanced_mode :1;
	bool state_control :1;
	bool speed_mode :1;
	bool right_light :1;
	bool left_light :1;
	bool front_light :1;
	bool EBP :1;
	uint8_t mode_selection :4;
	uint8_t gear_shift :4;
	int16_t steering;
	uint16_t braking;
	uint16_t speed;

	t818_driving_commands_t *driving_commands;
	auto_control_state state;
} auto_control_t;


AutoControl_StatusTypeDef auto_control_init(auto_control_t *auto_control,
		t818_driving_commands_t *driving_commands);

AutoControl_StatusTypeDef auto_control_step(auto_control_t *auto_control);

#endif /* INC_AUTO_CONTROL_H_ */
