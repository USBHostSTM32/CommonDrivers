/*
 * t818_ff_manager.h
 *
 *  Created on: Jul 4, 2024
 *      Author: vital
 */

#ifndef INC_T818_FF_MANAGER_H_
#define INC_T818_FF_MANAGER_H_

#include "stdint.h"
#include "stdio.h"
#include "usbh_hid.h"
#include "common_drivers.h"

typedef uint8_t T818_FF_Manager_StatusTypeDef;

/* Defines ------------------------------------------------------------------*/
/**
 * @brief Macro indicating successful operation.
 */
#define T818_FF_MANAGER_OK 										((T818_FF_Manager_StatusTypeDef) 0U)
/**
 * @brief Macro indicating an error occurred.
 */
#define T818_FF_MANAGER_ERROR 									((T818_FF_Manager_StatusTypeDef) 1U)

#define T818_FF_MANAGER_FF_PIPE		0x03

#define T818_FF_MANAGER_MAX_DELAY	1000U

T818_FF_Manager_StatusTypeDef t818_ff_manager_init(USBH_HandleTypeDef *phost);

T818_FF_Manager_StatusTypeDef t818_ff_manager_set_gain(USBH_HandleTypeDef *phost,
		uint8_t value);

T818_FF_Manager_StatusTypeDef t818_ff_manager_upload_spring(USBH_HandleTypeDef *phost);

T818_FF_Manager_StatusTypeDef t818_ff_manager_upload_costant(USBH_HandleTypeDef *phost,
		int16_t value);

T818_FF_Manager_StatusTypeDef t818_ff_manager_play_spring(
		USBH_HandleTypeDef *phost);

T818_FF_Manager_StatusTypeDef t818_ff_manager_play_costant(
		USBH_HandleTypeDef *phost);

T818_FF_Manager_StatusTypeDef t818_ff_manager_stop_spring(
		USBH_HandleTypeDef *phost);

T818_FF_Manager_StatusTypeDef t818_ff_manager_stop_costant(
		USBH_HandleTypeDef *phost);

#endif /* INC_T818_FF_MANAGER_H_ */
