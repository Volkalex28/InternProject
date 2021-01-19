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

#ifndef __STM32F051_UART__
#define __STM32F051_UART__

#include "stm32f0xx.h"
#include <stddef.h>

// Exported types --------------------------------------------------------------
/** @defgroup UART_Exported_Types UART/USART Exported Types
 * @ingroup UART
 * 
 * Declaring types available from other files
 * @{
 */

/**
 * @brief Declaring a structure to work with USART
 * 
 */
typedef struct 
{
  USART_TypeDef* handle;  ///< Pointer to the UART module peripheral
  uint32_t baudRate;      ///< Speed of the UART module
  
  uint8_t* pDataRx;       ///< Pointer to the data area where the received data should be placed
  uint16_t countRx;       ///< Counter of bytes remaining until the end of reception
  uint16_t sizeRx;        ///< Amount of data to be received

  void (*pRxCall)(void);  ///< Pointer to Callback function of reception on UART module
} STM32F051_UART_t;
///@}

// Exported variables ----------------------------------------------------------
/** @defgroup UART_Exported_Variables UART/USART Exported Variables
 * @ingroup UART
 * 
 * Variables available from other files
 * @{
 */

/**
 * @brief Variables to work with USART1
 * 
 * Used to save the parameters USART1
 */
extern STM32F051_UART_t uart1;
///@}

// Exported Function ---------------------------------------------------------
/** @addtogroup UART_Exported_Function
 * @ingroup UART
 * @{
 */

uint32_t UART1_Init(void);
uint32_t UART_Transmit(const STM32F051_UART_t* pUART, uint8_t* pData, uint16_t Size, const uint32_t Timeout);
uint32_t UART_ReceiveIT(STM32F051_UART_t* pUART, uint8_t *pData, uint16_t Size);

void UART_IRQHandler(STM32F051_UART_t* pUART);
///@}

// CallBack Function ---------------------------------------------------------
/** @addtogroup UART_CallBack_Function
 * @ingroup UART
 * @{
 */

void UART1_RxCallback(void);
///@}

#endif // __STM32F051_UART__
