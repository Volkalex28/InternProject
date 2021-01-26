/**
 * @file ring_buffer.h
 * @author Oleksandr Ananiev (alexander.ananiev@sigma.sofware)
 * @brief Ring Buffer Header File
 * @version 0.1
 * @date 2021-01-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _RING_BUFFER_H_
#define _RING_BUFFER_H_

#include <stddef.h>
#include <stdint.h>

// Exported types -------------------------------------------------------------
/**
 * @defgroup RingBuff_Exported_Types Exported Types
 * @ingroup Ring_Buff
 * 
 * @brief Ring Buffer types available from other files
 */
///@{

/**
 * @brief Ring Buffer Object Data Type Declaration
 */
typedef struct
{
  uint8_t* pMemory;   ///< Pointer to buffer memory area
  uint16_t head;      ///< The index of the cell into which the next byte will be placed
  uint16_t tail;      ///< The index of the cell in which the earliest byte is stored
  uint16_t size;      ///< Buffer size

  uint8_t fulled : 1; ///< Buffer full flag
  uint8_t empty  : 1; ///< Buffer empty flag
  
} RingBuffer_t;
///@}

// Exported function ----------------------------------------------------------

uint32_t RING_Init(RingBuffer_t* const pRing, uint8_t* const pMemory, const uint16_t size);
void RING_Clear(RingBuffer_t* const pRing);
uint32_t RING_Append(RingBuffer_t* const pRing, const uint8_t value);
uint8_t RING_Pop(RingBuffer_t* const pRing);
uint8_t RING_Check(const RingBuffer_t* const pRing);
uint16_t RING_GetCount(const RingBuffer_t* const pRing);

#endif //_RING_BUFFER_H_