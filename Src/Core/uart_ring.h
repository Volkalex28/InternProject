/**
 * @file uart_ring.h
 * @author Oleksandr Ananiev (alexander.ananiev@sigma.sofware)
 * @brief Header file for UART with ring buffer support
 * @version 0.1
 * @date 2021-01-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

/**
 * @defgroup UART_Ring UART with ring buffer
 * @ingroup Core
 * 
 * @brief UART interface that must support ring buffer
 */

#ifndef _UART_RING_H_
#define _UART_RING_H_

#include "uart.h"
#include "ring_buffer.h"

// Exported Macros ------------------------------------------------------------
/**
 * @defgroup UART_Ring_Exported_Macros Exported Macros
 * @ingroup UART_Ring
 * 
 * @brief Public definitions and macros of the UART module with a ring buffer
 */
///@{

/**
 * @brief Ring buffer size
 */
#define UART_RING_SIZE 20
///@}

// Exported Types ------------------------------------------------------------
/**
 * @defgroup UART_Ring_Exported_Types Exported Types
 * @ingroup UART_Ring
 * 
 * @brief Public Types of the UART module with a ring buffer
 */
///@{

/**
 * @brief Declaring the data type of the object of the UART module with a ring buffer
 */
typedef struct
{
  STM32F051_UART_t uart;         ///< UART module object
  RingBuffer_t ring;             ///< Ring buffer object
  uint8_t buff[UART_RING_SIZE];  ///< Ring buffer memory area
  uint16_t rxCountStr;           ///< Counter for UART_Ring_GetStr() function
  /**
   * @brief Pointer to the standard callback function 
   * for at the end of reception via the UART interface
   */
  void (*UART_RxCallback)(STM32F051_UART_t * const);
  
} UART_Ring_t;
///@}

// Exported Functions ----------------------------------------------------------
/**
 * @defgroup UART_Ring_Exported_Functions Exported Functions
 * @ingroup UART_Ring
 * 
 * @brief UART functionality with a ring buffer to be reached from the outside
 */
///@{

/**
 * @brief Ring Buffer UART Initialization
 * 
 * The function initializes the ring buffer and UART module, overrides the callback 
 * and starts the reception of one byte via the UART interface in interrupt mode
 * 
 * @param ptr Pointer to UART object with ring buffer
 * 
 * @return Initialization status
 * @retval 0 If initialization was successfully
 * @retval 1 If problems occur during initialization
 */
const uint32_t UART_Ring_Init(UART_Ring_t * const ptr);

/**
 * @brief Extracting one byte from the UART buffer
 * 
 * The function retrieves one byte from the buffer, places it 
 * in memory by @p pVarGetValue pointer and returns the retrieval status
 * 
 * @param[in]  ptr Pointer to UART object with ring buffer
 * @param[out] pVarGetValue Pointer to a variable where to place the extracted byte
 * @return Status
 * @retval 0 If the extraction went smoothly 
 * @retval 1 If the buffer is empty
 */
const uint8_t UART_Ring_PopByte(UART_Ring_t * const ptr, uint8_t * const pVarGetValue);

/**
 * @brief 
 * 
 * @param ptr 
 * @param buffer 
 * @param size 
 * @param endSym 
 * @return const uint8_t 
 */
const int8_t UART_Ring_GetStr(UART_Ring_t * const ptr, char * const buffer,
  const size_t size, const char endSym);

///@}

#endif // _UART_RING_H_
