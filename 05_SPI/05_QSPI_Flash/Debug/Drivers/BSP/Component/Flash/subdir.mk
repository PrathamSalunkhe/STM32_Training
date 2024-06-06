################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/Component/Flash/FlashCircularBuffer.c \
../Drivers/BSP/Component/Flash/flash.c 

OBJS += \
./Drivers/BSP/Component/Flash/FlashCircularBuffer.o \
./Drivers/BSP/Component/Flash/flash.o 

C_DEPS += \
./Drivers/BSP/Component/Flash/FlashCircularBuffer.d \
./Drivers/BSP/Component/Flash/flash.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Component/Flash/%.o Drivers/BSP/Component/Flash/%.su Drivers/BSP/Component/Flash/%.cyclo: ../Drivers/BSP/Component/Flash/%.c Drivers/BSP/Component/Flash/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L4A6xx -DCUSTOM_DEBUG=0 -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"/home/dnk066/Pratham/STM32_Training/Workspace/05_SPI/05_QSPI_Flash/Drivers/BSP/Component/Flash" -I"/home/dnk066/Pratham/STM32_Training/Workspace/05_SPI/05_QSPI_Flash/Drivers/BSP/Component/Reference_Driver" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-Component-2f-Flash

clean-Drivers-2f-BSP-2f-Component-2f-Flash:
	-$(RM) ./Drivers/BSP/Component/Flash/FlashCircularBuffer.cyclo ./Drivers/BSP/Component/Flash/FlashCircularBuffer.d ./Drivers/BSP/Component/Flash/FlashCircularBuffer.o ./Drivers/BSP/Component/Flash/FlashCircularBuffer.su ./Drivers/BSP/Component/Flash/flash.cyclo ./Drivers/BSP/Component/Flash/flash.d ./Drivers/BSP/Component/Flash/flash.o ./Drivers/BSP/Component/Flash/flash.su

.PHONY: clean-Drivers-2f-BSP-2f-Component-2f-Flash

