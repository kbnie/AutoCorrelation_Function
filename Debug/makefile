################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := cs-rm -f

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include Cortex-M3-src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(HCT_DEPS)),)
-include $(HCT_DEPS)
endif
ifneq ($(strip $(CFS_DEPS)),)
-include $(CFS_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
toNVMM += \
All_Core_Passthrough_AutoCorrelation.cfe \

GCC_LINKER_IO_ELF_OUTPUTS += \
All_Core_Passthrough_AutoCorrelation.elf \


# All Target
all: All_Core_Passthrough_AutoCorrelation.nvi secondary-outputs

# Tool invocations
All_Core_Passthrough_AutoCorrelation.nvi: $(toNVMM)
	@echo 'Building target: $@'
	@echo 'Invoking: NVMM'
	nvmm -s SK4FS -t NVI -l 128kb -b "C:\PROGRA~2\ONSEMI~1\EZAIRO~1/source/cfx/bootloader/master/sk4_bootloader_cid102.cfe" "C:/Users/Henry Ong/workspace/All_Core_Passthrough_AutoCorrelation/Debug/All_Core_Passthrough_AutoCorrelation.cfe" -o "All_Core_Passthrough_AutoCorrelation.nvi"
	@echo 'Finished building target: $@'
	@echo ' '

All_Core_Passthrough_AutoCorrelation.cfe: $(CFS_OBJS)
	@echo 'Invoking: CFCC Linker'
	cfcc -bSK4 --toolchain=gnu -g --entry=main --map -LC:\PROGRA~2\ONSEMI~1\EZAIRO~1/lib/cfx -lsk4_eeprom.a -lsk4_sys.a -lsk4_math.a --mem0=0x8000/0xDBFF --mem0=0xDFE0/0xDFFF --mem0=0xE000/0xEFFF --mem0=0xF300/0xF312 --mem0=0xF800/0xF8BF --mem0=0xF900/0xF9FF --mem0=0x4000/0x4FFF --mem0=0x5000/0x7FFF --mem1=0x0120/0x2FFF --mem1=0x4000/0x67FF --mem1=0x8000/0xFFFF --mem2=0x0120/0x0FFF --mem2=0xC000/0xFFFF --mem3=0x0020/0x011F -o "All_Core_Passthrough_AutoCorrelation.cfe" $(CFS_OBJS)
	@echo 'Finished building: $@'
	@echo ' '

All_Core_Passthrough_AutoCorrelation.elf: $(C_OBJS)
	@echo 'Invoking: GCC Linker'
	arm-none-eabi-gcc $(C_OBJS) -mcpu=cortex-m3 -mthumb -msoft-float -l:sk4_cm3_sys.a -l:sk4_cm3_math.a -l:sk4_isr_defaults.a -LC:\PROGRA~2\ONSEMI~1\EZAIRO~1/lib/cm3 -g -Tsk4-cm3-ram.ld -o "All_Core_Passthrough_AutoCorrelation.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CFS_OBJS)$(C_DEPS)$(toNVMM)$(CFX_ROM_IMAGES___NVI__OUTPUTS)$(HCT_DEPS)$(GCC_LINKER_IO_ELF_OUTPUTS)$(CFX_RELOCATABLE_METADATA_FILE___ORC__OUTPUTS)$(C_OBJS)$(toAssembler)$(CFS_DEPS) All_Core_Passthrough_AutoCorrelation.nvi
	-@echo ' '

secondary-outputs: $(toNVMM)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
