cmake_minimum_required(VERSION 3.15.3)

project(InternProject)

enable_language(C ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(HEADER 
  ../Drivers/STM32F051/Inc/stm32f051_rcc.h
  ../Drivers/STM32F051/Inc/stm32f051_gpio.h
  ../Drivers/STM32F051/Inc/stm32f051_uart.h
  ../Drivers/STM32F051/Inc/stm32f051_i2c.h

  ../Drivers/DS3231/ds3231.h

  ../Core/Inc/main.h
  ../Core/Inc/user_assert.h
  ../Core/Inc/stm32f0xx_it.h
  ../Core/Inc/pins.h
  ../Core/Inc/ring_buffer.h
  ../Core/Inc/uart_ring.h
)
set(SOURCE
  ../Drivers/STM32F051/Src/stm32f051_rcc.c
  ../Drivers/STM32F051/Src/stm32f051_gpio.c
  ../Drivers/STM32F051/Src/stm32f051_uart.c
  ../Drivers/STM32F051/Src/stm32f051_i2c.c

  ../Drivers/DS3231/ds3231.c

  ../Core/Src/main.c
  ../Core/Src/user_assert.c
  ../Core/Src/stm32f0xx_it.c
  ../Core/Src/ring_buffer.c
  ../Core/Src/uart_ring.c
)
set(STARTUP
  ../Core/Startup/startup_stm32f051r8tx.s
)

set(EXECUTABLE ${PROJECT_NAME}.out)

add_executable(${EXECUTABLE} ${HEADER} ${SOURCE} ${STARTUP})

target_compile_definitions(${EXECUTABLE} PRIVATE
  -DSTM32F051x8
  $<$<CONFIG:Debug>: -DDEBUG>
)

target_include_directories(${EXECUTABLE} PRIVATE
  ${CMAKE_SOURCE_DIR}/Core/Inc
  ${CMAKE_SOURCE_DIR}/Drivers/STM32F051/Inc
  ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32F0xx/Include
  ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include
  ${CMAKE_SOURCE_DIR}/Drivers/DS3231
)

target_compile_options(${EXECUTABLE} PRIVATE
  -mcpu=cortex-m0
  -mthumb
  -mfloat-abi=soft

  -fdata-sections
  -ffunction-sections
  -Wall
  -Winline
  -fstack-usage

  --specs=nano.specs

  $<$<CONFIG:Debug>: -Og>
  $<$<CONFIG:Release>: -O3>
)

target_link_options(${EXECUTABLE} PRIVATE
  -T${CMAKE_SOURCE_DIR}/STM32F051R8TX_FLASH.ld
  -mcpu=cortex-m0
  -mthumb
  -mfloat-abi=soft
  -u _printf_float

  --specs=nano.specs
  --specs=nosys.specs

  -Wl,-Map=${PROJECT_NAME}.map,--cref
  -Wl,--gc-sections
  -static
  -Wl,--start-group -lc -lm -Wl,--end-group
)

# Print executable size
add_custom_command(TARGET ${EXECUTABLE}
  POST_BUILD
  COMMAND arm-none-eabi-size ${EXECUTABLE})

# Create hex file
add_custom_command(TARGET ${EXECUTABLE}
  POST_BUILD
  COMMAND arm-none-eabi-objcopy -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
  COMMAND arm-none-eabi-objcopy -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin)
  