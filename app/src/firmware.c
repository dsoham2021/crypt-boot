// Blitzkraft gmbh

#include "bk_common.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/vector.h>
#include <libopencm3/cm3/systick.h>

#define LED_PORT        (GPIOA)
#define LED_PIN         (GPIO5)

volatile uint64_t ticks = 0;
#define SYSTICK_FREQ        (1000)
#define CPU_FREQ            (72000000U)


void sys_tick_handler(void) {
  ticks++;
}

static uint64_t get_ticks(void) {
    return ticks;
}

static void systick_setup(void) {

  systick_set_frequency(SYSTICK_FREQ, CPU_FREQ);
  systick_counter_enable();
  systick_interrupt_enable();

}


static void bk_gpio_setup(uint32_t port, uint16_t pins) {

    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_set_mode(port, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL ,pins);
    
}

static void bk_rcc_setup(void) {
    
    // HSE not working ** update now working
    // RCC_CLOCK_HSI_24MHZ working   44sec
    // RCC_CLOCK_HSI_48MHZ working   28sec
    // RCC_CLOCK_HSI_64MHZ working   25sec
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);
}

//static void bk_bad_delay(uint32_t cycles) {
//    
//    for(uint32_t i = 0; i < cycles; i++) {
//        __asm__("nop");
//    }
//}


int main(void) {

    bk_rcc_setup();
    bk_gpio_setup(LED_PORT, LED_PIN);
    systick_setup();

    uint64_t start_time = get_ticks();

    while(1) {
        
        //gpio_toggle(LED_PORT, LED_PIN);
        //bk_bad_delay(0xFFFFFF);

        if ( (get_ticks() - start_time) >= 200) {
            gpio_toggle(LED_PORT, LED_PIN);
            start_time = get_ticks();
        }
        
    }
}