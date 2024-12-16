/**
 * @file            ApplicationCode.h
 * @brief           Header for `ApplicationCode.c` \n
 *                    This file contains functions and structure 
 *                    definitions for interfacing game logic to the peripherals.
 * \n Created on: Sep 24, 2024
 * \n Author: peter
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

/**
 * @brief Contains the synchronous state of the peripherals used for the game
 * 
 */
typedef struct AppState {
    Gyro* Gyro;             //!< Gyro state; Unused
    GameState* Game;        //!< Game state
    bool ScreenTouched;     //!< Screen has been touched since last frame
    struct {                
        uint16_t x;
        uint16_t y;
    } ScreenTouchedPos;     //!< Screen touch position
    bool TimerEvent;        //!< Game tick timer overflow
    bool ButtonPressed;     //!< User button has been pressed since last frame
    uint32_t Random;        //!< Latest random number generated

    GpTimers GameTick;      //!< Game tick timer instance
    GpTimers GameTimer;     //!< Game timer timer instance

    uint32_t GameTime;      //!< In-game time
} AppState;

#else
typedef void AppState;
#endif

#define NAME_LEN 5

/**
 * Initializes the application, the game, and all nescessary peripherals to run the game.
 * # Inputs
 * None
 * # Returns
 * - `AppState *` on succesfull initialization of peripherals and application
 * - NULL on error
 * # Notes
 * Calling this function multiple times is undefined behaiviour
 */
AppState* init_app();

/** 
 * Executes the application loop, including fetching information from peripherals
 * and executing game states.
 * # Inputs
 * - `AppState**` state
 * # Returns
 * - `Result(0)` on sucessful game exit
 * - `Result(char* error)` on error
 * # Notes
 * `app_loop` is expected not to exit.
 */
Result app_loop(AppState** state);