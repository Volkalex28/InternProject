# Toolchain must come before first project
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/arm-none-eabi-gcc.cmake
  CACHE FILEPATH "CMake ARM GCC toolchain file" FORCE
)
  
cmake_minimum_required(VERSION 3.0)

project(InternProject)

enable_language(C ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Build type
set(EXECUTABLE_OUTPUT_PATH  ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE})

set(CMAKE_EXECUTABLE_SUFFIX ".elf")

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(DRIVER_DIR ${SOURCE_DIR}/drivers)
set(ASSERT_DIR ${SOURCE_DIR}/assert)
set(CORE_DIR ${SOURCE_DIR}/core)

set(SOURCE
  ${DRIVER_DIR}/STM32F051/stm32f051_rcc.c
  ${DRIVER_DIR}/STM32F051/stm32f051_gpio.c
  ${DRIVER_DIR}/STM32F051/stm32f051_uart.c
  ${DRIVER_DIR}/STM32F051/stm32f051_i2c.c

  ${DRIVER_DIR}/DS3231/ds3231.c

  ${ASSERT_DIR}/user_assert.c

  ${CORE_DIR}/main.c
  ${CORE_DIR}/stm32f0xx_it.c
  ${CORE_DIR}/ring_buffer.c
  ${CORE_DIR}/uart_ring.c
  ${CORE_DIR}/uart.c
)
set(STARTUP
  ${CORE_DIR}/startup_stm32f051r8tx.s
)

set(EXECUTABLE ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME})

add_executable(${PROJECT_NAME} ${SOURCE} ${STARTUP})

target_compile_definitions(${PROJECT_NAME} PRIVATE
  -DSTM32F051x8
  $<$<CONFIG:Debug>: -DDEBUG>
)

target_include_directories(${PROJECT_NAME} PRIVATE
  ${CORE_DIR}
  ${ASSERT_DIR}
  ${DRIVER_DIR}/STM32F051
  ${DRIVER_DIR}/CMSIS/Device/ST/STM32F0xx/Include
  ${DRIVER_DIR}/CMSIS/Include
  ${DRIVER_DIR}/DS3231
)

target_compile_options(${PROJECT_NAME} PRIVATE
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

target_link_options(${PROJECT_NAME} PRIVATE
  -T${CMAKE_SOURCE_DIR}/STM32F051R8TX_FLASH.ld
  -mcpu=cortex-m0
  -mthumb
  -mfloat-abi=soft
  -u _printf_float

  --specs=nano.specs
  --specs=nosys.specs

  -Wl,-Map=${EXECUTABLE}.map,--cref
  -Wl,--gc-sections
  -static
  -Wl,--start-group -lc -lm -Wl,--end-group
)

# Print executable size
add_custom_command(TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND arm-none-eabi-size ${EXECUTABLE}${CMAKE_EXECUTABLE_SUFFIX}
)

# Create hex file
add_custom_command(TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND arm-none-eabi-objcopy -O ihex ${EXECUTABLE}${CMAKE_EXECUTABLE_SUFFIX} ${EXECUTABLE}.hex
  COMMAND arm-none-eabi-objcopy -O binary ${EXECUTABLE}${CMAKE_EXECUTABLE_SUFFIX} ${EXECUTABLE}.bin
)

include(ExternalProject)

ExternalProject_Add(
  unit_tests
  PREFIX            ${CMAKE_SOURCE_DIR}/tests
  SOURCE_DIR        ${CMAKE_SOURCE_DIR}/tests
  BINARY_DIR        ${CMAKE_SOURCE_DIR}/tests/build
  STAMP_DIR         ${CMAKE_SOURCE_DIR}/tests/build
  TMP_DIR           ${CMAKE_SOURCE_DIR}/tests/build
  DOWNLOAD_COMMAND  ""
  PATCH_COMMAND     ""
  INSTALL_COMMAND   ""
  UPDATE_COMMAND    ""
  CONFIGURE_COMMAND ${CMAKE_COMMAND} -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -S ${CMAKE_SOURCE_DIR}/tests -G ${CMAKE_GENERATOR}
  BUILD_ALWAYS      TRUE
  ALWAYS            TRUE
  DEPENDS           ${PROJECT_NAME}
)

ExternalProject_Add(
  cpputest
  PREFIX            ${CMAKE_SOURCE_DIR}/tests/cpputest
  SOURCE_DIR        ${CMAKE_SOURCE_DIR}/tests/cpputest
  BINARY_DIR        ${CMAKE_SOURCE_DIR}/tests/cpputest/cpputest_build
  STAMP_DIR         ${CMAKE_SOURCE_DIR}/tests/cpputest/cpputest_build
  TMP_DIR           ${CMAKE_SOURCE_DIR}/tests/cpputest/cpputest_build
  DOWNLOAD_COMMAND  ""
  PATCH_COMMAND     ""
  INSTALL_COMMAND   ""
  UPDATE_COMMAND    ""
  CONFIGURE_COMMAND ${CMAKE_COMMAND} -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -S ${CMAKE_SOURCE_DIR}/tests/cpputest -G ${CMAKE_GENERATOR} -DC++11=ON -DTESTS=OFF -DTESTS_BUILD_DISCOVER=OFF
  BUILD_ALWAYS      TRUE
  ALWAYS            TRUE
  DEPENDS           ${PROJECT_NAME}
)
