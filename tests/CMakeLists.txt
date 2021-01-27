project (RunAllTest)

cmake_minimum_required(VERSION 3.0)

# include project source
include_directories(${PROJECT_SOURCE_DIR}/../src/core)
include_directories(${PROJECT_SOURCE_DIR}/../src/assert)
include_directories(${PROJECT_SOURCE_DIR}/cpputest/include)

add_library(imp_cpputest STATIC IMPORTED)
set_property(TARGET imp_cpputest PROPERTY
  IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/cpputest/cpputest_build/src/CppUTest/libCppUTest.a)

add_library(RingBufferTest ${PROJECT_SOURCE_DIR}/src/RingBuffer/RingBufferTests.cpp)

add_library(RingBuffer ${PROJECT_SOURCE_DIR}/../src/core/ring_buffer.c)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/src/RunAllTests.cpp)
target_link_libraries(${PROJECT_NAME} imp_cpputest RingBufferTest RingBuffer)
