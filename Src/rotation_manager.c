/*
 * rotation_manager.c
 *
 *  Created on: Jul 5, 2024
 *      Author: vital
 */

#include "rotation_manager.h"

Rotation_Manager_StatusTypeDef rotation_manager_init(
		rotation_manager_t *rotation_manager, pid_t *pid,
		urb_sender_t *urb_sender) {
	Rotation_Manager_StatusTypeDef status = ROTATION_MANAGER_ERROR;

	if ((rotation_manager != NULL) && (pid != NULL) && (urb_sender != NULL)) {
		rotation_manager->pid = pid;
		rotation_manager->urb_sender = urb_sender;
		status = ROTATION_MANAGER_OK;
	}

	return status;
}


Rotation_Manager_StatusTypeDef rotation_manager_update(
		rotation_manager_t *rotation_manager, int16_t auto_steer_feedback,
		int16_t auto_control_steer) {
	Rotation_Manager_StatusTypeDef status = ROTATION_MANAGER_ERROR;
	double u = 0.0;
	double e = 0.0;
	if ((rotation_manager != NULL)) {
		status = ROTATION_MANAGER_OK;
		e = (((double) auto_steer_feedback / 300.0) * 1024.0)
				- (double) auto_control_steer;

		if (pid_calculate_output(rotation_manager->pid, e, &u) == PID_ERROR) {
			status = ROTATION_MANAGER_ERROR;
		}

		int16_t input_force = (int16_t) (u*(-1.0));

		if ((t818_ff_manager_upload_costant(rotation_manager->urb_sender,
				input_force) == T818_FF_MANAGER_ERROR)
				|| (t818_ff_manager_play_costant(rotation_manager->urb_sender)
						== T818_FF_MANAGER_ERROR)) {
			status = ROTATION_MANAGER_ERROR;
		}

	}

	return status;
}

