#include "stm32g4xx.h"


void spi1_init(void) {
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    GPIOA->MODER &= ~(GPIO_MODER_MODE5);
    GPIOA->MODER |= GPIO_MODER_MODE5_1;

    GPIOA->MODER &= ~(GPIO_MODER_MODE7);
    GPIOA->MODER |= GPIO_MODER_MODE7_1;


    GPIOA->OTYPER &= ~(GPIO_OTYPER_IDR_5);
    GPIOA->OTYPER &= ~(GPIO_OTYPER_IDR_7);


    GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED5);
    GPIOA->OSPEEDR |= (2UL << GPIO_OSPEEDR_OSPEED5_Pos);

    GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED7);
    GPIOA->OSPEEDR |= (2UL << GPIO_OSPEEDR_OSPEED7_Pos);


    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD5);
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD7);


    GPIOA->AFR[0] &= ~(GPIO_AFRL_AFRL5);
    GPIOA->AFR[0] |= (5UL << GPIO_AFRL_AFSEL5_Pos);

    GPIOA->AFR[0] &= ~(GPIO_AFRL_AFRL7);
    GPIOA->AFR[0] |= (5UL << GPIO_AFRL_AFSEL7_Pos);


    SPI1->CR1 = 0UL;

    SPI1->CR1 |= SPI_CR1_MSTR;

    SPI1->CR1 |= (SPI_CR1_SSM | SPI_CR1_SSI);

    SPI1->CR1 &= ~(SPI_CR1_CPOL);
    SPI1->CR1 &= ~(SPI_CR1_CPHA);

    SPI1->CR1 &= ~(SPI_CR1_LSBFIRST);

    SPI1->CR1 &= ~(SPI_CR1_BR);
    SPI1->CR1 |= (6UL << SPI_CR1_BR_Pos);


    SPI1->CR2 = 0X0700;  // reset

    SPI1->CR2 &= ~(SPI_CR2_DS);
    SPI1->CR2 |= (7UL << SPI_CR2_DS_Pos);

    SPI1->CR2 |= SPI_CR2_FRXTH;


    SPI1->CR1 |= SPI_CR1_SPE;

}


void spi1_transmit(uint8_t *buffer, uint32_t n) {
    for (uint32_t i = 0; i < n; ++i) {
        while (!(SPI1->SR & SPI_SR_TXE)) {
            __asm volatile ("NOP");
        }
        SPI1->SR &= ~(SPI_SR_RXNE);  // drain RXNE

        *((volatile uint8_t *)&SPI1->DR) = buffer[i];

        while (SPI1->SR & SPI_SR_BSY) {
            __asm volatile ("NOP");
        }

    }
}


int main(void) {
    spi1_init();
    uint8_t buffer[] = {0xAA, 0x55, 0xF0, 0x0F};

    while (1) {
        spi1_transmit(buffer, 4);

        for (uint32_t i = 0; i < 1700; ++i) {
            __asm volatile ("NOP");
        }

    }

}