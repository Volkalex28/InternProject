/**
 * @file stm32f051_i2c.h
 * @author Oleksandr Ananiev (alexander.ananiev@sigma.sofware)
 * @brief Header file of I2C module driver
 * @version 0.1
 * @date 2021-01-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __STM32F051_I2C__
#define __STM32F051_I2C__

#include "stm32f0xx.h"

// Exported Function ---------------------------------------------------------
/**
 * @addtogroup I2C_Exported_Function
 * @{
 */
uint32_t I2C1_Transmit(const uint16_t DevAddress, uint8_t *pData, uint16_t Size, const uint32_t Timeout);
uint32_t I2C1_Receive(const uint16_t DevAddress, uint8_t *pData, uint16_t Size, const uint32_t Timeout);
uint32_t I2C1_Init(void);
///@}

#endif // __STM32F051_I2C__
