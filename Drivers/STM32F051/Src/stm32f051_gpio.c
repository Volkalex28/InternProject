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

/**
 * @addtogroup Drivers
 * @addtogroup STM32f051_Driver STM32F051
 * @ingroup Drivers
 */

/**
 * @defgroup GPIO GPIO
 * @ingroup STM32f051_Driver
 * @brief GPIO module driver
 */

#include "stm32f051_gpio.h"
#include "user_assert.h"

// Exported Function ----------------------------------------------------------
/** @defgroup GPIO_Exported_Function Exported Function
 * @ingroup GPIO
 * 
 * GPIO module function available from other files
 * @{
 */

/**
 * @brief Set the specified GPIO pin
 * @param[in] GPIOx where x can be (A..F) to select the GPIO peripheral for STM32F0 family
 * @param[in] GPIO_Pin specifies the pin
 * 
 * @retval None
 */
inline void GPIO_SetPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) 
{
  ASSERT(GPIOx);
  GPIOx->BSRR = (uint32_t)GPIO_Pin;
}

/**
 * @brief Reset the specified GPIO pin
 * @param[in] GPIOx where x can be (A..F) to select the GPIO peripheral for STM32F0 family
 * @param[in] GPIO_Pin specifies the pin
 * 
 * @retval None
 */
inline void GPIO_ResetPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) 
{
  ASSERT(GPIOx);
  GPIOx->BRR = (uint32_t)GPIO_Pin;
}

/**
 * @brief Toggle the specified GPIO pin
 * @param GPIOx where x can be (A..F) to select the GPIO peripheral for STM32F0 family
 * @param GPIO_Pin specifies the pin
 * 
 * @retval None
 */
inline void GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) 
{
  ASSERT(GPIOx);
  // reading curent Output Data Register value
  uint32_t odr = GPIOx->ODR;
  // set or reset selected pin
  GPIOx->BSRR = ((odr & GPIO_Pin) << 16) | (~odr & GPIO_Pin);
}

/**
 * @brief Read the specified input port pin
 * @param[in] GPIOx where x can be (A..F) to select the GPIO peripheral for STM32F0 family
 * @param[in] GPIO_Pin specifies the pin
 * 
 * @return The input port pin value
 */
inline GPIO_PinState GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) 
{
  ASSERT(GPIOx);
  return ((GPIOx->IDR & GPIO_Pin) != (uint32_t)GPIO_PIN_RESET) ? 
    GPIO_PIN_SET : GPIO_PIN_RESET;
}

/**
 * @brief Initializating GPIO
 * 
 * @return GPIO Init status
 * @retval 0 If initialization was successful.
 * @retval 1 If an initialization error occurs. 
 */
uint32_t GPIO_Init(void)
{
  // Enable clocks for GPIOC port
  SET_BIT(RCC->AHBENR, RCC_AHBENR_GPIOCEN);
  // Configurate pin 9 to output
  SET_BIT(GPIOC->MODER, GPIO_MODER_MODER9_0);
  SET_BIT(GPIOC->MODER, GPIO_MODER_MODER8_0);

  return 0;
}
///@}
