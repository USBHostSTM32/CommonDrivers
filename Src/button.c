/*
 * button_manager.c
 *
 *  Created on: Jun 21, 2024
 *      Author: vital
 */

#include "button.h"

Button_StatusTypeDef button_is_pressed_long(button_t *button) {
	Button_StatusTypeDef status = BUTTON_ERROR;

	if (button != NULL) {
		switch (button->long_pressed_state) {
		case NOT_PRESSED:
			if (button->actual_raw_state == BUTTON_PRESSED) {
				button->start_pressing_time = HAL_GetTick();
				button->long_pressed_state = PRESSING;
			}
			status = BUTTON_OK;
			break;
		case PRESSING:
			if (button->actual_raw_state == BUTTON_NOT_PRESSED) {
				button->long_pressed_state = NOT_PRESSED;
			} else if ((HAL_GetTick()
					- button->start_pressing_time)>=BUTTON_LONG_PRESSING_WAITING_TIME) {
				button->state = ((!button->state) & BUTTON_8BIT_MASK);
				button->long_pressed_state = STATE_CHANGED;
			}
			else
			{
				button->long_pressed_state = PRESSING;
			}
			status = BUTTON_OK;
			break;
		case STATE_CHANGED:
			if (button->actual_raw_state == BUTTON_NOT_PRESSED) {
				button->long_pressed_state = NOT_PRESSED;
			}
			status = BUTTON_OK;
			break;
		default:
			break;
		}
	}

	return status;
}

Button_StatusTypeDef button_is_pressed_level(button_t *button) {
	Button_StatusTypeDef status = BUTTON_ERROR;

	if (button != NULL) {
		if ((button->actual_raw_state == BUTTON_PRESSED)
				&& (button->previous_raw_state == BUTTON_NOT_PRESSED)) {
			button->state = ((!button->state) & BUTTON_8BIT_MASK);
		}
		status = BUTTON_OK;
	}

	return status;
}

Button_StatusTypeDef button_is_pressed_edge(button_t *button) {
	Button_StatusTypeDef status = BUTTON_ERROR;

	if (button != NULL) {
		if ((button->actual_raw_state == BUTTON_PRESSED)
				&& (button->previous_raw_state == BUTTON_NOT_PRESSED)) {
			button->state = BUTTON_PRESSED;
		} else {
			button->state = BUTTON_NOT_PRESSED;
		}
		status = BUTTON_OK;
	}

	return status;
}

Button_StatusTypeDef button_is_pressed_base(button_t *button) {
	Button_StatusTypeDef status = BUTTON_ERROR;

	if (button != NULL) {
		button->state = button->actual_raw_state;
		status = BUTTON_OK;
	}

	return status;
}

Button_StatusTypeDef button_init(button_t *button, is_pressed_func function) {
	Button_StatusTypeDef status = BUTTON_ERROR;
	if ((button != NULL) && (function != NULL)) {
		button->is_pressed = function;
		button->actual_raw_state = BUTTON_NOT_PRESSED;
		button->previous_raw_state = BUTTON_NOT_PRESSED;
		button->start_pressing_time = 0U;
		button->state = BUTTON_NOT_PRESSED;
		button->long_pressed_state = NOT_PRESSED;
		status = BUTTON_OK;
	}
	return status;
}

Button_StatusTypeDef button_update(button_t *button, uint8_t new_raw_state) {
	Button_StatusTypeDef status = BUTTON_ERROR;
	if (button != NULL) {
		button->previous_raw_state = button->actual_raw_state;
		button->actual_raw_state = new_raw_state;
		button->is_pressed(button);
		status = BUTTON_OK;
	}
	return status;
}


