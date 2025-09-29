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

/**
 * AmdGetApobEntry
 * @brief Return Array of Entry instance as specified GroupId, DataTypeId
 *
 * @details  Passing the APOB base address as a parameter is optional.
 *           ApobBaseAddress will not set the address beyond this Time Point.
 *           If the APOB address has been initialized in this time point then
 *           this argument will not be used.
 *
 *  @param    SilContext       A context structure through which host firmware defined data
 *                             can be passed to openSIL. The host firmware is responsible
 *                             for initializing the SIL_CONTEXT structure.
 *  @param    GroupID          GroupId of Apob entry
 *  @param    DataTypeID       DataTypeID of Apob entry
 *  @param    NumofEntry       Number of entry found of specific GroupId, DataTypeId
 *  @param    ApobBaseAddress  location of the APOB optional (Deprecated)
 *  @param    ApobEntries      Point to the array which used to hold the ApobEntries
 *                             The array need be allocated outside of the routine
 *
 *  @retval       SilPass           Function succeed
 *  @retval       NON-ZERO          Error occurs
 */
static
SIL_STATUS
AmdGetApobEntry (
  SIL_CONTEXT       *SilContext,
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

  UNUSED(ApobBaseAddress);

  *NumofEntry = 0;
  *ApobEntries = NULL;
  ApobEntryBin = NULL;
  APOB_TRACEPOINT(SIL_TRACE_INFO, "AmdGetApobEntry %x %x\n", GroupID, DataTypeID);

  if ((SilContext->ApobBaseAddress != 0) && (SilContext->ApobBaseAddress != 0xFFFFFFFF)) {
    ApobHeaderPtr = (APOB_BASE_HEADER *) (uintptr_t) SilContext->ApobBaseAddress;
  } else {
    APOB_TRACEPOINT(SIL_TRACE_INFO, "SilContext APOB base address is wrong\n");
    return SilUnsupported;
  }

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
 * @param   SilContext       A context structure through which host firmware defined data
 *                           can be passed to openSIL. The host firmware is responsible
 *                           for initializing the SIL_CONTEXT structure.
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
  SIL_CONTEXT       *SilContext,
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
  AmdGetApobEntry(SilContext,
    GroupID,
    DataTypeID,
    &LclNumofEntry,
    ApobBaseAddress,
    &LclApobEntries[0]
    );
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

  //Instance ID can't be duplicated
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
