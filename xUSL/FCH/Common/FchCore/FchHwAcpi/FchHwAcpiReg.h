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

#define FCHSMI_40                0x40

#define FCH_SMI_GEVENT1                (FCHSMI_40 + 1)
#define FCH_SMI_GEVENT23               (FCHSMI_40 + 23)
#define FCH_SMI_GEVENT3                (FCHSMI_40 + 3)
#define FCH_SMI_GEVENT4                (FCHSMI_40 + 4)
#define FCH_SMI_GEVENT5                (FCHSMI_40 + 5)
#define FCH_SMI_GEVENT6                (FCHSMI_40 + 6)
#define FCH_SMI_GEVENT8                (FCHSMI_40 + 8)

#define FCHSMI_48                   (FCHSMI_40 + 48)
#define FCHSMI_44            (FCHSMI_40 + 44)
#define FCHSMI_29               (FCHSMI_40 + 29)
#define FCHSMI_30                (FCHSMI_40 + 30)

#define FCHSMI_56               (FCHSMI_40 + 56)
#define FCHSMI_57               (FCHSMI_40 + 57)
#define FCHSMI_61               (FCHSMI_40 + 61)
#define FCHSMI_62               (FCHSMI_40 + 62)

#define FCHSMI_37             (FCHSMI_40 + 37)
#define FCHSMI_38             (FCHSMI_40 + 38)
