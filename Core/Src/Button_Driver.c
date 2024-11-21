/*
 * Button_Driver.c
 *
 *  Created on: Sep 24, 2024
 *      Author: peter
 */

#define __BUTTON_DRIVER_INTERNAL__
#include "Button_Driver.h"

/// @brief Stores the pin number of the given button in dest
/// @param button
/// @param dest
/// @return 1 if button is not recognized, 0 on succesfull store
uint8_t get_button_pin(Buttons button, pinNumbers* dest) {
    switch (button) {
    case UserButton:
        *dest = USER_BUTTON_PIN;
        break;
    default:
        return 1;
    };
    return 0;
}

/// @brief Stores the port of the given button in dest
/// @param button
/// @param dest
/// @return 1 if button is not recognized, 0 on succesfull store
uint8_t get_button_port(Buttons button, GpioPorts* dest) {
    switch (button) {
    case UserButton:
        *dest = USER_BUTTON_PORT;
        break;
    default:
        return 1;
    };
    return 0;
}

void init_button(Buttons button) {
    // Get the GPIO port for the button.
    // Do it now to avoid callocing excesively
    GpioPorts port = GpioPortA;
    if (get_button_port(button, &port)) {
        return;
    }

    // Configure pin
    GpioPinConfig* pin_conf = (GpioPinConfig*)calloc(1, sizeof(*pin_conf));
    pin_conf->OutputType = GpioPushPull;
    pin_conf->PinMode = GpioPinModeInput;
    pin_conf->PinSpeed = GpioSpeedLow;
    pin_conf->PinPuPdControl = GpioFloat;
    pin_conf->IntrMode = InterruptModeNone;
    if (get_button_pin(button, &(pin_conf->PinNumber))) {
        free(pin_conf);
        return;
    }

    // Initialize the pin
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_init_gpio(port, pin_conf);
    free(pin_conf);
}

void init_interrupt_button(Buttons button) {
    // Get the GPIO port for the button.
    // Do it now to avoid callocing excesively
    GpioPorts port = GpioPortA;
    if (get_button_port(button, &port)) {
        return;
    }

    // Configure pin
    GpioPinConfig* pin_conf = (GpioPinConfig*)calloc(1, sizeof(*pin_conf));
    pin_conf->OutputType = GpioPushPull;
    pin_conf->PinMode = GpioPinModeInput;
    pin_conf->PinSpeed = GpioSpeedLow;
    pin_conf->PinPuPdControl = GpioFloat;
    pin_conf->IntrMode = InterruptModeRising;
    if (get_button_pin(button, &(pin_conf->PinNumber))) {
        free(pin_conf);
        return;
    }

    // Initialize the pin
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_init_gpio(port, pin_conf);
    enable_IRQ(EXTI0_IRQn);
    free(pin_conf);
}

// void enable_button_clock(Buttons button);

uint8_t button_is_pressed(Buttons button) {
    GpioPorts port = GpioPortA;
    pinNumbers pin = GpioPin0;
    if (get_button_port(button, &port)) {
        return 0;
    }

    if (get_button_pin(button, &pin)) {
        return 0;
    }

    return GPIO_read_gpio_pin(port, pin);
}
