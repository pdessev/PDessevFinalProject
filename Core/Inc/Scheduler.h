/*
 * Scheduler.h
 *
 *  Created on: Sep 24, 2024
 *      Author: peter
 */

#pragma once
#ifdef __SCHEDULER_INTERNAL__
#include <stdint.h>
#endif

typedef enum Event {
    EventLedToggle = 0,
    EventDelay = 1,
    EventPollButton = 2,
    EventIdAndTemp = 3,
} Event;

// #define LED_TOGGLE_EVENT (1 << EventLedToggle)
// #define DELAY_EVENT (1 << EventLedToggle)
// #define POLL_BUTTON_EVENT (1 << EventPollButton)

uint32_t getScheduledEvents();

void addSchedulerEvent(Event e);

void removeSchedulerEvent(Event e);
