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

// @todo Warning 4100 'unreferenced formal parameter' shows on underdeveloped functions where input  argument is not
// used in the function. Remove the following #pragma when the code is mature and all arguments in all NBIO functions
// are effectively used.
#pragma warning( disable : 4100 )

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
