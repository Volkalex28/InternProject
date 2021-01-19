
#include "uart_ring.h"

static uint8_t temp;

void UART_Ring_RxCallback(const STM32F051_UART_t* pUART)
{
  RING_Append(&((UART_Ring_t*)pUART)->ring, (uint8_t)pUART->handle->RDR);
  UART_ReceiveIT(pUART, &temp, 1);

  ((UART_Ring_t*)pUART)->UART_RxCallback();
}

uint32_t UART_Ring_Init(UART_Ring_t* ptr)
{
  RING_Init(&ptr->ring, ptr->buff, UART_RING_SIZE);

  ptr->UART_RxCallback = ptr->uart.pRxCall;
  ptr->uart.pRxCall = UART_Ring_RxCallback;
  UART_ReceiveIT(&ptr->uart, &temp, 1);
}