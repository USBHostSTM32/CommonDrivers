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

/*
 * @brief Auto Control CAN frame ID
 */
#define AUTO_CONTRO_FRAME_ID 								((uint8_t)387U)

CanManager_StatusTypeDef can_manager_init(can_manager_t *can_manager, CAN_HandleTypeDef *hcan);

CanManager_StatusTypeDef can_manager_auto_control_tx(can_manager_t *can_manager, uint8_t *can_data);

#endif /* INC_CAN_MANAGER_H_ */
