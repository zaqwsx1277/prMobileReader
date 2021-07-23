################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/crc.c \
../Core/Src/dcmi.c \
../Core/Src/dma.c \
../Core/Src/gpio.c \
../Core/Src/i2c.c \
../Core/Src/i2s.c \
../Core/Src/ov2640.c \
../Core/Src/rtc.c \
../Core/Src/sdio.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/system_stm32f4xx.c \
../Core/Src/tim.c \
../Core/Src/usart.c 

CPP_SRCS += \
../Core/Src/main.cpp 

OBJS += \
./Core/Src/crc.o \
./Core/Src/dcmi.o \
./Core/Src/dma.o \
./Core/Src/gpio.o \
./Core/Src/i2c.o \
./Core/Src/i2s.o \
./Core/Src/main.o \
./Core/Src/ov2640.o \
./Core/Src/rtc.o \
./Core/Src/sdio.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/system_stm32f4xx.o \
./Core/Src/tim.o \
./Core/Src/usart.o 

C_DEPS += \
./Core/Src/crc.d \
./Core/Src/dcmi.d \
./Core/Src/dma.d \
./Core/Src/gpio.d \
./Core/Src/i2c.d \
./Core/Src/i2s.d \
./Core/Src/ov2640.d \
./Core/Src/rtc.d \
./Core/Src/sdio.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/system_stm32f4xx.d \
./Core/Src/tim.d \
./Core/Src/usart.d 

CPP_DEPS += \
./Core/Src/main.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/crc.o: ../Core/Src/crc.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../FATFS/App -I../USB_DEVICE/Target -I../PDM2PCM/App -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../USB_DEVICE/App -I../FATFS/Target -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Middlewares/Third_Party/FatFs/src -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/crc.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/dcmi.o: ../Core/Src/dcmi.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../FATFS/App -I../USB_DEVICE/Target -I../PDM2PCM/App -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../USB_DEVICE/App -I../FATFS/Target -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Middlewares/Third_Party/FatFs/src -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/dcmi.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/dma.o: ../Core/Src/dma.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../FATFS/App -I../USB_DEVICE/Target -I../PDM2PCM/App -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../USB_DEVICE/App -I../FATFS/Target -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Middlewares/Third_Party/FatFs/src -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/dma.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/gpio.o: ../Core/Src/gpio.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../FATFS/App -I../USB_DEVICE/Target -I../PDM2PCM/App -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../USB_DEVICE/App -I../FATFS/Target -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Middlewares/Third_Party/FatFs/src -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/gpio.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/i2c.o: ../Core/Src/i2c.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../FATFS/App -I../USB_DEVICE/Target -I../PDM2PCM/App -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../USB_DEVICE/App -I../FATFS/Target -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Middlewares/Third_Party/FatFs/src -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/i2c.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/i2s.o: ../Core/Src/i2s.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../FATFS/App -I../USB_DEVICE/Target -I../PDM2PCM/App -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../USB_DEVICE/App -I../FATFS/Target -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Middlewares/Third_Party/FatFs/src -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/i2s.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/main.o: ../Core/Src/main.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../FATFS/App -I../USB_DEVICE/Target -I../PDM2PCM/App -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Core/cpp -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../USB_DEVICE/App -I../FATFS/Target -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Middlewares/Third_Party/FatFs/src -Os -ffunction-sections -fdata-sections -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Core/Src/main.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/ov2640.o: ../Core/Src/ov2640.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../FATFS/App -I../USB_DEVICE/Target -I../PDM2PCM/App -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../USB_DEVICE/App -I../FATFS/Target -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Middlewares/Third_Party/FatFs/src -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/ov2640.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/rtc.o: ../Core/Src/rtc.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../FATFS/App -I../USB_DEVICE/Target -I../PDM2PCM/App -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../USB_DEVICE/App -I../FATFS/Target -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Middlewares/Third_Party/FatFs/src -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/rtc.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/sdio.o: ../Core/Src/sdio.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../FATFS/App -I../USB_DEVICE/Target -I../PDM2PCM/App -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../USB_DEVICE/App -I../FATFS/Target -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Middlewares/Third_Party/FatFs/src -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/sdio.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/stm32f4xx_hal_msp.o: ../Core/Src/stm32f4xx_hal_msp.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../FATFS/App -I../USB_DEVICE/Target -I../PDM2PCM/App -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../USB_DEVICE/App -I../FATFS/Target -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Middlewares/Third_Party/FatFs/src -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/stm32f4xx_hal_msp.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/stm32f4xx_it.o: ../Core/Src/stm32f4xx_it.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../FATFS/App -I../USB_DEVICE/Target -I../PDM2PCM/App -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../USB_DEVICE/App -I../FATFS/Target -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Middlewares/Third_Party/FatFs/src -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/stm32f4xx_it.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/system_stm32f4xx.o: ../Core/Src/system_stm32f4xx.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../FATFS/App -I../USB_DEVICE/Target -I../PDM2PCM/App -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../USB_DEVICE/App -I../FATFS/Target -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Middlewares/Third_Party/FatFs/src -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/system_stm32f4xx.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/tim.o: ../Core/Src/tim.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../FATFS/App -I../USB_DEVICE/Target -I../PDM2PCM/App -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../USB_DEVICE/App -I../FATFS/Target -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Middlewares/Third_Party/FatFs/src -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/tim.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/usart.o: ../Core/Src/usart.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../FATFS/App -I../USB_DEVICE/Target -I../PDM2PCM/App -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../USB_DEVICE/App -I../FATFS/Target -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Middlewares/Third_Party/FatFs/src -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/Src/usart.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

