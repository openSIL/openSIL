/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file xSIM.c
 * @brief Common Host API functions
 */
/**
 * @defgroup xSIM_group xSIMgroup:Silicon Initialization Module
 * This module is responsible for the very early (basic) Si initialization
 *   coming off of reset.
 */

#include <SilCommon.h>
#include <string.h>
#include <xSIM.h>
#include <SilSocLogicalId.h>
#include "IpHandler.h"

// Extern global const variables for SoC list, defined in SocList.c
extern const SOC_IP_TABLE *mPlatformSocsTp1[];
extern const SOC_IP_TABLE *mPlatformSocsTp2[];
extern const SOC_IP_TABLE *mPlatformSocsTp3[];

/** Rounds the Data up to the RoundTo value
 */
static
size_t
RoundUp (
  size_t  Data,
  size_t  RoundTo
  )
{
  return Data + RoundTo - (Data % RoundTo);
}

/**
 * Locate the Information record for the active SoC for timepoint 1
 *
 * There may be more than one SoC supported in this build. This function
 * determines what SoC is present in the system and locates the
 * appropriate IP list corresponding to that SoC.
 * @param none
 * @returns pointer to the beginning of the SoC IP table, IP block list.
 */
static const
IP_RECORD
*GetActiveSocIpListTp1 ( void )
{
  const IP_RECORD    *IpRecord = NULL;
  SOC_LOGICAL_ID     LogicalId;
  uint8_t            Index;

  Index = 0;
  if (GetSocLogicalIdOnCurrentCore(&LogicalId) == SilPass) {
    while (mPlatformSocsTp1[Index] != NULL) {
      if (LogicalId.Family == mPlatformSocsTp1[Index]->SocLogicalId) {
        IpRecord = mPlatformSocsTp1[Index]->IpList;
        break;
      }
      Index++;
    }
  }

  XSIM_TRACEPOINT(SIL_TRACE_INFO, "IpRecord: %x\n", IpRecord);
  assert(IpRecord != NULL);

  return IpRecord;
}

/**
 * Locate the Information record for the active SoC for timepoint 2
 *
 * There may be more than one SoC supported in this build. This function
 * determines what SoC is present in the system and locates the
 * appropriate IP list corresponding to that SoC.
 * @param none
 * @returns pointer to the beginning of the SoC IP table, IP block list.
 */
static const
IP_RECORD
*GetActiveSocIpListTp2 ( void )
{
  const IP_RECORD    *IpRecord = NULL;
  SOC_LOGICAL_ID     LogicalId;
  uint8_t            Index;

  Index = 0;
  if (GetSocLogicalIdOnCurrentCore(&LogicalId) == SilPass) {
    while (mPlatformSocsTp2[Index] != NULL) {
      if (LogicalId.Family == mPlatformSocsTp2[Index]->SocLogicalId) {
        IpRecord = mPlatformSocsTp2[Index]->IpList;
        break;
      }
      Index++;
    }
  }

  XSIM_TRACEPOINT(SIL_TRACE_INFO, "IpRecord: %x\n", IpRecord);
  assert(IpRecord != NULL);

  return IpRecord;
}

/**
 * Locate the Information record for the active SoC for timepoint 3
 *
 * There may be more than one SoC supported in this build. This function
 * determines what SoC is present in the system and locates the
 * appropriate IP list corresponding to that SoC.
 * @param none
 * @returns pointer to the beginning of the SoC IP table, IP block list.
 */
static const
IP_RECORD
*GetActiveSocIpListTp3 ( void )
{
  const IP_RECORD    *IpRecord = NULL;
  SOC_LOGICAL_ID     LogicalId;
  uint8_t            Index;

  Index = 0;
  if (GetSocLogicalIdOnCurrentCore(&LogicalId) == SilPass) {
    while (mPlatformSocsTp3[Index] != NULL) {
      if (LogicalId.Family == mPlatformSocsTp3[Index]->SocLogicalId) {
        IpRecord = mPlatformSocsTp3[Index]->IpList;
        break;
      }
      Index++;
    }
  }

  XSIM_TRACEPOINT(SIL_TRACE_INFO, "IpRecord: %x\n", IpRecord);
  assert(IpRecord != NULL);

  return IpRecord;
}

/*********************************************************************
 * API Functions
 *********************************************************************/
/* Determine size of memory needed
 * @details This is a host API function.  The prototype, text
 *          description and Doxygen text are in in xSim-api.h
 *
 */
size_t
xSimQueryMemoryRequirements (void)
{
  const IP_RECORD         *LclIpRecord;
  size_t                  RequestTotal = 0;

  XSIM_TRACEPOINT(SIL_TRACE_ENTRY, "QueryMem.\n");

  // First apply the needs of the xSIM core
  RequestTotal = sizeof (SIL_BLOCK_VARIABLES);

  //Now add the data needs of each IP
  LclIpRecord = GetActiveSocIpListTp1 ();
  if (LclIpRecord == NULL) {
    assert(LclIpRecord != NULL);
    XSIM_TRACEPOINT(SIL_TRACE_ERROR, "Unable to get active SoC Ip List.\n");
    RequestTotal = 0;
    return RequestTotal;
  }
  while (LclIpRecord->IpID < SilId_ListEnd) {
    XSIM_TRACEPOINT(SIL_TRACE_INFO,
      "QueryMem:IpRecord at: 0x%x \n",
      LclIpRecord
      );
    if (LclIpRecord->BlkRequestSize > 0) {
      RequestTotal += RoundUp(sizeof (SIL_INFO_BLOCK_HEADER) + LclIpRecord->BlkRequestSize,
        sizeof (uint32_t)
        );
    }
    LclIpRecord++;
  }

  //Finally, round up to a convenient boundary (2K)
  RequestTotal = RoundUp(RequestTotal, 2 * KILOBYTE);

  XSIM_TRACEPOINT(SIL_TRACE_INFO,
    "QueryMem:ReqTotal at: 0x%x \n",
    RequestTotal
    );
  return RequestTotal;
}

/**
 * xSimClearIpApis
 *
 * @brief   Clear Ip2Ip API and Cmn2Rev xfer entries for all IPs
 *
 * @param   SilContext   A context structure through which host firmware defined data
 *                       can be passed to openSIL. The host firmware is responsible
 *                       for initializing the SIL_CONTEXT structure.
 * @param   LclIpRecord  Input pointer to the IP record list.
 *
 * @return  N/A
 *
 */
static
void
xSimClearIpApis (
  SIL_CONTEXT  *SilContext
  )
{
  SIL_BLOCK_VARIABLES *SilVars;
  SIL_DATA_BLOCK_ID    IpID = 0;

  SilVars = (SIL_BLOCK_VARIABLES *)(uintptr_t)SilContext->SilMemBaseAddress;

  while (IpID < SilId_ListEnd) {
    SilVars->Ip2IpApi[IpID] = (uintptr_t) NULL;
    SilVars->Common2RevXferTable[IpID] = (uintptr_t) NULL;
    IpID++;  // move to next record
  }

}

/**
 * xSimInitializeIpApis
 *
 * @brief   Call all IP ApiInit functions to initialize IP-to-IP and IP Internal APIs
 *
 * @param   SilContext   A context structure through which host firmware defined data
 *                       can be passed to openSIL. The host firmware is responsible
 *                       for initializing the SIL_CONTEXT structure.
 * @param   LclIpRecord  Input pointer to the IP record list.
 *
 * @return  SIL_STATUS
 *
 */
static
SIL_STATUS
xSimInitializeIpApis (
  SIL_CONTEXT         *SilContext,
  const IP_RECORD     *LclIpRecord
  )
{
  SIL_STATUS          LclStatus;

  LclStatus = SilPass;

  // Clear IP APIs before initializing them so that entries not meant to be available
  // during a certain timepoint cannot be accessed
  xSimClearIpApis(SilContext);

  // This needs to be initialized before any IP initialization.
  while (LclIpRecord->IpID < SilId_ListEnd) {
    if (LclIpRecord->ApiInit != NULL) {
      LclStatus = LclIpRecord->ApiInit(SilContext);
      if (LclStatus != SilPass) {
        XSIM_TRACEPOINT(SIL_TRACE_ERROR,
          "Api init failed for Ip ID: %d\n",
          LclIpRecord->IpID
          );
        return LclStatus;
      }
    }
    LclIpRecord++;  // move to next record
  }

  return LclStatus;
}

/**
 * xSimAssignMemoryTp1
 *
 * @brief     Assign Memory to sub-modules and initialize internal APIs for
 *            openSIL IPs.
 *
 * @details This is a host API function.  The prototype, text
 *          description and Doxygen text are in in xSim-api.h
 *
 * @param    SilContext     A context structure through which host firmware defined data
 *                          can be passed to openSIL. The host firmware is responsible
 *                          for initializing the SIL_CONTEXT structure.
 * @param     MemorySize    The size of the block allocated at BaseAddress
 *
 * @returns   SIL_STATUS
 *
 */
SIL_STATUS
xSimAssignMemoryTp1 (
  SIL_CONTEXT     *SilContext,
  size_t           MemorySize
  )
{
  SIL_BLOCK_VARIABLES *LclVarsPtr;    ///< pointer to global Vars
  const IP_RECORD     *LclIpRecord;   ///< pointer to IP Record being scanned
  const IP_RECORD     *IpRecordHead;  ///< pointer to IP record list HEAD
  size_t              LclStatus;      ///< collects status from calls made

  IpRecordHead = GetActiveSocIpListTp1 ();
  if (IpRecordHead == NULL) {
    assert(IpRecordHead != NULL);
    XSIM_TRACEPOINT(SIL_TRACE_ERROR, "Unable to get active SoC Ip List.\n");
    return SilNotFound;
  }

  LclIpRecord = IpRecordHead;

  // Fill the SIL private vars block

  XSIM_TRACEPOINT(SIL_TRACE_INFO,
    "SIL AssignMemBlk1:gVars at: 0x%x \n",
    SilContext->SilMemBaseAddress
    );
  LclVarsPtr = (SIL_BLOCK_VARIABLES *)(uintptr_t)SilContext->SilMemBaseAddress;
  LclVarsPtr->HostBlockSize = (uint32_t) MemorySize;
  LclVarsPtr->FreeSpaceOffset = sizeof (SIL_BLOCK_VARIABLES);
  LclVarsPtr->FreeSpaceLeft = (uint32_t) (MemorySize -
    sizeof (SIL_BLOCK_VARIABLES));

  // Initialize IP APIs specified in the IP list. This is required before any IP entrypoint is called.
  LclStatus = xSimInitializeIpApis(SilContext, LclIpRecord);
  if (LclStatus != SilPass) {
    return LclStatus;
  }

  XSIM_TRACEPOINT(SIL_TRACE_INFO,
    "SilMemoryBase: 0x%x, FreeSpaceOffset: 0x%x, FreeSpaceLeft = 0x%x\n",
    LclVarsPtr,
    LclVarsPtr->FreeSpaceOffset,
    LclVarsPtr->FreeSpaceLeft
    );
  // Loop through the IP records array, calling each IPblock to set their
  // Input blocks with defaults. IP block uses calls to SilCreateBlock() to
  // establish its Input block, Private block(s) and Output block(s).
  // IP code must fill its Input block with default values.

  LclStatus = SilPass;

  for (LclIpRecord = IpRecordHead; LclIpRecord->IpID < SilId_ListEnd; LclIpRecord++) {
    XSIM_TRACEPOINT(SIL_TRACE_INFO,
      "SIL Assign:IpRcd %x: %x, %x, %x, %x\n",
      LclIpRecord,
      LclIpRecord->IpID,
      LclIpRecord->BlkRequestSize,
      LclIpRecord->SetInput,
      LclIpRecord->Initialize
      );

    if (LclIpRecord->BlkRequestSize == 0) continue;

    /**
     * Check if space is available
     * LclVarsPtr->FreeSpaceLeft is updated during SilCreateInfoBlock
     * called from IP's SetInput functions
     */
    if (LclIpRecord->BlkRequestSize > LclVarsPtr->FreeSpaceLeft)
      return SilOutOfResources;

    if (LclIpRecord->SetInput == NULL) {
      XSIM_TRACEPOINT(SIL_TRACE_INFO,
        "SIL SetInput function is not reported for IP#0x%x fails.\n",
        LclIpRecord->IpID
        );
      continue;
    }

    LclStatus = LclIpRecord->SetInput(SilContext);
    if (LclStatus != SilPass) {
      XSIM_TRACEPOINT(SIL_TRACE_ERROR,
        "SIL SetInput function for IP#0x%x fails.\n",
        LclIpRecord->IpID
        );
      return LclStatus;
    }
  }
  return LclStatus;
}

/**
 * xSimAssignMemoryTp2
 *
 * @brief     Assign Memory to sub-modules and initialize internal APIs for
 *            openSIL IPs.
 *
 * @details This is a host API function.  The prototype, text
 *          description and Doxygen text are in in xSim-api.h
 *
 * @param     SilContext    A context structure through which host firmware defined data
 *                          can be passed to openSIL. The host firmware is responsible
 *                          for initializing the SIL_CONTEXT structure.
 * @param     MemorySize    The size of the block allocated at BaseAddress
 *
 * @returns   SIL_STATUS
 *
 */
SIL_STATUS
xSimAssignMemoryTp2 (
  SIL_CONTEXT     *SilContext,
  size_t           MemorySize
  )
{
  const IP_RECORD     *LclIpRecord;   ///< pointer to IP Record being scanned
  size_t              LclStatus;      ///< collects status from calls made
  UNUSED(MemorySize);

  LclIpRecord = GetActiveSocIpListTp2 ();
  if (LclIpRecord == NULL) {
    assert(LclIpRecord != NULL);
    XSIM_TRACEPOINT(SIL_TRACE_ERROR, "Unable to get active SoC Ip List.\n");
    return SilNotFound;
  }

  // Initialize IP APIs specified in the IP list. This is required before any IP entrypoint is called.
  LclStatus = xSimInitializeIpApis(SilContext, LclIpRecord);
  if (LclStatus != SilPass) {
    return LclStatus;
  }

  // For secondary time points, openSIL should not reinitialize its data.
  XSIM_TRACEPOINT(SIL_TRACE_INFO, "Assign new SIL base: 0x%x\n", SilContext->SilMemBaseAddress);
  return SilPass;
}

/**
 * xSimAssignMemoryTp3
 *
 * @brief     Assign Memory to sub-modules and initialize internal APIs for
 *            openSIL IPs.
 *
 * @details This is a host API function.  The prototype, text
 *          description and Doxygen text are in in xSim-api.h
 *
 * @param     SilContext    A context structure through which host firmware defined data
 *                          can be passed to openSIL. The host firmware is responsible
 *                          for initializing the SIL_CONTEXT structure.
 * @param     MemorySize    The size of the block allocated at BaseAddress
 *
 * @returns   SIL_STATUS
 *
 */
SIL_STATUS
xSimAssignMemoryTp3 (
  SIL_CONTEXT     *SilContext,
  size_t           MemorySize
  )
{
  const IP_RECORD     *LclIpRecord;   ///< pointer to IP Record being scanned
  size_t              LclStatus;      ///< collects status from calls made
  UNUSED(MemorySize);

  LclIpRecord = GetActiveSocIpListTp3 ();
  if (LclIpRecord == NULL) {
    assert(LclIpRecord != NULL);
    XSIM_TRACEPOINT(SIL_TRACE_ERROR, "Unable to get active SoC Ip List.\n");
    return SilNotFound;
  }

  // Initialize IP APIs specified in the IP list. This is required before any IP entrypoint is called.
  LclStatus = xSimInitializeIpApis(SilContext, LclIpRecord);
  if (LclStatus != SilPass) {
    return LclStatus;
  }

  // For secondary time points, openSIL should not reinitialize its data.
  XSIM_TRACEPOINT(SIL_TRACE_INFO, "Assign new SIL base: 0x%x\n", SilContext->SilMemBaseAddress);
  return SilPass;
}

/**
 * SilCreateInfoBlock
 * @brief Create an Info Block
 *
 * This internal xSIM routine is called by the IPs to assign some of the
 * Host memory to the IP for use as an Input block, private block or
 * output block. Each block must have an unique identifier in the SIL ID
 * list.
 *
 * @param SilContext A context structure through which host firmware defined data
 *                   can be passed to openSIL. The host firmware is responsible
 *                   for initializing the SIL_CONTEXT structure.
 * @param BlockTag - IP block unique identifier
 * @param BlockSize - size of the requested block
 * @param Block_Instance - Instance of block using this ID
 * @param Block_MajorRev - Revision of the IP data structure used
 * @param Block_MinorRev  - Revision (minor) of the IP data struct used.
 * @return pointer to the IP data segment of the assigned memory space.
 * @return NULL if there is an error
 *
 */
void *
SilCreateInfoBlock (
  SIL_CONTEXT     *SilContext,
  uint32_t        BlockTag,
  size_t          BlockSize,
  uint16_t        Block_Instance,
  uint8_t         Block_MajorRev,
  uint8_t         Block_MinorRev
  )
{
  SIL_INFO_BLOCK_HEADER *Header = NULL;
  SIL_BLOCK_VARIABLES   *LclVarsPtr;
  size_t                RequestSize;
  void                  *StructurePtr;

  StructurePtr = xUslFindStructure(SilContext, BlockTag, Block_Instance);

  if (StructurePtr != NULL) {
    XSIM_TRACEPOINT(SIL_TRACE_ERROR, "The requested info block already exists.\n");
    assert(StructurePtr == NULL);
    return StructurePtr;
  }

  if (BlockTag >= (uint32_t)SilId_ListEnd) {
    XSIM_TRACEPOINT(SIL_TRACE_ERROR, "The specified BlockTag ID is invalid.\n");
    return NULL;
  }

  LclVarsPtr = (SIL_BLOCK_VARIABLES *)(uintptr_t)SilContext->SilMemBaseAddress;

  if (LclVarsPtr == NULL) { // SilMemBaseAddress is not properly configured
    assert(LclVarsPtr != NULL);
    return NULL;
  }

  // calculate the amount of memory, rounded up to DWORD boundary
  RequestSize = RoundUp(sizeof (SIL_INFO_BLOCK_HEADER) + BlockSize,
    sizeof (uint32_t)
    );

  Header = (SIL_INFO_BLOCK_HEADER *)((uint8_t *)LclVarsPtr +
    LclVarsPtr->FreeSpaceOffset);
  XSIM_TRACEPOINT(SIL_TRACE_INFO,
    "SIL CreateBlk at: 0x%x [0x%x] \n",
    Header,
    RequestSize
    );
  if ((LclVarsPtr->FreeSpaceLeft) <= RequestSize) {
    return NULL;
  }

  LclVarsPtr->FreeSpaceOffset += (uint32_t) RequestSize;
  LclVarsPtr->FreeSpaceLeft -= (uint32_t) RequestSize;
  XSIM_TRACEPOINT(SIL_TRACE_INFO,
    "SilMemoryBase: 0x%x, FreeSpaceOffset: 0x%x, FreeSpaceLeft = 0x%x\n",
    LclVarsPtr,
    LclVarsPtr->FreeSpaceOffset,
    LclVarsPtr->FreeSpaceLeft
    );
  // Fill the Info Block
  memset(Header, 0, RequestSize);
  Header->Id = BlockTag;
  Header->InfoBlockDataSize = (uint32_t)RequestSize;
  Header->Instance = Block_Instance;
  Header->RevMajor = Block_MajorRev;
  Header->RevMinor = Block_MinorRev;
  return Header->InfoBlockData;
}

/*
 * SilFindStructure
 * @details This is a host API function.  The prototype, text
 *          description and Doxygen text are in in xSim-api.h
 *
 *
 * @param   SilContext   A context structure through which host firmware defined data
 *                       can be passed to openSIL. The host firmware is responsible
 *                       for initializing the SIL_CONTEXT structure.
 */
void *
SilFindStructure (
  SIL_CONTEXT       *SilContext,
  SIL_DATA_BLOCK_ID structureID,
  uint16_t          InstanceNum
  )
{
  return xUslFindStructure(SilContext, structureID, InstanceNum);
};

/**
 * SetDeferredResetType
 *
 * @brief   Internal function to track the highest order reset requested by IPs
 *
 * @param   ResetType          The input reset type requested.  Only valid requests are
 *                             SilResetRequestWarmDef or SilResetRequestColdDef.  Any
 *                             other request will force a cold reset.
 * @param   DeferredResetType  The flag to track the deferred reset type. This flag will only
 *                             maintain the highest order reset type with cold reset
 *                             taking precedence over warm reset.
 * @return  void
 */
static
void
SetDeferredResetType (
  SIL_STATUS  ResetType,
  SIL_STATUS  *DeferredResetType
  )
{
  // If the deferred reset type is already cold reset, do nothing.
  if (*DeferredResetType != SilResetRequestColdDef) {
    if ((ResetType == SilResetRequestWarmDef) ||
      (ResetType == SilResetRequestColdDef)) {
      *DeferredResetType = ResetType;
    } else {
      // Input reset type is invalid, perform cold reset
      XSIM_TRACEPOINT(SIL_TRACE_ENTRY,
        "Invalid deferred reset request: 0x%x, requesting cold reset\n",
        ResetType
        );
      *DeferredResetType = SilResetRequestColdDef;
    }
  }
}

/**
 * xSimInitializeIps
 *
 * @brief  Loop through the IP records array, calling each IPblock to init their respective silicon blocks.
 *
 * @param  SilContext  A context structure through which host firmware defined data
 *                     can be passed to openSIL. The host firmware is responsible
 *                     for initializing the SIL_CONTEXT structure.
 * @param  LclIpRecord Input pointer to the IP record list.
 *
 * @return SIL_STATUS
 *
 */
static
SIL_STATUS
xSimInitializeIps (
  SIL_CONTEXT     *SilContext,
  const IP_RECORD *LclIpRecord
  )
{
  SIL_STATUS LclStatus;
  SIL_STATUS DeferredResetType;

  LclStatus = SilPass;

  // Initialize the flag that tracks the deferred reset type.
  DeferredResetType = SilPass;

  while (LclIpRecord->IpID < SilId_ListEnd) {
    XSIM_TRACEPOINT(SIL_TRACE_INFO,
      "SIL Init:IpRcd %x: %x, %x, %x, %x\n",
      LclIpRecord,
      LclIpRecord->IpID,
      LclIpRecord->BlkRequestSize,
      LclIpRecord->SetInput,
      LclIpRecord->Initialize
      );
    LclStatus = (LclIpRecord->Initialize == NULL)? SilPass :
      LclIpRecord->Initialize(SilContext);

    if (LclStatus != SilPass) {
      if ((LclStatus == SilResetRequestColdDef) ||
        (LclStatus == SilResetRequestWarmDef)) {
        SetDeferredResetType(LclStatus, &DeferredResetType);
        // If the status was a deferred reset request, continue to next IP
        continue;
      }
      break;
    }
    LclIpRecord++;          // move to next record
  }

  if (DeferredResetType != SilPass) {
    // There was a deferred reset request.  Return the reset type in status.
    LclStatus = DeferredResetType;
  }

  return LclStatus;
}

/**
 * InitializeAMDSiTp1
 *
 * @brief   Start the Silicon Initialization process for timepoint 1
 *
 * @details This is a host API function.  The prototype, text
 *          description and Doxygen text are in in xSim-api.h
 *
 * @param   SilContext  A context structure through which host firmware defined data
 *                      can be passed to openSIL. The host firmware is responsible
 *                      for initializing the SIL_CONTEXT structure.
 * @return  SIL_STATUS
 */
SIL_STATUS
InitializeAMDSiTp1 (
  SIL_CONTEXT   *SilContext
  )
{
  SIL_STATUS      LclStatus = SilPass;
  const IP_RECORD *LclIpRecord;  // pointer to IP Record being scanned

  XSIM_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  LclIpRecord = GetActiveSocIpListTp1 ();
  if (LclIpRecord == NULL) {
    assert(LclIpRecord != NULL);
    XSIM_TRACEPOINT(SIL_TRACE_ERROR, "Unable to get active SoC Ip List.\n");
    return SilNotFound;
  }

  XSIM_TRACEPOINT(SIL_TRACE_INFO, "Execute xSIM IP module Init for TP1\n");

  LclStatus = xSimInitializeIps(SilContext, LclIpRecord);

  XSIM_TRACEPOINT(SIL_TRACE_EXIT, "Status: %x\n", LclStatus);
  return LclStatus;
}

/**
 * InitializeAMDSiTp2
 *
 * @brief   Start the Silicon Initialization process for timepoint 2
 *
 * @details This is a host API function.  The prototype, text
 *          description and Doxygen text are in in xSim-api.h
 *
 * @param   SilContext   A context structure through which host firmware defined data
 *                       can be passed to openSIL. The host firmware is responsible
 *                       for initializing the SIL_CONTEXT structure.
 * @return  SIL_STATUS
 */
SIL_STATUS
InitializeAMDSiTp2 (
  SIL_CONTEXT   *SilContext
  )
{
  SIL_STATUS      LclStatus = SilPass;
  const IP_RECORD *LclIpRecord;  // pointer to IP Record being scanned

  XSIM_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  LclIpRecord = GetActiveSocIpListTp2 ();
  if (LclIpRecord == NULL) {
    assert(LclIpRecord != NULL);
    XSIM_TRACEPOINT(SIL_TRACE_ERROR, "Unable to get active SoC Ip List.\n");
    return SilNotFound;
  }

  XSIM_TRACEPOINT(SIL_TRACE_INFO, "Execute xSIM IP module Init for TP2\n");

  LclStatus = xSimInitializeIps(SilContext, LclIpRecord);

  XSIM_TRACEPOINT(SIL_TRACE_EXIT, "Status: %x\n", LclStatus);
  return LclStatus;
}

/**
 * InitializeAMDSiTp3
 *
 * @brief   Start the Silicon Initialization process for timepoint 3
 *
 * @details This is a host API function.  The prototype, text
 *          description and Doxygen text are in in xSim-api.h
 *
 * @param   SilContext    A context structure through which host firmware defined data
 *                        can be passed to openSIL. The host firmware is responsible
 *                        for initializing the SIL_CONTEXT structure.
 * @return  SIL_STATUS
 */
SIL_STATUS
InitializeAMDSiTp3 (
  SIL_CONTEXT   *SilContext
  )
{
  SIL_STATUS      LclStatus = SilPass;
  const IP_RECORD *LclIpRecord;  // pointer to IP Record being scanned

  XSIM_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  LclIpRecord = GetActiveSocIpListTp3 ();
  if (LclIpRecord == NULL) {
    assert(LclIpRecord != NULL);
    XSIM_TRACEPOINT(SIL_TRACE_ERROR, "Unable to get active SoC Ip List.\n");
    return SilNotFound;
  }

  XSIM_TRACEPOINT(SIL_TRACE_INFO, "Execute xSIM IP module Init for TP3\n");

  LclStatus = xSimInitializeIps(SilContext, LclIpRecord);

  XSIM_TRACEPOINT(SIL_TRACE_EXIT, "Status: %x\n", LclStatus);
  return LclStatus;
}
