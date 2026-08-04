/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file SmuInitPhx.c
 * @brief PHX SMU core silicon initialization code.
 *
 */

#include <SilCommon.h>
#include <CCX/CcxClass-api.h>
#include <FCH/FchClass-api.h>
#include <SMU/Common/SmuCommon.h>
#include <SMU/SmuClass-api.h>
#include <Utils.h>
#include <CpuLib.h>
#include <string.h>
#include <CoreTopologyService.h>
#include <APOB/Common/ApobCmn.h>
#include <ProjSocConst.h>
#include <APOB/ApobIp2Ip.h>
#include <Mpio/Common/MpioStructs.h>
#include <Nbio/Phx/includePHX/PHX_SYSHUBMM.h>
#include "SmuCmn2Phx.h"
#include "SmuPhxReg.h"
#include "SmuPhxIp2Ip.h"
#include "SmuInitPhx.h"
#include "SmuPhx.h"

/**
 * SmuGetOpnCorePresenceExPhx
 *
 * @brief This service retrieves information on the OPN's core thread presence with extended functionality.
 *
 * @param SystemDieNumber       Logical system die number to check.
 * @param CcdEnableMap          Pointer to the OPN default enabled CCD map.
 * @param CcdDisableMap         Pointer to the OPN disabled/removed CCD map.
 * @param CoreDisMap            Pointer to the OPN disabled core map.
 * @param CoreDisMapBufferSize  Pointer to a buffer containing the size of the CoreDisMap buffer.
 * @param CoreDisMapLength      Pointer to the OPN disabled core map length in bits.
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
SmuGetOpnCorePresenceExPhx (
  uint32_t  SystemDieNumber,
  uint32_t  *CcdEnableMap,
  uint32_t  *CcdDisableMap,
  uint32_t  *CoreDisMap,
  uint32_t  *CoreDisMapBufferSize,
  uint32_t  *CoreDisMapLength,
  bool      *SmtEnable,
  uint32_t  *SmtEnableBufferSize
  )
{
  return SilInvalidParameter;
}

/**
 * SmuGetOpnCorePresencePhx
 * @brief This service retrieves information on the OPN's core thread presence.
 *
 * @param SilContext        A context structure through which host firmware defined data
 *                          can be passed to openSIL. The host firmware is responsible
 *                          for initializing the SIL_CONTEXT structure.
 * @param SystemDieNumber   Logical system die number to check.
 * @param CcdEnableMap      Pointer to the OPN default enabled CCD map.
 * @param CcdDisableMap     Pointer to the OPN disabled/removed CCD map.
 * @param CoreDisMap        Pointer to the OPN disabled core map.
 * @param CoreDisMapLength  Pointer to the OPN disabled core map length in bits.
 * @param SmtEnable         Pointer to whether or not the OPN supports SMT.
 *
 * @retval SilPass                            Presence info was successfully retrieved.
 */
SIL_STATUS
SmuGetOpnCorePresencePhx (
  SIL_CONTEXT    *SilContext,
  uint32_t       SystemDieNumber,
  uint32_t       *CcdEnableMap,
  uint32_t       *CcdDisableMap,
  uint32_t       *CoreDisMap,
  uint32_t       *CoreDisMapLength,
  bool           *SmtEnable
  )
{
  SIL_STATUS      Status;
  uint32_t        CoreFuse;

  if ((CcdEnableMap == NULL) &&
    (CcdDisableMap == NULL) &&
    (CoreDisMap == NULL) &&
    (SmtEnable == NULL)) {
    SMU_TRACEPOINT(SIL_TRACE_ERROR, "Not all pointer arguments should be NULL.\n");
    assert(false);
  }

  if (CcdEnableMap != NULL) {
    // Enable virtual CCD 0
    *CcdEnableMap = 0x00000001;
  }
  if (CcdDisableMap != NULL) {
    *CcdDisableMap = 0x00000000;
  }

  SmuRegisterReadDie(SilContext, SystemDieNumber, 0x5D528, &CoreFuse);
  if (SmtEnable != NULL) {
    *SmtEnable = (bool) ((CoreFuse & BIT_32(6)) != 0);
  }
  if (CoreDisMap != NULL) {
    *CoreDisMap = (CoreFuse >> 7) & 0xFF;
  }
  if (CoreDisMapLength) {
    *CoreDisMapLength = 8;
  }

  Status = SilPass;

  return Status;
}

/**
 * SmuServiceRequestPhx
 *
 * @brief   SMU service request for PHX
 *
 * @param   PciAddress      PCI_ADDR of this NBIO
 * @param   RequestId       BIOS to SMU Message ID
 * @param   RequestArgument Request Argument
 * @param   AccessFlags     See GNB_ACCESS_FLAGS_* definitions
 *
 * @retval  SMC_RESULT
 */
SMC_RESULT
SmuServiceRequestPhx (
  PCI_ADDR PciAddress,
  uint32_t RequestId,
  uint32_t *RequestArgument,
  uint32_t AccessFlags
  )
{
  uint32_t SmuMessageResponse;

  SMU_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (RequestId > SMC_Message_Count_PHX) {
    SMU_TRACEPOINT(SIL_TRACE_ERROR, "SmuServiceRequest INVALID!!.\n");
    return SMC_Result_OK;
  }

  SMU_TRACEPOINT(SIL_TRACE_INFO, "Service Request 0x%x\n", RequestId);
  SMU_TRACEPOINT(SIL_TRACE_INFO,
    "Service Request Argument 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n",
    RequestArgument[0],
    RequestArgument[1],
    RequestArgument[2],
    RequestArgument[3],
    RequestArgument[4],
    RequestArgument[5]
    );

  xUSLIndirectPciRead32(PciAddress.AddressValue, MP1MMIOx3B10578, &SmuMessageResponse);

  // 1 Clear Response
  SmuMessageResponse = 0;
  xUSLIndirectPciWrite32(PciAddress.AddressValue, MP1MMIOx3B10578, SmuMessageResponse);

  // 2 Write message arguments
  xUSLIndirectPciWrite32(PciAddress.AddressValue, MP1MMIOx3B10998, RequestArgument[0]);
  xUSLIndirectPciWrite32(PciAddress.AddressValue, MP1MMIOx3B1099C, RequestArgument[1]);
  xUSLIndirectPciWrite32(PciAddress.AddressValue, MP1MMIOx3B109A0, RequestArgument[2]);
  xUSLIndirectPciWrite32(PciAddress.AddressValue, MP1MMIOx3B109A4, RequestArgument[3]);
  xUSLIndirectPciWrite32(PciAddress.AddressValue, MP1MMIOx3B109A8, RequestArgument[4]);
  xUSLIndirectPciWrite32(PciAddress.AddressValue, MP1MMIOx3B109AC, RequestArgument[5]);

  // 3 Write message ID
  xUSLIndirectPciWrite32(PciAddress.AddressValue, MP1MMIOx3B10528, (uint32_t)RequestId);

  // 4 Poll Response until non-zero
  SMU_TRACEPOINT(SIL_TRACE_INFO, "Poll SMU Message Response until non-zero!!\n");
  do {
    xUSLIndirectPciRead32(PciAddress.AddressValue, MP1MMIOx3B10578, &SmuMessageResponse);
  } while (SmuMessageResponse == 0x0);
  SMU_TRACEPOINT(SIL_TRACE_INFO, "After SMU Message Responded!! (%x)\n", SmuMessageResponse);

  if ((SMC_RESULT)SmuMessageResponse == SMC_Result_Fatal) {
    assert((SMC_RESULT)SmuMessageResponse != SMC_Result_Fatal);
    return SMC_Result_Fatal;
  }

  // 5 Read updated SMU message arguments
  xUSLIndirectPciRead32(PciAddress.AddressValue, MP1MMIOx3B10998, &RequestArgument[0]);
  xUSLIndirectPciRead32(PciAddress.AddressValue, MP1MMIOx3B1099C, &RequestArgument[1]);
  xUSLIndirectPciRead32(PciAddress.AddressValue, MP1MMIOx3B109A0, &RequestArgument[2]);
  xUSLIndirectPciRead32(PciAddress.AddressValue, MP1MMIOx3B109A4, &RequestArgument[3]);
  xUSLIndirectPciRead32(PciAddress.AddressValue, MP1MMIOx3B109A8, &RequestArgument[4]);
  xUSLIndirectPciRead32(PciAddress.AddressValue, MP1MMIOx3B109AC, &RequestArgument[5]);

  SMU_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return (SMC_RESULT)SmuMessageResponse;
}

/**
 * SmuServiceRequestOnlyPhx
 *
 * @brief   SMU service request for PHX
 *
 * @details This function differs from SmuServiceRequestRs in that is send the SMU request but does not wait for
 *          a response.
 *
 * @param   PciAddress      PCI_ADDR of this NBIO
 * @param   RequestId       BIOS to SMU Message ID
 * @param   RequestArgument Request Argument
 * @param   AccessFlags     See GNB_ACCESS_FLAGS_* definitions
 *
 * @retval  SMC_RESULT
 */
SMC_RESULT
SmuServiceRequestOnlyPhx (
  PCI_ADDR PciAddress,
  uint32_t RequestId,
  uint32_t *RequestArgument,
  uint32_t AccessFlags
  )
{
  uint32_t SmuMessageResponse;

  SMU_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (RequestId > SMC_Message_Count_PHX) {
    SMU_TRACEPOINT(SIL_TRACE_ERROR, "SmuServiceRequest INVALID!!.\n");
    return SMC_Result_OK;
  }

  SMU_TRACEPOINT(SIL_TRACE_INFO, "Service Request 0x%x\n", RequestId);
  SMU_TRACEPOINT(SIL_TRACE_INFO,
    "Service Request Argument 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\n",
    RequestArgument[0],
    RequestArgument[1],
    RequestArgument[2],
    RequestArgument[3],
    RequestArgument[4],
    RequestArgument[5]
    );

  xUSLIndirectPciRead32(PciAddress.AddressValue, MP1MMIOx3B10578, &SmuMessageResponse);

  // 1 Clear Response
  SmuMessageResponse = 0;
  if (AccessFlags) {
    xUSLIndirectPciWrite32(PciAddress.AddressValue, MP1MMIOx3B10578, SmuMessageResponse);

    // 2 Write message arguments
    xUSLIndirectPciWrite32(PciAddress.AddressValue, MP1MMIOx3B10998, RequestArgument[0]);
    xUSLIndirectPciWrite32(PciAddress.AddressValue, MP1MMIOx3B1099C, RequestArgument[1]);
    xUSLIndirectPciWrite32(PciAddress.AddressValue, MP1MMIOx3B109A0, RequestArgument[2]);
    xUSLIndirectPciWrite32(PciAddress.AddressValue, MP1MMIOx3B109A4, RequestArgument[3]);
    xUSLIndirectPciWrite32(PciAddress.AddressValue, MP1MMIOx3B109A8, RequestArgument[4]);
    xUSLIndirectPciWrite32(PciAddress.AddressValue, MP1MMIOx3B109AC, RequestArgument[5]);

    // 3 Write message ID
    xUSLIndirectPciWrite32(PciAddress.AddressValue, MP1MMIOx3B10528, (uint32_t)RequestId);
  } else {
    // 4 Poll Response until non-zero
    do {
      xUSLIndirectPciRead32(PciAddress.AddressValue, MP1MMIOx3B10578, &SmuMessageResponse);
      SMU_TRACEPOINT(SIL_TRACE_INFO,
        "Poll SMU Message Response until non-zero!! Current SMU Message Response 0x%x\n",
        SmuMessageResponse
        );
    } while (SmuMessageResponse == 0x0);
    SMU_TRACEPOINT(SIL_TRACE_INFO, "After SMU Message Responded!!\n");

    if ((SMC_RESULT)SmuMessageResponse == SMC_Result_Fatal) {
      assert((SMC_RESULT)SmuMessageResponse != SMC_Result_Fatal);
      return SMC_Result_Fatal;
    }

    // 5 Read updated SMU message arguments
    xUSLIndirectPciRead32(PciAddress.AddressValue, MP1MMIOx3B10998, &RequestArgument[0]);
    xUSLIndirectPciRead32(PciAddress.AddressValue, MP1MMIOx3B1099C, &RequestArgument[1]);
    xUSLIndirectPciRead32(PciAddress.AddressValue, MP1MMIOx3B109A0, &RequestArgument[2]);
    xUSLIndirectPciRead32(PciAddress.AddressValue, MP1MMIOx3B109A4, &RequestArgument[3]);
    xUSLIndirectPciRead32(PciAddress.AddressValue, MP1MMIOx3B109A8, &RequestArgument[4]);
    xUSLIndirectPciRead32(PciAddress.AddressValue, MP1MMIOx3B109AC, &RequestArgument[5]);
  }

  SMU_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return (SMC_RESULT)SmuMessageResponse;
}


/**
 * SmuNotifyS3EntryPhx
 *
 * @brief SMU S3 Entry Notification
 *
 */
void
SmuNotifyS3EntryPhx (void)
{
  PCI_ADDR PciAddress;
  uint32_t RequestId;
  uint32_t RxSmuMessageResponse;
  PciAddress.AddressValue = MAKE_SBDFO(0, 0, 0, 0, 0);
  xUSLIndirectPciRead32(PciAddress.AddressValue, MP1MMIOx3B10578, &RxSmuMessageResponse);

  // 1 Clear Response
  RxSmuMessageResponse = 0;
  xUSLIndirectPciWrite32(PciAddress.AddressValue, MP1MMIOx3B10578, RxSmuMessageResponse);

  // 2 Clear Message Arguments
  xUSLIndirectPciWrite32(PciAddress.AddressValue, MP1MMIOx3B10998, RxSmuMessageResponse);
  xUSLIndirectPciWrite32(PciAddress.AddressValue, MP1MMIOx3B1099C, RxSmuMessageResponse);
  xUSLIndirectPciWrite32(PciAddress.AddressValue, MP1MMIOx3B109A0, RxSmuMessageResponse);
  xUSLIndirectPciWrite32(PciAddress.AddressValue, MP1MMIOx3B109A4, RxSmuMessageResponse);
  xUSLIndirectPciWrite32(PciAddress.AddressValue, MP1MMIOx3B109A8, RxSmuMessageResponse);
  xUSLIndirectPciWrite32(PciAddress.AddressValue, MP1MMIOx3B109AC, RxSmuMessageResponse);

  // 3 Write message ID
  RequestId = SMC_MSG_SleepEntry;
  xUSLIndirectPciWrite32(PciAddress.AddressValue, MP1MMIOx3B10528, RequestId);

  // Halt execution
  xUslCpuSleep ();
}

/**
 * SmuReadBistInfoPhx
 *
 * @brief Retrieve BIST info from SMU for Phoenix
 *
 * @param   SilContext  A context structure through which host firmware defined data
 *                      can be passed to openSIL. The host firmware is responsible
 *                      for initializing the SIL_CONTEXT structure.
 * @param   InstanceId  The unique identifier of the SMU instance associated
 *                      with this socket (for PHX, the socket number)
 * @param   PhysicalCcx Number of the physical CCX
 * @param   BistInfo    Pointer to uint32_t where BIST info should be copied.
 *                      Caller should allocate the memory for BistData.
 *
 * @note    This service require the GnbHandle which is created during NBIO Timepoint 1 execution.
 *
 * @return  SIL_STATUS  SilNotFound         If unable to make SMU call
 *                      SilNotFound         IP transfer table not found
 *                      SilInvalidParameter If null pointer passed in
 *                      SilPass             If call succeeded and return argument is populated. Returns
 *                      SilUnsupported      SMU does not support this call
 *
 **/
SIL_STATUS
SmuReadBistInfoPhx (
  SIL_CONTEXT    *SilContext,
  uint8_t        InstanceId,
  uint8_t        PhysicalCcx,
  uint32_t       *BistInfo
  )
{
  uint32_t                    SmuArg[6];
  SIL_STATUS                  Status;
  SMC_RESULT                  SmcStatus;
  GNB_HANDLE                  *GnbHandle;
  APOB_IP2IP_API              *ApobIp2IpApi;

  SMU_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Status = SilGetIp2IpApi(SilContext, SilId_ApobClass, (void **) &ApobIp2IpApi);
  if ((Status != SilPass) || (ApobIp2IpApi == NULL)) {
    assert(Status == SilPass);
    return Status;
  }

  if (BistInfo == NULL) {
    return SilInvalidParameter;
  }

  Status = SmuGetGnbHandleCommon(SilContext, InstanceId, &GnbHandle);

  if (Status == SilPass) {
    SmuServiceInitArgumentsCommon(SmuArg);
    SmuArg[0] = PhysicalCcx;
    SmcStatus = SmuServiceRequestPhx(GnbHandle->Address,
      SMC_MSG_GetCoreBistPF_PHX,
      SmuArg,
      0
      );
    if (SmcStatus == SMC_Result_OK) {
      *BistInfo = SmuArg[0];
      SMU_TRACEPOINT(SIL_TRACE_INFO, "BIST INFO = 0x%x.\n", *BistInfo);
      Status = SilPass;
    } else {
      SMU_TRACEPOINT(SIL_TRACE_INFO, "BIST call to SMU failed.\n");
      Status = SilUnsupported;
    }
  } else {
    SMU_TRACEPOINT(SIL_TRACE_INFO, "Unable to make BIST call to SMU. Status: 0x%x\n", Status);
    Status = SilNotFound;
  }

  SMU_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return Status;
}

/**
 * SmuReadBrandStringPhx
 *
 * @brief Returns the brand string read from SMN space.
 *
 * @param   SilContext        A context structure through which host firmware defined data
 *                            can be passed to openSIL. The host firmware is responsible
 *                            for initializing the SIL_CONTEXT structure.
 * @param   InstanceId        The unique identifier of the SMU instance
 *                            associated with this socket (for GN, the socket
 *                            number)
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
SmuReadBrandStringPhx (
  SIL_CONTEXT    *SilContext,
  uint32_t       InstanceId,
  uint32_t       BrandStringLength,
  uint8_t        *BrandString
  )
{
  GNB_HANDLE                  *GnbHandle = NULL;
  SIL_STATUS                  Status = SilPass;
  uint32_t                    SmuArg[6];
  uint32_t                    SmuResult;
  uint8_t                     *StringPtr;
  uint32_t                    StringCount;
  uint32_t                    ByteCount;
  uint32_t                    Iteration;

  SMU_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Status = SmuGetGnbHandleCommon(SilContext, InstanceId, &GnbHandle);
  if (Status == SilPass) {
    SMU_TRACEPOINT(SIL_TRACE_INFO, "GnbHandle Found.\n");
    StringCount = BrandStringLength;
    StringPtr = BrandString;
    Iteration = 0;
    while (StringCount != 0) {
      SmuServiceInitArgumentsCommon(SmuArg);
      SmuArg[0] = Iteration++;
      SmuResult = SmuServiceRequestPhx(GnbHandle->Address,
        SMC_MSG_GetNameString_PHX,
        SmuArg,
        0
        );
      if (SmuResult == SMC_Result_OK) {
        ByteCount = (StringCount < STRING_COUNT_LIMIT) ? StringCount : STRING_COUNT_LIMIT;
        memcpy(StringPtr, SmuArg, ByteCount);
        StringPtr += ByteCount;
        StringCount -= ByteCount;
      } else {
        Status = SilInvalidParameter;
        SMU_TRACEPOINT(SIL_TRACE_INFO, "SmuRequestFail!!\n");
        break;
      }
    }
  }

  SMU_TRACEPOINT(SIL_TRACE_EXIT, "Exit Status:0x%x.\n", Status);
  return Status;
}

/**
 * SmuReadCacWeightsPhx
 *
 * @brief Returns the CAC weights read from SMU
 *
 * @param   SilContext    A context structure through which host firmware defined data
 *                        can be passed to openSIL. The host firmware is responsible
 *                        for initializing the SIL_CONTEXT structure.
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
SmuReadCacWeightsPhx (
  SIL_CONTEXT  *SilContext,
  uint32_t     MaxNumWeights,
  uint64_t     *ApmWeights
  )
{
  GNB_HANDLE                             *GnbHandle = NULL;
  SIL_STATUS                             Status = SilPass;
  uint8_t                                WeightIndex;
  uint32_t                               SmuArg[6];
  SMC_RESULT                             SmcStatus;

  SMU_TRACEPOINT(SIL_TRACE_INFO, "SmuReadCacWeightsV13 Entry.\n");
  SMU_TRACEPOINT(SIL_TRACE_INFO, "MaxNumWeights %x\n", MaxNumWeights);

  if (MaxNumWeights > MAX_CAC_WEIGHT_NUM) {
    SMU_TRACEPOINT(SIL_TRACE_INFO, "Input over Max of Weights number\n");
    return SilUnsupported;
  }

  Status = SmuGetGnbHandleCommon(SilContext, 0, &GnbHandle);
  if (Status == SilPass) {
    SMU_TRACEPOINT(SIL_TRACE_INFO, "GnbHandle Found.\n");
    for (WeightIndex = 0; WeightIndex < MaxNumWeights; WeightIndex++) {
      SmuServiceInitArgumentsCommon(SmuArg);
      SmuArg[0] = WeightIndex;

      SmcStatus = SmuServiceRequestPhx(GnbHandle->Address,
        SMC_MSG_ReadCoreCacWeightRegister_PHX,
        SmuArg,
        0
        );

      if (SmcStatus == SMC_Result_OK) {
        SMU_TRACEPOINT(SIL_TRACE_INFO, "SmuArg[0] = %x\n", SmuArg[0]);
        SMU_TRACEPOINT(SIL_TRACE_INFO, "SmuArg[0] = %x\n", SmuArg[1]);
        ApmWeights[WeightIndex] = ((uint64_t) SmuArg[1] << 32) | SmuArg[0];
      } else {
        Status = SilInvalidParameter;
        SMU_TRACEPOINT(SIL_TRACE_INFO, "SmuRequestFail!!\n");
      }
    }
  }

  SMU_TRACEPOINT(SIL_TRACE_INFO, "Exit Status: 0x%x\n", Status);
  return Status;
}

/// Core Dldo PSM offset Array
typedef struct {
  uint32_t  CoreIndicator; ///< [31:28] --> CCD,
                           ///< [27:20] --> CORE,
                           ///< 0xFFFFFFFF --> all Core,
                           ///< 0xFFFFFFFE --> Array Terminate flag
  uint32_t  PsmOffset;     ///< PSM offset
} CORE_PSM_OFFSET;

SIL_STATUS
SmuInitAfterCcxDonePhx (
  SIL_CONTEXT                   *SilContext
  )
{
  SIL_STATUS                           Status;
  uint32_t                             SmuArg[6];
  MPIO_PRESIL_CTRL0                    PreSilCtrl0;
  MPIO_PRESIL_CTRL1                    PreSilCtrl1;
  CORE_PSM_OFFSET                      *PsmOffsetArray;
  GNB_HANDLE                           *GnbHandle;
  uint32_t                             PackageType;
  uint32_t                             *Svi3TableAddr;
  APOB_IP2IP_API                       *ApobIp2IpApi;
  APOB_ENV_FLAGS_STRUCT                *ApobFlags;
  SMUCLASS_INPUT_BLK                   *SmuInputBlock;
  CCXCLASS_DATA_BLK                    *CcxData;

  SMU_TRACEPOINT(SIL_TRACE_ENTRY, "Start\n");

  SmuInputBlock = (SMUCLASS_INPUT_BLK *) xUslFindStructure(SilContext, SilId_SmuClass, 0);
  if (SmuInputBlock == NULL) {
    SMU_TRACEPOINT(SIL_TRACE_ERROR, "Failed to find SMU input data blk\n");
    return SilNotFound;
  }

  CcxData = (CCXCLASS_DATA_BLK *)xUslFindStructure(SilContext, SilId_CcxClass, 0);
  if (CcxData == NULL) {
    SMU_TRACEPOINT(SIL_TRACE_INFO, "CcxData buffer not found.\n");
    return SilNotFound; // Could not find the IP input block
  }

  Status = SmuGetGnbHandleCommon(SilContext, 0, &GnbHandle);
  if (Status != SilPass) {
    return Status;
  }

  Status = SilGetIp2IpApi(SilContext, SilId_ApobClass, (void **)(&ApobIp2IpApi));
  if (Status != SilPass) {
    SMU_TRACEPOINT (SIL_TRACE_ERROR, " APOB I2I API is not found.\n");
    return Status;
  }

  Status = ApobIp2IpApi->ApobAmdGetApobEntryInstance(SilContext,
    APOB_GEN,
    APOB_ENV_FLAGS_INFO_TYPE,
    0,
    0,
    (APOB_TYPE_HEADER **)&ApobFlags
    );
  if (Status != SilPass) {
    SMU_TRACEPOINT (SIL_TRACE_ERROR, " APOB I2I API is not found.\n");
    return Status;
  }

  PreSilCtrl0.Value = ApobFlags->Flags.PreSilCtrl0;
  PreSilCtrl1.Value = ApobFlags->Flags.PreSilCtrl1;

  PackageType = xUSLGetPackageType ();
  SMU_TRACEPOINT(SIL_TRACE_INFO, "PackageType = %d\n", PackageType);

  //SVI3 rails register programming
  Svi3TableAddr = (uint32_t*)(uintptr_t)SmuInputBlock->Svi3TableAddress;
  SMU_TRACEPOINT(SIL_TRACE_INFO, "Svi3TableAddr = %08x\n", Svi3TableAddr);
  if (Svi3TableAddr != 0) {
    while (*Svi3TableAddr != 0x0FFFFFFFF) {
      SmuServiceInitArgumentsCommon(SmuArg);
      SmuArg[0] = *Svi3TableAddr;
      SMU_TRACEPOINT(SIL_TRACE_INFO, "  SVI3 entry = 0x%08x\n", SmuArg[0]);
      SmuServiceRequestPhx(GnbHandle->Address,
        SMC_MSG_WriteSviRegister,
        SmuArg,
        0
        );
      Svi3TableAddr += 1;
    }
  }

  if ((SmuInputBlock->AmdFabricCstate != 0) && (SmuInputBlock->AmdFabricCstate != 1)) {
    // DF CState is auto.
    SmuInputBlock->AmdFabricCstate = CcxData->CcxInputBlock.AmdCStateMode;
  }

   SMU_TRACEPOINT(SIL_TRACE_INFO, "AmdFabricCstate %x \n", SmuInputBlock->AmdFabricCstate);
   if (SmuInputBlock->AmdFabricCstate != 0xFF) {
     if(SmuInputBlock->AmdFabricCstate == 0x1) {
       SmuInputBlock->SmuFeatureControlExt |= (uint32_t)(1 << (FEATURE_DF_CSTATES_BIT % 32));
     } else {
       SmuInputBlock->SmuFeatureControlExt &= ~((uint32_t)(1 << (FEATURE_DF_CSTATES_BIT % 32)));
     }
   }

  //
  // BIOS call SMC_MSG_EnableAllSmuFeatures with a bitmask indicating which SMU features to enable.
  // Masks are allocated smu11.h
  // Each mask bit should be linked to a setup option to enable/disable it. If DC BTC is enabled alter sequence
  //
  SMU_TRACEPOINT(SIL_TRACE_INFO, "Set SmuFeatureControls, Extended : 0x%x, Standard : 0x%x\n",
    SmuInputBlock->SmuFeatureControlExt, SmuInputBlock->SmuFeatureControl);

  if (PreSilCtrl1.Field.FASTSIM_SMU_MSGS == 0) {
    if (!SmuInputBlock->DcBtc) {
      SmuServiceInitArgumentsCommon(SmuArg);
      SmuArg[0] = SmuInputBlock->SmuFeatureControl;
      SmuArg[1] = SmuInputBlock->SmuFeatureControlExt;
      SmuArg[2] = SmuInputBlock->SmuFeatureControl64;
      SmuServiceRequestPhx(GnbHandle->Address,
        SMC_MSG_EnableSmuFeatures,
        SmuArg,
        0
        );
    } else {
      // Send enable mask with CC6, PSI masked out if DC BTC is enabled. Initialize DC BTC and then send CC6 enable separately
      SmuServiceInitArgumentsCommon(SmuArg);
      SmuArg[0] = SmuInputBlock->SmuFeatureControl;
      SmuArg[1] = SmuInputBlock->SmuFeatureControlExt & ~((uint32_t)(FEATURE_CC6_MASK | FEATURE_PSI_MASK));
      SmuArg[2] = SmuInputBlock->SmuFeatureControl64;
      SmuServiceRequestPhx(GnbHandle->Address,
        SMC_MSG_EnableSmuFeatures,
        SmuArg,
        0
        );
      
      SMU_TRACEPOINT(SIL_TRACE_INFO, "DcBtc Enable on socket %d\n", GnbHandle->SocketId);
      SMU_TRACEPOINT(SIL_TRACE_INFO, "DcBtc VID is %x and margin %x\n", SmuInputBlock->DcBtcVid, SmuInputBlock->DcBtcErrorOfsetVoltageMargin);
      // VID is LSB 8 bits, Voltage margin the next significant 8 bits. PCDs remain at their default unless overriden

      SmuServiceInitArgumentsCommon(SmuArg);
      SmuArg[0] = (uint32_t)SmuInputBlock->DcBtcVid | (((uint32_t)SmuInputBlock->DcBtcErrorOfsetVoltageMargin)) << 8;
      SmuServiceRequestPhx(GnbHandle->Address,
        SMC_MSG_DcBtc,
        SmuArg,
        0
        );
      // Send CC6, PSI feature bit if it still needs to be enabled after DC BTC
      SmuServiceInitArgumentsCommon(SmuArg);
      SmuArg[1] = SmuInputBlock->SmuFeatureControlExt & (uint32_t)(FEATURE_CC6_MASK | FEATURE_PSI_MASK);
      SmuServiceRequestPhx(GnbHandle->Address,
        SMC_MSG_EnableSmuFeatures,
        SmuArg,
        0
        );
    }
  }

  if (SmuInputBlock->FllBtcEnable) {
    SMU_TRACEPOINT(SIL_TRACE_INFO, "FllBtc Enable \n");
    SmuServiceInitArgumentsCommon(SmuArg);
    SmuServiceRequestPhx(GnbHandle->Address, SMC_MSG_FllBootTimeCalibration, SmuArg, 0);
  }

    //
    //  Ulv Vid Offset Control
    //
    if (PreSilCtrl1.Field.FASTSIM_PWR_MSGS == 0) {
      SMU_TRACEPOINT(SIL_TRACE_INFO, "PcdUlvVidCtrl = 0%x\n", SmuInputBlock->UlvVidCtrl);
      SMU_TRACEPOINT(SIL_TRACE_INFO, "PcdUlvVidOffset = 0%x\n", SmuInputBlock->UlvVidOffset);
      if (SmuInputBlock->UlvVidCtrl) {
        // Send enable Ulv Vid override
        SmuServiceInitArgumentsCommon(SmuArg);
        SmuArg[0] = SmuInputBlock->UlvVidOffset;
        SmuServiceRequestPhx(GnbHandle->Address, SMC_MSG_SetZ8VidOffset, SmuArg, 0);
      }
      //
      //  Vdd off Vid Offset Control
      //
      SMU_TRACEPOINT(SIL_TRACE_INFO, "PcdVddOffVidCtrl = 0%x\n", SmuInputBlock->VddOffVidCtrl);
      SMU_TRACEPOINT(SIL_TRACE_INFO, "PcdVddOffVid = 0%x\n", SmuInputBlock->VddOffVid);
      SmuServiceInitArgumentsCommon(SmuArg);
      if (SmuInputBlock->VddOffVidCtrl) {
        if (SmuInputBlock->VddOffVid >= 250) {
          SmuArg[0] = ((SmuInputBlock->VddOffVid - 250) * 200 + 1500) / 1000;
        } else {
          SmuArg[0] = 0;
        }

        SmuServiceRequestPhx(GnbHandle->Address, SMC_MSG_SetVddOffVid, SmuArg, 0);
      }
    }

  //
  // Apply those changes after all feature enable
  //

  if (SmuInputBlock->SlowPPTLimitApuOnly != 0) {
    SmuServiceInitArgumentsCommon(SmuArg);
    SmuArg[0] = SmuInputBlock->SlowPPTLimitApuOnly;
    SmuServiceRequestPhx(GnbHandle->Address, SMC_MSG_SetSlowPPTLimitApuOnly, SmuArg, 0);
  }

  if (SmuInputBlock->MsgSetSustainedPowerLimit != 0) {
    SmuServiceInitArgumentsCommon(SmuArg);
    SmuArg[0] = SmuInputBlock->MsgSetSustainedPowerLimit;
    SmuServiceRequestPhx(GnbHandle->Address, SMC_MSG_SetSustainedPowerLimit, SmuArg, 0);
  }
  if (SmuInputBlock->MsgSetFastPPTLimit != 0) {
    SmuServiceInitArgumentsCommon(SmuArg);
    SmuArg[0] = SmuInputBlock->MsgSetFastPPTLimit;
    SmuServiceRequestPhx(GnbHandle->Address, SMC_MSG_SetFastPPTLimit, SmuArg, 0);
  }

  if (SmuInputBlock->MsgSetSlowPPTLimit != 0) {
    SmuServiceInitArgumentsCommon(SmuArg);
    SmuArg[0] = SmuInputBlock->MsgSetSlowPPTLimit;
    SmuServiceRequestPhx(GnbHandle->Address, SMC_MSG_SetSlowPPTLimit, SmuArg, 0);
  }

  if (SmuInputBlock->MsgStapmTimeConstant != 0) {
    SmuServiceInitArgumentsCommon(SmuArg);
    SmuArg[0] = SmuInputBlock->MsgStapmTimeConstant;
    SmuServiceRequestPhx(GnbHandle->Address, SMC_MSG_SetStapmTimeConstant, SmuArg, 0);
  }

  //
  // OC feature
  //
  // PPT Limit
  SmuServiceInitArgumentsCommon(SmuArg);
  SmuArg[0] = SmuInputBlock->MocPPTLimit;
  if (SmuArg[0] != 0) {
    SmuServiceRequestPhx(GnbHandle->Address, SMC_MSG_SetFastPPTLimit, SmuArg, 0);
    SmuServiceRequestPhx(GnbHandle->Address, SMC_MSG_SetSlowPPTLimit, SmuArg, 0);
    SmuServiceRequestPhx(GnbHandle->Address, SMC_MSG_SetSlowPPTLimitApuOnly, SmuArg, 0);
  }

  // TDC Limit
  SmuServiceInitArgumentsCommon(SmuArg);
  SmuArg[0] = SmuInputBlock->MocTDCLimit;
  if (SmuArg[0] != 0) {
    SmuServiceRequestPhx(GnbHandle->Address, SMC_MSG_SetVrmVddCurrentLimit, SmuArg, 0);
  }
  SmuServiceInitArgumentsCommon(SmuArg);
  SmuArg[0] = SmuInputBlock->MocSocTDCLimit;
  if (SmuArg[0] != 0) {
    SmuServiceRequestPhx(GnbHandle->Address, SMC_MSG_SetVrmSocCurrentLimit, SmuArg, 0);
  }
  // EDC Limit
  SmuServiceInitArgumentsCommon(SmuArg);
  SmuArg[0] = SmuInputBlock->MocEDCLimit;
  if (SmuArg[0] != 0) {
    SmuServiceRequestPhx(GnbHandle->Address, SMC_MSG_SetVrmVddMaximumCurrentLimit, SmuArg, 0);
  }
  SmuServiceInitArgumentsCommon(SmuArg);
  SmuArg[0] = SmuInputBlock->MocSocEDCLimit;
  if (SmuArg[0] != 0) {
    SmuServiceRequestPhx(GnbHandle->Address, SMC_MSG_SetVrmSocMaximumCurrentLimit, SmuArg, 0);
  }
  SmuServiceInitArgumentsCommon(SmuArg);
  SmuArg[0] = SmuInputBlock->MocPBOLimitScalar;
  if (SmuArg[0] != 0) {
    SmuServiceRequestPhx(GnbHandle->Address, SMC_MSG_SetFitLimitScalar, SmuArg, 0);
  }
  SmuServiceInitArgumentsCommon(SmuArg);
  SmuArg[0] = SmuInputBlock->MocTjMax;
  if (SmuArg[0] != 0) {
    SmuServiceRequestPhx(GnbHandle->Address, SMC_MSG_SetTjMax, SmuArg, 0);
  }

  //  set CoreDldoPsmMargin
  if (SmuInputBlock->CfgCoreDldoPsmArray != 0x0) {
    PsmOffsetArray = (CORE_PSM_OFFSET *)(uintptr_t)SmuInputBlock->CfgCoreDldoPsmArray;
    while (PsmOffsetArray->CoreIndicator != 0xFFFFFFFE) {
      SMU_TRACEPOINT(SIL_TRACE_INFO, "CoreIndicator = 0x%x, PsmOffset = 0x%x\n", PsmOffsetArray->CoreIndicator, PsmOffsetArray->PsmOffset);
      SmuServiceInitArgumentsCommon(SmuArg);
      if (PsmOffsetArray->CoreIndicator != 0xFFFFFFFF) {
        SmuArg[0] = (uint32_t)((PsmOffsetArray->CoreIndicator & 0xFFF00000) | PsmOffsetArray->PsmOffset);
        SmuServiceRequestPhx(GnbHandle->Address, SMC_MSG_SetCorePsmMargin, SmuArg, 0);
      } else {
        SmuArg[0] = (uint32_t) PsmOffsetArray->PsmOffset;
        SmuServiceRequestPhx(GnbHandle->Address, SMC_MSG_SetAllCorePsmMargin, SmuArg, 0);
      }
      PsmOffsetArray++;
    }
  }

  // Per request always send this message default is 10ms also be able change from CBS
  if (PreSilCtrl0.Field.Environment == 0) {
      SmuServiceInitArgumentsCommon(SmuArg);
      SmuArg[0] = SmuInputBlock->SetS0i3PmeTurnOffDelay;
      SmuServiceRequestPhx(GnbHandle->Address, SMC_MSG_SetS0i3PmeTurnOffDelay, SmuArg, 0);
  }

  //VDDCR_SOC DC BTC
  if (SmuInputBlock->SocDcBtcEnable) {
    SmuServiceInitArgumentsCommon(SmuArg);
    SmuServiceRequestPhx(GnbHandle->Address, SMC_MSG_VDDCR_SOC_DcBtc, SmuArg, 0);
  }

  if (SmuInputBlock->SmuGfxPsmMarginControl) {
    SmuServiceInitArgumentsCommon(SmuArg);
    SmuArg[0] = SmuInputBlock->SmuGfxPsmMarginValue;
    SmuServiceRequestPhx(GnbHandle->Address, SMC_MSG_SetGfxVidMargin, SmuArg, 0);
  }

  if((SmuInputBlock->MocGfxCoreVid != 0x0) || (SmuInputBlock->MocGfxClockFrequency != 0x0)) {
    SmuServiceInitArgumentsCommon(SmuArg);
    SmuArg[0] = (uint32_t)SmuInputBlock->MocGfxCoreVid << 16;
    SmuArg[0] |= SmuInputBlock->MocGfxClockFrequency;
    SmuServiceRequestPhx(GnbHandle->Address, SMC_MSG_SetGfxclkOverdriveByFreqVid, SmuArg, 0);
  }

  SMU_TRACEPOINT(SIL_TRACE_EXIT, "\n");

  return SilPass;
}

static void
EnableOrDisableIpDeepSleep (
  SMUCLASS_INPUT_BLK  *SmuInputBlock,
  GNB_HANDLE          *GnbHandle,
  uint32_t            PackageType
  )
{
  uint32_t Data32 = 0;
  SMU_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if ((SmuInputBlock->SmuFeatureControl & FEATURE_DS_SHUBCLK_MASK) != 0) {
    if ((1 << ZEN4_PKG_AM5) == PackageType) {
      if (ISSOCPHX2AM5) {
        xUSLSmnReadModifyWrite(0, GnbHandle->Address.Address.Bus,
          SYSHUBMMx1411000,
          (uint32_t) ~((1 << 4) |
                       (1 << 5)),
          (1 << 4) |
          (1 << 5)
          );
        Data32 = xUSLSmnRead(0, GnbHandle->Address.Address.Bus, SYSHUBMMx1411000);
        SMU_TRACEPOINT(SIL_TRACE_INFO,  "PHX2AM5: SYSHUB_DS_CTRL_SHUBCLK = 0x%x\n", Data32);
      } else {
        xUSLSmnReadModifyWrite(0, GnbHandle->Address.Address.Bus,
          SYSHUBMMx1411000,
          (uint32_t) ~(1 << 4),
          (1 << 4)
          );
        Data32 = xUSLSmnRead(0, GnbHandle->Address.Address.Bus, SYSHUBMMx1411000);
        SMU_TRACEPOINT(SIL_TRACE_INFO,  "PHX1AM5: SYSHUB_DS_CTRL_SHUBCLK = 0x%x\n", Data32);
      }
    }
  }
}

SMU_PP_TABLE PPTableDefault = {

  18, //  uint32_t Version;

  //PCD infrastructure limits
  0, //  uint32_t SUSTAINED_POWER_LIMIT;           //Power [mWatts] (STAPM)
  0, //  uint32_t FAST_PPT_LIMIT;                  //Power [mWatts] (fPPT)
  0, //  uint32_t SLOW_PPT_LIMIT;                  //Power [mWatts] (sPPT)
  0, //  uint32_t SLOW_PPT_TIME_CONSTANT;          //Time constant (seconds)
  0, //  uint8_t  SYSTEM_TEMPERATURE_TRACKING;     //Tskin feedback from EC; 1 = enabled; 0 = disabled
  0, //  uint8_t  STAPM_BOOST_OVERRIDE;            //1 = use value in STAPM_BOOST; 0 = use SMU FW default
  0, //  uint8_t  STAPM_BOOST;                     //1 = enabled; 0 = disabled
  0, //  uint8_t  AA_MODE_EN;                      //A+A power/thermal sharing algorithm; 1 = enable; 0 = disable
  0, //  uint32_t STAPM_TIME_CONSTANT;             //Time constant (seconds)
  0, //  uint32_t THERMAL_LIMIT;                   //TjMax ['C] (THM)
  0, //  uint32_t VRM_VDD_CURRENT_LIMIT;           //Current [mA] (VDDCR_VDD TDC)
  0, //  uint32_t VRM_VDD_MAXIMUM_CURRENT_LIMIT;   //Current [mA] (VDDCR_VDD EDC)
  0, //  uint32_t VRM_SOC_CURRENT_LIMIT;           //Current [mA] (VDDCR_SOC TDC)
  0, //  uint32_t VRM_SOC_MAXIMUM_CURRENT_LIMIT;   //Current [mA] (VDDCR_SOC EDC)
  0, //  uint32_t VRM_SR_CURRENT_LIMIT;            //Current [mA] (VDDCR_SR  TDC)
  0, //  uint32_t VRM_SR_MAXIMUM_CURRENT_LIMIT;    //Current [mA] (VDDCR_SR  EDC)
  0, //  uint32_t PSI3_VDD_CURRENT_LIMIT;          //Current [mA] (VDDCR_VDD PSI3)
  0, //  uint32_t PROCHOT_L_DEASSERTION_RAMP_TIME; //Time [ms] (PROCHOT)
  0, //  uint32_t P3T_LIMIT;                       //Power [mWatts] (P3T - Peak Package Power)
  0, //  uint8_t  SYSTEM_CONFIG;                   //0 = use fused default; >0 = user specified value
  0, //  uint8_t  SW_SCI_GPE_ID;                   //GPE Event ID [0-31] used by MP1 to trigger software SCI (needs to be edge configured)
  0, //  uint8_t  FCH_IOMUX_19;                    //1 or 2 = save/restore for s0i3
  0, //  uint8_t  FCH_IOMUX_20;                    //1 or 2 = save/restore for s0i3

  //STT parameters/co-efficients
  0, //  uint8_t  STT_PCB_SENSOR_COUNT;           //2 = APU only; 3 = APU+dGPU; all other values are invalid
  0, //  uint8_t  STT_spare;
  0, //  uint16_t STT_MIN_POWER_LIMIT;            //Minimum APU power limit for STT control [mWatts]
  0, //  int16_t  STT_M1;                         //Signed Q6.10 encoding
  0, //  int16_t  STT_M2;                         //Signed Q6.10 encoding
  0, //  int16_t  STT_M3;                         //Signed Q6.10 encoding
  0, //  int16_t  STT_M4;                         //Signed Q6.10 encoding
  0, //  int16_t  STT_M5;                         //Signed Q6.10 encoding
  0, //  int16_t  STT_M6;                         //Signed Q6.10 encoding
  0, //  int16_t  STT_C_APU;                      //Signed Q8.8 encoding
  0, //  int16_t  STT_C_HS2;                      //Signed Q8.8 encoding
  0, //  uint16_t STT_ALPHA_APU;                  //Unsigned Q0.16 encoding
  0, //  uint16_t STT_ALPHA_HS2;                  //Unsigned Q0.16 encoding
  0, //  uint16_t STT_SKIN_TEMPERATURE_LIMIT_APU; //Unsigned Q8.8 encoding
  0, //  uint16_t STT_SKIN_TEMPERATURE_LIMIT_HS2; //Unsigned Q8.8 encoding
  0, //  uint16_t STT_ERROR_COEFF;                //Unsigned Q0.16 encoding
  0, //  uint16_t STT_ERROR_RATE_COEFF;           //Unsigned Q0.16 encoding

  //PCD/CBS fan table
  0, //  uint8_t FanTable_Override;     //[1 means use the full set of data specified below; 0 means use default fan table]
  0, //  uint8_t FanTable_Hysteresis;
  0, //  uint8_t FanTable_TempLow;
  0, //  uint8_t FanTable_TempMed;
  0, //  uint8_t FanTable_TempHigh;
  0, //  uint8_t FanTable_TempCritical;
  0, //  uint8_t FanTable_PwmLow;
  0, //  uint8_t FanTable_PwmMed;
  0, //  uint8_t FanTable_PwmHigh;
  0, //  uint8_t FanTable_PwmFreq;      //[0 = 25kHz; 1 = 100Hz]
  0, //  uint8_t FanTable_Polarity;     //[0 = negative; 1 = positive]
  0, //  uint8_t FanTable_spare;

  //CBS debug options
  {0,0,0,0,0,0,0,0}, //    int16_t  PerCoreDldoPsmMargin[8];     //[PSM count] [signed] for each core
  0, //  int16_t  CoreDldoPsmMargin;           //[PSM count] [signed]
  0, //  int16_t  GfxDldoPsmMargin;            //[PSM count] [signed]
  0, //  int16_t  VddcrVddVoltageMargin;       //[mV] [signed]
  0, //  int16_t  VddcrSocVoltageMargin;       //[mV] [signed]
  0, //  int16_t  VddcrSrVoltageMargin;       //[mV] [signed]
  0, //  uint16_t ForceVddcrVddVoltage;        //[mV; 0 means no force]
  0, //  uint16_t ForceVddcrSocVoltage;        //[mV; 0 means no force]
  0, //  uint16_t ForceVddcrSrVoltage;        //[mV; 0 means no force]

  0, //  uint8_t  C1eEnabled;                  //[0 = disable C1e; 1 = enable C1e]
  0, //  uint8_t  CCX_VdciAsync;               //[0 = predictive mode; 1 = async mode]
  0, //  uint8_t  DisableMpCcxRootCg;          //[0 = Enable 1= MPCCX root clock wont be gated during vddoff
  0, //  uint8_t  ForceFanPwmEn;               //[1 means use the ForceFanPwm value below]
  0, //  uint8_t  ForceFanPwm;                 //[% 0-100]
  0, //  uint8_t  DfsBypassOff;                //PMFW-995
  0, //  uint8_t  CoreCountControlEn;          //[0 = Disable 1 = Enable CoreCountControl feature]
  0, //  uint8_t  HTFmaxTemperature;           //High temperature Fmax --- temperature ['C]
  0, //  uint16_t HTFmaxFrequency;             //High temperature Fmax --- frequency [MHz]
  0, //  uint16_t ForceCclkFrequency;          //Force the CCLK frequency to the specified frequency [MHz]
  0, //  uint16_t CclkFmaxOverride;            //Specify the CCLK Fmax override [MHz]
  0, //  uint16_t CclkFminOverride;            //Specify the CCLK Fmin override [MHz]
  0, //  uint16_t ForceGfxclkFrequency;        //Force the GFXCLK frequency to the specified frequency [MHz]
  0, //  uint16_t GfxclkFmaxOverride;          //Specify the GFXCLK Fmax override [MHz]
  0, //  uint8_t  PostCodeLevel;               //Postcode Verbosity: 0 = Disable, 1 = LEVEL1, 2 = LEVEL2, 3 = LEVEL3 (max = 3)
  0, //  uint8_t  StbVerbosityLevel;           //STB Verbosity: 0 = Disable, 1 = LEVEL1, 2 = LEVEL2, 3 = LEVEL3 (max = 3)
  0, //  uint8_t  EnableQos;                   //[0 = disable QoS; 1 = Enable QoS]
  0, //  uint8_t  LP2eEnable;
  0, //  uint8_t  DisableZ8;
  0, //  uint8_t  spare1;
  0, //  uint8_t  DisableZ10;
  0, //  uint8_t  ZstatesMaskEnable;
  0, //  uint32_t ZscIpIdleMask;
  0, //  uint32_t ZscWakeEventMask0;
  0, //  uint32_t ZscWakeEventMask1;
  0, //  uint32_t ZscIpFenceAckMask;

  //PCD/CBS Telemetry setup
  0, //  uint32_t TelemetryVddcrVddSlope;
  0, //  int32_t  TelemetryVddcrVddOffset;
  0, //  uint32_t TelemetryVddcrSocSlope;
  0, //  int32_t  TelemetryVddcrSocOffset;
  0, //  uint32_t TelemetryVddcrSrSlope;
  0, //  int32_t  TelemetryVddcrSrOffset;

  //PRECISE AND DIRECT OVERCLOCKING CONFIG
  0, //  uint8_t  OC_DISABLE; //1=disable overclocking; 0=allow overclocking
  0, //  uint8_t  OC_MINVID;  //[VID] SVI3 VID encoding
  0, //  uint16_t OC_FREQMAX; //MHz

  //DF CSTATE CONFIG
  0, //  uint8_t DfCstateConfigOverride;
  0, //  uint8_t DfCstateMmStutterOptEn;
  0, //  uint8_t DfCstateDceElptModeEn;
  0, //  uint8_t DfCstatePwrGateEn;
  0, //  uint8_t DfCstateFclkPwrDnEn;
  0, //  uint8_t DfCstateSelfRefrEn;
  0, //  uint8_t DfCstateUclkPwrDnEn;
  0, //  uint8_t spare2;

  //DF Light CSTATE CONFIG
  1, //  uint8_t DfLightCstateConfigOverride;
  8, //  uint8_t CcmHystDly;
  2, //  uint8_t Ccmhystscale;
  4, //  uint8_t Ccmmonitorsuccdly;
  2, //  uint8_t Ccmmonitorsuccscale;
  7, //  uint8_t Ccmmonitorsatlvl;
  6, //  uint8_t Ccmmonitorsuccthr;
  2, //  uint8_t Ccmmonitorfaildec;
  3, //  uint8_t Ccmmonitorburstfilter;
  0, //  uint8_t Ccmmonitorburstscale;
  0, //  uint8_t spare3;
  0, //  uint8_t CC1Dis; //[0 = CC1 default; 1 = CC1 disabled]

  //CPPC Constraints
  0, //  uint8_t CppcConstraintsEnabled;
  {0, 0, 0}, //  uint8_t spare4[3];
  0, //  uint8_t PerfLimit_MaxRange;
  0, //  uint8_t PerfLimit_MinRange;
  0, //  uint8_t EPP_MaxRange;
  0, //  uint8_t EPP_MinRange;

  //Misc
  0, //  uint32_t System;
  0, //  uint32_t MinS0i3SleepTimeInMs;
  0, //  uint32_t SystemRtcTime;

  //GFX OC limits
  0, //  uint8_t  OC_GFX_DISABLE;  //1=disable overclocking; 0=allow overclocking
  0, //  uint8_t  OC_GFX_MINVID;   //[PMFW-14874][VID] Setting a maximum voltage limitation with SVI3 VID encoding
  0, //  uint16_t OC_GFX_FREQMAX;  //MHz

  //GFXCLK/GFXOFF related parameters
  0, //  uint8_t   GceaLinkMgrIdleThreshold;        //Set by SMU FW during enablment of SOC_ULV. Controls delay for GFX SDP port disconnection during idle events
  0, //  uint8_t   GfxClkSource;                    // 0 = PLL, 1 = DFLL
  0, //  uint8_t   spare5;
  1, //  uint8_t   ApccEnable;                      //1 = Enable APCC, should be mutually exclusive to GFX EDC controller

  0, //  uint16_t  GfxVinit;                        // Voltage required to support Finit during GFX power up/GFXOFF exit (in mV)
  0, //  uint16_t  GfxclkInitFreq;                  // in Mhz. Options are 800, 600, 400, 300, 200, 100
  0, //  uint16_t  GfxclkGfxoffEntryFreq;           // in Mhz
  0, //  uint16_t  GfxclkIdleFreq;                  // in MHz
  0, //  uint16_t  GfxclkSlewRate;                  // for PLL babystepping
  0, //  uint16_t  GfxclkFminOverride;              // Specify the GFXCLK Fmin override [MHz]

  0, //  uint16_t  GfxMinVoltage;                   //Minimum GFX voltage, in mV
  0, //  uint16_t  GfxMaxVoltage;                   //Maximum GFX voltage, in mV

  0, //  uint8_t  GfxDldoBypass;
  0, //  uint8_t  GfxDynamicRmEn;
  //GFX temperature dependent Vmin feature
  0, //  uint8_t VDDGFX_TVminOverrideEn;
  0, //  uint8_t  spare6;
  0, //  uint16_t VDDGFX_TVmin;       //Celcius
  0, //  uint16_t VDDGFX_Vmin_HiTemp; // mV Q2
  0, //  uint16_t VDDGFX_Vmin_LoTemp; // mV Q2
  0, //  uint16_t VDDGFX_TVminHystersis; // Celcius

  // Enable automated phase shedding and diode emulation (PSI7)
  1, //  uint8_t VddcrVddPsi7En;
  1, //  uint8_t VddcrSocPsi7En;
  0, //  uint8_t VddcrSrPsi7En;
  0, //  uint8_t PsiDecayConditionVddRail;
  0, //  uint8_t PsiDecayConditionSocRail;
  0, //  uint8_t PsiDecayConditionSrRail;
  1, //  uint8_t PsiDecayConditionOverride; // Override bit to PSI Decay Condition
  0, //  uint8_t spare7;

  1, //  uint8_t  VddmCldoFastStartupEn;
  1, //  uint8_t  VddmCldoOvDetEn;
  0, //  uint16_t PllSscSpreadPercentage; // PLL Spread down percentage, Q8 format. e.g. 192 gives 0.75%
  1, //  uint8_t  FclkDpmOptimizationEn;  //[0: disable; 1: enable]
  0, //  uint8_t  DisplayIdleOptimizationEn;  //[0: disable; 1: enable]
  0, //  uint16_t VddpRegDdrRateThreshold; // DDR RATE thresold for VDDP CLDO regulation
  0, //  uint16_t LclkMaxFreq;            // in MHz
  0, //  uint16_t LclkMinFreq;            // in MHz

  0, //  uint16_t VDD_MISC_Voltage;       // Voltage in mV (used in AOD menu)
  0, //  uint8_t  SbTsiSlaveAddrOverride; // This field must be set, so PMFW override the SB-TSI Slave address
  0, //  uint8_t  SbTsiSlaveAddrSelect;   // SB-TSI Slave Address Select: 0 = 98h, 1 = 9Ah, 2 = 9Ch, 3 = 9Eh, 4 = 90h, 5 = 92h, 6 = 94h, 7 = 96h

  // Slew rate control for VDDCR_VDD rail
  0, //  uint8_t  SlewRateOverrideVdd; // Set this field to indicate the Slew Rate must change
  0, //  uint8_t  SlewRateUpIndexVdd;  // 0 = 2.5 mV/us, 1 = 10 mV/us, 2 = 20 mV/us, 3 = 40 mV/us.
  0, //  uint8_t  SlewRateDownCntlVdd; // 0: Negative equal to Positive Slew Rate, 1: Negative equal to 1/4 Positive Slew Rate
  0, //  uint8_t  spare8;

  // Slew rate control for VDDCR_SOC and VDDCR_SR rails
  0, //  uint8_t  SlewRateOverrideSoc; // Set this field to indicate the Slew Rate must change
  0, //  uint8_t  SlewRateUpIndexSoc;  // 0 = 2.5 mV/us, 1 = 10 mV/us, 2 = 20 mV/us, 3 = 40 mV/us.
  0, //  uint8_t  SlewRateDownCntlSoc; // 0: Negative equal to Positive Slew Rate, 1: Negative equal to 1/4 Positive Slew Rate
  0, //  uint8_t  spare9;

  // PSM GuardBand
  {{0},{0},{0},{0},{0}},//  int8_t   PsmGuardband_INTERNAL[5][4]; //5 frequency points * 4 temperature points
  {{0},{0},{0},{0},{0}}, //  int8_t   PsmGuardband_EXTERNAL[5][4]; //5 frequency points * 4 temperature points

  // Z10 Slew Rate control
  0, //  uint8_t  Z10SlewRateOverride; // Set this field to indicate the Zstates Slew Rate must change
  0, //  uint8_t  Z10UpSlewRateVdd;    // 4 bits [3:0]: Up Slew Rate = [3:0] * 2.5 + 2.5 mV/us
  0, //  uint8_t  Z10UpSlewRateSoc;    // 4 bits [3:0]: Up Slew Rate = [3:0] * 2.5 + 2.5 mV/us
  0, //  uint8_t  Z10UpSlewRateSr;     // 4 bits [3:0]: Up Slew Rate = [3:0] * 2.5 + 2.5 mV/us
  0, //  uint8_t  Z10DownSlewRateVdd;  // 0: Negative equal to Positive Slew Rate, 1: Negative equal to 1/4 Positive Slew Rate
  0, //  uint8_t  Z10DownSlewRateSoc;  // 0: Negative equal to Positive Slew Rate, 1: Negative equal to 1/4 Positive Slew Rate
  0, //  uint8_t  Z10DownSlewRateSr;   // 0: Negative equal to Positive Slew Rate, 1: Negative equal to 1/4 Positive Slew Rate
  0, //  uint8_t  Z8Gate;

  // Core DLDO regulation override for non-secure or unlocked parts
  {0}, //uint8_t  CoreDldoRegulation[8]; // Per core DLDO regulation control (each index is for the corresponding physical core): 0 = bypass, 1 = regulation

  0, //  uint8_t  HeteroCoreManagement;  // 0: Disable; 1: Enable Static; 2: Enable Dynamic
  0, //uint8_t  VRHOTEn;               // 0: VR_HOT throttle disabled, 1: VR_HOT throttle enabled  
  0, //SB1
  0, //SB2

  0, // reserved

  //SPARE FOR FUTURE USE
  {0} //  uint32_t spare12[32];
};

SMU_PP_TABLE gPPTable;

/**
  Populate PPTable with values from PCDs


  @param PPTable Pointer to a PPTable which must be pre allocated.
**/
static void
PopulatePPTable (
  SIL_CONTEXT          *SilContext,
  SMUCLASS_INPUT_BLK   *SmuInputBlock,
  SMU_PP_TABLE         *PPTable
  )
{
  uint8_t *PsmArray;
  FCHCLASS_INPUT_BLK *FchBlk;
  FchBlk = (FCHCLASS_INPUT_BLK *)xUslFindStructure(SilContext, SilId_FchClass, 0);

  memset(PPTable, 0, sizeof (SMU_PP_TABLE));
  memcpy(PPTable, &PPTableDefault, sizeof (SMU_PP_TABLE));

  PPTable->SUSTAINED_POWER_LIMIT = SmuInputBlock->SustainedPowerLimit;
  PPTable->VRM_SOC_CURRENT_LIMIT = SmuInputBlock->VrmSocCurrentLimit;
  PPTable->VRM_VDD_MAXIMUM_CURRENT_LIMIT = SmuInputBlock->VrmMaximumCurrentLimit;
  PPTable->VRM_SOC_MAXIMUM_CURRENT_LIMIT = SmuInputBlock->VrmSocMaximumCurrentLimit;
  PPTable->PROCHOT_L_DEASSERTION_RAMP_TIME = SmuInputBlock->ProchotlDeassertionRampTime;
  PPTable->SYSTEM_CONFIG = SmuInputBlock->CfgSystemConfiguration;

  //CBS debug options
  PPTable->CoreDldoPsmMargin = SmuInputBlock->DldoPsmMargin;
  PPTable->VddcrVddVoltageMargin = (uint16_t) SmuInputBlock->VddcrCpuVoltageMargin;
  PPTable->VddcrSocVoltageMargin = (uint16_t) SmuInputBlock->VddcrSocVoltageMargin;

  //TELEMETRY CALIBRATION
  PPTable->TelemetryVddcrVddSlope = SmuInputBlock->Telemetry_VddcrVddfull_Scale_Current;
  PPTable->TelemetryVddcrVddOffset = SmuInputBlock->Telemetry_VddcrVddOffset;
  PPTable->TelemetryVddcrSocSlope = SmuInputBlock->Telemetry_VddcrSocfull_Scale_Current;
  PPTable->TelemetryVddcrSocOffset = SmuInputBlock->Telemetry_VddcrSocOffset;
  PPTable->TelemetryVddcrSrSlope = SmuInputBlock->Telemetry_VddcrSrfull_Scale_Current;
  PPTable->TelemetryVddcrSrOffset = SmuInputBlock->Telemetry_VddcrSrOffset;

  // Telemetry VR_HOT throttle
  PPTable->VRHOTEn = !!SmuInputBlock->VRHOTEnable;

  //// OVERCLOCKING CONFIG
  PPTable->OC_DISABLE = SmuInputBlock->OcDisable;
  if (PPTable->OC_DISABLE == 0) {
    PPTable->OC_MINVID = SmuInputBlock->OcVoltageMax;
    PPTable->OC_FREQMAX = SmuInputBlock->OcFrequencyMax;
    PPTable->ForceGfxclkFrequency = SmuInputBlock->ForceGfxclkFrequency;
    PPTable->ForceVddcrSocVoltage = SmuInputBlock->ForceVddcrSocVoltage;
    PPTable->ForceVddcrVddVoltage = SmuInputBlock->ForceVddcrCpuVoltage;
  }

  //CCLK FREQUENCY FORCE
  PPTable->CclkFmaxOverride = (uint16_t)SmuInputBlock->FMaxFrequency;
  PPTable->GfxclkFmaxOverride = (uint16_t)SmuInputBlock->GfxclkFmaxOverride;
  PPTable->CclkFminOverride = SmuInputBlock->CclkFminOverride;

 // STT parameters/co-efficients
  PPTable->SYSTEM_TEMPERATURE_TRACKING = SmuInputBlock->SttEnable;
  if (SmuInputBlock->SttEnable == 0x1) {
    PPTable->STT_PCB_SENSOR_COUNT = SmuInputBlock->SttPcbSensorCount;
    PPTable->STT_MIN_POWER_LIMIT = SmuInputBlock->SttMinPowerLimit;
    PPTable->STT_M1 = SmuInputBlock->SttM1;
    PPTable->STT_M2 = SmuInputBlock->SttM2;
    PPTable->STT_M3 = SmuInputBlock->SttM3;
    PPTable->STT_M4 = SmuInputBlock->SttM4;
    PPTable->STT_M5 = SmuInputBlock->SttM5;
    PPTable->STT_M6 = SmuInputBlock->SttM6;
    PPTable->STT_C_APU = SmuInputBlock->SttCApu;
    PPTable->STT_C_HS2 = SmuInputBlock->SttCGpu;
    PPTable->STT_ALPHA_APU = SmuInputBlock->SttAlphaApu;
    PPTable->STT_ALPHA_HS2 = SmuInputBlock->SttAlphaGpu;
    PPTable->STT_SKIN_TEMPERATURE_LIMIT_APU = SmuInputBlock->SttSkinTemperatureLimitApu;
    PPTable->STT_SKIN_TEMPERATURE_LIMIT_HS2 = SmuInputBlock->SttSkinTemperatureLimitGpu;
    PPTable->STT_ERROR_COEFF = SmuInputBlock->SttErrorCoeff;
    PPTable->STT_ERROR_RATE_COEFF = SmuInputBlock->SttErrorRateCoeff;
  }

  // STAPM Control
  PPTable->STAPM_BOOST_OVERRIDE = SmuInputBlock->StapmBoost;
  if (SmuInputBlock->StapmBoost != 0) {
    PPTable->STAPM_TIME_CONSTANT = SmuInputBlock->StapmTimeConstant;
  }

  // CPPC Control
  PPTable->CppcConstraintsEnabled = SmuInputBlock->CppcConstraintsEnabled;
  if (PPTable->CppcConstraintsEnabled == 0x1) {
    // set CPPC as enable
    PPTable->PerfLimit_MaxRange = SmuInputBlock->CppcPerfLimitMaxRange;
    PPTable->PerfLimit_MinRange = SmuInputBlock->CppcPerfLimitMinRange;
    PPTable->EPP_MaxRange = SmuInputBlock->CppcEppMaxRange;
    PPTable->EPP_MinRange = SmuInputBlock->CppcEppMinRange;
  }

  // PWM Control
  PPTable->ForceFanPwmEn = SmuInputBlock->ForceFanPwmEn;
  if (PPTable->ForceFanPwmEn != 0 ) {
    PPTable->ForceFanPwm = SmuInputBlock->ForceFanPwm;
 }

  PPTable->FAST_PPT_LIMIT = SmuInputBlock->FastPptLimit;
  PPTable->SLOW_PPT_LIMIT = SmuInputBlock->SlowPptLimit;
  PPTable->SLOW_PPT_TIME_CONSTANT = SmuInputBlock->SlowPptTimeConstant;
  PPTable->THERMAL_LIMIT = SmuInputBlock->CfgThermCtlValue;
  PPTable->VRM_VDD_CURRENT_LIMIT = SmuInputBlock->VrmCurrentLimit;

  PPTable->OC_GFX_DISABLE = SmuInputBlock->GfxOcDisable;
  if (PPTable->OC_GFX_DISABLE == 0) {
    PPTable->OC_GFX_MINVID = SmuInputBlock->OcGFXMinVID;
    PPTable->OC_GFX_FREQMAX = SmuInputBlock->OcGFXFreqMax;
  }

  PPTable->GfxclkFminOverride = SmuInputBlock->GfxclkFminOverride;

  // Core count control
  PPTable->SW_SCI_GPE_ID = SmuInputBlock->SwSciGpeID;
  PPTable->CoreCountControlEn= SmuInputBlock->CoreCountControlEnable;

  //  STB Verbosity Control SMU
  PPTable->StbVerbosityLevel= SmuInputBlock->StbSmuVerbosityControl;

  //P3T Limit
  if (SmuInputBlock->P3TLimitCtrl != 0) {
    SMU_TRACEPOINT(SIL_TRACE_INFO, "PcdP3TLimit %x\n", SmuInputBlock->P3TLimit);
    PPTable->P3T_LIMIT = SmuInputBlock->P3TLimit;
  }

  // Lclk Frequency
  if (SmuInputBlock->CfgLclkFrequencyRange != 0x0) {
    PPTable->LclkMaxFreq = (uint16_t)SmuInputBlock->CfgLclkFrequencyRange;
    PPTable->LclkMinFreq = (uint16_t)(SmuInputBlock->CfgLclkFrequencyRange) >> 16;
  }

  // VDD MISC Voltage in mV
  if (SmuInputBlock->VddMiscVoltage != 0x0) {
    PPTable->VDD_MISC_Voltage = SmuInputBlock->VddMiscVoltage;
  }

  if (SmuInputBlock->AmdSmuVddcrVddSlewRateIndex != 0xF) {
    PPTable->SlewRateOverrideVdd = 1;
    PPTable->SlewRateUpIndexVdd = SmuInputBlock->AmdSmuVddcrVddSlewRateIndex;
    PPTable->SlewRateDownCntlVdd = SmuInputBlock->AmdSmuVddcrVddSlewRateDownCntl;
  }

  PPTable->AA_MODE_EN = SmuInputBlock->CfgPeApmEnable;

  if (SmuInputBlock->AmdSmuPsiDecayConditionOverride) {
    PPTable->PsiDecayConditionOverride = SmuInputBlock->AmdSmuPsiDecayConditionOverride;
    PPTable->PsiDecayConditionVddRail = SmuInputBlock->AmdSmuPsiDecayConditionVddRail;
    PPTable->PsiDecayConditionSocRail = SmuInputBlock->AmdSmuPsiDecayConditionSocRail;
    PPTable->PsiDecayConditionSrRail = SmuInputBlock->AmdSmuPsiDecayConditionSrRail;
  }

  if (SmuInputBlock->AmdSmuVddcrSocSlewRateOverride) {
    PPTable->SlewRateOverrideSoc = SmuInputBlock->AmdSmuVddcrSocSlewRateOverride;
    PPTable->SlewRateUpIndexSoc = SmuInputBlock->AmdSmuVddcrSocSlewRateIndex;
    PPTable->SlewRateDownCntlSoc = SmuInputBlock->AmdSmuVddcrSocSlewRateDownCntl;
  }

  if (SmuInputBlock->AmdSmuZ10SlewRateOverride) {
    PPTable->Z10SlewRateOverride = SmuInputBlock->AmdSmuZ10SlewRateOverride;
    PPTable->Z10UpSlewRateVdd = SmuInputBlock->AmdSmuZ10UpSlewRateVdd;
    PPTable->Z10UpSlewRateSoc = SmuInputBlock->AmdSmuZ10UpSlewRateSoc;
    PPTable->Z10UpSlewRateSr = SmuInputBlock->AmdSmuZ10UpSlewRateSr;
    PPTable->Z10DownSlewRateVdd = SmuInputBlock->AmdSmuZ10DownSlewRateVdd;
    PPTable->Z10DownSlewRateSoc = SmuInputBlock->AmdSmuZ10DownSlewRateSoc;
    PPTable->Z10DownSlewRateSr = SmuInputBlock->AmdSmuZ10DownSlewRateSr;
  }

  PPTable->DisplayIdleOptimizationEn = SmuInputBlock->DisplayIdleOptimizationEn;

  if (SmuInputBlock->CurveShaperPsmArray != 0) {
    PsmArray = (uint8_t *)(uintptr_t)SmuInputBlock->CurveShaperPsmArray;
    if (PsmArray != NULL) {
      memcpy (PPTable->PsmGuardband_EXTERNAL, PsmArray, sizeof (PPTable->PsmGuardband_EXTERNAL));
    }
  }

  if (SmuInputBlock->AmdSmuSbTsiSlaveAddrOverride) {
    PPTable->SbTsiSlaveAddrOverride = SmuInputBlock->AmdSmuSbTsiSlaveAddrOverride;
    PPTable->SbTsiSlaveAddrSelect = SmuInputBlock->AmdSmuSbTsiSlaveAddrSelect;
  }

  if (FchBlk == NULL) {
    return;
  }

  if (FchBlk->FchRunTime.FchDeviceEnableMap & BIT_32(8)) {
    PPTable->FCH_IOMUX_19 = 1;
    PPTable->FCH_IOMUX_20 = 1;
  }

  if (FchBlk->FchRunTime.FchDeviceEnableMap & BIT_32(15)) {
    PPTable->FCH_IOMUX_19 = 2;
    PPTable->FCH_IOMUX_20 = 2;
  }
}

SIL_STATUS
SmuInitAfterPcieTrainingDonePhx (
  SIL_CONTEXT                   *SilContext
  )
{
  SIL_STATUS                      Status;
  uint32_t                        SmuArg[6];
  MPIO_PRESIL_CTRL1               PreSilCtrl1;
  GNB_HANDLE                      *GnbHandle;
  uint32_t                        PackageType;
  uint32_t                        GPIOTableAddress = 0;
  APOB_IP2IP_API                  *ApobIp2IpApi;
  APOB_ENV_FLAGS_STRUCT           *ApobFlags;
  SMUCLASS_INPUT_BLK              *SmuInputBlock;

  SMU_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  SmuInputBlock = (SMUCLASS_INPUT_BLK *) xUslFindStructure(SilContext, SilId_SmuClass, 0);
  if (SmuInputBlock == NULL) {
    SMU_TRACEPOINT(SIL_TRACE_ERROR, "Failed to find SMU input data blk\n");
    return SilNotFound;
  }

  Status = SilGetIp2IpApi(SilContext, SilId_ApobClass, (void **)(&ApobIp2IpApi));
  if (Status != SilPass) {
    SMU_TRACEPOINT (SIL_TRACE_ERROR, " APOB I2I API is not found.\n");
    return Status;
  }

  Status = SmuGetGnbHandleCommon(SilContext, 0, &GnbHandle);
  if (Status != SilPass) {
    return Status;
  }

  Status = ApobIp2IpApi->ApobAmdGetApobEntryInstance(SilContext,
    APOB_GEN,
    APOB_ENV_FLAGS_INFO_TYPE,
    0,
    0,
    (APOB_TYPE_HEADER **)&ApobFlags
    );
  if (Status != SilPass) {
    SMU_TRACEPOINT (SIL_TRACE_ERROR, " APOB I2I API is not found.\n");
    return Status;
  }

  PreSilCtrl1.Value = ApobFlags->Flags.PreSilCtrl1;
  if (PreSilCtrl1.Field.FASTSIM_DFLT_TBL == 0) {
    PopulatePPTable (SilContext, SmuInputBlock, &gPPTable);
  }

  PackageType = xUSLGetPackageType ();
  SMU_TRACEPOINT(SIL_TRACE_INFO, "PackageType = %d\n", PackageType);

  SmuFixupPlatformConfig(GnbHandle, SmuInputBlock, &gPPTable, PackageType);

  while (GnbHandle != NULL) {
    SMU_TRACEPOINT(SIL_TRACE_INFO, "SMU Version End\n");

    if (PreSilCtrl1.Field.FASTSIM_SMU_MSGS == 0) {
      EnableOrDisableIpDeepSleep (SmuInputBlock, GnbHandle, PackageType);

      assert(((uint64_t)&gPPTable >> 32) != 0);
      //
      //  Pass DRAM space for the PPTable Structure.
      //
      SmuServiceInitArgumentsCommon(SmuArg);
      SmuArg[0] = (uint32_t)((uintptr_t)&gPPTable & 0xFFFFFFFF);
      SmuServiceRequestPhx(GnbHandle->Address, SMC_MSG_SetBiosDramAddr, SmuArg, 0);

      //
      //  Call SMC_MSG_TransferBiosIfTableToSmu to ask SMU to read in the PP Table,
      // SMU reply when the DRAM read is complete.
      //
      SmuServiceInitArgumentsCommon(SmuArg);
      SmuServiceRequestPhx(GnbHandle->Address, SMC_MSG_TransferTableDram2Smu, SmuArg, 0);
  
      SMU_TRACEPOINT(SIL_TRACE_INFO, "2 SMU Version End\n");

      // Get external GPIO table for Z9/S0i3
      GPIOTableAddress = SmuInputBlock->AmdSmuGpioConfigTable;
      if (GPIOTableAddress != 0) {
        //
        //  Pass DRAM space for the GPIO table Structure.
        //
        SmuServiceInitArgumentsCommon(SmuArg);
        SmuArg[0] = GPIOTableAddress;
        SmuServiceRequestPhx(GnbHandle->Address, SMC_MSG_SetBiosDramAddr, SmuArg, 0);
        //
        //  Call SMC_MSG_TransferBiosIfTableToSmu to ask SMU to read in the GPIO Table,
        // SMU reply when the DRAM read is complete.
        //
        SmuServiceInitArgumentsCommon(SmuArg);
        SmuArg[0] = 3;
        SmuServiceRequestPhx(GnbHandle->Address, SMC_MSG_TransferTableDram2Smu, SmuArg, 0);
        SMU_TRACEPOINT(SIL_TRACE_INFO, "GPIO Table to SMU end\n");
      }
    }

   GnbHandle = GnbGetNextHandle (GnbHandle);

  }

  SMU_TRACEPOINT(SIL_TRACE_EXIT, "\n");

  return SilPass;
}
