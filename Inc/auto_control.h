/*
 * @file auto_control.h
 * @brief Auto Control Interface
 *
 * This file contains the interface for the Auto Control module, which handles
 * the generation of logical values to be transmitted over the CAN bus.
 * It includes definitions, types, and function prototypes used in the Auto Control
 * implementation.
 *
 * The Auto Control module interfaces with the `t818_drive_control.h` module,
 * utilizing the `t818_driving_commands_t` structure to receive input commands
 * from the vehicle's control systems. The module manages the state machine
 * for the vehicle's gear states, including PARKING, RETRO, NEUTRAL, and DRIVE.
 * These states are updated based on the input commands and internal logic
 * to ensure correct vehicle behavior.
 *
 *
 * Created on: Jun 24, 2024
 * Authors: Alessio Guarini, Antonio Vitale
 */

#ifndef INC_AUTO_CONTROL_H_
#define INC_AUTO_CONTROL_H_

#include "common_drivers.h"
#include "t818_drive_control.h"
#include "auto_data_feedback.h"

/* Button Definitions -------------------------------------------------------*/
/** @brief Left light button definition */
#define AUTO_CONTROL_LEFT_LIGHT_BUTTON        (BUTTON_K1)
/** @brief Right light button definition */
#define AUTO_CONTROL_RIGHT_LIGHT_BUTTON       (BUTTON_K2)
/** @brief Front light button definition */
#define AUTO_CONTROL_FRONT_LIGHT_BUTTON       (BUTTON_SHA)
/** @brief Gear up button definition */
#define AUTO_CONTROL_GEAR_UP_BUTTON           (BUTTON_PADDLE_SHIFTER_RIGHT)
/** @brief Gear down button definition */
#define AUTO_CONTROL_GEAR_DOWN_BUTTON         (BUTTON_PADDLE_SHIFTER_LEFT)
/** @brief Neutral button definition */
#define AUTO_CONTROL_NEUTRAL_BUTTON           (BUTTON_NEUTRAL)
/** @brief Parking button definition */
#define AUTO_CONTROL_PARKING_BUTTON           (BUTTON_PARKING)

/* Mode Selection Definitions -----------------------------------------------*/
/** @brief Mode selection different definition */
#define AUTO_CONTROL_MODE_SELECTION_DIFFERENT (1U)
/** @brief Mode selection field definition */
#define AUTO_CONTROL_MODE_SELECTION_FIELD     (2U)
/** @brief Mode selection same definition */
#define AUTO_CONTROL_MODE_SELECTION_SAME      (3U)

/* Gear Shift Definitions ---------------------------------------------------*/
/** @brief Gear shift park definition */
#define AUTO_CONTROL_GEAR_SHIFT_PARK          (2U)
/** @brief Gear shift drive definition */
#define AUTO_CONTROL_GEAR_SHIFT_DRIVE         (1U)
/** @brief Gear shift retro definition */
#define AUTO_CONTROL_GEAR_SHIFT_RETRO         (3U)
/** @brief Gear shift neutral definition */
#define AUTO_CONTROL_GEAR_SHIFT_NEUTRAL       (2U)

/* Speed Definitions --------------------------------------------------------*/
/** @brief Maximum speed definition */
#define AUTO_CONTROL_MAX_SPEED                (1024U)
/** @brief Minimum speed definition */
#define AUTO_CONTROL_MIN_SPEED                (0U)
/** @brief Maximum speed increment */
#define AUTO_CONTROL_SPEED_MAX_INCREMENT	  (100.0F)
/** @brief Maximum speed decrement */
#define AUTO_CONTROL_SPEED_MAX_DECREMENT	  (100.0F)

/* Braking Definitions ------------------------------------------------------*/
/** @brief Maximum braking definition */
#define AUTO_CONTROL_MAX_BRAKING              (1024U)
/** @brief Minimum braking definition */
#define AUTO_CONTROL_MIN_BRAKING              (0U)

/* Steering Definitions -----------------------------------------------------*/
/** @brief Maximum steering definition */
#define AUTO_CONTROL_MAX_STEERING             (1024)
/** @brief Minimum steering definition */
#define AUTO_CONTROL_MIN_STEERING             (-1024)

/* Status Type Definition ---------------------------------------------------*/
/** @brief Status type definition for Auto Control */
typedef uint8_t AutoControl_StatusTypeDef;

/** @brief Macro indicating successful operation */
#define AUTO_CONTROL_OK                       ((AutoControl_StatusTypeDef) 0U)

/** @brief Macro indicating an error occurred */
#define AUTO_CONTROL_ERROR                    ((AutoControl_StatusTypeDef) 1U)

/* State Enumeration --------------------------------------------------------*/
/**
 * @brief Enumeration representing the Auto Control state
 */
typedef enum {
	PARKING, /**< Parking state */
	RETRO, /**< Reverse gear state */
	NEUTRAL, /**< Neutral gear state */
	DRIVE /**< Drive gear state */
} auto_control_state;

/* Data Structure Definitions -----------------------------------------------*/
/**
 * @brief Structure representing the Auto Control data
 *
 * This structure contains the data required by the Auto Control module to manage
 * the vehicle's control states and commands.
 */
typedef struct {
	uint16_t speed; /**< Speed command */
	uint16_t braking; /**< Braking command */
	int16_t steering; /**< Steering command */
	uint8_t gear_shift; /**< Gear shift command */
	uint8_t mode_selection; /**< Mode selection command */
	bool8u EBP; /**< Electronic Parking Brake status */
	bool8u front_light; /**< Front light status */
	bool8u left_light; /**< Left light status */
	bool8u right_light; /**< Right light status */
	bool8u speed_mode; /**< Speed mode status */
	bool8u state_control; /**< State control status */
	bool8u advanced_mode; /**< Advanced mode status */
	bool8u self_driving; /**< Self-driving mode status */
} auto_control_data_t;

/**
 * @brief Structure representing an Auto Control instance
 *
 * This structure represents an instance of the Auto Control module.
 */
typedef struct {
	auto_data_feedback_t *auto_data_feedback;
	auto_control_data_t auto_control_data; /**< Auto Control data */
	t818_driving_commands_t *driving_commands; /**< Pointer to driving commands */
	auto_control_state state; /**< Current state of the Auto Control */
} auto_control_t;

/* Function Prototypes ------------------------------------------------------*/
/**
 * @brief Initializes the Auto Control module
 *
 * This function initializes the Auto Control module with the specified driving commands.
 * It sets the initial state and parameters for the Auto Control.
 *
 * @param auto_control Pointer to the Auto Control instance.
 * @param driving_commands Pointer to the driving commands structure.
 * @return AUTO_CONTROL_OK if initialization was successful, otherwise AUTO_CONTROL_ERROR.
 */
AutoControl_StatusTypeDef auto_control_init(auto_control_t *auto_control,
		t818_driving_commands_t *driving_commands, auto_data_feedback_t *auto_data_feedback);

/**
 * @brief Performs a control step in the Auto Control module
 *
 * This function performs a control step, updating the state and control data
 * based on the current state and driving commands.
 *
 * @param auto_control Pointer to the Auto Control instance.
 * @return AUTO_CONTROL_OK if the step was successful, otherwise AUTO_CONTROL_ERROR.
 */
AutoControl_StatusTypeDef auto_control_step(auto_control_t *auto_control);

#endif /* INC_AUTO_CONTROL_H_ */
