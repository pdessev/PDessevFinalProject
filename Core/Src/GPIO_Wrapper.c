#include "GPIO_Wrapper.h"
#include <stdlib.h>

GPIO_TypeDef* getGpioFromPort(GpioPorts port) {
    switch (port) {
    case GpioPortA:
        return GPIOA;
    case GpioPortB:
        return GPIOB;
    case GpioPortC:
        return GPIOC;
    case GpioPortD:
        return GPIOD;
    case GpioPortE:
        return GPIOE;
    case GpioPortF:
        return GPIOF;
    case GpioPortG:
        return GPIOG;
    case GpioPortH:
        return GPIOH;
    case GpioPortI:
        return GPIOI;
    case GpioPortJ:
        return GPIOJ;
    case GpioPortK:
        return GPIOK;
    default:
        return NULL;
    }
}

void GPIO_init_gpio(GpioPorts port, GpioPinConfig* config) {
    GPIO_TypeDef* gpio = getGpioFromPort(port);
    GPIO_InitTypeDef* i = calloc(1, sizeof(*i));
    if (gpio == NULL) {
        return;
    }

    i->Pin = config->PinNumber;
    i->Alternate = config->PinAltFunMode;
    i->Mode = config->OutputType | config->IntrMode | config->PinMode;
    i->Pull = config->PinPuPdControl;
    i->Speed = config->PinSpeed;

    // config->PinMode |= config->OutputType | config->IntrMode;
    HAL_GPIO_Init(gpio, i);
    free(i);
}

uint8_t GPIO_read_gpio_pin(GpioPorts port, pinNumbers pin) {
    GPIO_TypeDef* gpio = getGpioFromPort(port);
    if (gpio == NULL) {
        return -1;
    }

    GPIO_PinState ret = HAL_GPIO_ReadPin(gpio, pin);
//    free(gpio);
    return ret;
}

void GPIO_write_gpio_pin(GpioPorts port, pinNumbers pin, uint8_t set) {
    GPIO_TypeDef* gpio = getGpioFromPort(port);
    if (gpio == NULL) {
        return;
    }

    HAL_GPIO_WritePin(gpio, pin, set ? GPIO_PIN_SET : GPIO_PIN_RESET);
//    free(gpio);
}

void GPIO_toggle_gpio_pin(GpioPorts port, pinNumbers pin) {
    GPIO_TypeDef* gpio = getGpioFromPort(port);
    if (gpio == NULL) {
        return;
    }

    HAL_GPIO_TogglePin(gpio, pin);
//    free(gpio);
}
