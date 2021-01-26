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

#ifndef _UART_RING_H_
#define _UART_RING_H_

#include "stm32f051_uart.h"
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

  void (*UART_RxCallback)(void); ///< Pointer to the standard callback function for at the end of reception via the UART interface
  
} UART_Ring_t;
///@}

uint32_t UART_Ring_Init(UART_Ring_t* const ptr);
uint8_t UART_Ring_PopByte(UART_Ring_t* const ptr);

#endif // _UART_RING_H_
