################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/Component/Reference_Driver/MX25L_QSPI_Flash.c 

OBJS += \
./Drivers/BSP/Component/Reference_Driver/MX25L_QSPI_Flash.o 

C_DEPS += \
./Drivers/BSP/Component/Reference_Driver/MX25L_QSPI_Flash.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Component/Reference_Driver/%.o Drivers/BSP/Component/Reference_Driver/%.su Drivers/BSP/Component/Reference_Driver/%.cyclo: ../Drivers/BSP/Component/Reference_Driver/%.c Drivers/BSP/Component/Reference_Driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L4A6xx -DCUSTOM_DEBUG=0 -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"/home/dnk066/Pratham/STM32_Training/Workspace/05_SPI/05_QSPI_Flash/Drivers/BSP/Component/Flash" -I"/home/dnk066/Pratham/STM32_Training/Workspace/05_SPI/05_QSPI_Flash/Drivers/BSP/Component/Reference_Driver" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-Component-2f-Reference_Driver

clean-Drivers-2f-BSP-2f-Component-2f-Reference_Driver:
	-$(RM) ./Drivers/BSP/Component/Reference_Driver/MX25L_QSPI_Flash.cyclo ./Drivers/BSP/Component/Reference_Driver/MX25L_QSPI_Flash.d ./Drivers/BSP/Component/Reference_Driver/MX25L_QSPI_Flash.o ./Drivers/BSP/Component/Reference_Driver/MX25L_QSPI_Flash.su

.PHONY: clean-Drivers-2f-BSP-2f-Component-2f-Reference_Driver

