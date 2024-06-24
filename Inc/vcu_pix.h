/**
 * @file vcu_pix.h
 * @brief Header file for VCU Pix module.
 *
 * This file contains the type definitions and function prototypes for
 * the VCU Pix module, which is responsible for handling VCU Pix states and
 * related functionalities.
 *
 * Created on: Jun 20, 2024
 * Authors: Alessio Guarini, Antonio Vitale
 */

#ifndef INC_VCU_PIX_H_
#define INC_VCU_PIX_H_

#include <stdint.h>
#include <t818_drive_control.h>

/* Type Definitions ---------------------------------------------------------*/
/**
 * @brief VCU Pix Status Type Definition
 *
 * This typedef defines the status type used for VCU Pix functions.
 * The status is represented as an 8-bit unsigned integer.
 */
typedef uint8_t VCUPix_StatusTypeDef;

/**
 * @brief VCU Pix State Structure
 *
 * This structure represents the state of the VCU Pix module. It contains
 * information about the current HID report from the T818 device.
 */
typedef struct {
	t818_driving_commands_t t818_driving_commands;
} vcu_pix_state_t;



/* Defines ------------------------------------------------------------------*/
/**
 * @brief Section for defines
 *
 * This section is reserved for macro definitions and constants related to
 * the VCU Pix module.
 */

/**
 * @brief Macro indicating successful operation.
 */
#define PIX_OK                              ((VCUPix_StatusTypeDef) 0U)

/**
 * @brief Macro indicating an error occurred.
 */
#define PIX_ERROR                           ((VCUPix_StatusTypeDef) 1U)

/* Function Prototypes ------------------------------------------------------*/
/**
 * @brief Section for function prototypes
 *
 * This section is reserved for the prototypes of the functions related to
 * the VCU Pix module.
 */

vcu_pix_state_t* const vcu_pix_state_get_instance();


#endif /* INC_VCU_PIX_H_ */
