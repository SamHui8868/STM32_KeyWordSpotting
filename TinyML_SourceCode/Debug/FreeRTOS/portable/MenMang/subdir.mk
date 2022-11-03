################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS/portable/MenMang/heap_4.c 

C_DEPS += \
./FreeRTOS/portable/MenMang/heap_4.d 

OBJS += \
./FreeRTOS/portable/MenMang/heap_4.o 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/portable/MenMang/%.o: ../FreeRTOS/portable/MenMang/%.c FreeRTOS/portable/MenMang/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/EdgeImpulse_Keyword-Spotting-STM32_MachineLearning/edgeimpulse/ -I../PDM2PCM/App -I../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../Middlewares/Third_Party/EdgeImpulse_keyword2_MachineLearning/edgeimpulse/ -I../Middlewares/Third_Party/EdgeImpulse_keyword_yes_MachineLearning/edgeimpulse/ -I"C:/Users/sam88/Desktop/MS0_Class_Fall/EmbeddedOS/workspace1.8/TinyML/FreeRTOS/include" -I"C:/Users/sam88/Desktop/MS0_Class_Fall/EmbeddedOS/workspace1.8/TinyML/FreeRTOS/portable/MenMang/ARM_CM4F" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-FreeRTOS-2f-portable-2f-MenMang

clean-FreeRTOS-2f-portable-2f-MenMang:
	-$(RM) ./FreeRTOS/portable/MenMang/heap_4.d ./FreeRTOS/portable/MenMang/heap_4.o

.PHONY: clean-FreeRTOS-2f-portable-2f-MenMang

