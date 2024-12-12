/*
 * ApplicationCode.h
 *
 *  Created on: Sep 24, 2024
 *      Author: peter
 */

#pragma once
#include "ErrorHandling.h"

#ifdef __APPLICATION_CODE_INTERNAL__
// #include <stdlib.h>
#include <stdio.h>
#include "Gyro.h"
#include "Game/Game.h"

typedef struct AppState {
    Gyro* Gyro;
    GameState* Game;
    bool ScreenTouched;
    struct {
        uint16_t x;
        uint16_t y;
    } ScreenTouchedPos;
    bool TimerEvent;
    uint32_t Random;
} AppState;

#else
typedef void AppState;
#endif

#define NAME_LEN 5

AppState* init_app();

Result app_loop(AppState** state);