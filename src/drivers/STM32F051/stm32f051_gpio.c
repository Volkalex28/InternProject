/**
 * @file stm32f051_gpio.c
 * @author Oleksandr Ananiev (alexander.ananiev@sigma.sofware)
 * @brief Source file of GPIO module driver
 * @version 0.1
 * @date 2021-01-17
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "stm32f051_gpio.h"
#include <user_assert.h>

// Exported Function ----------------------------------------------------------
inline void GPIO_SetPin(GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin) 
{
  ASSERT(GPIOx);
  GPIOx->BSRR = GPIO_Pin;
}

inline void GPIO_ResetPin(GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin) 
{
  ASSERT(GPIOx);
  GPIOx->BRR = GPIO_Pin;
}

inline void GPIO_TogglePin(GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin) 
{
  ASSERT(GPIOx);
  // reading curent Output Data Register value
  const uint32_t odr = GPIOx->ODR;
  // set or reset selected pin
  GPIOx->BSRR = ((odr & GPIO_Pin) << 16) | (~odr & GPIO_Pin);
}

inline const GPIO_PinState GPIO_ReadPin(const GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin) 
{
  ASSERT(GPIOx);
  return ((GPIOx->IDR & GPIO_Pin) != (uint32_t)GPIO_PIN_RESET) ? 
    GPIO_PIN_SET : GPIO_PIN_RESET;
}

const uint32_t GPIO_Init(void)
{
  // Enable clocks for GPIOC port
  SET_BIT(RCC->AHBENR, RCC_AHBENR_GPIOCEN);
  // Configurate pin 9 to output
  SET_BIT(GPIOC->MODER, GPIO_MODER_MODER9_0);
  SET_BIT(GPIOC->MODER, GPIO_MODER_MODER8_0);

  return 0;
}
