################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include STM32F4xx_HAL_Drivers_Sources/subdir.mk
-include Nucleo-F429ZI_Sources/subdir.mk
-include FreeRTOS_Sources/subdir.mk
-include Demo_Sources/subdir.mk
-include CycloneTCP_Sources/subdir.mk
-include CycloneCrypto_Sources/subdir.mk
-include CycloneBOOT_Sources/subdir.mk
-include Common_Sources/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := http_server_iap_demo
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
http_server_iap_demo.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
http_server_iap_demo.list \

OBJCOPY_BIN += \
http_server_iap_demo.bin \


# All Target
all:
	+@$(MAKE) --no-print-directory pre-build && $(MAKE) --no-print-directory main-build && $(MAKE) --no-print-directory post-build

# Main-build Target
main-build: http_server_iap_demo.elf secondary-outputs

# Tool invocations
http_server_iap_demo.elf: $(OBJS) $(USER_OBJS) F:\DS_STM32F429\IAP\CycloneBOOT_IAP_Demo_Open_1_0_1\demo\st\nucleo_f429zi\http_server_iap_demo\stm32_cube_ide\stm32f429_flash.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "http_server_iap_demo.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"F:\DS_STM32F429\IAP\CycloneBOOT_IAP_Demo_Open_1_0_1\demo\st\nucleo_f429zi\http_server_iap_demo\stm32_cube_ide\stm32f429_flash.ld" --specs=nosys.specs -Wl,-Map="http_server_iap_demo.map" -Wl,--gc-sections -static  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

http_server_iap_demo.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "http_server_iap_demo.list"
	@echo 'Finished building: $@'
	@echo ' '

http_server_iap_demo.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "http_server_iap_demo.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(SIZE_OUTPUT)$(OBJDUMP_LIST)$(EXECUTABLES)$(OBJS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS)$(OBJCOPY_BIN) http_server_iap_demo.elf
	-@echo ' '

pre-build:
	F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../utils/ResourceCompiler/bin/rc.exe F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../resources/ F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../src/res.c
	-@echo ' '

post-build:
	wscript.exe F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../scripts/misc/copyFile.js F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/scenario_1_simple_app_and_integrity/http_server_iap_demo.bin F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../http_server_iap_demo.bin
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents fail-specified-linker-script-missing warn-no-linker-script-specified main-build pre-build post-build

-include ../makefile.targets
