################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/src/cmsis/CommonTables/arm_common_tables.c \
../system/src/cmsis/CommonTables/arm_const_structs.c 

OBJS += \
./system/src/cmsis/CommonTables/arm_common_tables.o \
./system/src/cmsis/CommonTables/arm_const_structs.o 

C_DEPS += \
./system/src/cmsis/CommonTables/arm_common_tables.d \
./system/src/cmsis/CommonTables/arm_const_structs.d 


# Each subdirectory must supply rules for building sources it contributes
system/src/cmsis/CommonTables/%.o: ../system/src/cmsis/CommonTables/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DTRACE -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/ADuCM350" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


