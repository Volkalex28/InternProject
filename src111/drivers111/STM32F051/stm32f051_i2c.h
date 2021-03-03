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

/**
 * @addtogroup Drivers
 * @addtogroup STM32f051_Driver STM32F051
 * @ingroup Drivers
 */

/**
 * @defgroup I2C I2C
 * @ingroup STM32f051_Driver
 * @brief I2C module driver
 */

#ifndef __STM32F051_I2C__
#define __STM32F051_I2C__

#include <stm32f0xx.h>

// Exported Function ---------------------------------------------------------
/** @defgroup I2C_Exported_Function Exported Function
 * @ingroup I2C
 * 
 * I2C module function available from other files
 * @{
 */

/**
 * @brief Function for sending via I2C1 interface in blocking mode
 * 
 * The function sends data via the I2S1 interface from
 * the memory area @p pData to the device at the address @p DevAddress
 * in the amount of @p Size
 * 
 * @param[in] DevAddress The address of the device to contact
 * @param[in] pData Memory area where to take data to send
 * @param[in] Size The amount of data to be sent to the device
 * @param[in] Timeout Device timeout in milliseconds
 * @return I2C1 Transmit status
 * @retval 0 If the sending was successful
 * @retval 1 If an error occurred during sending
 */
const uint32_t I2C1_Transmit(const uint16_t DevAddress, uint8_t * pData, uint16_t Size, const uint32_t Timeout);

/**
 * @brief Data reception function via I2C1 interface in blocking mode
 * 
 * The function reads data via the I2C1 interface from the device at the
 * address @p DevAddress and places them in the memory area @p pData 
 * in the amount of @p Size
 * 
 * @param[in] DevAddress The address of the device to contact
 * @param[out] pData Memory area where the received data should be placed
 * @param[in] Size The number of bytes to get from the device
 * @param[in] Timeout Device timeout in milliseconds
 * @return I2C1 Receive status
 * @retval 0 If the reception was successful
 * @retval 1 If an error occurs during reception
 */
const uint32_t I2C1_Receive(const uint16_t DevAddress, uint8_t *pData, uint16_t Size, const uint32_t Timeout);

/**
 * @brief Initialization of the I2C1 module and its periphery
 * 
 * The function adjusts the PB6 and PB7 microcontroller legs to work 
 * with the I2C1 module, configures the module itself to a frequency 
 * of 400 KHz, and also disables the analog and digital filters
 * 
 * @return I2C1 initialization status
 * @retval 0 If initialization was successful.
 * @retval 1 If an initialization error occurs. 
 */
const uint32_t I2C1_Init(void);

///@}

#endif // __STM32F051_I2C__
