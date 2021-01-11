cmake_minimum_required(VERSION 3.15.3)

project(InternProject)

enable_language(C ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(HEADER 
  ../Drivers/STM32F051_Driver/Inc/stm32f051_rcc.h
  ../Drivers/STM32F051_Driver/Inc/stm32f051_gpio.h

  ../Core/Inc/main.h
#    ../Core/Inc/stm32f0xx_hal_conf.h
    ../Core/Inc/stm32f0xx_it.h
)
set(SOURCE
#  ../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal.c
#  ../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_cortex.c
#  ../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_dma.c
#  ../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_exti.c
#  ../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_flash.c
#  ../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_flash_ex.c
#  ../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_gpio.c
#  ../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_i2c.c
#  ../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_i2c_ex.c
#  ../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pwr.c
#  ../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pwr_ex.c
#  ../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc.c
#  ../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc_ex.c
#  ../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim.c
#  ../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim_ex.c

  ../Drivers/STM32F051_Driver/Src/stm32f051_rcc.c
  ../Drivers/STM32F051_Driver/Src/stm32f051_gpio.c

  ../Core/Src/main.c
#  ../Core/Src/stm32f0xx_hal_msp.c
  ../Core/Src/stm32f0xx_it.c
#  ../Core/Src/system_stm32f0xx.c
)
set(STARTUP
  ../Core/Startup/startup_stm32f051r8tx.s
)

set(EXECUTABLE ${PROJECT_NAME}.out)

add_executable(${EXECUTABLE} ${HEADER} ${SOURCE} ${STARTUP})

target_compile_definitions(${EXECUTABLE} PRIVATE
  -DSTM32F051x8
  -DDEBAG
#  -DUSE_HAL_DRIVER
)

target_include_directories(${EXECUTABLE} PRIVATE
  ${CMAKE_SOURCE_DIR}/Core/Inc
#  ${CMAKE_SOURCE_DIR}/Drivers/STM32F0xx_HAL_Driver/Inc
#  ${CMAKE_SOURCE_DIR}/Drivers/STM32F0xx_HAL_Driver/Inc/Legacy
  ${CMAKE_SOURCE_DIR}/Drivers/STM32F051_Driver/Inc
  ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32F0xx/Include
  ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include
)

target_compile_options(${EXECUTABLE} PRIVATE
  -mcpu=cortex-m0
  -mthumb
  -mfloat-abi=soft

  -fdata-sections
  -ffunction-sections
  -Wall
  -fstack-usage

  --specs=nano.specs

  $<$<CONFIG:Debug>:-Ofast>
)

target_link_options(${EXECUTABLE} PRIVATE
  -T${CMAKE_SOURCE_DIR}/STM32F051R8TX_FLASH.ld
  -mcpu=cortex-m0
  -mthumb
  -mfloat-abi=soft

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