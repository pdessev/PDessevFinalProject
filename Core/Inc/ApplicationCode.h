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

Result app_loop(AppState** state);

void print_gyro(AppState* state);

void delay(uint32_t count);