#ifndef __STM32F051_I2C__
#define __STM32F051_I2C__

#include "stm32f0xx.h"

extern volatile uint32_t ticks;

uint32_t I2C_Transmit(uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);
uint32_t I2C_Receive(uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);
uint32_t I2C_Init(void);

#endif // __STM32F051_I2C__
