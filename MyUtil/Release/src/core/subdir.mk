################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/core/Main.cc \
../src/core/Solver.cc 

OBJS += \
./src/core/Main.o \
./src/core/Solver.o 

CC_DEPS += \
./src/core/Main.d \
./src/core/Solver.d 


# Each subdirectory must supply rules for building sources it contributes
src/core/%.o: ../src/core/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -I"/home/geryo/workspace/GlucoseLib/src" -O3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


