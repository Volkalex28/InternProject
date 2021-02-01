#include "../../cpputest/include/CppUTest/TestHarness.h"
#include <vector>
#include <iostream>

extern "C"
{
  #include "../../../src/core/ring_buffer.h"
}

using namespace std;

// ============================= / Funtions prototypes / ======================
vector<uint8_t> BufferFilling(RingBuffer_t& pRing);

// ============================= / Testing / ==================================
// Create test group for testing ring buffer functionality
TEST_GROUP(RingBuffer)
{
  static const uint8_t sizeRingBuffer = 20;
  const uint8_t appendByteValue = 100;

  RingBuffer_t ring;
  uint8_t buff[sizeRingBuffer] = {};

  void setup()
  {
    RING_Init(&ring, buff, sizeRingBuffer);
  }
};

// ******************************* Init ***************************************
// Verifying correct ring buffer initializations 
TEST(RingBuffer, correctInitializationBuffer)
{
  CHECK_TRUE(ring.empty);
  CHECK_FALSE(ring.fulled);
  CHECK_TRUE(ring.pMemory == buff);
  CHECK_TRUE(ring.size == sizeRingBuffer);
}

// ******************************* Clear **************************************
// Verifying correct ring buffer flushed
TEST(RingBuffer, clearingBuffer)
{
  RING_Clear(&ring);

  CHECK_TRUE(ring.empty);
  CHECK_FALSE(ring.fulled);
}

// ******************************* Append *************************************
// Checking that one byte is correctly added to an empty circular buffer
TEST(RingBuffer, appendOneByteToEmptyBuffer)
{
  RING_Clear(&ring);

  CHECK_EQUAL(1, RING_Append(&ring, appendByteValue));
  CHECK_TRUE(appendByteValue == buff[0]);
  CHECK_FALSE(ring.empty);
  CHECK_FALSE(ring.fulled);
}

// Verifying that one byte is correctly added to a circular buffer 
// that already contains data
TEST(RingBuffer, appendBytesToNonEmptyBuffer)
{
  const uint8_t countCell = 3;

  RING_Clear(&ring);

  for(size_t i = 0; i < countCell; i++)
  {
    RING_Append(&ring, 1);
  }

  CHECK_EQUAL(1, RING_Append(&ring, appendByteValue));
  CHECK_TRUE(appendByteValue == buff[countCell]);
  CHECK_FALSE(ring.empty);
  CHECK_FALSE(ring.fulled);
}

// Verifying that one byte is correctly added to a circular buffer
// that already contains data
TEST(RingBuffer, appendBytesInAllCellBuffer)
{
  vector<uint8_t> values = BufferFilling(ring);

  for(size_t i = 0; i < sizeRingBuffer; i++)
  {
    CHECK_TRUE(values[i] == buff[i]);
  }
  CHECK_FALSE(ring.empty);
  CHECK_TRUE(ring.fulled);
}

// Checking for an error when trying to add bytes to a full buffer
TEST(RingBuffer, appendOneByteToFullBuffer)
{
  BufferFilling(ring);

  CHECK_EQUAL(0, RING_Append(&ring, appendByteValue));
  CHECK_FALSE(ring.empty);
  CHECK_TRUE(ring.fulled);
}

// ******************************* Pop ****************************************
// Checking for an error when trying to extract a byte from an empty buffer
TEST(RingBuffer, popByteFromEmptyBuffer)
{
  uint8_t value = 0;

  RING_Clear(&ring);

  CHECK_EQUAL(0, RING_Pop(&ring, &value));
}

// Validating Data Retrieval Under Normal Conditions
TEST(RingBuffer, popByteFromNonEmptyBuffer)
{
  uint8_t value = 0;

  RING_Clear(&ring);
  RING_Append(&ring, appendByteValue);

  CHECK_EQUAL(1, RING_Pop(&ring, &value));
  CHECK_TRUE(value == appendByteValue);
  CHECK_TRUE(ring.empty);
}

// Checking the correctness of data retrieval when the buffer is full
TEST(RingBuffer, popBytesFromFullBuffer)
{
  uint8_t value = 0;

  BufferFilling(ring);

  CHECK_EQUAL(1, RING_Pop(&ring, &value));
  CHECK_FALSE(ring.fulled);
}

// ******************************* Check **************************************
// Checking for an error when trying to view data in an empty buffer
TEST(RingBuffer, checkingByteInEmptyBuffer)
{
  uint8_t value = 0;

  RING_Clear(&ring);

  CHECK_EQUAL(0, RING_Check(&ring, &value));
}

// Checking that the data is viewed correctly and that the buffer state
// does not change
TEST(RingBuffer, checkingByteInNonEmptyBuffer)
{
  uint8_t value = 0;

  RING_Clear(&ring);
  RING_Append(&ring, appendByteValue);

  CHECK_EQUAL(1, RING_Check(&ring, &value));
  CHECK_TRUE(value == appendByteValue);
  CHECK_FALSE(ring.empty);
}

// Checking the correctness of data viewing and that the state of the
// filled buffer does not change
TEST(RingBuffer, checkingBytesInFullBuffer)
{
  uint8_t value = 0;

  BufferFilling(ring);

  CHECK_EQUAL(1, RING_Check(&ring, &value));
  CHECK_TRUE(ring.fulled);
}

// ******************************* Get count **********************************
// Checking the number of bytes in an empty buffer
TEST(RingBuffer, checkingByteCountInEmptyBuffer)
{
  RING_Clear(&ring);

  CHECK_EQUAL(0, RING_GetCount(&ring));
}

// Checking the number of bytes in a non-empty buffer
TEST(RingBuffer, checkingByteCountInNonEmptyBuffer)
{
  const uint8_t countBytes = 5;

  RING_Clear(&ring);
  for(size_t i = 0; i < countBytes; i++)
  {
    RING_Append(&ring, appendByteValue);
  }

  CHECK_EQUAL(countBytes, RING_GetCount(&ring));
}

// Checking the number of bytes in the filled buffer.
TEST(RingBuffer, checkingByteCountInFullBuffer)
{
  BufferFilling(ring);

  CHECK_EQUAL(ring.size, RING_GetCount(&ring));
  CHECK_TRUE(ring.fulled);
}

// Checking the number of bytes in a non-empty buffer. In this case,
// the full buffer has already been looped
TEST(RingBuffer, checkingByteCountInFullOffsetBuffer)
{
  const uint8_t countBytes = 2;
  uint8_t value = 0;

  BufferFilling(ring);
  for(size_t i = 0; i < countBytes; i++)
  {
    RING_Pop(&ring, &value);
    RING_Append(&ring, value);
  }
  
  CHECK_EQUAL(ring.size, RING_GetCount(&ring));
  CHECK_TRUE(ring.fulled);
}
// ============================= / Functions / ================================
// Function to clear and fill ring buffer
vector<uint8_t> BufferFilling(RingBuffer_t& refRing)
{
  vector<uint8_t> valueForBuffer(refRing.size);

  RING_Clear(&refRing);
  for(size_t i = 0; refRing.fulled != 1; i++)
  {
    valueForBuffer.push_back(i+1);
    RING_Append(&refRing, valueForBuffer[i]);
  }

  return valueForBuffer;
}
