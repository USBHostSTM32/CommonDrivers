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
    .urb_queueBuffer = {0},  // Added extra braces for array initialization
    .urb_queueControlBlock = {{0}},  // Added extra braces for structure initialization
    .urb_sender = {
        .config = NULL,
        .xQueue = NULL,
        .interr_buff = {
            .msg = {0},  // Added extra braces for array initialization
            .pipe_num = 0
        }
    },
    .drive_control = {
        .t818_info = NULL,
        .config = NULL,
        .t818_driving_commands = {
            .wheel_steering_degree = 0.0f,
            .braking_module = 0.0f,
            .throttling_module = 0.0f,
            .clutching_module = 0.0f,
            .buttons = {{0}},  // Added extra braces for array initialization
            .pad_arrow_position = DIRECTION_NONE
        },
        .state = WAITING_WHEEL_COFIGURATION
    },
    .auto_control = {
        .auto_data_feedback = NULL,
        .auto_control_data = {
            .speed = 0,
            .braking = 0,
            .steering = 0,
            .gear_shift = 0,
            .mode_selection = 0,
            .EBP = CD_FALSE,
            .front_light = CD_FALSE,
            .left_light = CD_FALSE,
            .right_light = CD_FALSE,
            .speed_mode = CD_FALSE,
            .state_control = CD_FALSE,
            .advanced_mode = CD_FALSE,
            .self_driving = CD_FALSE
        },
        .driving_commands = NULL,
        .state = PARKING  // Assume AUTO_CONTROL_STATE_INITIAL is a valid state
    },
    .auto_data_feedback = {
        .speed = 0,
        .steer = 0,
        .braking = 0,
        .gear = 0,
        .mode = 0,
        .l_steer_light = 0,
        .r_steer_light = 0,
        .tail_light = 0,
        .braking_light = 0,
        .vehicle_status = 0,
        .vehicle_mode = 0,
        .emergency_stop = 0
    },
    .pid = {
        .ki = 0.0,
        .kp = 0.0,
        .kd = 0.0,
        .e_old = 0.0,
        .u_old = 0.0
    #ifdef USE_CLAMPING
        , .ukmax = 0.0,
        .ukmin = 0.0,
        .sk = 0.0
    #endif
    },
    .rotation_manager = {
        .pid = NULL,
        .urb_sender = NULL
    },
    .can_manager = {
        .config = NULL,
        .auto_control_tx_mailbox = 0,
        .RxHeader = {0},  // Added extra braces for structure initialization
        .tx_data = {0},  // Added extra braces for array initialization
        .rx_data = {0}
    }
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
    	(pid_init(&instance->pid,PID_KP, PID_KI, PID_KD, T818_FF_MANAGER_MIN_CONSTANT_VALUE, T818_FF_MANAGER_MAX_CONSTANT_VALUE) == PID_OK) &&
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
    DBWKernel_StatusTypeDef status = DBW_OK;

#ifdef USE_CAN
    if (can_parser_from_array_to_auto_control_feedback(dbw_kernel_state.can_manager.rx_data,
            dbw_kernel_state.auto_control.auto_data_feedback) != CAN_PARSER_OK) {
        status = DBW_ERROR;
    }
#endif

    if (status == DBW_OK) {
        if (t818_drive_control_step(&dbw_kernel_state.drive_control, &dbw_kernel_state.urb_sender, &dbw_kernel_state.rotation_manager, dbw_kernel_state.auto_data_feedback.steer) != T818_DC_OK) {
            status = DBW_ERROR;
        }
    }

    if (status == DBW_OK) {
        if (auto_control_step(&dbw_kernel_state.auto_control) != AUTO_CONTROL_OK) {
            status = DBW_ERROR;
        }
    }

#ifdef USE_CAN
    if (status == DBW_OK) {
        if (can_parser_from_auto_control_to_array(dbw_kernel_state.auto_control.auto_control_data, dbw_kernel_state.can_manager.tx_data) != CAN_PARSER_OK) {
            status = DBW_ERROR;
        }
    }

    if (status == DBW_OK) {
        if (can_manager_auto_control_tx(&dbw_kernel_state.can_manager, dbw_kernel_state.can_manager.tx_data) != CAN_MANAGER_OK) {
            status = DBW_ERROR;
        }
    }
#endif

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
    DBWKernel_StatusTypeDef status = DBW_OK;

    if (urb_sender_dequeue_msg(&dbw_kernel_state.urb_sender) != URB_SENDER_OK) {
        status = DBW_ERROR;
    }

    return status;
}
