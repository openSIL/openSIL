/**
 * @file  NbioPcie.h
 * @brief OpenSIL NBIO Pcie initialization prototypes
 *
 */
/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <xSIM.h>
#include <SilPcie.h>

/**********************************************************************************************************************
 * Declare macros here
 *
 */

/**********************************************************************************************************************
 * Declare common variables here
 *
 */

/**
 * Declare function prototypes here
 *
 */
SIL_STATUS PcieConfigurationInit (void);
void PcieFixupComplexDefaults(PCIe_PLATFORM_CONFIG *Pcie);
void PcieFixupPlatformConfig(PCIe_PLATFORM_CONFIG *Pcie);
void PcieEnumerateAndHarvestWrappers (PCIe_PLATFORM_CONFIG *Pcie);
SIL_STATUS PcieComplexHidePcieCore (GNB_HANDLE *GnbHandle, uint32_t  WrapperNum);
