/**
 * @file Ccx.h
 * @brief OpenSIL CCX IP initialization function declaration.
 *
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once
#include <SMU/SmuIp2Ip.h>
#pragma pack (push, 1)

#include <CCX/CcxClass-api.h>
#include "CCX/Common/AmdTable.h"
#include <CCX/Common/CcxApic.h>

/**********************************************************************************************************************
 * Declare macros here
 *
 */

#define CCXCLASS_MAJOR_REV 0
#define CCXCLASS_MINOR_REV 1
#define CCXCLASS_INSTANCE  0

/**
 * Layout of ApStartupVector:
 * +--------------------------------+ +->
 *
 *  AllowToLaunchNextThreadLocation   // APs increment this to indicate they are done,
 *  2 bytes (UINT16)                  //  then we launch the next.
 *
 * +--------------------------------+ +-> ApStartupVector + E
 *
 *  BSP GDTR
 *  10 bytes (UINT16 + UINT64)        // Data APs use to load their GDTs in the
 *  Depends if 32b or 64b             // ApStartupCode byte code.
 *
 * +--------------------------------+ +-> ApStartupVector + 4
 *
 *  Near Jump to ApStartupCode
 *  4 bytes                           //  The AP starts executing right here.
 *
 * +--------------------------------+ +-> ApStartupVector (got from Host Firmware directory)
 *
 *  C3 Value
 *  4 bytes (UINT32)                  //The BSP's C3 value is placed here so the APs can copy it.
 *
 * +--------------------------------+ +-> ApStartupVector - 0x8
 *
 *  GDT                               //BSP GDTR at ApStartupVector + 4 points here.
 *  BSP_GDT_SIZE                      //Contains the BSP GDT entries.
 *  0x48 should be enough             //9 x 8 byte entries.
 *
 * +--------------------------------+ +-> ApStartupVector - BSP_GDT_OFFSET
 *
 *  ApMtrrSyncList[]                  // The data here is used by the APs
 *  BSP_MSR_SIZE                      // to set their MSRs to be the same
 *  0x170 should be enough            //  as what the BSP has.
 *
 * +--------------------------------+ +-> ApStartupVector - BSP_MSR_OFFSET
 *
 *  Code for ApStartupCode            // The code AT ApStartupVector simply
 *  AP_STARTUP_CODE_SIZE              // jumps here, where the ApStartupCode
 *  0x80 should be enough             // byte code gets copied.
 *
 * +--------------------------------+ +-> ApStartupVector - AP_STARTUP_CODE_OFFSET
 */


#define  AP_STARTUP_CODE_SIZE   0x80
#define  BSP_MSR_SIZE           0x170
#define  BSP_GDT_SIZE           0x50
#define  AP_TEMP_BUFFER_SIZE    (BSP_GDT_SIZE + BSP_MSR_SIZE + AP_STARTUP_CODE_SIZE + 0x10) // 0x250

#define  AP_STARTUP_CODE_OFFSET (BSP_GDT_SIZE + BSP_MSR_SIZE + AP_STARTUP_CODE_SIZE) // 0x240
#define  BSP_MSR_OFFSET         (BSP_GDT_SIZE + BSP_MSR_SIZE) // 0x1C0
#define  BSP_GDT_OFFSET         (BSP_GDT_SIZE) // 0x50

// If below size is changed, please update the same definition in ApAsm32.nasm and ApAsm64.nasm
#define  AP_STACK_SIZE          0x200

#define CPU_LIST_TERMINAL       0xFFFFFFFFul

#define CCX_TRACEPOINT(MsgLevel, Message, ...)        \
  do {                \
    if (DEBUG_FILTER_CCX & SIL_DEBUG_MODULE_FILTER) {    \
      XUSL_TRACEPOINT(MsgLevel, Message, ##__VA_ARGS__);  \
        }\
  } while (0)




#define SIL_XAPIC_ID_MAX             0xFF  // moved from CcxClass-api.h (not needed by Host)

/**********************************************************************************************************************
 * variable declaration
 *
 */

typedef struct {
  uint32_t MsrAddr;     ///< Fixed-Sized MTRR address
  uint64_t MsrData;     ///< MTRR Settings
} AP_MTRR_SETTINGS;

/// AP MSR sync up
typedef struct {
  uint32_t MsrAddr;     ///< MSR address
  uint64_t MsrData;     ///< MSR Settings
  uint64_t MsrMask;     ///< MSR mask
} AP_MSR_SYNC;

/// GDT descriptor
typedef struct {
  uint16_t  Limit;        ///< Size
  uint64_t  Base;         ///< Pointer
} CCX_GDT_DESCRIPTOR;

typedef struct {
  uint32_t                   BspMsrLocation;                  ///< Do NOT change the offset of this variable,
                                                              // as offset to this element is used in ApAsm nasm file.
  uint32_t                   AllowToLaunchNextThreadLocation; ///< Do NOT change the offset of this variable
                                                              // as offset to this element is used in ApAsm nasm file.
  uint64_t                   ApStackBasePtr;                  ///< Do NOT change the offset of this variable
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
  uint64_t                   CacWeights[MAX_CAC_WEIGHT_NUM];
  const REGISTER_TABLE_AT_GIVEN_TP *CcxRegTableListAtGivenTP;
} AMD_CCX_AP_LAUNCH_GLOBAL_DATA;

/******************************************************************************
 * Declare Function prototypes
 *
 */

SIL_STATUS CcxClassSetInputBlk (void);
SIL_STATUS InitializeCcx (
  const REGISTER_TABLE_AT_GIVEN_TP  *CcxRegTableListAtGivenTP,
  uint32_t                          CcdDisMask,
  uint32_t                          DesiredCcdCount
  );
void CcxSetMca (void);
void CcxInitializeC6 (CCXCLASS_INPUT_BLK *CcxInputBlock);
void ApAsmCode (void);
void RegSettingBeforeLaunchingNextThread (void);
void ApEntryPointInC (
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
void CcxEnableSmee (bool AmdSmee);
SIL_STATUS CcxGetCacWeights (uint64_t *CacWeights);
void CcxSetCacWeights (uint64_t *CacWeights);
void CcxInitializeCpb (uint8_t AmdCpbMode);

void
SetupApStartupRegion (
  volatile AMD_CCX_AP_LAUNCH_GLOBAL_DATA *ApLaunchGlobalData,
  uint64_t                               *ApStartupVector,
  uint8_t                                *MemoryContentCopy,
  CCXCLASS_DATA_BLK                      *CcxDataBlock
  );

#pragma pack (pop)
