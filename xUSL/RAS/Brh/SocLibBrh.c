/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  SocLibBrh.c
 * @brief RAS SoC Library functions for BRH
 */

#include <SilCommon.h>
#include <CommonLib/SmnAccess.h>
#include <SMU/Brh/SmuBrhReg.h>
#include <RAS/Brh/RasInitBrh.h>
#include <RAS/RasDefs.h>
#include <RAS/Common/RasReg.h>
#include <DF/Common/SilBaseFabricTopologyLib.h>
#include <MsrReg.h>
#include <Utils.h>
#include <APOB/Common/ApobCmn.h>
#include <APOB/BRH/Apob-BRH.h>

/**
 * ProgramCoreMcaConfigUmcBrh
 *
 * @brief Enable/Disable FRU text for UMC MCA.  When enabled the FRU text is reported in McaSynd1/McaSynd2 MSRs.
 *
 * @param  EnableFruText                Input flag to enable/disable FRU text in MCA.
 * @param EnableFruText Input flag to enable/disable FRU text in MCA.
 *
 */
void
ProgramCoreMcaConfigUmcBrh (
  bool  EnableFruText
  )
{
  uint32_t                    i;
  uint64_t                    MsrData;
  uint8_t                     BankNum;
  SIL_MCA_IPID_MSR            McaIpidMsr;
  uint32_t                    McaConfigMsrAddr;
  MCA_CONFIG_UMC_STRUCT       McaConfigUmc;

  MsrData = xUslRdMsr(MSR_MCG_CAP);           // MCG_CAP
  BankNum = (uint8_t)(MsrData & 0xFF);
  for (i = 0; i < BankNum; i++) {
    if (i == 4) {
      continue;
    }

    McaIpidMsr.Value = xUslRdMsr((MCA_EXTENSION_BASE + ((i * SMCA_REG_PER_BANK) | MCA_IPID_OFFSET))); // MCA_IPID
    if ((McaIpidMsr.Field.HardwareID == 0) ||
      (McaIpidMsr.Field.HardwareID != MCA_UMC_ID)) {
      continue;
    }

    McaConfigMsrAddr = (MCA_EXTENSION_BASE + ((i * SMCA_REG_PER_BANK) | MCA_CONFIG_OFFSET));
    McaConfigUmc.Value = xUslRdMsr(McaConfigMsrAddr);
    McaConfigUmc.Field.McaFruTextInMca = EnableFruText;
    xUslWrMsr(McaConfigMsrAddr, McaConfigUmc.Value);
  }

  return;
}
/**
 * GetUmcPerDieBrh
 *
 * @brief Get the Umc Per Die object for BRH
 *
 * @return uint8_t  The number of unified memory controllers per die
 */
uint8_t
GetUmcPerDieBrh (void)
{
  return (uint8_t) UMC_PER_DIE;
}

/**
 * GetUmcHarvestFuseBrh
 *
 * @brief Get the Umc Harvest Fuse object
 *
 * @param   BusNumberBase The bus base to read the fuse from
 *
 * @return  uint32_t      The fuse register value
 */
uint32_t
GetUmcHarvestFuseBrh (
  size_t    BusNumberBase
  )
{
  SIL_RESERVED_UNION_0044 SmuFuse;

  /*
   * Read SMUFUSE_UMCHARVEST_PUBLICADDR and check the MP0_UMC_CTRL_x_HARVEST bits (bits 24:13). Any UMC's that have
   * their corresponding fuse bit set to 1 are not present and should not be accessed
   */
  SmuFuse.Value = xUSLSmnRead(0, (uint32_t)BusNumberBase, SMUFUSE_UMCHARVEST_PUBLICADDR);
  RAS_TRACEPOINT(SIL_TRACE_INFO,
    "BusNumberBase = %x, SMUFUSE_UMCHARVEST = %x\n",
    BusNumberBase,
    SmuFuse.Value
    );

  return SmuFuse.Field.field_bits_13_to_24;
}

/**
 * UpdateFruTextToUmcBrh
 *
 * @brief Update FRU text into UMC SMN registers
 *
 * @param IohcBus          The Iohc bus number for the memory
 * @param AmdFruTextSEntry The FRU strings to write to UMC SMN registers.
 *
 * @return SIL_STATUS
 *
 * @retval SilInvalidParameter If the FRU text entry module is not 0 or 1.
 * @retval SilPass             The FRU string was updated.
 */
SIL_STATUS
UpdateFruTextToUmcBrh (
  uint32_t            IohcBus,
  SIL_FRUTEXT_ENTRY   *AmdFruTextSEntry
  )
{
  uint32_t UmcBaseAddress;

  if (AmdFruTextSEntry->Channel < MAX_UMC_CHANNEL_PER_DIE) {
    UmcBaseAddress = UMC0_CH_REG_BASE + (UMC_SMN_ADDR_OFFSET * AmdFruTextSEntry->Channel);
  } else {
    return SilInvalidParameter;
  }

  if (AmdFruTextSEntry->Module == 0) {
    UmcBaseAddress |= UMC_MCAFRUTEXTDW0_DIMM0;
  } else if (AmdFruTextSEntry->Module == 1) {
    UmcBaseAddress |= UMC_MCAFRUTEXTDW0_DIMM1;
  } else {
    return SilInvalidParameter;
  }
  RAS_TRACEPOINT(SIL_TRACE_INFO,
    "  Iohc Bus: 0x%x, UMC base address: 0x%08x\n",
    IohcBus,
    UmcBaseAddress
    );

  xUSLSmnWrite((IohcBus / MAX_PCI_BUS_NUMBER_PER_SEGMENT),
    IohcBus,
    UmcBaseAddress,
    AmdFruTextSEntry->AmdFrutextStr.McaFruTextDW0
    );
  xUSLSmnWrite((IohcBus / MAX_PCI_BUS_NUMBER_PER_SEGMENT),
    IohcBus,
    UmcBaseAddress + 0x04,
    AmdFruTextSEntry->AmdFrutextStr.McaFruTextDW1
    );
  xUSLSmnWrite((IohcBus / MAX_PCI_BUS_NUMBER_PER_SEGMENT),
    IohcBus,
    UmcBaseAddress + 0x08,
    AmdFruTextSEntry->AmdFrutextStr.McaFruTextDW2
    );
  xUSLSmnWrite((IohcBus / MAX_PCI_BUS_NUMBER_PER_SEGMENT),
    IohcBus,
    UmcBaseAddress + 0x0C,
    AmdFruTextSEntry->AmdFrutextStr.McaFruTextDW3
    );

  return SilPass;
}

/**
 * ProcessorNumberToPhysicalThread
 *
 * @brief Convert the cpu input information to a physical thread id.
 *
 * @param RasCpuInfo       Cpu info input to convert to thread id
 * @param PhysicalSocketId Socket id output of the logical processor described by the cpu info
 * @param PhysicalThread   The physical thread of the input cpu info
 *
 * @return SIL_STATUS
 */
static
SIL_STATUS
ProcessorNumberToPhysicalThread (
  SIL_CPU_INFO      *RasCpuInfo,
  uint32_t          *PhysicalSocketId,
  uint32_t          *PhysicalThread
  )
{
  uint32_t          MaxCcxPerCcd;
  uint32_t          MaxCoresPerCcx;
  uint32_t          DieId;
  uint32_t          CcxId;
  uint32_t          CoreId;
  uint32_t          ThreadID;

  DieId = RasCpuInfo->DieId;
  CcxId = RasCpuInfo->CcxId;
  CoreId = RasCpuInfo->CoreId;
  ThreadID = RasCpuInfo->ThreadID;
  *PhysicalSocketId = RasCpuInfo->SocketId;

  // BRH - 1 CCX/CCX, 8 Cores/CCX
  MaxCcxPerCcd = MAX_CCX_PER_CCD_BRH;
  MaxCoresPerCcx = MAX_CORES_PER_CCX_BRH;

  *PhysicalThread = (uint32_t)(
    ((((DieId * MaxCcxPerCcd) + CcxId) * MaxCoresPerCcx) + CoreId) * MAX_THREADS_PER_CORE + ThreadID);

  return SilPass;

}

/**
 * HexToAscii
 *
 * @brief Convert the input value to ASCii values
 *
 * Example: 0x1234 => 0x31323334
 *
 * @param   Data   On input, the hex digits to convert.  On output, the ASCii representation.
 * @param   Width  Number of digits you want to convert to ASCii
 *
 */
static
void
HexToAscii (
  uint64_t                  *Data,
  uint8_t                    Width
  )
{
  uint8_t   i;
  uint64_t  Temp;
  uint64_t  Ascii;

  assert(Width <= 8);

  Ascii = 0;
  for (i = 0; i < Width; i++) {
    Temp = ((*Data) >> (4 * i)) & 0xF;
    if (Temp < 0xA) {
      Temp = '0' + Temp;
    } else {
      Temp = 'A' + Temp - 0xA;
    }
    Ascii |= Temp << (8 * i);
  }
  *Data &= ~((1ull << (8 * Width)) - 1);
  *Data |= Ascii;

  return;
}

/**
 * CollectMcaErrorInfoBrh
 *
 * @brief   Function for performing a search for MCA errors through all banks on
 *          a specific thread.
 *
 * @details It is the responsibility of the Host to ensure the input buffer is
 *          sufficient to contain the SIL_RAS_MCA_ERROR_INFO_V2 (defined in
 *          RasClass-api.h).
 *
 * @param   RasMcaErrorInfo Input pointer to a SIL_RAS_MCA_ERROR_INFO_V2 buffer
 */
void
CollectMcaErrorInfoBrh (
  SIL_RAS_MCA_ERROR_INFO_V2 *RasMcaErrorInfo
  )
{
  uint32_t                  i;
  uint32_t                  ErrorCount;
  uint8_t                   BankNum;
  SIL_MCA_STATUS_MSR        McaStatusMsr;
  SIL_MCA_IPID_MSR          McaIpidMsr;
  SIL_MCA_DESTAT_MSR        McaDeStatusMsr;
  MCG_CAP_STRUCT            McgCap;
  bool                      DestatExists;

  McgCap.Value = xUslRdMsr(MSR_MCG_CAP);// MCG_CAP
  BankNum = (uint8_t)McgCap.Field.Count;
  ErrorCount = 0;

  for (i = 0; i < BankNum; i++) {
    McaIpidMsr.Value = xUslRdMsr((MCA_EXTENSION_BASE + ((i * SMCA_REG_PER_BANK) | MCA_IPID_OFFSET))
      );
    if (McaIpidMsr.Field.HardwareID == 0) {
      continue;
    }

    DestatExists = true;
    switch (McaIpidMsr.Field.HardwareID) {
    case MCA_CPU_CORE_ID:
      switch (McaIpidMsr.Field.McaType) {
      case IF_MCA_TYPE:
      case DE_MCA_TYPE:
      case EX_MCA_TYPE:
      case FP_MCA_TYPE:
        DestatExists = false;
        break;
      }
      break;
    }

    //Find error log
    McaStatusMsr.Value = xUslRdMsr((MCA_EXTENSION_BASE + ((i * SMCA_REG_PER_BANK) | MCA_STATUS_OFFSET))
      );
    McaDeStatusMsr.Value = 0;
    if (DestatExists) {
      McaDeStatusMsr.Value = xUslRdMsr((MCA_EXTENSION_BASE + ((i * SMCA_REG_PER_BANK) | MCA_DESTAT_OFFSET))
        );
    }

    if (McaStatusMsr.Field.Val || McaDeStatusMsr.Field.Val) {
      //Collect MSR value
      RasMcaErrorInfo->McaBankErrorInfo[ErrorCount].McaBankNumber = i;
      RasMcaErrorInfo->McaBankErrorInfo[ErrorCount].McaStatusMsr =
        McaStatusMsr.Value;
      RasMcaErrorInfo->McaBankErrorInfo[ErrorCount].McaAddrMsr = xUslRdMsr((MCA_EXTENSION_BASE +
        ((i * SMCA_REG_PER_BANK) | MCA_ADDR_OFFSET))
        );
      RasMcaErrorInfo->McaBankErrorInfo[ErrorCount].McaConfigMsr = xUslRdMsr((MCA_EXTENSION_BASE +
        ((i * SMCA_REG_PER_BANK) | MCA_CONFIG_OFFSET))
        );
      RasMcaErrorInfo->McaBankErrorInfo[ErrorCount].McaIpidMsr = McaIpidMsr.Value;
      RasMcaErrorInfo->McaBankErrorInfo[ErrorCount].McaSyndMsr = xUslRdMsr((MCA_EXTENSION_BASE +
        ((i * SMCA_REG_PER_BANK) | MCA_SYND_OFFSET))
        );
      RasMcaErrorInfo->McaBankErrorInfo[ErrorCount].McaMisc0Msr = xUslRdMsr((MCA_EXTENSION_BASE +
        ((i * SMCA_REG_PER_BANK) | MCA_MISC0_OFFSET))
        );
      RasMcaErrorInfo->McaBankErrorInfo[ErrorCount].McaSynd1Msr = xUslRdMsr((MCA_EXTENSION_BASE +
        ((i * SMCA_REG_PER_BANK) | MCA_SYND1_OFFSET))
        );
      RasMcaErrorInfo->McaBankErrorInfo[ErrorCount].McaSynd2Msr = xUslRdMsr((MCA_EXTENSION_BASE +
        ((i * SMCA_REG_PER_BANK) | MCA_SYND2_OFFSET))
        );
      if (McaIpidMsr.Field.HardwareID == MCA_UMC_ID) {
        RasMcaErrorInfo->McaBankErrorInfo[ErrorCount].McaMisc1Msr = xUslRdMsr((MCA_EXTENSION_BASE +
          ((i * SMCA_REG_PER_BANK) | MCA_MISC1_OFFSET))
          );
      } else {
        RasMcaErrorInfo->McaBankErrorInfo[ErrorCount].McaMisc1Msr = 0;
      }
      if (DestatExists) {
        RasMcaErrorInfo->McaBankErrorInfo[ErrorCount].McaDeStatMsr = xUslRdMsr((MCA_EXTENSION_BASE +
          ((i * SMCA_REG_PER_BANK) | MCA_DESTAT_OFFSET))
          );
        RasMcaErrorInfo->McaBankErrorInfo[ErrorCount].McaDeAddrMsr = xUslRdMsr((MCA_EXTENSION_BASE +
          ((i * SMCA_REG_PER_BANK) | MCA_DEADDR_OFFSET))
          );
      } else {
        RasMcaErrorInfo->McaBankErrorInfo[ErrorCount].McaDeStatMsr = 0;
        RasMcaErrorInfo->McaBankErrorInfo[ErrorCount].McaDeAddrMsr = 0;
      }
      if (ErrorCount < XMCA_MAX_BANK_COUNT) {
        ErrorCount++;
      }
    }
  }  //for (i = 0; i < BankNum; i++)
  RasMcaErrorInfo->McaBankCount = ErrorCount;
}

/**
 * UpdateMcaFruTextBrh
 *
 * @brief Update FRU Text for each MCA bank in the processor described by the cpu info input.
 *
 * @param RasCpuInfo  Cpu information for a logical processor.
 *
 */
void
UpdateMcaFruTextBrh (
  SIL_CPU_INFO      *RasCpuInfo
  )
{
  uint32_t                    SocketId;
  uint32_t                    ThreadId;
  MCA_CONFIG_UMC_STRUCT       McaConfig;
  uint32_t                    McaBankIndex = 0;
  uint64_t                    Temp = 0;
  //Mca FruText Format :Perr Sx:Txx:Bxx
  uint64_t             MSR_SYND1 = 0x506572722053003A; // initial to {' ','P','e','r','r',' ','S',0} for Socket.
  uint64_t             MSR_SYND2 = 0x540000003A420000; // initial to {'T', 0 , 0 , 0 ,':','B',0, 0 } for Thread, Bank.
  MCG_CAP_STRUCT       McgCap;
  uint8_t              BankNum;

  ProcessorNumberToPhysicalThread(RasCpuInfo, &SocketId, &ThreadId);

  Temp = (uint64_t)ThreadId;
  HexToAscii(&Temp, 3);
  MSR_SYND2 |= (Temp << 32);

  Temp = (uint64_t)SocketId;
  HexToAscii(&Temp, 1);
  MSR_SYND1 |= (Temp << 8);

  McgCap.Value = xUslRdMsr(MSR_MCG_CAP); //MCG_CAP
  BankNum = (uint8_t)McgCap.Field.Count;
  for (McaBankIndex = 0; McaBankIndex < BankNum; McaBankIndex++) {
    McaConfig.Value = xUslRdMsr((MCA_EXTENSION_BASE + ((McaBankIndex * SMCA_REG_PER_BANK) | MCA_CONFIG_OFFSET)));
    if (McaConfig.Value == 0) {  //Unimplemented MCA Bank
      continue;
    }
    McaConfig.Field.McaFruTextInMca = 1;  //BIT9: McaFruTextInMca
    xUslWrMsr((MCA_EXTENSION_BASE + ((McaBankIndex * SMCA_REG_PER_BANK) | MCA_CONFIG_OFFSET)), McaConfig.Value);

    MSR_SYND2 &= ~0x000000000000FFFF;
    Temp = (uint64_t)McaBankIndex;
    HexToAscii(&Temp, 2);
    MSR_SYND2 |= Temp;

    //Update the MCA_SYND1 and MCA_SYND2 for all MCA Banks.
    xUslWrMsr((MCA_EXTENSION_BASE + ((McaBankIndex * SMCA_REG_PER_BANK) | MCA_SYND1_OFFSET)),
      xUslSwapBytes64(MSR_SYND1)
      );
    xUslWrMsr((MCA_EXTENSION_BASE + ((McaBankIndex * SMCA_REG_PER_BANK) | MCA_SYND2_OFFSET)),
      xUslSwapBytes64(MSR_SYND2)
      );
  }

  return;
}
