#include "stm32g4xx.h"

#define CORE_CLOCK_HZ  16000000UL
#define SYSTICK_HZ     1000U
#define SYSTICK_LOAD   (CORE_CLOCK_HZ / SYSTICK_HZ - 1U)


static volatile uint32_t time_accumulator_ms = 0;

void SysTick_Handler(void) {
    time_accumulator_ms++;
}


static void systick_init(void) {
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    SysTick->LOAD = SYSTICK_LOAD;
    SysTick->VAL = 0U;

    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk   |
                    SysTick_CTRL_ENABLE_Msk;
}


static uint32_t systick_get_ms(void) {
    return time_accumulator_ms;
}


static void pa5_led_init(void) {
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    (void) RCC->AHB2ENR;

    GPIOA->MODER &= ~GPIO_MODER_MODE5_Msk;
    GPIOA->MODER |= GPIO_MODER_MODE5_0;

    GPIOA->OTYPER &= ~GPIO_OTYPER_OT5;

    GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED5_Msk;

    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD5_Msk;
}


int main(void) {
    uint32_t interval_ms = 500;
    uint32_t last_toggled_ms = 0;

    pa5_led_init();
    systick_init();

    while (1) {
        uint32_t current_ms = systick_get_ms();
        if (current_ms - last_toggled_ms >= interval_ms) {
            GPIOA->ODR ^= GPIO_ODR_OD5;
            last_toggled_ms += interval_ms;
        }
    }
}
