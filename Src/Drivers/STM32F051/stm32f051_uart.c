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
 * @param[in] flag Specifies the UART flag to check
 * @param[in] status Flag status (SET or RESET)
 * @param[in] tickStart Tick start value
 * @param[in] timeout Timeout duration
 * 
 * @return Status
 * @retval 0 If no errors occurred
 * @retval 1 If an error occurs during the process
 */
const uint32_t UART_WaitFlag(const STM32F051_UART_t * const pUART, const uint32_t flag, 
  const FlagStatus status, const uint32_t tickStart, const uint32_t timeout)
{
  ASSERT(pUART);
  ASSERT(pUART->handle);

  while((READ_BIT(pUART->handle->ISR, flag) ? SET : RESET) == status) 
  {
    if(GetTick() >= (tickStart + timeout))
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
const uint32_t UART_Init(const STM32F051_UART_t * const pUART)
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
__WEAK void UART_RxCallback(STM32F051_UART_t * const pUART)
{

}

// Exported Function ---------------------------------------------------------
const uint32_t UART1_Init(STM32F051_UART_t * const pUART) 
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

const uint32_t UART2_Init(STM32F051_UART_t * const pUART) 
{
  ASSERT(pUART);

  SET_BIT(RCC->APB1ENR, RCC_APB1ENR_USART2EN);
  SET_BIT(RCC->AHBENR, RCC_AHBENR_GPIOAEN);

  MODIFY_REG(GPIOA->OSPEEDR, 0, GPIO_OSPEEDER_OSPEEDR2);
  CLEAR_BIT(GPIOA->OTYPER, GPIO_OTYPER_OT_2);
  MODIFY_REG(GPIOA->PUPDR, GPIO_PUPDR_PUPDR2, 0);
  MODIFY_REG(GPIOA->AFR[0], 0xE << GPIO_AFRL_AFSEL2_Pos, 1 << GPIO_AFRL_AFSEL2_Pos);
  MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODER2_0, GPIO_MODER_MODER2_1);

  MODIFY_REG(GPIOA->OSPEEDR, 0, GPIO_OSPEEDER_OSPEEDR3);
  CLEAR_BIT(GPIOA->OTYPER, GPIO_OTYPER_OT_3);
  MODIFY_REG(GPIOA->PUPDR, GPIO_PUPDR_PUPDR3, 0);
  MODIFY_REG(GPIOA->AFR[0], 0xE << GPIO_AFRL_AFSEL3_Pos, 1 << GPIO_AFRL_AFSEL3_Pos);
  MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODER3_0, GPIO_MODER_MODER3_1);

  NVIC_SetPriority(USART2_IRQn, 0);
  NVIC_EnableIRQ(USART2_IRQn);

  pUART->handle = USART2;
  pUART->baudRate = 115200;
  pUART->pRxCall = UART_RxCallback;

  return UART_Init(pUART);
}

const uint32_t UART_Transmit(STM32F051_UART_t * const pUART, 
  const uint8_t * pData, uint16_t size, const uint32_t timeout)
{
  ASSERT(pUART);
  ASSERT(pUART->handle);
  ASSERT(pData);
  ASSERT(size > 0U);

  const uint32_t tickStart = GetTick();
  while((size--) > 0)
  {
    if(UART_WaitFlag(pUART, USART_ISR_TXE, RESET, tickStart, timeout) != 0) 
    { 
      return 1; 
    }
    pUART->handle->TDR = *(pData++);
  }
  if (UART_WaitFlag(pUART, USART_ISR_TC, RESET, tickStart, timeout) != 0) 
  { 
    return 1; 
  }

  return 0;
}

const uint32_t UART_ReceiveIT(STM32F051_UART_t * const pUART, 
  uint8_t * const pData, const uint16_t size)
{
  ASSERT(pUART);
  ASSERT(pUART->handle);
  ASSERT(pData);
  ASSERT(size > 0U);

  pUART->pDataRx = pData;
  pUART->sizeRx = pUART->countRx = size;

  if(READ_BIT(pUART->handle->CR1, USART_CR1_RE) != 0)
  {
    SET_BIT(pUART->handle->CR1, USART_CR1_RTOIE);
  }

  SET_BIT(pUART->handle->CR3, USART_CR3_EIE);

  SET_BIT(pUART->handle->CR1, USART_CR1_PEIE | USART_CR1_RXNEIE);

  return 0;
}

void UART_IRQHandler(STM32F051_UART_t * const pUART)
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
