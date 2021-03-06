/**
 * @file uart.c
 * @author Oleksandr Ananiev (alexander.ananiev@sigma.sofware)
 * @brief 
 * @version 0.1
 * @date 2021-02-12
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <user_assert.h>
#include "uart.h"

const uint32_t UART1_Init(STM32F051_UART_t * const pUART) 
{
  ASSERT(pUART);

  SET_BIT(RCC->APB2ENR, RCC_APB2ENR_USART1EN);
  SET_BIT(RCC->AHBENR, RCC_AHBENR_GPIOAEN);

  MODIFY_REG(GPIOA->OSPEEDR, 0, GPIO_OSPEEDER_OSPEEDR9);
  CLEAR_BIT(GPIOA->OTYPER, GPIO_OTYPER_OT_9);
  MODIFY_REG(GPIOA->PUPDR, GPIO_PUPDR_PUPDR9, 0);
  MODIFY_REG(GPIOA->AFR[1], 0xE << GPIO_AFRH_AFSEL9_Pos, 1 << GPIO_AFRH_AFSEL9_Pos);
  MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODER9_0, GPIO_MODER_MODER9_1);

  MODIFY_REG(GPIOA->OSPEEDR, 0, GPIO_OSPEEDER_OSPEEDR10);
  CLEAR_BIT(GPIOA->OTYPER, GPIO_OTYPER_OT_10);
  MODIFY_REG(GPIOA->PUPDR, GPIO_PUPDR_PUPDR10, 0);
  MODIFY_REG(GPIOA->AFR[1], 0xE << GPIO_AFRH_AFSEL10_Pos, 1 << GPIO_AFRH_AFSEL10_Pos);
  MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODER10_0, GPIO_MODER_MODER10_1);

  NVIC_SetPriority(USART1_IRQn, 0);
  NVIC_EnableIRQ(USART1_IRQn);

  pUART->handle = USART1;
  pUART->baudRate = 9600;

  return UART_Init(pUART);
}

const uint32_t UART2_Init(STM32F051_UART_t * const pUART) 
{
  ASSERT(pUART);

  SET_BIT(RCC->APB1ENR, RCC_APB1ENR_USART2EN);
  SET_BIT(RCC->AHBENR, RCC_AHBENR_GPIOAEN);

  MODIFY_REG(GPIOA->OSPEEDR, 0, GPIO_OSPEEDER_OSPEEDR2);
  CLEAR_BIT(GPIOA->OTYPER, GPIO_OTYPER_OT_2);
  MODIFY_REG(GPIOA->PUPDR, GPIO_PUPDR_PUPDR2, 0);
  MODIFY_REG(GPIOA->AFR[0], 0xE << GPIO_AFRL_AFSEL2_Pos, 1 << GPIO_AFRL_AFSEL2_Pos);
  MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODER2_0, GPIO_MODER_MODER2_1);

  MODIFY_REG(GPIOA->OSPEEDR, 0, GPIO_OSPEEDER_OSPEEDR3);
  CLEAR_BIT(GPIOA->OTYPER, GPIO_OTYPER_OT_3);
  MODIFY_REG(GPIOA->PUPDR, GPIO_PUPDR_PUPDR3, 0);
  MODIFY_REG(GPIOA->AFR[0], 0xE << GPIO_AFRL_AFSEL3_Pos, 1 << GPIO_AFRL_AFSEL3_Pos);
  MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODER3_0, GPIO_MODER_MODER3_1);

  NVIC_SetPriority(USART2_IRQn, 0);
  NVIC_EnableIRQ(USART2_IRQn);

  pUART->handle = USART2;
  pUART->baudRate = 115200;

  return UART_Init(pUART);
}