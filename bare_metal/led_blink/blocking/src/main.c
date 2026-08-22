#include <stdint.h>
#include "stm32g474xx.h"


static void delay(void) {
    for (volatile uint32_t i = 0; i < 500000; ++i) {
        __asm volatile("NOP");
    }
}


int main(void) {
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    (void) RCC->AHB2ENR; // dummy readback

    GPIOA->MODER &= ~GPIO_MODER_MODE5_Msk; // 3U as in 0b11
    GPIOA->MODER |= GPIO_MODER_MODE5_0; // set output mode for PA5

    GPIOA->OTYPER &= ~GPIO_OTYPER_OT5;

    GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED5_Msk;

    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD5_Msk;

    while (1) {
        GPIOA->BSRR = GPIO_BSRR_BS5;

        delay();

        GPIOA->BSRR = GPIO_BSRR_BR5;

        delay();

    }

}
