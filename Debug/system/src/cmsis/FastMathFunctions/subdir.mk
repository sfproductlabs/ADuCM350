################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/src/cmsis/FastMathFunctions/arm_cos_f32.c \
../system/src/cmsis/FastMathFunctions/arm_cos_q15.c \
../system/src/cmsis/FastMathFunctions/arm_cos_q31.c \
../system/src/cmsis/FastMathFunctions/arm_sin_f32.c \
../system/src/cmsis/FastMathFunctions/arm_sin_q15.c \
../system/src/cmsis/FastMathFunctions/arm_sin_q31.c \
../system/src/cmsis/FastMathFunctions/arm_sqrt_q15.c \
../system/src/cmsis/FastMathFunctions/arm_sqrt_q31.c 

OBJS += \
./system/src/cmsis/FastMathFunctions/arm_cos_f32.o \
./system/src/cmsis/FastMathFunctions/arm_cos_q15.o \
./system/src/cmsis/FastMathFunctions/arm_cos_q31.o \
./system/src/cmsis/FastMathFunctions/arm_sin_f32.o \
./system/src/cmsis/FastMathFunctions/arm_sin_q15.o \
./system/src/cmsis/FastMathFunctions/arm_sin_q31.o \
./system/src/cmsis/FastMathFunctions/arm_sqrt_q15.o \
./system/src/cmsis/FastMathFunctions/arm_sqrt_q31.o 

C_DEPS += \
./system/src/cmsis/FastMathFunctions/arm_cos_f32.d \
./system/src/cmsis/FastMathFunctions/arm_cos_q15.d \
./system/src/cmsis/FastMathFunctions/arm_cos_q31.d \
./system/src/cmsis/FastMathFunctions/arm_sin_f32.d \
./system/src/cmsis/FastMathFunctions/arm_sin_q15.d \
./system/src/cmsis/FastMathFunctions/arm_sin_q31.d \
./system/src/cmsis/FastMathFunctions/arm_sqrt_q15.d \
./system/src/cmsis/FastMathFunctions/arm_sqrt_q31.d 


# Each subdirectory must supply rules for building sources it contributes
system/src/cmsis/FastMathFunctions/%.o: ../system/src/cmsis/FastMathFunctions/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DTRACE -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/ADuCM350" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


