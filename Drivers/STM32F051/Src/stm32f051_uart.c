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
 * @addtogroup STM32f051_Driver Driver for STM32F051
 */

/**
 * @defgroup UART UART/USART
 * @ingroup STM32f051_Driver
 * @brief UART module driver
 */

#include "stm32f051_uart.h"
#include "stm32f051_rcc.h"

// Private types --------------------------------------------------------------
/** @defgroup UART_Private_Types UART/USART Private Types
 * @ingroup UART
 * 
 * Declaring the types required for the needs of the UART module driver
 * @{
 */

/**
 * @brief Declaring a structure to work with USART1
 * 
 */
typedef struct 
{
  uint8_t* pDataRx; ///< Pointer to the data area where the received data should be placed
  uint16_t countRx; ///< Counter of bytes remaining until the end of reception
  uint16_t sizeRx;  ///< Amount of data to be received
} STM32F051_UART_t;
///@}

// Private variables ----------------------------------------------------------
/** @defgroup UART_Private_Variables UART/USART Private Variables
 * @ingroup UART
 * 
 * Variables required for the needs of the UART module driver
 * @{
 */

/**
 * @brief Variables to work with USART1
 * 
 * Used to save the parameters of receiving data via USART1 in interrupt mode
 */
STM32F051_UART_t uart1;
///@}

// Private Function ---------------------------------------------------------
/** @defgroup UART_Private_Function UART/USART Private Function
 * @ingroup UART
 * 
 * Functions required for the needs of the UART module driver
 * @{
 */

/**
 * @brief Handle USART1 Communication Timeout
 * 
 * @param[in] Flag Specifies the UART1 flag to check
 * @param[in] Status Flag status (SET or RESET)
 * @param[in] tickStart Tick start value
 * @param[in] Timeout Timeout duration
 * 
 * @return Status
 * @retval 0 If no errors occurred
 * @retval 1 If an error occurs during the process
 */
uint32_t UART1_WaitFlag(const uint32_t Flag, const FlagStatus Status,
  const uint32_t tickStart, const uint32_t Timeout)
{
  while((READ_BIT(USART1->ISR, Flag) ? SET : RESET) == Status) 
  {
    if(GetTick() >= (tickStart + Timeout))
    {
      CLEAR_BIT(USART1->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE | USART_CR1_TXEIE));
      CLEAR_BIT(USART1->CR3, USART_CR3_EIE);

      return 1;
    }

    if(READ_BIT(USART1->CR1, USART_CR1_RE) != 0)
    {
      if(READ_BIT(USART1->ISR, USART_ISR_RTOF) != 0)
      {
        CLEAR_BIT(USART1->ICR, USART_ICR_RTOCF);

        CLEAR_BIT(USART1->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE | USART_CR1_TXEIE));
        CLEAR_BIT(USART1->CR3, USART_CR3_EIE);

        return 1;
      }
    }
  }
  return 0;
}
///@}

// CallBack Function ---------------------------------------------------------
/** @defgroup UART_CallBack_Function UART/USART CallBack Function
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
 * @retval None
 */
__WEAK void UART1_RxCallback(void)
{

}
///@}

// Exported Function ---------------------------------------------------------
/** @defgroup UART_Exported_Function UART/USART Exported Function
 * @ingroup UART
 * 
 * UART module function available from other files
 * @{
 */

/**
 * @brief Initialization of the UART1 module and its periphery
 * 
 * The function adjusts the legs of the PA9 and PA10 microcontroller 
 * to work with the UART1 module, configures the module itself with 
 * the following parameters: 
 * @verbatim
  Speed:     19200
  Parity:    None
  Data bits: 8
  Stop bits: 1
  @endverbatim
 * 
 * @return UART1 init status
 * @retval 0 If initialization was successful.
 * @retval 1 If an initialization error occurs. 
 */
uint32_t UART1_Init(void) 
{
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


  CLEAR_BIT(USART1->CR1, USART_CR1_UE);

  MODIFY_REG(USART1->CR1, USART_CR1_M | USART_CR1_PCE | USART_CR1_PS | 
    USART_CR1_TE | USART_CR1_RE | USART_CR1_OVER8, USART_CR1_RE | USART_CR1_TE);
  MODIFY_REG(USART1->CR2, USART_CR2_STOP, 0);
  MODIFY_REG(USART1->CR3, USART_CR3_RTSE | USART_CR3_CTSE | USART_CR3_ONEBIT, 0);

  const uint32_t usartdiv = ( 48000000 + 19200 / 2U ) / 19200;
  WRITE_REG(USART1->BRR, (uint16_t)(usartdiv & 0xFFF0U) + 
    (uint16_t)((usartdiv & (uint16_t)0x000FU) >> 1U));

  CLEAR_BIT(USART1->CR2, (USART_CR2_LINEN | USART_CR2_CLKEN));
  CLEAR_BIT(USART1->CR3, (USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN));

  SET_BIT(USART1->CR1, USART_CR1_UE);

  return 0;
}

/**
 * @brief  Function for sending via USART1 interface in blocking mode
 * 
 * The function sends data via the USART1 interface from the @p pData 
 * memory area in the amount of @p Size
 * 
 * @param[in] pData Pointer to data buffer
 * @param[in] Size Amount of data elements to be sent
 * @param[in] Timeout Timeout duration
 * 
 * @return I2C1 Transmit status
 * @retval 0 If the sending was successful
 * @retval 1 If an error occurred during sending
 */
uint32_t UART1_Transmit(uint8_t *  pData, uint16_t Size, const uint32_t Timeout)
{
  const uint32_t tickStart = GetTick();
  if ((pData == NULL) || (Size == 0U))
  {
    return 1;
  }
  while((Size--) > 0)
  {
    if(UART1_WaitFlag(USART_ISR_TXE, RESET, tickStart, Timeout)) 
    { 
      return 1; 
    }
    USART1->TDR = *(pData++);
  }
  if (UART1_WaitFlag(USART_ISR_TC, RESET, tickStart, Timeout)) 
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
 * @note Interrupt handler: UART1_IRQHandler()
 * @note Callback at the end of reception: UART1_RxCallback()
 * 
 * @param[out] pData Pointer to data buffer
 * @param[in] Size Amount of data elements to be received
 * 
 * @return USART1 Receive status
 * @retval 0 If the reception was successful
 * @retval 1 If an error occurs during reception
 */
uint32_t UART1_ReceiveIT(uint8_t *pData, uint16_t Size)
{
  if ((pData == NULL) || (Size == 0U))
  {
    return 1;
  }

  uart1.pDataRx = pData;
  uart1.sizeRx = uart1.countRx = Size;

  if(READ_BIT(USART1->CR1, USART_CR1_RE) != 0)
  {
    SET_BIT(USART1->CR1, USART_CR1_RTOIE);
  }

  SET_BIT(USART1->CR3, USART_CR3_EIE);

  SET_BIT(USART1->CR1, USART_CR1_PEIE | USART_CR1_RXNEIE);

  return 0;
}

/**
 * @brief Handle USART1 interrupt request
 * 
 * The function handles the interrupt from USART1 and calls 
 * the callback function depending on the interrupt source
 * 
 * @retval None
 */
void UART1_IRQHandler(void)
{
  const uint32_t isrflags   = READ_REG(USART1->ISR);
  const uint32_t cr1its     = READ_REG(USART1->CR1);

  if(READ_BIT(isrflags, USART_ISR_PE | USART_ISR_FE | \
      USART_ISR_ORE | USART_ISR_NE | USART_ISR_RTOF) == 0)
  {
    if((READ_BIT(isrflags, USART_ISR_RXNE) != 0) && 
      (READ_BIT(cr1its, USART_CR1_RXNEIE) != 0)) 
    {
      // Receive Handler
      uint16_t  data = READ_REG(USART1->RDR);

      *(uart1.pDataRx++) = (uint8_t)data;
      uart1.countRx--;  

      if(uart1.countRx == 0)
      {
        CLEAR_BIT(USART1->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));
        CLEAR_BIT(USART1->CR3, USART_CR3_EIE);

        UART1_RxCallback();
      }
    }
  }
}
///@}
