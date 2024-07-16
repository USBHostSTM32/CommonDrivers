/**
 * @file dbw_kernel.c
 * @brief Source file for Drive-by-Wire (DBW) Kernel module.
 *
 * This file contains the implementation of the functions for the DBW Kernel module,
 * which is responsible for handling DBW Kernel states and related functionalities.
 *
 * Created on: Jun 20, 2024
 * Authors: Alessio Guarini, Antonio Vitale
 */

#include "dbw_kernel.h"
#include "string.h"

/* Static Configurations ----------------------------------------------------*/
static const urb_sender_config_t urb_sender_config = { .phost = &hUsbHostFS };

static const CAN_TxHeaderTypeDef auto_control_tx_header = {
    .StdId = 0x183,
    .ExtId = 0x0,  
    .IDE = CAN_ID_STD,        // Using standard ID
    .RTR = CAN_RTR_DATA,      // Frame type: data
    .DLC = 8,                 // Data length: 8 bytes
    .TransmitGlobalTime = DISABLE  // Timestamp disabled
};

/* Static Initialization of can_manager_config */
static const can_manager_config_t can_manager_config = { 
    .hcan = &hcan1, // Pointer to CAN1 handle
    .auto_control_tx_header = auto_control_tx_header, // Transmission header
    .auto_data_feedback_rx_fifo = CAN_RX_FIFO0,    // Reception FIFO
    .auto_data_feedback_rx_interrupt = CAN_IT_RX_FIFO0_MSG_PENDING 
};

static const t818_drive_control_config_t t818_config = { 
    .t818_host_handle = &hUsbHostFS 
};

/* Initialization of dbw_kernel_state */
static dbw_kernel_t dbw_kernel_state = {
    .urb_queueHandle = NULL,
    .urb_queueBuffer = {0},
    .urb_queueControlBlock = {0},
    .urb_sender = {0},
    .drive_control = {0},
    .auto_control = {0},
    .pid = {0},
    .rotation_manager = {0},
    .can_manager = {0}
};

/* Constant pointer to the dbw_kernel_t instance */
static dbw_kernel_t * const instance = &dbw_kernel_state;

/**
 * @brief Get the instance of DBW Kernel state.
 *
 * This function returns a constant pointer to the DBW Kernel state instance.
 *
 * @return dbw_kernel_t* const Pointer to the DBW Kernel state instance.
 */
dbw_kernel_t* const dbw_kernel_get_instance() {
    return instance;
}

/**
 * @brief Initialize the DBW Kernel module.
 *
 * This function initializes the DBW Kernel module and all its components.
 *
 * @return Status of the initialization.
 */
DBWKernel_StatusTypeDef dbw_kernel_init(void) {
    DBWKernel_StatusTypeDef status = DBW_ERROR;

    osMessageQStaticDef(urb_queue, 40, urb_interr_msg_t, instance->urb_queueBuffer,  &instance->urb_queueControlBlock);
	instance->urb_queueHandle = osMessageCreate(osMessageQ(urb_queue), NULL);
    
    // Initialize URB Sender
    if ((urb_sender_init(&instance->urb_sender, &urb_sender_config, instance->urb_queueHandle) == URB_SENDER_OK) &&
    	(pid_init(&instance->pid,PID_KP, PID_KI, PID_KD, PID_MAX_U, PID_MIN_U) == PID_OK) &&
        (t818_drive_control_init(&instance->drive_control, &t818_config, USBH_HID_T818GetInstance()) == T818_DC_OK) &&
		(auto_data_feedback_init(&instance->auto_data_feedback)== AUTO_DATA_FEEDBACK_OK) &&
        (auto_control_init(&instance->auto_control, &instance->drive_control.t818_driving_commands,&instance->auto_data_feedback) == AUTO_CONTROL_OK) &&
        (can_manager_init(&instance->can_manager, &can_manager_config) == CAN_MANAGER_OK) &&
        (rotation_manager_init(&instance->rotation_manager, &instance->pid, &instance->urb_sender) == ROTATION_MANAGER_OK)) {
        
        status = DBW_OK;
    }
        
    return status;
}

/**
 * @brief Perform a state update step for the DBW Kernel module.
 *
 * This function performs a single step in the state update process for the DBW Kernel.
 *
 * @return Status of the state update step.
 */
DBWKernel_StatusTypeDef dbw_kernel_update_state_step(void) {
    // Update URB Sender State
    DBWKernel_StatusTypeDef status = DBW_ERROR;
    return status;
}

/**
 * @brief Perform a URB transmission step for the DBW Kernel module.
 *
 * This function performs a single step in the URB transmission process for the DBW Kernel.
 *
 * @return Status of the URB transmission step.
 */
DBWKernel_StatusTypeDef dbw_kernel_urb_tx_step(void) {
    DBWKernel_StatusTypeDef status = DBW_ERROR;
    return status;
}
