/*
 * Timer_Driver.c
 *
 *  Created on: Oct 8, 2024
 *      Author: peter
 */

#include "Timer_Wrapper.h"
#include "Debug/mem.h"

Irq get_gp_timer_irq(GpTimers timer) {
    switch (timer) {
    case TimerTwo:
        return TIM2_IRQn;
    case TimerThree:
        return TIM3_IRQn;
    case TimerFour:
        return TIM4_IRQn;
    case TimerFive:
        return TIM5_IRQn;
    default:
        return TIM2_IRQn;
    };
}

TIM_TypeDef* get_timer_instance(GpTimers timer){
    switch (timer) {
    case TimerTwo:
        return TIM2;
    case TimerThree:
        return TIM3;
    case TimerFour:
        return TIM4;
    case TimerFive:
        return TIM5;
    default:
        return NULL;
    };
}

static TIM_HandleTypeDef* timer_handles[GpTimerSize] = {0};

void init_timer(GpTimers timer, GpTimerConfig* cfg) {
    TIM_HandleTypeDef* tim = calloc(1, sizeof(*tim));
    if (tim == NULL)
        return;

    tim->Instance = get_timer_instance(timer);
    tim->Init.AutoReloadPreload = cfg->AutoReloadBuffer;
    tim->Init.ClockDivision = cfg->ClockDivision;
    tim->Init.CounterMode = cfg->CenterAlignMode ? cfg->CenterAlignMode : cfg->Direction;
    tim->Init.Period = cfg->AutoReload;
    tim->Init.Prescaler = cfg->Prescaler;
    // tim->Init.RepetitionCounter = ;

    // config->PinMode |= config->OutputType | config->IntrMode;
    HAL_TIM_Base_Init(tim);
    __HAL_TIM_URS_ENABLE(tim);
    timer_handles[timer] = tim;
    enable_IRQ(get_gp_timer_irq(timer));
}

void start_timer(GpTimers timer) {
    __HAL_TIM_ENABLE_IT(timer_handles[timer], TIM_IT_UPDATE);
    HAL_TIM_Base_Start_IT(timer_handles[timer]);
    clear_timer_interrupt_flag(timer);
}

void stop_timer(GpTimers timer) {
    __HAL_TIM_DISABLE_IT(timer_handles[timer], TIM_IT_UPDATE);
    HAL_TIM_Base_Stop_IT(timer_handles[timer]);
}

void reset_timer(GpTimers timer) {
    __HAL_TIM_SET_COUNTER(timer_handles[timer], 0);
}

uint32_t get_timer_count(GpTimers timer) {
    return __HAL_TIM_GET_COUNTER(timer_handles[timer]);
}

void timer_start_from(GpTimers timer, uint32_t val) {
    start_timer(timer);
    __HAL_TIM_SET_COUNTER(timer_handles[timer], val);
}

uint32_t get_timer_auto_reload(GpTimers timer) {
    return __HAL_TIM_GET_AUTORELOAD(timer_handles[timer]);
}

void set_timer_auto_reload(GpTimers timer, uint32_t val) {
    __HAL_TIM_SET_AUTORELOAD(timer_handles[timer], val);
}

void set_timer_interrupt(GpTimers timer, GpTimerInterruptUpdateEnable enable) {
    Irq irq = get_gp_timer_irq(timer);
    if (enable) {
        enable_IRQ(irq);
    } else {
        disable_IRQ(irq);
    }
}

void clear_timer_interrupt_flag(GpTimers timer) {
    __HAL_TIM_CLEAR_IT(timer_handles[timer], TIM_IT_UPDATE);
}

static union FtoU {
    float f;
    uint32_t u;
} FtoU;

uint16_t timer_calculate_prescalar(float seconds) {
    // Generation
    FtoU.f = seconds;
    FtoU.u = 0x1FC00000 + (FtoU.u >> 1);

    // Iteration
    float sq = FtoU.f * FtoU.f;
    float num = sq - seconds;
    float denom = 3 * sq + seconds;

    FtoU.f = FtoU.f - 2 * FtoU.f * (num / denom);

    // Transformation
    return 10 * ((uint16_t)FtoU.f) + 1;
}

uint32_t timer_calculate_AutoReload(float seconds, uint16_t prescalar) {
    float num = (float)GP_TIMER_CLOCK_FREQ * seconds;
    float denom = (1.0f + prescalar);

    return (uint32_t)((num / denom) - 1.0f);
}
