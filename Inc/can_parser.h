/*
 * @file can_parser.h
 * @brief CAN Parser Interface
 *
 * This file contains the interface for the CAN Parser module, which translates
 * logical values from the Auto Control module into a CAN frame payload.
 * It includes definitions, types, and function prototypes used in the CAN Parser
 * implementation.
 *
 * The CAN Parser module works in conjunction with the Auto Control module to
 * convert values from the `auto_control_data_t` structure into an array of 8
 * `uint8_t` elements, representing the payload of the CAN frame to be transmitted.
 *
 * Created on: Jun 25, 2024
 * Authors: Alessio Guarini, Antonio Vitale
 */

#ifndef INC_CAN_PARSER_H_
#define INC_CAN_PARSER_H_

#include "auto_control.h"
#include "string.h"

/* Status Type Definition ---------------------------------------------------*/
/** @brief Status type definition for CAN Parser */
typedef uint8_t CanParser_StatusTypeDef;

/** @brief Macro indicating successful operation */
#define CAN_PARSER_OK                        ((CanParser_StatusTypeDef) 0U)

/** @brief Macro indicating an error occurred */
#define CAN_PARSER_ERROR                     ((CanParser_StatusTypeDef) 1U)

/* Byte Position Definitions ------------------------------------------------*/
/** @brief Speed low byte position */
#define CAN_PARSER_SPEED_LOW_BYTE            (0U)
/** @brief Braking low byte position */
#define CAN_PARSER_BRAKING_LOW_BYTE          (2U)
/** @brief Steering low byte position */
#define CAN_PARSER_STEERING_LOW_BYTE         (4U)
/** @brief Gear shift byte position */
#define CAN_PARSER_GEAR_SHIFT_BYTE           (6U)
/** @brief Mode selection byte position */
#define CAN_PARSER_MODE_SELECTION_BYTE       (CAN_PARSER_GEAR_SHIFT_BYTE)
/** @brief Electronic Parking Brake byte position */
#define CAN_PARSER_EBP_BYTE                  (7U)
/** @brief Front light byte position */
#define CAN_PARSER_FRONT_LIGHT_BYTE          (CAN_PARSER_EBP_BYTE)
/** @brief Left light byte position */
#define CAN_PARSER_LEFT_LIGHT_BYTE           (CAN_PARSER_EBP_BYTE)
/** @brief Right light byte position */
#define CAN_PARSER_RIGHT_LIGHT_BYTE          (CAN_PARSER_EBP_BYTE)
/** @brief Speed mode byte position */
#define CAN_PARSER_SPEED_MODE_BYTE           (CAN_PARSER_EBP_BYTE)
/** @brief State control byte position */
#define CAN_PARSER_STATE_CONTROL_BYTE        (CAN_PARSER_EBP_BYTE)
/** @brief Advanced mode byte position */
#define CAN_PARSER_ADVANCED_MODE_BYTE        (CAN_PARSER_EBP_BYTE)
/** @brief Self-driving byte position */
#define CAN_PARSER_SELF_DRIVING_BYTE         (CAN_PARSER_EBP_BYTE)

/* Bit Shift Definitions ----------------------------------------------------*/
/** @brief Gear shift bit shift */
#define CAN_PARSER_GEAR_SHIFT_SHIFT          (0U)
/** @brief Mode selection bit shift */
#define CAN_PARSER_MODE_SELECTION_SHIFT      (4U)
/** @brief Electronic Parking Brake bit shift */
#define CAN_PARSER_EBP_SHIFT                 (3U)
/** @brief Front light bit shift */
#define CAN_PARSER_FRONT_LIGHT_SHIFT         (4U)
/** @brief Left light bit shift */
#define CAN_PARSER_LEFT_LIGHT_SHIFT          (0U)
/** @brief Right light bit shift */
#define CAN_PARSER_RIGHT_LIGHT_SHIFT         (2U)
/** @brief Speed mode bit shift */
#define CAN_PARSER_SPEED_MODE_SHIFT          (6U)
/** @brief State control bit shift */
#define CAN_PARSER_STATE_CONTROL_SHIFT       (1U)
/** @brief Advanced mode bit shift */
#define CAN_PARSER_ADVANCED_MODE_SHIFT       (5U)
/** @brief Self-driving bit shift */
#define CAN_PARSER_SELF_DRIVING_SHIFT        (7U)

/* Bit Mask Definitions -----------------------------------------------------*/
/** @brief Gear shift bit mask */
#define CAN_PARSER_GEAR_SHIFT_MASK           (0b00001111)
/** @brief Mode selection bit mask */
#define CAN_PARSER_MODE_SELECTION_MASK       (0b00001111)
/** @brief Electronic Parking Brake bit mask */
#define CAN_PARSER_EBP_MASK                  (0b00000001)
/** @brief Front light bit mask */
#define CAN_PARSER_FRONT_LIGHT_MASK          (0b00000001)
/** @brief Left light bit mask */
#define CAN_PARSER_LEFT_LIGHT_MASK           (0b00000001)
/** @brief Right light bit mask */
#define CAN_PARSER_RIGHT_LIGHT_MASK          (0b00000001)
/** @brief Speed mode bit mask */
#define CAN_PARSER_SPEED_MODE_MASK           (0b00000001)
/** @brief State control bit mask */
#define CAN_PARSER_STATE_CONTROL_MASK        (0b00000001)
/** @brief Advanced mode bit mask */
#define CAN_PARSER_ADVANCED_MODE_MASK        (0b00000001)
/** @brief Self-driving bit mask */
#define CAN_PARSER_SELF_DRIVING_MASK         (0b00000001)


/* Byte Position Definitions ------------------------------------------------*/
/** @brief Emergency stop feedback byte position */
#define CAN_PARSER_EMERGENCY_STOP_FEEDBACK_BYTE      (7U)
/** @brief Vehicle mode feedback byte position */
#define CAN_PARSER_VEHICLE_MODE_FEEDBACK_BYTE        (7U)
/** @brief Vehicle status feedback byte position */
#define CAN_PARSER_VEHICLE_STATUS_FEEDBACK_BYTE      (7U)
/** @brief Braking light feedback byte position */
#define CAN_PARSER_BRAKING_LIGHT_FEEDBACK_BYTE       (7U)
/** @brief Tail light feedback byte position */
#define CAN_PARSER_TAIL_LIGHT_FEEDBACK_BYTE          (7U)
/** @brief Right steer light feedback byte position */
#define CAN_PARSER_R_STEER_LIGHT_FEEDBACK_BYTE       (7U)
/** @brief Left steer light feedback byte position */
#define CAN_PARSER_L_STEER_LIGHT_FEEDBACK_BYTE       (7U)
/** @brief Mode feedback byte position */
#define CAN_PARSER_MODE_FEEDBACK_BYTE                (6U)
/** @brief Gear feedback byte position */
#define CAN_PARSER_GEAR_FEEDBACK_BYTE                (6U)
/** @brief Braking feedback byte position */
#define CAN_PARSER_BRAKING_FEEDBACK_BYTE             (4U)
/** @brief Front steer feedback byte position */
#define CAN_PARSER_STEER_FEEDBACK_BYTE             (2U)
/** @brief Speed feedback byte position */
#define CAN_PARSER_SPEED_FEEDBACK_BYTE               (0U)

/* Bit Shift Definitions ----------------------------------------------------*/
/** @brief Emergency stop feedback bit shift */
#define CAN_PARSER_EMERGENCY_STOP_FEEDBACK_SHIFT     (6U)
/** @brief Vehicle mode feedback bit shift */
#define CAN_PARSER_VEHICLE_MODE_FEEDBACK_SHIFT       (5U)
/** @brief Vehicle status feedback bit shift */
#define CAN_PARSER_VEHICLE_STATUS_FEEDBACK_SHIFT     (4U)
/** @brief Braking light feedback bit shift */
#define CAN_PARSER_BRAKING_LIGHT_FEEDBACK_SHIFT      (3U)
/** @brief Tail light feedback bit shift */
#define CAN_PARSER_TAIL_LIGHT_FEEDBACK_SHIFT         (2U)
/** @brief Right steer light feedback bit shift */
#define CAN_PARSER_R_STEER_LIGHT_FEEDBACK_SHIFT      (1U)
/** @brief Left steer light feedback bit shift */
#define CAN_PARSER_L_STEER_LIGHT_FEEDBACK_SHIFT      (0U)
/** @brief Mode feedback bit shift */
#define CAN_PARSER_MODE_FEEDBACK_SHIFT               (4U)
/** @brief Gear feedback bit shift */
#define CAN_PARSER_GEAR_FEEDBACK_SHIFT               (0U)

/* Bit Mask Definitions -----------------------------------------------------*/
/** @brief Emergency stop feedback bit mask */
#define CAN_PARSER_EMERGENCY_STOP_FEEDBACK_MASK      (0b01000000)
/** @brief Vehicle mode feedback bit mask */
#define CAN_PARSER_VEHICLE_MODE_FEEDBACK_MASK        (0b00110000)
/** @brief Vehicle status feedback bit mask */
#define CAN_PARSER_VEHICLE_STATUS_FEEDBACK_MASK      (0b00010000)
/** @brief Braking light feedback bit mask */
#define CAN_PARSER_BRAKING_LIGHT_FEEDBACK_MASK       (0b00001000)
/** @brief Tail light feedback bit mask */
#define CAN_PARSER_TAIL_LIGHT_FEEDBACK_MASK          (0b00000100)
/** @brief Right steer light feedback bit mask */
#define CAN_PARSER_R_STEER_LIGHT_FEEDBACK_MASK       (0b00000010)
/** @brief Left steer light feedback bit mask */
#define CAN_PARSER_L_STEER_LIGHT_FEEDBACK_MASK       (0b00000001)
/** @brief Mode feedback bit mask */
#define CAN_PARSER_MODE_FEEDBACK_MASK                (0b00110000)
/** @brief Gear feedback bit mask */
#define CAN_PARSER_GEAR_FEEDBACK_MASK                (0b00000011)

/* Function Prototypes ------------------------------------------------------*/
/**
 * @brief Converts Auto Control data to CAN frame payload.
 *
 * This function translates the logical values from the `auto_control_data_t`
 * structure into an array of 8 `uint8_t` elements, representing the payload
 * of the CAN frame to be transmitted.
 *
 * @param auto_control_data The Auto Control data to be converted.
 * @param data Pointer to the array where the CAN frame payload will be stored.
 * @return CAN_PARSER_OK if the conversion was successful, otherwise CAN_PARSER_ERROR.
 */
CanParser_StatusTypeDef can_parser_from_auto_control_to_array(auto_control_data_t auto_control_data, uint8_t* data);

/**
 * @brief Converts CAN frame payload to Auto Control Feedback data.
 *
 * This function translates an array of 8 `uint8_t` elements, representing
 * the payload of a received CAN frame, into logical values in the
 * `auto_data_feedback_t` structure.
 *
 * @param data Pointer to the array containing the CAN frame payload.
 * @param auto_data_feedback Pointer to the structure where the Auto Control Feedback data will be stored.
 * @return CAN_PARSER_OK if the conversion was successful, otherwise CAN_PARSER_ERROR.
 */
CanParser_StatusTypeDef can_parser_from_array_to_auto_control_feedback(uint8_t* data, auto_data_feedback_t *auto_data_feedback);

#endif /* INC_CAN_PARSER_H_ */
