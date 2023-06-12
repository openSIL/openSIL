/**
 * @file  Ras.c
 * @brief openSIL RAS IP initialization.
 *
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <xSIM.h>
#include "Ras.h"
#include <RAS/RasDefs.h>
#include <string.h>
#include <MsrReg.h>
#include <CommonLib/CpuLib.h>

/// Default values for Ras configuration
static const RASCLASS_DATA_BLK gRasConfigData = {
  .RasInputBlock = {
    .Temp        = 0x1,
  },
  .RasOutputBlock = {
    .Temp        = 0xFF,
  }
};

/**
 * SetNbioMcaCtlMask
 *
 * @brief   Return the MCA_CTL_MASK_NBIO (MCA::NBIO::MCA_CTL_MASK_NBIO) configuration
 *          based on host input parameters.
 *
 * @param   NbioRasPolicy NBIO policy configuration from host-FW.  Refer to
 *                        SIL_NBIO_RAS_POLICY in RasClass-api.h for configuration
 *                        structure details.
 *
 * @returns uint64_T
 *
 * @retval  The MCA Control Mask value setting based on the host firware policy
 *          input.
 */
static
uint64_t
SetNbioMcaCtlMask (
  SIL_NBIO_RAS_POLICY   *NbioRasPolicy
  )
{
  MCA_CTL_MASK_NBIO_STRUCT  McaCtlMask;

  // Determine MCA_CTL_MASK value and program all instances. First initialize it to zero
  McaCtlMask.Value = 0;
  // When MaskNbioSyncFlood is TRUE mask Ext_ErrEvent
  if (NbioRasPolicy->MaskNbioSyncFlood == true) {
    McaCtlMask.Field.Ext_ErrEvent = 1;
  }

  // Mask PCIe sideband if not MCA reporting
  if (NbioRasPolicy->PcieAerReportMechanism != 0) {
    McaCtlMask.Field.PCIE_Sideband = 1;
  }

  // If RasControl is MCA, leave EccParityError 0. If its legacy or disabled set it to 1.
  if (NbioRasPolicy->NbioRasControlV2 != 1) {
    McaCtlMask.Field.EccParityError = 1;
  }

  // When NbioPoisonConsumption is TRUE mask Egress_Poison
  if (NbioRasPolicy->NbioPoisonConsumption == true) {
    McaCtlMask.Field.Egress_Poison = 1;
  }

  return McaCtlMask.Value;
}

/**
 * SetIpMcaCtlMask
 *
 * @brief Set MCA_CTL_MASK MSR for an IP MCA bank.
 *
 * @note  This is a common function
 *
 * @param HardwareId      Hardware ID input to match with MCA Bank MCA IP ID.
 *
 * @param McaType         MCA Type input to match with MCA Bank MCA IP ID.
 * * @param IpMcaPolicyCfg  Host firmware input containing platform policy information that is used to configure MCA
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

  /*
   * Call IP specific functions for setting the MCA Control Mask depending
   * on configuration parameters provided by the host firmware.
   */
  switch (HardwareId) {
    case MCA_NBIO_ID:
      McaCtlMask = SetNbioMcaCtlMask (&IpMcaPolicyCfg->NbioRasPolicy);
      break;
    default:
      McaCtlMask = 0; // For invalid HardwareID, do not mask any controls
  }

  // MSR read to MCG_CAP (MSR0000_0179) to get the value of the Count field
  McgCap.Value = xUslRdMsr (MSR_MCG_CAP); // MCG_CAP
  BankCount = (uint8_t)(McgCap.Field.Count);

  // For all banks
  for (BankNumber = 0; BankNumber < BankCount; BankNumber++) {
    // MSR read to MCA_IPID (MSRC000_2005 + (0x10 (bank size) * bank number))
    McaIpid.Value = xUslRdMsr (((MCA_EXTENSION_BASE + (BankNumber * SMCA_REG_PER_BANK)) | MCA_IPID_OFFSET));
    // Check for matching bank
    if ((McaIpid.Field.HardwareID == HardwareId) && (McaIpid.Field.McaType == McaType)) {
      // Program the MCA_CTL_MASK register for this IP
      McaCtlMaskAddr = MCA_CTL_MASK_BASE + BankNumber;
      xUslWrMsr (McaCtlMaskAddr, McaCtlMask);

      /**
       * Debug code for dumping MCA_CTL_MASK_XXX MSR where XXX = IP name
       *
      McaCtlMask = xUslRdMsr (McaCtlMaskAddr);
      RAS_TRACEPOINT (SIL_TRACE_INFO,
                      "HardwareID: 0x%x , McaType = 0x%x McaCtlMask = 0x%x \n",
                      HardwareId,
                      McaType,
                      McaCtlMask
                      );
       */
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

  CcdId     = RasCpuInfo->DieId;
  CoreId    = RasCpuInfo->CoreId;
  ThreadId  = RasCpuInfo->ThreadID;
  SocketId  = RasCpuInfo->SocketId;

  for (CoreMcaBankIndex = 0; CoreMcaBankIndex < MAX_CORE_MCA_BANK_COUNT; CoreMcaBankIndex++) {
    if (CoreMcaBankIndex == 4) {
      continue;
    }

    McaIpidMsr.Value = xUslRdMsr ((MCA_EXTENSION_BASE + ((CoreMcaBankIndex * SMCA_REG_PER_BANK) | MCA_IPID_OFFSET)));

    if (McaIpidMsr.Field.HardwareID != MCA_CPU_CORE_ID) {
      continue; //Should not be here
    }

    switch (McaIpidMsr.Field.McaType) {
      case LS_MCA_TYPE:
        CoreMcaSmnAddrByte1 = LS_THR0_SMNADDR_BYTE1;
        break;
      case IF_MCA_TYPE:
        CoreMcaSmnAddrByte1 = IF_THR0_SMNADDR_BYTE1;
        break;
      case L2_MCA_TYPE:
        CoreMcaSmnAddrByte1 = L2_THR0_SMNADDR_BYTE1;
        break;
      case DE_MCA_TYPE:
        CoreMcaSmnAddrByte1 = DE_THR0_SMNADDR_BYTE1;
        break;
      case EX_MCA_TYPE:
        CoreMcaSmnAddrByte1 = EX_THR0_SMNADDR_BYTE1;
        break;
      case FP_MCA_TYPE:
        CoreMcaSmnAddrByte1 = FP_THR0_SMNADDR_BYTE1;
        break;
      default: //Should not be here
        CoreMcaSmnAddrByte1 = 0;
        break;
    }

    McaIpidMsr.Field.InstanceId = (((0x2000 | ((CcdId * 8) << 4) | (CoreId * 2)) << 16) |
      ((CoreMcaSmnAddrByte1 + ThreadId) << 8));
    McaIpidMsr.Field.InstanceIdHi = SocketId;
    xUslWrMsr ((MCA_EXTENSION_BASE + ((CoreMcaBankIndex * SMCA_REG_PER_BANK) | MCA_IPID_OFFSET)), McaIpidMsr.Value);
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

  RasConfigData = (RASCLASS_DATA_BLK *)SilCreateInfoBlock (SilId_RasClass,
                                  sizeof(RASCLASS_DATA_BLK),
                                  RASCLASS_INSTANCE,
                                  RASCLASS_MAJOR_REV,
                                  RASCLASS_MINOR_REV);
  RAS_TRACEPOINT (SIL_TRACE_INFO,
    "openSIL RasClassSetInputBlk at: 0x%x \n",
    RasConfigData);

  if (RasConfigData == NULL) {
    return SilAborted;
  }

  // Fill RAS structure with defaults
  memcpy ((void *)RasConfigData, &gRasConfigData, sizeof(RASCLASS_DATA_BLK));

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

  UNUSED (RasConfigData);

  return Status;
}
