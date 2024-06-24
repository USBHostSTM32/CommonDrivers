/* Define to prevent recursive  ----------------------------------------------*/
#ifndef __USBH_HID_JOYSTICK_H
#define __USBH_HID_JOYSTICK_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbh_hid.h"

/** @addtogroup USBH_LIB
  * @{
  */

/** @addtogroup USBH_CLASS
  * @{
  */

/** @addtogroup USBH_HID_CLASS
  * @{
  */

/** @defgroup USBH_HID_T818
  * @brief This file is the Header file for usbh_hid_t818.c
  * @{
  */


/** @defgroup USBH_HID_T818_Exported_Types
  * @{
  */

//COSTANTI DA AGGIUNGERE
#define T818_ID_VENDOR					(1103U)
#define T818_ID_PRODUCT					(46742U)
#define T818_REPORT_SIZE				(64)

#define BUTTON_COUNT					((uint8_t)25U)

typedef enum {
	BUTTON_PADDLE_SHIFTER_LEFT,
	BUTTON_PADDLE_SHIFTER_RIGHT,
	BUTTON_DRINK,
	BUTTON_RADIO,
	BUTTON_ONE_PLUS,
	BUTTON_TEN_MINUS,
	BUTTON_SHA,
	BUTTON_OIL,
	BUTTON_PARKING,
	BUTTON_NEUTRAL,
	BUTTON_K1,
	BUTTON_K2,
	BUTTON_S1,
	BUTTON_LEFT_SIDE_WHEEL_UP,
	BUTTON_LEFT_SIDE_WHEEL_DOWN,
	BUTTON_RIGHT_SIDE_WHEEL_UP,
	BUTTON_RIGHT_SIDE_WHEEL_DOWN,
	BUTTON_GRIP_ANTICLOCKWISE,
	BUTTON_GRIP_CLOCKWISE,
	BUTTON_ENG_ANTICLOCKWISE,
	BUTTON_ENG_CLOCKWISE,
	BUTTON_22,
	BUTTON_23,
	BUTTON_GRIP,
	BUTTON_ENG,
} ButtonIndex;

typedef struct _HID_T818_Info {

	uint16_t wheel_rotation; //X axis
	uint16_t brake:10; // Y axis
	uint16_t throttle:10; // Rz axis
	uint16_t clutch:10; // Slider axis
	uint8_t vx_axis; // not mapped
	uint8_t vy_axis; // not mapped
	uint8_t rx_axis;
	uint8_t ry_axis;
	uint8_t z_axis; // not mapped

	uint8_t buttons[BUTTON_COUNT];

	uint8_t pad_arrow:4;
} HID_T818_Info_TypeDef;

/**
  * @}
  */

/** @defgroup USBH_HID_T818_Exported_Defines
  * @{
  */
/**
  * @}
  */

/** @defgroup USBH_HID_T818_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup USBH_HID_T818_Exported_Variables
  * @{
  */
/**
  * @}
  */

/** @defgroup USBH_HID_T818_Exported_FunctionsPrototype
  * @{
  */
//ADD DOXYGEN
extern USBH_StatusTypeDef USBH_HID_T818Init(USBH_HandleTypeDef *phost);
//ADD DOXYGEN
USBH_StatusTypeDef USBH_HID_GetT818Info(USBH_HandleTypeDef *phost);
//ADD DOXYGEN
HID_T818_Info_TypeDef* USBH_HID_T818GetInstance(void);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __USBH_HID_T818_H */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
