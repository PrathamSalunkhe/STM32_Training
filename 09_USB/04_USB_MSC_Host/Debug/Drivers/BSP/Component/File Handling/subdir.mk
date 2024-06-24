################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/Component/File\ Handling/File_Handling.c 

OBJS += \
./Drivers/BSP/Component/File\ Handling/File_Handling.o 

C_DEPS += \
./Drivers/BSP/Component/File\ Handling/File_Handling.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Component/File\ Handling/File_Handling.o: ../Drivers/BSP/Component/File\ Handling/File_Handling.c Drivers/BSP/Component/File\ Handling/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../USB_HOST/App -I../USB_HOST/Target -I../Middlewares/Third_Party/FatFs/src -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc -I"/home/dnk066/Pratham/STM32_Training/Workspace/09_USB/04_USB_MSC_Host/Drivers/BSP/Component/File Handling" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Drivers/BSP/Component/File Handling/File_Handling.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-Component-2f-File-20-Handling

clean-Drivers-2f-BSP-2f-Component-2f-File-20-Handling:
	-$(RM) ./Drivers/BSP/Component/File\ Handling/File_Handling.cyclo ./Drivers/BSP/Component/File\ Handling/File_Handling.d ./Drivers/BSP/Component/File\ Handling/File_Handling.o ./Drivers/BSP/Component/File\ Handling/File_Handling.su

.PHONY: clean-Drivers-2f-BSP-2f-Component-2f-File-20-Handling

