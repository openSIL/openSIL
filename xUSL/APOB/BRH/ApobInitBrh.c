/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  ApobInitBrh.c
 * @brief OpenSIL APOB functions
 *
 */

#include <SilCommon.h>
#include <APOB/Common/ApobCmn.h>
#include <string.h>
#include <Apob-BRH.h>
#include <ApobCmn2Brh.h>
#include <ApobInitBrh.h>

/**
 * ApobGetPhysCcdNumberBrh
 * @brief Get the physical CCD number by reading APOB APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE
 *
 *  @param[in] ApobInstanceId
 *  @param[in] CcxIndex used access ComplexMap
 *  @param[in, out] PhysComplexNumber
 *
 *  @retval       SilPass           Function succeed
 *  @retval       NON-ZERO          Error occurs
 */
SIL_STATUS
ApobGetPhysCcdNumberBrh (
  uint32_t ApobInstanceId,
  uint32_t CcdIndex,
  uint8_t *PhysCcdNumber
  )
{
  SIL_STATUS                                   Status;
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT *ApobEntry;

  Status = AmdGetApobEntryInstance(APOB_CCX,
    APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE,
    ApobInstanceId,
    0,
    (APOB_TYPE_HEADER **) &ApobEntry
    );

  if (Status == SilPass) {
    *PhysCcdNumber = ApobEntry->CcdMap[CcdIndex].PhysCcdNumber;
  }
  return Status;
}

/**
 * ApobGetPhysComplexNumberBrh
 * @brief Get the physical complex number by reading APOB APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE
 *
 *  @param[in] ApobInstanceId
 *  @param[in] CcxIndex used to access ComplexMap
 *  @param[in, out] PhysComplexNumber
 *
 *  @retval       SilPass           Function succeed
 *  @retval       NON-ZERO          Error occurs
 */
SIL_STATUS
ApobGetPhysComplexNumberBrh (
  uint32_t ApobInstanceId,
  uint32_t CcdIndex,
  uint32_t CcxIndex,
  uint8_t *PhysComplexNumber
  )
{
  SIL_STATUS                                   Status;
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT *ApobEntry;

  Status = AmdGetApobEntryInstance(APOB_CCX,
    APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE,
    ApobInstanceId,
    0,
    (APOB_TYPE_HEADER **) &ApobEntry
    );

  if (Status == SilPass) {
    *PhysComplexNumber = ApobEntry->CcdMap[CcdIndex].ComplexMap[CcxIndex].PhysComplexNumber;
  }
  return Status;
}

/**
 * ApobGetPhysCoreNumberBrh
 * @brief Get the physical core number by reading
 *        APOB APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE
 *
 *  @param[in] ApobInstanceId
 *  @param[in] CcxIndex             Index to access ComplexMap
 *  @param[in] CoreIndex            Index to access CoreInfo
 *  @param[in, out] PhysCoreNumber
 *
 *  @retval       SilPass           Function succeed
 *  @retval       NON-ZERO          Error occurs
 */
SIL_STATUS
ApobGetPhysCoreNumberBrh (
  uint32_t ApobInstanceId,
  uint32_t CcdIndex,
  uint32_t CcxIndex,
  uint32_t CoreIndex,
  uint8_t *PhysCoreNumber
  )
{
  SIL_STATUS                                   Status;
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT *ApobEntry;

  Status = AmdGetApobEntryInstance(APOB_CCX,
    APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE,
    ApobInstanceId,
    0,
    (APOB_TYPE_HEADER **) &ApobEntry
    );

  if (Status == SilPass) {
    *PhysCoreNumber = ApobEntry->CcdMap[CcdIndex].
      ComplexMap[CcxIndex].CoreInfo[CoreIndex].PhysCoreNumber;
  }

  return Status;
}

/**
 * ApobGetIsThreadEnabledBrh
 * @brief Check if the thread is enable by reading
 *        APOB APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE
 *
 *  @param[in] ApobInstanceId
 *  @param[in] CcxIndex             Index to access ComplexMap
 *  @param[in] CoreIndex            Index to access CoreInfo
 *  @param[in] ThreadIndex          Index to access IsThreadEnabled
 *  @param[in, out] IsThreadEnabled
 *
 *  @retval       SilPass           Function succeed
 *  @retval       NON-ZERO          Error occurs
 */
SIL_STATUS
ApobGetIsThreadEnabledBrh (
  uint32_t ApobInstanceId,
  uint32_t CcdIndex,
  uint32_t CcxIndex,
  uint32_t CoreIndex,
  uint32_t ThreadIndex,
  bool *IsThreadEnabled
  )
{
  SIL_STATUS                                   Status;
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT *ApobEntry;

  Status = AmdGetApobEntryInstance(APOB_CCX,
    APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE,
    ApobInstanceId,
    0,
    (APOB_TYPE_HEADER **) &ApobEntry
    );

  if (Status == SilPass) {
    *IsThreadEnabled =
      ApobEntry->CcdMap[CcdIndex].ComplexMap[CcxIndex].
      CoreInfo[CoreIndex].IsThreadEnabled[ThreadIndex];
  }
  return Status;
}

/**
 * ApobGetSubProgramBrh
 * @brief Get SubProgram through read APOB_GEN_CONFIGURATION_INFO_TYPE.SubProgram
 *
 * @param SubProgram Point to the address to hold SubProgram
 *
 *  @retval       SilPass           Function succeed
 *  @retval       NON-ZERO          Error occurs
 */
SIL_STATUS
ApobGetSubProgramBrh (
  uint32_t *SubProgram
  )
{
  SIL_STATUS                       Status;
  APOB_GEN_INFO_TYPE_STRUCT        *ApobEntry;

  Status = AmdGetApobEntryInstance(APOB_GEN,
    APOB_GEN_CONFIGURATION_INFO_TYPE,
    0,
    0,
    (APOB_TYPE_HEADER **) &ApobEntry
    );

  if (Status == SilPass) {
    *SubProgram = ApobEntry->ApobParamInfo.SubProgram;
  }

  return Status;
}

/**
 * ApobGetDimmSpdDataBrh
 * @brief Get specific SPD data by reading APOB APOB_MEM_DIMM_SPD_DATA_TYPE
 *
 * @param ApobInstanceId
 * @param Socket
 * @param Channel
 * @param Dimm
 * @param BufSize
 * @param SpdBufPtr Point to buffer to hold SPD data
 *
 * @retval       EFI_SUCCESS       Function succeed
 * @retval       NON-ZERO          Error occurs
 */
SIL_STATUS
ApobGetDimmSpdDataBrh (
  uint32_t ApobInstanceId,
  uint8_t  Socket,
  uint8_t  Channel,
  uint8_t  Dimm,
  uint32_t BufSize,
  uint8_t  *SpdBufPtr
  )
{
  uint8_t                          Index;
  uint32_t                         IndexCount;
  uint32_t                         TypeSize;
  SIL_STATUS                       Status;
  APOB_TYPE_HEADER                 *ApobTypeHeader;
  APOB_MEM_DIMM_D5_SPD_DATA_STRUCT *ApobEntry;


  Status = AmdGetApobEntryInstance(APOB_MEM,
    APOB_MEM_DIMM_SPD_DATA_TYPE,
    ApobInstanceId,
    0,
    (APOB_TYPE_HEADER **) &ApobEntry
    );
  ApobTypeHeader = (APOB_TYPE_HEADER *)ApobEntry;
  if (Status == SilPass) {
    TypeSize = ApobTypeHeader->TypeSize;
    IndexCount = (TypeSize - offsetof(APOB_MEM_DIMM_D5_SPD_DATA_STRUCT, DimmSmbusInfo))
      / sizeof (APOB_D5_SPD_STRUCT);
    Status = SilNotFound;
    for (Index = 0; Index < IndexCount; Index++) {
      if (ApobEntry->DimmSmbusInfo[Index].SocketNumber == Socket &&
        ApobEntry->DimmSmbusInfo[Index].ChannelNumber == Channel &&
        ApobEntry->DimmSmbusInfo[Index].DimmNumber == Dimm &&
        ApobEntry->DimmSmbusInfo[Index].DimmPresent) {
        memcpy(SpdBufPtr, ApobEntry->DimmSmbusInfo[Index].Data, (BufSize > 1024) ? 1024 : BufSize);
        Status = SilPass;
      }
    }
  }
  return Status;
}

/**
 * ConvertSocCcdLogToPhysMap
 *
 * @brief retrieves the SoC specific CCD logical to physical map from the APOB
 *
 * @details The SoC specific CCD logical to physical map is converted to a
 *          common map that can be used to support Multi SoC in common APOB service code.
 *
 * @param  CommonCcdLogToPhysMap    Generic Physical Map Structure
 * @param  SocCcdLogToPhysMap       SOC specific Physical Map Structure
 *
 **/
static
void
ConvertSocCcdLogToPhysMap (
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT     *CommonCcdLogToPhysMap,
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT_BRH *SocCcdLogToPhysMap
  )
{
  uint8_t             CcdLoop;
  uint8_t             ComplexLoop;
  uint8_t             CoreLoop;
  uint8_t             ThreadLoop;
  APOB_SOC_DIE_INFO   SocMaxDieInfo;

  ApobGetMaxDieInfoBrh(&SocMaxDieInfo);

  // Copy the header
  memcpy(CommonCcdLogToPhysMap, SocCcdLogToPhysMap, sizeof (APOB_TYPE_HEADER));

  assert(SocMaxDieInfo.MaxSocCcdsPerDieValue <= PROJ_MAX_CCD_DIES_PER_SOCKET);

  // Copy program specific Ccd Logical to Physical Map into a common map.
  for (CcdLoop = 0; CcdLoop < SocMaxDieInfo.MaxSocCcdsPerDieValue; CcdLoop++) {
    CommonCcdLogToPhysMap->CcdMap[CcdLoop].PhysCcdNumber = SocCcdLogToPhysMap->CcdMap[CcdLoop].PhysCcdNumber;

    for (ComplexLoop = 0; ComplexLoop < SocMaxDieInfo.MaxSocCcxPerCcdValue; ComplexLoop++) {
      CommonCcdLogToPhysMap->CcdMap[CcdLoop].ComplexMap[ComplexLoop].PhysComplexNumber =
        SocCcdLogToPhysMap->CcdMap[CcdLoop].ComplexMap[ComplexLoop].PhysComplexNumber;

      for (CoreLoop = 0; CoreLoop < SocMaxDieInfo.MaxSocCoresPerComplexValue; CoreLoop++) {
        CommonCcdLogToPhysMap->CcdMap[CcdLoop].ComplexMap[ComplexLoop].CoreInfo[CoreLoop].PhysCoreNumber =
          SocCcdLogToPhysMap->CcdMap[CcdLoop].ComplexMap[ComplexLoop].CoreInfo[CoreLoop].PhysCoreNumber;

        for (ThreadLoop = 0; ThreadLoop < SocMaxDieInfo.MaxSocThreadPerCore; ThreadLoop++) {
          CommonCcdLogToPhysMap->CcdMap[CcdLoop].ComplexMap[ComplexLoop].
          CoreInfo[CoreLoop].IsThreadEnabled[ThreadLoop] =
            SocCcdLogToPhysMap->CcdMap[CcdLoop].ComplexMap[ComplexLoop].
            CoreInfo[CoreLoop].IsThreadEnabled[ThreadLoop];
        }
      }
    }
  }
}

/**
 * ApobGetCcdLogToPhysMap
 *
 * @brief This retrieves the CCD logical to physical from the APOB.
 *
 * @param  Socket               Zero-based socket number to check.
 * @param  Die                  The target die's identifier within Socket.
 * @param  ApobCcdLogToPhysMap  Pointer to return the CCD Logical to physical
 *                              map.
 *
 * @retval SilPass    The CCD log to phys map was retrieved successfully.
 * @retval NON-ZERO   Error occurs
 *
 **/
SIL_STATUS
ApobGetCcdLogToPhysMapBrh (
  uint32_t                                      Socket,
  uint32_t                                      Die,
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT  *ApobCcdLogToPhysMap
  )
{
  uint32_t                                         ApobInstanceId;
  APOB_TYPE_HEADER                                 *ApobEntry;
  SIL_STATUS                                       Status;
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT_BRH *BrhApobEntry;

  APOB_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  ApobInstanceId = ((uint32_t) Socket << 8) | (uint32_t) Die;
  Status = AmdGetApobEntryInstance(APOB_CCX,
    APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE,
    ApobInstanceId,
    0,
    &ApobEntry
    );

  BrhApobEntry = (APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT_BRH *)ApobEntry;

  if (Status != SilPass) {
    APOB_TRACEPOINT(SIL_TRACE_ERROR,
      "Failed to retrieve Apob Instance (InstanceId = 0x%x)\n",
      ApobInstanceId
      );
    return Status;
  }

  ConvertSocCcdLogToPhysMap(ApobCcdLogToPhysMap, BrhApobEntry);

  APOB_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return Status;
}

/**
 * ApobGetActionOnBistFailure
 * @brief This retrieves information about memory BIST failure for APOB type APOB_APCB_BOOT_INFO_TYPE
 *
 * @param[out] ActionOnBistFailure         BIST Failure Action
 *
 *
 * @return SIL_STATUS                      BIST Failure information status.
 *
 **/
SIL_STATUS
ApobGetActionOnBistFailureBrh (
  uint8_t *ActionOnBistFailure
  )
{
  SIL_STATUS                        Status;
  APOB_APCB_BOOT_INFO_STRUCT        *ApobEntry = NULL;

  Status = AmdGetApobEntryInstance(APOB_MEM,
    APOB_APCB_BOOT_INFO_TYPE,
    0,
    0,
    (APOB_TYPE_HEADER **) &ApobEntry
    );

  if (Status != SilPass)
    return Status;

  APOB_TRACEPOINT(SIL_TRACE_INFO,
    "APCB_TOKEN_UID_ACTION_ON_BIST_FAILURE = 0x%x\n",
    ApobEntry->ActionOnBistFailure
    );

  *ActionOnBistFailure = ApobEntry->ActionOnBistFailure;
  return Status;
}

/**
 * ApobGetMaxCpusBrh
 *
 * @brief   Returns the maximum possible number of CPUs.  The maximum here
 *          represents the absolute maximum supported by any SoC in openSIL.
 *          This value is used to allocate memory for the Cpu map.
 *
 *          When the Host allocate memory for the RasCpuMap buffer, this
 *          function should be used to determine the number of SIL_CPU_INFO
 *          structures that need to be allocated.
 *
 * @return  The maximum number of cpus including logical CPUs (threads).
 */
uint32_t
ApobGetMaxCpusBrh (
  void
  )
{
  uint32_t            MaxBrhCpusValue;
  APOB_SOC_DIE_INFO   SocMaxDieInfo;

  ApobGetMaxDieInfoBrh(&SocMaxDieInfo);

  MaxBrhCpusValue = (uint32_t)( SocMaxDieInfo.MaxSocSocketsSupportedValue *
    SocMaxDieInfo.MaxSocDiesPerSocketValue *
    SocMaxDieInfo.MaxSocCcdsPerDieValue *
    SocMaxDieInfo.MaxSocCcxPerCcdValue *
    SocMaxDieInfo.MaxSocCoresPerComplexValue *
    SocMaxDieInfo.MaxSocThreadPerCore);

  return (MaxBrhCpusValue);
}

/**
 * ApobGetMaxDieInfoBrh
 *
 * @brief By calling this function, the SOC specific Die information
 *        will be updated into the Input structure, which can be exposed to the entire codebase.
 *
 * @param ApobSocDieInfo which hold SOC specific Die information.
 */

void
ApobGetMaxDieInfoBrh (
  APOB_SOC_DIE_INFO *ApobSocDieInfo
  )
{
  ApobSocDieInfo->MaxSocCcdsPerDieValue = MAX_CCDS_PER_DIE;
  ApobSocDieInfo->MaxSocCcxPerCcdValue = MAX_CCX_PER_CCD;
  ApobSocDieInfo->MaxSocCoresPerComplexValue = CCX_MAX_CORES_PER_COMPLEX;
  ApobSocDieInfo->MaxSocDiesPerSocketValue = CCX_MAX_DIES_PER_SOCKET;
  ApobSocDieInfo->MaxSocSocketsSupportedValue = CCX_MAX_SOCKETS;
  ApobSocDieInfo->MaxSocThreadPerCore = MAX_THREADS_PER_CORE;
  ApobSocDieInfo->MaxSocCcdPerSocket = MAX_CCD_PER_SOCKET;
}
