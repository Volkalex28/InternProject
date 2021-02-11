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

/**
 * @addtogroup Drivers
 * @addtogroup STM32f051_Driver STM32F051
 * @ingroup Drivers
 */

/**
 * @defgroup RCC RCC
 * @ingroup STM32f051_Driver
 * @brief RCC module driver
 */

#ifndef __STM32F051_RCC__
#define __STM32F051_RCC__

#include "stm32f0xx.h"

// Exported Function ---------------------------------------------------------
/** @defgroup RCC_Exported_Function Exported Function
 * @ingroup RCC
 * 
 * RCC module function available from other files
 * @{
 */

/**
 * @brief Initialization RCC, SysTick and Peripheral clocks
 * 
 * We switch to the internal clock pulse generator, configure 
 * the PLL and switch to it, configure the clocking of the HCLK, 
 * SYSCLK and PCLK1 buses, configure the SysTick at 1000 interrupts 
 * per second, configure the clocking for the periphery
 * 
 * @return RCC init status
 * @retval 0 If initialization was successful.
 * @retval 1 If an initialization error occurs.
 */
const uint32_t RCC_Init(void);

/**
 * @brief Stops program execution
 * 
 * Waits for the specified number of milliseconds
 * 
 * @warning Not recommended for long delays. Also cannot be used 
 * in interrupts. The function stops the program until 
 * the specified amount of time has passed, while all 
 * interrupts continue to work
 * 
 * @param[in] timeout Number of milliseconds to wait
 * @retval None
 */
void Delay(volatile const uint32_t miliseconds);

/**
 * @brief Provides a tick value in millisecond
 * 
 * This function is declared as __weak  to be overwritten  in case of other 
 * implementations in user file.
 * 
 * @return tick value 
 */
const uint32_t GetTick(void);

/**
 * @brief This function called to increment variable @link ticks @endlink
 * 
 * This function is declared as __weak  to be overwritten  in case of other 
 * implementations in user file.
 * 
 * @retval None
 */
void IncTick(void);

///@}

#endif // __STM32F051_RCC__
