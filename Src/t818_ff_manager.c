/**
 * @file t818_ff_manager.c
 * @brief Driver for managing force feedback on the T818 servo base via USB.
 *
 * This file contains the implementation of functions for initializing and controlling
 * the force feedback effects on the T818 servo base. The driver communicates with the
 * device over USB, sending configuration and control packets to manage effects like
 * spring, constant force, and gain.
 *
 * Created on: Jul 4, 2024
 * Author: Alessio Guarini, Antonio Vitale
 */

#include "t818_ff_manager.h"

/** @brief Index for the effect ID in the packet. */
#define ID_INDEX                                            (2U)
/** @brief ID for the spring effect. */
#define SPRING_ID                                           (0x01U)
/** @brief ID for the constant force effect. */
#define COSTANT_ID                                          (0x02U)
/** @brief Index for the gain value in the packet. */
#define GAIN_INDEX                                          (2U)
/** @brief Index for the low byte of the constant force value in the packet. */
#define COSTANT_LOW_VALUE_INDEX                             (4U)
/** @brief Index for the high byte of the constant force value in the packet. */
#define COSTANT_HI_VALUE_INDEX                              (5U)
/** @brief Delay for USB interrupt operations. */
#define T818_INTERRUPT_DELAY								(1U)
/** @brief Size of the packets sent to the device. */
#define PACKET_SIZE                                         (64U)
/** @brief Pipe index for force feedback management on the T818. */
#define FF_PIPE_INDEX       								(0x03)

/** @brief Configuration packet 1 for initializing the T818. */
static const uint8_t configuration_pack1[PACKET_SIZE] = {
    0x60, 0x01, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/** @brief Configuration packet 2 for initializing the T818. */
static const uint8_t configuration_pack2[PACKET_SIZE] = {
    0x60, 0x01, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/** @brief Base packet for setting the gain on the T818. */
static const uint8_t gain_base[PACKET_SIZE] = {
    0x60, 0x02, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/** @brief Packet for uploading the spring effect to the T818. */
static const uint8_t spring[PACKET_SIZE] = {
    0x60, 0x00, 0x01, 0x64, 0x66, 0x26, 0x66, 0x26,
    0xff, 0xfe, 0xff, 0xfe, 0xa6, 0x6a, 0xa6, 0x6a,
    0xfe, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xfe, 0xff,
    0xdf, 0x58, 0xa6, 0x6a, 0x06, 0x4f, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/** @brief Base packet for playing effects on the T818. */
static const uint8_t play_effect_base[PACKET_SIZE] = {
    0x60, 0x00, 0x01, 0x89, 0x41, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/** @brief Base packet for the constant force effect on the T818. */
static const uint8_t costant_base[PACKET_SIZE] = {
    0x60, 0x00, 0x01, 0x6A, 0xFF, 0xF0, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x4F, 0xFF, 0xFF,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/** @brief Packet for stopping the currently playing effect on the T818. */
static const uint8_t stop_effect_base[PACKET_SIZE] = {
    0x60, 0x00, 0x01, 0x89, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/** @brief Packet for setting the range on the T818. */
static const uint8_t set_range[PACKET_SIZE] = {
    0x60, 0x08, 0x11, 0x60, 0x54, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/**
 * @brief Sends an interrupt data packet to the device.
 *
 * @param phost Pointer to the USB host handle.
 * @param buff Pointer to the data buffer to be sent.
 * @param length Length of the data buffer.
 * @param pipe_index Index of the USB pipe to be used.
 * @param Timeout Timeout duration for the operation.
 * @return T818_FF_Manager_StatusTypeDef Status of the operation.
 */
static inline T818_FF_Manager_StatusTypeDef __send_interrupt_data(USBH_HandleTypeDef *phost,
    uint8_t *buff, uint8_t length, uint8_t pipe_index, uint32_t Timeout) {
    T818_FF_Manager_StatusTypeDef status = T818_FF_MANAGER_ERROR;
    uint32_t stop_time = HAL_GetTick() + Timeout;
    USBH_URBStateTypeDef urb_status = USBH_LL_GetURBState(phost, pipe_index);
    
    while ((urb_status != USBH_URB_DONE) && (urb_status != USBH_URB_IDLE) && (HAL_GetTick() < stop_time)){
        urb_status = USBH_LL_GetURBState(phost, pipe_index);
    }
   
    if ((urb_status == USBH_URB_DONE) || (urb_status == USBH_URB_IDLE)) {
        (void)USBH_InterruptSendData(phost, buff, length, pipe_index);
        urb_status = USBH_LL_GetURBState(phost, pipe_index);
        while ((urb_status != USBH_URB_DONE) && (urb_status != USBH_URB_IDLE) && (HAL_GetTick() < stop_time)){
            osDelay(T818_INTERRUPT_DELAY);
            urb_status = USBH_LL_GetURBState(phost, pipe_index);
        }
        if((urb_status == USBH_URB_DONE) || (urb_status == USBH_URB_IDLE)){
            status = T818_FF_MANAGER_OK;
        }
    }

    return status;
}

/**
 * @brief Sends a force feedback packet to the device.
 *
 * @param phost Pointer to the USB host handle.
 * @param buff Pointer to the data buffer to be sent.
 * @return T818_FF_Manager_StatusTypeDef Status of the operation.
 */
static inline T818_FF_Manager_StatusTypeDef __send_ff_packet(USBH_HandleTypeDef *phost, const uint8_t *buff) {
    return __send_interrupt_data(phost, (uint8_t *)buff, PACKET_SIZE, FF_PIPE_INDEX, T818_FF_MANAGER_MAX_DELAY);
}

T818_FF_Manager_StatusTypeDef t818_ff_manager_init(USBH_HandleTypeDef *phost) {
    T818_FF_Manager_StatusTypeDef status = T818_FF_MANAGER_ERROR;
    if ((phost != NULL) &&
        (__send_ff_packet(phost, configuration_pack1) == T818_FF_MANAGER_OK) &&
        (__send_ff_packet(phost, configuration_pack2) == T818_FF_MANAGER_OK) &&
        (__send_ff_packet(phost, set_range) == T818_FF_MANAGER_OK) &&
        (t818_ff_manager_set_gain(phost, 0xFF) == T818_FF_MANAGER_OK) &&
        (t818_ff_manager_upload_spring(phost) == T818_FF_MANAGER_OK) &&
        (t818_ff_manager_play_spring(phost) == T818_FF_MANAGER_OK)) {
        status = T818_FF_MANAGER_OK;
    }
    return status;
}

T818_FF_Manager_StatusTypeDef t818_ff_manager_set_gain(USBH_HandleTypeDef *phost, uint8_t value) {
    T818_FF_Manager_StatusTypeDef status = T818_FF_MANAGER_ERROR;
    if (phost != NULL) {
    	uint8_t tx_data[PACKET_SIZE];
       if(memcpy(tx_data, (uint8_t*) gain_base, PACKET_SIZE) == tx_data){
            tx_data[GAIN_INDEX] = value;
            status = __send_ff_packet(phost, tx_data);
       }
    }
    return status;
}

T818_FF_Manager_StatusTypeDef t818_ff_manager_upload_spring(USBH_HandleTypeDef *phost) {
	T818_FF_Manager_StatusTypeDef status = T818_FF_MANAGER_ERROR;
    if(phost != NULL){
    	status = __send_ff_packet(phost, (uint8_t*) spring);
    }
	return status;
}

T818_FF_Manager_StatusTypeDef t818_ff_manager_upload_costant(USBH_HandleTypeDef *phost, int16_t value) {
    T818_FF_Manager_StatusTypeDef status = T818_FF_MANAGER_ERROR;
    if (phost != NULL) {
        uint8_t tx_data[PACKET_SIZE];
        if (memcpy(tx_data, costant_base, PACKET_SIZE) == tx_data) {
            tx_data[ID_INDEX] = COSTANT_ID;
            tx_data[COSTANT_LOW_VALUE_INDEX] = value & 0x00FF;
            tx_data[COSTANT_HI_VALUE_INDEX] = (value >> 8) & (0x00FF);
            status = __send_ff_packet(phost, tx_data);
        }
    }
    return status;
}

T818_FF_Manager_StatusTypeDef t818_ff_manager_play_spring(USBH_HandleTypeDef *phost) {
    T818_FF_Manager_StatusTypeDef status = T818_FF_MANAGER_ERROR;
    if (phost != NULL) {
        uint8_t tx_data[PACKET_SIZE];
        if (memcpy(tx_data, play_effect_base, PACKET_SIZE) == tx_data) {
            tx_data[ID_INDEX] = SPRING_ID;
            status = __send_ff_packet(phost, tx_data);
        }
    }
    return status;
}

T818_FF_Manager_StatusTypeDef t818_ff_manager_play_costant(USBH_HandleTypeDef *phost) {
    T818_FF_Manager_StatusTypeDef status = T818_FF_MANAGER_ERROR;
    if (phost != NULL) {
        uint8_t tx_data[PACKET_SIZE];
        if (memcpy(tx_data, play_effect_base, PACKET_SIZE) == tx_data) {
            tx_data[ID_INDEX] = COSTANT_ID;
            status = __send_ff_packet(phost, tx_data);
        }
    }
    return status;
}

T818_FF_Manager_StatusTypeDef t818_ff_manager_stop_spring(USBH_HandleTypeDef *phost) {
    T818_FF_Manager_StatusTypeDef status = T818_FF_MANAGER_ERROR;
    if (phost != NULL) {
        uint8_t tx_data[PACKET_SIZE];
        if (memcpy(tx_data, stop_effect_base, PACKET_SIZE) == tx_data) {
            tx_data[ID_INDEX] = SPRING_ID;
            status = __send_ff_packet(phost, tx_data);
        }
    }
    return status;
}

T818_FF_Manager_StatusTypeDef t818_ff_manager_stop_costant(USBH_HandleTypeDef *phost) {
    T818_FF_Manager_StatusTypeDef status = T818_FF_MANAGER_ERROR;
    if (phost != NULL) {
        uint8_t tx_data[PACKET_SIZE];
        if (memcpy(tx_data, stop_effect_base, PACKET_SIZE) == tx_data) {
            tx_data[ID_INDEX] = COSTANT_ID;
            status = __send_ff_packet(phost, tx_data);
        }
    }
    return status;
}
