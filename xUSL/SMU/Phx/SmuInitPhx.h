/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file SmuInitPhx.h
 * @brief Prototypes for PHX SMU core specific transfer table functions
 *
 */

#pragma once

#include "SmuPhx.h"

///  to SMU Message Definitions for Phoenix
#define SMC_MSG_GetNameString_PHX                       0x4  ///< Get CPU name string
#define SMC_MSG_EnableSmuFeatures                       0x5  ///< Enable PMFW features
#define SMC_MSG_ReadCoreCacWeightRegister_PHX           0xA  ///< Read Core CAC weights
#define SMC_MSG_SetBiosDramAddr                         0xD ///< Inform PMFW of BIOS table DRAM address
#define SMC_MSG_SetToolsDramAddr                        0xE ///< Inform PMFW of tools DRAM address
#define SMC_MSG_TransferTableSmu2Dram                   0xF ///< Transfer PMFW tables from PMFW SRAM to DRAM
#define SMC_MSG_TransferTableDram2Smu                   0x10 ///< Transfer PMFW tables from DRAM to PMFW SRAM
#define SMC_MSG_DcBtc                                   0x13 ///< Run CPU bootup calibration
#define SMC_MSG_SetSustainedPowerLimit                  0x14 ///< Set sustained sower limit for STAPM controller in PMFW
#define SMC_MSG_SetFastPPTLimit                         0x15 ///< Set past PPT limit for PPT controller in PMFW
#define SMC_MSG_SetSlowPPTLimit                         0x16 ///< Set slow PPT limit for PPT controller in PMFW
#define SMC_MSG_SetStapmTimeConstant                    0x18 ///< Set STAPM time constant doe STAPM controller in PMFW
#define SMC_MSG_SetTjMax                                0x19 ///< Set TjMax in PMFW
#define SMC_MSG_SetVrmVddCurrentLimit                   0x1A ///< Set VDDCR current limit for TDC controller
#define SMC_MSG_SetVrmSocCurrentLimit                   0x1B ///< Set VDDCR_SOC current limit for TDC controller
#define SMC_MSG_SetVrmVddMaximumCurrentLimit            0x1C ///< Set VDDCR Max current limit for TDC controller
#define SMC_MSG_SetVrmSocMaximumCurrentLimit            0x1D ///< Set VDDCR_SOC Max current limit for TDC controller
#define SMC_MSG_SetSlowPPTLimitApuOnly                  0x23 ///< Set slow PPT limit for APU only
#define SMC_MSG_GetCoreBistPF_PHX                       0x3F ///< To get CCX memory build in test result
#define SMC_MSG_VDDCR_SOC_DcBtc                         0x41 ///< Add Description Here
#define SMC_MSG_SetZ8VidOffset                          0x45 ///< //CBS internal debug option
#define SMC_MSG_SetVddOffVid                            0x46 ///< Set VDD OFF VID
#define SMC_MSG_SetCorePsmMargin                        0x4B ///< Set PSM margin for a particular core
#define SMC_MSG_SetAllCorePsmMargin                     0x4C ///< Set PSM margin for all cores
#define SMC_MSG_FllBootTimeCalibration                  0x4F ///< Boot time calibration for CCX L3 FLL
#define SMC_MSG_SetS0i3PmeTurnOffDelay                  0x54 ///< Set S0i3 PME turn off delay
#define SMC_MSG_SetGfxclkOverdriveByFreqVid             0x5C ///< Set GFX CLK over clocking frequency and VDDCR VID
#define SMC_MSG_GetSustainedPowerAndThmLimit            0x5F ///< Get sustained power and thermal limit
#define SMC_MSG_SetFitLimitScalar                       0x63 ///< Set FIT Limit Scalar
#define SMC_MSG_SetGfxVidMargin                         0x64 ///< Force GFX vid margine. Input: signed vid (-30 to +30)
#define SMC_MSG_WriteSviRegister                        0x68 ///< Write SVI register
#define SMC_Message_Count_PHX                           0x69 ///< Total number of  messages

void
SmuFixupPlatformConfig (
  GNB_HANDLE           *GnbHandle,
  SMUCLASS_INPUT_BLK   *SmuInputBlock,
  SMU_PP_TABLE         *PPTable,
  uint32_t             PackageType
  );
