/**
 * @file            Timer_Wrapper.h
 * @brief           Header for `Timer_Wrapper.c` \n
 *                    This file contains functions and structure 
 *                    definitions for the general purpose timer peripheral interface.
 * \n Created on: Sep 24, 2024
 * \n Author: peter
 * 
 */


#pragma once
#include <stdint.h>

#include "InterruptControl.h"
#include "stm32f429xx.h"
#include "stm32f4xx_hal.h"

#define GP_TIMER_CLOCK_FREQ 84000000
#define GP_TIMER_CONTROL_ONE_ENABLE_OFST 0
// #define GP_TIMER_CONTROL_ONE_ENABLE_CLEAR 0b1

/** 
 * The list of all general purpose timers.
 */
typedef enum GpTimers {
    TimerTwo,
    TimerThree,
    TimerFour,
    TimerFive,
    GpTimerSize,
} GpTimers;

// ControlTwo(TIMx_CR2) Bit 6:4
#define GP_TIMER_MASTER_MODE_CLEAR 0b111
#define GP_TIMER_MASTER_MODE_OFST 4
/** 
 * Master modes for the general purpose timers.
 */
typedef enum GpTimerMasterModes {
    MasterModeReset = 0b000,
    MasterModeEnable = 0b001,
    MasterModeUpdate = 0b010,
    MasterModeComparePulse = 0b011,
    MasterModeOutCompOne = 0b100,
    MasterModeOutCompTwo = 0b101,
    MasterModeOutCompThree = 0b110,
    MasterModeOutCompFour = 0b111,
} GpTimerMasterModes;

// ControlOne(TIMx_CR1) Bit 9:8
#define GP_TIMER_CLOCK_DIVISION_CLEAR 0b11
#define GP_TIMER_CLOCK_DIVISION_OFST 8
/** 
 * General purpose timer clock division settings.
 */
typedef enum GpTimerClockDivision {
    ClockDivisionOne = TIM_CLOCKDIVISION_DIV1,
    ClockDivisionTwo = TIM_CLOCKDIVISION_DIV2,
    ClockDivisionFour = TIM_CLOCKDIVISION_DIV4,
    // ClockDivisionReserved = 0b11,
} GpTimerClockDivision;

// ControlOne(TIMx_CR1) Bits 6:5
#define GP_TIMER_CENTER_ALIGN_MODE_CLEAR 0b11
#define GP_TIMER_CENTER_ALIGN_MODE_OFST 5
/** 
 * General purpose timer center alignment mode settings.
 */
typedef enum GpTimerCenterAlignMode {
    EdgeAlign = 0b00,
    SetFlagsCountingDown = TIM_COUNTERMODE_CENTERALIGNED1,
    SetFlagsCountingUp = TIM_COUNTERMODE_CENTERALIGNED2,
    SetFlagsCountingBoth = TIM_COUNTERMODE_CENTERALIGNED3,
} GpTimerCenterAlignMode;

// ControlOne(TIMx_CR1) Bit 7
#define GP_TIMER_AUTO_RELOAD_BUFFER_EN_CLEAR 0b1
#define GP_TIMER_AUTO_RELOAD_BUFFER_EN_OFST 7
/** 
 * General purpose timer autoreload buffer settings.
 */
typedef enum GpTimerAutoReloadBufferEnable {
    DisableAutoReloadBuffer = TIM_AUTORELOAD_PRELOAD_DISABLE,
    EnableAutoReloadBuffer = TIM_AUTORELOAD_PRELOAD_ENABLE,
} GpTimerAutoReloadBufferEnable;

// ControlOne(TIMx_CR1) Bit 4
#define GP_TIMER_DIRECTION_CLEAR 0b1
#define GP_TIMER_DIRECTION_OFST 4
/** 
 * General purpose timer count direction settings.
 */
typedef enum GpTimerDirection {
    CountUp = TIM_COUNTERMODE_UP,
    CountDown = TIM_COUNTERMODE_DOWN,
} GpTimerDirection;

// DmaInterruptEnable(TIMx_DIER) Bit 0
#define GP_TIMER_GENERATE_INTERRUPT_CLEAR 0b1
#define GP_TIMER_GENERATE_INTERRUPT_OFST 0
/** 
 * General purpose timer generate interrupt on update settings.
 */
typedef enum GpTimerInterruptUpdateEnable {
    DisableInterruptUpdate = 0b0,
    EnableInterruptUpdate = 0b1,
} GpTimerInterruptUpdateEnable;

// ControlOne(TIMx_CR1) Bit 1
#define GP_TIMER_ENABLE_UPDATE_EVENTS_CLEAR 0b1
#define GP_TIMER_ENABLE_UPDATE_EVENTS_OFST 1
/** 
 * General purpose timer update event settings.
 */
typedef enum GpTimerEnableUpdateEvents {
    EnableUpdateEvents = 0b0,
    DisableUpdateEvents = 0b1,
} GpTimerEnableUpdateEvents;

// ControlOne(TIMx_CR1) Bit 3
#define GP_TIMER_ONE_PULSE_CLEAR 0b1
#define GP_TIMER_ONE_PULSE_OFST 3
/** 
 * General purpose timer one pulse settings.
 */
typedef enum GpTimerOnePulseMode {
    DisableOnePulseMode = 0b0,
    EnableOnePulseMode = 0b1,
} GpTimerOnePulseMode;

/** 
 * General purpose timer configuration structure. Passed to initialization functions.
 */
typedef struct GpTimerConfig {
    uint32_t AutoReload;                                //!< Autoreload value
    // GpTimerMasterModes MasterMode;
    GpTimerClockDivision ClockDivision;                 //!< Clock division setting
    uint16_t Prescaler;                                 //!< Prescalar value
    GpTimerCenterAlignMode CenterAlignMode;             //!< Cetner aligment setting
    GpTimerAutoReloadBufferEnable AutoReloadBuffer;     //!< Autoreload buffer enable setting
    GpTimerDirection Direction;                         //!< Coutning direction setting
    // GpTimerInterruptUpdateEnable GenerateInterrupt;
    // GpTimerEnableUpdateEvents EnableUpdateEvents;
    // GpTimerOnePulseMode OnePulseMode;
} GpTimerConfig;

// Status(TIMx_SR) Bit 0
#define GP_TIMER_UPDATE_INTERRUPT_FLAG_CLEAR 0b1
#define GP_TIMER_UPDATE_INTERRUPT_FLAG_OFST 0

/** 
 * Initializes the GP Timer `timer` using the configuration `config`.
 * # Inputs
 * - `GpTimers timer`: The timer to be initialized.
 * - `GpTimerConfig* cfg`: The configuration of the timer to be initialized.
 * # Returns
 * `void`
 */
void init_timer(GpTimers timer, GpTimerConfig* cfg);

/** 
 * Starts the GP Timer `timer`.
 * # Inputs
 * - `GpTimers timer`: The timer to be started.
 * # Returns
 * `void`
 */
void start_timer(GpTimers timer);

/** 
 * Stops the GP Timer `timer`.
 * # Inputs
 * - `GpTimers timer`: The timer to be stopped.
 * # Returns
 * `void`
 */
void stop_timer(GpTimers timer);

/** 
 * Resets the GP Timer `timer`, setting its internal counter to 0.
 * # Inputs
 * - `GpTimers timer`: The timer to be reset.
 * # Returns
 * `void`
 */
void reset_timer(GpTimers timer);

/** 
 * Returns the current count in the GP Timer `timer`.
 * # Inputs
 * - `GpTimers timer`: The timer to be inspected.
 * # Returns
 * - `uint32_t`: The current count in `timer`.
 */
uint32_t get_timer_count(GpTimers timer);

/** 
 * Starts the GP Timer `timer`, loading the value `val` into the counter before doing so.
 * # Inputs
 * - `GpTimers timer`: The timer to be started.
 * - `uint32_t val`: The value at which to start the timer
 * # Returns
 * `void`
 */
void timer_start_from(GpTimers timer, uint32_t val);

/** 
 * Returns the GP Timer `timer`'s autoreload value.
 * # Inputs
 * - `GpTimers timer`: The timer to be inspected.
 * # Returns
 * `uint32_t`: The current autoreload value of `timer`.
 */
uint32_t get_timer_auto_reload(GpTimers timer);

/** 
 * Sets the GP Timer `timer`'s autoreload value.
 * # Inputs
 * - `GpTimers timer`: The timer to be reconfigured.
 * - `uint32_t val`: The new autoreload value for `timer`.
 * # Returns
 * `void`
 */
void set_timer_auto_reload(GpTimers timer, uint32_t val);

/** 
 * Sets the interrupt mode of the GP Timer `timer` to `en`.
 * # Inputs
 * - `GpTimers timer`: The timer to be reconfigured.
 * - `GpTimerInterruptUpdateEnable en`: The new interrupt mode for the timer.
 * # Returns
 * `void`
 */
void set_timer_interrupt(GpTimers timer, GpTimerInterruptUpdateEnable en);

/** 
 * Clears the GP Timer `timer`'s internal interrupt flag.
 * # Inputs
 * - `GpTimers timer`: The timer to be reconfigured.
 * # Returns
 * `void`
 * # Notes
 * Must be called after `timer` has triggered an interrupt, otherwise new interrupts will not be generated.
 */
void clear_timer_interrupt_flag(GpTimers timer);

/** 
 * Sets the GP Timer `timer`'s event interval to occur every `seconds` seconds.
 * # Inputs
 * - `GpTimers timer`: The timer to be reconfigured.
 * - `float seconds`: The number of seconds to time.
 * # Returns
 * `void`
 * # Notes
 * To see the range of values which can be input into `seconds`, refer to `timer_calculate_prescalar`.
 */
void set_timer_time(GpTimers timer, float seconds);

/** 
 * Performs an automatic calculation for the prescalar value, given a time in seconds, 
 * with the purpose of maximizing the automatic calculation coverage of the available timer range.
 * # Inputs
 * - `float seconds`: The number of seconds to time. Can range from 10 microseconds up to 42 megaseconds.
 * # Returns
 * `uint16_t`: The value to be put into the timer prescalar.
 * # Notes
 * The expected accuracy of the timer for a given input is roughly 95% at 
 * 10 microseconds, and 99.5% at 100 microseconds.
 */
uint16_t timer_calculate_prescalar(float seconds);

/** 
 * Performs an automatic calculation for the autoreload value, given a time in seconds and a prescalar, 
 * with the purpose of maximizing the automatic calculation coverage of the available timer range.
 * # Inputs
 * - `float seconds`: The number of seconds to time. Can range from 10 microseconds up to 42 megaseconds.
 * - `uint16_t prescalar`: The prescalar the timer will use, can be from any source, not just `timer_calculate_prescalar`.
 * # Returns
 * `uint32_t`: The value to be put into the timer autoreload.
 * # Notes
 * The expected accuracy of the timer for a given input is roughly 95% at 
 * 10 microseconds, and 99.5% at 100 microseconds.
 */
uint32_t timer_calculate_AutoReload(float seconds, uint16_t prescalar);
