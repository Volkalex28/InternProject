#ifndef _MAIN_H_
#define _MAIN_H_

#include "stm32f0xx.h"
#include "stm32f051_rcc.h"
#include "stm32f051_gpio.h"
#include "stm32f051_uart.h"
#include "stm32f051_i2c.h"

#define SIZE_ARR(_ARR_) (sizeof(_ARR_)/sizeof(_ARR_[0]))

extern STM32F051_UART_t* puart1;

#endif /* _MAIN_H_ */
