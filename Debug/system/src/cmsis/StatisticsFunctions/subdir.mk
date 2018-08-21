################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/src/cmsis/StatisticsFunctions/arm_max_f32.c \
../system/src/cmsis/StatisticsFunctions/arm_max_q15.c \
../system/src/cmsis/StatisticsFunctions/arm_max_q31.c \
../system/src/cmsis/StatisticsFunctions/arm_max_q7.c \
../system/src/cmsis/StatisticsFunctions/arm_mean_f32.c \
../system/src/cmsis/StatisticsFunctions/arm_mean_q15.c \
../system/src/cmsis/StatisticsFunctions/arm_mean_q31.c \
../system/src/cmsis/StatisticsFunctions/arm_mean_q7.c \
../system/src/cmsis/StatisticsFunctions/arm_min_f32.c \
../system/src/cmsis/StatisticsFunctions/arm_min_q15.c \
../system/src/cmsis/StatisticsFunctions/arm_min_q31.c \
../system/src/cmsis/StatisticsFunctions/arm_min_q7.c \
../system/src/cmsis/StatisticsFunctions/arm_power_f32.c \
../system/src/cmsis/StatisticsFunctions/arm_power_q15.c \
../system/src/cmsis/StatisticsFunctions/arm_power_q31.c \
../system/src/cmsis/StatisticsFunctions/arm_power_q7.c \
../system/src/cmsis/StatisticsFunctions/arm_rms_f32.c \
../system/src/cmsis/StatisticsFunctions/arm_rms_q15.c \
../system/src/cmsis/StatisticsFunctions/arm_rms_q31.c \
../system/src/cmsis/StatisticsFunctions/arm_std_f32.c \
../system/src/cmsis/StatisticsFunctions/arm_std_q15.c \
../system/src/cmsis/StatisticsFunctions/arm_std_q31.c \
../system/src/cmsis/StatisticsFunctions/arm_var_f32.c \
../system/src/cmsis/StatisticsFunctions/arm_var_q15.c \
../system/src/cmsis/StatisticsFunctions/arm_var_q31.c 

OBJS += \
./system/src/cmsis/StatisticsFunctions/arm_max_f32.o \
./system/src/cmsis/StatisticsFunctions/arm_max_q15.o \
./system/src/cmsis/StatisticsFunctions/arm_max_q31.o \
./system/src/cmsis/StatisticsFunctions/arm_max_q7.o \
./system/src/cmsis/StatisticsFunctions/arm_mean_f32.o \
./system/src/cmsis/StatisticsFunctions/arm_mean_q15.o \
./system/src/cmsis/StatisticsFunctions/arm_mean_q31.o \
./system/src/cmsis/StatisticsFunctions/arm_mean_q7.o \
./system/src/cmsis/StatisticsFunctions/arm_min_f32.o \
./system/src/cmsis/StatisticsFunctions/arm_min_q15.o \
./system/src/cmsis/StatisticsFunctions/arm_min_q31.o \
./system/src/cmsis/StatisticsFunctions/arm_min_q7.o \
./system/src/cmsis/StatisticsFunctions/arm_power_f32.o \
./system/src/cmsis/StatisticsFunctions/arm_power_q15.o \
./system/src/cmsis/StatisticsFunctions/arm_power_q31.o \
./system/src/cmsis/StatisticsFunctions/arm_power_q7.o \
./system/src/cmsis/StatisticsFunctions/arm_rms_f32.o \
./system/src/cmsis/StatisticsFunctions/arm_rms_q15.o \
./system/src/cmsis/StatisticsFunctions/arm_rms_q31.o \
./system/src/cmsis/StatisticsFunctions/arm_std_f32.o \
./system/src/cmsis/StatisticsFunctions/arm_std_q15.o \
./system/src/cmsis/StatisticsFunctions/arm_std_q31.o \
./system/src/cmsis/StatisticsFunctions/arm_var_f32.o \
./system/src/cmsis/StatisticsFunctions/arm_var_q15.o \
./system/src/cmsis/StatisticsFunctions/arm_var_q31.o 

C_DEPS += \
./system/src/cmsis/StatisticsFunctions/arm_max_f32.d \
./system/src/cmsis/StatisticsFunctions/arm_max_q15.d \
./system/src/cmsis/StatisticsFunctions/arm_max_q31.d \
./system/src/cmsis/StatisticsFunctions/arm_max_q7.d \
./system/src/cmsis/StatisticsFunctions/arm_mean_f32.d \
./system/src/cmsis/StatisticsFunctions/arm_mean_q15.d \
./system/src/cmsis/StatisticsFunctions/arm_mean_q31.d \
./system/src/cmsis/StatisticsFunctions/arm_mean_q7.d \
./system/src/cmsis/StatisticsFunctions/arm_min_f32.d \
./system/src/cmsis/StatisticsFunctions/arm_min_q15.d \
./system/src/cmsis/StatisticsFunctions/arm_min_q31.d \
./system/src/cmsis/StatisticsFunctions/arm_min_q7.d \
./system/src/cmsis/StatisticsFunctions/arm_power_f32.d \
./system/src/cmsis/StatisticsFunctions/arm_power_q15.d \
./system/src/cmsis/StatisticsFunctions/arm_power_q31.d \
./system/src/cmsis/StatisticsFunctions/arm_power_q7.d \
./system/src/cmsis/StatisticsFunctions/arm_rms_f32.d \
./system/src/cmsis/StatisticsFunctions/arm_rms_q15.d \
./system/src/cmsis/StatisticsFunctions/arm_rms_q31.d \
./system/src/cmsis/StatisticsFunctions/arm_std_f32.d \
./system/src/cmsis/StatisticsFunctions/arm_std_q15.d \
./system/src/cmsis/StatisticsFunctions/arm_std_q31.d \
./system/src/cmsis/StatisticsFunctions/arm_var_f32.d \
./system/src/cmsis/StatisticsFunctions/arm_var_q15.d \
./system/src/cmsis/StatisticsFunctions/arm_var_q31.d 


# Each subdirectory must supply rules for building sources it contributes
system/src/cmsis/StatisticsFunctions/%.o: ../system/src/cmsis/StatisticsFunctions/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DTRACE -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/ADuCM350" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


