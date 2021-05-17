/**
  ******************************************************************************
  * @file    usbh_hid_generic.h
  * @author  MCD Application Team
  * @brief   This file contains all the prototypes for the usbh_hid_generic.c
  ******************************************************************************
  * @attention
  *
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

/* Define to prevent recursive  ----------------------------------------------*/
#ifndef __USBH_HID_GENERIC_H
#define __USBH_HID_GENERIC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <USBH_HID_generic/usbh_hid.h>

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
  * @brief This file is the Header file for usbh_hid_generic.c
  * @{
  */


/** @defgroup USBH_HID_GENERIC_Exported_Types
  * @{
  */


typedef struct {
  char name[32];
  uint16_t vid;
  uint16_t pid;
  USBH_StatusTypeDef(*DeviceSpecificInit)(struct _USBH_HandleTypeDef *phost);
} USBH_HID_DriverTypeDef;

/**
  * @}
  */

/** @defgroup USBH_HID_GENERIC_Exported_Defines
  * @{
  */
/**
  * @}
  */

/** @defgroup USBH_HID_GENERIC_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup USBH_HID_GENERIC_Exported_Variables
  * @{
  */
/**
  * @}
  */

/** @defgroup USBH_HID_GENERIC_Exported_FunctionsPrototype
  * @{
  */
USBH_StatusTypeDef USBH_HID_GenericInit(USBH_HandleTypeDef *phost);
USBH_StatusTypeDef USBH_HID_GetRawReport(USBH_HandleTypeDef *phost, uint8_t* report);
USBH_StatusTypeDef USBH_HID_DeviceSpecificInit(USBH_HandleTypeDef *phost);
USBH_StatusTypeDef USBH_HID_GenericLoadDriver(USBH_HID_DriverTypeDef driver);


/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __USBH_HID_GENERIC_H */

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
