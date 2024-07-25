/**
 * @file can_manager.h
 * @brief CAN Manager Interface
 *
 * This file contains the interface for the CAN Manager module, which handles
 * the initialization and transmission of CAN messages in a Pix Moving's vehicle.
 *
 * To ensure robustness, the headers of the frames to be transmitted are preconfigured
 * and set as constants, along with other parameters in the `can_manager_config_t` structure.
 * This preconfiguration minimizes the risk of errors during runtime by ensuring that
 * critical transmission parameters remain consistent.
 *
 * Created on: Jun 26, 2024
 * Authors: Alessio Guarini, Antonio Vitale
 */

#ifndef INC_CAN_MANAGER_H_
#define INC_CAN_MANAGER_H_

#include <stdint.h>
#include "can.h"
#include "common_drivers.h"

/* Type Definitions ---------------------------------------------------------*/
/**
 * @brief Status type definition for CAN Manager.
 *
 * This type is used to represent the status of the CAN Manager operations.
 */
typedef uint8_t CanManager_StatusTypeDef;

/* Constant Definitions ---------------------------------------------------------*/
/**
 * @brief Size of the CAN manager transmission data buffer.
 */
#define CAN_MANAGER_TX_DATA_SIZE (8U)
/**
 * @brief Size of the CAN manager reception data buffer.
 */
#define CAN_MANAGER_RX_DATA_SIZE (8U)

/**
 * @brief Value meaning the mailbox is free.
 */
#define CAN_MANAGER_MESSAGE_NOT_PENDING (0U)


/* Data Structure Definitions -----------------------------------------------*/
/**
 * @brief Configuration structure for CAN Manager.
 *
 * This structure contains the configuration parameters required by the CAN Manager.
 */
typedef struct {
	CAN_HandleTypeDef *hcan; /**< Pointer to the CAN handle */
	const CAN_TxHeaderTypeDef auto_control_tx_header; /**< Header for auto control transmission */
	uint32_t auto_data_feedback_rx_fifo; /**< FIFO for auto data feedback reception */
	uint32_t auto_data_feedback_rx_interrupt; /**< Interrupt for auto data feedback reception */
} can_manager_config_t;

/**
 * @brief Structure representing a CAN Manager instance.
 *
 * This structure represents an instance of the CAN Manager.
 */
typedef struct {
    can_manager_config_t const *config;             /**< Pointer to the CAN Manager configuration */
    uint32_t auto_control_tx_mailbox;               /**< Mailbox for auto control transmission */
    CAN_RxHeaderTypeDef RxHeader;                   /**< CAN receive header */
    uint8_t tx_data[CAN_MANAGER_TX_DATA_SIZE];      /**< Transmission data buffer */
    uint8_t rx_data[CAN_MANAGER_RX_DATA_SIZE];      /**< Reception data buffer */
    uint32_t max_can_occupancy_cnt;                 /**< Maximum CAN occupancy count */
    uint32_t can_occupancy_cnt;                     /**< Current CAN occupancy count */
} can_manager_t;

/* Defines ------------------------------------------------------------------*/
/**
 * @brief Macro indicating successful operation.
 */
#define CAN_MANAGER_OK                                        ((CanManager_StatusTypeDef) 0U)

/**
 * @brief Macro indicating an error occurred.
 */
#define CAN_MANAGER_ERROR                                     ((CanManager_StatusTypeDef) 1U)

/**
 * @brief The maximum number of times a transmission can be aborted.
 *
 * The logic is that if attempting to transmit I find a pending transmission
 * (which I must therefore abort) n times in a row,
 * then I conclude that the CAN Bus is not working well.
 */
#define CAN_MANAGER_MAX_ABORTS                                (3U)

/**
 * @brief Auto Control CAN frame ID.
 */
#define AUTO_CONTROL_FRAME_ID                                 ((uint8_t)387U)

/* Function Prototypes ------------------------------------------------------*/
/**
 * @brief Initializes the CAN Manager.
 *
 * This function initializes the CAN Manager with the specified configuration.
 * It starts the CAN peripheral and activates the notification for the
 * auto data feedback reception.
 *
 * @param can_manager Pointer to the CAN Manager instance.
 * @param config Pointer to the configuration structure.
 * @return CAN_MANAGER_OK if initialization was successful, otherwise CAN_MANAGER_ERROR.
 */
CanManager_StatusTypeDef can_manager_init(can_manager_t *can_manager,
		const can_manager_config_t *config);

/**
 * @brief Transmits Auto Control data.
 *
 * This function transmits the Auto Control data using the CAN interface.
 * It checks if the mailbox is available for transmission, attempts to send
 * the data, and manages the number of transmission retries.
 *
 * @param can_manager Pointer to the CAN Manager instance.
 * @param can_data Pointer to the data to be transmitted.
 * @return CAN_MANAGER_OK if transmission was successful, otherwise CAN_MANAGER_ERROR.
 */
CanManager_StatusTypeDef can_manager_auto_control_tx(can_manager_t *can_manager,
		const uint8_t *can_data);

#endif /* INC_CAN_MANAGER_H_ */
