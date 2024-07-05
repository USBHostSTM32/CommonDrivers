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

/* Constant Definitions -----------------------------------------------*/
/** @def T818_ID_VENDOR
 *  @brief Vendor ID for the T818 device.
 */
#define T818_ID_VENDOR (1103U)

/** @def T818_ID_PRODUCT
 *  @brief Product ID for the T818 device.
 */
#define T818_ID_PRODUCT (46742U)

/** @def T818_REPORT_SIZE
 *  @brief Report size for the T818 device.
 */
#define T818_REPORT_SIZE (64U)

/** @def BUTTON_COUNT
 *  @brief Number of buttons on the T818 device.
 */
#define BUTTON_COUNT ((uint8_t)25U)

/** @def BUTTON_PADDLE_SHIFTER_LEFT
 *  @brief Button ID for the left paddle shifter.
 */
#define BUTTON_PADDLE_SHIFTER_LEFT ((uint8_t)0U)

/** @def BUTTON_PADDLE_SHIFTER_RIGHT
 *  @brief Button ID for the right paddle shifter.
 */
#define BUTTON_PADDLE_SHIFTER_RIGHT ((uint8_t)1U)

/** @def BUTTON_DRINK
 *  @brief Button ID for the drink button.
 */
#define BUTTON_DRINK ((uint8_t)2U)

/** @def BUTTON_RADIO
 *  @brief Button ID for the radio button.
 */
#define BUTTON_RADIO ((uint8_t)3U)

/** @def BUTTON_ONE_PLUS
 *  @brief Button ID for the one plus button.
 */
#define BUTTON_ONE_PLUS ((uint8_t)4U)

/** @def BUTTON_TEN_MINUS
 *  @brief Button ID for the ten minus button.
 */
#define BUTTON_TEN_MINUS ((uint8_t)5U)

/** @def BUTTON_SHA
 *  @brief Button ID for the SHA button.
 */
#define BUTTON_SHA ((uint8_t)6U)

/** @def BUTTON_OIL
 *  @brief Button ID for the oil button.
 */
#define BUTTON_OIL ((uint8_t)7U)

/** @def BUTTON_PARKING
 *  @brief Button ID for the parking button.
 */
#define BUTTON_PARKING ((uint8_t)8U)

/** @def BUTTON_NEUTRAL
 *  @brief Button ID for the neutral button.
 */
#define BUTTON_NEUTRAL ((uint8_t)9U)

/** @def BUTTON_K1
 *  @brief Button ID for the K1 button.
 */
#define BUTTON_K1 ((uint8_t)10U)

/** @def BUTTON_K2
 *  @brief Button ID for the K2 button.
 */
#define BUTTON_K2 ((uint8_t)11U)

/** @def BUTTON_S1
 *  @brief Button ID for the S1 button.
 */
#define BUTTON_S1 ((uint8_t)12U)

/** @def BUTTON_LEFT_SIDE_WHEEL_UP
 *  @brief Button ID for the left side wheel up button.
 */
#define BUTTON_LEFT_SIDE_WHEEL_UP ((uint8_t)13U)

/** @def BUTTON_LEFT_SIDE_WHEEL_DOWN
 *  @brief Button ID for the left side wheel down button.
 */
#define BUTTON_LEFT_SIDE_WHEEL_DOWN ((uint8_t)14U)

/** @def BUTTON_RIGHT_SIDE_WHEEL_UP
 *  @brief Button ID for the right side wheel up button.
 */
#define BUTTON_RIGHT_SIDE_WHEEL_UP ((uint8_t)15U)

/** @def BUTTON_RIGHT_SIDE_WHEEL_DOWN
 *  @brief Button ID for the right side wheel down button.
 */
#define BUTTON_RIGHT_SIDE_WHEEL_DOWN ((uint8_t)16U)

/** @def BUTTON_GRIP_ANTICLOCKWISE
 *  @brief Button ID for the grip anticlockwise button.
 */
#define BUTTON_GRIP_ANTICLOCKWISE ((uint8_t)17U)

/** @def BUTTON_GRIP_CLOCKWISE
 *  @brief Button ID for the grip clockwise button.
 */
#define BUTTON_GRIP_CLOCKWISE ((uint8_t)18U)

/** @def BUTTON_ENG_ANTICLOCKWISE
 *  @brief Button ID for the engine anticlockwise button.
 */
#define BUTTON_ENG_ANTICLOCKWISE ((uint8_t)19U)

/** @def BUTTON_ENG_CLOCKWISE
 *  @brief Button ID for the engine clockwise button.
 */
#define BUTTON_ENG_CLOCKWISE ((uint8_t)20U)

/** @def BUTTON_22
 *  @brief Button ID for button 22.
 */
#define BUTTON_22 ((uint8_t)21U)

/** @def BUTTON_23
 *  @brief Button ID for button 23.
 */
#define BUTTON_23 ((uint8_t)22U)

/** @def BUTTON_GRIP
 *  @brief Button ID for the grip button.
 */
#define BUTTON_GRIP ((uint8_t)23U)

/** @def BUTTON_ENG
 *  @brief Button ID for the engine button.
 */
#define BUTTON_ENG ((uint8_t)24U)

/* Define constants for min and max values */
/** @def T818_WHEEL_ROTATION_MIN
 *  @brief Minimum value for wheel rotation.
 */
#define T818_WHEEL_ROTATION_MIN (0U)

/** @def T818_WHEEL_ROTATION_MAX
 *  @brief Maximum value for wheel rotation.
 */
#define T818_WHEEL_ROTATION_MAX (0xFFFFU)

/** @def T818_BRAKE_MIN
 *  @brief Minimum value for brake input.
 */
#define T818_BRAKE_MIN (0U)

/** @def T818_BRAKE_MAX
 *  @brief Maximum value for brake input.
 */
#define T818_BRAKE_MAX (1023U)

/** @def T818_THROTTLE_MIN
 *  @brief Minimum value for throttle input.
 */
#define T818_THROTTLE_MIN (0U)

/** @def T818_THROTTLE_MAX
 *  @brief Maximum value for throttle input.
 */
#define T818_THROTTLE_MAX (1023U)

/** @def T818_CLUTCH_MIN
 *  @brief Minimum value for clutch input.
 */
#define T818_CLUTCH_MIN (0U)

/** @def T818_CLUTCH_MAX
 *  @brief Maximum value for clutch input.
 */
#define T818_CLUTCH_MAX (1023U)

/** @def T818_VX_AXIS_MIN
 *  @brief Minimum value for Vx axis.
 */
#define T818_VX_AXIS_MIN (0U)

/** @def T818_VX_AXIS_MAX
 *  @brief Maximum value for Vx axis.
 */
#define T818_VX_AXIS_MAX (0xFFU)

/** @def T818_VY_AXIS_MIN
 *  @brief Minimum value for Vy axis.
 */
#define T818_VY_AXIS_MIN (0U)

/** @def T818_VY_AXIS_MAX
 *  @brief Maximum value for Vy axis.
 */
#define T818_VY_AXIS_MAX (0xFFU)

/** @def T818_RX_AXIS_MIN
 *  @brief Minimum value for Rx axis.
 */
#define T818_RX_AXIS_MIN (0U)

/** @def T818_RX_AXIS_MAX
 *  @brief Maximum value for Rx axis.
 */
#define T818_RX_AXIS_MAX (0xFFU)

/** @def T818_RY_AXIS_MIN
 *  @brief Minimum value for Ry axis.
 */
#define T818_RY_AXIS_MIN (0U)

/** @def T818_RY_AXIS_MAX
 *  @brief Maximum value for Ry axis.
 */
#define T818_RY_AXIS_MAX (0xFFU)

/** @def T818_Z_AXIS_MIN
 *  @brief Minimum value for Z axis.
 */
#define T818_Z_AXIS_MIN (0U)

/** @def T818_Z_AXIS_MAX
 *  @brief Maximum value for Z axis.
 */
#define T818_Z_AXIS_MAX (0xFFU)

/** @def T818_PAD_ARROW_MIN
 *  @brief Minimum value for pad arrow input.
 */
#define T818_PAD_ARROW_MIN (0U)

/** @def T818_PAD_ARROW_MAX
 *  @brief Maximum value for pad arrow input.
 */
#define T818_PAD_ARROW_MAX (0x0FU)


/* Data Structure Definitions -----------------------------------------------*/
/**
  * @defgroup USBH_HID_T818_Exported_Types USBH HID T818 Exported Types
  * @brief This module contains the exported types for the T818 HID device.
  * @{
  */

/**
  * @brief HID T818 Information Structure
  * @details This structure contains the information for the T818 HID device, including axis values and button states.
  */
typedef struct _HID_T818_Info {
    uint16_t wheel_rotation; /**< X axis rotation of the wheel */
    uint16_t brake:10;       /**< Y axis value for brake */
    uint16_t throttle:10;    /**< Rz axis value for throttle */
    uint16_t clutch:10;      /**< Slider axis value for clutch */
    uint8_t vx_axis;         /**< Vx axis, not mapped */
    uint8_t vy_axis;         /**< Vy axis, not mapped */
    uint8_t rx_axis;         /**< Rx axis value */
    uint8_t ry_axis;         /**< Ry axis value */
    uint8_t z_axis;          /**< Z axis, not mapped */
    uint8_t buttons[BUTTON_COUNT]; /**< Array of button states */
    uint8_t pad_arrow:4;     /**< D-pad arrow state */
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
