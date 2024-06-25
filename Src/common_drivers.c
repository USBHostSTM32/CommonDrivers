/**
 * @file common_drivers.c
 * @brief Implementation of Common Driver Utilities.
 *
 * This source file provides the implementation for common utility functions
 * defined in common_drivers.h.
 *
 * Created on: Jun 25, 2024
 * Author: Alessio Guarini, Antonio Vitale
 */

#include "common_drivers.h"

float clamp_float(float x, float min, float max){
    float ret_val;
    if (x < min) {
        ret_val = min;
    } else if (x > max) {
        ret_val = max;
    } else {
        ret_val = x;
    }
    return ret_val;
}


float map_value_float(float x, float in_min, float in_max, float out_min, float out_max){
    float ret_val;
    if (in_max == in_min) {
        ret_val = out_min;
    } else {
        ret_val = clamp_float(x, in_min, in_max);
        ret_val = (ret_val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }
    return ret_val;
}
