# usbh_hid_generic

This repository provides a working USB host library to interact with generic HID devices using STM32F USB host compatible MCUs.
* It is intended to be used in an auto generated STM32CubeMX project, and to be robust to re-generation of the code.
* It can be used in an OS-free system but it is also compatible with FreeRTOS (using the CMSIS-OS V2 provided by ST). 
* It was tested on a STM32F446RE nucleo board and proved to be working with a Dualshock 3 controller. (see examples folder)

## Using the library (the easy way)

In order to use the library, a project should be created and configured, through cubeMX, to use the USB Full Speed peripheral in HID host mode and optionnaly to use FreeRTOS. The settings to check are:
* System core - SYS - Mode - Timebase Source = TIM6
* Connectivity - USB_OTG_FS - Mode - Mode = Host_Only
* Middleware - USB_HOST - Mode - Class for FS IP = Mass Storage Host Class (in order to include all usb core files in the project, the initialization will be bypassed so it won't be initialized as a USB MSC host)
* Middleware - USB_HOST - Configuration - Platform Settings = choose a GPIO output here to enable/disable VBUS power (can be used on other nucleo board with integrated USB host hardware)

Then you can copy the content of the repository `src` folder into the root folder of your project. You can then use the `usb_hid_generic.c` library to interface with a custom driver for the HID device you want to interact with (see `examples/Dualshock_3` to know how to do that).

### Testing

The example project in `examples/Dualshock_3` was tested functionnal. It should compile directly for a NUCELO-F446RE board. The following picture shows how to connect the usb cable (using a small micro usb breakout board and an OTG adapter). The USB device power supply is provided by a +5V pin on the CN7 connector. When a DS3 gamepad is connected, the nucleo board will write the gamepad buttons state to the STLink serial port, as well as the 3 axes accelerometer values and the gyroscope value.

<p align="center">
  <img src="/ressources/wiring.jpg" alt="Wiring" width="480"/>
</p>

## Manually modding usbh_hid from ST (the hard way)
### Issues with the usbh_hid library
There are mainly two issues with this library:
* It does not work when using a FreeRTOS, even if it appears to be supposed to. (generate HardFaults)
* It only works with boot mode keyboards or mouses and was not designed to be used with generic HID devices such as game controllers.

### Issue 1 (part 1)

The hardfault issue seems to come from a malloc in the usbh_hid.c file:
https://github.com/STMicroelectronics/STM32CubeF4/blob/master/Middlewares/ST/STM32_USB_Host_Library/Class/HID/Src/usbh_hid.c#L160
```
static USBH_StatusTypeDef USBH_HID_InterfaceInit(USBH_HandleTypeDef *phost)
{
  ...
  phost->pActiveClass->pData = (HID_HandleTypeDef *)USBH_malloc(sizeof(HID_HandleTypeDef));
  ...
}
```
In order to fix that, the HID handle is created as a static global variable: 
```
static HID_HandleTypeDef class_pData[sizeof(HID_HandleTypeDef)];
...
static USBH_StatusTypeDef USBH_HID_InterfaceInit(USBH_HandleTypeDef *phost)
{
  ...
  phost->pActiveClass->pData = class_pData;
  ...
}
```

### Issue 1 (part 2)

When using the USB HID library with FreeRTOS, the library creates its own thread to run the USB process function. This also generates hardfault issues. Instead of debugging ST library, I just created another thread in the `main.c` file that uses a mutex on the usbh ressource, and runs the USB process. In order to avoid conflicts with the auto generated thread, it is stopped by a `MX_USB_HOST_SuspendThread()` function added to `usb_host.c`. The last thing to do is to suspend the default task using `osThreadSuspend(defaultTaskHandle)` before the `osKernelStart()` and to init the usb peripheral using `MX_USB_HOST_Init()` in the main (also before `osKernelStart()`).

### Issue 2

In order to use generic HID devices, a few modifications are recquired in usbh_hid.c:

#### Finding generic interfaces

(original code: https://github.com/STMicroelectronics/STM32CubeF4/blob/2f3b26f16559f7af495727a98253067a31182cfc/Middlewares/ST/STM32_USB_Host_Library/Class/HID/Src/usbh_hid.c#L145)

The original code only looks for BOOT subclass devices. This code is adapted using prepocessor directives to be able to easily roll back to the BOOT mode by just defining BOOTMODE.
```
#ifndef BOOTMODE
interface = USBH_FindInterface(phost, phost->pActiveClass->ClassCode, 0xFFU, 0xFFU);
#else
interface = USBH_FindInterface(phost, phost->pActiveClass->ClassCode, HID_BOOT_CODE, 0xFFU);
#endif
```

#### Calling `usbh_hid_generic.c`

The `usbh_hid_generic.c` code is based ont the mouse and keyboard examples from ST, but it allows to register and to automatically load a driver for a connected device (see `examples/Dualshock_3` to know how to create a driver). The driver is selected based on the VID and PID of the connected device.

In https://github.com/STMicroelectronics/STM32CubeF4/blob/2f3b26f16559f7af495727a98253067a31182cfc/Middlewares/ST/STM32_USB_Host_Library/Class/HID/Src/usbh_hid.c#L174, the code becomes:
```
  /*Decode Bootclass Protocol: Mouse or Keyboard*/
  if (phost->device.CfgDesc.Itf_Desc[interface].bInterfaceProtocol == HID_KEYBRD_BOOT_CODE)
  {
    USBH_UsrLog("KeyBoard device found!");
    HID_Handle->Init = USBH_HID_KeybdInit;
  }
  else if (phost->device.CfgDesc.Itf_Desc[interface].bInterfaceProtocol  == HID_MOUSE_BOOT_CODE)
  {
    USBH_UsrLog("Mouse device found!");
    HID_Handle->Init = USBH_HID_MouseInit;
  }
  else
  {
    USBH_UsrLog("Generic HID device found!");
    HID_Handle->Init = USBH_HID_GenericInit;
  }
```

#### Bypassing the set protocol state

When configuring a boot mode device, the USB host state machine goes through a state that will send a SetProtocol request to the usb device. This request must be bypassed when using a generic HID device:
```
  case HID_REQ_SET_PROTOCOL:
#ifndef BOOTMODE
    /* bypass the set protocol state */
    HID_Handle->ctl_state = HID_REQ_IDLE;
    phost->pUser(phost, HOST_USER_CLASS_ACTIVE);
    status = USBH_OK;
#else
    /* set protocol */
    classReqStatus = USBH_HID_SetProtocol(phost, 0U);
    if (classReqStatus == USBH_OK)
    {
      HID_Handle->ctl_state = HID_REQ_IDLE;

      /* all requests performed*/
      phost->pUser(phost, HOST_USER_CLASS_ACTIVE);
      status = USBH_OK;
    }
    else if (classReqStatus == USBH_NOT_SUPPORTED)
    {
      USBH_ErrLog("Control error: HID: Device Set protocol request failed");
      status = USBH_FAIL;
    }
    else
    {
      /* .. */
    }

#endif
    break;
```
### Summary of the modifications
The modifications are only required in the usbh_hid.c file. The system then works with the `usbh_hid_generic.c` code to load the proper driver for a connected device. The modifications are necessary because of bugs when using FreeRTOS alongside the usb host library, and to add the HID generic capability.



