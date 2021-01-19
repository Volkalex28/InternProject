#include "ring_buffer.h"
#include "user_assert.h"

void RING_Clear(RingBuffer_t* pRing)
{
  ASSERT(pRing);
  
  pRing->head = pRing->tail = 0;
  pRing->fulled = 0;
  pRing->empty = 1;
}

uint32_t RING_Init(RingBuffer_t* pRing, uint8_t* pMemory, uint16_t size)
{
  ASSERT(pRing);
  ASSERT(pMemory);
  
  pRing->pMemory = pMemory;
  pRing->size = size;
  RING_Clear(pRing);

  return 0;
}

void RING_Append(RingBuffer_t* pRing, uint8_t value)
{
  if(pRing == NULL || pRing->fulled == 1)
  {
    return;
  }

  pRing->pMemory[pRing->tail++] = value;

  pRing->tail %= pRing->size;

  if(pRing->tail == pRing->head)
  {
    pRing->fulled = 1;
  }
  pRing->empty = 0;
}

uint8_t RING_Pop(RingBuffer_t* pRing)
{
  if(pRing == NULL || pRing->empty == 1)
  {
    return 0;
  }

  const uint8_t rValue = pRing->pMemory[pRing->head++];

  pRing->head %= pRing->size;

  if(pRing->head == pRing->tail)
  {
    pRing->empty = 1;
  }
  pRing->fulled = 0;

  return rValue;
}

uint16_t RING_GetCount(RingBuffer_t* pRing)
{
  if(pRing == NULL)
  {
    return 0;
  }

  if(pRing->empty == 1)
  {
    return 0;
  }
  else if(pRing->fulled == 1)
  {
    return pRing->size;
  }
  else 
  {
    return (pRing->tail < pRing->head ? pRing->size : 0) + pRing->tail - pRing->head;
  }
}
