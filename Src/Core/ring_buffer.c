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

/**
 * @defgroup Ring_Buff Ring buffer
 * @ingroup Core
 * 
 * Ring Buffer Implementation
 */

#include "ring_buffer.h"
#include "user_assert.h"

// Exported Function ----------------------------------------------------------
/**
 * @defgroup RingBuff_Exported_Function Exported Function
 * @ingroup Ring_Buff
 * 
 * Functionality for the ring buffer accessible from the outside
 */
///@{

/**
 * @brief Clearing the ring buffer
 * 
 * The function resets the entire ring buffer to the initial setting
 * 
 * @param pRing A pointer to a circular buffer object
 */
void RING_Clear(RingBuffer_t* const pRing)
{
  ASSERT(pRing);
  
  pRing->head = pRing->tail = 0;
  pRing->fulled = 0;
  pRing->empty = 1;
}

/**
 * @brief Ring buffer initialization
 * 
 * The function initializes memory for the ring buffer 
 * and clears it with the RING_Clear() function
 * 
 * @param[out] pRing A pointer to a circular buffer object
 * @param[in]  pMemory Pointer to memory area for circular buffer
 * @param[in]  size Memory area size in bytes
 * 
 * @return None 
 */
void RING_Init(RingBuffer_t* const pRing, uint8_t* const pMemory, const uint16_t size)
{
  ASSERT(pRing);
  ASSERT(pMemory);
  
  pRing->pMemory = pMemory;
  pRing->size = size;
  RING_Clear(pRing);
}

/**
 * @brief Adding a value to the buffer
 * 
 * The function adds a byte to the buffer and returns the add status
 * 
 * @param[out] pRing A pointer to a circular buffer object
 * @param[in]  value The value to add
 * 
 * @return Add status
 * @retval 1 If added was successfully
 * @retval 0 If the buffer is full
 */
uint32_t RING_Append(RingBuffer_t* const pRing, const uint8_t value)
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

/**
 * @brief Retrieving data from the buffer
 * 
 * The function retrieves one byte from the buffer and returns the retrieval status
 * 
 * @param[in] pRing A pointer to a circular buffer object
 * @param[out] pRing Pointer to the memory area where the cell value should be placed
 * 
 * @return Byte value or status
 * @retval 0 If the buffer is empty
 * @retval 1 If value retrieves
 */
uint8_t RING_Pop(RingBuffer_t* const pRing, uint8_t*const pValue)
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

/**
 * @brief View the last byte in the buffer
 * 
 * The function shows the last byte from the buffer 
 * without deleting it and returns the retrieval status.
 * 
 * @param[in] pRing A pointer to a circular buffer object
 * @param[out] pRing Pointer to the memory area where the cell value should be placed
 * 
 * @return Status
 * @retval 0 If the buffer is empty
 * @retval 1 If value viewed
 */
uint8_t RING_Check(const RingBuffer_t*const pRing, uint8_t*const pValue)
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

/**
 * @brief Getting the number of bytes in the buffer
 * 
 * The function returns the number of bytes in the circular buffer
 * 
 * @param pRing A pointer to a circular buffer object
 * 
 * @return Number of bytes in the buffer
 */
uint16_t RING_GetCount(const RingBuffer_t* const pRing)
{
  ASSERT(pRing);

  return ((pRing->fulled == 1 || pRing->tail < pRing->head) ? pRing->size : 0) + \
    pRing->tail - pRing->head;
}

///@}
