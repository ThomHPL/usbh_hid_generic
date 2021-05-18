# usbh_hid_generic

This repository aim is to provide a working USB host library to interact with generic HID devices using STM32F USB host compatible MCUs.
* It is intended to be used in an auto generated STM32CubeMX project, and to be robust to re-generation of the code.
* It can be used in an OS-free system but it is also compatible with FreeRTOS (using the CMSIS-OS V2 provided by ST). 
* It was tested on a STM32F446RE nucleo board and proved to be working with a Dualshock 3 controller. (see examples folder)

## Issues with the usbh_hid library from ST

There are mainly two issues with this library:
* It only works with boot mode keyboards or mouses and was not designed to be used with generic HID devices such as game controllers.
* It does not work when using a FreeRTOS, even if it appears to be supposed to.

## Testing the library

The example project in `examples/Dualshock_3` was tested functionnal. It should compile directly for a NUCELO-F446RE board.
![Alt text](/ressources/wiring.jpg?raw=true "Title")
