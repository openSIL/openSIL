/* SPDX-License-Identifier: MIT */
/**
 * Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */
/**
 * @file  Ras.c
 * @brief openSIL RAS IP initialization.
 *
 */

#include <xSIM.h>
#include "Ras.h"
#include <RAS/RasDefs.h>
#include <string.h>
#include <MsrReg.h>
#include <CommonLib/CpuLib.h>
#include <RAS/Common/RasCmn2Rev.h>

/// Default values for Ras configuration
static const RASCLASS_DATA_BLK gRasConfigData = {
  .RasInputBlock = {
    .Temp = 0x1,
  },
  .RasOutputBlock = {
    .Temp = 0xFF,
  }
};

/**
 * SetIpMcaCtlMask
 *
 * @brief Set MCA_CTL_MASK MSR for an IP MCA bank.
 *
 * @note  This is a common function
 *
 * @param HardwareId      Hardware ID input to match with MCA Bank MCA IP ID. Refer to the Processor Programming
 *                        Reference (PPR) for details on acceptable HardwareID values for each IP.
 * @param McaType         MCA Type input to match with MCA Bank MCA IP ID. Refer to the Processor Programming
 *                        Reference (PPR) for details on acceptable McaType values for each IP.
 * @param IpMcaPolicyCfg  Host firmware input containing platform policy information that is used to configure MCA
 *                        Control mask settings. The structure is a union of all supported IP RAS policy structure. The
 *                        host is responsible for populating the appropriate IP structure with valid policy information.
 *                        Currently supported IP structures:
 *
 *                        - SIL_NBIO_RAS_POLICY
 *
 */
void
SetIpMcaCtlMask (
  uint16_t          HardwareId,
  uint16_t          McaType,
  SIL_IP_RAS_POLICY *IpMcaPolicyCfg
  )
{
  SIL_MCA_IPID_MSR  McaIpid;
  MCG_CAP_STRUCT    McgCap;
  uint8_t           BankNumber;
  uint8_t           BankCount;
  uint32_t          McaCtlMaskAddr;
  uint64_t          McaCtlMask;

  McaCtlMask = 0; // For invalid HardwareID, do not mask any controls

  // MSR read to MCG_CAP (MSR0000_0179) to get the value of the Count field
  McgCap.Value = xUslRdMsr(MSR_MCG_CAP);// MCG_CAP
  BankCount = (uint8_t)(McgCap.Field.Count);

  // For all banks
  for (BankNumber = 0; BankNumber < BankCount; BankNumber++) {
    // MSR read to MCA_IPID (MSRC000_2005 + (0x10 (bank size) * bank number))
    McaIpid.Value = xUslRdMsr(((MCA_EXTENSION_BASE + (BankNumber * SMCA_REG_PER_BANK)) | MCA_IPID_OFFSET));
    // Check for matching bank
    if ((McaIpid.Field.HardwareID == HardwareId) && (McaIpid.Field.McaType == McaType)) {
      // Program the MCA_CTL_MASK register for this IP
      McaCtlMaskAddr = MCA_CTL_MASK_BASE + BankNumber;
      xUslWrMsr(McaCtlMaskAddr, McaCtlMask);
    }
  }
}

/**
 * ProgramCoreMcaIpIdInstanceId
 *
 * @brief Program the Core MCA_IPID MSR Instance ID values for the CPU specified in the SIL_CPU_INFO input parameter.
 *
 * @details The instance ID is initialized to a unique ID per instance of the register.  Valid range of values is
 *          defined in the Processor Programming Reference.
 *
 * @param RasCpuInfo The CPU info structure for the core to program.
 *
 */
void
ProgramCoreMcaIpIdInstanceId (
  SIL_CPU_INFO *RasCpuInfo
  )
{
  uint32_t          CoreMcaBankIndex;
  SIL_MCA_IPID_MSR  McaIpidMsr;
  uint8_t           CcdId;
  uint8_t           CoreId;
  uint8_t           ThreadId;
  uint8_t           CoreMcaSmnAddrByte1;
  uint8_t           SocketId;
  RAS_XFER_TABLE    *RasXfer;
  SIL_STATUS        Status;

  Status = SilGetCommon2RevXferTable(SilId_RasClass, (void **) &RasXfer);
  if (Status != SilPass) {
    RAS_TRACEPOINT(SIL_TRACE_ERROR, "RAS C2R not found!\n");
    assert(Status == SilPass);
    return;
  }

  CcdId = RasCpuInfo->DieId;
  CoreId = RasCpuInfo->CoreId;
  ThreadId = RasCpuInfo->ThreadID;
  SocketId = RasCpuInfo->SocketId;

  for (CoreMcaBankIndex = 0; CoreMcaBankIndex < MAX_CORE_MCA_BANK_COUNT; CoreMcaBankIndex++) {
    if (CoreMcaBankIndex == 4) {
      continue;
    }

    McaIpidMsr.Value = xUslRdMsr((MCA_EXTENSION_BASE + ((CoreMcaBankIndex * SMCA_REG_PER_BANK) | MCA_IPID_OFFSET)));

    if (McaIpidMsr.Field.HardwareID != MCA_CPU_CORE_ID) {
      continue; //Should not be here
    }

    CoreMcaSmnAddrByte1 = RasXfer->GetCoreMcaSmnAddrByte((uint16_t)McaIpidMsr.Field.McaType);

    McaIpidMsr.Field.InstanceId = (((0x2000 | ((CcdId * 8) << 4) | (CoreId * 2)) << 16) |
      ((CoreMcaSmnAddrByte1 + ThreadId) << 8));
    McaIpidMsr.Field.InstanceIdHi = SocketId;
    xUslWrMsr((MCA_EXTENSION_BASE + ((CoreMcaBankIndex * SMCA_REG_PER_BANK) | MCA_IPID_OFFSET)), McaIpidMsr.Value);
  }

  return;
}

/**
 * RasClassSetInputBlk
 *
 * @brief   Establish RAS config data
 *
 * @return  SIL_STATUS
 */
SIL_STATUS
RasClassSetInputBlk (void)
{
  RASCLASS_DATA_BLK *RasConfigData;

  RasConfigData = (RASCLASS_DATA_BLK *)SilCreateInfoBlock(SilId_RasClass,
    sizeof (RASCLASS_DATA_BLK),
    RASCLASS_INSTANCE,
    RASCLASS_MAJOR_REV,
    RASCLASS_MINOR_REV
    );
  RAS_TRACEPOINT(SIL_TRACE_INFO,
    "SIL RasClassSetInputBlk at: 0x%x \n",
    RasConfigData
    );

  if (RasConfigData == NULL) {
    return SilAborted;
  }

  // Fill RAS structure with defaults
  memcpy((void *)RasConfigData, &gRasConfigData, sizeof (RASCLASS_DATA_BLK));

  return SilPass;
}

/**
 * InitializeRas
 *
 * @brief   TBD
 *
 * @details TBD
 *
 * @param   RasConfigData   RAS input and output data block
 *
 * @return  SIL_STATUS  initialization status
 *
 * @retval  TBD
 */
SIL_STATUS
InitializeRas (
  RASCLASS_DATA_BLK *RasConfigData
  )
{
  SIL_STATUS    Status = SilPass;

  UNUSED(RasConfigData);

  return Status;
}
