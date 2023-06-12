/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  ApobCommon.c
 * @brief OpenSIL APOB functions
 *
 */

#include <SilCommon.h>
#include <APOB/Common/ApobCmn.h>
#include <APOB/ApobIp2Ip.h>

APOBLIB_INFO gApobLibInfo = {false, 0, 0};

/**
 * ApobInit
 * @brief Get APOB address, and save to the global variable
 *
 * @details  Passing the APOB base address as a parameter is optional.
 *           ApobBaseAddress will not set the address beyond this Time Point.
 *           If the APOB address has been initialized in this time point then
 *           this argument will not be used.
 *
 * @param   ApobBaseAddress  location of the APOB optional
 *
 * @return     SIL_STATUS       The APOB save to gApobLibInfo successfully
 *
 **/
static
SIL_STATUS
ApobInit (
  uintptr_t      ApobBaseAddress
  )
{

  APOB_BASE_HEADER                *ApobHeaderPtr;

  APOB_TRACEPOINT(SIL_TRACE_ENTRY, "ApobInit\n");

  if (gApobLibInfo.Supported == false) {
    //
    // Check if the caller provided the APOB Address
    //
    ApobHeaderPtr = (ApobBaseAddress != 0) ? (APOB_BASE_HEADER *)(size_t)ApobBaseAddress :
      (APOB_BASE_HEADER *)(size_t)APOB_BASE_ADDRESS;
    //APOB Info data haven't been initialized
    APOB_TRACEPOINT(SIL_TRACE_INFO, "Init APOB HOB Info struct\n");
    APOB_TRACEPOINT(SIL_TRACE_INFO, "APOB header Addr 0x%x\n", ApobHeaderPtr);
    APOB_TRACEPOINT(SIL_TRACE_INFO, "APOB header size 0x%x\n", ApobHeaderPtr->Size);

    //Signature verification failed
    if (ApobHeaderPtr->Signature != APOB_SIGNATURE) {
      APOB_TRACEPOINT(SIL_TRACE_INFO, "APOB SIG Check fail, APOB is not supported\n");
      return SilUnsupported;
    }

    // Check APOB Size
    if ((ApobHeaderPtr->Size == 0) ||
      (ApobHeaderPtr->Size == 0xFFFFFFFFul)) {
      APOB_TRACEPOINT(SIL_TRACE_INFO, "APOB Size Check fail\n");
      return SilOutOfResources;
    }

    gApobLibInfo.Supported = true;
    gApobLibInfo.ApobAddr = (uintptr_t)ApobHeaderPtr;
    gApobLibInfo.ApobSize = ApobHeaderPtr->Size;
  }

  return SilPass;
}

/**
 * AmdGetApobInfo
 * @brief Get APOBLib Info Data based on APOB base address
 *
 * @param  ApobInfo             Input buffer to fill the APOB Info
 *
 * @retval SilPass              The APOB info retrieved successfully
 * @retval SilInvalidParameter  Parameter input is invalid
 * @retval SilOutOfResources    ApobInit returned out of resources
 *
 **/
static
SIL_STATUS
AmdGetApobInfo (
  APOBLIB_INFO   *ApobInfo
  )
{
  assert(ApobInfo != NULL);
  if (ApobInfo == NULL) {
    return SilUnsupported;
  }

  if (gApobLibInfo.Supported == false) {
    if (ApobInit((uintptr_t)ApobInfo->ApobAddr) != SilPass)
      return SilOutOfResources;
  }

  ApobInfo->Supported = gApobLibInfo.Supported;
  ApobInfo->ApobAddr = gApobLibInfo.ApobAddr;
  ApobInfo->ApobSize = gApobLibInfo.ApobSize;

  return SilPass;
}

/**
 * AmdGetApobEntry
 * @brief Return Array of Entry instance as specified GroupId, DataTypeId
 *
 * @details  Passing the APOB base address as a parameter is optional.
 *           ApobBaseAddress will not set the address beyond this Time Point.
 *           If the APOB address has been initialized in this time point then
 *           this argument will not be used.
 *
 *  @param    GroupID          GroupId of Apob entry
 *  @param    DataTypeID       DataTypeID of Apob entry
 *  @param    NumofEntry       Number of entry found of specific GroupId, DataTypeId
 *  @param    ApobBaseAddress  location of the APOB optional
 *  @param    ApobEntries      Point to the array which used to hold the ApobEntries
 *                             The array need be allocated outside of the routine
 *
 *  @retval       SilPass           Function succeed
 *  @retval       NON-ZERO          Error occurs
 */
static
SIL_STATUS
AmdGetApobEntry (
  uint32_t          GroupID,
  uint32_t          DataTypeID,
  uint32_t          *NumofEntry,
  uint32_t          ApobBaseAddress,
  APOB_TYPE_HEADER  **ApobEntries
  )
{
  uint8_t           *ApobEntryBin;
  APOB_TYPE_HEADER  *ApobEntry;
  uint32_t          LclNumofEntry;
  APOB_BASE_HEADER  *ApobHeaderPtr;
  APOBLIB_INFO      ApobInfoPtr;
  SIL_STATUS        Status;

  *NumofEntry = 0;
  *ApobEntries = NULL;
  ApobEntryBin = NULL;
  APOB_TRACEPOINT(SIL_TRACE_INFO, "AmdGetApobEntry %x %x\n", GroupID, DataTypeID);

  ApobInfoPtr.ApobAddr = 0;
  //
  // If the APOB ADDRESS provided by the caller is not 0
  // then use then pass it to the next step
  //
  if (ApobBaseAddress != 0) {
    ApobInfoPtr.ApobAddr = (uint64_t)ApobBaseAddress;
  }
  Status = AmdGetApobInfo(&ApobInfoPtr);
  if (Status != SilPass) {
    APOB_TRACEPOINT(SIL_TRACE_ERROR, "APOBLIB Info data uninitialized.\n");
    return Status;
  }

  if (ApobInfoPtr.Supported == false) {
    APOB_TRACEPOINT(SIL_TRACE_ERROR, "Error detected in the APOB DATA\n");
    return SilUnsupported;
  }

  ApobHeaderPtr = (APOB_BASE_HEADER *) (size_t) ApobInfoPtr.ApobAddr;
  APOB_TRACEPOINT(SIL_TRACE_INFO,
    "ApobInfo Supported %x ApobAddr %lx ApobSize %x\n",
    ApobInfoPtr.Supported,
    ApobInfoPtr.ApobAddr,
    ApobInfoPtr.ApobSize
    );

  APOB_TRACEPOINT(SIL_TRACE_INFO, "AmdGetApobEntry %x %x\n", GroupID, DataTypeID);
  APOB_TRACEPOINT(SIL_TRACE_INFO, "APOB Addr %x\n", ApobHeaderPtr);

  if (ApobHeaderPtr->Version >= 5) {
    ApobEntryBin = (uint8_t *) ApobHeaderPtr;
    ApobEntryBin += ApobHeaderPtr->OffsetOfFirstEntry;
  } else {
    APOB_TRACEPOINT(SIL_TRACE_ERROR, "APOB version is not supported\n");
    return SilUnsupported;
  }

  ApobEntry = (APOB_TYPE_HEADER *) ApobEntryBin;
  LclNumofEntry = 0;

  while (ApobEntryBin < ((uint8_t *) ApobHeaderPtr + ApobHeaderPtr->Size)) {
    if ((ApobEntry->GroupID == GroupID) &&
      (ApobEntry->DataTypeID == DataTypeID)) {
      //Found a matched entry
      LclNumofEntry++;
      *ApobEntries = ApobEntry;
      ApobEntries++;
      APOB_TRACEPOINT(SIL_TRACE_INFO, "Type found @0x%x\n", ApobEntry);
    }
    ApobEntryBin += (ApobEntry->TypeSize);
    ApobEntry = (APOB_TYPE_HEADER *) ApobEntryBin;
  }

  *NumofEntry = LclNumofEntry;
  APOB_TRACEPOINT(SIL_TRACE_INFO, "Total  %d entries found\n", LclNumofEntry);

  if (LclNumofEntry == 0) {
    APOB_TRACEPOINT(SIL_TRACE_INFO, "ApobEntry NOT found.\n");
    return SilNotFound;
  }

  return SilPass;
}

/**
 * AmdGetApobEntryInstance
 * @brief   Return Entry instance as specified GroupId, DataTypeId, InstanceId
 *
 * @details  Passing the APOB base address as a parameter is optional.
 *           ApobBaseAddress will not set the address beyond this Time Point.
 *           If the APOB address has been initialized in this time point then
 *           this argument will not be used.
 *
 * @param   GroupID          GroupId of Apob entry
 * @param   DataTypeID       DataTypeID of Apob entry
 * @param   InstanceID       InstanceID of Apob entry
 * @param   ApobBaseAddress  Location the the APOB optional
 * @param   ApobEntry        Point to the array which used to hold the ApobEntries
 *                              The array need be allocated outside of the routine
 *
 * @retval       SilPass        Function succeed
 * @retval       NON-ZERO       Error occurs
 */
SIL_STATUS
AmdGetApobEntryInstance (
  uint32_t          GroupID,
  uint32_t          DataTypeID,
  uint32_t          InstanceID,
  uint32_t          ApobBaseAddress,
  APOB_TYPE_HEADER  **ApobEntry
  )
{
  uint32_t          Index;
  uint32_t          NumofInstance;
  APOB_TYPE_HEADER  *LclApobEntries[APOB_ENTRY_INSTANCE_MAX];
  uint32_t          LclNumofEntry;
  APOB_TYPE_HEADER  *LclApobEntry;

  *ApobEntry = NULL;
  LclApobEntry = NULL;
  APOB_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  APOB_TRACEPOINT(SIL_TRACE_INFO,
    "AmdGetApobEntryInstance 0x%x 0x%x 0x%x\n",
    GroupID,
    DataTypeID,
    InstanceID
    );

  LclNumofEntry = 0;
  AmdGetApobEntry(GroupID, DataTypeID, &LclNumofEntry, ApobBaseAddress, &LclApobEntries[0]);
  if (LclNumofEntry == 0) {
    APOB_TRACEPOINT(SIL_TRACE_INFO, "No entry found\n");
    return SilNotFound;
  }

  //Scan the Array to get the specific instance
  NumofInstance = 0;
  for (Index = 0; Index < LclNumofEntry; Index++) {
    if (LclApobEntries[Index]->InstanceID == InstanceID) {
      NumofInstance++;
      LclApobEntry = LclApobEntries[Index];
    }
  }

  if (NumofInstance == 0) {
    return SilNotFound;
  } else if (NumofInstance == 1) {
    APOB_TRACEPOINT(SIL_TRACE_INFO, "Instance found @0x%x\n", LclApobEntry);
    *ApobEntry = LclApobEntry;
    return SilPass;
  } else {
    return SilUnsupported;
  }
}

/**
 * ApobGetApcbUpdate
 *
 * @brief Get the Apcb Update for the specified token from the APOB_APCB_UPDATES_STRUCT
 *
 * @param ApobEntry   A pointer to the APOB_APCB_UPDATES_STRUCT
 * @param Token       The token to get
 * @param Value       The value for the specified token
 *
 * @return SIL_STATUS
 * @retval SilPass     If the token is found
 *         SilNotFound Otherwise
 */
SIL_STATUS
ApobGetApcbUpdate (
  APOB_APCB_UPDATES_STRUCT  *ApobEntry,
  uint32_t                  Token,
  uint32_t                  *Value
  )
{
  SIL_STATUS                Status;
  uint32_t                  ApcbIndex;

  Status = SilNotFound;
  for (ApcbIndex = 0; ApcbIndex < ApobEntry->NumItems; ApcbIndex++) {
    if (ApobEntry->Item[ApcbIndex].Token == Token) {
      *Value = ApobEntry->Item[ApcbIndex].Value;
      Status = SilPass;
    }
  }
  return Status;
}
