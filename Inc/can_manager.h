/*
 * can_manager.h
 *
 *  Created on: Jun 26, 2024
 *      Author: Alessio Guarini, Antonio Vitale
 */

#ifndef INC_CAN_MANAGER_H_
#define INC_CAN_MANAGER_H_

#include <stdint.h>
#include "can.h"

/* Type Definitions ---------------------------------------------------------*/
/**
 * @brief Can Manager Status Type Definition
 *
 */
typedef uint8_t CanManager_StatusTypeDef;

/**
 * @brief Can Manager Configuration Structure
 *
 */
typedef struct {
	CAN_HandleTypeDef *hcan; /**< Pointer to the CAN handle */
	uint32_t auto_control_tx_mailbox;
	CAN_TxHeaderTypeDef auto_control_tx_header;
	uint32_t auto_data_feedback_rx_fifo;
}can_manager_config_t;

typedef struct {
	can_manager_config_t const *config;
	uint8_t n_aborts;
}can_manager_t;

/* Defines ------------------------------------------------------------------*/
/**
 * @brief Macro indicating successful operation.
 */
#define CAN_MANAGER_OK 										((CanManager_StatusTypeDef) 0U)

/**
 * @brief Macro indicating an error occurred.
 */
#define CAN_MANAGER_ERROR 									((CanManager_StatusTypeDef) 1U)

/**
 * @brief The maximum number of times a transmission can be aborted.
 *
 * The logic is that if attempting to transmit I find a pending transmission
 * (which I must therefore abort) n times in a row,
 * then I conclude that the CAN Bus is not working well.
 */
#define CAN_MANAGER_MAX_ABORTS								(3U)

/*
 * @brief Auto Control CAN frame ID
 */
#define AUTO_CONTROL_FRAME_ID 								((uint8_t)387U)

/* Function Prototypes ------------------------------------------------------*/

CanManager_StatusTypeDef can_manager_init(can_manager_t *can_manager, const can_manager_config_t* config);

CanManager_StatusTypeDef can_manager_auto_control_tx(can_manager_t *can_manager, const uint8_t *can_data);

#endif /* INC_CAN_MANAGER_H_ */
