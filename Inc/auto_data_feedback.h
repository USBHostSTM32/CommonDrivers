/*
 * auto_data_feedback.h
 *
 *  Created on: Jul 14, 2024
 *      Author: vital
 */

#ifndef INC_AUTO_DATA_FEEDBACK_H_
#define INC_AUTO_DATA_FEEDBACK_H_

#include "common_drivers.h"

/* Status Type Definition ---------------------------------------------------*/
/** @brief Status type definition for Auto Control */
typedef uint8_t AutoDataFeedback_StatusTypeDef;

/** @brief Macro indicating successful operation */
#define AUTO_DATA_FEEDBACK_OK                       ((AutoDataFeedback_StatusTypeDef) 0U)

/** @brief Macro indicating an error occurred */
#define AUTO_DATA_FEEDBACK_ERROR                    ((AutoDataFeedback_StatusTypeDef) 1U)

/* Auto feedback Definitions -------------------------------------------------------*/
/**
 * @brief Minimum speed feedback value.
 */
#define AUTO_DATA_FEEDBACK_SPEED_MIN (-600)
/**
 * @brief Maximum speed feedback value.
 */
#define AUTO_DATA_FEEDBACK_SPEED_MAX (600)
/**
 * @brief Zero speed feedback value.
 */
#define AUTO_DATA_FEEDBACK_SPEED_ZERO (0)
/**
 * @brief Minimum steering feedback value.
 */
#define AUTO_DATA_FEEDBACK_STEER_MIN (-300)
/**
 * @brief Maximum steering feedback value.
 */
#define AUTO_DATA_FEEDBACK_STEER_MAX (300)
/**
 * @brief Zero steering feedback value.
 */
#define AUTO_DATA_FEEDBACK_STEER_ZERO (0)
/**
 * @brief Minimum braking feedback value.
 */
#define AUTO_DATA_FEEDBACK_BRAKING_MIN (0U)
/**
 * @brief Maximum braking feedback value.
 */
#define AUTO_DATA_FEEDBACK_BRAKING_MAX (1000U)
/**
 * @brief Gear park feedback value.
 */
#define AUTO_DATA_FEEDBACK_GEAR_PARK (2U)
/**
 * @brief Gear drive feedback value.
 */
#define AUTO_DATA_FEEDBACK_GEAR_DRIVE (1U)
/**
 * @brief Gear retro feedback value.
 */
#define AUTO_DATA_FEEDBACK_GEAR_RETRO (3U)
/**
 * @brief Gear neutral feedback value.
 */
#define AUTO_DATA_FEEDBACK_GEAR_NEUTRAL (2U)
/**
 * @brief Front and rear mode feedback value.
 */
#define AUTO_DATA_FEEDBACK_MODE_FRONT_AND_REAR (1U)
/**
 * @brief General mode feedback value.
 */
#define AUTO_DATA_FEEDBACK_MODE_GENERAL (1U)
/**
 * @brief Back and forth mode feedback value.
 */
#define AUTO_DATA_FEEDBACK_MODE_BACK_AND_FORTH (3U)
/**
 * @brief Normal vehicle status feedback value.
 */
#define AUTO_DATA_FEEDBACK_VEICHLE_STATUS_NORMAL (0U)
/**
 * @brief Abnormal vehicle status feedback value.
 */
#define AUTO_DATA_FEEDBACK_VEICHLE_STATUS_ABNORMAL (1U)
/**
 * @brief Remote control vehicle mode feedback value.
 */
#define AUTO_DATA_FEEDBACK_VEICHLE_MODE_REMOTE_CONTROL (0U)
/**
 * @brief Autonomous driving vehicle mode feedback value.
 */
#define AUTO_DATA_FEEDBACK_VEICHLE_MODE_AUTONOMOUS_DRIVING (1U)

/**
 * @brief Structure to hold auto feedback data.
 */
typedef struct {
	int16_t speed; /**< Speed feedback value */
	int16_t steer; /**< Steering feedback value */
	uint16_t braking; /**< Braking feedback value */
	uint8_t gear :2; /**< Gear feedback value */
	uint8_t mode :2; /**< Mode feedback value */
	uint8_t l_steer_light :1;/**< Left steering light status */
	uint8_t r_steer_light :1;/**< Right steering light status */
	uint8_t tail_light :1; /**< Tail light status */
	uint8_t braking_light :1;/**< Braking light status */
	uint8_t vehicle_status :1;/**< Vehicle status feedback value */
	uint8_t vehicle_mode :1; /**< Vehicle mode feedback value */
	uint8_t emergency_stop :1;/**< Emergency stop status */
} auto_data_feedback_t;

/**
 * @brief Initializes auto data feedback structure with default values.
 *
 * This function initializes the fields of the `auto_data_feedback_t` structure
 * with default values.
 *
 * @param auto_data_feedback Pointer to the auto data feedback structure to initialize.
 * @return AUTO_DATA_FEEDBACK_OK if initialization was successful, otherwise AUTO_DATA_FEEDBACK_ERROR.
 */
AutoDataFeedback_StatusTypeDef auto_data_feedback_init(
		auto_data_feedback_t *auto_data_feedback);

#endif /* INC_AUTO_DATA_FEEDBACK_H_ */
