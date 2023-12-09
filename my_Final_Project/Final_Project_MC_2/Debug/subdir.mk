################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Buzzer.c \
../DC_Motor.c \
../PWM.c \
../Timer_1.c \
../external_eeprom.c \
../gpio.c \
../project_MC_2.c \
../twi.c \
../uart.c 

OBJS += \
./Buzzer.o \
./DC_Motor.o \
./PWM.o \
./Timer_1.o \
./external_eeprom.o \
./gpio.o \
./project_MC_2.o \
./twi.o \
./uart.o 

C_DEPS += \
./Buzzer.d \
./DC_Motor.d \
./PWM.d \
./Timer_1.d \
./external_eeprom.d \
./gpio.d \
./project_MC_2.d \
./twi.d \
./uart.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


