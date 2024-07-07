/*
 * rotation_manager.h
 *
 *  Created on: Jul 5, 2024
 *      Author: vital
 */

#ifndef INC_ROTATION_MANAGER_H_
#define INC_ROTATION_MANAGER_H_

#include <pid_regulator.h>
#include "t818_ff_manager.h"

typedef uint8_t Rotation_Manager_StatusTypeDef;

#define ROTATION_MANAGER_OK       	((Rotation_Manager_StatusTypeDef) 0U)
#define ROTATION_MANAGER_ERROR    	((Rotation_Manager_StatusTypeDef) 1U)

typedef struct {
	pid_t *pid;
	USBH_HandleTypeDef *phost;
} rotation_manager_t;

Rotation_Manager_StatusTypeDef rotation_manager_init(
		rotation_manager_t *rotation_manager, pid_t *pid,USBH_HandleTypeDef *phost);

Rotation_Manager_StatusTypeDef rotation_manager_update(
		rotation_manager_t *rotation_manager, int16_t auto_steer_feedback,
		int16_t auto_control_steer);

#endif /* INC_ROTATION_MANAGER_H_ */
