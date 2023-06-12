/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  RasInitBrh.h
 * @brief RAS definitions for silicon initialization code.
 *
 */

#pragma once

#include <xSIM.h>
#include <RAS/Common/Ras.h>

/*******************************************************************************
 * Declare macros here
 *
 */
#define UMC_SMN_INDEX(UMC, OFFSET) ((((uint32_t)UMC) << 20) + 0x50000 + OFFSET)

/**
 * @brief The maximum number of sockets that can be supported
 *
 * @details The maximum number of Breithorn processors currently supported by a platform is 2.
 */
#define SIL_RESERVED_1755                  (2)


/**
 * Turin - Number of IOD.
 *
 * Turin microprocessor contains multiple core/cache complex dies (CCD) plus an I/O die (IOD).
 */
#define DIE_PER_SOCKET          (1)
/**
 * Turin - The total number of UMC on Turin IOD
 *
 * Twelve Unified Memory Controllers (UMC), each supporting one
 *         DRAM channel
 */
#define UMC_PER_DIE             (12)
/**
 * Turin - The number of UMC channel on an UMC
 *
 * Twelve Unified Memory Controllers (UMC), each supporting one DRAM channel
 */
#define CHANNEL_PER_UMC         (1)
/**
 * @brief The total number of UMC channels on Turin IOD
 *
 * @details Twelve Unified Memory Controllers (UMC), each supporting one DRAM channel
 */
#define MAX_UMC_CHANNEL_PER_DIE (UMC_PER_DIE * CHANNEL_PER_UMC)

/**
 * MCA_IPID[McaType]
 *
 * @brief MCA TYPE of CPU CORE Block unique for BRH
 *
 */
#define LS_MCA_TYPE_BRH                   (0x0000)  ///< Load-Store Unit
#define LS_THR0_SMNADDR_BYTE1_BRH         0xA2

/*******************************************************************************
 * variable declaration
 *
 */

typedef struct {
  bool Present[SIL_RESERVED_1755][MAX_UMC_CHANNEL_PER_DIE * DIE_PER_SOCKET]; ///< Identifies if a DIMM is present
  ///< at particular channel for a given
  ///< socket
} SIL_DIMM_PRESENT_DATA;

typedef struct {
  SIL_DIMM_PRESENT_DATA Dimm;
} RAS_DATA_BLOCK_BRH;

#define SIL_RESERVED_1756 1

/*******************************************************************************
 * Declare Function prototypes
 *
 */
uint32_t
GetUmcHarvestFuseBrh (
  size_t    BusNumberBase
  );

SIL_STATUS
SetDimmPresenceBrh (
  RAS_DATA_BLOCK_BRH  *RasDataBlock
  );

/*******************************************************************************
 * SocLibBrh API functions
 */

void
ProgramCoreMcaConfigUmcBrh (
  bool  EnableFruText
  );

uint8_t
GetUmcPerDieBrh (void);

uint32_t
GetUmcHarvestFuseBrh (
  size_t    BusNumberBase
  );

SIL_STATUS
UpdateFruTextToUmcBrh (
  uint32_t              IohcBus,
  SIL_FRUTEXT_ENTRY *AmdFruTextSEntry
  );

void
CollectMcaErrorInfoBrh (
  SIL_RAS_MCA_ERROR_INFO_V2 *RasMcaErrorInfo
  );

void
UpdateMcaFruTextBrh (
  SIL_CPU_INFO      *RasCpuInfo
  );

uint8_t
GetCoreMcaSmnAddrByteBrh (
  uint16_t    McaType
  );
