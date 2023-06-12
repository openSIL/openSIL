/**
 * @file  FchSpiReg.h
 * @brief FCH SPI controller registers
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

/**********************************************************************************************************************
 * @brief Function prototypes
 *
 */

/**********************************************************************************************************************
 * @brief Module data structure
 *
 */


/**********************************************************************************************************************
 * @brief Declare macros here
 *
 */
#define FCH_SPI_MMIO_REG00                               0x00
#define FCH_SPI_MMIO_REG0C                               0x0C //SPI_Cntrl1 Register
#define FCH_SPI_MMIO_REG1C                               0x1C
#define FCH_SPI_MMIO_REG20                               0x20
#define FCH_SPI_MMIO_REG22                               0x22
#define FCH_SPI_MMIO_REG2C                               0x2C
#define FCH_SPI_MMIO_REG32_SPI100_DUMMY_CYCLE_CONFIG     0x32
#define FCH_SPI_MMIO_REG58_ADDR32_CTRL2                  0x58
#define FCH_SPI_MODE_QUAL_114                            0x3
#define FCH_SPI_MODE_QUAL_122                            0x4
#define FCH_SPI_MODE_QUAL_144                            0x5
#define FCH_SPI_BUSY                                     0x80000000l
#define FCH_SPI_EXEC_OPCODE                              0x00010000l
#define FCH_SPI_MMIO_REG1F_X05_TX_BYTE_COUNT             0x05
#define FCH_SPI_MMIO_REG1F_X06_RX_BYTE_COUNT             0x06
#define FCH_SPI_MMIO_REG1E                               0x1E
#define FCH_SPI_MMIO_REG1F                               0x1F
#define FCH_SPI_RETRY_TIMES                              0x3
#define FCH_SPI_MMIO_REG80_FIFO                          0x80
#define FCH_SPI_MMIO_REG50_ADDR32_CTRL0                  0x50
#define FCH_SPI_DEVICE_COMPLETION_STATUS                 0x1
#define FCH_SPI_DEVICE_WRITE_ENABLED                     0x2
#define FCH_SPI_ROM_ADDR_32                              0x1
#define FCH_SPI_MANUFACTURER_ID_JEDEC                    0x20
#define FCH_SPI_DUMMY_CYCLE_COUNT                        0x8A
#define FCH_SPI_NORMAL_SPEED_BIT_OFFSET                  28
#define FCH_SPI_WRITE_SPEED_BIT_OFFSET                   20
#define FCH_SPI_TPM_SPEED_BIT_OFFSET                     16
