#include "main.h"

#define DELAY   500

volatile uint32_t ticks = 0;

inline void delay(volatile uint32_t timeout) {
    uint32_t ticks_start = ticks;
    while(ticks < ticks_start + timeout);
}

int main() {

    // --- RCC ---
    // HSI
    SET_BIT(RCC->CR, RCC_CR_HSION);
    while(READ_BIT(RCC->CR, RCC_CR_HSIRDY) == SET);

    MODIFY_REG(RCC->CR, RCC_CR_HSITRIM, (uint32_t)(0x10) << 3);

    // PLL
    MODIFY_REG(RCC->CFGR2, RCC_CFGR2_PREDIV, 0);
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PLLMUL | RCC_CFGR_PLLSRC, (uint32_t)(RCC_CFGR_PLLMUL12 | RCC_CFGR_PLLSRC_HSI_DIV2));
    SET_BIT(RCC->CR, RCC_CR_PLLON);
    while(READ_BIT(RCC->CR, RCC_CR_PLLRDY) == SET);

    // HCLK
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE, RCC_CFGR_PPRE_DIV16);
    MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV1);

    // SYSCLK
    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL);
    while((uint32_t)(RCC->CFGR & RCC_CFGR_SWS) == (RCC_CFGR_SW_PLL << RCC_CFGR_SWS_Pos));

    // PCLK1
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE, RCC_CFGR_PPRE_DIV1);

    // --- SYSTICK ----
    SysTick->LOAD  = (uint32_t)(48000 - 1UL);
    NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);
    SysTick->VAL   = 0UL;
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;

    NVIC_SetPriority(SysTick_IRQn, 0);

    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN);
    SET_BIT(RCC->APB1ENR, RCC_APB1ENR_PWREN);

    // GPIO
    SET_BIT(RCC->AHBENR, RCC_AHBENR_GPIOCEN);
    // --- GPIO setup ----
    SET_BIT(GPIOC->MODER, GPIO_MODER_MODER9_0);
    //GPIOC->OTYPER &= ~(uint32_t)GPIO_OTYPER_OT_9
    //GPIOC->OSPEEDR &= ~(uint32_t)GPIO_OSPEEDR_

    while(1) {
        SET_BIT(GPIOC->BSRR, GPIO_BSRR_BS_9);
        delay(DELAY);
        SET_BIT(GPIOC->BSRR, GPIO_BSRR_BR_9);
        delay(DELAY);
    }
}