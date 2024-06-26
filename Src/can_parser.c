/*
 * can_parser.c
 *
 *  Created on: Jun 25, 2024
 *      Author: vital
 */

#include "can_parser.h"

static inline void __fit_bits(uint8_t* data,uint8_t value,uint8_t position,uint8_t shift,uint8_t mask)
{
	data[position]&=(~(mask<<shift));
	data[position]|=value<<shift;
}

static inline void __fit_bytes(uint8_t* data,uint16_t value,uint8_t position)
{
	data[position]=value;
	data[position+1]=value>>8;
}

CanParser_StatusTypeDef can_parser_from_auto_control_to_array(auto_control_data_t auto_control_data,uint8_t* data)
{
	CanParser_StatusTypeDef status=CAN_PARSER_ERROR;

	if(data!=NULL)
	{
		__fit_bytes(data, auto_control_data.speed, CAN_PARSER_SPEED_LOW_BYTE);
		__fit_bytes(data, auto_control_data.braking, CAN_PARSER_BRAKING_LOW_BYTE);
		__fit_bytes(data, auto_control_data.steering, CAN_PARSER_STEERING_LOW_BYTE);

		__fit_bits(data, auto_control_data.gear_shift, CAN_PARSER_GEAR_SHIFT_BYTE, CAN_PARSER_GEAR_SHIFT_SHIFT, CAN_PARSER_GEAR_SHIFT_MASK);
		__fit_bits(data, auto_control_data.mode_selection, CAN_PARSER_MODE_SELECTION_BYTE, CAN_PARSER_MODE_SELECTION_SHIFT, CAN_PARSER_MODE_SELECTION_MASK);
		__fit_bits(data, auto_control_data.EBP, CAN_PARSER_EBP_BYTE, CAN_PARSER_EBP_SHIFT, CAN_PARSER_EBP_MASK);
		__fit_bits(data, auto_control_data.front_light, CAN_PARSER_FRONT_LIGHT_BYTE, CAN_PARSER_FRONT_LIGHT_SHIFT, CAN_PARSER_FRONT_LIGHT_MASK);
		__fit_bits(data, auto_control_data.left_light, CAN_PARSER_LEFT_LIGHT_BYTE, CAN_PARSER_LEFT_LIGHT_SHIFT, CAN_PARSER_LEFT_LIGHT_MASK);
		__fit_bits(data, auto_control_data.right_light, CAN_PARSER_RIGHT_LIGHT_BYTE, CAN_PARSER_RIGHT_LIGHT_SHIFT, CAN_PARSER_RIGHT_LIGHT_MASK);
		__fit_bits(data, auto_control_data.speed_mode, CAN_PARSER_SPEED_MODE_BYTE, CAN_PARSER_SPEED_MODE_SHIFT, CAN_PARSER_SPEED_MODE_MASK);
		__fit_bits(data, auto_control_data.state_control, CAN_PARSER_STATE_CONTROL_BYTE, CAN_PARSER_STATE_CONTROL_SHIFT, CAN_PARSER_STATE_CONTROL_MASK);
		__fit_bits(data, auto_control_data.advanced_mode, CAN_PARSER_ADVANCED_MODE_BYTE, CAN_PARSER_ADVANCED_MODE_SHIFT, CAN_PARSER_ADVANCED_MODE_MASK);
		__fit_bits(data, auto_control_data.self_driving, CAN_PARSER_SELF_DRIVING_BYTE, CAN_PARSER_SELF_DRIVING_SHIFT, CAN_PARSER_SELF_DRIVING_MASK);

		status=CAN_PARSER_OK;
	}

	return status;
}
