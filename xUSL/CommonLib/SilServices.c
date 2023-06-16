/**
 * @file  SilServices.c
 * @brief OpenSIL services provided by xUSL
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <xSIM-api.h>
#include <SilCommon.h>

/**
 * SilSetMemoryBase
 *
 * @brief This function sets the global variable containing the address of the openSIL memory block
 *
 * @param BaseAddress The base address of the openSIL memory block provided by host firmware
 *
 */
void
SilSetMemoryBase (
  void  *BaseAddress
  )
{
  mSilMemoryBase = BaseAddress; // Set the global var
}

/**
 * SilGetMemoryBase
 *
 * @brief This function returns the base address of the Sil memory block
 *
 * @return void*  The base address of the openSIL memory block
 */
void*
SilGetMemoryBase (void)
{
  return mSilMemoryBase; // Return the global var
}

/**
 * xUslFindStructure
 *
 * @brief This function returns the data block for the specified module (IpId)
 *
 * @param IpId        The SIL_DATA_BLOCK_ID value for the IP block to return.
 * @param InstanceNum The instance of the IP block data. (0 based)
 *
 * @return void*      The pointer to the IP block data
 *
 */
void *
xUslFindStructure (
  SIL_DATA_BLOCK_ID IpId,
  uint16_t          InstanceNum
  )
{
  SIL_BLOCK_VARIABLES     *SilVars;
  SIL_INFO_BLOCK_HEADER   *LclMemPtr;
  SIL_INFO_BLOCK_HEADER   *FreeSpaceBase;

  XUSL_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  SilVars = (SIL_BLOCK_VARIABLES *) mSilMemoryBase;

  /* make local pointer to first assigned Input block */
  LclMemPtr = (SIL_INFO_BLOCK_HEADER *) ((uint8_t *) mSilMemoryBase +
    sizeof(SIL_BLOCK_VARIABLES));

  XUSL_TRACEPOINT (SIL_TRACE_INFO, "Looking for IP block ID %d\n",
    IpId);

  FreeSpaceBase = (SIL_INFO_BLOCK_HEADER *)((uint8_t *)mSilMemoryBase +
    SilVars->FreeSpaceOffset);
  XUSL_TRACEPOINT(SIL_TRACE_INFO,
    "mSilMemoryBase: 0x%x, FreeSpaceOffset: 0x%x, FreeSpaceBase = 0x%x\n",
    mSilMemoryBase,
    SilVars->FreeSpaceOffset,
    FreeSpaceBase
    );

  while ((uintptr_t) LclMemPtr < (uintptr_t) FreeSpaceBase) {
    if ((LclMemPtr->Id == (uint32_t)IpId) &&
      (LclMemPtr->Instance == InstanceNum)) {
      XUSL_TRACEPOINT(SIL_TRACE_EXIT, "FoundStruc.Data @0x%x \n",
        &LclMemPtr->InfoBlockData);
      return &LclMemPtr->InfoBlockData;
    }
    LclMemPtr = (SIL_INFO_BLOCK_HEADER *)
      ((uint8_t *)LclMemPtr + LclMemPtr->InfoBlockDataSize);
  }

  XUSL_TRACEPOINT(SIL_TRACE_EXIT, "NoFindStruc ID:0x%x\n", LclMemPtr->Id);
  return NULL;
};

/**
 * SilInitIp2IpApi
 *
 * @brief   Register an Ip-2-Ip API for IpId to the API table.
 *
 * @param   IpId  The ID of the IP providing the API.
 * @param   Api   Input of the Ip-2-Ip API table to initialize.
 *
 * @retval  SilPass             The Ip-2-Ip API was initialized for IpId.
 * @retval  SilInvalidParameter The Ip ID was outside the bounds of valid IDs defined by SIL_DATA_BLOCK_ID.
 *
 */
SIL_STATUS
SilInitIp2IpApi (
  SIL_DATA_BLOCK_ID   IpId,
  void                *IpApi
  )
{
  SIL_BLOCK_VARIABLES *SilVars;

  XUSL_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  assert (IpId < SilId_ListEnd);
  if (IpId >= SilId_ListEnd) {
    return SilInvalidParameter;
  }

  SilVars = (SIL_BLOCK_VARIABLES *) mSilMemoryBase;

  /*
   * The API table (Ip2IpApi) is a member of the SIL_BLOCK_VARIABLES struct.
   */
  XUSL_TRACEPOINT(SIL_TRACE_INFO, "Initializing Ip-2-Ip API for IP ID = %d\n", IpId);
  SilVars->Ip2IpApi[IpId] = (uintptr_t)IpApi;

  XUSL_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * SilGetIp2IpApi
 *
 * @details This function is used by openSIL IPs to find and return a pointer to the Ip-2-Ip API installed for the
 *          specified IP ID.
 *
 * @param   IpId  The ID of the IP block to request the Ip-2-Ip API for.  The list of
 *                valid Ids is defined in xSIM-api.h.
 * @param   Api   Upon success, this output is a pointer to the Ip-2-Ip API.
 *
 * @returns SIL_STATUS
 * @retval  SilPass     The Api was found and returned in the Api pointer
 * @retval  SilNotFound No API was installed for IP
 *
 */
SIL_STATUS
SilGetIp2IpApi (
  SIL_DATA_BLOCK_ID   IpId,
  void                **Api
  )
{
  SIL_STATUS          Status;
  SIL_BLOCK_VARIABLES *SilVars;

  XUSL_TRACEPOINT (SIL_TRACE_INFO, "Get Ip-2-Ip API for IP ID %d\n", IpId);

  SilVars = (SIL_BLOCK_VARIABLES *) mSilMemoryBase;

  /*
   * The API table (Ip2IpApi) is a member of the SIL_BLOCK_VARIABLES struct.
   */

  Status = SilPass;

  *Api = (void *)((uintptr_t)SilVars->Ip2IpApi[IpId]);
  if (*Api == NULL) {
    Status = SilNotFound;
    XUSL_TRACEPOINT(SIL_TRACE_ERROR, "Ip-2-Ip API for IP ID %d NOT FOUND!\n", IpId);
    assert (*Api != NULL); // Assert if the requested API is null
  }

  return Status;
}

/**
 * SilInitCommon2RevXferTable
 *
 * @brief   Register an IP Common to revision specific transfer table.
 *
 * @details The common to revision specific transfer table is an interface from common code to abstracted program
 *          specific code.
 *
 * @param   IpId        The ID of the IP providing the table.
 * @param   XferTable   Input of the transfar table to initialize.
 *
 * @retval  SilPass             The Xfer table was initialized for IpId.
 * @retval  SilInvalidParameter The Ip ID was outside the bounds of valid IDs defined by SIL_DATA_BLOCK_ID.
 *
 */
SIL_STATUS
SilInitCommon2RevXferTable (
  SIL_DATA_BLOCK_ID   IpId,
  void                *XferTable
  )
{
  SIL_BLOCK_VARIABLES *SilVars;

  XUSL_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  assert (IpId < SilId_ListEnd);
  if (IpId >= SilId_ListEnd) {
    return SilInvalidParameter;
  }

  SilVars = (SIL_BLOCK_VARIABLES *) mSilMemoryBase;

  /*
   * The transfer table (Common2RevXferTable) is a member of the SIL_BLOCK_VARIABLES struct.
   */
  XUSL_TRACEPOINT(SIL_TRACE_INFO, "Initializing transfer table for IP ID = %d\n", IpId);
  SilVars->Common2RevXferTable[IpId] = (uintptr_t)XferTable;

  XUSL_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * SilGetCommon2RevXferTable
 *
 * @brief   Retrieves an IP Common to revision specific transfer table.
 *
 * @details The common to revision specific transfer table is an interface from common code to abstracted program
 *          specific code.
 *
 * @param   IpId        The ID of the IP providing the table.
 * @param   XferTable   Output pointer to return the transfar table.
 *
 * @retval  SilPass     The transfer table was returned for IpId.
 * @retval  SilNotFound No transfer table was installed for the specified IP.
 *
 */
SIL_STATUS
SilGetCommon2RevXferTable (
  SIL_DATA_BLOCK_ID   IpId,
  void                **XferTable
  )
{
  SIL_STATUS          Status;
  SIL_BLOCK_VARIABLES *SilVars;

  XUSL_TRACEPOINT (SIL_TRACE_INFO, "Get transfer table for IP ID %d\n", IpId);

  SilVars = (SIL_BLOCK_VARIABLES *) mSilMemoryBase;

  Status = SilPass;

  /*
   * The API table is a member of the SIL_BLOCK_VARIABLES struct.
   */

  *XferTable = (void *)((uintptr_t)SilVars->Common2RevXferTable[IpId]);
  if (*XferTable == NULL) {
    Status = SilNotFound;
    XUSL_TRACEPOINT(SIL_TRACE_ERROR, "Transfer table for IP ID %d NOT FOUND!\n", IpId);
    assert (*XferTable != NULL); // Assert if the requested API is null
  }

  return Status;
}
