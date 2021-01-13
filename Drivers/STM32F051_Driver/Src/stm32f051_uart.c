#include "stm32f051_uart.h"

struct stm32f051_uart
{
  uint8_t* pDataRx;
  uint16_t countRx;
  uint16_t sizeRx;

} uart1;

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

uint32_t UART1_WaitFlag(uint32_t Flag, FlagStatus Status, uint32_t Timeout)
{
  const uint32_t tickStart = ticks;
  while((READ_BIT(USART1->ISR, Flag) ? SET : RESET) == Status) 
  {
    if(ticks >= (tickStart + Timeout))
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

uint32_t UART1_Transmit(uint8_t *  pData, uint16_t Size, uint32_t Timeout)
{
  if ((pData == NULL) || (Size == 0U))
  {
    return 1;
  }
  for(int i = 0; i < Size; i++) 
  {
    if(UART1_WaitFlag(USART_ISR_TXE, RESET, Timeout)) 
    { 
      return 1; 
    }
    USART1->TDR = pData[i];
  }
  if (UART1_WaitFlag(USART_ISR_TC, RESET, Timeout)) 
  { 
    return 1; 
  }

  return 0;
}

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

__WEAK void UART1_RxCallback(void)
{

}

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