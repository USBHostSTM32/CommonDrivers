/*
 * can_manager.c
 *
 *  Created on: Jun 26, 2024
 *      Author: Alessio Guarini, Antonio Vitale
 */


#include "can_manager.h"


CanManager_StatusTypeDef can_manager_init(can_manager_t *can_manager, const can_manager_config_t* config){
	CanManager_StatusTypeDef status = CAN_MANAGER_ERROR;
	if((can_manager != NULL) && (config != NULL) &&(config->hcan != NULL)){
		can_manager->config = config;
		can_manager->n_aborts = 0U;
		status = CAN_MANAGER_OK;
	}
	return status;
}

CanManager_StatusTypeDef can_manager_auto_control_tx(can_manager_t *can_manager, const uint8_t *can_data){
	CanManager_StatusTypeDef status = CAN_MANAGER_ERROR;
	HAL_StatusTypeDef tx_status = HAL_OK;
	if((can_manager != NULL) && (can_data != NULL)){
		can_manager_config_t *config = (can_manager_config_t *)can_manager->config;
		if(HAL_CAN_IsTxMessagePending(config->hcan, config->auto_control_tx_mailbox)==0U){
			can_manager->n_aborts = 0U;
		} else {
			tx_status= HAL_CAN_AbortTxRequest(config->hcan, config->auto_control_tx_mailbox);
			can_manager->n_aborts++;
		}
		if(	(tx_status == HAL_OK) &&
			(can_manager->n_aborts < CAN_MANAGER_MAX_ABORTS)&&
			(HAL_CAN_AddTxMessage(config->hcan, &(config->auto_control_tx_header), can_data, &(config->auto_control_tx_mailbox)) == HAL_OK)){
			status = CAN_MANAGER_OK;
		}
	}
	return status;
}
