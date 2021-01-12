#ifndef _MAIN_H_
#define _MAIN_H_

#include "stm32f0xx.h"

#include "stm32f051_rcc.h"
#include "stm32f051_gpio.h"
#include "stm32f051_uart.h"

#define LED_GREEN_Port  GPIOC
#define LED_GREEN_Pin   GPIO_PIN_9
#define LED_BLUE_Port   GPIOC
#define LED_BLUE_Pin    GPIO_PIN_8

#endif /* _MAIN_H_ */