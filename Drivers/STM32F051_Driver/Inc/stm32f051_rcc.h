#ifndef __STM32F051_RCC__
#define __STM32F051_RCC__

#include "stm32f0xx.h"

uint32_t RCC_Init(void);
void Delay(volatile uint32_t miliseconds);

extern volatile uint32_t ticks;

#endif // __STM32F051_RCC__