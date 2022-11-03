################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../PDM2PCM/App/pdm2pcm.c 

C_DEPS += \
./PDM2PCM/App/pdm2pcm.d 

OBJS += \
./PDM2PCM/App/pdm2pcm.o 


# Each subdirectory must supply rules for building sources it contributes
PDM2PCM/App/%.o: ../PDM2PCM/App/%.c PDM2PCM/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/EdgeImpulse_Keyword-Spotting-STM32_MachineLearning/edgeimpulse/ -I../PDM2PCM/App -I../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../Middlewares/Third_Party/EdgeImpulse_keyword2_MachineLearning/edgeimpulse/ -I../Middlewares/Third_Party/EdgeImpulse_keyword_yes_MachineLearning/edgeimpulse/ -I"C:/Users/sam88/Desktop/MS0_Class_Fall/EmbeddedOS/workspace1.8/TinyML/FreeRTOS/include" -I"C:/Users/sam88/Desktop/MS0_Class_Fall/EmbeddedOS/workspace1.8/TinyML/FreeRTOS/portable/MenMang/ARM_CM4F" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-PDM2PCM-2f-App

clean-PDM2PCM-2f-App:
	-$(RM) ./PDM2PCM/App/pdm2pcm.d ./PDM2PCM/App/pdm2pcm.o

.PHONY: clean-PDM2PCM-2f-App

