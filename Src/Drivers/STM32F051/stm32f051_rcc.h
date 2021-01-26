/**
 * @file stm32f051_rcc.h
 * @author Oleksandr Ananiev (alexander.ananiev@sigma.software)
 * @brief Header file of RCC module driver
 * @version 0.1
 * @date 2021-01-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __STM32F051_RCC__
#define __STM32F051_RCC__

#include "stm32f0xx.h"

// Exported Function ---------------------------------------------------------
/**
 * @addtogroup RCC_Exported_Function
 * @{
 */
uint32_t RCC_Init(void);
void Delay(volatile uint32_t miliseconds);
uint32_t GetTick(void);
void IncTick(void);
///@}

#endif // __STM32F051_RCC__
