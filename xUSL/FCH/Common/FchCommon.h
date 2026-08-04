/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchCommon.h
 * @brief OpenSIL FCH common functions header.
 */

#pragma once

#include <SilCommon.h>
#include <FCH/FchClass-api.h>

// Sleep types
#define SLEEP_TYPE_S0                     0
#define SLEEP_TYPE_S1                     1
#define SLEEP_TYPE_S2                     2
#define SLEEP_TYPE_S3                     3
#define SLEEP_TYPE_S4                     4
#define SLEEP_TYPE_S5                     5

// ACPI
#define FCH_ACPI_MMIO_BASE                0xFED80000ul
#define ACPI_MMIO_BASE                    FCH_ACPI_MMIO_BASE

// ACPI sub components
#define SMI_BASE                          0x200
#define PMIO_BASE                         0x300
#define ASF_BASE                          0x900
#define IOMUX_BASE                        0xD00
#define MISC_BASE                         0xE00
#define FCHREGx1100                          0x1100
#define REMOTE_GPIO_BASE                  0x1200
#define GPIO_BANK0_BASE                   0x1500
#define GPIO_BANK1_BASE                   0x1600
#define GPIO_BANK2_BASE                   0x1700
#define GPIO_BANK3_BASE                   0x1800
#define AOAC_BASE                         0x1E00

// SMBUSPCI
#define FCHREGxFED80000                 0xFED80000ul

// ESPI
#define FCHREGxFEC20000            0xFEC20000ul

// Power Management
#define FCHREGx02D01300                 0x02D01300ul

// SPI
#define SPI_BASE                          0xFEC10000ul
#define FCH_HFP_BASE_ADDRESS              0xFEC11000ul
#define FCH_HID2_BASE_ADDRESS             0xFEC12000ul
#define FCH_HID_BASE_ADDRESS              0xFEC13000ul
#define HID_CONTROLLER_STEP               0x1000

// RSMU
#define FCH_SMN_RSMU_BASE                 0x02D00000ul

// SDP RSMU

#define FCHREGx02DC6000             0x02DC6000ul

// HFP
#define FCHREGxFEC11000                      0xFEC11000ul

// HID
#define FCHREGxFEC13000                      0xFEC13000ul
#define FCHREGxFEC12000                     0xFEC12000ul

#define FCH_MAX_TIMER                     0xFFFFFFFFul
#define FCHOEM_IO_DELAY_PORT              0x80
#define FCHOEM_ELAPSED_TIME_UNIT          28
#define FCHOEM_ELAPSED_TIME_DIVIDER       100

#define ACPI_SLPTYP_S1                    0x1
#define ACPI_SLPTYP_S3                    0x3
#define ACPI_SLPTYP_S4                    0x4
#define ACPI_SLPTYP_S5                    0x5

#define FCH_LPC_BUS                       0
#define FCH_LPC_DEV                       0x14
#define FCH_LPC_FUNC                      3

#define FCH_ISA_BUS                       0
#define FCH_ISA_DEV                       0x14
#define FCH_ISA_FUNC                      0

#define LPC_BUS_DEV_FUN                   ((FCH_LPC_BUS << 8) + (FCH_LPC_DEV << 3) + FCH_LPC_FUNC)
#define SMBUS_BUS_DEV_FUN                 ((0x14 << 3) + 0)

typedef struct _REG8_MASK {
  uint8_t                 RegIndex;
  uint8_t                 AndMask;
  uint8_t                 OrMask;
} REG8_MASK;

void
SilFchStall (
  uint32_t MicroSeconds
  );

void
ProgramPciByteTable (
  const REG8_MASK *pPciByteTable,
  uint16_t dwTableSize
  );

void
LibFchIndirectIoRead (
  uint8_t ByteCount,
  uint16_t IoBase,
  uint8_t IndexAddress,
  uint8_t *Value
  );

void
LibFchIndirectIoWrite (
  uint8_t ByteCount,
  uint16_t IoBase,
  uint8_t IndexAddress,
  uint8_t *Value
  );

uint8_t
SilFchReadSleepType (
  void
  );

SIL_STATUS
FchCommonSetSleepType (
  uint16_t SleepType
  );

void
SilFchReadPmio (
  uint8_t Address,
  uint8_t ByteCount,
  uint8_t *Values
  );

void
SilFchWritePmio (
  uint8_t Address,
  uint8_t ByteCount,
  uint8_t *Values
  );

void
SilFchRwPmio (
  uint8_t Address,
  uint8_t ByteCount,
  uint32_t AndMask,
  uint32_t Ormask
  );
