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
        can_manager->n_tries = 0U;
        if ((HAL_CAN_Start(can_manager->config->hcan) == HAL_OK) &&
            (HAL_CAN_ActivateNotification(can_manager->config->hcan, can_manager->config->auto_data_feedback_rx_interrupt) == HAL_OK)) {
            status = CAN_MANAGER_OK;
        }
    }
    return status;
}

CanManager_StatusTypeDef can_manager_auto_control_tx(can_manager_t *can_manager, const uint8_t *can_data) {
    CanManager_StatusTypeDef status = CAN_MANAGER_ERROR;
    if ((can_manager != NULL) && (can_data != NULL)) {
        const can_manager_config_t *config = (const can_manager_config_t*) can_manager->config;
        if (HAL_CAN_IsTxMessagePending(config->hcan, can_manager->auto_control_tx_mailbox) == 0U) {
            can_manager->n_tries = 0U;
            uint32_t *pTxMailbox =  (uint32_t *) &can_manager->auto_control_tx_mailbox;
            if (HAL_CAN_AddTxMessage(config->hcan, &(config->auto_control_tx_header), can_data, pTxMailbox) == HAL_OK) {
                status = CAN_MANAGER_OK;
            }
        } else {
            can_manager->n_tries++;
            if (can_manager->n_tries < CAN_MANAGER_MAX_ABORTS) {
                status = CAN_MANAGER_OK;
            }
        }
    }
    return status;
}
