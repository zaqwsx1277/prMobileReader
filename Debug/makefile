################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include USB_DEVICE/Target/subdir.mk
-include USB_DEVICE/App/subdir.mk
-include PDM2PCM/App/subdir.mk
-include Middlewares/Third_Party/FatFs/src/option/subdir.mk
-include Middlewares/Third_Party/FatFs/src/subdir.mk
-include Middlewares/ST/STM32_USB_Device_Library/Core/Src/subdir.mk
-include Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/subdir.mk
-include FATFS/Target/subdir.mk
-include FATFS/App/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Core/cpp/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
ptMobileRederVIT_cpp.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
ptMobileRederVIT_cpp.list \

OBJCOPY_BIN += \
ptMobileRederVIT_cpp.bin \


# All Target
all: ptMobileRederVIT_cpp.elf secondary-outputs

# Tool invocations
ptMobileRederVIT_cpp.elf: $(OBJS) $(USER_OBJS) /home/energia/prWork/prSTM32/prMobileReader/ptMobileRederVIT_cpp/STM32F429VITX_FLASH.ld
	arm-none-eabi-g++ -o "ptMobileRederVIT_cpp.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"/home/energia/prWork/prSTM32/prMobileReader/ptMobileRederVIT_cpp/STM32F429VITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="ptMobileRederVIT_cpp.map" -Wl,--gc-sections -static -L../Middlewares/ST/STM32_Audio/Addons/PDM/Lib --specs=standard_c_nano_cpp.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

ptMobileRederVIT_cpp.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "ptMobileRederVIT_cpp.list"
	@echo 'Finished building: $@'
	@echo ' '

ptMobileRederVIT_cpp.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "ptMobileRederVIT_cpp.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
