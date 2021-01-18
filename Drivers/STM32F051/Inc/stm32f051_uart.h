/**
 * @file stm32f051_uart.h
 * @author Oleksandr Ananiev (alexander.ananiev@sigma.sofware)
 * @brief Header file of UART module driver
 * @version 0.1
 * @date 2021-01-17
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __STM32F051_UART__
#define __STM32F051_UART__

#include "stm32f0xx.h"
#include <stddef.h>

// Exported Function ---------------------------------------------------------
/** @addtogroup UART_Exported_Function
 * @ingroup UART
 * @{
 */

uint32_t UART1_Init(void);
uint32_t UART1_Transmit(uint8_t *pData, uint16_t Size, const uint32_t Timeout);
uint32_t UART1_ReceiveIT(uint8_t *pData, uint16_t Size);

void UART1_IRQHandler(void);
///@}

// CallBack Function ---------------------------------------------------------
/** @addtogroup UART_CallBack_Function
 * @ingroup UART
 * @{
 */

void UART1_RxCallback(void);
///@}

#endif // __STM32F051_UART__
