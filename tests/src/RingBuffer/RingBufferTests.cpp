#include "../../cpputest/include/CppUTest/TestHarness.h"

extern "C"
{
  #include "../../../src/core/ring_buffer.h"
}

TEST_GROUP(RingBuffer)
{
  RingBuffer_t ring;
  uint8_t buff[20] = {};

  void setup()
  {
    RING_Init(&ring, buff, 20);
  }

  void teardown()
  {
    
  }
};

TEST(RingBuffer, RingAppendOneByte)
{
  CHECK_EQUAL(1, RING_Append(&ring, 1));
}