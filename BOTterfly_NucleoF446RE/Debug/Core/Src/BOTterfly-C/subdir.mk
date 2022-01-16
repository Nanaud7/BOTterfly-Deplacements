################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/BOTterfly-C/direction.c \
../Core/Src/BOTterfly-C/photodiodes.c \
../Core/Src/BOTterfly-C/rgb_sensor.c \
../Core/Src/BOTterfly-C/tof_sensor.c 

OBJS += \
./Core/Src/BOTterfly-C/direction.o \
./Core/Src/BOTterfly-C/photodiodes.o \
./Core/Src/BOTterfly-C/rgb_sensor.o \
./Core/Src/BOTterfly-C/tof_sensor.o 

C_DEPS += \
./Core/Src/BOTterfly-C/direction.d \
./Core/Src/BOTterfly-C/photodiodes.d \
./Core/Src/BOTterfly-C/rgb_sensor.d \
./Core/Src/BOTterfly-C/tof_sensor.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/BOTterfly-C/%.o: ../Core/Src/BOTterfly-C/%.c Core/Src/BOTterfly-C/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Git Folder/BOTterfly_NucleoF446RE/BOTterfly_NucleoF446RE/Drivers/VL53L0X_API/core/inc" -I"C:/Git Folder/BOTterfly_NucleoF446RE/BOTterfly_NucleoF446RE/Drivers/VL53L0X_API/platform/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

