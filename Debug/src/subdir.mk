################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/AfeTest.c \
../src/BlinkyTest.c \
../src/FlashTest.c \
../src/PinMux.c \
../src/Timer.c \
../src/UartEcho.c \
../src/_write.c \
../src/crc32.c \
../src/main.c \
../src/test_common.c 

OBJS += \
./src/AfeTest.o \
./src/BlinkyTest.o \
./src/FlashTest.o \
./src/PinMux.o \
./src/Timer.o \
./src/UartEcho.o \
./src/_write.o \
./src/crc32.o \
./src/main.o \
./src/test_common.o 

C_DEPS += \
./src/AfeTest.d \
./src/BlinkyTest.d \
./src/FlashTest.d \
./src/PinMux.d \
./src/Timer.d \
./src/UartEcho.d \
./src/_write.d \
./src/crc32.d \
./src/main.d \
./src/test_common.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DTRACE -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/ADuCM350" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


