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

// DDR5 VDDIO Range
#define VOLT1_1_ENCODED_VAL             0
#define VOLT1_1_MILLIVOLTS 1100

///< Dram technology type
typedef enum {

  SilDdr5Technology = 8,  ///< DDR5 technology
  SilLpDdr5Technology = 9 ///< LPDDR5 technology
} SIL_TECHNOLOGY_TYPE;

/// Low voltage support
typedef enum {
  SIL_VOLT_INITIAL,                        ///< Initial value for VDDIO
  SIL_VOLT1_5,                             ///< 1.5 Volt
  SIL_VOLT1_35,                            ///< 1.35 Volt
  SIL_VOLT1_25,                            ///< 1.25 Volt
  SIL_VOLT1_2,                             ///< 1.2 Volt
  SIL_VOLT_DDR5_RANGE_START,               ///< Start of DDR5 Voltage Range
  SIL_VOLT1_1 = SIL_VOLT_DDR5_RANGE_START, ///<
  SIL_VOLT_TBD2,                           ///<
  SIL_VOLT_UNSUPPORTED = 0xFF              ///< No common voltage found
} SIL_DIMM_VOLTAGE;

/// UMA Mode
typedef enum {
  SIL_UMA_NONE = 0,              ///< UMA None
  SIL_UMA_SPECIFIED = 1,         ///< UMA Specified
  SIL_UMA_AUTO = 2,              ///< UMA Auto
  SIL_UMA_GAME_OPTIMIZED = 3     ///< UMA Game Optimized mode
} SIL_UMA_MODE;

/**
 * @brief Amd Memory UMA Info
 */
typedef struct _SIL_AMD_MEMORY_UMA_INFO {
  SIL_UMA_MODE UmaMode;    ///<  Uma Mode
                           ///<  0 = None
                           ///<  1 = Specified
                           ///<  2 = Auto
  uint32_t     UmaSize;    ///<  The size of shared graphics dram (16-bits)
                           ///<  NV_UMA_Size[31:0]=Addr[47:16]
  uint32_t     UmaBase;
} SIL_AMD_MEMORY_UMA_INFO;

/**
 * @brief Status union for boolean or value status
 */
typedef union {
  bool     Enabled;    ///< Status.Enabled - TRUE: Enabled.
  uint16_t Value;      ///< Status.Value - Configured value.
} MEM_CFG_STATUS;

/// VDDP_VDDR Voltage Info for Low Power DIMM
typedef struct _SIL_VDDP_VDDR_VOLTAGE {
  bool    IsValid;    ///< Indicates if data is valid
  uint8_t Voltage;    ///< VDDP VDDR Voltage Value
} SIL_VDDP_VDDR_VOLTAGE;

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
  SIL_DIMM_VOLTAGE         AmdMemoryVddIo;             ///< Memory Vddio
  SIL_VDDP_VDDR_VOLTAGE    AmdMemoryVddpVddr;          ///< Memory Vddp Vddr
  SIL_AMD_MEMORY_UMA_INFO  AmdGetUmaInfo;              ///< AMD UMA Info
  uint32_t                 DdrMaxRate;                 ///< DdrMaxRate
  uint32_t                 Reserved;
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
  SIL_MEM_CFG_INFO MbistTestEnable;               ///< MbistTestEnable
  SIL_MEM_CFG_INFO MbistAggressorEnable;          ///< MbistAggressorEnable
  SIL_MEM_CFG_INFO MbistPerBitSecondaryDieReport; ///< MbistPerBitSecondaryDieReport
  SIL_MEM_CFG_INFO DramTempControlledRefreshEn;   ///< DramTempControlledRefreshEn
  SIL_MEM_CFG_INFO UserTimingMode;                ///< UserTimingMode
  SIL_MEM_CFG_INFO UserTimingValue;               ///< UserTimingValue
  SIL_MEM_CFG_INFO MemBusFreqLimit;               ///< MemBusFreqLimit
  SIL_MEM_CFG_INFO EnablePowerDown;               ///< EnablePowerDown
  SIL_MEM_CFG_INFO DramDoubleRefreshRate;         ///< DramDoubleRefreshRate
  SIL_MEM_CFG_INFO PmuTrainMode;                  ///< PmuTrainMode
  SIL_MEM_CFG_INFO EccSymbolSize;                 ///< EccSymbolSize
  SIL_MEM_CFG_INFO UEccRetry;                     ///< UEccRetry
  SIL_MEM_CFG_INFO IgnoreSpdChecksum;             ///< IgnoreSpdChecksum
  SIL_MEM_CFG_INFO EnableBankGroupSwapAlt;        ///< EnableBankGroupSwapAlt
  SIL_MEM_CFG_INFO EnableBankGroupSwap;           ///< EnableBankGroupSwap
  SIL_MEM_CFG_INFO DdrRouteBalancedTee;           ///< DdrRouteBalancedTee
  SIL_MEM_CFG_INFO DimmPowerSource;               ///< DimmPowerSource
  SIL_MEM_CFG_INFO OdtsCmdThrotEn;                ///< OdtsCmdThrotEn
  SIL_MEM_CFG_INFO OdtsCmdThrotCyc;               ///< OdtsCmdThrotCyc
} AMD_MEMORY_SUMMARY;
