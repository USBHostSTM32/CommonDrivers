/**
 * @file urb_sender.c
 * @brief Source file for URB Sender module.
 *
 * This file contains the implementation of the functions for the URB Sender module,
 * which is responsible for managing USB requests and ensuring proper communication via USB.
 *
 * Created on: Jul 9, 2024
 * Authors: Alessio Guarini, Antonio Vitale
 */

#include <urb_sender.h>

/**
 * @brief Checks if the wheel is linked.
 *
 * This function checks if the T818 wheel is linked by verifying the USB HID state.
 *
 * @param[in] host_handle Pointer to the USB host handle.
 * @return True if the wheel is linked, false otherwise.
 */
static inline bool8u __check_wheel_is_linked(USBH_HandleTypeDef *host_handle) {
    bool8u wheel_linked = CD_FALSE;
    if (host_handle->pActiveClass != NULL) {
        const HID_HandleTypeDef *active_class = (HID_HandleTypeDef*) host_handle->pActiveClass->pData;
        if ((active_class->state == USBH_HID_POLL) || (active_class->state == USBH_HID_GET_DATA)) {
            wheel_linked = CD_TRUE;
        }
    }
    return wheel_linked;
}

URBSender_StatusTypeDef urb_sender_init(urb_sender_t *urb_sender, const urb_sender_config_t *config, osMessageQId xQueue) {
    URBSender_StatusTypeDef status = URB_SENDER_ERROR;
    if ((urb_sender != NULL) && (config != NULL) && (config->phost != NULL) && (xQueue != NULL)) {
        urb_sender->config = config;
        urb_sender->xQueue = xQueue;
        status = URB_SENDER_OK;
    }
    return status;
}

URBSender_StatusTypeDef urb_sender_enqueue_msg(urb_sender_t *urb_sender, const urb_interr_msg_t *interr_msg) {
    URBSender_StatusTypeDef status = URB_SENDER_ERROR;
    if (urb_sender != NULL) {
        if (xQueueSend(urb_sender->xQueue, interr_msg, 0U) == pdPASS) {
            status = URB_SENDER_OK;
        }
    }
    return status;
}

URBSender_StatusTypeDef urb_sender_dequeue_msg(urb_sender_t *urb_sender) {
    URBSender_StatusTypeDef status = URB_SENDER_ERROR;
    if (urb_sender != NULL) {
        if ((__check_wheel_is_linked(urb_sender->config->phost) == CD_TRUE) &&
            (uxQueueMessagesWaiting(urb_sender->xQueue) > 0U)) {
            urb_interr_msg_t *interr_buff = &urb_sender->interr_buff;
            if (xQueuePeek(urb_sender->xQueue, interr_buff, 0U) == pdPASS) {
                USBH_URBStateTypeDef urb_status = USBH_LL_GetURBState(urb_sender->config->phost, interr_buff->pipe_num);
                if ((urb_status == USBH_URB_DONE) || (urb_status == USBH_URB_IDLE)) {
                    (void)USBH_InterruptSendData(urb_sender->config->phost, interr_buff->msg, URB_MESSAGE_DIM, interr_buff->pipe_num);
                    if (xQueueReceive(urb_sender->xQueue, interr_buff, 0U) == pdPASS) {
                        status = URB_SENDER_OK;
                    }
                }
            }
        } else {
            status = URB_SENDER_OK;
        }
    }
    return status;
}
