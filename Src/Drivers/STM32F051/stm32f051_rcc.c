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

#include "stm32f051_rcc.h"

// Private variables ---------------------------------------------------------
/** 
 * @defgroup RCC_Private_Variables Private Variables
 * @ingroup RCC
 * 
 * RCC module variables not available from other files
 * @{
 */ 

/**
 * @brief A variable that stores the number of milliseconds that have passed since the 
 * controller was turned on.
 * 
 * It is incremented in SysTick_Handler() and used in the Delay() function.
 */
static volatile uint32_t ticks = 0;
/// @}

// Exported Function ---------------------------------------------------------
const uint32_t RCC_Init(void) 
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

inline void Delay(volatile const uint32_t timeout) 
{
  // Number of ticks at the start of waiting
  const uint32_t tickStart = ticks;
  // Waiting 
  while(ticks < (tickStart + timeout));
}

__WEAK const uint32_t GetTick(void)
{
  return ticks;
}

__WEAK void IncTick(void)
{
  ticks++;
}
