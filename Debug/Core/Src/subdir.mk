################################################################################
# 自动生成的文件。不要编辑！
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/alarmClock.c \
../Core/Src/changeScreen.c \
../Core/Src/delayUs.c \
../Core/Src/dht11.c \
../Core/Src/gpio.c \
../Core/Src/i2c.c \
../Core/Src/key.c \
../Core/Src/led.c \
../Core/Src/main.c \
../Core/Src/mq2.c \
../Core/Src/myVolatile.c \
../Core/Src/oled.c \
../Core/Src/rtc.c \
../Core/Src/show.c \
../Core/Src/song.c \
../Core/Src/stm32f1xx_hal_msp.c \
../Core/Src/stm32f1xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f1xx.c \
../Core/Src/tim.c \
../Core/Src/usart.c 

OBJS += \
./Core/Src/alarmClock.o \
./Core/Src/changeScreen.o \
./Core/Src/delayUs.o \
./Core/Src/dht11.o \
./Core/Src/gpio.o \
./Core/Src/i2c.o \
./Core/Src/key.o \
./Core/Src/led.o \
./Core/Src/main.o \
./Core/Src/mq2.o \
./Core/Src/myVolatile.o \
./Core/Src/oled.o \
./Core/Src/rtc.o \
./Core/Src/show.o \
./Core/Src/song.o \
./Core/Src/stm32f1xx_hal_msp.o \
./Core/Src/stm32f1xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f1xx.o \
./Core/Src/tim.o \
./Core/Src/usart.o 

C_DEPS += \
./Core/Src/alarmClock.d \
./Core/Src/changeScreen.d \
./Core/Src/delayUs.d \
./Core/Src/dht11.d \
./Core/Src/gpio.d \
./Core/Src/i2c.d \
./Core/Src/key.d \
./Core/Src/led.d \
./Core/Src/main.d \
./Core/Src/mq2.d \
./Core/Src/myVolatile.d \
./Core/Src/oled.d \
./Core/Src/rtc.d \
./Core/Src/show.d \
./Core/Src/song.d \
./Core/Src/stm32f1xx_hal_msp.d \
./Core/Src/stm32f1xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f1xx.d \
./Core/Src/tim.d \
./Core/Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/alarmClock.cyclo ./Core/Src/alarmClock.d ./Core/Src/alarmClock.o ./Core/Src/alarmClock.su ./Core/Src/changeScreen.cyclo ./Core/Src/changeScreen.d ./Core/Src/changeScreen.o ./Core/Src/changeScreen.su ./Core/Src/delayUs.cyclo ./Core/Src/delayUs.d ./Core/Src/delayUs.o ./Core/Src/delayUs.su ./Core/Src/dht11.cyclo ./Core/Src/dht11.d ./Core/Src/dht11.o ./Core/Src/dht11.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/i2c.cyclo ./Core/Src/i2c.d ./Core/Src/i2c.o ./Core/Src/i2c.su ./Core/Src/key.cyclo ./Core/Src/key.d ./Core/Src/key.o ./Core/Src/key.su ./Core/Src/led.cyclo ./Core/Src/led.d ./Core/Src/led.o ./Core/Src/led.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/mq2.cyclo ./Core/Src/mq2.d ./Core/Src/mq2.o ./Core/Src/mq2.su ./Core/Src/myVolatile.cyclo ./Core/Src/myVolatile.d ./Core/Src/myVolatile.o ./Core/Src/myVolatile.su ./Core/Src/oled.cyclo ./Core/Src/oled.d ./Core/Src/oled.o ./Core/Src/oled.su ./Core/Src/rtc.cyclo ./Core/Src/rtc.d ./Core/Src/rtc.o ./Core/Src/rtc.su ./Core/Src/show.cyclo ./Core/Src/show.d ./Core/Src/show.o ./Core/Src/show.su ./Core/Src/song.cyclo ./Core/Src/song.d ./Core/Src/song.o ./Core/Src/song.su ./Core/Src/stm32f1xx_hal_msp.cyclo ./Core/Src/stm32f1xx_hal_msp.d ./Core/Src/stm32f1xx_hal_msp.o ./Core/Src/stm32f1xx_hal_msp.su ./Core/Src/stm32f1xx_it.cyclo ./Core/Src/stm32f1xx_it.d ./Core/Src/stm32f1xx_it.o ./Core/Src/stm32f1xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f1xx.cyclo ./Core/Src/system_stm32f1xx.d ./Core/Src/system_stm32f1xx.o ./Core/Src/system_stm32f1xx.su ./Core/Src/tim.cyclo ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/usart.cyclo ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su

.PHONY: clean-Core-2f-Src

