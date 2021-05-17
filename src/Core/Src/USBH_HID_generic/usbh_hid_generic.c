/**
  ******************************************************************************
  * @file    usbh_hid_generic.c
  * @author  Thomas Herpoel
  * @brief   This file is the application layer for USB Host HID generic device handling.
  ******************************************************************************
  * @attention
  *	Inspired by the usbh_hid_mouse.c code included in the usb host hid library from ST with following disclaimer:
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                      www.st.com/SLA0044
  *
  ******************************************************************************
  */



/* BSPDependencies
- "stm32xxxxx_{eval}{discovery}{nucleo_144}.c"
- "stm32xxxxx_{eval}{discovery}_io.c"
- "stm32xxxxx_{eval}{discovery}{adafruit}_lcd.c"
- "stm32xxxxx_{eval}{discovery}_sdram.c"
EndBSPDependencies */

/* Includes ------------------------------------------------------------------*/
#include "USBH_HID_generic/usbh_hid_generic.h"
#include "USBH_HID_generic/usbh_hid_parser.h"


/** @addtogroup USBH_LIB
  * @{
  */

/** @addtogroup USBH_CLASS
  * @{
  */

/** @addtogroup USBH_HID_CLASS
  * @{
  */

/** @defgroup USBH_HID_GENERIC
  * @brief    This file includes HID Layer Handlers for USB Host HID class.
  * @{
  */

/** @defgroup USBH_HID_GENERIC_Private_TypesDefinitions
  * @{
  */
/**
  * @}
  */

/** @defgroup USBH_HID_GENERIC_Private_Defines
  * @{
  */
/**
  * @}
  */


/** @defgroup USBH_HID_GENERIC_Private_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup USBH_HID_GENERIC_Private_FunctionPrototypes
  * @{
  */

/**
  * @}
  */


/** @defgroup USBH_HID_GENERIC_Private_Variables
  * @{
  */
uint8_t	rx_report_buf[128];
#define MAX_DRIVERS 4
USBH_HID_DriverTypeDef drivers[MAX_DRIVERS];
USBH_HID_DriverTypeDef current_driver;
uint8_t n_drivers = 0;
/**
  * @}
  */


/** @defgroup USBH_HID_GENERIC_Private_Functions
  * @{
  */

/**
  * @brief  USBH_HID_GenericLoadDriver
  *         The function loads a specific device driver to be used when the corresponding device is connected.
  *         It is added to a list of drivers available.
  * @param  phost: Host handle
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_HID_GenericLoadDriver(USBH_HID_DriverTypeDef driver)
{
	if (n_drivers == MAX_DRIVERS)
	{
		return USBH_FAIL;
	}
	drivers[n_drivers] = driver;
	n_drivers += 1;
	return USBH_OK;
}

/**
  * @brief  USBH_HID_GenericInit
  *         The function init the HID generic device.
  * @param  phost: Host handle
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_HID_GenericInit(USBH_HandleTypeDef *phost)
{
  uint32_t i;
  HID_HandleTypeDef *HID_Handle = (HID_HandleTypeDef *) phost->pActiveClass->pData;

  for (i = 0U; i < (sizeof(rx_report_buf) / sizeof(uint8_t)); i++)
  {

	  rx_report_buf[i] = 0U;
  }

  if (HID_Handle->length > sizeof(rx_report_buf))
  {
    HID_Handle->length = sizeof(rx_report_buf);
  }

  HID_Handle->pData = (uint8_t *)(void *)rx_report_buf;

  USBH_HID_FifoInit(&HID_Handle->fifo, phost->device.Data, 2 * sizeof(rx_report_buf));

  USBH_StatusTypeDef status = USBH_NOT_SUPPORTED;
  for(uint8_t i = 0; i < n_drivers; i++)
  {
	  if((phost->device.DevDesc.idVendor == drivers[i].vid) && (phost->device.DevDesc.idProduct == drivers[i].pid))
	  {
		  current_driver = drivers[i];
		  status = current_driver.DeviceSpecificInit(phost);
	  }
  }

  return status;
  //return USBH_HID_DeviceSpecificInit(phost);
}

/**
  * @brief  USBH_HID_GetRawReport
  *         The function gives the next raw HID report in the fifo
  * @param  phost: Host handle
  * @param  report: pointer to the raw report
  *
  * @retval USBH status
  */
USBH_StatusTypeDef USBH_HID_GetRawReport(USBH_HandleTypeDef *phost, uint8_t* report)
{
  HID_HandleTypeDef *HID_Handle = (HID_HandleTypeDef *) phost->pActiveClass->pData;

  if (HID_Handle->length == 0U)
  {
	return USBH_FAIL;
  }
  /*Fill report */
  if(USBH_HID_FifoRead(&HID_Handle->fifo, report, HID_Handle->length) !=  HID_Handle->length)
  {
	return USBH_FAIL;
  }
  return USBH_OK;
}

/**
  * @}
  */
__weak USBH_StatusTypeDef USBH_HID_DeviceSpecificInit(USBH_HandleTypeDef *phost)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(phost);
  return USBH_OK;
}

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
