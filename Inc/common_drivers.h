/**
 * @file common_drivers.h
 * @brief Header file for Common Driver Utilities.
 *
 * This file contains the definitions and prototypes for common utility functions
 * used across different modules, including mathematical operations and value transformations.
 *
 * Created on: Jun 25, 2024
 * Author: Alessio Guarini, Antonio Vitale
 */

#ifndef INC_COMMON_DRIVERS_H_
#define INC_COMMON_DRIVERS_H_

#include "stdint.h"

typedef uint8_t bool;

#define CD_TRUE							((bool) 1U)
#define CD_FALSE						((bool) 0U)

/**
 * Clamps a float value to a specified range.
 *
 * @param[in] x The value to clamp.
 * @param[in] min The minimum allowable value.
 * @param[in] max The maximum allowable value.
 * @return The clamped value.
 *
 * @note This function assumes that min is less than or equal to max.
 *       No validation is done inside the function to check this.
 */
float clamp_float(float x, float min, float max);

/**
 * Maps a float value from one range to another.
 *
 * @param[in] x The value to map.
 * @param[in] in_min The minimum value of the input range.
 * @param[in] in_max The maximum value of the input range.
 * @param[in] out_min The minimum value of the output range.
 * @param[in] out_max The maximum value of the output range.
 * @return The mapped value.
 *
 */
float map_value_float(float x, float in_min, float in_max, float out_min, float out_max);

#endif /* INC_COMMON_DRIVERS_H_ */
