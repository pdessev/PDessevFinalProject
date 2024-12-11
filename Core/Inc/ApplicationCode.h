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
#include "stm32f4xx_hal.h"
#include "LCD/LCD_Driver.h"
#include "ErrorHandling.h"
#include "Debug/mem.h"

#ifdef __APPLICATION_CODE_INTERNAL__
// #include <stdlib.h>
#include <stdio.h>

typedef struct AppState {
    Gyro* g;
} AppState;

#else
typedef void AppState;
#endif

#define NAME_LEN 5

AppState* init_app();

void LCD_Visual_Demo(void);

Result app_loop(AppState** state);

#if (COMPILE_TOUCH_FUNCTIONS == 1) && (COMPILE_TOUCH_INTERRUPT_SUPPORT == 0)
void LCD_Touch_Polling_Demo(void);
#endif // (COMPILE_TOUCH_FUNCTIONS == 1) && (COMPILE_TOUCH_INTERRUPT_SUPPORT == 0)

void print_gyro(AppState* state);

void delay(uint32_t count);