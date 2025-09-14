/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file Ccx.h
 * @brief OpenSIL CCX IP initialization function declaration.
 *
 */

#pragma once
#include <SilCommon.h>
#include <SMU/SmuIp2Ip.h>

#include <CCX/CcxClass-api.h>
#include "CCX/Common/AmdTable.h"
#include <CCX/Common/CcxApic.h>


/**********************************************************************************************************************
 * Declare macros here
 *
 */

#define CR3_SAVE_SIZE 8

#define  AP_TEMP_BUFFER_SIZE  1024


// !!! If below size is changed, please update the same definition in x32/ApAsm.nasm and x64/ApAsm64.nasm
#define  AP_STACK_SIZE          0x400

#define CPU_LIST_TERMINAL       0xFFFFFFFFul

#define CCX_TRACEPOINT(MsgLevel, Message, ...)        \
        do {                \
          if (DEBUG_FILTER_CCX & SIL_DEBUG_MODULE_FILTER) {    \
            XUSL_TRACEPOINT(MsgLevel, Message, ## __VA_ARGS__);  \
          } \
        } while (0)

#define SIL_XAPIC_ID_MAX             0xFF  // moved from CcxClass-api.h (not needed by Host)

/**********************************************************************************************************************
 * variable declaration
 *
 */

typedef struct {
  uint32_t  TopOfApRegion;
  uint32_t  PageTable;            // Should always be at ApStartup Vector - 16
  uint32_t  GdtTable;
  uint32_t  ApMttrSyncList;
  uint32_t  ApStartupCodeBase;
} AP_STARTUP_MEMORY_MAP;

/// AP MSR sync up
typedef struct {
  uint32_t MsrAddr;     ///< MSR address
  uint64_t MsrData;     ///< MSR Settings
  uint64_t MsrMask;     ///< MSR mask
} AP_MSR_SYNC;

///  The possible hardware prefetch mode settings.
typedef enum  {
  HARDWARE_PREFETCHER_AUTO,     ///< Use the recommended setting for the processor.
  //In most cases, the recommended setting is enabled.
  DISABLE_HW_PREFETCHER_TRAINING_ON_SOFTWARE_PREFETCHES,  ///< Use the recommended setting for the hardware
  // prefetcher, but disable training on software
  // prefetches.
  DISABLE_L1_PREFETCHER,        ///< Use the recommended settings for the hardware prefetcher,
  //but disable L1 prefetching and above.
  DISABLE_L2_STRIDE_PREFETCHER, ///< Use the recommended settings for the hardware prefetcher,
  //but disable the L2 stride prefetcher and above
  DISABLE_HARDWARE_PREFETCH,    ///< Disable hardware prefetching.
  MAX_HARDWARE_PREFETCH_MODE    ///< Not a hardware prefetch mode, use for limit checking.
} HARDWARE_PREFETCH_MODE;

///  The possible software prefetch mode settings.
typedef enum  {
  SOFTWARE_PREFETCHES_AUTO,    ///< Use the recommended setting for the processor.
  //In most cases, the recommended setting is enabled.
  DISABLE_SOFTWARE_PREFETCHES, ///< Disable software prefetches
  //(convert software prefetch instructions to NOP).
  MAX_SOFTWARE_PREFETCH_MODE   ///< Not a software prefetch mode, use for limit checking.
} SOFTWARE_PREFETCH_MODE;

/// Advanced performance tunings, prefetchers.
/// These settings provide for performance tuning to optimize for specific workloads.
typedef struct {
  HARDWARE_PREFETCH_MODE  HardwarePrefetchMode; ///< This value provides for
                                                ///< advanced performance tuning
                                                ///< by controlling the hardware
                                                ///< prefetcher setting.
  SOFTWARE_PREFETCH_MODE  SoftwarePrefetchMode; ///< This value provides for
                                                ///< advanced performance tuning
                                                ///< by controlling the software
                                                ///< prefetch instructions.
} CCX_PREFETCH_MODE;

/// Core Watchdog
typedef struct {
  bool      CpuWdtEn;
  uint16_t  CpuWdtTimeOut;
  uint8_t   CpuWdTmrCfgSeverity;
  uint8_t   AmdReserved1;
} CORE_WATCHDOG;


#pragma pack (push, 1)

typedef struct {
  uint32_t MsrAddr;     ///< Fixed-Sized MTRR address
  uint64_t MsrData;     ///< MTRR Settings
} AP_MTRR_SETTINGS;

// CCX_GDT32_DESCRIPTOR and CCX_GDT_DESCRIPTOR must be packed
// as this are saved by hardware and must not have extra alignment bytes.

/// GDT 32-bit descriptor
typedef struct {
  uint16_t    Limit;
  uint32_t    Base;   //Before switching to 64-bit mode, 32-bit base is used by lgdt.
} CCX_GDT32_DESCRIPTOR;

/// GDT descriptor
typedef struct {
  uint16_t   Limit;        ///< Size
  uintptr_t  Base;         ///< Pointer
} CCX_GDT_DESCRIPTOR;

// The first few fields of AMD_CCX_AP_LAUNCH_GLOBAL_DATA data is shared with ApAsm.nasm.
// Thus, pack so no alignment bytes will be the first few fields.

typedef struct {
  uint32_t                   BspMsrLocation;                  ///< Do NOT change the offset of this variable,
  // as offset to this element is used in ApAsm nasm file.
  volatile uint32_t          ApSyncCount;                      ///< Do NOT change the offset of this variable
  // as offset to this element is used in ApAsm nasm file.
  volatile uint32_t          AllowToLaunchNextThreadLocation; ///< Do NOT change the offset of this variable
  // as offset to this element is used in ApAsm nasm file.
  uint32_t                   ApStackBasePtr;                  ///< Do NOT change the offset of this variable
  // as offset to this element is used in ApAsm nasm file.
  CCX_GDT_DESCRIPTOR         ApGdtDescriptor;                 ///< Do NOT change the offset of this variable
  // as offset to this element is used in ApAsm nasm file.
  uint8_t                    SleepType;
  uint32_t                   SizeOfApMtrr;
  volatile AP_MTRR_SETTINGS  *ApMtrrSyncList;
  volatile AP_MSR_SYNC       *ApMsrSyncList;
  uint64_t                   BspPatchLevel;
  uint64_t                   UcodePatchAddr;
  ENTRY_CRITERIA             ResetTableCriteria;
  CORE_WATCHDOG              CpuWdt;
  bool                       SetCacWeightsEnable;
  uint64_t                   CacWeights[MAX_CAC_WEIGHT_NUM];
  const REGISTER_TABLE_AT_GIVEN_TP *CcxRegTableListAtGivenTP;
  bool                       SvmEnable;
  bool                       SvmLock;
} AMD_CCX_AP_LAUNCH_GLOBAL_DATA;


#pragma pack (pop)

/******************************************************************************
 * Declare Function prototypes
 *
 */

SIL_STATUS CcxClassSetInputBlk (void);
SIL_STATUS InitializeCcx (
  const REGISTER_TABLE_AT_GIVEN_TP  *CcxRegTableListAtGivenTP
  );
void CcxSetMca (void);
void CcxInitializeC6 (CCXCLASS_INPUT_BLK *CcxInputBlock);
void ApAsmCode (void);
NASM_ABI void RegSettingBeforeLaunchingNextThread (
  volatile AMD_CCX_AP_LAUNCH_GLOBAL_DATA *ApLaunchGlobalData);
NASM_ABI void ApEntryPointInC (
  volatile AMD_CCX_AP_LAUNCH_GLOBAL_DATA *ApLaunchGlobalData);
void CcxSetMiscMsrs (
  CCXCLASS_INPUT_BLK *CcxInputBlock
  );
void CcxSyncMiscMsrs (
  volatile AMD_CCX_AP_LAUNCH_GLOBAL_DATA *ApLaunchGlobalData);
void UpdateApMtrrSettings (
  volatile AP_MTRR_SETTINGS *ApMtrrSettingsList,
  CCXCLASS_INPUT_BLK *CcxInputBlock
  );
void CcxEnableSmeeHmkee (bool SmeeEnable, bool HmkeeEnable);
void CcxInitSvm (
  bool SvmEnable,
  bool SvmLock
  );
void CcxEnableWdt (
  CORE_WATCHDOG *CpuWdt
  );
SIL_STATUS CcxGetCacWeights (uint64_t *CacWeights);
void CcxSetCacWeights (uint64_t *CacWeights);
void CcxInitializeCpb (uint8_t AmdCpbMode);

SIL_STATUS
SetupApStartupRegion (
  volatile AMD_CCX_AP_LAUNCH_GLOBAL_DATA *ApLaunchGlobalData,
  void                                   **ApStartupBuffer,
  void                                   *ApStack,
  void                                   *MemoryContentCopy,
  uint32_t                               *MemoryContentCopySize,
  CCXCLASS_DATA_BLK                      *CcxDataBlock
  );

void
RestoreResetVector (
  volatile AMD_CCX_AP_LAUNCH_GLOBAL_DATA *ApLaunchGlobalData,
  uint32_t TotalApCoresLaunched,
  void     *ApStartupBuffer,
  void     *MemoryContentCopy,
  uint32_t  MemoryContentCopySize
  );


void ApStartupFixups (AP_STARTUP_MEMORY_MAP *ApStartupMemoryMap, void *ApLaunchGlobalData);

typedef void (ASSEMBLY_PATCH_LABEL) (
  void
  );

void
PatchAssemblyInstruction (
  ASSEMBLY_PATCH_LABEL  *InstructionEnd,
  uint64_t               PatchValue,
  uintptr_t              ValueSize
  );
