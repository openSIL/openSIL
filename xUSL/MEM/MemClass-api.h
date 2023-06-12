/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved.  */
/**
 * @file MemClass-api.h
 * @brief openSIL-Host MEM IP interface.
 *
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

/// VDDP_VDDR Voltage Info for Low Power DIMM
typedef struct _SIL_VDDP_VDDR_VOLTAGE {
  bool    IsValid;    ///< Indicates if data is valid
  uint8_t Voltage;    ///< VDDP VDDR Voltage Value
} SIL_VDDP_VDDR_VOLTAGE;

/**
 * @brief Status union for boolean or value status
 */
typedef union {
  bool     Enabled;    ///< Status.Enabled - TRUE: Enabled.
  uint16_t Value;      ///< Status.Value - Configured value.
} MEM_CFG_STATUS;

/**
 * @brief Memory Config Info
 */
typedef struct _SIL_MEM_CFG_INFO {
  MEM_CFG_STATUS   Status;        ///< Status: Enable/disable or configured status value
  uint16_t         StatusCode;    ///< Status Code: MEM_STATUS_CODE_GENERAL_INFO
} SIL_MEM_CFG_INFO;

/**
 * @brief Amd Memory Summary
 */
typedef struct _AMD_MEMORY_SUMMARY {
  uint32_t                 Revision;                   ///< revision
  uint16_t                 AmdBottomIo;                ///< Bottom IO
  uint32_t                 AmdMemoryBelow4gb;          ///< Memory below 4G
  uint32_t                 AmdMemoryAbove4gb;          ///< Memory above 4G
  uint32_t                 AmdMemoryBelow1Tb;          ///< Memory below 1T
  uint32_t                 AmdTotalMemorySize;         ///< Total Memory Size
  uint32_t                 AmdMemoryFrequency;         ///< Memory Frequency
  SIL_VDDP_VDDR_VOLTAGE    AmdMemoryVddpVddr;          ///< Memory Vddp Vddr
  uint32_t                 DdrMaxRate;                 ///< DdrMaxRate
  uint16_t                 MemPORMaxSpeed;             ///< DDR POR speed in Mhz on current DIMM configuration
  uint16_t                 Reserved;
  uint32_t                 Reserved1;
  uint32_t                 Reserved2;
  uint8_t                  MaxSocketSupported;          ///< Indicates max. socket supported
  uint8_t                  MaxDiePerSocket;             ///< Indicates max. die per socket
  uint8_t                  MaxChannelPerDie;            ///< Indicates max. channel per die
  uint8_t                  MaxDimmPerChannel;           ///< Indicates max. dimm per channel

  /// Dynamic data
  uint32_t *DimmPresentMap;               ///< DimmPresentMap[MaxSocketSupported * MaxDiePerSocket]
                                          ///< Bit[1:0] - Dimm[1:0] of Channel0, .. , Bit[15:14]-Dimm[1:0] of Channel7
  SIL_MEM_CFG_INFO *ChipselIntlv;         ///< ChipselIntlv[MaxSocketSupported * MaxDiePerSocket * MaxChannelPerDie]
  SIL_MEM_CFG_INFO *DramEcc;              ///< DramEcc[MaxSocketSupported * MaxDiePerSocket]
  SIL_MEM_CFG_INFO *DramParity;           ///< DramParity[MaxSocketSupported * MaxDiePerSocket]
  SIL_MEM_CFG_INFO *AutoRefFineGranMode;  ///< AutoRefFineGranMode[MaxSocketSupported * MaxDiePerSocket]
  /// Fixed data
  /// Status reporting stuff
  SIL_MEM_CFG_INFO MbistTestEnable;             ///< MbistTestEnable
  SIL_MEM_CFG_INFO MbistAggressorEnable;        ///< MbistAggressorEnable
  SIL_MEM_CFG_INFO MbistPerBitSlaveDieReport;   ///< MbistPerBitSlaveDieReport
  SIL_MEM_CFG_INFO DramTempControlledRefreshEn; ///< DramTempControlledRefreshEn
  SIL_MEM_CFG_INFO UserTimingMode;              ///< UserTimingMode
  SIL_MEM_CFG_INFO UserTimingValue;             ///< UserTimingValue
  SIL_MEM_CFG_INFO MemBusFreqLimit;             ///< MemBusFreqLimit
  SIL_MEM_CFG_INFO EnablePowerDown;             ///< EnablePowerDown
  SIL_MEM_CFG_INFO DramDoubleRefreshRate;       ///< DramDoubleRefreshRate
  SIL_MEM_CFG_INFO PmuTrainMode;                ///< PmuTrainMode
  SIL_MEM_CFG_INFO EccSymbolSize;               ///< EccSymbolSize
  SIL_MEM_CFG_INFO UEccRetry;                   ///< UEccRetry
  SIL_MEM_CFG_INFO IgnoreSpdChecksum;           ///< IgnoreSpdChecksum
  SIL_MEM_CFG_INFO EnableBankGroupSwapAlt;      ///< EnableBankGroupSwapAlt
  SIL_MEM_CFG_INFO EnableBankGroupSwap;         ///< EnableBankGroupSwap
  SIL_MEM_CFG_INFO DdrRouteBalancedTee;         ///< DdrRouteBalancedTee
  SIL_MEM_CFG_INFO NvdimmPowerSource;           ///< NvdimmPowerSource
  SIL_MEM_CFG_INFO OdtsCmdThrotEn;              ///< OdtsCmdThrotEn
  SIL_MEM_CFG_INFO OdtsCmdThrotCyc;             ///< OdtsCmdThrotCyc
} AMD_MEMORY_SUMMARY;

/** @cond API_Doc
 *  @ingroup MODULES_IP
 *  @page Memory Memory IP api
 *
 *   The Mem
 *
 *   See the 'Files - @ref MemClass-api.h' section of this document for
 *   details.
 *
 *
 * @endcond
 */
/**
 * @cond FullCode_Doc
 * @ingroup MEM_group
 * @endcond
 *
 * Definition for the Mem class module's Input Block
 *
 * Note: This struct contains the vars shared between the
 *    Host API and the MEM IP module.
 *    Include into this file ONLY those items the Host needs to access
 *    and work with your IP.
 *
 */
