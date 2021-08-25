################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/src/debug.c \
F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/src/http_server_callbacks.c \
F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/src/main.c \
F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/src/res.c \
F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/src/stm32f4xx_it.c \
F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/src/system_stm32f4xx.c 

OBJS += \
./Demo_Sources/debug.o \
./Demo_Sources/http_server_callbacks.o \
./Demo_Sources/main.o \
./Demo_Sources/res.o \
./Demo_Sources/stm32f4xx_it.o \
./Demo_Sources/system_stm32f4xx.o 

C_DEPS += \
./Demo_Sources/debug.d \
./Demo_Sources/http_server_callbacks.d \
./Demo_Sources/main.d \
./Demo_Sources/res.d \
./Demo_Sources/stm32f4xx_it.d \
./Demo_Sources/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Demo_Sources/debug.o: F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/src/debug.c Demo_Sources/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F429xx -DUSE_HAL_DRIVER -DUSE_STM32F4XX_NUCLEO_144 -DIAP_SCENARIO_1 -D_WINSOCK_H -c -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../src" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../third_party/cmsis/include" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../third_party/st/devices/stm32f4xx" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../third_party/st/drivers/stm32f4xx_hal_driver/inc" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../third_party/st/boards/stm32f4xx_nucleo_144" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../third_party/freertos/include" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../third_party/freertos/portable/gcc/arm_cm4f" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../common" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../cyclone_tcp" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../cyclone_ssl" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../cyclone_crypto" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../cyclone_boot" -O0 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Demo_Sources/http_server_callbacks.o: F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/src/http_server_callbacks.c Demo_Sources/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F429xx -DUSE_HAL_DRIVER -DUSE_STM32F4XX_NUCLEO_144 -DIAP_SCENARIO_1 -D_WINSOCK_H -c -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../src" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../third_party/cmsis/include" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../third_party/st/devices/stm32f4xx" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../third_party/st/drivers/stm32f4xx_hal_driver/inc" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../third_party/st/boards/stm32f4xx_nucleo_144" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../third_party/freertos/include" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../third_party/freertos/portable/gcc/arm_cm4f" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../common" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../cyclone_tcp" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../cyclone_ssl" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../cyclone_crypto" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../cyclone_boot" -O0 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Demo_Sources/main.o: F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/src/main.c Demo_Sources/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F429xx -DUSE_HAL_DRIVER -DUSE_STM32F4XX_NUCLEO_144 -DIAP_SCENARIO_1 -D_WINSOCK_H -c -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../src" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../third_party/cmsis/include" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../third_party/st/devices/stm32f4xx" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../third_party/st/drivers/stm32f4xx_hal_driver/inc" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../third_party/st/boards/stm32f4xx_nucleo_144" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../third_party/freertos/include" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../third_party/freertos/portable/gcc/arm_cm4f" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../common" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../cyclone_tcp" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../cyclone_ssl" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../cyclone_crypto" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../cyclone_boot" -O0 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Demo_Sources/res.o: F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/src/res.c Demo_Sources/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F429xx -DUSE_HAL_DRIVER -DUSE_STM32F4XX_NUCLEO_144 -DIAP_SCENARIO_1 -D_WINSOCK_H -c -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../src" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../third_party/cmsis/include" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../third_party/st/devices/stm32f4xx" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../third_party/st/drivers/stm32f4xx_hal_driver/inc" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../third_party/st/boards/stm32f4xx_nucleo_144" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../third_party/freertos/include" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../third_party/freertos/portable/gcc/arm_cm4f" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../common" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../cyclone_tcp" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../cyclone_ssl" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../cyclone_crypto" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../cyclone_boot" -O0 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Demo_Sources/stm32f4xx_it.o: F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/src/stm32f4xx_it.c Demo_Sources/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F429xx -DUSE_HAL_DRIVER -DUSE_STM32F4XX_NUCLEO_144 -DIAP_SCENARIO_1 -D_WINSOCK_H -c -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../src" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../third_party/cmsis/include" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../third_party/st/devices/stm32f4xx" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../third_party/st/drivers/stm32f4xx_hal_driver/inc" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../third_party/st/boards/stm32f4xx_nucleo_144" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../third_party/freertos/include" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../third_party/freertos/portable/gcc/arm_cm4f" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../common" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../cyclone_tcp" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../cyclone_ssl" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../cyclone_crypto" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../cyclone_boot" -O0 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Demo_Sources/system_stm32f4xx.o: F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/src/system_stm32f4xx.c Demo_Sources/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F429xx -DUSE_HAL_DRIVER -DUSE_STM32F4XX_NUCLEO_144 -DIAP_SCENARIO_1 -D_WINSOCK_H -c -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../src" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../third_party/cmsis/include" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../third_party/st/devices/stm32f4xx" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../third_party/st/drivers/stm32f4xx_hal_driver/inc" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../third_party/st/boards/stm32f4xx_nucleo_144" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../third_party/freertos/include" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../third_party/freertos/portable/gcc/arm_cm4f" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../common" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../cyclone_tcp" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../cyclone_ssl" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../cyclone_crypto" -I"F:/DS_STM32F429/IAP/CycloneBOOT_IAP_Demo_Open_1_0_1/demo/st/nucleo_f429zi/http_server_iap_demo/stm32_cube_ide/../../../../../cyclone_boot" -O0 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
