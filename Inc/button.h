/**
 * @file button.h
 * @brief Header file for Button Manager module.
 *
 * This file contains the type definitions and function prototypes for
 * the Button module, which handles button states and interactions.
 *
 * Created on: Jun 21, 2024
 * Author: Alessio Guarini, Antonio Vitale
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "stdint.h"
#include "stdio.h"
#include "stm32f7xx_hal.h"

/* Type Definitions ---------------------------------------------------------*/
/**
 * @brief Button Status Type Definition
 *
 * This typedef defines the status type used for Button functions.
 * The status is represented as an 8-bit unsigned integer.
 */
typedef uint8_t Button_StatusTypeDef;

/* Defines ------------------------------------------------------------------*/
/**
 * @brief Macro indicating successful operation.
 */
#define BUTTON_OK 										((Button_StatusTypeDef) 0U)

/**
 * @brief Macro indicating an error occurred.
 */
#define BUTTON_ERROR 									((Button_StatusTypeDef) 1U)

/**
 * @brief Macro indicating button pressed.
 */
#define BUTTON_PRESSED 									(1U)

/**
 * @brief Macro indicating button not pressed.
 */
#define BUTTON_NOT_PRESSED 								(0U)

/**
 * @brief 8-bit mask for button state.
 */
#define BUTTON_8BIT_MASK 								((uint8_t)0b00000001U)

/**
 * @brief Waiting time for long button press in milliseconds.
 */
#define BUTTON_LONG_PRESSING_WAITING_TIME 				(1000U)

/**
 * @brief Enumeration for long button press states.
 */
typedef enum {
    NOT_PRESSED, /**< Button is not pressed */
    PRESSING, /**< Button is being pressed */
    STATE_CHANGED /**< Button state has changed */
} button_long_pressed_enum;

/**
 * @brief Button structure definition.
 *
 * This structure represents the state and parameters of a button.
 */
typedef struct button_s button_t;

/**
 * @brief Function pointer type for button press functions.
 *
 * This typedef defines the function pointer type for checking if a button is pressed.
 */
typedef Button_StatusTypeDef (*is_pressed_func)(button_t *button);

/**
 * @brief Button structure.
 *
 * This structure contains the state information and function pointers for a button.
 */
struct button_s {
    is_pressed_func is_pressed; /**< Function pointer for checking if the button is pressed */
    uint8_t actual_raw_state :1; /**< Current raw state of the button */
    uint8_t previous_raw_state :1; /**< Previous raw state of the button */
    uint8_t state :1; /**< Current state of the button */
    button_long_pressed_enum long_pressed_state; /**< Long press state of the button */
    uint32_t start_pressing_time; /**< Time when the button was first pressed */
};

/* Function Prototypes ------------------------------------------------------*/

/**
 * @brief Checks if the button is long pressed.
 *
 * This function checks if the button has been pressed for a long duration.
 * It updates the button's state based on the duration it has been pressed.
 *
 * @param button Pointer to the button structure.
 * @return Button_StatusTypeDef Status of the operation.
 */
Button_StatusTypeDef button_is_pressed_long(button_t *button);


/**
 * @brief Checks if the button is pressed at a certain level.
 *
 * This function checks if the button has been pressed at a certain level,
 * and updates the button's state accordingly.
 *
 * @param button Pointer to the button structure.
 * @return Button_StatusTypeDef Status of the operation.
 */
Button_StatusTypeDef button_is_pressed_level(button_t *button);


/**
 * @brief Checks if the button is pressed at the edge.
 *
 * This function checks if the button has been pressed at the edge,
 * and updates the button's state accordingly.
 *
 * @param button Pointer to the button structure.
 * @return Button_StatusTypeDef Status of the operation.
 */
Button_StatusTypeDef button_is_pressed_edge(button_t *button);

/**
 * @brief Checks the base state of the button.
 *
 * This function updates the button's state to its actual raw state.
 *
 * @param button Pointer to the button structure.
 * @return Button_StatusTypeDef Status of the operation.
 */
Button_StatusTypeDef button_is_pressed_base(button_t *button);

/**
 * @brief Initializes the button.
 *
 * This function initializes the button structure and sets the initial state and function pointers.
 *
 * @param button Pointer to the button structure.
 * @param function Function pointer for checking if the button is pressed.
 * @return Button_StatusTypeDef Status of the initialization process.
 */
Button_StatusTypeDef button_init(button_t *button, is_pressed_func function);

/**
 * @brief Updates the button state with new raw state.
 *
 * This function updates the button's state with the new raw state
 * and calls the appropriate function to check the button's state.
 *
 * @param button Pointer to the button structure.
 * @param new_raw_state New raw state of the button.
 * @return Button_StatusTypeDef Status of the update process.
 */
Button_StatusTypeDef button_update(button_t *button, uint8_t new_raw_state);

/**
 * @brief Retrieves the current state of the button.
 *
 * @param button Pointer to the button structure.
 * @param state Pointer to store the current state of the button.
 * @return Button_StatusTypeDef Status of the retrieval process.
 */
inline Button_StatusTypeDef button_get_state(button_t *button, uint8_t *state) {
    Button_StatusTypeDef status = BUTTON_ERROR;
    if ((button != NULL) && (state != NULL)) {
        *state = button->state;
        status = BUTTON_OK;
    }
    return status;
}

#endif /* INC_BUTTON_H_ */