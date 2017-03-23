################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
Back2Reality.lss \

FLASH_IMAGE += \
Back2Reality.hex \

SIZEDUMMY += \
sizedummy \


# All Target
all: Back2Reality.elf secondary-outputs

# Tool invocations
Back2Reality.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,Back2Reality.map -mmcu=at90can128 -o "Back2Reality.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

Back2Reality.lss: Back2Reality.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S Back2Reality.elf  >"Back2Reality.lss"
	@echo 'Finished building: $@'
	@echo ' '

Back2Reality.hex: Back2Reality.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex Back2Reality.elf  "Back2Reality.hex"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: Back2Reality.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=at90can128 Back2Reality.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) Back2Reality.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
