/*
 * Spi.h
 *
 *  Created on: Nov 12, 2024
 *      Author: peter
 */

#pragma once

// #include "Spi.h"
#include "GPIO_Wrapper.h"
#include "stm32f4xx_hal.h"

typedef enum spiInterfaces {
    spi1 = (uint32_t)SPI1,
    spi2 = (uint32_t)SPI2,
    spi3 = (uint32_t)SPI3,
    spi4 = (uint32_t)SPI4,
    spi5 = (uint32_t)SPI5,
    spi6 = (uint32_t)SPI6,
} spiInterfaces;

#define SPI5_SCK_PORT GpioPortF
#define SPI5_SCK_PIN GpioPin7

#define SPI5_MISO_PORT GpioPortF
#define SPI5_MISO_PIN GpioPin8

#define SPI5_MOSI_PORT GpioPortF
#define SPI5_MOSI_PIN GpioPin9

#define SPI5_MEMS_PORT GpioPortC
#define SPI5_MEMS_PIN GpioPin1


// typedef enum spiBidMode {
//     spi2LineUnidirectional = SPI_DIRECTION_1LINE,
//     spi1LineBidirectional = SPI_DIRECTION_2LINES,
//     spi
// };

// typedef struct config {

// };

// void init_spi(spiInterfaces interface, spiConfig config);