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
/** @defgroup USBH_HID_T818_Exported_Defines
  * @{
  */

#define T818_ID_VENDOR					        (1103U)
#define T818_ID_PRODUCT					        (46742U)
#define T818_REPORT_SIZE				        (64U)

#define T818_ID_VENDOR               	  (1103U)
#define T818_ID_PRODUCT              	  (46742U)
#define T818_REPORT_SIZE             	  (64U)

#define BUTTON_COUNT                 	  ((uint8_t)25U)

#define BUTTON_PADDLE_SHIFTER_LEFT    	((uint8_t)0U)
#define BUTTON_PADDLE_SHIFTER_RIGHT   	((uint8_t)1U)
#define BUTTON_DRINK                  	((uint8_t)2U)
#define BUTTON_RADIO                  	((uint8_t)3U)
#define BUTTON_ONE_PLUS               	((uint8_t)4U)
#define BUTTON_TEN_MINUS              	((uint8_t)5U)
#define BUTTON_SHA                    	((uint8_t)6U)
#define BUTTON_OIL                    	((uint8_t)7U)
#define BUTTON_PARKING                	((uint8_t)8U)
#define BUTTON_NEUTRAL                	((uint8_t)9U)
#define BUTTON_K1                     	((uint8_t)10U)
#define BUTTON_K2                     	((uint8_t)11U)
#define BUTTON_S1                     	((uint8_t)12U)
#define BUTTON_LEFT_SIDE_WHEEL_UP     	((uint8_t)13U)
#define BUTTON_LEFT_SIDE_WHEEL_DOWN   	((uint8_t)14U)
#define BUTTON_RIGHT_SIDE_WHEEL_UP    	((uint8_t)15U)
#define BUTTON_RIGHT_SIDE_WHEEL_DOWN  	((uint8_t)16U)
#define BUTTON_GRIP_ANTICLOCKWISE     	((uint8_t)17U)
#define BUTTON_GRIP_CLOCKWISE         	((uint8_t)18U)
#define BUTTON_ENG_ANTICLOCKWISE      	((uint8_t)19U)
#define BUTTON_ENG_CLOCKWISE          	((uint8_t)20U)
#define BUTTON_22                     	((uint8_t)21U)
#define BUTTON_23                     	((uint8_t)22U)
#define BUTTON_GRIP                   	((uint8_t)23U)
#define BUTTON_ENG                    	((uint8_t)24U)

/* Define constants for min and max values */
#define T818_WHEEL_ROTATION_MIN       	(0U)
#define T818_WHEEL_ROTATION_MAX       	(0xFFFFU)
#define T818_BRAKE_MIN                	(0U)
#define T818_BRAKE_MAX                	(1023U)
#define T818_THROTTLE_MIN             	(0U)
#define T818_THROTTLE_MAX             	(1023U)
#define T818_CLUTCH_MIN               	(0U)
#define T818_CLUTCH_MAX               	(1023U)
#define T818_VX_AXIS_MIN              	(0U)
#define T818_VX_AXIS_MAX              	(0xFFU)
#define T818_VY_AXIS_MIN              	(0U)
#define T818_VY_AXIS_MAX              	(0xFFU)
#define T818_RX_AXIS_MIN              	(0U)
#define T818_RX_AXIS_MAX              	(0xFFU)
#define T818_RY_AXIS_MIN              	(0U)
#define T818_RY_AXIS_MAX              	(0xFFU)
#define T818_Z_AXIS_MIN               	(0U)
#define T818_Z_AXIS_MAX               	(0xFFU)
#define T818_PAD_ARROW_MIN            	(0U)
#define T818_PAD_ARROW_MAX            	(0x0FU)


/** @defgroup USBH_HID_T818_Exported_Types
  * @{
  */
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

/**
  * @brief  Initialize the HID T818.
  * @param  phost: Host handle
  * @retval USBH Status
  */
extern USBH_StatusTypeDef USBH_HID_T818Init(USBH_HandleTypeDef *phost);

/**
  * @brief  Get the T818 HID Information.
  * @param  phost: Host handle
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_HID_GetT818Info(USBH_HandleTypeDef *phost);

/**
  * @brief  Get the instance of T818 HID Information.
  * @retval Pointer to HID_T818_Info_TypeDef structure
  */HID_T818_Info_TypeDef* USBH_HID_T818GetInstance(void);
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
