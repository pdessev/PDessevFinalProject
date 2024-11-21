/*
 * GPIO_Driver.h
 *
 *  Created on: Sep 24, 2024
 *      Author: peter
 */

#pragma once
#include <stdint.h>

#include "InterruptControl.h"
#include "stm32f429xx.h"
#include "stm32f4xx_hal.h"

typedef enum GpioPorts {
	GpioPortA,
	GpioPortB,
	GpioPortC,
	GpioPortD,
	GpioPortE,
	GpioPortF,
	GpioPortG,
	GpioPortH,
	GpioPortI,
	GpioPortJ,
	GpioPortK,
} GpioPorts;

typedef enum pinNumbers {
    GpioPin0 = GPIO_PIN_0,
    GpioPin1 = GPIO_PIN_1,
    GpioPin2 = GPIO_PIN_2,
    GpioPin3 = GPIO_PIN_3,
    GpioPin4 = GPIO_PIN_4,
    GpioPin5 = GPIO_PIN_5,
    GpioPin6 = GPIO_PIN_6,
    GpioPin7 = GPIO_PIN_7,
    GpioPin8 = GPIO_PIN_8,
    GpioPin9 = GPIO_PIN_9,
    GpioPin10 = GPIO_PIN_10,
    GpioPin11 = GPIO_PIN_11,
    GpioPin12 = GPIO_PIN_12,
    GpioPin13 = GPIO_PIN_13,
    GpioPin14 = GPIO_PIN_14,
    GpioPin15 = GPIO_PIN_15,
    GpioPinAll = GPIO_PIN_All,
} pinNumbers;

typedef enum pinMode { 
    GpioPinModeInput = MODE_INPUT,
    GpioPinModeGpOut = MODE_OUTPUT,
    GpioPinModeAltFunc = MODE_AF,
    GpioPinModeAnalog = MODE_ANALOG,
} pinMode;

typedef enum outputType {
    GpioPushPull = OUTPUT_PP,
    GpioOpenDrain = OUTPUT_OD,
} outputType;

typedef enum outputSpeed {
    GpioSpeedLow = GPIO_SPEED_FREQ_LOW,
    GpioSpeedMedium = GPIO_SPEED_FREQ_MEDIUM,
    GpioSpeedHigh = GPIO_SPEED_FREQ_HIGH,
    GpioSpeedVeryHigh = GPIO_SPEED_FREQ_VERY_HIGH,
} outputSpeed;

typedef enum pullUpPullDown {
    GpioFloat = GPIO_NOPULL,
    GpioPullUp = GPIO_PULLUP,
    GpioPullDown = GPIO_PULLDOWN,
} pullUpPullDown;

typedef enum InterruptMode {
    InterruptModeNone = EXTI_IT,
    InterruptModeFalling = EXTI_IT | TRIGGER_FALLING,
    InterruptModeRising = EXTI_IT | TRIGGER_RISING,
    InterruptModeBoth = EXTI_IT | TRIGGER_FALLING | TRIGGER_RISING,
} InterruptMode;

#pragma pack(4)
typedef struct GpioPinConfig {
    pinNumbers PinNumber;           // Pin Number
    pinMode PinMode;                // Pin Mode
    pullUpPullDown PinPuPdControl;  // Pin Push up / Pull Down Control
    outputSpeed PinSpeed;           // Pin Speed
    uint8_t PinAltFunMode;          // Alternate Function mode
    outputType OutputType;          // Output Type
    InterruptMode IntrMode;         // Interrupt Mode
} GpioPinConfig;
#pragma pack()

void GPIO_init_gpio(GpioPorts port, GpioPinConfig* config);

uint8_t GPIO_read_gpio_pin(GpioPorts port, pinNumbers pin);

void GPIO_write_gpio_pin(GpioPorts port, pinNumbers pin, uint8_t set);

void GPIO_toggle_gpio_pin(GpioPorts port, pinNumbers pin);