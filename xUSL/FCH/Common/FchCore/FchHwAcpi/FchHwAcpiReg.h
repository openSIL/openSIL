/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchHwAcpiReg.h
 * @brief FCH HW ACPI registers definition
 *
 */

#pragma once

/// ACPI MMIO register R/W structure
typedef struct _ACPI_REG_WRITE {
  uint8_t        MmioBase;             ///> MmioBase    : Index of Fch block
  uint8_t        MmioReg;              ///> MmioReg     : Register index
  uint8_t        DataAndMask;          ///> DataANDMask : AND Register Data
  uint8_t        DataOrMask;           ///> DataOrMask  : Or Register Data
} ACPI_REG_WRITE;

#define FCH_IOAPIC_INDEX               0xFEC00000
#define FCH_IOAPIC_DATA                0xFEC00010
#define FCH_IOAPIC_ID_REG              0x00

#define FCH_SMI_GEVENT1                0x41 // SCIMAP1
#define FCH_SMI_GEVENT23               0x57 // SCIMAP23
#define FCH_SMI_GEVENT3                0x43 // SCIMAP3
#define FCH_SMI_GEVENT4                0x44 // SCIMAP4
#define FCH_SMI_GEVENT5                0x45 // SCIMAP5
#define FCH_SMI_GEVENT6                0x46 // SCIMAP6
#define FCH_SMI_GEVENT8                0x48 // SCIMAP8

#define FCH_SMI_SCIMAP0                0x40
#define FCH_SMI_TWRN                   (FCH_SMI_SCIMAP0 + 48)
#define FCH_SMI_FANTHGEVENT            (FCH_SMI_SCIMAP0 + 44)
#define SIL_RESERVED_0282               (FCH_SMI_SCIMAP0 + 29)
#define SIL_RESERVED_0281                (FCH_SMI_SCIMAP0 + 30)
