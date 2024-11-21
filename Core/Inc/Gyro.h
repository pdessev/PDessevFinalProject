/*
 * Gyro.h
 *
 *  Created on: Nov 12, 2024
 *      Author: peter
 */

#pragma once
#include <stdint.h>
// #define GYRO_Reserved_ADDR 0x00
// #define GYRO_Reserved_ADDR 0x10

#ifdef __GYRO_INTERNAL__
typedef struct Gyro {
    SPI_HandleTypeDef* handle;
} Gyro;

typedef enum gyroAddresses{
    GyroWhoAmIAddr = 0x0F,
    GyroCtrlReg1Addr = 0x20,
    GyroCtrlReg2Addr = 0x21,
    GyroCtrlReg3Addr = 0x22,
    GyroCtrlReg4Addr = 0x23,
    GyroCtrlReg5Addr = 0x24,
    GyroReferenceDatacaptureAddr = 0x25,
    GyroOutTempAddr = 0x26,
    GyroStatusRegAddr = 0x27,
    GyroOutXLAddr = 0x28,
    GyroOutXHAddr = 0x29,
    GyroOutYLAddr = 0x2A,
    GyroOutYHAddr = 0x2B,
    GyroOutZLAddr = 0x2C,
    GyroOutZHAddr = 0x2D,
    GyroFifoCtrlRegAddr = 0x2E,
    GyroFifoSrcRegAddr = 0x2F,
    GyroInt1CfgAddr = 0x30,
    GyroInt1SrcAddr = 0x31,
    GyroInt1ThsXhAddr = 0x32,
    GyroInt1ThsXlAddr = 0x33,
    GyroInt1ThsYhAddr = 0x34,
    GyroInt1ThsYlAddr = 0x35,
    GyroInt1ThsZhAddr = 0x36,
    GyroInt1ThsZlAddr = 0x37,
    GyroInt1DurationAddr = 0x38,
} gyroAddresses;

typedef enum GyroReadWrite {
    GyroWrite = 0,
    GyroRead = 1,
} GyroReadWrite;

typedef enum GyroMultiSelect { 
    GyroSingleData = 0,
    GyroMultiData = 1,
} GyroMultiSelect;

typedef enum GyroMosiSize {
    GyroMosiReadSize = 1,
    GyroMosiWriteSize = 2,
} GyroMosiSize; 

typedef enum GyroBitPositions {
    GyroAddrBitPos = 0,
    GyroMsBitPos = 6,
    GyroRwBitPos = 7,
    GyroDataBitPos = 8,
} GyroBitPositions;

typedef enum GyroBitMasks {
    GyroDataBitMask = 0xFF,
    GyroAddrBitMask = 0x3F,
    GyroMsBitMask = 0b1,
    GyroRwBitMask = 0b1,
} GyroBitMasks;

typedef enum GyroControl1Pos {
    GyroXen = 0,
    GyroYen = 1,
    GyroZen = 2,
    GyroPd = 3,
    GyroBw = 4,
    GyroOutputDataRate = 6,
} GyroControl1Pos;

typedef enum GyroControl2Pos {
    GyroOutSel0 = 0,
    GyroOutSel1 = 1,
    GyroInSel0 = 2,
    GyroInSel1 = 3,
    GyroHpen = 4,
    GyroFifoEn = 6,
    GyroBoot = 7,
} GyroControl2Pos;

#else
typedef void Gyro;
#endif

// typedef struct GyroOutput {
//     int16_t x,
//     int16_t y,
//     int16_t z,
// } GyroOutput;

Gyro* gyro_init();

uint8_t gyro_get_id(Gyro* g);

void gyro_power(Gyro* g);

int8_t gyro_get_temp(Gyro* g);

void gyro_config_regs(Gyro* g);

// void gyro_read_data(Gyro* g, GyroOutput* out);

void gyro_verify_spi_state(Gyro* g);

void gyro_enable_slave_comm();

void gyro_disable_slave_comm();
