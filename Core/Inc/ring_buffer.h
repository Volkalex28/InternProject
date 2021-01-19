#ifndef _RING_BUFFER_H_
#define _RING_BUFFER_H_

#include <stddef.h>
#include <stdint.h>

typedef struct
{
  uint8_t* pMemory;
  uint16_t head;
  uint16_t tail;
  uint16_t size;
  
  uint8_t temp;

  uint8_t fulled : 1;
  uint8_t empty  : 1;
  
} RingBuffer_t;

uint32_t RING_Init(RingBuffer_t* pRing, uint8_t* pMemory, uint16_t size);
void RING_Clear(RingBuffer_t* pRing);
void RING_Append(RingBuffer_t* pRing, uint8_t value);
uint8_t RING_Pop(RingBuffer_t* pRing);
uint16_t RING_GetCount(RingBuffer_t* pRing);

#endif //_RING_BUFFER_H_