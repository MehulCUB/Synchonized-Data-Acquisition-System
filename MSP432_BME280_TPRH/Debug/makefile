################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs1020/ccs/tools/compiler/ti-cgt-arm_20.2.2.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./ccs/startup_msp432p401r_ccs.obj" \
"./source/RTC.obj" \
"./source/UART0.obj" \
"./source/UART2.obj" \
"./source/app_mode.obj" \
"./source/bme280.obj" \
"./source/bme280_support.obj" \
"./source/cbfifo.obj" \
"./source/cli.obj" \
"./source/demo_sysctl.obj" \
"./source/i2c_driver.obj" \
"./source/main.obj" \
"./source/system_clock.obj" \
"./source/system_msp432p401r.obj" \
"./source/systick.obj" \
"../msp432p401r.cmd" \
$(GEN_CMDS__FLAG) \
-l"ti/display/lib/display.aem4f" \
-l"ti/grlib/lib/ccs/m4f/grlib.a" \
-l"third_party/spiffs/lib/ccs/m4f/spiffs.a" \
-l"ti/drivers/lib/drivers_msp432p401x.aem4f" \
-l"third_party/fatfs/lib/ccs/m4f/fatfs.a" \
-l"ti/devices/msp432p4xx/driverlib/ccs/msp432p4xx_driverlib.lib" \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include ccs/subdir_vars.mk
-include source/subdir_vars.mk
-include subdir_rules.mk
-include ccs/subdir_rules.mk
-include source/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
Final_Project_MSP432_Mehul.out \

EXE_OUTPUTS__QUOTED += \
"Final_Project_MSP432_Mehul.out" \

BIN_OUTPUTS += \
Final_Project_MSP432_Mehul.hex \

BIN_OUTPUTS__QUOTED += \
"Final_Project_MSP432_Mehul.hex" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "Final_Project_MSP432_Mehul.out"

# Tool invocations
Final_Project_MSP432_Mehul.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: Arm Linker'
	"C:/ti/ccs1020/ccs/tools/compiler/ti-cgt-arm_20.2.2.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --advice:power=none --define=__MSP432P401R__ --define=DeviceFamily_MSP432P401x -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on -z -m"Final_Project_MSP432_Mehul.map" --stack_size=512 -i"C:/ti/simplelink_msp432p4_sdk_3_40_01_02/source" -i"C:/ti/ccs1020/ccs/tools/compiler/ti-cgt-arm_20.2.2.LTS/lib" --diag_wrap=off --display_error_number --warn_sections --xml_link_info="Final_Project_MSP432_Mehul_linkInfo.xml" --rom_model -o "Final_Project_MSP432_Mehul.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

Final_Project_MSP432_Mehul.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: Arm Hex Utility'
	"C:/ti/ccs1020/ccs/tools/compiler/ti-cgt-arm_20.2.2.LTS/bin/armhex" --diag_wrap=off -o "Final_Project_MSP432_Mehul.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "ccs\startup_msp432p401r_ccs.obj" "source\RTC.obj" "source\UART0.obj" "source\UART2.obj" "source\app_mode.obj" "source\bme280.obj" "source\bme280_support.obj" "source\cbfifo.obj" "source\cli.obj" "source\demo_sysctl.obj" "source\i2c_driver.obj" "source\main.obj" "source\system_clock.obj" "source\system_msp432p401r.obj" "source\systick.obj" 
	-$(RM) "ccs\startup_msp432p401r_ccs.d" "source\RTC.d" "source\UART0.d" "source\UART2.d" "source\app_mode.d" "source\bme280.d" "source\bme280_support.d" "source\cbfifo.d" "source\cli.d" "source\demo_sysctl.d" "source\i2c_driver.d" "source\main.d" "source\system_clock.d" "source\system_msp432p401r.d" "source\systick.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

