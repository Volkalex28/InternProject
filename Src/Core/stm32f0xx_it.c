/**
  ******************************************************************************
  * @file    stm32f0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
 
#include <stm32f051_rcc.h>

#include "stm32f0xx_it.h"
#include "uart_ring.h"

extern UART_Ring_t uart2;

/******************************************************************************/
/*           Cortex-M0 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @ingroup IRQ_Handlers
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  while (1)
  {
    
  }
}

/**
  * @ingroup IRQ_Handlers
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  while (1)
  {
    
  }
}

/**
  * @ingroup IRQ_Handlers
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  IncTick();
}

/**
  * @ingroup IRQ_Handlers
  * @brief This function handles USART1 global interrupt / USART1 wake-up interrupt through EXTI line 25.
  */
void USART1_IRQHandler(void)
{
  
}

/**
  * @ingroup IRQ_Handlers
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
  UART_IRQHandler(&uart2.uart);
}

/******************************************************************************/
/* STM32F0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f0xx.s).                    */
/******************************************************************************/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
