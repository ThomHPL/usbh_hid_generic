/**
  ******************************************************************************
  * @file    usbh_hid_generic.h
  * @author  Thomas Herpoel
  * @brief   This file contains all the prototypes for the usbh_hid_generic.c
  ******************************************************************************
  */

#ifndef __USBH_HID_GENERIC_H
#define __USBH_HID_GENERIC_H

#include <USBH_HID_generic/usbh_hid.h>

typedef struct {
  char name[32];
  uint16_t vid;
  uint16_t pid;
  USBH_StatusTypeDef(*DeviceSpecificInit)(struct _USBH_HandleTypeDef *phost);
} USBH_HID_DriverTypeDef;

USBH_StatusTypeDef USBH_HID_GenericInit(USBH_HandleTypeDef *phost);
USBH_StatusTypeDef USBH_HID_GetRawReport(USBH_HandleTypeDef *phost, uint8_t* report);
USBH_StatusTypeDef USBH_HID_DeviceSpecificInit(USBH_HandleTypeDef *phost);
USBH_StatusTypeDef USBH_HID_GenericRegisterDriver(USBH_HID_DriverTypeDef driver);

#endif
