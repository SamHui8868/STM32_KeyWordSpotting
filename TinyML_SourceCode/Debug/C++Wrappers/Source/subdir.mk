################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../C++Wrappers/Source/ccondition_variable.cpp \
../C++Wrappers/Source/cevent_groups.cpp \
../C++Wrappers/Source/cmem_pool.cpp \
../C++Wrappers/Source/cmutex.cpp \
../C++Wrappers/Source/cqueue.cpp \
../C++Wrappers/Source/cread_write_lock.cpp \
../C++Wrappers/Source/csemaphore.cpp \
../C++Wrappers/Source/ctasklet.cpp \
../C++Wrappers/Source/cthread.cpp \
../C++Wrappers/Source/ctickhook.cpp \
../C++Wrappers/Source/ctimer.cpp \
../C++Wrappers/Source/cworkqueue.cpp 

OBJS += \
./C++Wrappers/Source/ccondition_variable.o \
./C++Wrappers/Source/cevent_groups.o \
./C++Wrappers/Source/cmem_pool.o \
./C++Wrappers/Source/cmutex.o \
./C++Wrappers/Source/cqueue.o \
./C++Wrappers/Source/cread_write_lock.o \
./C++Wrappers/Source/csemaphore.o \
./C++Wrappers/Source/ctasklet.o \
./C++Wrappers/Source/cthread.o \
./C++Wrappers/Source/ctickhook.o \
./C++Wrappers/Source/ctimer.o \
./C++Wrappers/Source/cworkqueue.o 

CPP_DEPS += \
./C++Wrappers/Source/ccondition_variable.d \
./C++Wrappers/Source/cevent_groups.d \
./C++Wrappers/Source/cmem_pool.d \
./C++Wrappers/Source/cmutex.d \
./C++Wrappers/Source/cqueue.d \
./C++Wrappers/Source/cread_write_lock.d \
./C++Wrappers/Source/csemaphore.d \
./C++Wrappers/Source/ctasklet.d \
./C++Wrappers/Source/cthread.d \
./C++Wrappers/Source/ctickhook.d \
./C++Wrappers/Source/ctimer.d \
./C++Wrappers/Source/cworkqueue.d 


# Each subdirectory must supply rules for building sources it contributes
C++Wrappers/Source/%.o: ../C++Wrappers/Source/%.cpp C++Wrappers/Source/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/EdgeImpulse_Keyword-Spotting-STM32_MachineLearning/edgeimpulse/ -I../PDM2PCM/App -I../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../Middlewares/Third_Party/EdgeImpulse_keyword2_MachineLearning/edgeimpulse/ -I../Middlewares/Third_Party/EdgeImpulse_keyword_yes_MachineLearning/edgeimpulse/ -I"C:/Users/sam88/Desktop/MS0_Class_Fall/EmbeddedOS/workspace1.8/TinyML/FreeRTOS/portable/MenMang/ARM_CM4F" -I"C:/Users/sam88/Desktop/MS0_Class_Fall/EmbeddedOS/workspace1.8/TinyML/FreeRTOS/include" -I"C:/Users/sam88/Desktop/MS0_Class_Fall/EmbeddedOS/workspace1.8/TinyML/C++Wrappers/Source/include" -O0 -ffunction-sections -fdata-sections -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-C-2b--2b-Wrappers-2f-Source

clean-C-2b--2b-Wrappers-2f-Source:
	-$(RM) ./C++Wrappers/Source/ccondition_variable.d ./C++Wrappers/Source/ccondition_variable.o ./C++Wrappers/Source/cevent_groups.d ./C++Wrappers/Source/cevent_groups.o ./C++Wrappers/Source/cmem_pool.d ./C++Wrappers/Source/cmem_pool.o ./C++Wrappers/Source/cmutex.d ./C++Wrappers/Source/cmutex.o ./C++Wrappers/Source/cqueue.d ./C++Wrappers/Source/cqueue.o ./C++Wrappers/Source/cread_write_lock.d ./C++Wrappers/Source/cread_write_lock.o ./C++Wrappers/Source/csemaphore.d ./C++Wrappers/Source/csemaphore.o ./C++Wrappers/Source/ctasklet.d ./C++Wrappers/Source/ctasklet.o ./C++Wrappers/Source/cthread.d ./C++Wrappers/Source/cthread.o ./C++Wrappers/Source/ctickhook.d ./C++Wrappers/Source/ctickhook.o ./C++Wrappers/Source/ctimer.d ./C++Wrappers/Source/ctimer.o ./C++Wrappers/Source/cworkqueue.d ./C++Wrappers/Source/cworkqueue.o

.PHONY: clean-C-2b--2b-Wrappers-2f-Source

