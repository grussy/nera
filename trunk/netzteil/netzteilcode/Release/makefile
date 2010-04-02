################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
netzteilcode.lss \

FLASH_IMAGE += \
netzteilcode.hex \

EEPROM_IMAGE += \
netzteilcode.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: netzteilcode.elf secondary-outputs

# Tool invocations
netzteilcode.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,netzteilcode.map -mmcu=atmega32 -o"netzteilcode.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

netzteilcode.lss: netzteilcode.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S netzteilcode.elf  >"netzteilcode.lss"
	@echo 'Finished building: $@'
	@echo ' '

netzteilcode.hex: netzteilcode.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex netzteilcode.elf  "netzteilcode.hex"
	@echo 'Finished building: $@'
	@echo ' '

netzteilcode.eep: netzteilcode.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex netzteilcode.elf  "netzteilcode.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: netzteilcode.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 netzteilcode.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) netzteilcode.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
