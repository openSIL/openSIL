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
 * Macros
 *
 */
#define CCX_DATA_BLOCK_INSTANCE      1 // Instance number of CCX_DATA_BLOCK
#define MAX_MSR_SYNC  64               // This value is arbitrarily chosen as the maximum number of sync MSRs supported
#define MAX_AP_GDT  16                 // This value is arbitrarily chosen

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

#define SIL_XAPIC_ID_MAX             0xFF

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
  uint8_t   AmdRedirectForReturnDis;
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
  uint32_t                   AllowToLaunchNextThreadLocation; ///< Do NOT change the offset of this variable
  // as offset to this element is used in ApAsm nasm file.
  uint32_t                   ApStackBasePtr;                  ///< Do NOT change the offset of this variable
  // as offset to this element is used in ApAsm nasm file.
  CCX_GDT_DESCRIPTOR         ApGdtDescriptor;                 ///< Do NOT change the offset of this variable
  // as offset to this element is used in ApAsm nasm file.
  uint8_t                    SleepType;
  uint32_t                   SizeOfApMtrr;
  volatile AP_MTRR_SETTINGS  *ApMtrrSyncList;
  volatile AP_MSR_SYNC       ApMsrSyncList[MAX_MSR_SYNC];
  uint64_t                   BspPatchLevel;
  uint64_t                   UcodePatchAddr;
  ENTRY_CRITERIA             ResetTableCriteria;
  CORE_WATCHDOG              CpuWdt;
  bool                       SetCacWeightsEnable;
  uint64_t                   CacWeights[MAX_CAC_WEIGHT_NUM];
  const REGISTER_TABLE_AT_GIVEN_TP *CcxRegTableListAtGivenTP;
  bool                       SvmEnable;
  bool                       SvmLock;
  uint64_t                   SilBaseAddress;
} AMD_CCX_AP_LAUNCH_GLOBAL_DATA;

typedef struct {
  uint32_t                   SystemDieNumber;
  uint64_t                   ApGdt[MAX_AP_GDT];
} CCX_DATA_BLOCK;

//
//The structure is copy from MdeModulePkg/Include/Guid/AcpiS3Context.h
//it needs sync with the structure in AcpiS3Context.h.
typedef struct {
  uint64_t    Signature;
  uint64_t    SmmS3ResumeEntryPoint;
  uint64_t    SmmS3StackBase;
  uint64_t    SmmS3StackSize;
  uint64_t    SmmS3Cr0;
  uint64_t    SmmS3Cr3;
  uint64_t    SmmS3Cr4;
  uint16_t    ReturnCs;
  uint64_t    ReturnEntryPoint;
  uint64_t    ReturnContext1;
  uint64_t    ReturnContext2;
  uint64_t    ReturnStackPointer;
  uint64_t    Smst;
} SMM_S3_RESUME_STATE;

typedef struct {
  uint32_t    BspStackSize;           ///< BSP Stack Size for resume
  uint32_t    ApStackSize;            ///< AP Stack Size for resume
  uint64_t    StackPtr;               ///< Point to the base of Stack
  uint64_t    RsmHeapPtr;             ///< Point to the base of Resume Heap
  uint32_t    HeapSize;               ///< Reserved Heap Size, Change the Order to avoid Warning 4366
  uint32_t    TempRegionSize;         ///< Reserved Temp Region Size
  uint64_t    TempRegionPtr;          ///< Point to the base of Temporary Region
} PSP_SMM_RSM_MEM_INFO;

typedef struct {
  SMM_S3_RESUME_STATE   SmmS3ResumeState;         ///< SmmS3ResumeState built by EDKII core
  PSP_SMM_RSM_MEM_INFO  PspSmmRsmMemInfo;         ///< SMM Resume Memory information specific for AMD
  uint64_t              CcxDataRegion;            ///< Data for CcxEarlyInit
  uint64_t              CcxEarlyInit;             ///< Point to CcxEarlyInit function
  uint64_t              ApStackTop;               ///< Pointer to AP Stack. This is updated when releasing APs.
  uint32_t              ApSyncFlag;               ///< Address to hold ApSyncFlag
  uint32_t              CapsuleUpdateSignature;   ///< Flag specify if it is on the capsule update S3 path
} PSP_SMM_HDR_DATA;

#pragma pack (pop)

/******************************************************************************
 * Function Prototypes
 *
 */

SIL_STATUS CcxClassSetInputBlk (SIL_CONTEXT  *SilContext);
SIL_STATUS InitializeCcx (
  SIL_CONTEXT                       *SilContext,
  const REGISTER_TABLE_AT_GIVEN_TP  *CcxRegTableListAtGivenTP
  );
void CcxSetMca (void);
void CcxInitializeC6 (CCXCLASS_INPUT_BLK *CcxInputBlock);
void ApAsmCode (void);
void RegSettingBeforeLaunchingNextThread (volatile AMD_CCX_AP_LAUNCH_GLOBAL_DATA *);
NASM_ABI void ApEntryPointInC (
  volatile AMD_CCX_AP_LAUNCH_GLOBAL_DATA *ApLaunchGlobalData);
void CcxSetMiscMsrs (
  SIL_CONTEXT        *SilContext,
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
  SIL_CONTEXT  *SilContext,
  CORE_WATCHDOG *CpuWdt
  );
SIL_STATUS CcxGetCacWeights (
  SIL_CONTEXT  *SilContext,
  uint64_t     *CacWeights
  );
void CcxSetCacWeights (uint64_t *CacWeights);
void CcxInitializeCpb (uint8_t AmdCpbMode);

SIL_STATUS
SetupApStartupRegion (
  volatile AMD_CCX_AP_LAUNCH_GLOBAL_DATA *ApLaunchGlobalData,
  void                                   **ApStartupBuffer,
  void                                   *ApStack,
  void                                   *MemoryContentCopy,
  uint32_t                               *MemoryContentCopySize,
  CCXCLASS_DATA_BLK                      *CcxDataBlock,
  CCX_DATA_BLOCK                         *CcxDataBlk
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
