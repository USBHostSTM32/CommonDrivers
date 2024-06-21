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

/* Type Definitions ---------------------------------------------------------*/
/**
 * @brief T818 Drive Control Status Type Definition
 *
 * This typedef defines the status type used for T818 Drive Control functions.
 * The status is represented as an 8-bit unsigned integer.
 */
typedef uint8_t T818DriveControl_StatusTypeDef;

/**
 * @brief T818 Drive Control Configuration Structure
 *
 * This structure contains the configuration parameters for the T818 Drive Control module.
 */
typedef struct {
    USBH_HandleTypeDef *t818_host_handle;   /**< Pointer to the USB host handle */
    USBH_HID_StateTypeDef *t818_hid_state;  /**< Pointer to the HID state structure */
    uint16_t wheel_range;                   /**< Wheel range value */
} t818_drive_control_config_t;

/**
 * @brief T818 Drive Control State Structure
 *
 * This structure represents the state of the T818 Drive Control module. It contains
 * information about the current initialization status, wheel angle, and configuration.
 */
typedef struct {
    uint8_t initialized;                                  /**< Initialization status */
    float wheel_angle;                                    /**< Current wheel angle */
    // Additional fields: buttons, acceleration, braking, etc.
    const t818_drive_control_config_t *config;            /**< Pointer to the configuration structure */
} t818_drive_control_t;

/* Defines ------------------------------------------------------------------*/
/**
 * @brief Section for defines
 *
 * This section is reserved for macro definitions and constants related to
 * the T818 Drive Control module.
 */

/**
 * @brief Macro indicating successful operation.
 */
#define T818_DC_OK                            ((T818DriveControl_StatusTypeDef) 0U)

/**
 * @brief Macro indicating an error occurred.
 */
#define T818_DC_ERROR                         ((T818DriveControl_StatusTypeDef) 1U)

/* Function Prototypes ------------------------------------------------------*/
/**
 * @brief Initialize the T818 Drive Control module.
 *
 * This function initializes the T818 Drive Control module, setting up the necessary
 * resources and configurations.
 *
 * @param t818_drive_control Pointer to the T818 drive control state structure.
 * @param t818_config Pointer to the T818 drive control configuration structure.
 * @return T818DriveControl_StatusTypeDef Status of the initialization.
 */
T818DriveControl_StatusTypeDef t818_drive_control_init(t818_drive_control_t *t818_drive_control, const t818_drive_control_config_t *t818_config);

#endif /* INC_T818_DRIVE_CONTROL_H_ */
