/*
 * can_manager.c
 *
 *  Created on: Jun 26, 2024
 *      Author: Alessio Guarini, Antonio Vitale
 */


#include "can_manager.h"


CanManager_StatusTypeDef can_manager_init(can_manager_t *can_manager, CAN_HandleTypeDef *hcan){
	CanManager_StatusTypeDef status = CAN_MANAGER_ERROR;
	if((can_manager != NULL) && (hcan != NULL)){
		can_manager->hcan = hcan;
		status = CAN_MANAGER_OK;
	}
	return status;
}
