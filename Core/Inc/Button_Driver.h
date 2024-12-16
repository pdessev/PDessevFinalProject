/**
 * @file            Button_Driver.h
 * @brief           Header for `Button_Driver.c` \n
 *                    This file contains functions and structure 
 *                    definitions for the button interface.
 * \n Created on: Sep 24, 2024
 * \n Author: peter
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

/**
 * The available buttons to be used by the user
 * # Buttons
 * - `UserButton`: The button which is not the reset button.
 * 
 */
typedef enum Buttons {
    UserButton,
} Buttons;

#define BUTTON_UN_PRESSED Unpressed
#define BUTTON_PRESSED Pressed

/** 
 * Initializes the selected button for use with polling.
 * # Inputs
 * - `Button button`: Button to be initialized 
 * # Returns
 * Void
 */
void init_button(Buttons button);

/** 
 * Initializes the selected button for use with interrupts when pressed.
 * # Inputs
 * - `Button button`: Button to be initialized 
 * # Returns
 * Void
 * # Notes
 * The requisite `IRQ` must be created seperatly.
 */
void init_interrupt_button(Buttons button);

/** 
 * Returns the state of the current button.
 * # Inputs
 * - `Button button`: Button to get state 
 * # Returns
 * - `0` if button is not pressed
 * - `1` if button is pressed
 */
uint8_t button_is_pressed(Buttons button);