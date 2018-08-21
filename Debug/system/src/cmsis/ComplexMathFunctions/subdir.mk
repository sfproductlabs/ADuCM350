################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/src/cmsis/ComplexMathFunctions/arm_cmplx_conj_f32.c \
../system/src/cmsis/ComplexMathFunctions/arm_cmplx_conj_q15.c \
../system/src/cmsis/ComplexMathFunctions/arm_cmplx_conj_q31.c \
../system/src/cmsis/ComplexMathFunctions/arm_cmplx_dot_prod_f32.c \
../system/src/cmsis/ComplexMathFunctions/arm_cmplx_dot_prod_q15.c \
../system/src/cmsis/ComplexMathFunctions/arm_cmplx_dot_prod_q31.c \
../system/src/cmsis/ComplexMathFunctions/arm_cmplx_mag_f32.c \
../system/src/cmsis/ComplexMathFunctions/arm_cmplx_mag_q15.c \
../system/src/cmsis/ComplexMathFunctions/arm_cmplx_mag_q31.c \
../system/src/cmsis/ComplexMathFunctions/arm_cmplx_mag_squared_f32.c \
../system/src/cmsis/ComplexMathFunctions/arm_cmplx_mag_squared_q15.c \
../system/src/cmsis/ComplexMathFunctions/arm_cmplx_mag_squared_q31.c \
../system/src/cmsis/ComplexMathFunctions/arm_cmplx_mult_cmplx_f32.c \
../system/src/cmsis/ComplexMathFunctions/arm_cmplx_mult_cmplx_q15.c \
../system/src/cmsis/ComplexMathFunctions/arm_cmplx_mult_cmplx_q31.c \
../system/src/cmsis/ComplexMathFunctions/arm_cmplx_mult_real_f32.c \
../system/src/cmsis/ComplexMathFunctions/arm_cmplx_mult_real_q15.c \
../system/src/cmsis/ComplexMathFunctions/arm_cmplx_mult_real_q31.c 

OBJS += \
./system/src/cmsis/ComplexMathFunctions/arm_cmplx_conj_f32.o \
./system/src/cmsis/ComplexMathFunctions/arm_cmplx_conj_q15.o \
./system/src/cmsis/ComplexMathFunctions/arm_cmplx_conj_q31.o \
./system/src/cmsis/ComplexMathFunctions/arm_cmplx_dot_prod_f32.o \
./system/src/cmsis/ComplexMathFunctions/arm_cmplx_dot_prod_q15.o \
./system/src/cmsis/ComplexMathFunctions/arm_cmplx_dot_prod_q31.o \
./system/src/cmsis/ComplexMathFunctions/arm_cmplx_mag_f32.o \
./system/src/cmsis/ComplexMathFunctions/arm_cmplx_mag_q15.o \
./system/src/cmsis/ComplexMathFunctions/arm_cmplx_mag_q31.o \
./system/src/cmsis/ComplexMathFunctions/arm_cmplx_mag_squared_f32.o \
./system/src/cmsis/ComplexMathFunctions/arm_cmplx_mag_squared_q15.o \
./system/src/cmsis/ComplexMathFunctions/arm_cmplx_mag_squared_q31.o \
./system/src/cmsis/ComplexMathFunctions/arm_cmplx_mult_cmplx_f32.o \
./system/src/cmsis/ComplexMathFunctions/arm_cmplx_mult_cmplx_q15.o \
./system/src/cmsis/ComplexMathFunctions/arm_cmplx_mult_cmplx_q31.o \
./system/src/cmsis/ComplexMathFunctions/arm_cmplx_mult_real_f32.o \
./system/src/cmsis/ComplexMathFunctions/arm_cmplx_mult_real_q15.o \
./system/src/cmsis/ComplexMathFunctions/arm_cmplx_mult_real_q31.o 

C_DEPS += \
./system/src/cmsis/ComplexMathFunctions/arm_cmplx_conj_f32.d \
./system/src/cmsis/ComplexMathFunctions/arm_cmplx_conj_q15.d \
./system/src/cmsis/ComplexMathFunctions/arm_cmplx_conj_q31.d \
./system/src/cmsis/ComplexMathFunctions/arm_cmplx_dot_prod_f32.d \
./system/src/cmsis/ComplexMathFunctions/arm_cmplx_dot_prod_q15.d \
./system/src/cmsis/ComplexMathFunctions/arm_cmplx_dot_prod_q31.d \
./system/src/cmsis/ComplexMathFunctions/arm_cmplx_mag_f32.d \
./system/src/cmsis/ComplexMathFunctions/arm_cmplx_mag_q15.d \
./system/src/cmsis/ComplexMathFunctions/arm_cmplx_mag_q31.d \
./system/src/cmsis/ComplexMathFunctions/arm_cmplx_mag_squared_f32.d \
./system/src/cmsis/ComplexMathFunctions/arm_cmplx_mag_squared_q15.d \
./system/src/cmsis/ComplexMathFunctions/arm_cmplx_mag_squared_q31.d \
./system/src/cmsis/ComplexMathFunctions/arm_cmplx_mult_cmplx_f32.d \
./system/src/cmsis/ComplexMathFunctions/arm_cmplx_mult_cmplx_q15.d \
./system/src/cmsis/ComplexMathFunctions/arm_cmplx_mult_cmplx_q31.d \
./system/src/cmsis/ComplexMathFunctions/arm_cmplx_mult_real_f32.d \
./system/src/cmsis/ComplexMathFunctions/arm_cmplx_mult_real_q15.d \
./system/src/cmsis/ComplexMathFunctions/arm_cmplx_mult_real_q31.d 


# Each subdirectory must supply rules for building sources it contributes
system/src/cmsis/ComplexMathFunctions/%.o: ../system/src/cmsis/ComplexMathFunctions/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DTRACE -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/ADuCM350" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


