################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/chan-fatfs/Inc/ffsystem.c 

OBJS += \
./Drivers/chan-fatfs/Inc/ffsystem.o 

C_DEPS += \
./Drivers/chan-fatfs/Inc/ffsystem.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/chan-fatfs/Inc/%.o: ../Drivers/chan-fatfs/Inc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F407xx -I"C:/Users/haru/workspace/fff/Inc" -I"C:/Users/haru/workspace/fff/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/haru/workspace/fff/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/haru/workspace/fff/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/haru/workspace/fff/Drivers/CMSIS/Include" -I"C:/Users/haru/workspace/fff/Drivers/chan-fatfs/Inc" -I"C:/Users/haru/workspace/fff/Drivers/ILI9488/Inc"  -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


