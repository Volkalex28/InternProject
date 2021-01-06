#include "main.h"

#define DELAY   100000

int main() {

    RCC->CR |= RCC_CR_HSION;

    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    // --- GPIO setup ----
    GPIOC->MODER |= GPIO_MODER_MODER9_0;
    //GPIOC->OTYPER &= ~(uint32_t)GPIO_OTYPER_OT_9
    //GPIOC->OSPEEDR &= ~(uint32_t)GPIO_OSPEEDR_

    while(1) {
        GPIOC->BRR = GPIO_PIN_9;
        for(int i = 0; i < DELAY; i++) {};
        GPIOC->BSRR = GPIO_PIN_9;
        for(int i = 0; i < DELAY; i++) {};
    }
}