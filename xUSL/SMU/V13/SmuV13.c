/**
 * @file  SmuV13.c
 * @brief OpenSIL SMU V13 specific initialization
 *
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <xSIM.h>
#include <SilSocLogicalId.h>
#include <SMU/Common/SmuCommon.h>
#include <SMU/Common/SmuCmn2Rev.h>
#include <SMU/SmuIp2Ip.h>
#include <ApobCmn.h>
#include <CoreTopologyService.h>
#include <CommonLib/SmnAccess.h>
#include <NBIO/IOD/include/NbioBaseReg.h>
#include <Utils.h>
#include "SmuV13.h"
#include "SmuV13Reg.h"

extern SMU_COMMON_2_REV_XFER_BLOCK mSmuXferV13;

static SMU_IP2IP_API mSmuV13Api = {
  .Header = {
    .IpId         = SilId_SmuClass,
    .IpVersion    = 13,
  },
  .SmuServiceInitArguments  = SmuServiceInitArgumentsCommon,
  .SmuServiceRequest        = SmuServiceRequestV13,
  .SmuFirmwareTest          = SmuFirmwareTestCommon,
  .DxioServiceRequest       = DxioServiceRequestV13,
  .SmuNotifyS3Entry         = SmuNotifyS3EntryCommon,
  .SmuGetGnbHandle          = SmuGetGnbHandleCommon,
  .SmuReadBrandString       = SmuReadBrandStringV13,
  .SmuLaunchThread          = SmuLaunchThreadV13,
  .SmuReadBistInfo          = SmuReadBistInfoV13,
  .SmuReadCacWeights        = SmuReadCacWeightsV13,
  .SmuRegisterRead          = SmuRegisterReadCommon,
  .SmuRegisterWrite         = SmuRegisterWriteCommon,
  .SmuRegisterRMW           = SmuRegisterRMWCommon,
  .SmuGetOpnCorePresence    = SmuGetOpnCorePresenceV13,
  .SmuGetOpnCorePresenceEx  = SmuGetOpnCorePresenceExV13
};

/**
 * DxioServiceRequestV13
 *
 * @brief DXIO firmware request - Common function
 *
 * @param   NbioHandle      Pointer to GNB_HANDLE
 * @param   DxioMBoxRequest DXIO Mailbox Request ID
 * @param   DxioMBoxArg     SMU Argument structure - [0] entry reserved for
 *                          DXIO Mailbox Request ID
 * @param   AccessFlags     See GNB_ACCESS_FLAGS_* definitions
 *
 * @retval  RetVal  Smu Response
 */
SMC_RESULT
DxioServiceRequestV13 (
  GNB_HANDLE  *NbioHandle,
  uint32_t    DxioMBoxRequest,
  uint32_t    *DxioMBoxArg,
  uint32_t    AccessFlags
  )
{
  return DxioServiceRequestCommon (
    NbioHandle,
    DxioMBoxRequest,
    DxioMBoxArg,
    AccessFlags
    );
}

/**
 * SmuGetOpnCorePresenceExV13
 *
 * @brief This service retrieves information on the OPN's core thread presence with extended functionality.
 *
 * @param SystemDieNumber       Logical system die number to check.
 * @param CcdEnableMap          Pointer to the OPN default enabled CCD map.
 * @param CcdDisableMap         Pointer to the OPN disabled/removed CCD map.
 * @param CoreDisMap            Pointer to the OPN disabled core map.
 * @param CoreDisMapBufferSize  Pointer to a buffer containing the size of the CoreDisMap buffer.
 * @param SmtEnable             Pointer to whether or not the OPN supports SMT.
 * @param SmtEnableBufferSize   Pointer to a buffer containing the size of the SmtEnable buffer.
 *
 * @return SIL_STATUS
 *
 * @retval SilInvalidParameter  Is returned if any of the following conditions are true:
 *                              1. All CcdEnableMap, CcdDisableMap, CoreDisMap, SmtEnable are NULL
 *                              2. CoreDisMap or SmtEnable is NOT NULL, but its corresponding buffer size is.
 *                              3. CoreDisMap or SmtEnable is NOT NULL, but its corresponding size is less than the
 *                                 required size.  In this case, the corresponding buffer size value is set to the
 *                                 required buffer size and returned to the caller.
 */
SIL_STATUS
SmuGetOpnCorePresenceExV13 (
  uint32_t  SystemDieNumber,
  uint32_t  *CcdEnableMap,
  uint32_t  *CcdDisableMap,
  uint32_t  *CoreDisMap,
  uint32_t  *CoreDisMapBufferSize,
  bool      *SmtEnable,
  uint32_t  *SmtEnableBufferSize
  )
{
  SIL_STATUS              Status;
  DFX_FUSE_CCD_DOWN       CcdDownFuse;
  DFX_FUSE_CCD_PRESENT_1  CcdPresentFuse1;
  DFX_CCD_PRESENT_2  CcdPresentFuse2;
  DFX_CORE_FUSE           CoreFuse;
  uint32_t                CcdPresentMap;
  uint32_t                CoreFuseAddress;
  uint32_t                CcdLoop;
  uint32_t                MaxCcdsPerSkt;
  SOC_LOGICAL_ID          CoreId;

  MaxCcdsPerSkt = CCX_MAX_DIES_PER_SOCKET * MAX_CCDS_PER_DIE;

  if ((CcdEnableMap == NULL) && (CcdDisableMap == NULL) && (CoreDisMap == NULL) && (SmtEnable == NULL)) {
    Status = SilInvalidParameter;
  } else if (((CoreDisMap != NULL) && (CoreDisMapBufferSize == NULL)) ||
    ((SmtEnable != NULL) && (SmtEnableBufferSize == NULL))) {
    Status = SilInvalidParameter;
  } else if (((CoreDisMap != NULL) && (*CoreDisMapBufferSize < sizeof (CoreDisMap[0]) * MaxCcdsPerSkt)) ||
    ((SmtEnable != NULL) && (*SmtEnableBufferSize < sizeof (SmtEnable[0]) * MaxCcdsPerSkt))) {
    Status = SilInvalidParameter;
    if (CoreDisMap != NULL) {
      *CoreDisMapBufferSize = sizeof (CoreDisMap[0]) * MaxCcdsPerSkt;
    }
    if (SmtEnable != NULL) {
      *SmtEnableBufferSize = sizeof (SmtEnable[0]) * MaxCcdsPerSkt;
    }
  } else {
    GetSocLogicalIdOnCurrentCore (&CoreId);

    SmuRegisterReadCommon (SystemDieNumber, SIL_RESERVED_807, &CcdPresentFuse1.Value);
    SmuRegisterReadCommon (SystemDieNumber, DFX_CCD_PRESENT_2_ADDR, &CcdPresentFuse2.Value);
    SmuRegisterReadCommon (SystemDieNumber, SIL_RESERVED_811, &CcdDownFuse.Value);
    CcdPresentMap = (CcdPresentFuse2.Field.CcdPresent << SIL_RESERVED_810) |
        CcdPresentFuse1.Field.CcdPresent;
    if (CcdEnableMap != NULL) {
      *CcdEnableMap = CcdPresentMap;
    }

    if (CcdDisableMap != NULL) {
      // Only report the disabled CCDs bits which is present in a max OPN
      *CcdDisableMap = CcdDownFuse.Field.CcdDown & CcdPresentMap;
    }

    if (SmtEnable != NULL) {
      *SmtEnableBufferSize = sizeof (SmtEnable[0]) * MaxCcdsPerSkt;
      for (CcdLoop = 0; CcdLoop < MaxCcdsPerSkt; CcdLoop++) {
        if ((CcdPresentMap & (1 << CcdLoop)) != 0) {
          CoreFuseAddress = CORE_FUSE_ADDR (CoreId.Family, CcdLoop);
          SmuRegisterReadCommon (SystemDieNumber, CoreFuseAddress, &CoreFuse.Value);
          SmtEnable[CcdLoop] = (bool) (CoreFuse.Field.Mp5SmtEnable != 0);
        } else {
          SmtEnable[CcdLoop] = false;
        }
      }
    }

    if (CoreDisMap != NULL) {
      *CoreDisMapBufferSize = sizeof (CoreDisMap[0]) * MaxCcdsPerSkt;
      for (CcdLoop = 0; CcdLoop < MaxCcdsPerSkt; CcdLoop++) {
        if ((CcdPresentMap & (1 << CcdLoop)) != 0) {
          CoreFuseAddress = CORE_FUSE_ADDR (CoreId.Family, CcdLoop);
          SmuRegisterReadCommon (SystemDieNumber, CoreFuseAddress, &CoreFuse.Value);
        } else {
          CoreFuse.Value = SIL_RESERVED_808 | MP5_CORE_DISABLE_CCX1_MASK;
        }
        CoreDisMap[CcdLoop] = CORE_DIS_MAP(CoreId.Family, CcdLoop, CoreFuse.Value);
      }
    }

    Status = SilPass;
  }

  return Status;
}

/**
 * SmuGetOpnCorePresenceV13
 * @brief This service retrieves information on the OPN's core thread presence.
 *
 * @param[in]  SystemDieNumber                Logical system die number to check.
 * @param[out] HwPresentCcdMap                Pointer to the OPN default enabled CCD map.
 * @param[out] SwRemovedCcdMap                Pointer to the OPN disabled/removed CCD map.
 * @param[out] CoreDisMap                     Pointer to the OPN disabled core map.
 * @param[out] SmtEnable                      Pointer to whether or not the OPN supports SMT.
 *
 * @retval SilPass                            Presence info was successfully retrieved.
 */
SIL_STATUS
SmuGetOpnCorePresenceV13 (
  uint32_t SystemDieNumber,
  uint32_t *HwPresentCcdMap,
  uint32_t *SwRemovedCcdMap,
  uint32_t *CoreDisMap,
  bool     *SmtEnable
  )
{
  SIL_STATUS      Status;
  uint32_t        CoreDisMapArray[CCX_MAX_DIES_PER_SOCKET * MAX_CCDS_PER_DIE];
  bool            SmtEnableArray[CCX_MAX_DIES_PER_SOCKET * MAX_CCDS_PER_DIE];
  uint32_t        CoreDisMapArraySize = sizeof (CoreDisMapArray);
  uint32_t        SmtEnableArraySize = sizeof (SmtEnableArray);
  uint32_t        CcdNum;
  uint32_t        CurrDisCoresCount;
  uint32_t        DisCoresCount;
  uint32_t        CurrDisCoresMap;
  uint32_t        MaxCcdsPerSkt;

  MaxCcdsPerSkt = CCX_MAX_DIES_PER_SOCKET * MAX_CCDS_PER_DIE;

  if ((HwPresentCcdMap == NULL) &&
      (SwRemovedCcdMap == NULL) &&
      (CoreDisMap == NULL)      &&
      (SmtEnable == NULL)) {
    SMU_TRACEPOINT (SIL_TRACE_ERROR, "Not all pointer arguments should be NULL.\n");
    assert (false);
  }

  Status = SmuGetOpnCorePresenceExV13 (SystemDieNumber,
                                      HwPresentCcdMap,
                                      SwRemovedCcdMap,
                                      CoreDisMapArray,
                                      &CoreDisMapArraySize,
                                      SmtEnableArray,
                                      &SmtEnableArraySize
                                      );


  if (CoreDisMap != NULL) {
    *CoreDisMap = 0;
    DisCoresCount = 0;
    for (CcdNum = 0; CcdNum < MaxCcdsPerSkt; CcdNum++) {
      // With Asymmetric Downcoring, some CCD, usually the highest-numbered, can have more cores
      // disabled than others. Determine the highest number of disabled cores on a CCD
      CurrDisCoresMap = CoreDisMapArray[CcdNum];
      CurrDisCoresCount = xUslGetSetBitCount (CurrDisCoresMap);
      if (CurrDisCoresCount > DisCoresCount) {
        *CoreDisMap = CurrDisCoresMap;
        DisCoresCount = CurrDisCoresCount;
      }
    }
  }

  if (SmtEnable != NULL) {
    *SmtEnable = false;
    for (CcdNum = 0; CcdNum < MaxCcdsPerSkt; CcdNum++) {
      if (SmtEnableArray[CcdNum] == true) {
        *SmtEnable = true;
        break;
      }
    }
  }

  return Status;
}

/**
 * SmuReadBrandStringV13
 *
 * @brief Returns the brand string read from SMN space.
 *
 * @param   InstanceId        The unique identifier of the SMU instance
 *                            associated with this socket
 * @param   BrandStringLength Number of characters to be read.  This value does
 *                            NOT have to be a multiple of 4.
                              NBIO code will be responsible for alignment,
                              endianness, and string length.
 * @param   BrandString       Pointer to a CHAR array to store the brand
                              string.
 *
 * @return SIL_STATUS
 **/
SIL_STATUS
SmuReadBrandStringV13 (
  uint32_t InstanceId,
  uint32_t BrandStringLength,
  uint8_t  *BrandString
  )
{
  return SmuReadBrandStringCommon (InstanceId, BrandStringLength, BrandString);
}

/**
 * SmuLaunchThreadV13
 *
 * @brief This will start a core to fetch its first instructions from the reset
 * vector.  This service may only be called from the BSP.
 *
 * @param   Socket          Zero-based socket number of the target thread.
 * @param   Die             Zero-based die number within Socket of the target
 *                          thread.
 * @param   LogicalCcd      Zero-based logical core complex die of the target
 *                          thread.
 * @param   LogicalComplex  Zero-based logical complex number of the target
 *                          thread.
 * @param   LogicalCore     Zero-based logical core number of the target
 *                          thread.
 * @param   LogicalThread   Zero-based logical thread number of the target
 *                          thread.
 *
 * @retval SilPass              The thread was successfully launched.
 * @retval SilDeviceError       The thread has already been launched.
 * @retval SilInvalidParameter  Socket is non-existent.
 * @retval SilInvalidParameter  Die is non-existent.
 * @retval SilInvalidParameter  LogicalComplex is non-existent.
 * @retval SilInvalidParameter  LogicalThread is non-existent.
 *
 **/
SIL_STATUS
SmuLaunchThreadV13 (
  uint32_t Socket,
  uint32_t Die,
  uint32_t LogicalCcd,
  uint32_t LogicalComplex,
  uint32_t LogicalCore,
  uint32_t LogicalThread
  )
{
  SIL_STATUS           Status;
  GNB_HANDLE           *GnbHandle;
  SIL_RESERVED3_1510 ThreadEnable;
  uint32_t             NumberOfCcds;
  uint32_t             NumberOfComplexes;
  uint32_t             NumberOfCores;
  uint32_t             NumberOfLogicalThreads;
  uint32_t             ApobInstanceId;
  uint32_t             WhichBit;
  uint8_t              PhysCcdNumber;
  uint8_t              PhysComplexNumber;
  uint8_t              PhysCoreNumber;
  bool                 IsThreadEnabled;

  SMU_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  Status = SmuGetGnbHandleCommon (Socket, &GnbHandle);
  if (GnbHandle != NULL) {
    ApobInstanceId = ((uint32_t) Socket << 8) | (uint32_t) Die;

    ApobGetPhysCcdNumber (ApobInstanceId, LogicalCcd, &PhysCcdNumber);
    if ((LogicalCcd >= MAX_CCDS_PER_DIE) ||
      (PhysCcdNumber == CCX_NOT_PRESENT)) {
      return SilInvalidParameter;
    }

    ApobGetPhysComplexNumber (ApobInstanceId,
        LogicalCcd,
        LogicalComplex,
        &PhysComplexNumber);

    if ((LogicalComplex >= MAX_COMPLEXES_PER_CCD) ||
      (PhysComplexNumber == CCX_NOT_PRESENT)) {
      return SilInvalidParameter;
    }

    ApobGetPhysCoreNumber (ApobInstanceId,
      LogicalCcd,
      LogicalComplex,
      LogicalCore,
      &PhysCoreNumber);
    if ((LogicalCore >= MAX_CORES_PER_COMPLEX) ||
      (PhysCoreNumber == CCX_NOT_PRESENT)) {
      return SilInvalidParameter;
    }

    ApobGetIsThreadEnabled (ApobInstanceId,
      LogicalCcd,
      LogicalComplex,
      LogicalCore,
      LogicalThread,
      &IsThreadEnabled);
    if ((LogicalThread >= MAX_THREADS_PER_CORE) ||
      IsThreadEnabled == false) {
      return SilInvalidParameter;
    }

    Status = GetCoreTopologyOnDie (Socket,
      Die,
      &NumberOfCcds,
      &NumberOfComplexes,
      &NumberOfCores,
      &NumberOfLogicalThreads);

    if (Status == SilPass) {
      WhichBit = LogicalComplex * 16;
      WhichBit += (LogicalCore * NumberOfLogicalThreads) + LogicalThread;

      if (PhysCcdNumber < 8) {
        ThreadEnable.Value =
          xUSLSmnRead (0,
            GnbHandle->Address.Address.Bus,
            (SIL_RESERVED2_1293 +
              (uint32_t)(PhysCcdNumber << 25))
            );
      } else {
        ThreadEnable.Value =
          xUSLSmnRead (0,
            GnbHandle->Address.Address.Bus,
            (SIL_RESERVED2_1294 +
              (uint32_t)((PhysCcdNumber - 8) << 25))
            );
      }

      if ((ThreadEnable.Field.ThreadEn & (1 << WhichBit)) == 0) {
        ThreadEnable.Field.ThreadEn |= 1 << WhichBit;
        if (PhysCcdNumber < 8) {
          xUSLSmnWrite (
            0,
            GnbHandle->Address.Address.Bus,
            (SIL_RESERVED2_1293 +
              (uint32_t)(PhysCcdNumber << 25)),
            ThreadEnable.Value
            );
        } else {
          xUSLSmnWrite (
            0,
            GnbHandle->Address.Address.Bus,
            (SIL_RESERVED2_1294 +
              (uint32_t)((PhysCcdNumber - 8) << 25)),
            ThreadEnable.Value
            );
        }
      } else {
        Status = SilDeviceError;
      }
    }
  } else {
    Status = SilInvalidParameter;
  }

  SMU_TRACEPOINT (SIL_TRACE_EXIT, "Status:0x%X\n", Status);

  return Status;
}

/**
 * SmuReadBistInfoV13
 *
 * @brief Retrieve BIST info from SMU
 *
 * @param   InstanceId  The unique identifier of the SMU instance associated
 *                      with this socket
 * @param   PhysicalCcx Number of the physical CCX
 * @param   BistInfo    Pointer to uint32_t where BIST info should be copied.
 *                      Caller should allocate the memory for BistData.
 *
 * @return  SIL_STATUS
 *
 * @retval  SilNotFound         If unable to make SMU call.
 * @retval  SilInvalidParameter If null pointer passed in
 * @retval  SilPass             If call succeded and return argument
 *                              is populated. Returns SilUnsupported for SMU
 *                              that do not support this call.
 *
 **/
SIL_STATUS
SmuReadBistInfoV13 (
  uint8_t       InstanceId,
  uint8_t       PhysicalCcx,
  uint32_t      *BistInfo
  )
{
  return SmuReadBistInfoCommon (InstanceId, PhysicalCcx, BistInfo);
}

/**
 * SmuReadCacWeightsV13
 *
 * @brief Returns the CAC weights read from SMU
 *
 * @param   MaxNumWeights The number of values to read from the SMU (defines
 *                        the maximum size of ApmWeights return data buffer)
 * @param   ApmWeights    Pointer to return data buffer
 *
 * @retval  SilPass             ApmWeights contains the CaC weights read from
 *                              SMU
 * @retval  SilInvalidParameter One of the input params was invalid or
 *                              InstanceId did not reference a valid NBIO
 *                              instance
 * @retval  SilUnsupported      This function is not supported by this version
 *                              of the driver
 *
 */
SIL_STATUS
SmuReadCacWeightsV13 (
  uint32_t MaxNumWeights,
  uint64_t *ApmWeights
  )
{
  UNUSED(MaxNumWeights);
  UNUSED(ApmWeights);

  return SilUnsupported;
}

/**
 * InitializeApiV13
 *
 * @brief   Initialize internal and external APIs for SMU V13
 *
 * @retval  SilPass             API initialized successfully
 * @retval  SilInvalidParameter Id class is invalid
 *
 */
SIL_STATUS
InitializeSmuApiV13 (void)
{
  SIL_STATUS  Status;

  // Initialize Common to Rev specific transfer table first
  Status = SilInitCommon2RevXferTable (SilId_SmuClass, (void *)&mSmuXferV13);
  if (Status != SilPass) {
    return Status;
  }

  // Initialize SMU IP-to-IP API
  return SilInitIp2IpApi (SilId_SmuClass, (void *)&mSmuV13Api);
}
