/*
 * @file can_parser.c
 * @brief Implementation of the CAN Parser Interface
 *
 * This file provides the implementation of the functions declared in the
 * `can_parser.h` header file. It includes the logic for translating
 * logical values from the Auto Control module into a CAN frame payload.
 *
 * Created on: Jun 25, 2024
 * Authors: Alessio Guarini, Antonio Vitale
 */

#include "can_parser.h"

/**
 * @brief Fits bits into the specified position in the data array.
 *
 * This inline function inserts the specified value into the data array
 * at the specified position, applying the given shift and mask.
 *
 * @param data Pointer to the data array.
 * @param value The value to be inserted.
 * @param position The position in the data array.
 * @param shift The bit shift to be applied.
 * @param mask The bit mask to be applied.
 */
static inline void __fit_bits(uint8_t* data,uint8_t value,uint8_t position,uint8_t shift,uint8_t mask)
{
	data[position]&=(~(mask<<shift));
	data[position]|=value<<shift;
}

/**
 * @brief Fits bytes into the specified position in the data array.
 *
 * This inline function inserts the specified 16-bit value into the data array
 * at the specified position, splitting it into two bytes (little endian).
 *
 * @param data Pointer to the data array.
 * @param value The 16-bit value to be inserted.
 * @param position The position in the data array.
 */
static inline void __fit_bytes(uint8_t* data,uint16_t value,uint8_t position)
{
	data[position]=value;
	data[position+1]=value>>((uint16_t)8U);
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
