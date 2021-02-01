/**
 * @file uart_ring.c
 * @author Oleksandr Ananiev (alexander.ananiev@sigma.sofware)
 * @brief Source file for UART with ring buffer support
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

#include "uart_ring.h"
#include "user_assert.h"

// Private Variables ----------------------------------------------------------
/**
 * @defgroup UART_Ring_Private_Variables Private Variables
 * @ingroup UART_Ring
 * 
 * @brief Variables for internal needs UART with a ring buffer
 */
///@{

static uint8_t plug; ///< Cap for receiving function
///@}

// Private Functions ----------------------------------------------------------
/**
 * @defgroup UART_Ring_Private_Functions Private Functions
 * @ingroup UART_Ring
 * 
 * @brief Functions for internal needs UART with a ring buffer
 */
///@{

/**
 * @brief Callback for receiving one byte via UART interface
 * 
 * The function reads the received byte via the UART interface, 
 * adds it to the ring buffer and calls the standard callback at the end 
 * of the reception. It also restarts the reception of one byte.
 * 
 * @param[in, out] pUART Pointer to the object of the UART module
 * 
 * @return None
 */
void UART_Ring_RxCallback(STM32F051_UART_t* const pUART)
{
  ASSERT(pUART);
  ASSERT(pUART->handle);

  RING_Append(&((UART_Ring_t*)pUART)->ring, (uint8_t)pUART->handle->RDR);

  if(((UART_Ring_t*)pUART)->UART_RxCallback != NULL)
  {
    ((UART_Ring_t*)pUART)->UART_RxCallback(pUART);
  }

  UART_ReceiveIT(pUART, &plug, 1);
}
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
uint32_t UART_Ring_Init(UART_Ring_t* const ptr)
{
  ASSERT(ptr);

  if(UART1_Init(&ptr->uart) != 0)
  {
    return 1;
  }

  RING_Init(&ptr->ring, ptr->buff, UART_RING_SIZE);

  ptr->UART_RxCallback = ptr->uart.pRxCall;
  ptr->uart.pRxCall = UART_Ring_RxCallback;

  return UART_ReceiveIT(&ptr->uart, &plug, 1);
}

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
uint8_t UART_Ring_PopByte(UART_Ring_t* const ptr, uint8_t* pVarGetValue)
{
  ASSERT(ptr);
  ASSERT(pVarGetValue);

  return 1 - RING_Pop(&ptr->ring, pVarGetValue);
}

///@}
