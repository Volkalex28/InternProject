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

#include "uart_ring.h"
#include <user_assert.h>


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
void UART_Ring_RxCallback(STM32F051_UART_t * const pUART)
{
  ASSERT(pUART);
  ASSERT(pUART->handle);

  RING_Append(&((UART_Ring_t *)pUART)->ring, (uint8_t)pUART->handle->RDR);

  if (((UART_Ring_t *)pUART)->UART_RxCallback != NULL)
  {
    ((UART_Ring_t *)pUART)->UART_RxCallback(pUART);
  }

  UART_ReceiveIT(pUART, &plug, 1);
}
///@}

// Exported Functions ----------------------------------------------------------
const uint32_t UART_Ring_Init(UART_Ring_t * const ptr, const UART_Ring_Init_t number)
{
  ASSERT(ptr);

  if (number == UART1_INIT)
  {
    if (UART1_Init(&ptr->uart) != 0)
    {
      return 1;
    }
  }
  else if(number == UART2_INIT)
  {
    if (UART2_Init(&ptr->uart) != 0)
    {
      return 1;
    }
  }

  RING_Init(&ptr->ring, ptr->buff, UART_RING_SIZE);

  ptr->UART_RxCallback = ptr->uart.pRxCall;
  ptr->uart.pRxCall = UART_Ring_RxCallback;

  return UART_ReceiveIT(&ptr->uart, &plug, 1);
}

const uint8_t UART_Ring_PopByte(UART_Ring_t * const ptr, uint8_t * const pVarGetValue)
{
  ASSERT(ptr);
  ASSERT(pVarGetValue);

  return RING_Pop(&ptr->ring, pVarGetValue);
}

const int8_t UART_Ring_GetStr(UART_Ring_t * const ptr, char * const buffer, const size_t size, const char endSym)
{
  while (UART_Ring_PopByte(ptr, (uint8_t *)&buffer[ptr->rxCountStr]))
  {
    if (buffer[ptr->rxCountStr] == endSym)
    {
      const uint16_t pos = ptr->rxCountStr;
      ptr->rxCountStr = 0;
      return pos;
    }
    else
    {
      ptr->rxCountStr++;

      if (ptr->rxCountStr < size)
      {
        continue;
      }
    }
    ptr->rxCountStr = 0;
    return -2;
  }
  return -1;
}
