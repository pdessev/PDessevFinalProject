// /*
//  * Timer_Driver.c
//  *
//  *  Created on: Oct 8, 2024
//  *      Author: peter
//  */

// #include "Timer_Driver.h"

// Irq get_gp_timer_irq(GpTimers timer) {
//     switch (timer) {
//     case TimerTwo:
//         return TIM2_IRQn;
//     case TimerFive:
//         return TIM5_IRQn;
//     default:
//         return TIM2_IRQn;
//     };
// }

// void init_timer(GpTimers timer, GpTimerConfig* cfg) {
//     timer_enable_clock(timer, 1);
//     GpTimerReg* timer_reg = get_gp_timer_reg(timer);

//     timer_reg->AutoReload = cfg->AutoReload;

//     timer_reg->ControlTwo &=
//         ~(GP_TIMER_MASTER_MODE_CLEAR << GP_TIMER_MASTER_MODE_OFST);
//     timer_reg->ControlTwo |= (cfg->MasterMode << GP_TIMER_MASTER_MODE_OFST);

//     timer_reg->ControlOne &=
//         ~(GP_TIMER_CLOCK_DIVISION_CLEAR << GP_TIMER_CLOCK_DIVISION_OFST);
//     timer_reg->ControlOne |= cfg->ClockDivision << GP_TIMER_CLOCK_DIVISION_OFST;

//     timer_reg->Prescaler = cfg->Prescaler;

//     timer_reg->ControlOne &=
//         ~(GP_TIMER_CENTER_ALIGN_MODE_CLEAR << GP_TIMER_CENTER_ALIGN_MODE_OFST);
//     timer_reg->ControlOne |=
//         (cfg->CenterAlignMode << GP_TIMER_CENTER_ALIGN_MODE_OFST);

//     timer_reg->ControlOne &= ~(GP_TIMER_AUTO_RELOAD_BUFFER_EN_CLEAR
//                                << GP_TIMER_AUTO_RELOAD_BUFFER_EN_OFST);
//     timer_reg->ControlOne |= cfg->AutoReloadBuffer
//                              << GP_TIMER_AUTO_RELOAD_BUFFER_EN_OFST;

//     timer_reg->ControlOne &=
//         ~(GP_TIMER_DIRECTION_CLEAR << GP_TIMER_DIRECTION_OFST);
//     timer_reg->ControlOne |= cfg->Direction << GP_TIMER_DIRECTION_OFST;

//     timer_reg->DmaInterruptEnable &= ~(GP_TIMER_GENERATE_INTERRUPT_CLEAR
//                                        << GP_TIMER_GENERATE_INTERRUPT_OFST);
//     timer_reg->DmaInterruptEnable |= cfg->GenerateInterrupt
//                                      << GP_TIMER_GENERATE_INTERRUPT_OFST;

//     timer_reg->ControlOne &= ~(GP_TIMER_ENABLE_UPDATE_EVENTS_CLEAR
//                                << GP_TIMER_ENABLE_UPDATE_EVENTS_OFST);
//     timer_reg->ControlOne |= cfg->EnableUpdateEvents
//                              << GP_TIMER_ENABLE_UPDATE_EVENTS_OFST;

//     timer_reg->ControlOne &=
//         ~(GP_TIMER_ONE_PULSE_CLEAR << GP_TIMER_ONE_PULSE_OFST);
//     timer_reg->ControlOne |= cfg->OnePulseMode << GP_TIMER_ONE_PULSE_OFST;

//     set_timer_interrupt(timer, cfg->GenerateInterrupt);
// }

// void timer_enable_clock(GpTimers timer, uint8_t enable) {
//     enable ? enable_gp_timer_clock(timer) : disable_gp_timer_clock(timer);
// }

// void start_timer(GpTimers timer) {
//     GpTimerReg* timer_reg = get_gp_timer_reg(timer);
//     timer_reg->ControlOne |= (1 << GP_TIMER_CONTROL_ONE_ENABLE_OFST);
// }

// void stop_timer(GpTimers timer) {
//     GpTimerReg* timer_reg = get_gp_timer_reg(timer);
//     timer_reg->ControlOne &= ~(1 << GP_TIMER_CONTROL_ONE_ENABLE_OFST);
// }

// void reset_timer(GpTimers timer) {
//     GpTimerReg* timer_reg = get_gp_timer_reg(timer);
//     timer_reg->Counter = 0;
// }

// uint32_t get_timer_count(GpTimers timer) {
//     GpTimerReg* timer_reg = get_gp_timer_reg(timer);
//     return timer_reg->Counter;
// }

// void timer_start_from(GpTimers timer, uint32_t val) {
//     start_timer(timer);

//     GpTimerReg* timer_reg = get_gp_timer_reg(timer);
//     timer_reg->Counter = val;
// }

// uint32_t get_timer_auto_reload(GpTimers timer) {
//     GpTimerReg* timer_reg = get_gp_timer_reg(timer);
//     return timer_reg->AutoReload;
// }

// void set_timer_auto_reload(GpTimers timer, uint32_t val) {
//     GpTimerReg* timer_reg = get_gp_timer_reg(timer);
//     timer_reg->AutoReload = val;
// }

// void set_timer_interrupt(GpTimers timer, GpTimerInterruptUpdateEnable enable) {
//     Irq irq = get_gp_timer_irq(timer);
//     if (enable) {
//         enable_IRQ(irq);
//     } else {
//         disable_IRQ(irq);
//     }
// }

// void clear_timer_interrupt_flag(GpTimers timer) {
//     GpTimerReg* timer_reg = get_gp_timer_reg(timer);

//     timer_reg->Status &= ~(GP_TIMER_UPDATE_INTERRUPT_FLAG_CLEAR
//                            << GP_TIMER_UPDATE_INTERRUPT_FLAG_OFST);
// }

// // void timer_disable_interrupts(GpTimers timer) {
// //     GpTimerReg* timer_reg = get_gp_timer_reg(timer);

// //     timer_reg->DmaInterruptEnable &= ~(GP_TIMER_GENERATE_INTERRUPT_CLEAR
// //                                        << GP_TIMER_GENERATE_INTERRUPT_OFST);
// // }

// // void timer_enable_interrupts(GpTimers timer) {
// //     GpTimerReg* timer_reg = get_gp_timer_reg(timer);
// //     timer_reg->DmaInterruptEnable |= (~GP_TIMER_GENERATE_INTERRUPT_CLEAR)
// //                                      << GP_TIMER_GENERATE_INTERRUPT_OFST;
// // }

// static union FtoU {
//     float f;
//     uint32_t u;
// } FtoU;

// uint16_t timer_calculate_prescalar(float seconds) {
//     // Generation
//     FtoU.f = seconds;
//     FtoU.u = 0x1FC00000 + (FtoU.u >> 1);

//     // Iteration
//     float sq = FtoU.f * FtoU.f;
//     float num = sq - seconds;
//     float denom = 3 * sq + seconds;

//     FtoU.f = FtoU.f - 2 * FtoU.f * (num / denom);

//     // Transformation
//     return 10 * ((uint16_t)FtoU.f) + 1;
// }

// uint32_t timer_calculate_AutoReload(float seconds, uint16_t prescalar) {
//     // float clk_div = 1.0f;
//     // switch (div) {
//     // case ClockDivisionOne:
//     // 	break;

//     // case ClockDivisionTwo:
//     //     clk_div = 2.0f;
//     //     break;

//     // case ClockDivisionFour:
//     //     clk_div = 4.0f;
//     // };

//     float num = (float)MCU_CLOCK * seconds;
//     float denom = (1.0f + prescalar);

//     return (uint32_t)((num / denom) - 1.0f);
// }
