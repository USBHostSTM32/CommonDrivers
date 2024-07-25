/*
 * @file can_manager.c
 * @brief Implementation of the CAN Manager Interface
 *
 * This file provides the implementation of the functions declared in the
 * `can_manager.h` header file. It includes initialization and transmission
 * functionalities for CAN messages in a Pix Moving's vehicle.
 *
 * Created on: Jun 26, 2024
 * Authors: Alessio Guarini, Antonio Vitale
 */

#include "can_manager.h"

CanManager_StatusTypeDef can_manager_init(can_manager_t *can_manager, const can_manager_config_t *config) {
    CanManager_StatusTypeDef status = CAN_MANAGER_ERROR;
    if ((can_manager != NULL) && (config != NULL) && (config->hcan != NULL)) {
        can_manager->config = config;
        can_manager->auto_control_tx_mailbox = 0U;
        can_manager->can_occupancy_cnt = 0U;
        can_manager->max_can_occupancy_cnt = 0U;

        if ((memset(can_manager->tx_data, 0x00, CAN_MANAGER_TX_DATA_SIZE) == can_manager->tx_data) &&
        	(memset(can_manager->rx_data, 0x00, CAN_MANAGER_RX_DATA_SIZE) == can_manager->rx_data) &&
        	(HAL_CAN_Start(can_manager->config->hcan) == HAL_OK) &&
            (HAL_CAN_ActivateNotification(can_manager->config->hcan, can_manager->config->auto_data_feedback_rx_interrupt) == HAL_OK)) {
            status = CAN_MANAGER_OK;
        }
    }
    return status;
}

CanManager_StatusTypeDef __add_message_to_mailbox(can_manager_t *can_manager, const uint8_t *can_data)
{
    CanManager_StatusTypeDef status = CAN_MANAGER_ERROR;
    const can_manager_config_t *config = (const can_manager_config_t*) can_manager->config;
	uint32_t *pTxMailbox =  (uint32_t *) &can_manager->auto_control_tx_mailbox;

	if (HAL_CAN_AddTxMessage(config->hcan, &(config->auto_control_tx_header), can_data, pTxMailbox) == HAL_OK) {
		status = CAN_MANAGER_OK;
	}

	return status;
}

CanManager_StatusTypeDef __abort_message(can_manager_t *can_manager){

    CanManager_StatusTypeDef status = CAN_MANAGER_ERROR;
    uint8_t abort_complete = CD_FALSE;
    const can_manager_config_t *config = (const can_manager_config_t*) can_manager->config;

    HAL_CAN_AbortTxRequest(can_manager->config->hcan, can_manager->auto_control_tx_mailbox);

    static const uint32_t Timeout = 2U;
    uint32_t stop_time = HAL_GetTick() + Timeout;

    do {
        if (HAL_CAN_IsTxMessagePending(config->hcan, can_manager->auto_control_tx_mailbox) == CAN_MANAGER_MESSAGE_NOT_PENDING) {
            abort_complete = CD_TRUE;
        }
    } while ((abort_complete == CD_FALSE) && (HAL_GetTick() <= stop_time));

    if (abort_complete == CD_TRUE) {
        status = CAN_MANAGER_OK;
    }

    return status;
}

CanManager_StatusTypeDef __send_message(can_manager_t *can_manager, const uint8_t *can_data){
    CanManager_StatusTypeDef status = CAN_MANAGER_ERROR;
    const can_manager_config_t *config = (const can_manager_config_t*) can_manager->config;

    if (HAL_CAN_IsTxMessagePending(config->hcan, can_manager->auto_control_tx_mailbox) == CAN_MANAGER_MESSAGE_NOT_PENDING) {
    	status=__add_message_to_mailbox(can_manager, can_data);
    	can_manager->can_occupancy_cnt = 0U;
    }
    else
    {
    	(can_manager->can_occupancy_cnt)++;
    	if(can_manager->can_occupancy_cnt > can_manager->max_can_occupancy_cnt){
    		can_manager->max_can_occupancy_cnt = can_manager->can_occupancy_cnt;
    	}

    	if(__abort_message(can_manager) == CAN_MANAGER_OK)
    	{
    		status=__add_message_to_mailbox(can_manager, can_data);
    	}
    }
    return status;
}


CanManager_StatusTypeDef can_manager_auto_control_tx(can_manager_t *can_manager, const uint8_t *can_data) {
    CanManager_StatusTypeDef status = CAN_MANAGER_ERROR;
    if ((can_manager != NULL) && (can_data != NULL)) {
        status=__send_message(can_manager,can_data);
    }
    return status;
}

