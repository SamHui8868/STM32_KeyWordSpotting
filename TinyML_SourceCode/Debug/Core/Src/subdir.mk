################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/ILI9341_GFX.c \
../Core/Src/ILI9341_STM32_Driver.c \
../Core/Src/fonts.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_hal_timebase_tim.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c 

CPP_SRCS += \
../Core/Src/main.cpp 

C_DEPS += \
./Core/Src/ILI9341_GFX.d \
./Core/Src/ILI9341_STM32_Driver.d \
./Core/Src/fonts.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_hal_timebase_tim.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d 

OBJS += \
./Core/Src/ILI9341_GFX.o \
./Core/Src/ILI9341_STM32_Driver.o \
./Core/Src/fonts.o \
./Core/Src/main.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_hal_timebase_tim.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o 

CPP_DEPS += \
./Core/Src/main.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/EdgeImpulse_Keyword-Spotting-STM32_MachineLearning/edgeimpulse/ -I../PDM2PCM/App -I../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../Middlewares/Third_Party/EdgeImpulse_keyword2_MachineLearning/edgeimpulse/ -I../Middlewares/Third_Party/EdgeImpulse_keyword_yes_MachineLearning/edgeimpulse/ -I"C:/Users/sam88/Desktop/MS0_Class_Fall/EmbeddedOS/workspace1.8/TinyML/FreeRTOS/include" -I"C:/Users/sam88/Desktop/MS0_Class_Fall/EmbeddedOS/workspace1.8/TinyML/FreeRTOS/portable/MenMang/ARM_CM4F" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/%.o: ../Core/Src/%.cpp Core/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/EdgeImpulse_Keyword-Spotting-STM32_MachineLearning/edgeimpulse/ -I../PDM2PCM/App -I../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../Middlewares/Third_Party/EdgeImpulse_keyword2_MachineLearning/edgeimpulse/ -I../Middlewares/Third_Party/EdgeImpulse_keyword_yes_MachineLearning/edgeimpulse/ -I"C:/Users/sam88/Desktop/MS0_Class_Fall/EmbeddedOS/workspace1.8/TinyML/FreeRTOS/portable/MenMang/ARM_CM4F" -I"C:/Users/sam88/Desktop/MS0_Class_Fall/EmbeddedOS/workspace1.8/TinyML/FreeRTOS/include" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/ILI9341_GFX.d ./Core/Src/ILI9341_GFX.o ./Core/Src/ILI9341_STM32_Driver.d ./Core/Src/ILI9341_STM32_Driver.o ./Core/Src/fonts.d ./Core/Src/fonts.o ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_timebase_tim.d ./Core/Src/stm32f4xx_hal_timebase_tim.o ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o

.PHONY: clean-Core-2f-Src

