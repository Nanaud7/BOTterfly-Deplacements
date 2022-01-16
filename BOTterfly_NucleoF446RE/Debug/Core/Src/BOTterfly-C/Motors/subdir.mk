################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/BOTterfly-C/Motors/ctrl_positionV1.c \
../Core/Src/BOTterfly-C/Motors/ctrl_speed.c \
../Core/Src/BOTterfly-C/Motors/encoder.c \
../Core/Src/BOTterfly-C/Motors/motor.c \
../Core/Src/BOTterfly-C/Motors/odometry.c \
../Core/Src/BOTterfly-C/Motors/shellOS.c \
../Core/Src/BOTterfly-C/Motors/strategy.c 

OBJS += \
./Core/Src/BOTterfly-C/Motors/ctrl_positionV1.o \
./Core/Src/BOTterfly-C/Motors/ctrl_speed.o \
./Core/Src/BOTterfly-C/Motors/encoder.o \
./Core/Src/BOTterfly-C/Motors/motor.o \
./Core/Src/BOTterfly-C/Motors/odometry.o \
./Core/Src/BOTterfly-C/Motors/shellOS.o \
./Core/Src/BOTterfly-C/Motors/strategy.o 

C_DEPS += \
./Core/Src/BOTterfly-C/Motors/ctrl_positionV1.d \
./Core/Src/BOTterfly-C/Motors/ctrl_speed.d \
./Core/Src/BOTterfly-C/Motors/encoder.d \
./Core/Src/BOTterfly-C/Motors/motor.d \
./Core/Src/BOTterfly-C/Motors/odometry.d \
./Core/Src/BOTterfly-C/Motors/shellOS.d \
./Core/Src/BOTterfly-C/Motors/strategy.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/BOTterfly-C/Motors/%.o: ../Core/Src/BOTterfly-C/Motors/%.c Core/Src/BOTterfly-C/Motors/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Git Folder/BOTterfly_NucleoF446RE/BOTterfly_NucleoF446RE/Drivers/VL53L0X_API/core/inc" -I"C:/Git Folder/BOTterfly_NucleoF446RE/BOTterfly_NucleoF446RE/Drivers/VL53L0X_API/platform/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

