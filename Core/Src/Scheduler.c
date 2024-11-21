/*
 * Scheduler.c
 *
 *  Created on: Sep 24, 2024
 *      Author: peter
 */

#define __SCHEDULER_INTERNAL__
#include "Scheduler.h"

static uint32_t scheduledEvents;

uint32_t getScheduledEvents() { return scheduledEvents; }

void addSchedulerEvent(Event e) {
    scheduledEvents |= (1 << e);
}

void removeSchedulerEvent(Event e) {
    scheduledEvents &= ~(1 << e);
}
