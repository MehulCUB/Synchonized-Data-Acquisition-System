################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/Init_MMA.c \
../source/MMA8451Q.c \
../source/TPM.c \
../source/UART0.c \
../source/UART1.c \
../source/appmode.c \
../source/cap_touch.c \
../source/cbfifo.c \
../source/cli.c \
../source/i2c.c \
../source/led.c \
../source/main.c \
../source/mtb.c \
../source/semihost_hardfault.c \
../source/sysclock.c \
../source/systick.c 

OBJS += \
./source/Init_MMA.o \
./source/MMA8451Q.o \
./source/TPM.o \
./source/UART0.o \
./source/UART1.o \
./source/appmode.o \
./source/cap_touch.o \
./source/cbfifo.o \
./source/cli.o \
./source/i2c.o \
./source/led.o \
./source/main.o \
./source/mtb.o \
./source/semihost_hardfault.o \
./source/sysclock.o \
./source/systick.o 

C_DEPS += \
./source/Init_MMA.d \
./source/MMA8451Q.d \
./source/TPM.d \
./source/UART0.d \
./source/UART1.d \
./source/appmode.d \
./source/cap_touch.d \
./source/cbfifo.d \
./source/cli.d \
./source/i2c.d \
./source/led.d \
./source/main.d \
./source/mtb.d \
./source/semihost_hardfault.d \
./source/sysclock.d \
./source/systick.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"D:\Final_Project_ESD_Mehul\Final_Project_ESD_frdm_KL25z_Mehul\board" -I"D:\Final_Project_ESD_Mehul\Final_Project_ESD_frdm_KL25z_Mehul\source" -I"D:\Final_Project_ESD_Mehul\Final_Project_ESD_frdm_KL25z_Mehul" -I"D:\Final_Project_ESD_Mehul\Final_Project_ESD_frdm_KL25z_Mehul\startup" -I"D:\Final_Project_ESD_Mehul\Final_Project_ESD_frdm_KL25z_Mehul\CMSIS" -I"D:\Final_Project_ESD_Mehul\Final_Project_ESD_frdm_KL25z_Mehul\drivers" -I"D:\Final_Project_ESD_Mehul\Final_Project_ESD_frdm_KL25z_Mehul\utilities" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


