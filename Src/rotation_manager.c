/*
 * rotation_manager.c
 *
 *  Created on: Jul 5, 2024
 *      Author: vital
 */

#include "rotation_manager.h"

Rotation_Manager_StatusTypeDef rotation_manager_init(
		rotation_manager_t *rotation_manager, pid_t *pid,
		USBH_HandleTypeDef *phost) {
	Rotation_Manager_StatusTypeDef status = ROTATION_MANAGER_ERROR;

	if ((rotation_manager != NULL) && (pid != NULL) && (phost != NULL)) {
		rotation_manager->pid = pid;
		rotation_manager->phost = phost;
		status = ROTATION_MANAGER_OK;
	}

	return status;
}

double u;
double e;

Rotation_Manager_StatusTypeDef rotation_manager_update(
		rotation_manager_t *rotation_manager, int16_t auto_steer_feedback,
		int16_t auto_control_steer) {
	Rotation_Manager_StatusTypeDef status = ROTATION_MANAGER_ERROR;

	if ((rotation_manager != NULL)) {
		status = ROTATION_MANAGER_OK;
		e = (((double) auto_steer_feedback / 300.0) * 1024.0)
				- (double) auto_control_steer;

		if (pid_calculate_output(rotation_manager->pid, e, &u) == PID_ERROR) {
			status = ROTATION_MANAGER_ERROR;
		}

		int16_t input_force = (int16_t) (u*(-1.0));

		if ((t818_ff_manager_upload_costant(rotation_manager->phost,
				input_force) == T818_FF_MANAGER_ERROR)
				|| (t818_ff_manager_play_costant(rotation_manager->phost)
						== T818_FF_MANAGER_ERROR)) {
			status = ROTATION_MANAGER_ERROR;
		}

	}

	return status;
}

