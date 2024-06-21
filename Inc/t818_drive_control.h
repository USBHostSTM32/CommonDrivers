#ifndef INC_T818_DRIVE_CONTROL_H_
#define INC_T818_DRIVE_CONTROL_H_


#include <usbh_hid_t818.h>

/* Type Definitions ---------------------------------------------------------*/
/**
 * @brief
 *
 *
 */
typedef uint8_t T818DriveControl_StatusTypeDef;

typedef struct {
	USBH_HandleTypeDef *t818_host_handle;
	USBH_HID_StateTypeDef *t818_hid_state;
	uint16_t wheel_range;
} t818_drive_control_config_t;

typedef struct {
	uint8_t initialized;
	float wheel_angle;
	//Aggiungere bottoni, accelerazione, frenata ecc ecc.
	const t818_drive_control_config_t * config;
} t818_drive_control_t;


/* Defines ------------------------------------------------------------------*/
/**
 * @brief Section for defines
 *
 * This section is reserved for macro definitions and constants related to
 * the VCU Pix module.
 */
#define T818_DC_OK							((T818DriveControl_StatusTypeDef) 0U)
#define T818_DC_ERROR						((T818DriveControl_StatusTypeDef) 1U)

T818DriveControl_StatusTypeDef t818_drive_control_init(t818_drive_control_t *t818_drive_control, const t818_drive_control_config_t *t818_config);


#endif /* INC_T818_DRIVE_CONTROL_H_ */
