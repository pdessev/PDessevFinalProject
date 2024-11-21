/*
 * InterruptControl.h
 *
 *  Created on: Oct 1, 2024
 *      Author: peter
 */

#pragma once
#include "stm32f429xx.h"
#include "stm32f4xx_hal.h"

typedef enum ExtiLines {
    ExtiPin0 = EXTI_LINE_0,
    ExtiPin1 = EXTI_LINE_1,
    ExtiPin2 = EXTI_LINE_2,
    ExtiPin3 = EXTI_LINE_3,
    ExtiPin4 = EXTI_LINE_4,
    ExtiPin5 = EXTI_LINE_5,
    ExtiPin6 = EXTI_LINE_6,
    ExtiPin7 = EXTI_LINE_7,
    ExtiPin8 = EXTI_LINE_8,
    ExtiPin9 = EXTI_LINE_9,
    ExtiPin10 = EXTI_LINE_10,
    ExtiPin11 = EXTI_LINE_11,
    ExtiPin12 = EXTI_LINE_12,
    ExtiPin13 = EXTI_LINE_13,
    ExtiPin14 = EXTI_LINE_14,
    ExtiPin15 = EXTI_LINE_15,
    ExtiPvd = EXTI_LINE_16,
    ExtiRtc = EXTI_LINE_17,
    ExtiUsbOtgFsWake = EXTI_LINE_18,
    ExtiEthWake = EXTI_LINE_19,
    ExtiUsbOtgHsWake = EXTI_LINE_20,
    ExtiRtcTmprTs = EXTI_LINE_21,
    ExtiRtcWake = EXTI_LINE_22,

#ifdef EXTI_IMR_IM23
    ExtiPin23 = EXTI_LINE_23,
#endif
} ExtiLines;

typedef IRQn_Type Irq;

void enable_IRQ(Irq irq);

void disable_IRQ(Irq irq);

void clear_pending_IRQ(Irq irq);

void set_pending_IRQ(Irq irq);

void clear_pending_exti_interrupt(ExtiLines line);