/**
 * @file  FchSataReg.h
 * @brief FCH SATA controller registers definition
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once
#include <FCH/Common/FchCommonCfg.h>

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
#define ACPI_MMIO_BASE          0xFED80000ul
#define FCH_GPIO_014_BP_AGPIO5  0x14
#define FCH_GPIO_018_BP_AGPIO6  0x18

#define FCH_SMN_SATA_CONTROL_BASE 0x03100000ul
#define FCH_SMN_SATA_CONTROL_RSMU FCH_SMN_SATA_CONTROL_BASE

#define FCH_SATA_AOAC_CONTROL     FCH_SMN_SATA_CONTROL_RSMU + 0x04

#define SIL_RESERVED_48 FCH_SMN_SATA_CONTROL_BASE + 0x1000

#define FCH_SMN_SATA_STEP     0x100000ul

#define FCH_REG05             0x05
#define FCH_REG06             0x06
#define FCH_REG09             0x09
#define FCH_REG0A             0x0A
#define FCH_REG17             0x17
#define FCH_REG1D             0x1D
#define FCH_REG1E             0x1E
#define FCH_REG1F             0x1F
#define FCH_REG20             0x20
#define FCH_REG28             0x28
#define FCH_REGC0             0xC0
#define FCH_REGC1             0xC1
#define FCH_REGC2             0xC2
#define FCH_REGC3             0xC3
#define FCH_REGC4             0xC4
#define FCH_REGC5             0xC5
#define FCH_REGC6             0xC6

#define SIL_RESERVED_31       0x000
#define SIL_RESERVED_32       0x00C
#define FCH_SATA_BAR5_REG20   0x020 // PRD Address - IDE0 - RW - 32b
#define FCH_SATA_BAR5_REG24   0x024 // PCI Bus Master Byte Cnt - IDE0- RW - 32b
#define SIL_RESERVED_33       0x0E0
#define SIL_RESERVED_51       0x0F4
#define SIL_RESERVED_50       0x0F8
#define SIL_RESERVED_49       0x0FC

#define GPIO_BANK0_BASE       0x1500 // DWORD

#define IOMUX_BASE            0xD00 // BYTE
#define REMOTE_GPIO_BASE      0x1200
