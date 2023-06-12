/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  SmuInitBrh.c
 * @brief OpenSIL SMU services specific to BRH
 *
 */

#include <SilCommon.h>
#include <SMU/Common/SmuCommon.h>
#include <Utils.h>
#include <CpuLib.h>
#include <string.h>
#include <CoreTopologyService.h>
#include <APOB/Common/ApobCmn.h>
#include <APOB/ApobIp2Ip.h>
#include <APOB/BRH/Apob-BRH.h>
#include "SmuBrhReg.h"
#include "SmuIp2IpBrh.h"
#include "SmuInitBrh.h"

const uint32_t  Mp5CoreDisableList[] = {
  SIL_RSVD_ADDR_25BC2451,
  SIL_RSVD_ADDR_43AF61E4,
  SIL_RSVD_ADDR_9205EC2C,
  SIL_RSVD_ADDR_A810A5EC,
  SIL_RSVD_ADDR_5D416F23,
  SIL_RSVD_ADDR_9F73B7B7,
  SIL_RSVD_ADDR_959340B7,
  SIL_RSVD_ADDR_B1DD628A,
  SIL_RSVD_ADDR_D21F5DE9,
  SIL_RSVD_ADDR_2188B4FF,
  SIL_RSVD_ADDR_2564FA4C,
  SIL_RSVD_ADDR_2FF37BCD,
  SIL_RSVD_ADDR_CF8F3723,
  SIL_RSVD_ADDR_21D9AECE,
  SIL_RSVD_ADDR_7963FAC0,
  SIL_RSVD_ADDR_D9265EF2,
};

/**
 * SmuGetOpnCorePresenceExBrh
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
SmuGetOpnCorePresenceExBrh (
  uint32_t                  SystemDieNumber,
  uint32_t                  *CcdEnableMap,
  uint32_t                  *CcdDisableMap,
  uint32_t                  *CoreDisMap,
  uint32_t                  *CoreDisMapBufferSize,
  uint32_t                  *CoreDisMapLength,
  bool                      *SmtEnable,
  uint32_t                  *SmtEnableBufferSize
  )
{
  SIL_STATUS                Status;
  uint32_t                  CcdDownFuse;
  uint32_t                  CcdPresentFuse1;
  uint32_t                  CoreFuse;
  uint32_t                  MaxCoresPerCcx;
  uint32_t                  CcdLoop;
  uint32_t                  Mp5CoreDisListValue;
  APOB_APCB_UPDATES_STRUCT  *ApobEntry;
  APOB_IP2IP_API            *ApobIp2IpApi;

  Status = SilGetIp2IpApi(SilId_ApobClass, (void **) &ApobIp2IpApi);
  if ((Status != SilPass) || (ApobIp2IpApi == NULL)) {
    assert(Status == SilPass);
    return Status;
  }

  if ((CcdEnableMap == NULL) && (CcdDisableMap == NULL) && (CoreDisMap == NULL) && (SmtEnable == NULL)) {
    Status = SilInvalidParameter;
  } else if (((CoreDisMap != NULL) && (CoreDisMapBufferSize == NULL)) ||
    ((SmtEnable != NULL) && (SmtEnableBufferSize == NULL))) {
    Status = SilInvalidParameter;
  } else if (((CoreDisMap != NULL) && (*CoreDisMapBufferSize < sizeof (CoreDisMap[0]) * MAX_CCDS_PER_IOD)) ||
    ((SmtEnable != NULL) && (*SmtEnableBufferSize < sizeof (SmtEnable[0]) * MAX_CCDS_PER_IOD))) {
    Status = SilOutOfBounds;
    if (CoreDisMap != NULL) {
      *CoreDisMapBufferSize = sizeof (CoreDisMap[0]) * MAX_CCDS_PER_IOD;
    }
    if (SmtEnable != NULL) {
      *SmtEnableBufferSize = sizeof (SmtEnable[0]) * MAX_CCDS_PER_IOD;
    }
  } else {
    Status = ApobIp2IpApi->ApobAmdGetApobEntryInstance(APOB_APCB,
      APOB_APCB_UPDATES_STRUCT_TYPE,
      0,
      APOB_BASE_ADDRESS,
      (APOB_TYPE_HEADER **) &ApobEntry
      );
    if (Status != SilPass) {
      SMU_TRACEPOINT(SIL_TRACE_INFO, "Failed to get ApobEntry. Return Status: %x\n", Status);
      return Status;
    }

    Status = ApobIp2IpApi->ApobGetApcbUpdate(ApobEntry, SIL_RSVD_ADDR_2690210C, &CcdPresentFuse1);
    assert(Status == SilPass);
    if (Status != SilPass) {
      SMU_TRACEPOINT(SIL_TRACE_ERROR, "CcdPresentFuse1 not found.  Setting CcdPresentFuse1 = 0\n");
      CcdPresentFuse1 = 0;
    }

    if (CcdEnableMap != NULL) {
      *CcdEnableMap = CcdPresentFuse1;
    }

    if (CcdDisableMap != NULL) {
      Status = ApobIp2IpApi->ApobGetApcbUpdate(ApobEntry, SIL_RSVD_ADDR_6BF64BCD, &CcdDownFuse);
      assert(Status == SilPass);
      if (Status != SilPass) {
        SMU_TRACEPOINT(SIL_TRACE_ERROR, "CcdDownFuse not found.  Setting CcdDownFuse = 0\n");
        CcdDownFuse = 0;
      }
      //
      // Only report the disabled CCDs bits which is present in a max OPN
      //
      *CcdDisableMap = CcdDownFuse & CcdPresentFuse1;
    }

    if (SmtEnable != NULL) {
      Status = ApobIp2IpApi->ApobGetApcbUpdate(ApobEntry, SIL_RSVD_ADDR_E56D9DFD, &CoreFuse);
      assert(Status == SilPass);
      if (Status != SilPass) {
        SMU_TRACEPOINT(SIL_TRACE_ERROR, "CoreFuse not found.  Setting CoreFuse = 0xFFFFFFFF\n");
        CoreFuse = 0xFFFFFFFF;
      }
      //
      // Only report the disabled CCDs bits which is present in a max OPN
      //
      *SmtEnableBufferSize = sizeof (SmtEnable[0]) * MAX_CCDS_PER_IOD;
      for (CcdLoop = 0; CcdLoop < MAX_CCDS_PER_IOD; CcdLoop++) {
        if ((CcdPresentFuse1 & (1 << CcdLoop)) != 0) {
          SmtEnable[CcdLoop] = (bool) ((CoreFuse & (1 << CcdLoop)) != 0);
        } else {
          SmtEnable[CcdLoop] = false;
        }
      }
    }

    MaxCoresPerCcx = CCX_MAX_CORES_PER_COMPLEX / 2;

    if (CoreDisMap != NULL) {
      *CoreDisMapBufferSize = sizeof (CoreDisMap[0]) * MAX_CCDS_PER_IOD;
      for (CcdLoop = 0; CcdLoop < SIL_ARRAY_SIZE(Mp5CoreDisableList); CcdLoop++) {
        Status = ApobIp2IpApi->ApobGetApcbUpdate(ApobEntry, Mp5CoreDisableList[CcdLoop], &Mp5CoreDisListValue);
        assert(Status == SilPass);
        if (Status != SilPass) {
          Mp5CoreDisListValue = 0;
        }
        if ((CcdPresentFuse1 & (1 << CcdLoop)) != 0) {
          CoreDisMap[CcdLoop] = Mp5CoreDisListValue;
        } else {
          CoreDisMap[CcdLoop] = (1 << MaxCoresPerCcx) - 1;
        }
      }
    }

    if (CoreDisMapLength != NULL) {
      *CoreDisMapLength = MaxCoresPerCcx;
    }
    Status = SilPass;
  }

  return Status;
}

/**
 * SmuGetOpnCorePresenceBrh
 * @brief This service retrieves information on the OPN's core thread presence.
 *
 * @param SystemDieNumber   Logical system die number to check.
 * @param CcdEnableMap      Pointer to the OPN default enabled CCD map.
 * @param CcdDisableMap     Pointer to the OPN disabled/removed CCD map.
 * @param CoreDisMap        Pointer to the OPN disabled core map.
 * @param CoreDisMapLength  Pointer to the OPN disabled core map length in bits.
 * @param SmtEnable         Pointer to whether or not the OPN supports SMT.
 *
 * @retval SilPass        Presence info was successfully retrieved.
 */
SIL_STATUS
SmuGetOpnCorePresenceBrh (
  uint32_t                  SystemDieNumber,
  uint32_t                  *CcdEnableMap,
  uint32_t                  *CcdDisableMap,
  uint32_t                  *CoreDisMap,
  uint32_t                  *CoreDisMapLength,
  bool                      *SmtEnable
  )
{
  SIL_STATUS                Status;
  uint32_t                  CcdPresentFuse1;
  uint32_t                  CcdDownFuse;
  uint32_t                  CoreFuse;
  uint32_t                  CurrDisCoresMap;
  uint32_t                  DisCoresMap;
  uint32_t                  CurrDisCoresCount;
  uint32_t                  DisCoresCount;
  uint32_t                  MaxCoresPerCcx;
  uint32_t                  CcdIndex;
  uint32_t                  Mp5CoreDisListValue;
  APOB_APCB_UPDATES_STRUCT  *ApobEntry;
  APOB_IP2IP_API            *ApobIp2IpApi;

  Status = SilGetIp2IpApi(SilId_ApobClass, (void **) &ApobIp2IpApi);
  if ((Status != SilPass) || (ApobIp2IpApi == NULL)) {
    assert(Status == SilPass);
    return Status;
  }

  if ((CcdEnableMap == NULL) && (CcdDisableMap == NULL) && (CoreDisMap == NULL) && (SmtEnable == NULL)) {
    Status = SilInvalidParameter;
  } else {
    Status = ApobIp2IpApi->ApobAmdGetApobEntryInstance(APOB_APCB,
      APOB_APCB_UPDATES_STRUCT_TYPE,
      0,
      APOB_BASE_ADDRESS,
      (APOB_TYPE_HEADER **) &ApobEntry
      );
    if (Status != SilPass) {
      SMU_TRACEPOINT(SIL_TRACE_INFO, "Failed to get ApobEntry. Return Status: %x\n", Status);
      return Status;
    }

    Status = ApobIp2IpApi->ApobGetApcbUpdate(ApobEntry, SIL_RSVD_ADDR_2690210C, &CcdPresentFuse1);
    assert(Status == SilPass);
    if (Status != SilPass) {
      SMU_TRACEPOINT(SIL_TRACE_ERROR, "CcdPresentFuse1 not found.  Setting CcdPresentFuse1 = 0\n");
      CcdPresentFuse1 = 0;
    }

    if (CcdEnableMap != NULL) {
      *CcdEnableMap = CcdPresentFuse1;
    }

    if (CcdDisableMap != NULL) {
      Status = ApobIp2IpApi->ApobGetApcbUpdate(ApobEntry, SIL_RSVD_ADDR_6BF64BCD, &CcdDownFuse);
      assert(Status == SilPass);
      if (Status != SilPass) {
        SMU_TRACEPOINT(SIL_TRACE_ERROR, "CcdDownFuse not found.  Setting CcdDownFuse = 0\n");
        CcdDownFuse = 0;
      }
      //
      // Only report the disabled CCDs bits which is present in a max OPN
      //
      *CcdDisableMap = CcdDownFuse & CcdPresentFuse1;
    }

    if (SmtEnable != NULL) {
      Status = ApobIp2IpApi->ApobGetApcbUpdate(ApobEntry, SIL_RSVD_ADDR_E56D9DFD, &CoreFuse);
      assert(Status == SilPass);
      if (Status != SilPass) {
        SMU_TRACEPOINT(SIL_TRACE_ERROR, "CoreFuse not found.  Setting CoreFuse = 0x1\n");
        CoreFuse = 0x1;
      }
      //
      // Only report the disabled CCDs bits which is present in a max OPN
      //
      *SmtEnable = (bool) (CoreFuse != 0);
    }

    // With Asymmetric Downcoring, some CCD, usually the highest-numbered, can have more cores
    // disabled than others. Determine the highest number of disabled cores on a CCD
    DisCoresMap = 0;
    DisCoresCount = 0;

    MaxCoresPerCcx = CCX_MAX_CORES_PER_COMPLEX / 2;

    for (CcdIndex = 0; CcdIndex < SIL_ARRAY_SIZE(Mp5CoreDisableList); CcdIndex++) {
      Status = ApobIp2IpApi->ApobGetApcbUpdate(ApobEntry, Mp5CoreDisableList[CcdIndex], &Mp5CoreDisListValue);
      assert(Status == SilPass);
      if (Status != SilPass) {
        Mp5CoreDisListValue = 0;
      }
      if ((CcdPresentFuse1 & (1 << CcdIndex)) != 0) {
        CurrDisCoresMap = Mp5CoreDisListValue;

        CurrDisCoresCount = xUslGetSetBitCount(CurrDisCoresMap);

        if (CurrDisCoresCount > DisCoresCount) {
          DisCoresCount = CurrDisCoresCount;
          DisCoresMap = CurrDisCoresMap;
        }
      }
    }
    if (CoreDisMap != NULL) {
      *CoreDisMap = DisCoresMap;
    }

    if (CoreDisMapLength != NULL) {
      *CoreDisMapLength = MaxCoresPerCcx;
    }

    Status = SilPass;
  }

  return Status;
}

/**
 * SmuServiceRequestBrh
 *
 * @brief   SMU service request for Breithorn
 *
 * @param   PciAddress      PCI_ADDR of this NBIO
 * @param   RequestId       BIOS to SMU Message ID
 * @param   RequestArgument Request Argument
 * @param   AccessFlags     See GNB_ACCESS_FLAGS_* definitions
 *
 * @retval  SMC_RESULT
 */
SMC_RESULT
SmuServiceRequestBrh (
  PCI_ADDR PciAddress,
  uint32_t RequestId,
  uint32_t *RequestArgument,
  uint32_t AccessFlags
  )
{
  uint32_t SmuMessageResponse;

  SMU_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (RequestId > SIL_RESERVED_1760) {
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

  SmuRegisterRead(PciAddress, BRH_MP1_C2PMSG_RESPONSE_ADDRESS, &SmuMessageResponse, 0);

  // 1 Clear Response
  SmuMessageResponse = 0;
  SmuRegisterWrite(PciAddress, BRH_MP1_C2PMSG_RESPONSE_ADDRESS, SmuMessageResponse, AccessFlags);

  // 2 Write message arguments
  SmuRegisterWrite(PciAddress, BRH_MP1_C2PMSG_ARGUMENT_0_ADDRESS, RequestArgument[0], AccessFlags);
  SmuRegisterWrite(PciAddress, BRH_MP1_C2PMSG_ARGUMENT_1_ADDRESS, RequestArgument[1], AccessFlags);
  SmuRegisterWrite(PciAddress, BRH_MP1_C2PMSG_ARGUMENT_2_ADDRESS, RequestArgument[2], AccessFlags);
  SmuRegisterWrite(PciAddress, BRH_MP1_C2PMSG_ARGUMENT_3_ADDRESS, RequestArgument[3], AccessFlags);
  SmuRegisterWrite(PciAddress, BRH_MP1_C2PMSG_ARGUMENT_4_ADDRESS, RequestArgument[4], AccessFlags);
  SmuRegisterWrite(PciAddress, BRH_MP1_C2PMSG_ARGUMENT_5_ADDRESS, RequestArgument[5], AccessFlags);

  // 3 Write message ID
  SmuRegisterWrite(PciAddress, BRH_MP1_C2PMSG_MESSAGE_ADDRESS, (uint32_t)RequestId, AccessFlags);

  // 4 Poll Response until non-zero
  do {
    SmuRegisterRead(PciAddress, BRH_MP1_C2PMSG_RESPONSE_ADDRESS, &SmuMessageResponse, 0);
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
  SmuRegisterRead(PciAddress, BRH_MP1_C2PMSG_ARGUMENT_0_ADDRESS, &RequestArgument[0], 0);
  SmuRegisterRead(PciAddress, BRH_MP1_C2PMSG_ARGUMENT_1_ADDRESS, &RequestArgument[1], 0);
  SmuRegisterRead(PciAddress, BRH_MP1_C2PMSG_ARGUMENT_2_ADDRESS, &RequestArgument[2], 0);
  SmuRegisterRead(PciAddress, BRH_MP1_C2PMSG_ARGUMENT_3_ADDRESS, &RequestArgument[3], 0);
  SmuRegisterRead(PciAddress, BRH_MP1_C2PMSG_ARGUMENT_4_ADDRESS, &RequestArgument[4], 0);
  SmuRegisterRead(PciAddress, BRH_MP1_C2PMSG_ARGUMENT_5_ADDRESS, &RequestArgument[5], 0);

  SMU_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return (SMC_RESULT)SmuMessageResponse;
}

/**
 * SmuServiceRequestOnlyBrh
 *
 * @brief   SMU service request for Breithorn
 *
 * @details This function differs from SmuServiceRequestBrh in that is send the SMU request but does not wait for
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
SmuServiceRequestOnlyBrh (
  PCI_ADDR PciAddress,
  uint32_t RequestId,
  uint32_t *RequestArgument,
  uint32_t AccessFlags
  )
{
  uint32_t SmuMessageResponse;
  SMU_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (RequestId > SIL_RESERVED_1760) {
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

  SmuRegisterRead(PciAddress, BRH_MP1_C2PMSG_RESPONSE_ADDRESS, &SmuMessageResponse, 0);

  // 1 Clear Response
  SmuMessageResponse = 0;
  if (AccessFlags) {
    SmuRegisterWrite(PciAddress, BRH_MP1_C2PMSG_RESPONSE_ADDRESS, SmuMessageResponse, AccessFlags);

    // 2 Write message arguments
    SmuRegisterWrite(PciAddress, BRH_MP1_C2PMSG_ARGUMENT_0_ADDRESS, RequestArgument[0], AccessFlags);
    SmuRegisterWrite(PciAddress, BRH_MP1_C2PMSG_ARGUMENT_1_ADDRESS, RequestArgument[1], AccessFlags);
    SmuRegisterWrite(PciAddress, BRH_MP1_C2PMSG_ARGUMENT_2_ADDRESS, RequestArgument[2], AccessFlags);
    SmuRegisterWrite(PciAddress, BRH_MP1_C2PMSG_ARGUMENT_3_ADDRESS, RequestArgument[3], AccessFlags);
    SmuRegisterWrite(PciAddress, BRH_MP1_C2PMSG_ARGUMENT_4_ADDRESS, RequestArgument[4], AccessFlags);
    SmuRegisterWrite(PciAddress, BRH_MP1_C2PMSG_ARGUMENT_5_ADDRESS, RequestArgument[5], AccessFlags);

    // 3 Write message ID
    SmuRegisterWrite(PciAddress, BRH_MP1_C2PMSG_MESSAGE_ADDRESS, (uint32_t)RequestId, 0);
  } else {
    // 4 Poll Response until non-zero
    do {
      SmuRegisterRead(PciAddress, BRH_MP1_C2PMSG_RESPONSE_ADDRESS, &SmuMessageResponse, 0);
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
    SmuRegisterRead(PciAddress, BRH_MP1_C2PMSG_ARGUMENT_0_ADDRESS, &RequestArgument[0], AccessFlags);
    SmuRegisterRead(PciAddress, BRH_MP1_C2PMSG_ARGUMENT_1_ADDRESS, &RequestArgument[1], AccessFlags);
    SmuRegisterRead(PciAddress, BRH_MP1_C2PMSG_ARGUMENT_2_ADDRESS, &RequestArgument[2], AccessFlags);
    SmuRegisterRead(PciAddress, BRH_MP1_C2PMSG_ARGUMENT_3_ADDRESS, &RequestArgument[3], AccessFlags);
    SmuRegisterRead(PciAddress, BRH_MP1_C2PMSG_ARGUMENT_4_ADDRESS, &RequestArgument[4], AccessFlags);
    SmuRegisterRead(PciAddress, BRH_MP1_C2PMSG_ARGUMENT_5_ADDRESS, &RequestArgument[5], AccessFlags);
  }

  SMU_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return (SMC_RESULT)SmuMessageResponse;
}

/**
 * SmuNotifyS3EntryBrh
 *
 * @brief SMU S3 Entry Notification
 *
 */
void
SmuNotifyS3EntryBrh (void)
{
  PCI_ADDR PciAddress;
  uint32_t RequestId;
  uint32_t RxSmuMessageResponse;
  PciAddress.AddressValue = MAKE_SBDFO(0, 0, 0, 0, 0);
  SmuRegisterRead(PciAddress, BRH_MP1_C2PMSG_RESPONSE_ADDRESS, &RxSmuMessageResponse, 0);

  // 1 Clear Response
  RxSmuMessageResponse = 0;
  SmuRegisterWrite(PciAddress, BRH_MP1_C2PMSG_RESPONSE_ADDRESS, RxSmuMessageResponse, 0);

  // 2 Clear Message Arguments
  SmuRegisterWrite(PciAddress, BRH_MP1_C2PMSG_ARGUMENT_0_ADDRESS, RxSmuMessageResponse, 0);
  SmuRegisterWrite(PciAddress, BRH_MP1_C2PMSG_ARGUMENT_1_ADDRESS, RxSmuMessageResponse, 0);
  SmuRegisterWrite(PciAddress, BRH_MP1_C2PMSG_ARGUMENT_2_ADDRESS, RxSmuMessageResponse, 0);
  SmuRegisterWrite(PciAddress, BRH_MP1_C2PMSG_ARGUMENT_3_ADDRESS, RxSmuMessageResponse, 0);
  SmuRegisterWrite(PciAddress, BRH_MP1_C2PMSG_ARGUMENT_4_ADDRESS, RxSmuMessageResponse, 0);
  SmuRegisterWrite(PciAddress, BRH_MP1_C2PMSG_ARGUMENT_5_ADDRESS, RxSmuMessageResponse, 0);

  // 3 Write message ID
  RequestId = SMC_MSG_SleepEntry;
  SmuRegisterWrite(PciAddress, BRH_MP1_C2PMSG_MESSAGE_ADDRESS, RequestId, 0);

  // Halt execution
  xUslCpuSleep ();
}

/**
 * SmuLaunchThreadBrh
 *
 * @details This service will start a core to fetch its first instructions from the reset vector.  This service may
 *          only be called from the BSP.
 *
 * @param   Socket   Zero-based socket number of the target thread.
 * @param   Die      Zero-based die number within Socket of the target thread.
 * @param   Ccd      Zero-based logical core complex die of the target thread.
 * @param   Complex  Zero-based logical complex number of the target thread.
 * @param   Core     Zero-based logical core number of the target thread.
 * @param   Thread   Zero-based logical thread number of the target thread.
 *
 * @retval SilPass              The thread was successfully launched.
 * @retval SilDeviceError       The thread has already been launched.
 * @retval SilInvalidParameter  Socket is non-existent.
 * @retval SilInvalidParameter  Die is non-existent.
 * @retval SilInvalidParameter  Complex is non-existent.
 * @retval SilInvalidParameter  Thread is non-existent.
 * @retval SilInvalidParameter  Ccd is non-existent.
 * @retval SilInvalidParameter  Core is non-existent.
 * @retval SilInvalidParameter  Could not find CcdLogToPhysMap from APOB.
 *
 **/
SIL_STATUS
SmuLaunchThreadBrh (
  uint32_t                                      Socket,
  uint32_t                                      Die,
  uint32_t                                      Ccd,
  uint32_t                                      Complex,
  uint32_t                                      Core,
  uint32_t                                      Thread
  )
{
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT  ApobCcdLogToPhysMap;
  SIL_STATUS                                    Status;
  uint8_t                                       i;
  uint8_t                                       NumberOfLogicalThreads;
  uint32_t                                      WhichBit;
  uint32_t                                      ThreadEnableAdress;
  uint32_t                                      Register32;
  APOB_IP2IP_API                                *ApobIp2IpApi;
  APOB_SOC_DIE_INFO                             SocMaxDieInfo;

  SMU_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Status = SilGetIp2IpApi(SilId_ApobClass, (void **) &ApobIp2IpApi);
  if ((Status != SilPass) || (ApobIp2IpApi == NULL)) {
    assert(Status == SilPass);
    return Status;
  }
  ApobIp2IpApi->ApobGetMaxDieInfo(&SocMaxDieInfo);

  if (Socket >= SocMaxDieInfo.MaxSocSocketsSupportedValue) {
    return SilInvalidParameter;
  }
  if (Die >= SocMaxDieInfo.MaxSocDiesPerSocketValue) {
    return SilInvalidParameter;
  }
  if (Ccd >= SocMaxDieInfo.MaxSocCcdsPerDieValue) {
    return SilInvalidParameter;
  }
  if (Complex >= SocMaxDieInfo.MaxSocCcxPerCcdValue) {
    return SilInvalidParameter;
  }
  if (Core >= SocMaxDieInfo.MaxSocCoresPerComplexValue) {
    return SilInvalidParameter;
  }

  Status = ApobIp2IpApi->ApobGetCcdLogToPhysMap(Socket, Die, &ApobCcdLogToPhysMap);
  if (Status != SilPass) {
    return SilInvalidParameter;
  }

  if (ApobCcdLogToPhysMap.CcdMap[Ccd].PhysCcdNumber == CCX_NOT_PRESENT) {
    return SilInvalidParameter;
  }
  if (ApobCcdLogToPhysMap.CcdMap[Ccd].ComplexMap[Complex].PhysComplexNumber == CCX_NOT_PRESENT) {
    return SilInvalidParameter;
  }
  if (ApobCcdLogToPhysMap.CcdMap[Ccd].ComplexMap[Complex].CoreInfo[Core].PhysCoreNumber ==
    CCX_NOT_PRESENT) {
    return SilInvalidParameter;
  }
  if (!ApobCcdLogToPhysMap.CcdMap[Ccd].ComplexMap[Complex].CoreInfo[Core].IsThreadEnabled[Thread]) {
    return SilInvalidParameter;
  }

  for (i = 0; i < SocMaxDieInfo.MaxSocThreadPerCore; i++) {
    if (!ApobCcdLogToPhysMap.CcdMap[0].ComplexMap[0].CoreInfo[0].IsThreadEnabled[i]) {
      break;
    }
  }
  NumberOfLogicalThreads = i;

  ThreadEnableAdress = SIL_RSVD_ADDR_203C0020 | (ApobCcdLogToPhysMap.CcdMap[Ccd].PhysCcdNumber << 23);
  SMU_TRACEPOINT(SIL_TRACE_INFO,
    "Logical CCD %d, Physical CCD %d, ThreadEnableAddress:0x%x\n",
    Ccd,
    ApobCcdLogToPhysMap.CcdMap[Ccd].PhysCcdNumber,
    ThreadEnableAdress
    );

  WhichBit = (Core * NumberOfLogicalThreads) + Thread;

  Status = SmuRegisterReadDie(Socket, ThreadEnableAdress, &Register32);
  if (Status == SilPass) {
    SMU_TRACEPOINT(SIL_TRACE_INFO, "ThreadEnable register read: 0x%x\n", Register32);
    Register32 |= (0x1) << WhichBit;
    SMU_TRACEPOINT(SIL_TRACE_INFO, "ThreadEnable register write: 0x%x\n", Register32);
    Status = SmuRegisterWriteDie(Socket, ThreadEnableAdress, &Register32);
    assert(Status == SilPass);
  }

  SMU_TRACEPOINT(SIL_TRACE_EXIT, "\n");

  return Status;
}

/**
 * SmuReadBistInfoBrh
 *
 * @brief Retrieve BIST info from SMU
 *
 * @param   InstanceId  The unique identifier of the SMU instance associated
 *                      with this socket (For BRH, the socket number)
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
SmuReadBistInfoBrh (
  uint8_t   InstanceId,
  uint8_t   PhysicalCcx,
  uint32_t  *BistInfo
  )
{
  uint32_t                    SmuArg[6];
  SIL_STATUS                  Status;
  SMC_RESULT                  SmcStatus;
  GNB_HANDLE                  *GnbHandle;
  APOB_IP2IP_API              *ApobIp2IpApi;

  SMU_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Status = SilGetIp2IpApi(SilId_ApobClass, (void **) &ApobIp2IpApi);
  if ((Status != SilPass) || (ApobIp2IpApi == NULL)) {
    assert(Status == SilPass);
    return Status;
  }

  if (BistInfo == NULL) {
    return SilInvalidParameter;
  }

  Status = SmuGetGnbHandleCommon(InstanceId, &GnbHandle);

  if (Status == SilPass) {
    SmuServiceInitArgumentsCommon(SmuArg);
    SmuArg[0] = PhysicalCcx;
    SmcStatus = SmuServiceRequestBrh(GnbHandle->Address,
      SIL_RESERVED_1762,
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
 * SmuReadBrandStringBrh
 *
 * @brief   Returns the brand string read from SMN space.
 *
 * @param   InstanceId        The unique identifier of the SMU instance
 *                            associated with this socket (for GN, the socket
 *                            number)
 * @param   BrandStringLength Number of characters to be read. This value does
 *                            NOT have to be a multiple of 4. NBIO driver code
 *                            will be responsible for alignment, endianness,
 *                            and string length.
 * @param   BrandString       Pointer to a CHAR array to store the brand string.
 *
 * @return  SIL_STATUS
 * @retval  SilNotFound       IP transfer table was not found
 **/
SIL_STATUS
SmuReadBrandStringBrh (
  uint32_t  InstanceId,
  uint32_t  BrandStringLength,
  uint8_t   *BrandString
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
  APOB_IP2IP_API              *ApobIp2IpApi;

  SMU_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Status = SilGetIp2IpApi(SilId_ApobClass, (void **) &ApobIp2IpApi);
  if ((Status != SilPass) || (ApobIp2IpApi == NULL)) {
    assert(Status == SilPass);
    return Status;
  }

  Status = SmuGetGnbHandleCommon(InstanceId, &GnbHandle);
  if (Status == SilPass) {
    SMU_TRACEPOINT(SIL_TRACE_INFO, "GnbHandle Found.\n");
    StringCount = BrandStringLength;
    StringPtr = BrandString;
    Iteration = 0;
    while (StringCount != 0) {
      SmuServiceInitArgumentsCommon(SmuArg);
      SmuArg[0] = Iteration++;
      SmuResult = SmuServiceRequestBrh(GnbHandle->Address,
        SIL_RESERVED_1761,
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
 * SmuReadCacWeightsBrh
 *
 * @brief Returns the CAC weights read from SMU.  Not supported in BRH
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
SmuReadCacWeightsBrh (
  uint32_t MaxNumWeights,
  uint64_t *ApmWeights
  )
{
  UNUSED(MaxNumWeights);
  UNUSED(ApmWeights);

  return SilUnsupported;
}
