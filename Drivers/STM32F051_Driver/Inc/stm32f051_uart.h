#ifndef __STM32F051_UART__
#define __STM32F051_UART__

#include "stm32f0xx.h"
#include <stddef.h>

extern volatile uint32_t ticks;

uint32_t UART1_Init(void);
uint32_t UART1_Transmit(uint8_t *pData, uint16_t Size, uint32_t Timeout);
uint32_t UART1_ReceiveIT(uint8_t *pData, uint16_t Size);

void UART1_IRQHandler(void);

void UART1_RxCallback(void);

#endif // __STM32F051_UART__
