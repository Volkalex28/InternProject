#include "stm32f051_i2c.h"

uint32_t I2C_Init(void) 
{
  SET_BIT(RCC->AHBENR, RCC_AHBENR_GPIOBEN);

  MODIFY_REG(GPIOB->OSPEEDR, 0, GPIO_OSPEEDER_OSPEEDR6);
  SET_BIT(GPIOB->OTYPER, GPIO_OTYPER_OT_6);
  MODIFY_REG(GPIOB->PUPDR, GPIO_PUPDR_PUPDR6, 0);
  MODIFY_REG(GPIOB->AFR[0], 0xE << GPIO_AFRL_AFSEL6_Pos, 1 << GPIO_AFRL_AFSEL6_Pos);
  MODIFY_REG(GPIOB->MODER, GPIO_MODER_MODER6_0, GPIO_MODER_MODER6_1);

  MODIFY_REG(GPIOB->OSPEEDR, 0, GPIO_OSPEEDER_OSPEEDR7);
  SET_BIT(GPIOB->OTYPER, GPIO_OTYPER_OT_7);
  MODIFY_REG(GPIOB->PUPDR, GPIO_PUPDR_PUPDR7, 0);
  MODIFY_REG(GPIOB->AFR[0], 0xE << GPIO_AFRL_AFSEL7_Pos, 1 << GPIO_AFRL_AFSEL7_Pos);
  MODIFY_REG(GPIOB->MODER, GPIO_MODER_MODER7_0, GPIO_MODER_MODER7_1);

  SET_BIT(RCC->APB1ENR, RCC_APB1ENR_I2C1EN);

  CLEAR_BIT(I2C1->CR1,  I2C_CR1_PE);

  WRITE_REG(I2C1->TIMINGR, 0x0000020B & (0xF0FFFFFFU));
  WRITE_REG(I2C1->OAR1, I2C_OAR1_OA1EN);
  SET_BIT(I2C1->CR2, I2C_CR2_AUTOEND | I2C_CR2_NACK);
  WRITE_REG(I2C1->OAR2, 0);
  WRITE_REG(I2C1->CR1, 0);

  SET_BIT(I2C1->CR1,  I2C_CR1_PE);

  CLEAR_BIT(I2C1->CR1, I2C_CR1_ANFOFF);
  CLEAR_BIT(I2C1->CR1, I2C_CR1_DNF);

  return 0;
}

uint32_t I2C_WaitFlag(uint32_t Flag, FlagStatus Status, uint32_t Timeout)
{
  const uint32_t tickStart = ticks;
  while((READ_BIT(I2C1->ISR, Flag) ? SET : RESET) == Status) 
  {
    if(ticks >= (tickStart + Timeout))
    {
      return 1;
    }
  }
  return 0;
}

uint32_t I2C_IsAcknowledgeFailed(uint32_t Timeout)
{
  const uint32_t tickStart = ticks;
  if((READ_BIT(I2C1->ISR, I2C_ISR_NACKF) ? SET : RESET) == SET)
  {
    while((READ_BIT(I2C1->ISR, I2C_ISR_STOPF) ? SET : RESET) == RESET)
    {
      if(ticks >= (tickStart + Timeout))
      {
        return 1;
      }
    }
    SET_BIT(I2C1->ICR, I2C_ICR_NACKCF);
    SET_BIT(I2C1->ICR, I2C_ICR_STOPCF);

    if((READ_BIT(I2C1->ISR, I2C_ISR_TXIS) ? SET : RESET) != RESET)
    {
      I2C1->TXDR = 0;
    }
    if((READ_BIT(I2C1->ISR, I2C_ISR_TXE) ? SET : RESET) == RESET)
    {
      SET_BIT(I2C1->ISR, I2C_ISR_TXE);
    }
    CLEAR_BIT(I2C1->CR2, I2C_CR2_SADD | I2C_CR2_HEAD10R | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_RD_WRN);

    return 1;
  }
  return 0;
}

uint32_t I2C_WaitRXNEFlag(uint32_t Timeout)
{
  const uint32_t tickStart = ticks;
  while((READ_BIT(I2C1->ISR, I2C_ISR_RXNE) ? SET : RESET) == RESET)
  {
    if(I2C_IsAcknowledgeFailed(Timeout))
    {
      return 1;
    }

    if((READ_BIT(I2C1->ISR, I2C_ISR_STOPF) ? SET : RESET) == SET)
    {
      if((READ_BIT(I2C1->ISR, I2C_ISR_RXNE) ? SET : RESET) == SET)
      {
        return 0;
      }
      else
      {
        SET_BIT(I2C1->ICR, I2C_ICR_STOPCF);
        CLEAR_BIT(I2C1->CR2, I2C_CR2_SADD | I2C_CR2_HEAD10R | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_RD_WRN);
        return 1;
      }
      
    }

    if(ticks >= (tickStart + Timeout))
    {
      return 1;
    }
  }
  return 0;
}

uint32_t I2C_WaitTXISFlag(uint32_t Timeout)
{
  const uint32_t tickStart = ticks;
  while((READ_BIT(I2C1->ISR, I2C_ISR_TXIS) ? SET : RESET) == RESET)
  {
    if(I2C_IsAcknowledgeFailed(Timeout))
    {
      return 1;
    }

    if(ticks >= (tickStart + Timeout))
    {
      return 1;
    }
  }
  return 0;
}

uint32_t I2C_WaitStopFlag(uint32_t Timeout)
{
  const uint32_t tickStart = ticks;
  while((READ_BIT(I2C1->ISR, I2C_ISR_STOPF) ? SET : RESET) == RESET) 
  {
    if(ticks >= (tickStart + Timeout))
    {
      return 1;
    }

    if(I2C_IsAcknowledgeFailed(Timeout))
    {
      return 1;
    }
    
  }
  return 0;
}

uint32_t I2C_Receive(uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  if(I2C_WaitFlag(I2C_ISR_BUSY, SET, 50))
  {
    return 1;
  }
  const uint32_t Request = (uint32_t)(0x80000000U | I2C_CR2_START | I2C_CR2_RD_WRN);
  MODIFY_REG(I2C1->CR2, (I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_STOP |
    (I2C_CR2_RD_WRN & (uint32_t)(Request >> (31U - I2C_CR2_RD_WRN_Pos)))),
    ((uint32_t)DevAddress & I2C_CR2_SADD) | (((uint32_t)Size << I2C_CR2_NBYTES_Pos) & I2C_CR2_NBYTES) | 
    Request | (uint32_t)I2C_CR2_AUTOEND);
  
  while(Size > 0)
  {
    if(I2C_WaitRXNEFlag(Timeout)) 
    {
      return 1;
    }

    *(pData++) = I2C1->RXDR;
    Size--;
  }

  if(I2C_WaitStopFlag(Timeout))
  {
    return 1;
  }

  SET_BIT(I2C1->ICR, I2C_ICR_STOPCF);
  CLEAR_BIT(I2C1->CR2, I2C_CR2_SADD | I2C_CR2_HEAD10R | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_RD_WRN);

  return 0;
}

uint32_t I2C_Transmit(uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
  if(I2C_WaitFlag(I2C_ISR_BUSY, SET, 50))
  {
    return 1;
  }
  const uint32_t Request = (uint32_t)(0x80000000U | I2C_CR2_START);
  MODIFY_REG(I2C1->CR2, (I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_STOP |
    (I2C_CR2_RD_WRN & (uint32_t)(Request >> (31U - I2C_CR2_RD_WRN_Pos)))),
    ((uint32_t)DevAddress & I2C_CR2_SADD) | (((uint32_t)Size << I2C_CR2_NBYTES_Pos) & I2C_CR2_NBYTES) | 
    Request | (uint32_t)I2C_CR2_AUTOEND);
  
  while(Size > 0)
  {
    if(I2C_WaitTXISFlag(Timeout)) 
    {
      return 1;
    }

    I2C1->TXDR = *(pData++);
    Size--;
  }

  if(I2C_WaitStopFlag(Timeout))
  {
    return 1;
  }

  SET_BIT(I2C1->ICR, I2C_ICR_STOPCF);
  CLEAR_BIT(I2C1->CR2, I2C_CR2_SADD | I2C_CR2_HEAD10R | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_RD_WRN);

  return 0;
}
