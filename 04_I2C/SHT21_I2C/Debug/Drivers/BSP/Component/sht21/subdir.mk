################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/Component/sht21/sht2x_for_stm32_hal.c 

OBJS += \
./Drivers/BSP/Component/sht21/sht2x_for_stm32_hal.o 

C_DEPS += \
./Drivers/BSP/Component/sht21/sht2x_for_stm32_hal.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Component/sht21/%.o Drivers/BSP/Component/sht21/%.su Drivers/BSP/Component/sht21/%.cyclo: ../Drivers/BSP/Component/sht21/%.c Drivers/BSP/Component/sht21/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L4A6xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"/home/dnk066/Pratham/STM32_Training/Workspace/04_I2C/SHT21_I2C/Drivers/BSP/Component/sht21" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-Component-2f-sht21

clean-Drivers-2f-BSP-2f-Component-2f-sht21:
	-$(RM) ./Drivers/BSP/Component/sht21/sht2x_for_stm32_hal.cyclo ./Drivers/BSP/Component/sht21/sht2x_for_stm32_hal.d ./Drivers/BSP/Component/sht21/sht2x_for_stm32_hal.o ./Drivers/BSP/Component/sht21/sht2x_for_stm32_hal.su

.PHONY: clean-Drivers-2f-BSP-2f-Component-2f-sht21

