/*
 * ApplicationCode.c
 *
 *  Created on: Sep 24, 2024
 *      Author: peter
 */

#define __APPLICATION_CODE_INTERNAL__
#include "ApplicationCode.h"

AppState* init_app() {
    AppState* state = calloc(1, sizeof(*state));

    state->g = gyro_init();
    HAL_Delay(1000);
    gyro_power(state->g);

    init_user_button_with_interrupt();
    
    addSchedulerEvent(EventDelay);
    

    gyro_power(state->g);
    HAL_Delay(100);

    gyro_config_regs(state->g);

    return state;
}

void print_gyro(AppState* state) {
    uint8_t id = gyro_get_id(state->g);
    int8_t temp = gyro_get_temp(state->g);

    printf("Gyro Id: %X\nTemperature: %d\n", id, temp);
}

void init_green_led() { init_led(LedGreen); }

void init_red_led() { init_led(LedRed); }

void toggle_green_led() { toggle_led(LedGreen); }

void toggle_red_led() { toggle_led(LedRed); }

void on_green_led() { on_led(LedGreen); }

void on_red_led() { on_led(LedRed); }

void off_green_led() { off_led(LedGreen); }

void off_red_led() { off_led(LedRed); }

void delay(uint32_t count) {
    char name[] = "Peter";
    char* copy = calloc(NAME_LEN, sizeof(*copy));
    for (uint32_t _ = count; _; _--) {
        for (uint8_t i = NAME_LEN; i; i--) {
            *(copy + i - 1) = *(name + i - 1);
        }
    }
    free(copy);
}

void init_user_button_with_interrupt() { init_interrupt_button(UserButton); }

void EXTI0_IRQHandler() {
    disable_IRQ(EXTI0_IRQn);
    __HAL_GPIO_EXTI_CLEAR_IT(EXTI_LINE_1);
    
    addSchedulerEvent(EventIdAndTemp);

    clear_pending_IRQ(EXTI0_IRQn);
    enable_IRQ(EXTI0_IRQn);
}
