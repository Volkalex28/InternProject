project (RunAllTests)

cmake_minimum_required(VERSION 3.0)

enable_language(C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# include project source
include_directories(${PROJECT_SOURCE_DIR}/../src/core)
include_directories(${PROJECT_SOURCE_DIR}/../src/assert)
include_directories(${PROJECT_SOURCE_DIR}/cpputest/include)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

add_executable(${PROJECT_NAME} 
  ${PROJECT_SOURCE_DIR}/src/RunAllTests.cpp
  ${PROJECT_SOURCE_DIR}/src/RingBuffer/RingBufferTests.cpp
  ${PROJECT_SOURCE_DIR}/../src/core/ring_buffer.c
)
target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/cpputest/cpputest_build/src/CppUTest/libCppUTest.a)