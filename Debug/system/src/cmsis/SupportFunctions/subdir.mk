################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/src/cmsis/SupportFunctions/arm_copy_f32.c \
../system/src/cmsis/SupportFunctions/arm_copy_q15.c \
../system/src/cmsis/SupportFunctions/arm_copy_q31.c \
../system/src/cmsis/SupportFunctions/arm_copy_q7.c \
../system/src/cmsis/SupportFunctions/arm_fill_f32.c \
../system/src/cmsis/SupportFunctions/arm_fill_q15.c \
../system/src/cmsis/SupportFunctions/arm_fill_q31.c \
../system/src/cmsis/SupportFunctions/arm_fill_q7.c \
../system/src/cmsis/SupportFunctions/arm_float_to_q15.c \
../system/src/cmsis/SupportFunctions/arm_float_to_q31.c \
../system/src/cmsis/SupportFunctions/arm_float_to_q7.c \
../system/src/cmsis/SupportFunctions/arm_q15_to_float.c \
../system/src/cmsis/SupportFunctions/arm_q15_to_q31.c \
../system/src/cmsis/SupportFunctions/arm_q15_to_q7.c \
../system/src/cmsis/SupportFunctions/arm_q31_to_float.c \
../system/src/cmsis/SupportFunctions/arm_q31_to_q15.c \
../system/src/cmsis/SupportFunctions/arm_q31_to_q7.c \
../system/src/cmsis/SupportFunctions/arm_q7_to_float.c \
../system/src/cmsis/SupportFunctions/arm_q7_to_q15.c \
../system/src/cmsis/SupportFunctions/arm_q7_to_q31.c 

OBJS += \
./system/src/cmsis/SupportFunctions/arm_copy_f32.o \
./system/src/cmsis/SupportFunctions/arm_copy_q15.o \
./system/src/cmsis/SupportFunctions/arm_copy_q31.o \
./system/src/cmsis/SupportFunctions/arm_copy_q7.o \
./system/src/cmsis/SupportFunctions/arm_fill_f32.o \
./system/src/cmsis/SupportFunctions/arm_fill_q15.o \
./system/src/cmsis/SupportFunctions/arm_fill_q31.o \
./system/src/cmsis/SupportFunctions/arm_fill_q7.o \
./system/src/cmsis/SupportFunctions/arm_float_to_q15.o \
./system/src/cmsis/SupportFunctions/arm_float_to_q31.o \
./system/src/cmsis/SupportFunctions/arm_float_to_q7.o \
./system/src/cmsis/SupportFunctions/arm_q15_to_float.o \
./system/src/cmsis/SupportFunctions/arm_q15_to_q31.o \
./system/src/cmsis/SupportFunctions/arm_q15_to_q7.o \
./system/src/cmsis/SupportFunctions/arm_q31_to_float.o \
./system/src/cmsis/SupportFunctions/arm_q31_to_q15.o \
./system/src/cmsis/SupportFunctions/arm_q31_to_q7.o \
./system/src/cmsis/SupportFunctions/arm_q7_to_float.o \
./system/src/cmsis/SupportFunctions/arm_q7_to_q15.o \
./system/src/cmsis/SupportFunctions/arm_q7_to_q31.o 

C_DEPS += \
./system/src/cmsis/SupportFunctions/arm_copy_f32.d \
./system/src/cmsis/SupportFunctions/arm_copy_q15.d \
./system/src/cmsis/SupportFunctions/arm_copy_q31.d \
./system/src/cmsis/SupportFunctions/arm_copy_q7.d \
./system/src/cmsis/SupportFunctions/arm_fill_f32.d \
./system/src/cmsis/SupportFunctions/arm_fill_q15.d \
./system/src/cmsis/SupportFunctions/arm_fill_q31.d \
./system/src/cmsis/SupportFunctions/arm_fill_q7.d \
./system/src/cmsis/SupportFunctions/arm_float_to_q15.d \
./system/src/cmsis/SupportFunctions/arm_float_to_q31.d \
./system/src/cmsis/SupportFunctions/arm_float_to_q7.d \
./system/src/cmsis/SupportFunctions/arm_q15_to_float.d \
./system/src/cmsis/SupportFunctions/arm_q15_to_q31.d \
./system/src/cmsis/SupportFunctions/arm_q15_to_q7.d \
./system/src/cmsis/SupportFunctions/arm_q31_to_float.d \
./system/src/cmsis/SupportFunctions/arm_q31_to_q15.d \
./system/src/cmsis/SupportFunctions/arm_q31_to_q7.d \
./system/src/cmsis/SupportFunctions/arm_q7_to_float.d \
./system/src/cmsis/SupportFunctions/arm_q7_to_q15.d \
./system/src/cmsis/SupportFunctions/arm_q7_to_q31.d 


# Each subdirectory must supply rules for building sources it contributes
system/src/cmsis/SupportFunctions/%.o: ../system/src/cmsis/SupportFunctions/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DTRACE -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/ADuCM350" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


