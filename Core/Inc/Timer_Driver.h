// /*
//  * Timer_Driver.h
//  *
//  *  Created on: Oct 8, 2024
//  *      Author: peter
//  */

// #pragma once
// #include "InterruptControl.h"
// #include <stdint.h>

// #define GP_TIMER_CONTROL_ONE_ENABLE_OFST 0
// // #define GP_TIMER_CONTROL_ONE_ENABLE_CLEAR 0b1

// typedef enum GpTimers {
//     TimerTwo,
//     TimerThree,
//     TimerFour,
//     TimerFive,
// } GpTimers;

// // ControlTwo(TIMx_CR2) Bit 6:4
// #define GP_TIMER_MASTER_MODE_CLEAR 0b111
// #define GP_TIMER_MASTER_MODE_OFST 4
// typedef enum GpTimerMasterModes {
//     MasterModeReset = 0b000,
//     MasterModeEnable = 0b001,
//     MasterModeUpdate = 0b010,
//     MasterModeComparePulse = 0b011,
//     MasterModeOutCompOne = 0b100,
//     MasterModeOutCompTwo = 0b101,
//     MasterModeOutCompThree = 0b110,
//     MasterModeOutCompFour = 0b111,
// } GpTimerMasterModes;

// // ControlOne(TIMx_CR1) Bit 9:8
// #define GP_TIMER_CLOCK_DIVISION_CLEAR 0b11
// #define GP_TIMER_CLOCK_DIVISION_OFST 8
// typedef enum GpTimerClockDivision {
//     ClockDivisionOne = 0b00,
//     ClockDivisionTwo = 0b01,
//     ClockDivisionFour = 0b10,
//     // ClockDivisionReserved = 0b11,
// } GpTimerClockDivision;

// // ControlOne(TIMx_CR1) Bits 6:5
// #define GP_TIMER_CENTER_ALIGN_MODE_CLEAR 0b11
// #define GP_TIMER_CENTER_ALIGN_MODE_OFST 5
// typedef enum GpTimerCenterAlignMode {
//     EdgeAlign = 0b00,
//     SetFlagsCountingDown = 0b01,
//     SetFlagsCountingUp = 0b10,
//     SetFlagsCountingBoth = 0b11,
// } GpTimerCenterAlignMode;

// // ControlOne(TIMx_CR1) Bit 7
// #define GP_TIMER_AUTO_RELOAD_BUFFER_EN_CLEAR 0b1
// #define GP_TIMER_AUTO_RELOAD_BUFFER_EN_OFST 7
// typedef enum GpTimerAutoReloadBufferEnable {
//     DisableAutoReloadBuffer = 0b0,
//     EnableAutoReloadBuffer = 0b1,
// } GpTimerAutoReloadBufferEnable;

// // ControlOne(TIMx_CR1) Bit 4
// #define GP_TIMER_DIRECTION_CLEAR 0b1
// #define GP_TIMER_DIRECTION_OFST 4
// typedef enum GpTimerDirection {
//     CountUp = 0b0,
//     CountDown = 0b1,
// } GpTimerDirection;

// // DmaInterruptEnable(TIMx_DIER) Bit 0
// #define GP_TIMER_GENERATE_INTERRUPT_CLEAR 0b1
// #define GP_TIMER_GENERATE_INTERRUPT_OFST 0
// typedef enum GpTimerInterruptUpdateEnable {
//     DisableInterruptUpdate = 0b0,
//     EnableInterruptUpdate = 0b1,
// } GpTimerInterruptUpdateEnable;

// // ControlOne(TIMx_CR1) Bit 1
// #define GP_TIMER_ENABLE_UPDATE_EVENTS_CLEAR 0b1
// #define GP_TIMER_ENABLE_UPDATE_EVENTS_OFST 1
// typedef enum GpTimerEnableUpdateEvents {
//     EnableUpdateEvents = 0b0,
//     DisableUpdateEvents = 0b1,
// } GpTimerEnableUpdateEvents;

// // ControlOne(TIMx_CR1) Bit 3
// #define GP_TIMER_ONE_PULSE_CLEAR 0b1
// #define GP_TIMER_ONE_PULSE_OFST 3
// typedef enum GpTimerOnePulseMode {
//     DisableOnePulseMode = 0b0,
//     EnableOnePulseMode = 0b1,
// } GpTimerOnePulseMode;

// typedef struct GpTimerConfig {
//     uint32_t AutoReload;
//     GpTimerMasterModes MasterMode;
//     GpTimerClockDivision ClockDivision;
//     uint16_t Prescaler;
//     GpTimerCenterAlignMode CenterAlignMode;
//     GpTimerAutoReloadBufferEnable AutoReloadBuffer;
//     GpTimerDirection Direction;
//     GpTimerInterruptUpdateEnable GenerateInterrupt;
//     GpTimerEnableUpdateEvents EnableUpdateEvents;
//     GpTimerOnePulseMode OnePulseMode;
// } GpTimerConfig;

// // Status(TIMx_SR) Bit 0
// #define GP_TIMER_UPDATE_INTERRUPT_FLAG_CLEAR 0b1
// #define GP_TIMER_UPDATE_INTERRUPT_FLAG_OFST 0

// void init_timer(GpTimers timer, GpTimerConfig* cfg);

// void timer_enable_clock(GpTimers timer, uint8_t enable);

// void start_timer(GpTimers timer);

// void stop_timer(GpTimers timer);

// void reset_timer(GpTimers timer);

// uint32_t get_timer_count(GpTimers timer);

// void timer_start_from(GpTimers timer, uint32_t val);

// uint32_t get_timer_auto_reload(GpTimers timer);

// void set_timer_auto_reload(GpTimers timer, uint32_t val);

// void set_timer_interrupt(GpTimers timer, GpTimerInterruptUpdateEnable en);

// void clear_timer_interrupt_flag(GpTimers timer);

// // -----------------------------------------------
// // Automatic Prescalar and AutoReload Calculation
// // -----------------------------------------------
// // Works best for values above 10 microseconds, when error between expected and
// // actual timer timing is at most 95% and becomes 99.5% at 100 microseconds. Is
// // capable of producing values from 1 microsecond to 42 megaseconds
// uint16_t timer_calculate_prescalar(float seconds);

// uint32_t timer_calculate_AutoReload(float seconds, uint16_t prescalar);
