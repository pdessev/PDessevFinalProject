/*
 * ApplicationCode.h
 *
 *  Created on: Sep 24, 2024
 *      Author: peter
 */

#pragma once
#include "ErrorHandling.h"

#define BLOCK_FALL_RATE_SEC 3.0f

#ifdef __APPLICATION_CODE_INTERNAL__
// #include <stdlib.h>
#include <stdio.h>
#include "Gyro.h"
#include "Game/Game.h"
#include "Timer_Wrapper.h"

typedef struct AppState {
    Gyro* Gyro;
    GameState* Game;
    bool ScreenTouched;
    struct {
        uint16_t x;
        uint16_t y;
    } ScreenTouchedPos;
    bool TimerEvent;
    bool ButtonPressed;
    uint32_t Random;

    GpTimers GameTick;
    GpTimers GameTimer;

    uint32_t GameTime;
} AppState;

#else
typedef void AppState;
#endif

#define NAME_LEN 5

AppState* init_app();

Result app_loop(AppState** state);