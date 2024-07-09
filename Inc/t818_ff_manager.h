/**
 * @file t818_ff_manager.h
 * @brief Header file for the T818 force feedback manager.
 * 
 * This file contains the declarations of functions and macros used for initializing
 * and controlling the force feedback effects on the T818 servo base. The driver
 * communicates with the device over USB, sending interrupts packets
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
#include "urb_sender.h"

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
T818_FF_Manager_StatusTypeDef t818_ff_manager_init(urb_sender_t *urb_sender);

/**
 * @brief Sets the gain for the force feedback effects.
 * 
 * @param phost Pointer to the USB host handle.
 * @param value Gain value to be set.
 * @return T818_FF_Manager_StatusTypeDef Status of the operation.
 */
T818_FF_Manager_StatusTypeDef t818_ff_manager_set_gain(urb_sender_t *urb_sender, uint8_t value);

/**
 * @brief Uploads the spring effect to the device.
 * 
 * @param phost Pointer to the USB host handle.
 * @return T818_FF_Manager_StatusTypeDef Status of the operation.
 */
T818_FF_Manager_StatusTypeDef t818_ff_manager_upload_spring(urb_sender_t *urb_sender);

/**
 * @brief Uploads the constant force effect to the device.
 * 
 * @param phost Pointer to the USB host handle.
 * @param value Value of the constant force to be set.
 * @return T818_FF_Manager_StatusTypeDef Status of the operation.
 */
T818_FF_Manager_StatusTypeDef t818_ff_manager_upload_costant(urb_sender_t *urb_sender, int16_t value);

/**
 * @brief Plays the spring effect on the device.
 * 
 * @param phost Pointer to the USB host handle.
 * @return T818_FF_Manager_StatusTypeDef Status of the operation.
 */
T818_FF_Manager_StatusTypeDef t818_ff_manager_play_spring(urb_sender_t *urb_sender);

/**
 * @brief Plays the constant force effect on the device.
 * 
 * @param phost Pointer to the USB host handle.
 * @return T818_FF_Manager_StatusTypeDef Status of the operation.
 */
T818_FF_Manager_StatusTypeDef t818_ff_manager_play_costant(urb_sender_t *urb_sender);

/**
 * @brief Stops the spring effect on the device.
 * 
 * @param phost Pointer to the USB host handle.
 * @return T818_FF_Manager_StatusTypeDef Status of the operation.
 */
T818_FF_Manager_StatusTypeDef t818_ff_manager_stop_spring(urb_sender_t *urb_sender);

/**
 * @brief Stops the constant force effect on the device.
 * 
 * @param phost Pointer to the USB host handle.
 * @return T818_FF_Manager_StatusTypeDef Status of the operation.
 */
T818_FF_Manager_StatusTypeDef t818_ff_manager_stop_costant(urb_sender_t *urb_sender);

#endif /* INC_T818_FF_MANAGER_H_ */
