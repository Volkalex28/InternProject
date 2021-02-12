/**
 * @file stm32f051_gpio.h
 * @author Oleksandr Ananiev (alexander.ananiev@sigma.sofware)
 * @brief Header file of GPIO module driver
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

#ifndef __STM32F051_GPIO__
#define __STM32F051_GPIO__

#include <stm32f0xx.h>

// Exported Types -------------------------------------------------------------
/**
 * @defgroup GPIO_Exported_Types Exported Types
 * @ingroup GPIO
 * 
 * Data type declarations that are accessible from other files
 * 
 * @{
 */

/** 
  * @brief  GPIO Bit SET and Bit RESET type 
  */
typedef enum
{
  GPIO_PIN_RESET = 0U,  ///< Pin is low
  GPIO_PIN_SET          ///< Pin is high
} GPIO_PinState;
///@}

// Exported Enumeration -------------------------------------------------------------
/**
 * @defgroup GPIO_Exported_Enumeration Exported Enumeration
 * @ingroup GPIO
 * 
 * Enumerations and constants that are accessible from other files
 * @{
 */

/** 
  * @brief  GPIO Pin number enumeration 
  */
enum GPIO_Pin_N
{
  GPIO_PIN_0 = 0x0001U,  ///< Pin 0 selected
  GPIO_PIN_1 = 0x0002U,  ///< Pin 1 selected
  GPIO_PIN_2 = 0x0004U,  ///< Pin 2 selected
  GPIO_PIN_3 = 0x0008U,  ///< Pin 3 selected
  GPIO_PIN_4 = 0x0010U,  ///< Pin 4 selected
  GPIO_PIN_5 = 0x0020U,  ///< Pin 5 selected
  GPIO_PIN_6 = 0x0040U,  ///< Pin 6 selected
  GPIO_PIN_7 = 0x0080U,  ///< Pin 7 selected
  GPIO_PIN_8 = 0x0100U,  ///< Pin 8 selected
  GPIO_PIN_9 = 0x0200U,  ///< Pin 9 selected
  GPIO_PIN_10 = 0x0400U,  ///< Pin 10 selected
  GPIO_PIN_11 = 0x0800U,  ///< Pin 11 selected
  GPIO_PIN_12 = 0x1000U,  ///< Pin 12 selected
  GPIO_PIN_13 = 0x2000U,  ///< Pin 13 selected
  GPIO_PIN_14 = 0x4000U,  ///< Pin 14 selected
  GPIO_PIN_15 = 0x8000U,  ///< Pin 15 selected
  GPIO_PIN_All = 0xFFFFU  ///< All pins selected
};
///@}

// Exported Function ----------------------------------------------------------
/** @defgroup GPIO_Exported_Function Exported Function
 * @ingroup GPIO
 * 
 * GPIO module function available from other files
 * @{
 */

/**
 * @brief Initializating GPIO
 * 
 * @return GPIO Init status
 * @retval 0 If initialization was successful.
 * @retval 1 If an initialization error occurs. 
 */
const uint32_t GPIO_Init(void);

/**
 * @brief Read the specified input port pin
 * @param[in] GPIOx where x can be (A..F) to select the GPIO peripheral for STM32F0 family
 * @param[in] GPIO_Pin specifies the pin
 * 
 * @return The input port pin value
 */
const GPIO_PinState GPIO_ReadPin(const GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin);

/**
 * @brief Set the specified GPIO pin
 * @param[in] GPIOx where x can be (A..F) to select the GPIO peripheral for STM32F0 family
 * @param[in] GPIO_Pin specifies the pin
 * 
 * @retval None
 */
void GPIO_SetPin(GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin);

/**
 * @brief Reset the specified GPIO pin
 * @param[in] GPIOx where x can be (A..F) to select the GPIO peripheral for STM32F0 family
 * @param[in] GPIO_Pin specifies the pin
 * 
 * @retval None
 */
void GPIO_ResetPin(GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin);

/**
 * @brief Toggle the specified GPIO pin
 * @param GPIOx where x can be (A..F) to select the GPIO peripheral for STM32F0 family
 * @param GPIO_Pin specifies the pin
 * 
 * @retval None
 */
void GPIO_TogglePin(GPIO_TypeDef * const GPIOx, const uint16_t GPIO_Pin);

///@}

#endif // __STM32F051_GPIO__
