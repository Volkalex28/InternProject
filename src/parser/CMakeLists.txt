# Toolchain must come before first project
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/arm-none-eabi-gcc.cmake
  CACHE FILEPATH "CMake ARM GCC toolchain file" FORCE
)

cmake_minimum_required(VERSION 3.0)

project(InternProject)

enable_language(CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCE_ESP
  ${CMAKE_SOURCE_DIR}/src/parser/esp8266.cpp
  PARENT_SCOPE
)
