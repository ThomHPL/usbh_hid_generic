################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/USBH_HID_generic/usbh_hid.c \
../Core/Src/USBH_HID_generic/usbh_hid_generic.c \
../Core/Src/USBH_HID_generic/usbh_hid_keybd.c \
../Core/Src/USBH_HID_generic/usbh_hid_mouse.c \
../Core/Src/USBH_HID_generic/usbh_hid_parser.c 

OBJS += \
./Core/Src/USBH_HID_generic/usbh_hid.o \
./Core/Src/USBH_HID_generic/usbh_hid_generic.o \
./Core/Src/USBH_HID_generic/usbh_hid_keybd.o \
./Core/Src/USBH_HID_generic/usbh_hid_mouse.o \
./Core/Src/USBH_HID_generic/usbh_hid_parser.o 

C_DEPS += \
./Core/Src/USBH_HID_generic/usbh_hid.d \
./Core/Src/USBH_HID_generic/usbh_hid_generic.d \
./Core/Src/USBH_HID_generic/usbh_hid_keybd.d \
./Core/Src/USBH_HID_generic/usbh_hid_mouse.d \
./Core/Src/USBH_HID_generic/usbh_hid_parser.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/USBH_HID_generic/usbh_hid.o: ../Core/Src/USBH_HID_generic/usbh_hid.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/USBH_HID_generic/usbh_hid.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/USBH_HID_generic/usbh_hid_generic.o: ../Core/Src/USBH_HID_generic/usbh_hid_generic.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/USBH_HID_generic/usbh_hid_generic.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/USBH_HID_generic/usbh_hid_keybd.o: ../Core/Src/USBH_HID_generic/usbh_hid_keybd.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/USBH_HID_generic/usbh_hid_keybd.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/USBH_HID_generic/usbh_hid_mouse.o: ../Core/Src/USBH_HID_generic/usbh_hid_mouse.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/USBH_HID_generic/usbh_hid_mouse.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/USBH_HID_generic/usbh_hid_parser.o: ../Core/Src/USBH_HID_generic/usbh_hid_parser.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/USBH_HID_generic/usbh_hid_parser.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

