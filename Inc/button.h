/*
 * button_manager.h
 *
 *  Created on: Jun 21, 2024
 *      Author: vital
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

/* Type Definitions ---------------------------------------------------------*/
/**
 * @brief bUTTONl Status Type Definition
 *
 * This typedef defines the status type used for Button functions.
 * The status is represented as an 8-bit unsigned integer.
 */
#include "stdint.h"
#include "stdio.h"
#include "main.h"

typedef uint8_t Button_StatusTypeDef;

/* Defines ------------------------------------------------------------------*/
/**
 * @brief Section for defines
 *
 * This section is reserved for macro definitions and constants related to
 * the button module.
 */

/**
 * @brief Macro indicating successful operation.
 */
#define BUTTON_OK                            ((Button_StatusTypeDef) 0U)

/**
 * @brief Macro indicating an error occurred.
 */
#define BUTTON_ERROR                         ((Button_StatusTypeDef) 1U)

/**
 * @brief Macro indicating button pressed.
 */
#define BUTTON_PRESSED							(1U)

/**
 * @brief Macro indicating button not pressed.
 */
#define BUTTON_NOT_PRESSED						(0U)

#define BUTTON_8BIT_MASK						((uint8_t)0b00000001U)

#define BUTTON_LONG_PRESSING_WAITING_TIME		(1000U)


typedef enum {
	NOT_PRESSED, PRESSING, STATE_CHANGED
} button_long_pressed_enum;

typedef struct button_s button_t;

typedef Button_StatusTypeDef (*is_pressed_func)(button_t *button);

struct button_s {
	is_pressed_func is_pressed;
	uint8_t actual_raw_state :1;
	uint8_t previous_raw_state :1;
	uint8_t state :1;
	button_long_pressed_enum long_pressed_state;
	uint32_t start_pressing_time;
};



Button_StatusTypeDef button_is_pressed_long(button_t *button);
Button_StatusTypeDef button_is_pressed_level(button_t *button);
Button_StatusTypeDef button_is_pressed_edge(button_t *button);
Button_StatusTypeDef button_is_pressed_base(button_t *button);

Button_StatusTypeDef button_init(button_t *button, is_pressed_func function);

Button_StatusTypeDef button_update(button_t *button, uint8_t new_raw_state);

inline Button_StatusTypeDef button_get_state(button_t *button, uint8_t *state) {
	Button_StatusTypeDef status = BUTTON_ERROR;
	if ((button != NULL) && (state != NULL)) {
		*state = button->state;
		status = BUTTON_OK;
	}
	return status;
}

#endif /* INC_BUTTON_H_ */
