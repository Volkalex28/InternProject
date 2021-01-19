#ifndef _UART_RING_H_
#define _UART_RING_H_

#include "stm32f051_uart.h"
#include "ring_buffer.h"

#define UART_RING_SIZE 20

typedef struct
{
  STM32F051_UART_t uart;
  RingBuffer_t ring;
  uint8_t buff[UART_RING_SIZE];

  void (*UART_RxCallback)();
  
} UART_Ring_t;

uint32_t UART_Ring_Init(UART_Ring_t* ptr);

#endif // _UART_RING_H_
