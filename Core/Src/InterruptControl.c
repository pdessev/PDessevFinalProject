/*
 * InterruptControl.c
 *
 *  Created on: Oct 1, 2024
 *      Author: peter
 */

// #include <stdlib.h>

#define __INTERRUPT_CONTROL_INTERNAL__
#include "InterruptControl.h"
#include "stm32f4xx_hal.h"
#include "Debug/mem.h"

void enable_IRQ(Irq irq) {
    HAL_NVIC_EnableIRQ((IRQn_Type)irq);
}

void disable_IRQ(Irq irq) {
    HAL_NVIC_DisableIRQ((IRQn_Type)irq);
}

void clear_pending_IRQ(Irq irq) {
    HAL_NVIC_ClearPendingIRQ((IRQn_Type)irq);
}

void set_pending_IRQ(Irq irq) {
    HAL_NVIC_SetPendingIRQ((IRQn_Type)irq);
}

void clear_pending_exti_interrupt(ExtiLines line) {
    EXTI_HandleTypeDef* hexti = calloc(1, sizeof(*hexti));
    hexti->Line = line;

    HAL_EXTI_ClearPending(hexti, EXTI_TRIGGER_RISING_FALLING);

    free(hexti);
}