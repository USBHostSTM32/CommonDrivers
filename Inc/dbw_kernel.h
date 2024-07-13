/**
 * @file dbw_kernel.h
 * @brief Header file for Drive-by-Wire (DBW) Kernel module.
 *
 * This file contains the type definitions and function prototypes for
 * the DBW Kernel module, which is responsible for handling the core
 * functionalities of the Pix Moving vehicle's drive-by-wire system.
 * It integrates various modules to ensure the safe operation of the vehicle,
 * processing input data from the steering wheel via USB, applying force feedback
 * commands, and managing CAN bus communication.
 *
 * Created on: Jun 20, 2024
 * Authors: Alessio Guarini, Antonio Vitale
 */

#ifndef INC_DBW_KERNEL_H_
#define INC_DBW_KERNEL_H_

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include <stdint.h>
#include <common_drivers.h>
#include <t818_drive_control.h>
#include <auto_control.h>
#include <can_parser.h>
#include <can.h>
#include <can_manager.h>
#include <t818_ff_manager.h>
#include <rotation_manager.h>
#include <urb_sender.h>

/* Defines ------------------------------------------------------------------*/
#define UPDATE_STATE_PERIOD_MS                    (20U)
#define URB_TX_PERIOD_MS                          (2U)
#define USE_CAN

/* Type Definitions ---------------------------------------------------------*/
/**
 * @brief DBW Kernel Status Type Definition
 *
 * This typedef defines the status type used for DBW Kernel functions.
 * The status is represented as an 8-bit unsigned integer.
 */
typedef uint8_t DBWKernel_StatusTypeDef;

/* External Variables -------------------------------------------------------*/
extern USBH_HandleTypeDef hUsbHostFS;
/* Structure Definitions ----------------------------------------------------*/
/**
 * @brief DBW Kernel State Structure
 *
 * This structure represents the state of the DBW Kernel module. It contains
 * information about the current HID report from the T818 device.
 */
typedef struct {
    osMessageQId urb_queueHandle; /* Queue for USB Messages */
    uint8_t urb_queueBuffer[40 * sizeof(urb_interr_msg_t)]; /* Buffer for USB Messages */
    osStaticMessageQDef_t urb_queueControlBlock; /* Control block for USB Messages */
    urb_sender_t urb_sender; /* URB Sender instance */

    t818_drive_control_t drive_control;
    auto_control_t auto_control;
    pid_t pid;
    rotation_manager_t rotation_manager;
    can_manager_t can_manager;
} dbw_kernel_t;

/* Defines ------------------------------------------------------------------*/
/**
 * @brief Macro indicating successful operation.
 */
#define DBW_OK                              ((DBWKernel_StatusTypeDef) 0U)

/**
 * @brief Macro indicating an error occurred.
 */
#define DBW_ERROR                           ((DBWKernel_StatusTypeDef) 1U)

/* Function Prototypes ------------------------------------------------------*/
/**
 * @brief Retrieve the singleton instance of the DBW Kernel state.
 *
 * @return Pointer to the singleton instance of dbw_kernel_t.
 */
dbw_kernel_t* const dbw_kernel_get_instance();

/**
 * @brief Initialize the DBW Kernel module.
 *
 * This function initializes the DBW Kernel module and all its components.
 *
 * @return Status of the initialization.
 */
DBWKernel_StatusTypeDef dbw_kernel_init(void);

/**
 * @brief Perform a state update step for the DBW Kernel module.
 *
 * This function performs a single step in the state update process for the DBW Kernel.
 *
 * @return Status of the state update step.
 */
DBWKernel_StatusTypeDef dbw_kernel_update_state_step(void);

/**
 * @brief Perform a URB transmission step for the DBW Kernel module.
 *
 * This function performs a single step in the URB transmission process for the DBW Kernel.
 *
 * @return Status of the URB transmission step.
 */
DBWKernel_StatusTypeDef dbw_kernel_urb_tx_step(void);

#endif /* INC_DBW_KERNEL_H_ */