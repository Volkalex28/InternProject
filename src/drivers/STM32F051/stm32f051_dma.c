
#include "stm32f051_dma.h"

/**
 * @brief Initialization DMA controller
 * @retval DMA controller status
 */
uint32_t DMA_Init(void)
{
  // Enable clock for DMA controller
  SET_BIT(RCC->AHBENR, RCC_AHBENR_DMA1EN);
  /** DMA interrupt init
    * DMA1_Channel2_3_IRQn interrupt configuration
    */
  NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0);
  NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);

  return 0;
}
