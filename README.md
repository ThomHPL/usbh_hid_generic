# usbh_hid_generic

This repository provides a working USB host library to interact with generic HID devices using STM32F USB host compatible MCUs.
* It is intended to be used in an auto generated STM32CubeMX project, and to be robust to re-generation of the code.
* It can be used in an OS-free system but it is also compatible with FreeRTOS (using the CMSIS-OS V2 provided by ST). 
* It was tested on a STM32F446RE nucleo board and proved to be working with a Dualshock 3 controller. (see examples folder)

## Issues with the usbh_hid library from ST

There are mainly two issues with this library:
* It only works with boot mode keyboards or mouses and was not designed to be used with generic HID devices such as game controllers.
* It does not work when using a FreeRTOS, even if it appears to be supposed to.

## Using the library

In order to use the library, a project should be created and configured, through cubeMX, to use the USB Full Speed peripheral in HID host mode and optionnaly to use FreeRTOS. The settings to check are:
* System core - SYS - Mode - Timebase Source = TIM6
* Connectivity - USB_OTG_FS - Mode - Mode = Host_Only
* Middleware - USB_HOST - Mode - Class for FS IP = Mass Storage Host Class (in order to include all usb core files in the project)
* Middleware - USB_HOST - Configuration - Platform Settings = choose a GPIO output here to enable/disable VBUS power (can be used on other nucleo board with integrated USB host hardware)

Then you can copy the content of the repository `src` folder into the root folder of your project. You can then use the `usb_hid_generic.c` library to interface with a custom driver for the HID device you want to interact with (see `examples/Dualshock_3` to know how to do that).

## Testing the library

The example project in `examples/Dualshock_3` was tested functionnal. It should compile directly for a NUCELO-F446RE board. The following picture shows how to connect the usb cable (using a small micro usb breakout board and an OTG adapter). The USB device power supply is provided by a +5V pin on the CN7 connector. When a DS3 gamepad is connected, the nucleo board will write the gamepad buttons state to the STLink serial port, as well as the 3 axes accelerometer values and the gyroscope value.

<p align="center">
  <img src="/ressources/wiring.jpg" alt="Wiring" width="480"/>
</p>
