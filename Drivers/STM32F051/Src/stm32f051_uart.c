/**
 * @file stm32f051_uart.c
 * @author Oleksandr Ananiev (alexander.ananiev@sigma.sofware)
 * @brief Source file of UART module driver
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

#include "stm32f051_uart.h"
#include "stm32f051_rcc.h"
#include "user_assert.h"

// Private Function ---------------------------------------------------------
/** @defgroup UART_Private_Function Private Function
 * @ingroup UART
 * 
 * Functions required for the needs of the UART module driver
 * @{
 */

/**
 * @brief Handle USART Communication Timeout
 * 
 * @param[in] pUART Pointer to UART module object
 * @param[in] Flag Specifies the UART1 flag to check
 * @param[in] Status Flag status (SET or RESET)
 * @param[in] tickStart Tick start value
 * @param[in] Timeout Timeout duration
 * 
 * @return Status
 * @retval 0 If no errors occurred
 * @retval 1 If an error occurs during the process
 */
uint32_t UART_WaitFlag(const STM32F051_UART_t* pUART, const uint32_t Flag, 
  const FlagStatus Status, const uint32_t tickStart, const uint32_t Timeout)
{
  ASSERT(pUART);
  ASSERT(pUART->handle);

  while((READ_BIT(pUART->handle->ISR, Flag) ? SET : RESET) == Status) 
  {
    if(GetTick() >= (tickStart + Timeout))
    {
      CLEAR_BIT(pUART->handle->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE | USART_CR1_TXEIE));
      CLEAR_BIT(pUART->handle->CR3, USART_CR3_EIE);

      return 1;
    }

    if(READ_BIT(pUART->handle->CR1, USART_CR1_RE) != 0)
    {
      if(READ_BIT(pUART->handle->ISR, USART_ISR_RTOF) != 0)
      {
        CLEAR_BIT(pUART->handle->ICR, USART_ICR_RTOCF);

        CLEAR_BIT(pUART->handle->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE | USART_CR1_TXEIE));
        CLEAR_BIT(pUART->handle->CR3, USART_CR3_EIE);

        return 1;
      }
    }
  }
  return 0;
}

/**
 * @brief Initialization of peripheral UART module
 * 
 * @param[in] pUART Pointer to UART module object
 * 
 * @return UART initialization status
 * @retval 0 If initialization completed successfully
 * @retval 1 If an error occurs during the process
 */
uint32_t UART_Init(const STM32F051_UART_t* pUART)
{
  ASSERT(pUART);
  ASSERT(pUART->handle);
  ASSERT(pUART->baudRate <= 256000U);

  CLEAR_BIT(pUART->handle->CR1, USART_CR1_UE);

  MODIFY_REG(pUART->handle->CR1, 
    USART_CR1_M | USART_CR1_PCE | USART_CR1_PS | USART_CR1_OVER8, 
    USART_CR1_RE | USART_CR1_TE);
  MODIFY_REG(pUART->handle->CR2, USART_CR2_STOP, 0);
  MODIFY_REG(pUART->handle->CR3, USART_CR3_RTSE | USART_CR3_CTSE | USART_CR3_ONEBIT, 0);

  const uint32_t usartdiv = ( 48000000 + pUART->baudRate / 2U ) / pUART->baudRate;
  WRITE_REG(pUART->handle->BRR, (uint16_t)(usartdiv & 0xFFF0U) + 
    (uint16_t)((usartdiv & (uint16_t)0x000FU) >> 1U));

  CLEAR_BIT(pUART->handle->CR2, (USART_CR2_LINEN | USART_CR2_CLKEN));
  CLEAR_BIT(pUART->handle->CR3, (USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN));

  SET_BIT(pUART->handle->CR1, USART_CR1_UE);

  return 0;
}
///@}

// CallBack Function ---------------------------------------------------------
/** @defgroup UART_CallBack_Function CallBack Function
 * @ingroup UART
 * 
 * UART module function callback
 * @{
 */

/**
 * @brief Callback at the end of receiving data via USART1
 * 
 * The function is declared __weak, since it is assumed that 
 * the user will override it
 * 
 * @param[in] pUART Pointer to UART module object
 * 
 * @retval None
 */
__WEAK void UART_RxCallback(STM32F051_UART_t* pUART)
{

}
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

uint32_t UART1_Init(STM32F051_UART_t* pUART) 
{
  ASSERT(pUART);

  SET_BIT(RCC->APB2ENR, RCC_APB2ENR_USART1EN);
  SET_BIT(RCC->AHBENR, RCC_AHBENR_GPIOAEN);

  MODIFY_REG(GPIOA->OSPEEDR, 0, GPIO_OSPEEDER_OSPEEDR9);
  CLEAR_BIT(GPIOA->OTYPER, GPIO_OTYPER_OT_9);
  MODIFY_REG(GPIOA->PUPDR, GPIO_PUPDR_PUPDR9, 0);
  MODIFY_REG(GPIOA->AFR[1], 0xE << GPIO_AFRH_AFSEL9_Pos, 1 << GPIO_AFRH_AFSEL9_Pos);
  MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODER9_0, GPIO_MODER_MODER9_1);

  MODIFY_REG(GPIOA->OSPEEDR, 0, GPIO_OSPEEDER_OSPEEDR10);
  CLEAR_BIT(GPIOA->OTYPER, GPIO_OTYPER_OT_10);
  MODIFY_REG(GPIOA->PUPDR, GPIO_PUPDR_PUPDR10, 0);
  MODIFY_REG(GPIOA->AFR[1], 0xE << GPIO_AFRH_AFSEL10_Pos, 1 << GPIO_AFRH_AFSEL10_Pos);
  MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODER10_0, GPIO_MODER_MODER10_1);

  NVIC_SetPriority(USART1_IRQn, 0);
  NVIC_EnableIRQ(USART1_IRQn);

  pUART->handle = USART1;
  pUART->baudRate = 19200;
  pUART->pRxCall = UART_RxCallback;

  return UART_Init(pUART);
}

/**
 * @brief  Function for sending via USART interface in blocking mode
 * 
 * The function sends data via the USART1 interface from the @p pData 
 * memory area in the amount of @p Size
 * 
 * @param[in] pUART Pointer to UART module object
 * @param[in] pData Pointer to data buffer
 * @param[in] Size Amount of data elements to be sent
 * @param[in] Timeout Timeout duration
 * 
 * @return UART Transmit status
 * @retval 0 If the sending was successful
 * @retval 1 If an error occurred during sending
 */
uint32_t UART_Transmit(const STM32F051_UART_t* pUART, uint8_t* pData, uint16_t Size, const uint32_t Timeout)
{
  ASSERT(pUART);
  ASSERT(pUART->handle);
  ASSERT(pData);
  ASSERT(Size > 0U);

  const uint32_t tickStart = GetTick();
  while((Size--) > 0)
  {
    if(UART_WaitFlag(pUART, USART_ISR_TXE, RESET, tickStart, Timeout) != 0) 
    { 
      return 1; 
    }
    pUART->handle->TDR = *(pData++);
  }
  if (UART_WaitFlag(pUART, USART_ISR_TC, RESET, tickStart, Timeout) != 0) 
  { 
    return 1; 
  }

  return 0;
}

/**
 * @brief Function for receive via USART1 interface in interrupt mode
 * 
 * The function configures the USART1 module to receive @p Size bytes, 
 * place them in the @p pData memory area, and then trigger an interrupt.
 * 
 * @note Interrupt handler: UART_IRQHandler()
 * @note Callback at the end of reception: UART_RxCallback()
 * 
 * @param[in] pUART Pointer to UART module object
 * @param[out] pData Pointer to data buffer
 * @param[in] Size Amount of data elements to be received
 * 
 * @return USART Receive status
 * @retval 0 If the reception was successful
 * @retval 1 If an error occurs during reception
 */
uint32_t UART_ReceiveIT(STM32F051_UART_t* pUART, uint8_t *pData, uint16_t Size)
{
  ASSERT(pUART);
  ASSERT(pUART->handle);
  ASSERT(pData);
  ASSERT(Size > 0U);

  pUART->pDataRx = pData;
  pUART->sizeRx = pUART->countRx = Size;

  if(READ_BIT(pUART->handle->CR1, USART_CR1_RE) != 0)
  {
    SET_BIT(pUART->handle->CR1, USART_CR1_RTOIE);
  }

  SET_BIT(pUART->handle->CR3, USART_CR3_EIE);

  SET_BIT(pUART->handle->CR1, USART_CR1_PEIE | USART_CR1_RXNEIE);

  return 0;
}

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
void UART_IRQHandler(STM32F051_UART_t* pUART)
{
  ASSERT(pUART);
  ASSERT(pUART->handle);

  const uint32_t isrflags   = READ_REG(pUART->handle->ISR);
  const uint32_t cr1its     = READ_REG(pUART->handle->CR1);

  if(READ_BIT(isrflags, USART_ISR_PE | USART_ISR_FE | \
      USART_ISR_ORE | USART_ISR_NE | USART_ISR_RTOF) == 0)
  {
    if((READ_BIT(isrflags, USART_ISR_RXNE) != 0) && 
      (READ_BIT(cr1its, USART_CR1_RXNEIE) != 0)) 
    {
      // Receive Handler
      uint16_t  data = READ_REG(pUART->handle->RDR);

      *(pUART->pDataRx++) = (uint8_t)data;
      pUART->countRx--;  

      if(pUART->countRx == 0)
      {
        CLEAR_BIT(pUART->handle->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));
        CLEAR_BIT(pUART->handle->CR3, USART_CR3_EIE);

        if(pUART->pRxCall != NULL)
        {
          pUART->pRxCall(pUART);
        }
      }
    }
  }
}
///@}
