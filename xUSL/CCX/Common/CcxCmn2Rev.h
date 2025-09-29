/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file CcxCmn2Rev.h
 * @brief Common-2-Rev core specific functions
 * @details  This files declares the structure for the common-2-Rev
 *  transfer table and the functions' prototypes therein.
 *
 */

#pragma once
#include "Ccx.h"

/* *************** Section for the Common-2-Rev Transfer Table *****************/

/* **********  P R O T O T Y P E S
 *   for the Internal functions used in the Common-2-Rev transfer block */

/**
 * SetMiscMsrs
 *
 * @brief   IP specific function to configure miscellaneous MSRs
 *
 * @details The IP specific implementation should handle configuration of
 *          miscellaneous MSRs that are not part of the common configuration.
 */
typedef void (*SET_MISC_MSRS) (
  CCXCLASS_INPUT_BLK *CcxInputBlock
  );

typedef APIC_MODE (*GET_AMD_APIC_MODE) (
  uint32_t NumberOfSockets,
  uint32_t NumberOfCcds,
  uint32_t NumberOfComplexes,
  uint32_t NumberOfCores,
  uint32_t NumberOfThreads
  );

typedef uint32_t (*CALC_LOCAL_APIC) (
  SIL_CONTEXT   *SilContext,
  uint32_t      Socket,
  uint32_t      Die,
  uint32_t      Ccd,
  uint32_t      Complex,
  uint32_t      Core,
  uint32_t      Thread
  );

typedef void (*CALC_RMP_TABLEBASE) (
  SIL_CONTEXT       *SilContext,
  uint64_t          *RmpTableSize,
  uint64_t          *RmpTableBase,
  uint64_t          *SnpMemSizeToCover,
  uint8_t           SplitRmpTableValue
  );

typedef volatile AP_MSR_SYNC * (*GET_AP_MSR_SYNC_LIST) (void);

typedef void (*STORE_DOWNCORE_CORE_COUNT) (
  SIL_CONTEXT       *SilContext,
  CCXCLASS_DATA_BLK *CcxConfigData
  );

typedef void (*CCX_PREFETCHER) (
  CCXCLASS_INPUT_BLK *CcxInputBlock
  );

typedef struct {
  CcxRevisions   Version;
} CCX_INT_API_HEADER;

typedef void (*CCX_INIT_PREFETCH_MODE) (
  CCXCLASS_INPUT_BLK *CcxInputBlock
  );

typedef void (*CCX_INITIALIZE_C6) (
  CCXCLASS_INPUT_BLK *CcxInputBlock
  );

typedef SIL_STATUS (*CCX_LAUNCH_THREAD) (
  SIL_CONTEXT    *SilContext,
  uint32_t       Socket,
  uint32_t       Die,
  uint32_t       LogicalCcd,
  uint32_t       LogicalComplex,
  uint32_t       LogicalCore,
  uint32_t       LogicalThread
  );

typedef void (*CCX_CACHE_INIT) (void);

typedef void (*SET_BRAND_STRING) (
  SIL_CONTEXT    *SilContext
  );

typedef bool (*S3_SUPPORTED)(
  void
  );

typedef bool (*GAME_MODE_SUPPORTED)(
  void
  );

typedef bool (*ONECCD_MODE_SUPPORTED)(
  void
  );

/* **********  T R A N S F E R   B L O C K  **********
 *   for the Internal Rev specific functions used in the Common code
 */

/** Internal Common-2-Rev Transfer Block
 *  @details This is the definition for the block of transfer structure
 *  listing functions used by the common code that must have content
 *  provided by the code specific to the revision(s) of the SoC.
 *
 *  Each revision of the IP declares an instance of this transfer block
 *  that contains pointers to functions that provide the data/action.
 *
 *  The Common code calls the specific code through these transfer blocks.
 *          Common  --> Rev Specific
 *  This table is private to the IP
 *  It is not available to other IPs, nor the Host
 */
typedef struct {
  CCX_INT_API_HEADER        Header;
  SET_MISC_MSRS             SetMiscMsrs;
  GET_AMD_APIC_MODE         GetAmdApicMode;
  CALC_LOCAL_APIC           CalcLocalApic;
  CALC_RMP_TABLEBASE        CalcRMPTableBase;
  GET_AP_MSR_SYNC_LIST      GetApMsrSyncList;
  STORE_DOWNCORE_CORE_COUNT StoreDowncoreCoreCount;
  CCX_PREFETCHER            CcxPrefetcher;
  CCX_INIT_PREFETCH_MODE    CcxInitializePrefetchMode;
  CCX_INITIALIZE_C6         CcxInitializeC6;
  CCX_LAUNCH_THREAD         CcxLaunchThread;
  CCX_CACHE_INIT            CcxCacheInit;
  SET_BRAND_STRING          SetBrandString;
  S3_SUPPORTED              IsS3Supported;
  GAME_MODE_SUPPORTED       IsGameModeSupported;
  ONECCD_MODE_SUPPORTED     IsOneCcdModeSupported;
} CCX_XFER_TABLE;
