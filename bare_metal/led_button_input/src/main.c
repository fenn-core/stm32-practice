#include <stdint.h>
#include "stm32g4xx.h"

#define CORE_CLOCK_HZ  16000000UL
#define SYSTICK_HZ     1000U
#define SYSTICK_LOAD   (CORE_CLOCK_HZ / SYSTICK_HZ - 1U)


volatile uint32_t time_accumulator_ms = 0;

typedef enum {
    LED_OFF,
    LED_ON,
} led_state_t;

typedef enum {
    LED_PERMA_ON,
    LED_BLINKING_SLOW,
    LED_BLINKING_FAST,
    LED_PERMA_OFF,
} led_mode_t;


void SysTick_Handler(void) {
    time_accumulator_ms++;
}


static void systick_init(void) {
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    SysTick->LOAD = SYSTICK_LOAD;
    SysTick->VAL = 0U;

    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk |
                    SysTick_CTRL_ENABLE_Msk;
}


static uint32_t systick_get_ms(void) {
    return time_accumulator_ms;
}


static void gpio_init(void) {
    // config for PA5

    RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN_Msk | RCC_AHB2ENR_GPIOCEN_Msk);

    GPIOA->MODER &= ~GPIO_MODER_MODE5_Msk;
    GPIOA->MODER |= GPIO_MODER_MODER5_0;          // output
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT5;            // push pull
    GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED5_Msk;  // low speed
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD5_Msk;        // no pull-up, pull-down


    // config for PC13

    GPIOC->MODER &= ~GPIO_MODER_MODE13_Msk; // input
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD13_Msk; // no pull-up, pull-down
}


static void button_input_sentinel(led_mode_t *led_mode) {
    static uint8_t previous_pressed = 0;

    uint8_t pressed =
        ((GPIOC->IDR & GPIO_IDR_ID13_Msk) != 0);

    if (pressed && !previous_pressed) {
        switch (*led_mode) {
            case LED_PERMA_ON:
                *led_mode = LED_BLINKING_SLOW;
                break;

            case LED_BLINKING_SLOW:
                *led_mode = LED_BLINKING_FAST;
                break;

            case LED_BLINKING_FAST:
                *led_mode = LED_PERMA_OFF;
                break;

            case LED_PERMA_OFF:
                *led_mode = LED_PERMA_ON;
                break;
        }
    }

    previous_pressed = pressed;
}


static void led_on(led_state_t *led_state) {
    GPIOA->ODR |= GPIO_ODR_OD5;
    *led_state = LED_ON;
}


static void led_off(led_state_t *led_state) {
    GPIOA->ODR &= ~(GPIO_ODR_OD5);
    *led_state = LED_OFF;
}


static void led_state_switch(led_state_t *led_state) {
    switch (*led_state) {
        case LED_OFF: led_on(led_state); break;
        case LED_ON: led_off(led_state); break;
    }
}


static void led_blink(led_state_t *led_state, const uint32_t delay,
                      const uint32_t current_time_ms, uint32_t *last_toggled_ms) {
    if (current_time_ms - *last_toggled_ms >= delay) {
        led_state_switch(led_state);
        *last_toggled_ms = current_time_ms;
    }
}


int main(void) {
    uint32_t last_toggled_ms = 0;
    uint32_t slow_delay = 500;
    uint32_t fast_delay = 100;

    gpio_init();
    systick_init();

    led_mode_t led_mode = LED_PERMA_ON;
    led_state_t led_state = LED_ON;
    led_on(&led_state);

    while (1) {
        uint32_t current_time_ms = systick_get_ms();
        button_input_sentinel(&led_mode);

        switch (led_mode) {
            case LED_PERMA_ON:
                led_on(&led_state);
                break;

            case LED_BLINKING_SLOW:
                led_blink(&led_state, slow_delay, current_time_ms, &last_toggled_ms);
                break;

            case LED_BLINKING_FAST:
                led_blink(&led_state, fast_delay, current_time_ms, &last_toggled_ms);
                break;

            case LED_PERMA_OFF:
                led_off(&led_state);
                break;
        }
    }
}
