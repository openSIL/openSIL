/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file SmuInitPhx.c
 * @brief PHX SMU core silicon initialization code.
 *
 */

#include <SilCommon.h>
#include <SMU/Common/SmuCommon.h>
#include <Utils.h>
#include <CpuLib.h>
#include <string.h>
#include <CoreTopologyService.h>
#include <APOB/Common/ApobCmn.h>
#include <ProjSocConst.h>
#include <APOB/ApobIp2Ip.h>
#include "SmuPhxReg.h"
#include "SmuPhxIp2Ip.h"
#include "SmuInitPhx.h"

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
