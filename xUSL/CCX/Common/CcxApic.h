/* SPDX-License-Identifier: MIT */
/**
 * Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 */
/**
 * @file  CcxApic.h
 * @brief OpenSIL CCX Apic modes and function declarations.
 *
 */

#pragma once
#include <Utils.h>

/// Configuration modes for AmdApicMode
typedef enum {
  ApicCompatibilityMode = 0x00, // threads below 255 run in xAPIC with xAPIC ACPI structures and threads 255 &
                                // above run in x2 mode with x2 ACPI structures
  xApicMode = 0x01,             // force legacy xApic mode (downcores if enabled threads are >= 255)
  x2ApicMode = 0x02,            // force x2Apic mode independent of thread count
  ApicAutoMode = 0xFF           // Auto
} APIC_MODE;

// If IOMMU or X2APIC IOMMU is disabled, xApicMode need to be applied
typedef struct {
  uint32_t                Iommu_Sup:1;
  uint32_t                XT_Sup:1;
  uint32_t                UNUSED:30;
} CCX_IOMMU_FEATURE_INFO;

#define XAPIC_ID_MAX 0xFF
