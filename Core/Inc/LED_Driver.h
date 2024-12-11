/*
 * LED_Driver.h
 *
 *  Created on: Sep 24, 2024
 *      Author: peter
 */

#pragma once
#include "GPIO_Wrapper.h"
#include "Timer_Wrapper.h"

#ifdef __LED_DRIVER_INTERNAL__
#include "Debug/mem.h"
// #include <stdlib.h>
#endif 

#define RED_LED_PIN GpioPin14
#define GREEN_LED_PIN GpioPin13

typedef enum Leds {
	LedRed,
	LedGreen,
} Leds;

void init_led(Leds led);

void toggle_led(Leds led);

void off_led(Leds led);

void on_led(Leds led);
