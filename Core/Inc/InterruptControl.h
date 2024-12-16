/**
 * @file            InterruptControl.h
 * @brief           Header for `InterruptControl.c` \n
 *                    This file contains functions and structure 
 *                    definitions for controlling NVIC and EXTI interrupts.
 * \n Created on: Oct 1, 2024
 * \n Author: peter
 * 
 */

#pragma once
#include "stm32f429xx.h"
#include "stm32f4xx_hal.h"

/**
 * Defines the pin numbers for EXTI interrupt lines.
 * 
 */
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

/**
 * Redefines the existing IRQ datatype for nicer typenames
 * 
 */
typedef IRQn_Type Irq;

/** 
 * Enables the IRQ line `irq`.
 * # Inputs
 * - `IRQ irq`: The IRQ line to be enabled.
 * # Returns
 * - `void'
 */
void enable_IRQ(Irq irq);

/** 
 * Disables the IRQ line `irq`.
 * # Inputs
 * - `IRQ irq`: The IRQ line to be disabled.
 * # Returns
 * - `void'
 */
void disable_IRQ(Irq irq);

/** 
 * Clears the IRQ `irq`'s pending interrupt flag.
 * # Inputs
 * - `IRQ irq`: The IRQ line to be cleared.
 * # Returns
 * - `void'
 */
void clear_pending_IRQ(Irq irq);

/** 
 * Sets the IRQ `irq`'s pending interrupt flag.
 * # Inputs
 * - `IRQ irq`: The IRQ line to be set.
 * # Returns
 * - `void'
 */
void set_pending_IRQ(Irq irq);

/** 
 * Clears the EXIT interrupt `line`'s pending interrupt flag.
 * # Inputs
 * - `ExtiLines line`: The EXTI line to be cleared.
 * # Returns
 * - `void'
 */
void clear_pending_exti_interrupt(ExtiLines line);