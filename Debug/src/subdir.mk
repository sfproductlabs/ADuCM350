################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/ImpedanceMeasurement_2Wire.c \
../src/Timer.c \
../src/_write.c \
../src/main.c \
../src/test_common.c 

OBJS += \
./src/ImpedanceMeasurement_2Wire.o \
./src/Timer.o \
./src/_write.o \
./src/main.o \
./src/test_common.o 

C_DEPS += \
./src/ImpedanceMeasurement_2Wire.d \
./src/Timer.d \
./src/_write.d \
./src/main.d \
./src/test_common.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DTRACE -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/ADuCM350" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


