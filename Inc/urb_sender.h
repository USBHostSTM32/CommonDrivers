/**
 * @file urb_sender.h
 * @brief Header file for URB Sender module.
 *
 * This file contains the type definitions and function prototypes for the URB Sender module,
 * which is responsible for managing USB requests and ensuring proper communication via USB.
 *
 * Created on: Jul 9, 2024
 * Authors: Alessio Guarini, Antonio Vitale
 */

#ifndef INC_URB_SENDER_H_
#define INC_URB_SENDER_H_

#include "common_drivers.h"
#include "cmsis_os.h"
#include "usbh_hid.h"
#include "usbh_ioreq.h"

/* Type Definitions ---------------------------------------------------------*/
/**
 * @brief URB Sender Status Type Definition
 *
 * This typedef defines the status type used for URB Sender functions.
 * The status is represented as an 8-bit unsigned integer.
 */
typedef uint8_t URBSender_StatusTypeDef;

/* Defines ------------------------------------------------------------------*/
#define URB_MESSAGE_DIM                         (64U)

/** @brief Macro indicating successful operation */
#define URB_SENDER_OK                           ((URBSender_StatusTypeDef) 0U)

/** @brief Macro indicating an error occurred */
#define URB_SENDER_ERROR                        ((URBSender_StatusTypeDef) 1U)

/**
 * @brief Structure representing an interrupt message for URB transmission.
 */
typedef struct {
    uint8_t msg[URB_MESSAGE_DIM]; /**< Message buffer */
    uint8_t pipe_num; /**< Pipe number for the USB transfer */
} urb_interr_msg_t;

/**
 * @brief Configuration structure for URB Sender.
 */
typedef struct {
    USBH_HandleTypeDef *phost; /**< Pointer to the USB host handle */
} urb_sender_config_t;

/**
 * @brief Structure representing the URB Sender.
 */
typedef struct {
    const urb_sender_config_t *config; /**< Pointer to the URB sender configuration */
    osMessageQId xQueue; /**< Handle to the message queue */
    urb_interr_msg_t interr_buff; /**< Interrupt buffer for message handling */
} urb_sender_t;

/* Function Prototypes ------------------------------------------------------*/
/**
 * @brief Initializes the URB Sender module.
 *
 * This function initializes the URB Sender module and configures it with the provided
 * settings.
 *
 * @param[in] urb_sender Pointer to the URB sender structure.
 * @param[in] config Pointer to the URB sender configuration structure.
 * @param[in] xQueue Handle to the message queue.
 * @return Status of the initialization.
 */
URBSender_StatusTypeDef urb_sender_init(urb_sender_t *urb_sender, const urb_sender_config_t *config, osMessageQId xQueue);

/**
 * @brief Enqueues a message to the URB sender.
 *
 * This function enqueues an interrupt message to the URB sender's message queue.
 *
 * @param[in] urb_sender Pointer to the URB sender structure.
 * @param[in] interr_msg Pointer to the interrupt message structure.
 * @return Status of the enqueue operation.
 */
URBSender_StatusTypeDef urb_sender_enqueue_msg(urb_sender_t *urb_sender, const urb_interr_msg_t *interr_msg);

/**
 * @brief Dequeues and processes a message from the URB sender.
 *
 * This function dequeues an interrupt message from the URB sender's message queue
 * and sends the data via USB if the wheel is linked and the USB transfer is idle.
 *
 * @param[in] urb_sender Pointer to the URB sender structure.
 * @return Status of the dequeue and send operation.
 */
URBSender_StatusTypeDef urb_sender_dequeue_msg(urb_sender_t *urb_sender);

#endif /* INC_URB_SENDER_H_ */
