################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/IgraphSAT.c \
../src/algorithmFunctions.c \
../src/createCNF.c \
../src/file.c 

OBJS += \
./src/IgraphSAT.o \
./src/algorithmFunctions.o \
./src/createCNF.o \
./src/file.o 

C_DEPS += \
./src/IgraphSAT.d \
./src/algorithmFunctions.d \
./src/createCNF.d \
./src/file.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/usr/local/include/igraph -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


