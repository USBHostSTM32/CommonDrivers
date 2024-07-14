/*
 * auto_data_feedback.c
 *
 *  Created on: Jul 14, 2024
 *      Author: vital
 */

#include "auto_data_feedback.h"


AutoDataFeedback_StatusTypeDef auto_data_feedback_init(
		auto_data_feedback_t *auto_data_feedback) {
	AutoDataFeedback_StatusTypeDef status = AUTO_DATA_FEEDBACK_ERROR;

	if (auto_data_feedback != NULL) {
		auto_data_feedback->speed = AUTO_DATA_FEEDBACK_SPEED_ZERO;
		auto_data_feedback->steer = AUTO_DATA_FEEDBACK_STEER_ZERO;
		auto_data_feedback->braking = AUTO_DATA_FEEDBACK_BRAKING_MIN;
		auto_data_feedback->gear = AUTO_DATA_FEEDBACK_GEAR_PARK;
		auto_data_feedback->mode = AUTO_DATA_FEEDBACK_MODE_FRONT_AND_REAR;
		auto_data_feedback->l_steer_light = CD_FALSE;
		auto_data_feedback->r_steer_light = CD_FALSE;
		auto_data_feedback->tail_light = CD_FALSE;
		auto_data_feedback->braking_light = CD_FALSE;
		auto_data_feedback->vehicle_status =
				AUTO_DATA_FEEDBACK_VEICHLE_STATUS_NORMAL;
		auto_data_feedback->vehicle_mode =
				AUTO_DATA_FEEDBACK_VEICHLE_MODE_AUTONOMOUS_DRIVING;
		auto_data_feedback->emergency_stop = CD_TRUE;
		status = AUTO_DATA_FEEDBACK_OK;
	}
	return status;
}
