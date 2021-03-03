/**
 * @file uart.h
 * @author Oleksandr Ananiev (alexander.ananiev@sigma.sofware)
 * @brief 
 * @version 0.1
 * @date 2021-02-12
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _UART_H_
#define _UART_H_

#include <stm32f051_uart.h>

/**
 * 
 * @addtogroup UART_Ini UART
 * @ingroup Core
 * 
 * @{
 */

/**
 * @brief Initialization of the UART1 module and its periphery
 * 
 * The function adjusts the pins of the PA9 and PA10 microcontroller 
 * to work with the UART1 module, configures the module itself with 
 * the following parameters: 
 * @verbatim
  Speed:     9600
  Parity:    None
  Data bits: 8
  Stop bits: 1
  @endverbatim
 * 
 * @param[in, out] pUART Pointer to object of UART1 module
 * 
 * @return UART1 initialization status
 * @retval 0 If initialization was successful.
 * @retval 1 If an initialization error occurs. 
 */
const uint32_t UART1_Init(STM32F051_UART_t * const pUART);

/**
 * @brief Initialization of the UART2 module and its periphery
 * 
 * The function adjusts the pins of the PA2 and PA3 microcontroller 
 * to work with the UART2 module, configures the module itself with 
 * the following parameters: 
 * @verbatim
  Speed:     115200
  Parity:    None
  Data bits: 8
  Stop bits: 1
  @endverbatim
 * 
 * @param[in, out] pUART Pointer to object of UART2 module
 * 
 * @return UART2 initialization status
 * @retval 0 If initialization was successful.
 * @retval 1 If an initialization error occurs. 
 */
const uint32_t UART2_Init(STM32F051_UART_t * const pUART);

///@}

#endif // _UART_H_
