/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchCommon.h
 * @brief OpenSIL FCH common functions header.
 */

#pragma once

#include <SilCommon.h>
#include <FCH/FchClass-api.h>

// ACPI
#define FCH_ACPI_MMIO_BASE                0xFED80000ul
#define SIL_RSVD_ADDR_02D01000                 0x02D01000ul
#define ACPI_MMIO_BASE                    FCH_ACPI_MMIO_BASE

// ACPI sub components
#define SMI_BASE                          0x200
#define PMIO_BASE                         0x300
#define ASF_BASE                          0x900
#define IOMUX_BASE                        0xD00
#define MISC_BASE                         0xE00
#define REMOTE_GPIO_BASE                  0x1200
#define GPIO_BANK0_BASE                   0x1500
#define GPIO_BANK1_BASE                   0x1600
#define GPIO_BANK2_BASE                   0x1700
#define GPIO_BANK3_BASE                   0x1800
#define AOAC_BASE                         0x1E00


// Remote GPIO
#define SIL_RSVD_ADDR_02D02200          0x02D02200ul

// ESPI
#define SIL_RSVD_ADDR_FEC20000            0xFEC20000ul
#define SIL_RSVD_ADDR_FEC30000            0xFEC30000ul

// Power Management
#define SIL_RSVD_ADDR_02D01300                 0x02D01300ul

// SPI
#define SPI_BASE                          0xFEC10000ul

// RSMU
#define FCH_SMN_RSMU_BASE                 0x02D00000ul

// SATA
#define FCH_SMN_SATA_CONTROL_BASE         0x03100000ul
#define FCH_SMN_SATA_CONTROL_RSMU         FCH_SMN_SATA_CONTROL_BASE
#define SIL_RESERVED_0274         FCH_SMN_SATA_CONTROL_BASE + 0x1000
#define SIL_RESERVED_0275         FCH_SMN_SATA_CONTROL_BASE + 0x1800

// XHCI
#define SIL_RESERVED_0277  0x00120000ul
#define SIL_RESERVED_0276       0x00180000ul

#define FCH_MAX_TIMER                     0xFFFFFFFFul
#define FCHOEM_IO_DELAY_PORT              0x80
#define FCHOEM_ELAPSED_TIME_UNIT          28
#define FCHOEM_ELAPSED_TIME_DIVIDER       100

#define ACPIMMIO32(x)                     (*(volatile uint32_t *)(size_t)(x))
#define ACPIMMIO16(x)                     (*(volatile uint16_t *)(size_t)(x))
#define ACPIMMIO8(x)                      (*(volatile uint8_t * )(size_t)(x))

#define ACPI_SLPTYP_S1                    0x1
#define ACPI_SLPTYP_S3                    0x3
#define ACPI_SLPTYP_S4                    0x4
#define ACPI_SLPTYP_S5                    0x5

#define SIL_RESERVED_0273             16
#define FCH_GPIO_OUTPUT_ENABLE            (1 << 7)

#define FCH_LPC_BUS                       0
#define FCH_LPC_DEV                       0x14
#define FCH_LPC_FUNC                      3

#define FCH_ISA_BUS                       0
#define FCH_ISA_DEV                       0x14
#define FCH_ISA_FUNC                      0

#define LPC_BUS_DEV_FUN                   ((FCH_LPC_BUS << 8) + (FCH_LPC_DEV << 3) + FCH_LPC_FUNC)
#define SMBUS_BUS_DEV_FUN                 ((0x14 << 3) + 0)

#define PCI_ADDRESS(bus, dev, func, reg) \
        (uint32_t) ((((uint32_t)bus) << 24) + (((uint32_t)dev) << 19) + (((uint32_t)func) << 16) + ((uint32_t)reg))

typedef struct _REG8_MASK {
  uint8_t                 RegIndex;
  uint8_t                 AndMask;
  uint8_t                 OrMask;
} REG8_MASK;

void
SilFchStall (
  uint32_t MicroSeconds
  );

SIL_STATUS
SilFchGpioSlotResetControl (
  void
  );

void
ProgramPciByteTable (
  REG8_MASK *pPciByteTable,
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

void
SilFchWriteSleepType (
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
