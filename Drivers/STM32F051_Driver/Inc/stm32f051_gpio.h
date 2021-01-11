#ifndef __STM32F051_GPIO__
#define __STM32F051_GPIO__

#include "stm32f0xx.h"

/** 
  * @brief  GPIO Bit SET and Bit RESET enumeration 
  */
typedef enum
{
  GPIO_PIN_RESET = 0U,
  GPIO_PIN_SET
} GPIO_PinState;

enum GPIO_Pin_N
{
  GPIO_PIN_0 = 0x0001U,  /* Pin 0 selected    */
  GPIO_PIN_1 = 0x0002U,  /* Pin 1 selected    */
  GPIO_PIN_2 = 0x0004U, /* Pin 2 selected    */
  GPIO_PIN_3 = 0x0008U,  /* Pin 3 selected    */
  GPIO_PIN_4 = 0x0010U,  /* Pin 4 selected    */
  GPIO_PIN_5 = 0x0020U,  /* Pin 5 selected    */
  GPIO_PIN_6 = 0x0040U,  /* Pin 6 selected    */
  GPIO_PIN_7 = 0x0080U,  /* Pin 7 selected    */
  GPIO_PIN_8 = 0x0100U,  /* Pin 8 selected    */
  GPIO_PIN_9 = 0x0200U,  /* Pin 9 selected    */
  GPIO_PIN_10 = 0x0400U,  /* Pin 10 selected   */
  GPIO_PIN_11 = 0x0800U,  /* Pin 11 selected   */
  GPIO_PIN_12 = 0x1000U,  /* Pin 12 selected   */
  GPIO_PIN_13 = 0x2000U,  /* Pin 13 selected   */
  GPIO_PIN_14 = 0x4000U,  /* Pin 14 selected   */
  GPIO_PIN_15 = 0x8000U,  /* Pin 15 selected   */
  GPIO_PIN_All = 0xFFFFU  /* All pins selected */
};

uint32_t GPIO_Init(void);

GPIO_PinState GPIO_ReadPin(GPIO_TypeDef* GPIO_Port, uint16_t GPIO_Pin);
void GPIO_SetPin(GPIO_TypeDef* GPIO_Port, uint16_t GPIO_Pin);
void GPIO_ResetPin(GPIO_TypeDef* GPIO_Port, uint16_t GPIO_Pin);
void GPIO_TogglePin(GPIO_TypeDef* GPIO_Port, uint16_t GPIO_Pin);

#endif // __STM32F051_GPIO__