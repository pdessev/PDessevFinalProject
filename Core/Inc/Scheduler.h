/**
 * @file            Scheduler.h
 * @brief           Header for `Scheduler.c` \n
 *                    This file contains functions and structure 
 *                    definitions for the task scheduler for a `Game` instance.
 * \n Created on: Sep 24, 2024
 * \n Author: peter
 * 
 */

#pragma once
#ifdef __SCHEDULER_INTERNAL__
#include <stdint.h>
#endif

/**
 * Game events for controlling game state.
 * 
 */
typedef enum Event {
    EventDrawMainMenu,
    EventStartGame,
    EventEndGame,
} Event;

// #define LED_TOGGLE_EVENT (1 << EventLedToggle)
// #define DELAY_EVENT (1 << EventLedToggle)
// #define POLL_BUTTON_EVENT (1 << EventPollButton)

/** 
 * Gets the currently scheduled events.
 * # Inputs
 * None
 * # Returns
 * - `uint32_t`: The scheduled events.
 */
uint32_t getScheduledEvents();

/** 
 * Schedules an event to be executed.
 * # Inputs
 * - `Event e`: The event to be scheduled.
 * # Returns
 * - `void`
 */
void addSchedulerEvent(Event e);

/** 
 * Unschedules an event to be executed.
 * # Inputs
 * - `Event e`: The event to be unscheduled.
 * # Returns
 * - `void`
 */
void removeSchedulerEvent(Event e);
