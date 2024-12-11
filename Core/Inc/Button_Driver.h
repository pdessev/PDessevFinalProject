/*
 * Button_Driver.h
 *
 *  Created on: Sep 24, 2024
 *      Author: peter
 */

#pragma once
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_conf.h"
#include "GPIO_Wrapper.h"
#include "InterruptControl.h"

#ifdef __BUTTON_DRIVER_INTERNAL__
#include <stdint.h>
// #include <stdlib.h>
#include "Debug/mem.h"
#endif

// PA0
#define USER_BUTTON_PORT GpioPortA
#define USER_BUTTON_PIN GpioPin0

typedef enum ButtonState {
    Unpressed = 0,
    Pressed = 1,
} ButtonState;

typedef enum Buttons {
    UserButton,
} Buttons;

#define BUTTON_UN_PRESSED Unpressed
#define BUTTON_PRESSED Pressed

void init_button(Buttons button);

void init_interrupt_button(Buttons button);

uint8_t button_is_pressed(Buttons button);