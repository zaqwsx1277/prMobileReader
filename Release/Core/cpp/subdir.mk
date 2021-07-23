################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/cpp/TApplication.cpp \
../Core/cpp/TAudio.cpp \
../Core/cpp/TBq25121.cpp \
../Core/cpp/TButton.cpp \
../Core/cpp/TCallback.cpp \
../Core/cpp/TCommon.cpp \
../Core/cpp/TFileSystem.cpp \
../Core/cpp/TI2C.cpp \
../Core/cpp/TPhoto.cpp \
../Core/cpp/TSdio.cpp \
../Core/cpp/TTag.cpp \
../Core/cpp/TUnit.cpp 

OBJS += \
./Core/cpp/TApplication.o \
./Core/cpp/TAudio.o \
./Core/cpp/TBq25121.o \
./Core/cpp/TButton.o \
./Core/cpp/TCallback.o \
./Core/cpp/TCommon.o \
./Core/cpp/TFileSystem.o \
./Core/cpp/TI2C.o \
./Core/cpp/TPhoto.o \
./Core/cpp/TSdio.o \
./Core/cpp/TTag.o \
./Core/cpp/TUnit.o 

CPP_DEPS += \
./Core/cpp/TApplication.d \
./Core/cpp/TAudio.d \
./Core/cpp/TBq25121.d \
./Core/cpp/TButton.d \
./Core/cpp/TCallback.d \
./Core/cpp/TCommon.d \
./Core/cpp/TFileSystem.d \
./Core/cpp/TI2C.d \
./Core/cpp/TPhoto.d \
./Core/cpp/TSdio.d \
./Core/cpp/TTag.d \
./Core/cpp/TUnit.d 


# Each subdirectory must supply rules for building sources it contributes
Core/cpp/TApplication.o: ../Core/cpp/TApplication.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../FATFS/App -I../USB_DEVICE/Target -I../PDM2PCM/App -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Core/cpp -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../USB_DEVICE/App -I../FATFS/Target -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Middlewares/Third_Party/FatFs/src -Os -ffunction-sections -fdata-sections -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Core/cpp/TApplication.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/cpp/TAudio.o: ../Core/cpp/TAudio.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../FATFS/App -I../USB_DEVICE/Target -I../PDM2PCM/App -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Core/cpp -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../USB_DEVICE/App -I../FATFS/Target -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Middlewares/Third_Party/FatFs/src -Os -ffunction-sections -fdata-sections -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Core/cpp/TAudio.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/cpp/TBq25121.o: ../Core/cpp/TBq25121.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../FATFS/App -I../USB_DEVICE/Target -I../PDM2PCM/App -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Core/cpp -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../USB_DEVICE/App -I../FATFS/Target -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Middlewares/Third_Party/FatFs/src -Os -ffunction-sections -fdata-sections -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Core/cpp/TBq25121.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/cpp/TButton.o: ../Core/cpp/TButton.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../FATFS/App -I../USB_DEVICE/Target -I../PDM2PCM/App -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Core/cpp -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../USB_DEVICE/App -I../FATFS/Target -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Middlewares/Third_Party/FatFs/src -Os -ffunction-sections -fdata-sections -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Core/cpp/TButton.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/cpp/TCallback.o: ../Core/cpp/TCallback.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../FATFS/App -I../USB_DEVICE/Target -I../PDM2PCM/App -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Core/cpp -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../USB_DEVICE/App -I../FATFS/Target -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Middlewares/Third_Party/FatFs/src -Os -ffunction-sections -fdata-sections -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Core/cpp/TCallback.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/cpp/TCommon.o: ../Core/cpp/TCommon.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../FATFS/App -I../USB_DEVICE/Target -I../PDM2PCM/App -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Core/cpp -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../USB_DEVICE/App -I../FATFS/Target -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Middlewares/Third_Party/FatFs/src -Os -ffunction-sections -fdata-sections -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Core/cpp/TCommon.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/cpp/TFileSystem.o: ../Core/cpp/TFileSystem.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../FATFS/App -I../USB_DEVICE/Target -I../PDM2PCM/App -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Core/cpp -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../USB_DEVICE/App -I../FATFS/Target -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Middlewares/Third_Party/FatFs/src -Os -ffunction-sections -fdata-sections -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Core/cpp/TFileSystem.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/cpp/TI2C.o: ../Core/cpp/TI2C.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../FATFS/App -I../USB_DEVICE/Target -I../PDM2PCM/App -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Core/cpp -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../USB_DEVICE/App -I../FATFS/Target -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Middlewares/Third_Party/FatFs/src -Os -ffunction-sections -fdata-sections -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Core/cpp/TI2C.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/cpp/TPhoto.o: ../Core/cpp/TPhoto.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../FATFS/App -I../USB_DEVICE/Target -I../PDM2PCM/App -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Core/cpp -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../USB_DEVICE/App -I../FATFS/Target -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Middlewares/Third_Party/FatFs/src -Os -ffunction-sections -fdata-sections -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Core/cpp/TPhoto.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/cpp/TSdio.o: ../Core/cpp/TSdio.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../FATFS/App -I../USB_DEVICE/Target -I../PDM2PCM/App -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Core/cpp -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../USB_DEVICE/App -I../FATFS/Target -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Middlewares/Third_Party/FatFs/src -Os -ffunction-sections -fdata-sections -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Core/cpp/TSdio.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/cpp/TTag.o: ../Core/cpp/TTag.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../FATFS/App -I../USB_DEVICE/Target -I../PDM2PCM/App -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Core/cpp -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../USB_DEVICE/App -I../FATFS/Target -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Middlewares/Third_Party/FatFs/src -Os -ffunction-sections -fdata-sections -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Core/cpp/TTag.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/cpp/TUnit.o: ../Core/cpp/TUnit.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../FATFS/App -I../USB_DEVICE/Target -I../PDM2PCM/App -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Core/cpp -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../Drivers/CMSIS/Include -I../Core/Inc -I../USB_DEVICE/App -I../FATFS/Target -I../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -I../Middlewares/Third_Party/FatFs/src -Os -ffunction-sections -fdata-sections -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Core/cpp/TUnit.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

