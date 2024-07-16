/**
 * @file t818_drive_control.h
 * @brief Header file for T818 Drive Control module.
 *
 * This file contains the type definitions and function prototypes for
 * the T818 Drive Control module, which is responsible for handling T818
 * drive control states and related functionalities.
 *
 * Created on: Jun 20, 2024
 * Authors: Alessio Guarini, Antonio Vitale
 */

#ifndef INC_T818_DRIVE_CONTROL_H_
#define INC_T818_DRIVE_CONTROL_H_

#include <usbh_hid_t818.h>
#include "button.h"
#include "math.h"
#include "common_drivers.h"
#include "t818_ff_manager.h"
#include "rotation_manager.h"


/* Type Definitions ---------------------------------------------------------*/
/**
 * @brief T818 Drive Control Status Type Definition
 *
 * This typedef defines the status type used for T818 Drive Control functions.
 * The status is represented as an 8-bit unsigned integer.
 */
typedef uint8_t T818DriveControl_StatusTypeDef;

/* Enumeration Definitions -----------------------------------------------*/
/**
 * @brief T818 Drive Control Configuration Structure
 *
 * This structure contains the configuration parameters for the T818 Drive Control module.
 */
typedef struct {
    USBH_HandleTypeDef *t818_host_handle; /**< Pointer to the USB host handle */
} t818_drive_control_config_t;

/**
 * @brief Directional Pad Arrow Position Enumeration
 *
 * This enum defines the possible positions for the directional pad arrows.
 */
typedef enum {
    DIRECTION_NONE = 15,
    DIRECTION_UP = 0,
    DIRECTION_DOWN = 4,
    DIRECTION_LEFT = 6,
    DIRECTION_RIGHT = 2,
    DIRECTION_UP_LEFT = 7,
    DIRECTION_UP_RIGHT = 1,
    DIRECTION_DOWN_LEFT = 5,
    DIRECTION_DOWN_RIGHT = 3
} DirectionalPadArrowPosition;

/**
 * @brief Enumeration of drive control states.
 *
 * This enumeration defines the possible states for the drive control.
 */
typedef enum {
   WAITING_WHEEL_COFIGURATION,
   READING_WHEEL,
   AUTONOMOUS_DRIVING
} t818_drive_control_state;

/* Data Structure Definitions -----------------------------------------------*/
/**
 * @brief T818 Driving Commands Structure
 *
 * This structure contains the current state of the driving commands for the T818 Drive Control module.
 */
typedef struct {
    float wheel_steering_degree; /**< Current steering degree of the wheel */
    float braking_module; /**< Current braking module value */
    float throttling_module; /**< Current throttling module value */
    float clutching_module; /**< Current clutching module value */

    button_t buttons[BUTTON_COUNT]; /**< Array of button states */
    DirectionalPadArrowPosition pad_arrow_position; /**< Current position of the directional pad arrow */
} t818_driving_commands_t;

/**
 * @brief T818 Drive Control State Structure
 *
 * This structure represents the state of the T818 Drive Control module. It contains
 * information about the current initialization status, wheel angle, and configuration.
 */
typedef struct {
    HID_T818_Info_TypeDef *t818_info; /**< Pointer to the T818 info structure */
    const t818_drive_control_config_t *config; /**< Pointer to the configuration structure */
    t818_driving_commands_t t818_driving_commands; /**< Current driving commands */
    t818_drive_control_state state; /**< current drive control state */
} t818_drive_control_t;

/* Defines ------------------------------------------------------------------*/
/**
 * @brief Macro indicating successful operation.
 */
#define T818_DC_OK         			((T818DriveControl_StatusTypeDef) 0U)

/**
 * @brief Macro indicating an error occurred.
 */
#define T818_DC_ERROR     			 ((T818DriveControl_StatusTypeDef) 1U)

/**
 * @brief Throttling set point value.
 */
#define T818_THROTTLING_SET_POINT (0.0f)

/**
 * @brief Maximum steering angle value.
 */
#define T818_MAX_STEERING_ANGLE (30.0f)

/**
 * @brief Minimum steering angle value.
 */
#define T818_MIN_STEERING_ANGLE (-30.0f)


/* Function Prototypes ------------------------------------------------------*/
/**
 * @brief Initializes the T818 Drive Control module.
 *
 * @param t818_drive_control Pointer to the T818 Drive Control state structure.
 * @param t818_config Pointer to the T818 Drive Control configuration structure.
 * @param t818_info Pointer to the HID T818 info structure.
 * @return T818DriveControl_StatusTypeDef Status of the initialization process.
 */
T818DriveControl_StatusTypeDef t818_drive_control_init(
    t818_drive_control_t *t818_drive_control,
    const t818_drive_control_config_t *t818_config,
    HID_T818_Info_TypeDef *t818_info);

/**
 * @brief Executes a single step of the drive control logic.
 *
 * This function performs one iteration of the drive control logic for the provided
 * `t818_drive_control_t` instance.
 *
 * @param t818_drive_control Pointer to the drive control instance.
 * @return T818_DC_OK if the step was executed successfully, otherwise T818_DC_ERROR.
 */
T818DriveControl_StatusTypeDef t818_drive_control_step(t818_drive_control_t *t818_drive_control, urb_sender_t *urb_sender, rotation_manager_t* rotation_manager,int16_t steer_feedback);

#endif /* INC_T818_DRIVE_CONTROL_H_ */
