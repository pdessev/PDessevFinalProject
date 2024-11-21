/*
 * Gyro.c
 *
 *  Created on: Nov 12, 2024
 *      Author: peter
 */

#include "stm32f4xx_hal.h"
#include "Spi.h"

#include <stdlib.h>
#include <stdbool.h>

#define __GYRO_INTERNAL__
#include "Gyro.h"
#include "ErrorHandling.h"

void assert_gyro(Gyro* g, HAL_SPI_StateTypeDef state){
    a_assert(HAL_SPI_GetState(g->handle) == state);
}

void enable_gyro() {
    GPIO_write_gpio_pin(SPI5_MEMS_PORT, SPI5_MEMS_PIN, 0);
}

void disable_gyro() {
    GPIO_write_gpio_pin(SPI5_MEMS_PORT, SPI5_MEMS_PIN, 1);
}

HAL_StatusTypeDef write_data(Gyro* g, GyroReadWrite rw, GyroMultiSelect ms, gyroAddresses addr, uint8_t data) {
    uint16_t transmition = 0;
    transmition |= (rw & GyroRwBitMask) << GyroRwBitPos;
    transmition |= (ms & GyroMsBitMask) << GyroMsBitPos;
    transmition |= (addr & GyroAddrBitMask) << GyroAddrBitPos;
    transmition |= (data & GyroDataBitMask) << GyroDataBitPos;

    enable_gyro();
    while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1) != GPIO_PIN_RESET);
    HAL_StatusTypeDef res = HAL_SPI_Transmit(g->handle, (uint8_t*)&transmition, GyroMosiWriteSize, HAL_MAX_DELAY);
    disable_gyro();

    return res;
}

HAL_StatusTypeDef read_data(Gyro* g, GyroReadWrite rw, GyroMultiSelect ms, gyroAddresses addr, uint16_t* output) {
    uint16_t transmition = 0;
    transmition |= (rw & GyroRwBitMask) << GyroRwBitPos;
    transmition |= (ms & GyroMsBitMask) << GyroMsBitPos;
    transmition |= (addr & GyroAddrBitMask) << GyroAddrBitPos;

    enable_gyro();
    while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1) != GPIO_PIN_RESET);
    HAL_StatusTypeDef res = HAL_SPI_TransmitReceive(g->handle, (uint8_t*)&transmition, (uint8_t*)output, GyroMosiWriteSize, HAL_MAX_DELAY);
    *output >>= 8;

    disable_gyro();
    return res;
}


Gyro* gyro_init(){
    Gyro* g = calloc(1, sizeof(*g));
    g->handle = calloc(1, sizeof(*(g->handle)));
    
    g->handle->Instance = (SPI_TypeDef*)spi5;

    // SPI_CR?->{MSTR, SPE} set
    g->handle->Init.Mode = SPI_MODE_MASTER;
    g->handle->Init.Direction = SPI_DIRECTION_2LINES;
    // SPI_CR1->DFF set
    g->handle->Init.DataSize = SPI_DATASIZE_8BIT;
    // CPOL set
    g->handle->Init.CLKPolarity = SPI_POLARITY_HIGH;
    // CPHA set
    g->handle->Init.CLKPhase = SPI_PHASE_2EDGE;
    // NSS is software driven, SPI_CR1->SSM = 1
    //      SPI_CR1->SSI controls CS
    g->handle->Init.NSS = SPI_NSS_SOFT;
    // SPI_CR1->BR[2:0] sets the baud rate
    g->handle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
    // SPI_CR2->FRF reset
    g->handle->Init.FirstBit = SPI_FIRSTBIT_MSB;

    g->handle->Init.TIMode = SPI_TIMODE_DISABLE;
    g->handle->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;

    HAL_SPI_Init(g->handle);
    

    GpioPinConfig* pins = calloc(4, sizeof(*pins));
    GpioPinConfig* sck = pins; 
    GpioPinConfig* miso = pins + 1;
    GpioPinConfig* mosi = pins + 2;
    GpioPinConfig* cs = pins + 3;

    sck->PinNumber = SPI5_SCK_PIN;
    sck->PinMode = GpioPinModeAltFunc;
    sck->PinPuPdControl = GpioFloat;
    sck->PinSpeed = GpioSpeedLow;
    sck->PinAltFunMode = GPIO_AF5_SPI5;
    sck->OutputType = GpioPushPull;
    sck->IntrMode = InterruptModeNone;

    miso->PinNumber = SPI5_MISO_PIN;
    miso->PinMode = GpioPinModeAltFunc;
    miso->PinPuPdControl = GpioFloat;
    miso->PinSpeed = GpioSpeedLow;
    miso->PinAltFunMode = GPIO_AF5_SPI5;
    miso->OutputType = GpioPushPull;
    miso->IntrMode = InterruptModeNone;

    mosi->PinNumber = SPI5_MOSI_PIN;
    mosi->PinMode = GpioPinModeAltFunc;
    mosi->PinPuPdControl = GpioFloat;
    mosi->PinSpeed = GpioSpeedLow;
    mosi->PinAltFunMode = GPIO_AF5_SPI5;
    mosi->OutputType = GpioPushPull;
    mosi->IntrMode = InterruptModeNone;

    cs->PinNumber = SPI5_MEMS_PIN;
    cs->PinMode = GpioPinModeGpOut;
    cs->PinPuPdControl = GpioPullUp;
    cs->PinSpeed = GpioSpeedVeryHigh;
    cs->PinAltFunMode = 0;
    cs->OutputType = GpioOpenDrain;
    cs->IntrMode = InterruptModeNone;

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();

    GPIO_init_gpio(SPI5_SCK_PORT, sck);
    GPIO_init_gpio(SPI5_MOSI_PORT, mosi);
    GPIO_init_gpio(SPI5_MISO_PORT, miso);
    GPIO_init_gpio(SPI5_MEMS_PORT, cs);
    // GPIO_InitTypeDef* a = calloc(1, sizeof(*a));
    // a->Pin = GPIO_PIN_1;
    // a->Mode = GPIO_MODE_OUTPUT_OD;
    // a->Pull = GPIO_PULLUP;
    // a->Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    // HAL_GPIO_Init(GPIOC, a);
    // free(a);

    free(pins);
    gyro_verify_spi_state(g);
    return g;
}

uint8_t gyro_get_id(Gyro* g){
    uint16_t id = 0;
    
    a_assert(read_data(g, GyroRead, GyroSingleData, GyroWhoAmIAddr, &id) == HAL_OK);

    return id;
}

void gyro_power(Gyro* g){
    uint16_t ctrl1 = 0;
    // read_data(g, GyroRead, GyroSingleData, GyroCtrlReg1Addr, &ctrl1);

    ctrl1 |= 1 << GyroPd;
    write_data(g, GyroWrite, GyroSingleData, GyroCtrlReg1Addr, ctrl1);
}

int8_t gyro_get_temp(Gyro* g){
    uint16_t temp = 0;

    a_assert(read_data(g, GyroRead, GyroSingleData, GyroOutTempAddr, &temp) == HAL_OK);

    return (int8_t) temp;
}

void gyro_config_regs(Gyro* g){
    uint16_t ctrl1 = 0;
    a_assert(read_data(g, GyroRead, GyroSingleData, GyroCtrlReg1Addr, &ctrl1) == HAL_OK);

    ctrl1 &= 0b11 << GyroBw;
    ctrl1 |= 0b01 << GyroBw;

    a_assert(write_data(g, GyroRead, GyroSingleData, GyroCtrlReg1Addr, ctrl1) == HAL_OK);

    
    uint16_t ctrl5 = 0;
    a_assert(read_data(g, GyroRead, GyroSingleData, GyroCtrlReg5Addr, &ctrl5) == HAL_OK);

    ctrl5 |= 1 << GyroBoot;

    a_assert(write_data(g, GyroRead, GyroSingleData, GyroCtrlReg5Addr, ctrl5) == HAL_OK);
}

// void gyro_read_data(Gyro* g, GyroOutput* out){
//     a_assert(read_data(g, GyroRead, GyroSingleData, GyroOutXHAddr, (uint8_t*)(out->x)) == HAL_OK);

//     a_assert(read_data(g, GyroRead, GyroSingleData, GyroOutXLAddr, (uint8_t*)(out->x) + 1) == HAL_OK);
// }

void gyro_verify_spi_state(Gyro* g){
    a_assert(HAL_SPI_GetState(g->handle) == HAL_SPI_STATE_READY);
}

void gyro_enable_slave_comm(){
    enable_gyro();
}

void gyro_disable_slave_comm(){
    disable_gyro();
}
