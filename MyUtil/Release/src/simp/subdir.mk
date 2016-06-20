################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/simp/Main.cc \
../src/simp/SimpSolver.cc 

OBJS += \
./src/simp/Main.o \
./src/simp/SimpSolver.o 

CC_DEPS += \
./src/simp/Main.d \
./src/simp/SimpSolver.d 


# Each subdirectory must supply rules for building sources it contributes
src/simp/%.o: ../src/simp/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -I"/home/geryo/workspace/GlucoseLib/src" -O3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


