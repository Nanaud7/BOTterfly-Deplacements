################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/BOTterfly-C/ctrl_positionV2.c \
../Core/Src/BOTterfly-C/ctrl_speed.c \
../Core/Src/BOTterfly-C/encoder.c \
../Core/Src/BOTterfly-C/motor.c \
../Core/Src/BOTterfly-C/odometry.c \
../Core/Src/BOTterfly-C/shellOS.c \
../Core/Src/BOTterfly-C/strategy.c 

OBJS += \
./Core/Src/BOTterfly-C/ctrl_positionV2.o \
./Core/Src/BOTterfly-C/ctrl_speed.o \
./Core/Src/BOTterfly-C/encoder.o \
./Core/Src/BOTterfly-C/motor.o \
./Core/Src/BOTterfly-C/odometry.o \
./Core/Src/BOTterfly-C/shellOS.o \
./Core/Src/BOTterfly-C/strategy.o 

C_DEPS += \
./Core/Src/BOTterfly-C/ctrl_positionV2.d \
./Core/Src/BOTterfly-C/ctrl_speed.d \
./Core/Src/BOTterfly-C/encoder.d \
./Core/Src/BOTterfly-C/motor.d \
./Core/Src/BOTterfly-C/odometry.d \
./Core/Src/BOTterfly-C/shellOS.d \
./Core/Src/BOTterfly-C/strategy.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/BOTterfly-C/%.o: ../Core/Src/BOTterfly-C/%.c Core/Src/BOTterfly-C/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

