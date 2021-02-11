/**
 * @file ring_buffer.c
 * @author Oleksandr Ananiev (alexander.ananiev@sigma.sofware)
 * @brief Ring Buffer Source File
 * @version 0.1
 * @date 2021-01-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "ring_buffer.h"
#include "user_assert.h"

// Exported Function ----------------------------------------------------------
void RING_Clear(RingBuffer_t * const pRing)
{
  ASSERT(pRing);
  
  pRing->head = pRing->tail = 0;
  pRing->fulled = 0;
  pRing->empty = 1;
}

void RING_Init(RingBuffer_t * const pRing, uint8_t * const pMemory, const uint16_t size)
{
  ASSERT(pRing);
  ASSERT(pMemory);
  
  pRing->pMemory = pMemory;
  pRing->size = size;
  RING_Clear(pRing);
}

const uint32_t RING_Append(RingBuffer_t * const pRing, const uint8_t value)
{
  ASSERT(pRing);
  ASSERT(pRing->pMemory);

  if(pRing->fulled == 1)
  {
    return 0;
  }

  pRing->pMemory[pRing->tail++] = value;

  pRing->tail %= pRing->size;

  if(pRing->tail == pRing->head)
  {
    pRing->fulled = 1;
  }
  pRing->empty = 0;

  return 1;
}

const uint8_t RING_Pop(RingBuffer_t * const pRing, uint8_t * const pValue)
{
  ASSERT(pRing);
  ASSERT(pRing->pMemory);

  if(pRing->empty == 1)
  {
    return 0;
  }

  *pValue = pRing->pMemory[pRing->head++];

  pRing->head %= pRing->size;

  if(pRing->head == pRing->tail)
  {
    pRing->empty = 1;
  }
  pRing->fulled = 0;

  return 1;
}

const uint8_t RING_Check(const RingBuffer_t * const pRing, uint8_t * const pValue)
{
  ASSERT(pRing);
  ASSERT(pRing->pMemory);
  ASSERT(pValue);

  if(pRing->empty == 1)
  {
    return 0;
  }
  *pValue = pRing->pMemory[pRing->head];

  return 1;
}

const uint16_t RING_GetCount(const RingBuffer_t * const pRing)
{
  ASSERT(pRing);

  return ((pRing->fulled == 1 || pRing->tail < pRing->head) ? pRing->size : 0) + \
    pRing->tail - pRing->head;
}
