/**
 * @file vcu_pix.c
 * @brief Source file for VCU Pix module.
 *
 * This file contains the implementation of the functions for the VCU Pix module,
 * which is responsible for handling VCU Pix states and related functionalities.
 *
 * Created on: Jun 20, 2024
 * Authors: Alessio Guarini, Antonio Vitale
 */

#include "vcu_pix.h"
#include "string.h"

/* Initialization of vcu_pix_state */
static vcu_pix_state_t vcu_pix_state = {
    .t818_control = NULL  						/**< t818 control pointer initialized to NULL */
};

/* Constant pointer to the vcu_pix_state_t instance */
static vcu_pix_state_t * const instance = &vcu_pix_state;

/**
 * @brief Get the instance of VCU Pix state.
 *
 * This function returns a constant pointer to the VCU Pix state instance.
 *
 * @return vcu_pix_state_t* const Pointer to the VCU Pix state instance.
 */
vcu_pix_state_t* const vcu_pix_state_get_instance() {
    return instance;
}
