/*
 * ApplicationCode.h
 *
 *  Created on: Sep 24, 2024
 *      Author: peter
 */

#pragma once
#include "LED_Driver.h"
#include "Scheduler.h"
#include "Button_Driver.h"
#include "Gyro.h"

#ifdef __APPLICATION_CODE_INTERNAL__
#include <stdlib.h>
#include <stdio.h>

typedef struct AppState {
    Gyro* g;
} AppState;

#else
typedef void AppState;
#endif

#define NAME_LEN 5
#define DEFAULT_DELAY_TICK 50000
#define STATIC_LED_BLINK_RATE_SEC 5
#define DYNAMIC_LED_BLINK_MAX_ENABLE_TIME 500

#define DUAL_TIMER 1

AppState* init_app();

void LCD_Visual_Demo(void);

#if (COMPILE_TOUCH_FUNCTIONS == 1) && (COMPILE_TOUCH_INTERRUPT_SUPPORT == 0)
void LCD_Touch_Polling_Demo(void);
#endif // (COMPILE_TOUCH_FUNCTIONS == 1) && (COMPILE_TOUCH_INTERRUPT_SUPPORT == 0)


void print_gyro(AppState* state);

void init_green_led();

void init_red_led();

void toggle_green_led();

void toggle_red_led();

void on_green_led();

void on_red_led();

void off_green_led();

void off_red_led();

void delay(uint32_t count);

// #if USE_INTERRUPT_FOR_BUTTON == 0
// void init_user_button();

// void user_button_polling_routine();
// #else
void init_user_button_with_interrupt();
// #endif
