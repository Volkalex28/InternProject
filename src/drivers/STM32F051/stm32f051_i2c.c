/**
 * @file stm32f051_i2c.c
 * @author Oleksandr Ananiev (alexander.ananiev@sigma.sofware)
 * @brief Source file of I2C module driver
 * @version 0.1
 * @date 2021-01-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "stm32f051_i2c.h"
#include "stm32f051_rcc.h"
#include <user_assert.h>

// Private Function -----------------------------------------------------------
/** @defgroup I2C_Private_Function Private Function
 * @ingroup I2C
 * 
 * Functions required for the needs of the I2C module driver
 * @{
 */

/**
 * @brief This function handles I2C1 Communication Timeout
 * 
 * @param[in] Flag Specifies the I2C1 flag to check
 * @param[in] Status The new Flag status (SET or RESET)
 * @param[in] tickStart Tick start value
 * @param[in] Timeout Timeout duration
 * 
 * @return Status
 * @retval 0 If no errors occurred
 * @retval 1 If an error occurs during the process
 */
const uint32_t I2C1_WaitFlag(const uint32_t Flag, const FlagStatus Status,
  const uint32_t tickStart, const uint32_t Timeout)
{
  while((READ_BIT(I2C1->ISR, Flag) ? SET : RESET) == Status) 
  {
    if(GetTick() >= (tickStart + Timeout))
    {
      return 1;
    }
  }
  return 0;
}

/**
 * @brief This function handles Acknowledge failed detection during an I2C1 Communication
 * 
 * @param[in] tickStart Tick start value
 * @param[in] Timeout Timeout duration
 * 
 * @return Status
 * @retval 0 If no errors occurred
 * @retval 1 If an error occurs during the process
 */
const uint32_t I2C1_IsAcknowledgeFailed(const uint32_t tickStart, const uint32_t Timeout)
{
  if((READ_BIT(I2C1->ISR, I2C_ISR_NACKF) ? SET : RESET) == SET)
  {
    while((READ_BIT(I2C1->ISR, I2C_ISR_STOPF) ? SET : RESET) == RESET)
    {
      if(GetTick() >= (tickStart + Timeout))
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

/**
 * @brief This function handles I2C1 Communication Timeout for specific usage of RXNE flag
 * 
 * @param[in] tickStart Tick start value
 * @param[in] Timeout Timeout duration
 * 
 * @return Status
 * @retval 0 If no errors occurred
 * @retval 1 If an error occurs during the process
 */
const uint32_t I2C1_WaitRXNEFlag(const uint32_t tickStart, const uint32_t Timeout)
{
  while((READ_BIT(I2C1->ISR, I2C_ISR_RXNE) ? SET : RESET) == RESET)
  {
    if(I2C1_IsAcknowledgeFailed(tickStart, Timeout))
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

    if(GetTick() >= (tickStart + Timeout))
    {
      return 1;
    }
  }
  return 0;
}

/**
 * @brief This function handles I2C1 Communication Timeout for specific usage of TXIS flag
 * 
 * @param[in] tickStart Tick start value
 * @param[in] Timeout Timeout duration
 * 
 * @return Status
 * @retval 0 If no errors occurred
 * @retval 1 If an error occurs during the process
 */
const uint32_t I2C1_WaitTXISFlag(const uint32_t tickStart, const uint32_t Timeout)
{
  while((READ_BIT(I2C1->ISR, I2C_ISR_TXIS) ? SET : RESET) == RESET)
  {
    if(I2C1_IsAcknowledgeFailed(tickStart, Timeout))
    {
      return 1;
    }

    if(GetTick() >= (tickStart + Timeout))
    {
      return 1;
    }
  }
  return 0;
}

/**
 * @brief This function handles I2C1 Communication Timeout for specific usage of STOP flag
 * 
 * @param[in] tickStart Tick start value
 * @param[in] Timeout Timeout duration
 * 
 * @return Status
 * @retval 0 If no errors occurred
 * @retval 1 If an error occurs during the process
 */
const uint32_t I2C1_WaitSTOPFlag(const uint32_t tickStart, const uint32_t Timeout)
{
  while((READ_BIT(I2C1->ISR, I2C_ISR_STOPF) ? SET : RESET) == RESET) 
  {
    if(GetTick() >= (tickStart + Timeout))
    {
      return 1;
    }

    if(I2C1_IsAcknowledgeFailed(tickStart, Timeout))
    {
      return 1;
    }
    
  }
  return 0;
}
///@}

// Exported Function ----------------------------------------------------------
const uint32_t I2C1_Receive(const uint16_t DevAddress, uint8_t * pData, uint16_t Size,
  const uint32_t Timeout)
{
  ASSERT(pData);
  ASSERT(Size > 0);

  const uint32_t tickStart = GetTick();
  if(I2C1_WaitFlag(I2C_ISR_BUSY, SET, tickStart, 25))
  {
    return 1;
  }
  const uint32_t Request = (uint32_t)(0x80000000U | I2C_CR2_START | I2C_CR2_RD_WRN);
  MODIFY_REG(I2C1->CR2, (I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_STOP |
    (I2C_CR2_RD_WRN & (uint32_t)(Request >> (31U - I2C_CR2_RD_WRN_Pos)))),
    ((uint32_t)DevAddress & I2C_CR2_SADD) | (((uint32_t)Size << I2C_CR2_NBYTES_Pos) & I2C_CR2_NBYTES) | 
    Request | (uint32_t)I2C_CR2_AUTOEND);
  
  while((Size--) > 0)
  {
    if(I2C1_WaitRXNEFlag(tickStart, Timeout)) 
    {
      return 1;
    }

    *(pData++) = I2C1->RXDR;
  }

  if(I2C1_WaitSTOPFlag(tickStart, Timeout))
  {
    return 1;
  }

  SET_BIT(I2C1->ICR, I2C_ICR_STOPCF);
  CLEAR_BIT(I2C1->CR2, I2C_CR2_SADD | I2C_CR2_HEAD10R | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_RD_WRN);

  return 0;
}

const uint32_t I2C1_Transmit(const uint16_t DevAddress, uint8_t *pData, uint16_t Size,
  const uint32_t Timeout)
{
  ASSERT(pData);
  ASSERT(Size > 0);

  const uint32_t tickStart = GetTick();
  if(I2C1_WaitFlag(I2C_ISR_BUSY, SET, tickStart, 25))
  {
    return 1;
  }
  const uint32_t Request = (uint32_t)(0x80000000U | I2C_CR2_START);
  MODIFY_REG(I2C1->CR2, (I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_STOP |
    (I2C_CR2_RD_WRN & (uint32_t)(Request >> (31U - I2C_CR2_RD_WRN_Pos)))),
    ((uint32_t)DevAddress & I2C_CR2_SADD) | (((uint32_t)Size << I2C_CR2_NBYTES_Pos) & I2C_CR2_NBYTES) | 
    Request | (uint32_t)I2C_CR2_AUTOEND);
  
  while((Size--) > 0)
  {
    if(I2C1_WaitTXISFlag(tickStart, Timeout)) 
    {
      return 1;
    }
    I2C1->TXDR = *(pData++);
  }

  if(I2C1_WaitSTOPFlag(tickStart, Timeout))
  {
    return 1;
  }

  SET_BIT(I2C1->ICR, I2C_ICR_STOPCF);
  CLEAR_BIT(I2C1->CR2, I2C_CR2_SADD | I2C_CR2_HEAD10R | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_RD_WRN);

  return 0;
}

const uint32_t I2C1_Init(void) 
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
