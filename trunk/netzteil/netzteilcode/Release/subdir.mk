################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../adc.c \
../commands.c \
../drehgeber.c \
../lcd.c \
../main.c \
../segment.c \
../uart.c 

OBJS += \
./adc.o \
./commands.o \
./drehgeber.o \
./lcd.o \
./main.o \
./segment.o \
./uart.o 

C_DEPS += \
./adc.d \
./commands.d \
./drehgeber.d \
./lcd.d \
./main.d \
./segment.d \
./uart.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


