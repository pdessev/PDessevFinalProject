/*
 * LED_Driver.c
 *
 *  Created on: Sep 24, 2024
 *      Author: peter
 */

#define __LED_DRIVER_INTERNAL__
#include "LED_Driver.h"

/// @brief Stores the pin number of the given led in dest
/// @param led 
/// @param dest 
/// @return 1 if led is not recognized, 0 on succesfull store
uint8_t get_led_pin(Leds led, pinNumbers* dest){
    switch (led){
    case LedRed:
        *dest = RED_LED_PIN;
        break;
    case LedGreen:
        *dest = GREEN_LED_PIN;
        break;
    default:
        return 1;
    };
    return 0;
}


void init_led(Leds led){
    GpioPinConfig* pin = (GpioPinConfig*)calloc(1, sizeof(*pin));
    pin->OutputType = GpioPushPull;
    pin->PinMode = GpioPinModeGpOut;
    pin->PinSpeed = GpioSpeedLow;
    pin->PinPuPdControl = GpioFloat;
    if (get_led_pin(led, &(pin->PinNumber))) {
        free(pin);
        return;
    }

    GPIO_init_gpio(GpioPortG, pin);
    free(pin);
}

void toggle_led(Leds led){
    pinNumbers pin;
    if (get_led_pin(led, &pin)) {
        return;
    }

    GPIO_toggle_gpio_pin(GpioPortG, pin);
}

void off_led(Leds led){    
    pinNumbers pin;
    if (get_led_pin(led, &pin)) {
        return;
    }
    GPIO_write_gpio_pin(GpioPortG, pin, 0);
}

void on_led(Leds led){
    pinNumbers pin;
    if (get_led_pin(led, &pin)) {
        return;
    }
    GPIO_write_gpio_pin(GpioPortG, pin, 1);
}

// void led_init_timer(GpTimers timer, float seconds) {
//     GpTimerConfig* timer_cfg = (GpTimerConfig*)calloc(1, sizeof(*timer_cfg));

//     uint16_t prescalar = timer_calculate_prescalar(seconds);
//     uint32_t auto_reload = timer_calculate_AutoReload(seconds, prescalar);

//     timer_cfg->AutoReload = auto_reload;
//     timer_cfg-> MasterMode = MasterModeReset;
//     timer_cfg->ClockDivision = ClockDivisionFour;
//     timer_cfg->Prescaler = prescalar;
//     timer_cfg->CenterAlignMode = EdgeAlign;
//     timer_cfg->AutoReloadBuffer = DisableAutoReloadBuffer;
//     timer_cfg->Direction = CountUp;
//     timer_cfg->GenerateInterrupt = EnableInterruptUpdate;
//     timer_cfg->EnableUpdateEvents = EnableUpdateEvents;
//     timer_cfg->OnePulseMode = DisableOnePulseMode;

//     init_timer(timer, timer_cfg);

//     free(timer_cfg);
// }

// void led_init_timer_dual(GpTimers timer) {
//     GpTimerConfig* timer_cfg = (GpTimerConfig*)calloc(1, sizeof(*timer_cfg));

//     uint16_t prescalar = 0;
//     uint32_t auto_reload = -1;

//     timer_cfg->AutoReload = auto_reload;
//     timer_cfg-> MasterMode = MasterModeReset;
//     timer_cfg->ClockDivision = ClockDivisionFour;
//     timer_cfg->Prescaler = prescalar;
//     timer_cfg->CenterAlignMode = EdgeAlign;
//     timer_cfg->AutoReloadBuffer = DisableAutoReloadBuffer;
//     timer_cfg->Direction = CountUp;
//     timer_cfg->GenerateInterrupt = EnableInterruptUpdate;
//     timer_cfg->EnableUpdateEvents = EnableUpdateEvents;
//     timer_cfg->OnePulseMode = DisableOnePulseMode;

//     init_timer(timer, timer_cfg);

//     free(timer_cfg);
// }
