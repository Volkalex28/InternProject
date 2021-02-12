/**
 * @file stm32f051_uart.h
 * @author Oleksandr Ananiev (alexander.ananiev@sigma.sofware)
 * @brief Header file of UART module driver
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
 * @defgroup UART UART
 * @ingroup STM32f051_Driver
 * @brief UART module driver
 */

#ifndef __STM32F051_UART__
#define __STM32F051_UART__

#include "stm32f0xx.h"
#include <stddef.h>

// Exported types --------------------------------------------------------------
/** @defgroup UART_Exported_Types Exported Types
 * @ingroup UART
 * 
 * Declaring types available from other files
 * @{
 */

/**
 * @brief Declaring a structure to work with USART
 * 
 */
typedef struct sSTM32F051_UART_t
{
  USART_TypeDef * handle;  ///< Pointer to the UART module peripheral
  uint32_t baudRate;      ///< Speed of the UART module
  
  uint8_t * pDataRx;       ///< Pointer to the data area where the received data should be placed
  uint16_t countRx;       ///< Counter of bytes remaining until the end of reception
  uint16_t sizeRx;        ///< Amount of data to be received

  void (*pRxCall)(struct sSTM32F051_UART_t *);  ///< Pointer to Callback function of reception on UART module
} STM32F051_UART_t;
///@}

// Exported Function ---------------------------------------------------------
/** @defgroup UART_Exported_Function Exported Function
 * @ingroup UART
 * 
 * UART module function available from other files
 * @{
 */

/**
 * @brief Initialization of the UART1 module and its periphery
 * 
 * The function adjusts the pins of the PA9 and PA10 microcontroller 
 * to work with the UART1 module, configures the module itself with 
 * the following parameters: 
 * @verbatim
  Speed:     19200
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

/**
 * @brief  Send function via @p pUART interface in blocking mode
 * 
 * The function sends data via the USART interface at the @p pUART pointer 
 * from the @p pData memory area in @p Size
 * 
 * @param[in] pUART Pointer to UART module object
 * @param[in] pData Pointer to data buffer
 * @param[in] size Amount of data elements to be sent
 * @param[in] timeout Timeout duration
 * 
 * @return UART Transmit status
 * @retval 0 If the sending was successful
 * @retval 1 If an error occurred during sending
 */
const uint32_t UART_Transmit(STM32F051_UART_t * const pUART, 
  const uint8_t * pData, uint16_t size, const uint32_t timeout);

/**
 * @brief Receive function via interface @p pUART in interrupt mode
 * 
 * The function configures the module at the @p pUART pointer to receive bytes of @p size
 * , place them in the @p pData memory area, and then trigger an interrupt.
 * 
 * @note Interrupt handler: UART_IRQHandler()
 * @note Callback at the end of reception: UART_RxCallback()
 * 
 * @param[in] pUART Pointer to UART module object
 * @param[out] pData Pointer to data buffer
 * @param[in] size Amount of data elements to be received
 * 
 * @return UART Receive status
 * @retval 0 If the reception was successful
 * @retval 1 If an error occurs during reception
 */
const uint32_t UART_ReceiveIT(STM32F051_UART_t * const pUART, 
  uint8_t * const pData, const uint16_t size);

/**
 * @brief Handle USART interrupt request
 * 
 * The function handles the interrupt from USART and calls 
 * the callback function depending on the interrupt source
 * 
 * @param[in] pUART Pointer to UART module object
 * 
 * @retval None
 */
void UART_IRQHandler(STM32F051_UART_t * const pUART);
///@}

// CallBack Function ---------------------------------------------------------
/** @defgroup UART_CallBack_Function CallBack Function
 * @ingroup UART
 * 
 * UART module function callback
 * @{
 */

/**
 * @brief Callback at the end of receiving data via USART interface
 * 
 * The function is declared __weak, since it is assumed that 
 * the user will override it
 * 
 * @param[in] pUART Pointer to UART module object
 * 
 * @retval None
 */
void UART_RxCallback(STM32F051_UART_t* pUART);

///@}

#endif // __STM32F051_UART__
