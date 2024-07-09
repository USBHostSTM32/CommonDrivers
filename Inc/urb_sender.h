/*
 * urb_sender.h
 *
 *  Created on: Jul 9, 2024
 *      Author: Alessio Guarini, Antonio Vitale
 */

#ifndef INC_URB_SENDER_H_
#define INC_URB_SENDER_H_

//#include "FreeRTOS.h"
#include "common_drivers.h"
#include "cmsis_os.h"
#include "usbh_hid.h"
#include "usbh_ioreq.h"


typedef uint8_t URBSender_StatusTypeDef;

#define URB_MESSAGE_DIM							(64U)

/** @brief Macro indicating successful operation */
#define URB_SENDER_OK                       	((URBSender_StatusTypeDef) 0U)

/** @brief Macro indicating an error occurred */
#define URB_SENDER_ERROR                    	((URBSender_StatusTypeDef) 1U)

typedef struct {
	uint8_t msg[64];
	uint8_t pipe_num;
}urb_interr_msg_t;

typedef struct {
	USBH_HandleTypeDef *phost;
}urb_sender_config_t;

typedef struct {
	const urb_sender_config_t *config;
	osMessageQId xQueue;
	urb_interr_msg_t interr_buff;
}urb_sender_t;


URBSender_StatusTypeDef urb_sender_init(urb_sender_t *urb_sender, const urb_sender_config_t *config, osMessageQId xQueue);

URBSender_StatusTypeDef urb_sender_enqueue_msg(urb_sender_t *urb_sender, const urb_interr_msg_t *interr_msg);

URBSender_StatusTypeDef urb_sender_dequeue_msg(urb_sender_t *urb_sender);

#endif /* INC_URB_SENDER_H_ */
