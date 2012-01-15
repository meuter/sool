################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/cobject/class.c \
../src/cobject/io.c \
../src/cobject/list.c 

OBJS += \
./src/cobject/class.o \
./src/cobject/io.o \
./src/cobject/list.o 

C_DEPS += \
./src/cobject/class.d \
./src/cobject/io.d \
./src/cobject/list.d 


# Each subdirectory must supply rules for building sources it contributes
src/cobject/%.o: ../src/cobject/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/ibeast/meuter/Workspace/cobject/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


