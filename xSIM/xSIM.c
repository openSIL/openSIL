
/**
 * @file xSIM.c
 * @brief Common Host API functions
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

/**
 * @defgroup xSIM_group xSIMgroup:Silicon Initialization Module
 * This module is responsible for the very early (basic) Si initialization
 *   comming off of reset.
 */

#include <SilCommon.h>
#include <string.h>
#include <xSIM.h>
#include "IpHandler.h"

/**
 * Global API table.  This table contains an entry for every IP defined by
 * SIL_DATA_BLOCK_ID.  This structure is populated during xSIM init.
 *
 */
void  *mApi[SilId_ListEnd] = {NULL};

/*
 * Flag to track the deferred reset type.
 * This flag will only maintain the highest order reset type with cold reset
 * taking precedence over warm reset.
 *
 * By default the value is SilPass (0)
 *
 * The deferred reset type is used to minimize the number of resets necessary
 * for silicon initialization.  If a reset is critical and has timing
 * constraints, an immediate reset * should be used.  If the reset can wait to
 * allow other IPs to perform their initialization before reset, then the reset
 * should be deferred.
 */
SIL_STATUS  mDeferredResetType = SilPass;

/** Rounds the Data up to the RoundTo value
 */
static
size_t
RoundUp(
  size_t  Data,
  size_t  RoundTo
)
{
  return Data + RoundTo - (Data % RoundTo);
}

/**
 * Locate the Information record for the active SoC for timepoint 1
 *
 * There may be more than one SoC supported in this build. This fucntion
 * determines what SoC is present in the system and locates the
 * appropriate IP list corresponding to that SoC.
 * @param none
 * @returns pointer to the begining of the SoC IP table, IP block list.
 */
static const
IP_RECORD
*GetActiveSocIpListTp1( void )
{

//  SOC_IP_TABLE  *SocInfoRecord;
  const IP_RECORD   *IpRecord = NULL;

  // Get the Logical CPU ID
  // loop through the SoC list to match the Logical ID
  // return the address of the 1st entry in the IP list, else ASSERT

  /* Scan the SoC list for the platform,
     locate record for the SoC present in the socket. */
  // PLAT_SOC_LIST   *ListOfSoc;
  // IpRecord = &SocInfoRecord.IpList[0];
  //
  extern const SOC_IP_TABLE SocIpTblF19M10Tp1;
  IpRecord = SocIpTblF19M10Tp1.IpList;

  XSIM_TRACEPOINT (SIL_TRACE_INFO, "IpRecord: %x\n", IpRecord);
  assert (IpRecord != NULL);

  return IpRecord;
}

/**
 * Locate the Information record for the active SoC for timepoint 2
 *
 * There may be more than one SoC supported in this build. This fucntion
 * determines what SoC is present in the system and locates the
 * appropriate IP list corresponding to that SoC.
 * @param none
 * @returns pointer to the begining of the SoC IP table, IP block list.
 */
static const
IP_RECORD
*GetActiveSocIpListTp2( void )
{

//  SOC_IP_TABLE  *SocInfoRecord;
  const IP_RECORD   *IpRecord = NULL;

  // Get the Logical CPU ID
  // loop through the SoC list to match the Logical ID
  // return the address of the 1st entry in the IP list, else ASSERT

  /* Scan the SoC list for the platform,
     locate record for the SoC present in the socket. */
  // PLAT_SOC_LIST   *ListOfSoc;
  // IpRecord = &SocInfoRecord.IpList[0];
  //
  extern const SOC_IP_TABLE SocIpTblF19M10Tp2;
  IpRecord = SocIpTblF19M10Tp2.IpList;

  XSIM_TRACEPOINT (SIL_TRACE_INFO, "IpRecord: %x\n", IpRecord);
  assert (IpRecord != NULL);

  return IpRecord;
}

/**
 * Locate the Information record for the active SoC for timepoint 3
 *
 * There may be more than one SoC supported in this build. This fucntion
 * determines what SoC is present in the system and locates the
 * appropriate IP list corresponding to that SoC.
 * @param none
 * @returns pointer to the begining of the SoC IP table, IP block list.
 */
static const
IP_RECORD
*GetActiveSocIpListTp3( void )
{

//  SOC_IP_TABLE  *SocInfoRecord;
  const IP_RECORD   *IpRecord = NULL;

  // Get the Logical CPU ID
  // loop through the SoC list to match the Logical ID
  // return the address of the 1st entry in the IP list, else ASSERT

  /* Scan the SoC list for the platform,
     locate record for the SoC present in the socket. */
  // PLAT_SOC_LIST   *ListOfSoc;
  // IpRecord = &SocInfoRecord.IpList[0];
  //
  extern const SOC_IP_TABLE SocIpTblF19M10Tp3;
  IpRecord = SocIpTblF19M10Tp3.IpList;

  XSIM_TRACEPOINT (SIL_TRACE_INFO, "IpRecord: %x\n", IpRecord);
  assert (IpRecord != NULL);

  return IpRecord;
}
/*********************************************************************
 * API Functions
 *********************************************************************/
/* Determine size of memory needed
 *  This is a host API function, so you can find the
 *  prototype, text description and Doxygen text in xSim-api.h
 *
 */
size_t
xSimQueryMemoryRequirements(void)
{
  const IP_RECORD         *LclIpRecord;
  size_t                  RequestTotal = 0;

  XSIM_TRACEPOINT(SIL_TRACE_ENTRY, "QueryMem.\n");

  // First apply the needs of the xSIM core
  RequestTotal = sizeof(SIL_BLOCK_VARIABLES);

  //Now add the data needs of each IP
  LclIpRecord = GetActiveSocIpListTp1();
  while (LclIpRecord->IpID < SilId_ListEnd) {
    XSIM_TRACEPOINT(SIL_TRACE_INFO,
      "QueryMem:IpRecord at: 0x%x \n",
      LclIpRecord
      );
    if (LclIpRecord->BlkRequestSize > 0) {
      RequestTotal += RoundUp (
        sizeof(SIL_INFO_BLOCK_HEADER) + LclIpRecord->BlkRequestSize,
        sizeof(uint32_t)
        );
    }
    LclIpRecord++;
  }

  //Finallly, round up to a convenient boundary (2K)
  RequestTotal = RoundUp (RequestTotal, 2 * KILOBYTE);

  XSIM_TRACEPOINT(SIL_TRACE_INFO,
    "QueryMem:ReqTotal at: 0x%x \n",
    RequestTotal
    );
  return RequestTotal;
}

/**
 * xSimInitializeIpApis
 *
 * @brief   Call all IP ApiInit functions to initialize IP-to-IP and IP Internal APIs
 *
 * @param   LclIpRecord  Input pointer to the IP record list.
 *
 * @return  SIL_STATUS
 *
 */
static
SIL_STATUS
xSimInitializeIpApis (
  const IP_RECORD     *LclIpRecord
  )
{
  SIL_STATUS          LclStatus;

  LclStatus = SilPass;
  /**
   * Initialize global mApi with the IP API specified in the IP list.
   * This needs to be intialized before any IP initialization.
   */
  while (LclIpRecord->IpID < SilId_ListEnd) {
    if (LclIpRecord->ApiInit != NULL) {
      LclStatus = LclIpRecord->ApiInit ();
      if (LclStatus != SilPass) {
        XSIM_TRACEPOINT(SIL_TRACE_ERROR, "Api init failed for Ip ID: %d\n",
          LclIpRecord->IpID);
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
 * @details   This is a host API function, so you can find the prototype, text
 *            description and Doxygen text in xSim-api.h
 *
 * @param     BaseAddress   The base address of the openSIL memory block allocated
 *                          and passed in from Host.
 * @param     MemorySize    The size of the block allocated at BaseAddress
 *
 * @returns   SIL_STATUS
 *
 */
SIL_STATUS
xSimAssignMemoryTp1 (
  void            *BaseAddress,
  size_t           MemorySize
  )
{
  SIL_BLOCK_VARIABLES *LclVarsPtr;    ///< pointer to global Vars
  const IP_RECORD     *LclIpRecord;   ///< pointer to IP Record being scanned
  const IP_RECORD     *IpRecordHead;  ///< pointer to IP record list HEAD
  size_t              LclStatus;      ///< collects status from calls made

  // Set the sil block base address
  SilSetMemoryBase (BaseAddress);     // Set the global var

  IpRecordHead = GetActiveSocIpListTp1();
  LclIpRecord = IpRecordHead;

  // Fill the openSIL private vars block

  XSIM_TRACEPOINT(SIL_TRACE_INFO,
    "openSIL AssignMemBlk1:gVars at: 0x%x \n",
    BaseAddress
    );
  LclVarsPtr = (SIL_BLOCK_VARIABLES *)BaseAddress;
  LclVarsPtr->HostBlockSize = (uint32_t) MemorySize;
  LclVarsPtr->FreeSpaceOffset = sizeof(SIL_BLOCK_VARIABLES);
  LclVarsPtr->FreeSpaceLeft = (uint32_t) (MemorySize -
    sizeof(SIL_BLOCK_VARIABLES));

/* Waiting for SoC table to be generated by Kconfig (next PR)
 *  LclVarsPtr->ActiveSoC = SocInfoRecord->XsimVars;   // block copy of var struct
 *  LclVarsPtr->PlatformData.ApobBaseAddress = CONFIG_PLAT_APOB_ADDRESS;
 */

  // Initialize IP APIs specified in the IP list. This is required before any IP entrypoint is called.
  LclStatus = xSimInitializeIpApis (LclIpRecord);
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
    XSIM_TRACEPOINT (SIL_TRACE_INFO,
      "openSIL Assign:IpRcd %x: %x, %x, %x, %x\n",
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
      XSIM_TRACEPOINT (SIL_TRACE_INFO,
        "openSIL SetInput function is not reported for IP#0x%x fails.\n",
        LclIpRecord->IpID);
      continue;
    }

    LclStatus = LclIpRecord->SetInput();
    if (LclStatus != SilPass) {
      XSIM_TRACEPOINT (SIL_TRACE_ERROR,
        "openSIL SetInput function for IP#0x%x fails.\n",
        LclIpRecord->IpID);
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
 * @details   This is a host API function, so you can find the prototype, text
 *            description and Doxygen text in xSim-api.h
 *
 * @param     BaseAddress   The base address of the openSIL memory block allocated
 *                          and passed in from Host.
 * @param     MemorySize    The size of the block allocated at BaseAddress
 *
 * @returns   SIL_STATUS
 *
 */
SIL_STATUS
xSimAssignMemoryTp2 (
  void            *BaseAddress,
  size_t           MemorySize
  )
{
  const IP_RECORD     *LclIpRecord;   ///< pointer to IP Record being scanned
  size_t              LclStatus;      ///< collects status from calls made

  // At this time the MemorySize pramteter is not used
  UNUSED (MemorySize);

  // Set the sil block base address
  SilSetMemoryBase (BaseAddress);     // Set the global var

  LclIpRecord = GetActiveSocIpListTp2();

  // Initialize IP APIs specified in the IP list. This is required before any IP entrypoint is called.
  LclStatus = xSimInitializeIpApis (LclIpRecord);
  if (LclStatus != SilPass) {
    return LclStatus;
  }

  // For secondary time points, openSIL should not reinitialize its data.
  XSIM_TRACEPOINT(SIL_TRACE_INFO, "Assign new openSIL base: 0x%x\n", BaseAddress);
  return SilPass;
}

/**
 * xSimAssignMemoryTp3
 *
 * @brief     Assign Memory to sub-modules and initialize internal APIs for
 *            openSIL IPs.
 *
 * @details   This is a host API function, so you can find the prototype, text
 *            description and Doxygen text in xSim-api.h
 *
 * @param     BaseAddress   The base address of the openSIL memory block allocated
 *                          and passed in from Host.
 * @param     MemorySize    The size of the block allocated at BaseAddress
 *
 * @returns   SIL_STATUS
 *
 */
SIL_STATUS
xSimAssignMemoryTp3 (
  void            *BaseAddress,
  size_t           MemorySize
  )
{
  const IP_RECORD     *LclIpRecord;   ///< pointer to IP Record being scanned
  size_t              LclStatus;      ///< collects status from calls made

  // At this time the MemorySize pramteter is not used
  UNUSED (MemorySize);

  // Set the sil block base address
  SilSetMemoryBase (BaseAddress);     // Set the global var

  LclIpRecord = GetActiveSocIpListTp3();

  // Initialize IP APIs specified in the IP list. This is required before any IP entrypoint is called.
  LclStatus = xSimInitializeIpApis (LclIpRecord);
  if (LclStatus != SilPass) {
    return LclStatus;
  }

  // For secondary time points, openSIL should not reinitialize its data.
  XSIM_TRACEPOINT(SIL_TRACE_INFO, "Assign new openSIL base: 0x%x\n", BaseAddress);
  return SilPass;
}

/**
 * SilCreateInfoBlock
 * @brief Create an Info Block
 *
 * This internal xSIM routine is called by the IPs to assign some of the
 * Host memory to the IP for use as an Input block, private block or
 * output block. Each block must have an unique identifier in the openSIL ID
 * list.
 *
 * @param BlockTag - IP block unique identifier
 * @param BlockSize - size of the requested block
 * @param Block_Instance - Instance of block using this ID
 * @param Block_MajorRev - Revision of the IP data structure used
 * @param Block_MinorRev  - Revision (minor) of the IP data struct used.
 * @return pointer to the IP data segment of the assigned memory space.
 *
 */
void*
SilCreateInfoBlock (
  uint32_t BlockTag,
  size_t   BlockSize,
  uint16_t Block_Instance,
  uint8_t  Block_MajorRev,
  uint8_t  Block_MinorRev
  )
{
  SIL_INFO_BLOCK_HEADER *Header = NULL;
  SIL_BLOCK_VARIABLES   *LclVarsPtr;
  size_t                RequestSize;

  LclVarsPtr = (SIL_BLOCK_VARIABLES *)(SilGetMemoryBase ());

  if (LclVarsPtr == NULL) { //Global not yet assinged - error
    assert(LclVarsPtr != NULL);
    return NULL;
  }

  // calculate the amount of memory, rounded up to DWORD boundary
  RequestSize = RoundUp (sizeof(SIL_INFO_BLOCK_HEADER) + BlockSize,
    sizeof(uint32_t));

  Header = (SIL_INFO_BLOCK_HEADER *)((uint8_t *)LclVarsPtr +
    LclVarsPtr->FreeSpaceOffset);
  XSIM_TRACEPOINT (SIL_TRACE_INFO, "openSIL CreateBlk at: 0x%x [0x%x] \n", Header,
    RequestSize);
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
  Header->Id        = BlockTag;
  Header->InfoBlockDataSize = (uint32_t)RequestSize;
  Header->Instance  = Block_Instance;
  Header->RevMajor  = Block_MajorRev;
  Header->RevMinor  = Block_MinorRev;
  return Header->InfoBlockData;
}

/*
 * SilFindStructure
 *  This is a host API function, so you can find the
 *  prototype, text description and Doxygen text in xSim-api.h
 */
void*
SilFindStructure (
  SIL_DATA_BLOCK_ID structureID,
  uint16_t          InstanceNum
  )
{
  return xUslFindStructure (structureID, InstanceNum);
};

/*--------------------------------------------------------------------
 * SilDebugSetup
 *  This is a host API function, so you can find the
 *  prototype, text description and Doxygen text in xSim-api.h
 */
SIL_STATUS
SilDebugSetup (
  HOST_DEBUG_SERVICE HostDbgService
  )
{
  if (mHostDebugService != NULL) {
    return SilAborted;  // Host debug service is initialized already
  }
  if (HostDbgService == NULL) {
    return SilInvalidParameter;
  } else {
    // Set the host debug service
    mHostDebugService = HostDbgService;
    XSIM_TRACEPOINT(SIL_TRACE_INFO,
      "Host debug service successfully initialized for openSIL.\n");
  }

  return SilPass;
}

/**
 * SetDeferredResetType
 *
 * @brief   Internal function to track the highest order reset requested by IPs
 *
 * @param   ResetType The input reset type requested.  Only valid requests are
 *                    SilResetRequestWarmDef or SilResetRequestColdDef.  Any
 *                    other request will force a cold reset.
 * @return  void
 */
static
void
SetDeferredResetType (
  SIL_STATUS  ResetType
)
{
  // If the deferred reset type is already cold reset, do nothing.
  if (mDeferredResetType != SilResetRequestColdDef) {
    if ((ResetType == SilResetRequestWarmDef) ||
    (ResetType == SilResetRequestColdDef)) {
      mDeferredResetType = ResetType;
    } else {
      // Input reset type is invalid, perform cold reset
      XSIM_TRACEPOINT (SIL_TRACE_ENTRY,
        "Invalid deferred reset request: 0x%x, requesting cold reset\n",
        ResetType);
      mDeferredResetType = SilResetRequestColdDef;
    }
  }
}

/**
 * xSimInitializeIps
 *
 * @brief  Loop through the IP records array, calling each IPblock to init their respective silicon blocks.
 *
 * @param  LclIpRecord Input pointer to the IP record list.
 *
 * @return SIL_STATUS
 *
 */
static
SIL_STATUS
xSimInitializeIps (
  const IP_RECORD *LclIpRecord
  )
{
  SIL_STATUS LclStatus;

  LclStatus = SilPass;

  while (LclIpRecord->IpID < SilId_ListEnd) {
    XSIM_TRACEPOINT(SIL_TRACE_INFO, "openSIL Init:IpRcd %x: %x, %x, %x, %x\n",
      LclIpRecord, LclIpRecord->IpID, LclIpRecord->BlkRequestSize,
      LclIpRecord->SetInput, LclIpRecord->Initialize);
    LclStatus = (LclIpRecord->Initialize == NULL)? SilPass :
      LclIpRecord->Initialize ();

    if (LclStatus != SilPass) {
      if ((LclStatus == SilResetRequestColdDef) ||
        (LclStatus == SilResetRequestWarmDef)) {
        SetDeferredResetType (LclStatus);
        // If the status was a deferred reset request, continue to next IP
        continue;
      }
      break;
    }
    LclIpRecord++;          // move to next record
  }

  if (mDeferredResetType != SilPass) {
    // There was a deferred reset request.  Return the reset type in status.
    LclStatus = mDeferredResetType;
  }

  return LclStatus;
}

/**
 * InitializeSiTp1
 *
 * @brief   Start the Silicon Initialization process for timepoint 1
 *
 * @details This is a host API function.  You can find the prototype, text
 *          description and Doxygen text in xSim-api.h
 *
 * @return  SIL_STATUS
 */
SIL_STATUS
InitializeSiTp1 (void)
{
  SIL_STATUS      LclStatus = SilPass;
  const IP_RECORD *LclIpRecord;  // pointer to IP Record being scanned

  XSIM_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  LclIpRecord = GetActiveSocIpListTp1();

  XSIM_TRACEPOINT(SIL_TRACE_INFO, "Execute xSIM IP module Init for TP1\n");

  LclStatus = xSimInitializeIps (LclIpRecord);

  XSIM_TRACEPOINT(SIL_TRACE_EXIT, "Status: %x\n", LclStatus);
  return LclStatus;
}

/**
 * InitializeSiTp2
 *
 * @brief   Start the Silicon Initialization process for timepoint 2
 *
 * @details This is a host API function.  You can find the prototype, text
 *          description and Doxygen text in xSim-api.h
 *
 * @return  SIL_STATUS
 */
SIL_STATUS
InitializeSiTp2 (void)
{
  SIL_STATUS      LclStatus = SilPass;
  const IP_RECORD *LclIpRecord;  // pointer to IP Record being scanned

  XSIM_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  LclIpRecord = GetActiveSocIpListTp2();

  XSIM_TRACEPOINT(SIL_TRACE_INFO, "Execute xSIM IP module Init for TP2\n");

  LclStatus = xSimInitializeIps (LclIpRecord);

  XSIM_TRACEPOINT(SIL_TRACE_EXIT, "Status: %x\n", LclStatus);
  return LclStatus;
}

/**
 * InitializeSiTp3
 *
 * @brief   Start the Silicon Initialization process for timepoint 3
 *
 * @details This is a host API function.  You can find the prototype, text
 *          description and Doxygen text in xSim-api.h
 *
 * @return  SIL_STATUS
 */
SIL_STATUS
InitializeSiTp3 (void)
{
  SIL_STATUS      LclStatus = SilPass;
  const IP_RECORD *LclIpRecord;  // pointer to IP Record being scanned

  XSIM_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  LclIpRecord = GetActiveSocIpListTp3();

  XSIM_TRACEPOINT(SIL_TRACE_INFO, "Execute xSIM IP module Init for TP3\n");

  LclStatus = xSimInitializeIps (LclIpRecord);

  XSIM_TRACEPOINT(SIL_TRACE_EXIT, "Status: %x\n", LclStatus);
  return LclStatus;
}
