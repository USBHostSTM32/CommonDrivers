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
	int16_t wheel_range;
} t818_drive_control_config_t;

typedef struct {
	float wheel_angle;
	//Aggiungere bottoni, accelerazione, frenata ecc ecc.
	t818_drive_control_config_t *config;
} t818_drive_control_t;

T818DriveControl_StatusTypeDef t818_drive_control_init(	t818_drive_control_t *t818_drive_control,
														const t818_drive_control_config_t *t818_config);


#endif /* INC_T818_DRIVE_CONTROL_H_ */
