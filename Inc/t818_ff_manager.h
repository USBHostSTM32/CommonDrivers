/**
 * @file t818_ff_manager.h
 * @brief Header file for the T818 force feedback manager.
 * 
 * This file contains the declarations of functions and macros used for initializing
 * and controlling the force feedback effects on the T818 servo base. The driver
 * communicates with the device over USB, sending configuration and control packets
 * to manage effects like spring, constant force, and gain.
 * 
 * Created on: Jul 4, 2024
 * Author: Alessio Guarini, Antonio Vitale
 */

#ifndef INC_T818_FF_MANAGER_H_
#define INC_T818_FF_MANAGER_H_


#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#include "usbh_hid.h"
#include "usbh_def.h"
#include "common_drivers.h"

/** @brief Status type definition for the T818 force feedback manager. */
typedef uint8_t T818_FF_Manager_StatusTypeDef;

/* Defines ------------------------------------------------------------------*/
/** @brief Macro indicating successful operation. */
#define T818_FF_MANAGER_OK 										((T818_FF_Manager_StatusTypeDef) 0U)
/** @brief Macro indicating an error occurred. */
#define T818_FF_MANAGER_ERROR 									((T818_FF_Manager_StatusTypeDef) 1U)

/** @brief Maximum delay for T818 operations. */
#define T818_FF_MANAGER_MAX_DELAY	1000U

/**
 * @brief Initializes the force feedback manager.
 * 
 * @param phost Pointer to the USB host handle.
 * @return T818_FF_Manager_StatusTypeDef Status of the initialization.
 */
T818_FF_Manager_StatusTypeDef t818_ff_manager_init(USBH_HandleTypeDef *phost);

/**
 * @brief Sets the gain for the force feedback effects.
 * 
 * @param phost Pointer to the USB host handle.
 * @param value Gain value to be set.
 * @return T818_FF_Manager_StatusTypeDef Status of the operation.
 */
T818_FF_Manager_StatusTypeDef t818_ff_manager_set_gain(USBH_HandleTypeDef *phost, uint8_t value);

/**
 * @brief Uploads the spring effect to the device.
 * 
 * @param phost Pointer to the USB host handle.
 * @return T818_FF_Manager_StatusTypeDef Status of the operation.
 */
T818_FF_Manager_StatusTypeDef t818_ff_manager_upload_spring(USBH_HandleTypeDef *phost);

/**
 * @brief Uploads the constant force effect to the device.
 * 
 * @param phost Pointer to the USB host handle.
 * @param value Value of the constant force to be set.
 * @return T818_FF_Manager_StatusTypeDef Status of the operation.
 */
T818_FF_Manager_StatusTypeDef t818_ff_manager_upload_costant(USBH_HandleTypeDef *phost, int16_t value);

/**
 * @brief Plays the spring effect on the device.
 * 
 * @param phost Pointer to the USB host handle.
 * @return T818_FF_Manager_StatusTypeDef Status of the operation.
 */
T818_FF_Manager_StatusTypeDef t818_ff_manager_play_spring(USBH_HandleTypeDef *phost);

/**
 * @brief Plays the constant force effect on the device.
 * 
 * @param phost Pointer to the USB host handle.
 * @return T818_FF_Manager_StatusTypeDef Status of the operation.
 */
T818_FF_Manager_StatusTypeDef t818_ff_manager_play_costant(USBH_HandleTypeDef *phost);

/**
 * @brief Stops the spring effect on the device.
 * 
 * @param phost Pointer to the USB host handle.
 * @return T818_FF_Manager_StatusTypeDef Status of the operation.
 */
T818_FF_Manager_StatusTypeDef t818_ff_manager_stop_spring(USBH_HandleTypeDef *phost);

/**
 * @brief Stops the constant force effect on the device.
 * 
 * @param phost Pointer to the USB host handle.
 * @return T818_FF_Manager_StatusTypeDef Status of the operation.
 */
T818_FF_Manager_StatusTypeDef t818_ff_manager_stop_costant(USBH_HandleTypeDef *phost);

#endif /* INC_T818_FF_MANAGER_H_ */
