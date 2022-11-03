################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Middlewares/Third_Party/EdgeImpulse_keyword_yes_MachineLearning/edgeimpulse/edge-impulse-sdk/dsp/image/processing.cpp 

OBJS += \
./Middlewares/Third_Party/EdgeImpulse_keyword_yes_MachineLearning/edgeimpulse/edge-impulse-sdk/dsp/image/processing.o 

CPP_DEPS += \
./Middlewares/Third_Party/EdgeImpulse_keyword_yes_MachineLearning/edgeimpulse/edge-impulse-sdk/dsp/image/processing.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/EdgeImpulse_keyword_yes_MachineLearning/edgeimpulse/edge-impulse-sdk/dsp/image/%.o: ../Middlewares/Third_Party/EdgeImpulse_keyword_yes_MachineLearning/edgeimpulse/edge-impulse-sdk/dsp/image/%.cpp Middlewares/Third_Party/EdgeImpulse_keyword_yes_MachineLearning/edgeimpulse/edge-impulse-sdk/dsp/image/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/EdgeImpulse_Keyword-Spotting-STM32_MachineLearning/edgeimpulse/ -I../PDM2PCM/App -I../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../Middlewares/Third_Party/EdgeImpulse_keyword2_MachineLearning/edgeimpulse/ -I../Middlewares/Third_Party/EdgeImpulse_keyword_yes_MachineLearning/edgeimpulse/ -I"C:/Users/sam88/Desktop/MS0_Class_Fall/EmbeddedOS/workspace1.8/TinyML/FreeRTOS/portable/MenMang/ARM_CM4F" -I"C:/Users/sam88/Desktop/MS0_Class_Fall/EmbeddedOS/workspace1.8/TinyML/FreeRTOS/include" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-EdgeImpulse_keyword_yes_MachineLearning-2f-edgeimpulse-2f-edge-2d-impulse-2d-sdk-2f-dsp-2f-image

clean-Middlewares-2f-Third_Party-2f-EdgeImpulse_keyword_yes_MachineLearning-2f-edgeimpulse-2f-edge-2d-impulse-2d-sdk-2f-dsp-2f-image:
	-$(RM) ./Middlewares/Third_Party/EdgeImpulse_keyword_yes_MachineLearning/edgeimpulse/edge-impulse-sdk/dsp/image/processing.d ./Middlewares/Third_Party/EdgeImpulse_keyword_yes_MachineLearning/edgeimpulse/edge-impulse-sdk/dsp/image/processing.o

.PHONY: clean-Middlewares-2f-Third_Party-2f-EdgeImpulse_keyword_yes_MachineLearning-2f-edgeimpulse-2f-edge-2d-impulse-2d-sdk-2f-dsp-2f-image

