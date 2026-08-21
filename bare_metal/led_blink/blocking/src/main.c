#include "stm32g474xx.h"


static void delay(void) {
    for (volatile uint32_t i = 0; i < 500000; ++i) {
        __asm volatile("NOP");
    }
}


int main(void) {
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    (void) RCC->AHB2ENR; // dummy readback

    GPIOA->MODER &= ~((uint32_t) (3U) << 10); // 3U as in 0b11
    GPIOA->MODER |= ((uint32_t) (1U) << 10); // set output mode for PA5

    GPIOA->OTYPER &= ~((uint32_t) (1U) << 5);

    GPIOA->OSPEEDR &= ~((uint32_t) (3U) << 10);

    GPIOA->PUPDR &= ~((uint32_t) (3U) << 10);

    while (1) {
        GPIOA->BSRR = GPIO_BSRR_BS5;

        delay();

        GPIOA->BSRR = GPIO_BSRR_BR5;

        delay();

    }

}
