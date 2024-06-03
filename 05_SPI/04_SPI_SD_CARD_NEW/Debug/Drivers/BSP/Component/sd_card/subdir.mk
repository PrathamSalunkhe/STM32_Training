################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/Component/sd_card/fatfs_sd.c 

OBJS += \
./Drivers/BSP/Component/sd_card/fatfs_sd.o 

C_DEPS += \
./Drivers/BSP/Component/sd_card/fatfs_sd.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Component/sd_card/%.o Drivers/BSP/Component/sd_card/%.su Drivers/BSP/Component/sd_card/%.cyclo: ../Drivers/BSP/Component/sd_card/%.c Drivers/BSP/Component/sd_card/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L4A6xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"/home/dnk066/Pratham/STM32_Training/Workspace/05_SPI/04_SPI_SD_CARD_NEW/Drivers/BSP/Component/sd_card" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-Component-2f-sd_card

clean-Drivers-2f-BSP-2f-Component-2f-sd_card:
	-$(RM) ./Drivers/BSP/Component/sd_card/fatfs_sd.cyclo ./Drivers/BSP/Component/sd_card/fatfs_sd.d ./Drivers/BSP/Component/sd_card/fatfs_sd.o ./Drivers/BSP/Component/sd_card/fatfs_sd.su

.PHONY: clean-Drivers-2f-BSP-2f-Component-2f-sd_card

