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

/**
 * @defgroup Ring_Buff Ring buffer
 * @ingroup Core
 * 
 * Ring Buffer Implementation
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
/**
 * @defgroup RingBuff_Exported_Function Exported Function
 * @ingroup Ring_Buff
 * 
 * Functionality for the ring buffer accessible from the outside
 */
///@{

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
void RING_Init(RingBuffer_t * const pRing, uint8_t * const pMemory, const uint16_t size);

/**
 * @brief Clearing the ring buffer
 * 
 * The function resets the entire ring buffer to the initial setting
 * 
 * @param pRing A pointer to a circular buffer object
 */
void RING_Clear(RingBuffer_t * const pRing);

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
const uint32_t RING_Append(RingBuffer_t * const pRing, const uint8_t value);

/**
 * @brief Retrieving data from the buffer
 * 
 * The function retrieves one byte from the buffer and returns the retrieval status
 * 
 * @param[in] pRing A pointer to a circular buffer object
 * @param[out] pRing Pointer to the memory area where the cell value should be placed
 * 
 * @return Status
 * @retval 0 If the buffer is empty
 * @retval 1 If value retrieves
 */
const uint8_t RING_Pop(RingBuffer_t * const pRing, uint8_t * const pValue);

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
const uint8_t RING_Check(const RingBuffer_t * const pRing, uint8_t * const pValue);

/**
 * @brief Getting the number of bytes in the buffer
 * 
 * The function returns the number of bytes in the circular buffer
 * 
 * @param pRing A pointer to a circular buffer object
 * 
 * @return Number of bytes in the buffer
 */
const uint16_t RING_GetCount(const RingBuffer_t * const pRing);

///@}

#endif //_RING_BUFFER_H_
