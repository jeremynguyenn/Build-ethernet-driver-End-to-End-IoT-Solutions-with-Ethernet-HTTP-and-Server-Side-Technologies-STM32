################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Wiznet_W5500/dhcp.c \
../Wiznet_W5500/httpParser.c \
../Wiznet_W5500/httpServer.c \
../Wiznet_W5500/httpUtil.c \
../Wiznet_W5500/socket.c \
../Wiznet_W5500/w5500.c \
../Wiznet_W5500/wizchip_conf.c 

OBJS += \
./Wiznet_W5500/dhcp.o \
./Wiznet_W5500/httpParser.o \
./Wiznet_W5500/httpServer.o \
./Wiznet_W5500/httpUtil.o \
./Wiznet_W5500/socket.o \
./Wiznet_W5500/w5500.o \
./Wiznet_W5500/wizchip_conf.o 

C_DEPS += \
./Wiznet_W5500/dhcp.d \
./Wiznet_W5500/httpParser.d \
./Wiznet_W5500/httpServer.d \
./Wiznet_W5500/httpUtil.d \
./Wiznet_W5500/socket.d \
./Wiznet_W5500/w5500.d \
./Wiznet_W5500/wizchip_conf.d 


# Each subdirectory must supply rules for building sources it contributes
Wiznet_W5500/%.o Wiznet_W5500/%.su Wiznet_W5500/%.cyclo: ../Wiznet_W5500/%.c Wiznet_W5500/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F411RETx -DSTM32F411xE -c -I"C:/Users/Ninsaw/Documents/Externel_Ethernet/9_w5500_comm_driver/chip_headers/CMSIS/Include" -I"C:/Users/Ninsaw/Documents/Externel_Ethernet/9_w5500_comm_driver/chip_headers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Ninsaw/Documents/Externel_Ethernet/9_w5500_comm_driver/Inc" -I"C:/Users/Ninsaw/Documents/Externel_Ethernet/9_w5500_comm_driver/Wiznet_W5500" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Wiznet_W5500

clean-Wiznet_W5500:
	-$(RM) ./Wiznet_W5500/dhcp.cyclo ./Wiznet_W5500/dhcp.d ./Wiznet_W5500/dhcp.o ./Wiznet_W5500/dhcp.su ./Wiznet_W5500/httpParser.cyclo ./Wiznet_W5500/httpParser.d ./Wiznet_W5500/httpParser.o ./Wiznet_W5500/httpParser.su ./Wiznet_W5500/httpServer.cyclo ./Wiznet_W5500/httpServer.d ./Wiznet_W5500/httpServer.o ./Wiznet_W5500/httpServer.su ./Wiznet_W5500/httpUtil.cyclo ./Wiznet_W5500/httpUtil.d ./Wiznet_W5500/httpUtil.o ./Wiznet_W5500/httpUtil.su ./Wiznet_W5500/socket.cyclo ./Wiznet_W5500/socket.d ./Wiznet_W5500/socket.o ./Wiznet_W5500/socket.su ./Wiznet_W5500/w5500.cyclo ./Wiznet_W5500/w5500.d ./Wiznet_W5500/w5500.o ./Wiznet_W5500/w5500.su ./Wiznet_W5500/wizchip_conf.cyclo ./Wiznet_W5500/wizchip_conf.d ./Wiznet_W5500/wizchip_conf.o ./Wiznet_W5500/wizchip_conf.su

.PHONY: clean-Wiznet_W5500

