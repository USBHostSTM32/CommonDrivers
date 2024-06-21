/* Includes ------------------------------------------------------------------*/
#include "usbh_hid_t818.h"
#include "usbh_hid_parser.h"

static USBH_StatusTypeDef USBH_HID_T818Decode(USBH_HandleTypeDef *phost);

static HID_T818_Info_TypeDef t818_info;

uint8_t t818_report_data[T818_REPORT_SIZE];
uint8_t t818_rx_report_buf[T818_REPORT_SIZE];

/* Structures defining how to access items in a HID T818 report */

/* Access x coordinate change. */
static const HID_Report_ItemTypedef x_axis_state =
{
  t818_report_data + 1, /*data*/
  16,     /*size*/
  0,     /*shift*/
  0,     /*count (only for array items)*/
  0,     /*signed?*/
  0,     /*min value read can return*/
  0xFFFF,  /*max value read can return*/
  0,     /*min value device can report*/
  0xFFFF,  /*max value device can report*/
  1      /*resolution*/
};

/* Access y coordinate change. */
static const HID_Report_ItemTypedef y_axis_state =
{
  t818_report_data + 3, /*data*/
  16,     /*size*/
  0,     /*shift*/
  0,     /*count (only for array items)*/
  0,     /*signed?*/
  0,     /*min value read can return*/
  1023,  /*max value read can return*/
  0,     /*min value device can report*/
  1023,  /*max value device can report*/
  1      /*resolution*/
};

/* Access rz coordinate change. */
static const HID_Report_ItemTypedef rz_axis_state =
{
  t818_report_data + 5, /*data*/
  16,     /*size*/
  0,     /*shift*/
  0,     /*count (only for array items)*/
  0,     /*signed?*/
  0,     /*min value read can return*/
  1023,  /*max value read can return*/
  0,     /*min value device can report*/
  1023,  /*max value device can report*/
  1      /*resolution*/
};

/* Access slider coordinate change. */
static const HID_Report_ItemTypedef slider_axis_state =
{
  t818_report_data + 7, /*data*/
  16,     /*size*/
  0,     /*shift*/
  0,     /*count (only for array items)*/
  0,     /*signed?*/
  0,     /*min value read can return*/
  1023,  /*max value read can return*/
  0,     /*min value device can report*/
  1023,  /*max value device can report*/
  1      /*resolution*/
};

/* Access vx coordinate change. */
static const HID_Report_ItemTypedef vx_axis_state =
{
  t818_report_data + 9, /*data*/
  8,     /*size*/
  0,     /*shift*/
  0,     /*count (only for array items)*/
  0,     /*signed?*/
  0,     /*min value read can return*/
  0xFF,  /*max value read can return*/
  0,     /*min value device can report*/
  0xFF,     /*max value device can report*/
  1      /*resolution*/
};

/* Access vy coordinate change. */
static const HID_Report_ItemTypedef vy_axis_state =
{
  t818_report_data + 10, /*data*/
  8,     /*size*/
  0,     /*shift*/
  0,     /*count (only for array items)*/
  0,     /*signed?*/
  0,     /*min value read can return*/
  0xFF,  /*max value read can return*/
  0,     /*min value device can report*/
  0xFF,     /*max value device can report*/
  1      /*resolution*/
};

/* Access rx coordinate change. */
static const HID_Report_ItemTypedef rx_axis_state =
{
  t818_report_data + 11, /*data*/
  8,     /*size*/
  0,     /*shift*/
  0,     /*count (only for array items)*/
  0,     /*signed?*/
  0,     /*min value read can return*/
  0xFF,  /*max value read can return*/
  0,     /*min value device can report*/
  0xFF,     /*max value device can report*/
  1      /*resolution*/
};

/* Access ry coordinate change. */
static const HID_Report_ItemTypedef ry_axis_state =
{
  t818_report_data + 12, /*data*/
  8,     /*size*/
  0,     /*shift*/
  0,     /*count (only for array items)*/
  0,     /*signed?*/
  0,     /*min value read can return*/
  0xFF,  /*max value read can return*/
  0,     /*min value device can report*/
  0xFF,     /*max value device can report*/
  1      /*resolution*/
};

/* Access z coordinate change. */
static const HID_Report_ItemTypedef z_axis_state =
{
  t818_report_data + 13, /*data*/
  8,     /*size*/
  0,     /*shift*/
  0,     /*count (only for array items)*/
  0,     /*signed?*/
  0,     /*min value read can return*/
  0xFF,  /*max value read can return*/
  0,     /*min value device can report*/
  0xFF,     /*max value device can report*/
  1      /*resolution*/
};

/* Access arrow pad state. */
static const HID_Report_ItemTypedef pad_arrow_state =
{
  t818_report_data + 19, /*data*/
  4,     /*size*/
  0,     /*shift*/
  0,     /*count (only for array items)*/
  0,     /*signed?*/
  0,     /*min value read can return*/
  0x0F,  /*max value read can return*/
  0,     /*min value device can report*/
  1,     /*max value device can report*/
  1      /*resolution*/
};

/* Define button states mapping */
typedef struct {
	ButtonIndex index;
	HID_Report_ItemTypedef report_item;
} ButtonReportConfig;

static const ButtonReportConfig button_report_configs[BUTTON_COUNT] = {
	{BUTTON_PADDLE_SHIFTER_LEFT, {t818_report_data + 15, 1, 0, 0, 0, 0, 1, 0, 1, 1}},
	{BUTTON_PADDLE_SHIFTER_RIGHT, {t818_report_data + 15, 1, 1, 0, 0, 0, 1, 0, 1, 1}},
	{BUTTON_DRINK, {t818_report_data + 15, 1, 2, 0, 0, 0, 1, 0, 1, 1}},
	{BUTTON_RADIO, {t818_report_data + 15, 1, 3, 0, 0, 0, 1, 0, 1, 1}},
	{BUTTON_ONE_PLUS, {t818_report_data + 15, 1, 4, 0, 0, 0, 1, 0, 1, 1}},
	{BUTTON_TEN_MINUS, {t818_report_data + 15, 1, 5, 0, 0, 0, 1, 0, 1, 1}},
	{BUTTON_SHA, {t818_report_data + 15, 1, 6, 0, 0, 0, 1, 0, 1, 1}},
	{BUTTON_OIL, {t818_report_data + 15, 1, 7, 0, 0, 0, 1, 0, 1, 1}},
	{BUTTON_PARKING, {t818_report_data + 16, 1, 0, 0, 0, 0, 1, 0, 1, 1}},
	{BUTTON_NEUTRAL, {t818_report_data + 16, 1, 1, 0, 0, 0, 1, 0, 1, 1}},
	{BUTTON_K1, {t818_report_data + 16, 1, 2, 0, 0, 0, 1, 0, 1, 1}},
	{BUTTON_K2, {t818_report_data + 16, 1, 3, 0, 0, 0, 1, 0, 1, 1}},
	{BUTTON_S1, {t818_report_data + 16, 1, 4, 0, 0, 0, 1, 0, 1, 1}},
	{BUTTON_LEFT_SIDE_WHEEL_UP, {t818_report_data + 16, 1, 5, 0, 0, 0, 1, 0, 1, 1}},
	{BUTTON_LEFT_SIDE_WHEEL_DOWN, {t818_report_data + 16, 1, 6, 0, 0, 0, 1, 0, 1, 1}},
	{BUTTON_RIGHT_SIDE_WHEEL_UP, {t818_report_data + 17, 1, 0, 0, 0, 0, 1, 0, 1, 1}},
	{BUTTON_RIGHT_SIDE_WHEEL_DOWN, {t818_report_data + 16, 1, 7, 0, 0, 0, 1, 0, 1, 1}},
	{BUTTON_GRIP_ANTICLOCKWISE, {t818_report_data + 17, 1, 1, 0, 0, 0, 1, 0, 1, 1}},
	{BUTTON_GRIP_CLOCKWISE, {t818_report_data + 17, 1, 2, 0, 0, 0, 1, 0, 1, 1}},
	{BUTTON_ENG_ANTICLOCKWISE, {t818_report_data + 17, 1, 3, 0, 0, 0, 1, 0, 1, 1}},
	{BUTTON_ENG_CLOCKWISE, {t818_report_data + 17, 1, 4, 0, 0, 0, 1, 0, 1, 1}},
	{BUTTON_22, {t818_report_data + 17, 1, 5, 0, 0, 0, 1, 0, 1, 1}},
	{BUTTON_23, {t818_report_data + 17, 1, 6, 0, 0, 0, 1, 0, 1, 1}},
	{BUTTON_GRIP, {t818_report_data + 17, 1, 7, 0, 0, 0, 1, 0, 1, 1}},
	{BUTTON_ENG, {t818_report_data + 18, 1, 0, 0, 0, 0, 1, 0, 1, 1}}
};

/**
  * @}
  */

/** @defgroup USBH_HID_T818_Private_Functions
  * @{
  */

/**
  * @brief  USBH_HID_T818Init
  *         The function init the HID T818.
  * @param  phost: Host handle
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_HID_T818Init(USBH_HandleTypeDef *phost)
{
  uint32_t i;
  HID_HandleTypeDef *HID_Handle = (HID_HandleTypeDef *) phost->pActiveClass->pData;

  memset(&t818_info, 0, sizeof(HID_T818_Info_TypeDef));

  for (i = 0U; i < (sizeof(t818_report_data)); i++)
  {
  	t818_report_data[i] = 0U;
  	t818_rx_report_buf[i]=0U;
  }

  if (HID_Handle->length > sizeof(t818_report_data))
  {
    HID_Handle->length = (uint16_t)sizeof(t818_report_data);
  }

  HID_Handle->pData = t818_rx_report_buf;
  if ((HID_QUEUE_SIZE * sizeof(t818_report_data)) > sizeof(phost->device.Data))
  {
	  return USBH_FAIL;
  }
  else
  {
	  USBH_HID_FifoInit(&HID_Handle->fifo, phost->device.Data, HID_QUEUE_SIZE * sizeof(t818_report_data));
  }
  return USBH_OK;
}

//ADD DOXYGEN
USBH_StatusTypeDef USBH_HID_GetT818Info(USBH_HandleTypeDef *phost)
{
	USBH_StatusTypeDef status=USBH_FAIL;

  if (USBH_HID_T818Decode(phost) == USBH_OK)
  {
    status=USBH_OK;
  }

  return status;

}

//ADD DOXYGEN
HID_T818_Info_TypeDef* USBH_HID_T818GetInstance()
{
	return &t818_info;
}

/**
  * @brief  USBH_HID_T818Decode
  *         The function decode T818 data.
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_HID_T818Decode(USBH_HandleTypeDef *phost)
{
  HID_HandleTypeDef *HID_Handle = (HID_HandleTypeDef *) phost->pActiveClass->pData;

  if (HID_Handle->length == 0U || (HID_Handle->fifo.buf == NULL))
  {
    return USBH_FAIL;
  }

  /*Fill report */
  if (USBH_HID_FifoRead(&HID_Handle->fifo, &t818_report_data, HID_Handle->length) ==  HID_Handle->length)
  {

    /*Decode report */
    t818_info.wheel_rotation = (uint16_t)HID_ReadItem((HID_Report_ItemTypedef *) &x_axis_state, 0U);
    t818_info.brake = (uint16_t)HID_ReadItem((HID_Report_ItemTypedef *) &y_axis_state, 0U);
    t818_info.throttle = (uint16_t)HID_ReadItem((HID_Report_ItemTypedef *) &rz_axis_state, 0U);
    t818_info.clutch = (uint16_t)HID_ReadItem((HID_Report_ItemTypedef *) &slider_axis_state, 0U);
    t818_info.vx_axis = (uint16_t)HID_ReadItem((HID_Report_ItemTypedef *) &vx_axis_state, 0U);
    t818_info.vy_axis = (uint16_t)HID_ReadItem((HID_Report_ItemTypedef *) &vy_axis_state, 0U);
    t818_info.rx_axis = (uint16_t)HID_ReadItem((HID_Report_ItemTypedef *) &rx_axis_state, 0U);
    t818_info.ry_axis = (uint16_t)HID_ReadItem((HID_Report_ItemTypedef *) &ry_axis_state, 0U);
    t818_info.z_axis = (uint16_t)HID_ReadItem((HID_Report_ItemTypedef *) &z_axis_state, 0U);

    for (int i = 0; i < BUTTON_COUNT; i++) {
    	HID_Report_ItemTypedef * report_item = (HID_Report_ItemTypedef *) &button_report_configs[i].report_item;
    	t818_info.buttons[button_report_configs[i].index] = (uint8_t)HID_ReadItem(report_item, 0U);
    }

    t818_info.pad_arrow = (uint8_t)HID_ReadItem((HID_Report_ItemTypedef *) &pad_arrow_state, 0U);

    return USBH_OK;
  }

  return USBH_FAIL;
}

/************************ END OF FILE****/
