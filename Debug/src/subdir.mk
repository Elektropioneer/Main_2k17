################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/actuators.c \
../src/blueside.c \
../src/can.c \
../src/gpio.c \
../src/odometry.c \
../src/system.c \
../src/uart.c \
../src/yellowside.c 

OBJS += \
./src/actuators.o \
./src/blueside.o \
./src/can.o \
./src/gpio.o \
./src/odometry.o \
./src/system.o \
./src/uart.o \
./src/yellowside.o 

C_DEPS += \
./src/actuators.d \
./src/blueside.d \
./src/can.d \
./src/gpio.d \
./src/odometry.d \
./src/system.d \
./src/uart.d \
./src/yellowside.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/home/silard_g/workspace/Back2Reality/inc" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=at90can128 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


