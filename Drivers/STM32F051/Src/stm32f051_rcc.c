/**
 * @file stm32f051_rcc.c
 * @author Oleksandr Ananiev (alexander.ananiev@sigma.software)
 * @brief Source file of RCC module driver
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

#include "stm32f051_rcc.h"

// Exported variables ---------------------------------------------------------
/** 
 * @defgroup RCC_Exported_Variables Exported Variables
 * @ingroup RCC
 * 
 * RCC module variables available from other files
 * @{
 */ 

/**
 * @brief A variable that stores the number of milliseconds that have passed since the 
 * controller was turned on.
 * 
 * It is incremented in SysTick ISR and used in the Delay() function.
 */
static volatile uint32_t ticks = 0;
/// @}

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
uint32_t RCC_Init(void) 
{
  // ---- HSI ----
  // Start HSI oscillator
  SET_BIT(RCC->CR, RCC_CR_HSION); 
  // Wait Ready state HSI oscillator
  while(READ_BIT(RCC->CR, RCC_CR_HSIRDY) == SET); 
  // Calibration HSI oscillator
  MODIFY_REG(RCC->CR, RCC_CR_HSITRIM, (uint32_t)(0x10) << 3);

  // ---- PLL ----
  // Reset PREDIV HSE oscillator
  MODIFY_REG(RCC->CFGR2, RCC_CFGR2_PREDIV, RCC_CFGR2_PREDIV_DIV1);
  // Select source clock and set multiplication factor for PLL
  MODIFY_REG(RCC->CFGR, RCC_CFGR_PLLMUL | RCC_CFGR_PLLSRC, 
    (uint32_t)(RCC_CFGR_PLLMUL12 | RCC_CFGR_PLLSRC_HSI_DIV2));
  // Start PLL
  SET_BIT(RCC->CR, RCC_CR_PLLON);
  // Wait PLL Ready state
  while(READ_BIT(RCC->CR, RCC_CR_PLLRDY) == SET);

  // ---- HCLK ----
  // Set APB prescaler for configuration
  MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE, RCC_CFGR_PPRE_DIV16);
  // Set AGB prescaler
  MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV1);

  // ---- SYSCLK -----
  // Selected SYSCLK source
  MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL);
  // Waiting Ready state fot the selected source
  while((uint32_t)(RCC->CFGR & RCC_CFGR_SWS) == (RCC_CFGR_SW_PLL << RCC_CFGR_SWS_Pos));

  // ---- PCLK1 ----
  // Set APB prescaler for working
  MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE, RCC_CFGR_PPRE_DIV1);

  // --- SYSTICK ----
  // Set reload register
  SysTick->LOAD  = (uint32_t)(48000 - 1UL);
  // Set NVIC for configuration
  NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);
  // Reset counter value 
  SysTick->VAL   = 0UL;
  // Start SysTick
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
  // Set NVIC for working
  NVIC_SetPriority(SysTick_IRQn, 0);

  // ---- ----
  // Enable SYSCFG & COMP clock
  SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN);
  // Enable Power interface clock
  SET_BIT(RCC->APB1ENR, RCC_APB1ENR_PWREN);

  // ---- USART1 ----
  MODIFY_REG(RCC->CFGR3, RCC_CFGR3_USART1SW, RCC_CFGR3_USART1SW_PCLK);

  // ---- I2C1 ----
  MODIFY_REG(RCC->CFGR3, RCC_CFGR3_I2C1SW, RCC_CFGR3_I2C1SW_HSI);

  return 0;
}

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
inline void Delay(volatile uint32_t timeout) 
{
  // Number of ticks at the start of waiting
  uint32_t tickStart = ticks;
  // Waiting 
  while(ticks < (tickStart + timeout));
}

/**
 * @brief Provides a tick value in millisecond
 * 
 * This function is declared as __weak  to be overwritten  in case of other 
 * implementations in user file.
 * 
 * @return tick value 
 */
__WEAK uint32_t GetTick(void)
{
  return ticks;
}

/**
 * @brief This function called to increment variable @link ticks @endlink
 * 
 * This function is declared as __weak  to be overwritten  in case of other 
 * implementations in user file.
 * 
 * @retval None
 */
__WEAK void IncTick(void)
{
  ticks++;
}
/// @}

/// @}
