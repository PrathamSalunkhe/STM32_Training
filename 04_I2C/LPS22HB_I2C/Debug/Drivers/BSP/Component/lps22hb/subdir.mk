################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/Component/lps22hb/lps22hb_reg.c 

OBJS += \
./Drivers/BSP/Component/lps22hb/lps22hb_reg.o 

C_DEPS += \
./Drivers/BSP/Component/lps22hb/lps22hb_reg.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Component/lps22hb/%.o Drivers/BSP/Component/lps22hb/%.su Drivers/BSP/Component/lps22hb/%.cyclo: ../Drivers/BSP/Component/lps22hb/%.c Drivers/BSP/Component/lps22hb/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L4A6xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"/home/dnk066/Pratham/STM32_Training/Workspace/04_I2C/LPS22HB_I2C/Drivers/BSP/Component/lps22hb" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-Component-2f-lps22hb

clean-Drivers-2f-BSP-2f-Component-2f-lps22hb:
	-$(RM) ./Drivers/BSP/Component/lps22hb/lps22hb_reg.cyclo ./Drivers/BSP/Component/lps22hb/lps22hb_reg.d ./Drivers/BSP/Component/lps22hb/lps22hb_reg.o ./Drivers/BSP/Component/lps22hb/lps22hb_reg.su

.PHONY: clean-Drivers-2f-BSP-2f-Component-2f-lps22hb

