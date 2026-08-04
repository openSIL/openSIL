/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2024 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file GfxPhx.c
 *  @brief GFX configuration routines
 */

#include <string.h>
#include <stdint.h>
#include <SilCommon.h>
#include <APOB/Common/ApobCmn.h>
#include <CommonLib/Mmio.h>
#include <CommonLib/SmnAccess.h>
#include <FCH/Common/FchCommon.h>
#include <FCH/Common/Include/FchPMRegs.h>
#include <GFX/Common/Gfx.h>
#include <GFX/GfxClass-api.h>
#include <Nbio/NbioIp2Ip.h>
#include <Nbio/Phx/includePHX/PHX_GnbRegistersPhx.h>
#include <Nbio/Phx/includePHX/PHX_AZALIA.h>
#include <Nbio/Phx/includePHX/PHX_IOHC.h>
#include <MsrReg.h>
#include "GfxInitPhx.h"
#include "GfxPhx.h"
#include <GFX/Common/GfxV4.h>

#define MAX_DMCUB_SIZE                (1024 * 1024) //1024KB is max DMCUB size
#define MAX_IP_DISCOVERY_SIZE         (64 * 1024)   //64KB is max IP discovery size

/**
 * ApplyIntegratedSysInfoOverride
 *
 * @brief To apply IntegratedSysInfo Override
 *
 * @param SysInfoBuffer      Pointer to ATOM_FUSION_SYSTEM_INFO_V6 buffer
 *
 * @return SIL_STATUS
 */
static void
ApplyIntegratedSysInfoOverridePhx (
  SIL_CONTEXT                    *SilContext,
  GNB_HANDLE                     *GnbHandle,
  ATOM_FUSION_SYSTEM_INFO_V6     *SysInfoBuffer
  )
{
  uint32_t             DdrType;
  uint32_t             NumOfUmcPerSoc;
  uint32_t             NumOfUmcPerChan;
  uint32_t             NumOfActiveChan;
  uint32_t             Umc;
  uint32_t             AddrMaskDimm0;
  uint32_t             AddrMaskDimm1;
  uint32_t             UmcConfig;

  // Soc relevant
  NumOfUmcPerSoc = 2;
  NumOfUmcPerChan = 0;

  // Check Dram Type
  DdrType = 0;
  for (Umc = 0; Umc < NumOfUmcPerSoc; Umc++) {
    UmcConfig = 0;
    UmcConfig = xUSLSmnRead(
                  GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  0x50100 + (Umc << 20));
    DdrType |= (UmcConfig & 0x7);
  }


  switch (DdrType) {
  case 1:  // DDR5
    NumOfUmcPerChan = 1;
    SysInfoBuffer->sIntegratedSysInfo.memorytype = Ddr5MemType;
    break;
  case 6:  // LPDDR5
    NumOfUmcPerChan = 1;
    SysInfoBuffer->sIntegratedSysInfo.memorytype = LpDdr5MemType;
    break;
  default:
    GFX_TRACEPOINT(SIL_TRACE_ERROR, "Invalid Dram Type!\n");
    assert (false);
    break;
  }

  GFX_TRACEPOINT(SIL_TRACE_INFO, "Memory Type = 0x%x\n", SysInfoBuffer->sIntegratedSysInfo.memorytype);

  // Check Active Package Channel
  NumOfActiveChan = 0;
  for (Umc = 0; Umc < NumOfUmcPerSoc; Umc += NumOfUmcPerChan) {
    AddrMaskDimm0 = xUSLSmnRead(
                      GnbHandle->Address.Address.Segment,
                      GnbHandle->Address.Address.Bus,
                      0x50020 + (Umc << 20));
    AddrMaskDimm1 = xUSLSmnRead(
                      GnbHandle->Address.Address.Segment,
                      GnbHandle->Address.Address.Bus,
                      0x50024 + (Umc << 20));

    if ((AddrMaskDimm0 | AddrMaskDimm1) != 0) {
      NumOfActiveChan ++;
    }
  }

  SysInfoBuffer->sIntegratedSysInfo.umachannelnumber = (uint8_t) NumOfActiveChan;
  GFX_TRACEPOINT(SIL_TRACE_INFO, "Umachannelnumber %x\n", SysInfoBuffer->sIntegratedSysInfo.umachannelnumber);
}

static void
WriteRecoveryBiosram (
  void
  )
{
  xUSLMemReadModifyWrite8((void *)(size_t)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PM_ISACONTROL),
    (uint8_t) ~BIT_8(1),
    BIT_8(1)
    );

  xUSLMemWrite8((void *)(size_t)(0xFED10088), 0x5A);
}

/**
 * ProgramUMARegister
 *
 * @brief ProgramUMARegister for PHX
 *
 * @return SIL_STATUS
 */
static SIL_STATUS
ProgramUMARegister (
  GFXCLASS_INPUT_BLK             *InputBlk,
  GNB_HANDLE                     *GnbHandle,
  MEMORY_HOLE_DESCRIPTOR         *MemoryRange,
  bool                           IsRecovery
  )
{
  uint64_t                        TopOfMemory;
  uint64_t                        TopOfMemory2;
  uint32_t                        Value;

  GFX_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  if (IsRecovery && InputBlk->PeiGopEnable) {
    if (ISSOCPHXAM5) {
      GFX_TRACEPOINT (SIL_TRACE_INFO, "PHX or PHX2 AM5 SMU MemoryRange->Base %lx \n", MemoryRange->Base);
      if ((MemoryRange->Base > 0x100000000ULL) && (sizeof(uintptr_t) < 8)) {
        GFX_TRACEPOINT (SIL_TRACE_INFO, "UMA is above4G, set Recovery Flag as 0x5A for ABL setting \n");
        WriteRecoveryBiosram();
        GFX_TRACEPOINT (SIL_TRACE_INFO, "Issue cf9 reset\n");
        return SilResetRequestColdImm;
      }
    }
  }

  InputBlk->PeiGopConfigMemsize = MemoryRange->Size >> 20;
  InputBlk->PeiGopVmFbOffset = MemoryRange->Base >> 24;
  InputBlk->PeiGopVmFbLocationTop = (MemoryRange->Base + MemoryRange->Size - 1) >> 24;
  GFX_TRACEPOINT (SIL_TRACE_INFO, "PeiGopConfigMemsize : %lx\n", InputBlk->PeiGopConfigMemsize);
  GFX_TRACEPOINT (SIL_TRACE_INFO, "PeiGopVmFbOffset : %lx\n", InputBlk->PeiGopVmFbOffset);
  GFX_TRACEPOINT (SIL_TRACE_INFO, "PeiGopVmFbLocationTop : %lx\n", InputBlk->PeiGopVmFbLocationTop);

  TopOfMemory = xUslRdMsr(MSR_TOM);
  xUSLSmnReadModifyWrite(
    GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    GNBREGx6a350,
    ~(uint32_t)(0xff800000),
    (uint32_t)TopOfMemory
    );

  TopOfMemory2 = xUslRdMsr(MSR_TOM2);
  xUSLSmnReadModifyWrite(
    GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    GNBREGx6a354,
    ~(uint32_t)(0xff800001),
    (uint32_t)(TopOfMemory2 | 1)
    );

  xUSLSmnReadModifyWrite(
    GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    GNBREGx6a358,
    ~(uint32_t)(0xfff),
    (uint32_t)(TopOfMemory2 >> 32)
    );

  Value = xUSLSmnRead(
               GnbHandle->Address.Address.Segment,
               GnbHandle->Address.Address.Bus,
               IOHCx13b1005c);
  xUSLSmnWrite(
    GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    GNBREGx6a340,
    Value);

  Value = xUSLSmnRead(
               GnbHandle->Address.Address.Segment,
               GnbHandle->Address.Address.Bus,
               IOHCx13b10060);
  xUSLSmnWrite(
    GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    GNBREGx6a344,
    Value);

  Value = xUSLSmnRead(
               GnbHandle->Address.Address.Segment,
               GnbHandle->Address.Address.Bus,
               IOHCx13b00084);
  xUSLSmnReadModifyWrite(
    GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    GNBREGx6a34c,
    ~BIT_32(3),
    Value & BIT_32(3)
    );

  Value = xUSLSmnRead(
               GnbHandle->Address.Address.Segment,
               GnbHandle->Address.Address.Bus,
               IOHCx13b0004c);

  xUSLSmnReadModifyWrite(
    GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    GNBREGx6a348,
    ~BIT_32(23),
    Value & BIT_32(23)
    );

  if (InputBlk->CfgMaxNumAudioEndpoints != 0) {
    Value = (7 - InputBlk->CfgMaxNumAudioEndpoints) & 7;
    Value |= BIT_32(4);
    xUSLSmnWrite(
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      AZALIAxe370,
      Value);
  }

  if (InputBlk->CfgDisableAllNumAudioEndpoints) {
    xUSLSmnWrite(
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      AZALIAxe370,
      (7 | BIT_32(4))
      );
  }

  GFX_TRACEPOINT (SIL_TRACE_EXIT, "\n");

  return SilPass;
}

/**
 * GfxInitPhx
 *
 * @brief Gfx driver entry point for PHX
 *
 * @param[in]  SilContext         Pointer to openSIL context
 * @param[in]  InputBlk           Pointer to the GFX IP block input data
 *
 * @retval     SIL_STATUS
 */
SIL_STATUS
GfxInitPhx (
  SIL_CONTEXT         *SilContext,
  GFXCLASS_INPUT_BLK  *InputBlk
  )
{
  SIL_STATUS                      Status;
  ATOM_FUSION_SYSTEM_INFO_V6      SystemInfoTable;
  uintptr_t                       UMAPeiGopDestination;
  bool                            ProgramValues;
  MEMORY_HOLE_DESCRIPTOR          Range = {0};
  GNB_HANDLE                      *GnbHandle;
  bool                            IsRecovery;
  ATOM_DISPLAY_PHY_TUNING_INFO    *DispPhyTuningInfo;
  NBIO_IP2IP_API                  *NbioIp2Ip;
  NBIOCLASS_DATA_BLOCK            *NbioData;

  GFX_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  IsRecovery = false;
  UMAPeiGopDestination = 0;
  IsRecovery = (InputBlk->BootMode == 0x20); /* EFI_BOOT_IN_RECOVERY_MODE */

  NbioData = GetNbioBlockData(SilContext);
  if (NbioData != NULL) {
    InputBlk->CfgIgpuControl = NbioData->NbioConfigData.CfgIgpuControl;
    /* Sync with NBIO data, as it could change the config value based on fuse */
    InputBlk->CfgIgpuControl = NbioData->NbioConfigData.CfgIgpuControl;
  }

  if ((InputBlk->AmdPreSilCtrl1 & BIT_32(12)) != 0) {
    return SilPass;
  }

  if (SilGetIp2IpApi(SilContext, SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    GFX_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return SilNotFound;
  }

  GnbHandle = NbioIp2Ip->GetGnbHandle(SilContext);
  if (GnbHandle == NULL) {
    return SilNotFound;
  }

  // Get memory location for system info table
  Status = GetUmaInformation(SilContext, &Range);
  ProgramValues = (Status == SilPass);

  if (!ProgramValues) {
    return SilPass;
  }

  if (SilFchReadSleepType () == 0x3) {
    return SilPass;
  }

  PopulateSystemInfoTablePhx(SilContext, InputBlk, &SystemInfoTable);
  ApplyIntegratedSysInfoOverridePhx(SilContext, GnbHandle, &SystemInfoTable);
  Status = ProgramUMARegister(InputBlk, GnbHandle, &Range, IsRecovery);
  /* ProgramUMARegister may return reset request, so return immediately */
  if (Status != SilPass) {
    return Status;
  }

  if ((IsRecovery || (sizeof(uintptr_t) == 8)) && InputBlk->PeiGopEnable) {
    /*
     * In this case, UMA integrated table need report in PEI phase.
     * Copy integrated table to UMA - IP discovery - DMCUB.
     */
    UMAPeiGopDestination = (uintptr_t)(Range.Base + Range.Size);
    UMAPeiGopDestination -= MAX_IP_DISCOVERY_SIZE;
    UMAPeiGopDestination -= MAX_DMCUB_SIZE;
    UMAPeiGopDestination -= sizeof(ATOM_FUSION_SYSTEM_INFO_V6);

    GFX_TRACEPOINT(SIL_TRACE_INFO, "UMAPeiGopDestination %p\n", (void *)UMAPeiGopDestination);

    memcpy((void *)UMAPeiGopDestination, &SystemInfoTable, sizeof(ATOM_FUSION_SYSTEM_INFO_V6));

    // UMA Top - IP Discovery - Integrated table - Phy header - phy tunning setting data
    UMAPeiGopDestination -= sizeof(ATOM_COMMON_TABLE_HEADER);
    DispPhyTuningInfo = (ATOM_DISPLAY_PHY_TUNING_INFO *)SilFindStructure(SilContext,
                                                          SilId_GfxClass,
                                                          GFXCLASS_N6_INSTANCE
                                                          );

    if (DispPhyTuningInfo != NULL) {
      GFX_TRACEPOINT(SIL_TRACE_INFO, "Done DispPhyTuningInfo->table_header.structuresize %x\n",
                    DispPhyTuningInfo->table_header.structuresize);
      GFX_TRACEPOINT(SIL_TRACE_INFO, "Done DispPhyTuningInfo->table_header.content_revision %x\n",
                    DispPhyTuningInfo->table_header.content_revision);
      GFX_TRACEPOINT(SIL_TRACE_INFO, "Done DispPhyTuningInfo->table_header.format_revision %x\n",
                    DispPhyTuningInfo->table_header.format_revision);

      memcpy ((void *)UMAPeiGopDestination,
        (void *)&DispPhyTuningInfo->table_header,
        sizeof(ATOM_COMMON_TABLE_HEADER)
        );

      GFX_TRACEPOINT(SIL_TRACE_INFO, "Done ATOM_COMMON_TABLE_HEADER %p\n", (void *)UMAPeiGopDestination);
      UMAPeiGopDestination = (UMAPeiGopDestination + sizeof(ATOM_COMMON_TABLE_HEADER)) - (DispPhyTuningInfo->table_header.structuresize);
      memcpy((void *)UMAPeiGopDestination,
        (void *)&DispPhyTuningInfo->disp_phy_tuning,
        (DispPhyTuningInfo->table_header.structuresize - sizeof(ATOM_COMMON_TABLE_HEADER)));

      GFX_TRACEPOINT(SIL_TRACE_INFO, "Done UMAPeiGopDestination %p\n", (void *)UMAPeiGopDestination);
    }
  }

  Status = GfxProgramVgaEn(SilContext);
  if (Status != SilPass) {
    GFX_TRACEPOINT(SIL_TRACE_ERROR, "Failed to program VgaEn register\n");
  }

  GFX_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  return SilPass;
}

void
SilDumpDdiTable (
  DDI_DESCRIPTOR         *DdiConfigData
  )
{
  uint8_t index;

  for (index = 0; index < 5; index++) {
    GFX_TRACEPOINT(SIL_TRACE_INFO,
      "SIL: Ddi ConnectorType 0x%x \n",
      DdiConfigData[index].Ddi.ConnectorType
      );
    GFX_TRACEPOINT(SIL_TRACE_INFO,
      "SIL: Ddi AuxInde 0x%x \n",
      DdiConfigData[index].Ddi.AuxIndex
      );
    GFX_TRACEPOINT(SIL_TRACE_INFO,
      "SIL: Ddi HdpIndex 0x%x \n",
      DdiConfigData[index].Ddi.HdpIndex
      );
    GFX_TRACEPOINT(SIL_TRACE_INFO,
      "SIL: Ddi LanePnInversionMask 0x%x \n",
      DdiConfigData[index].Ddi.LanePnInversionMask
      );
    GFX_TRACEPOINT(SIL_TRACE_INFO,
      "SIL: Ddi Flags 0x%x \n",
      DdiConfigData[index].Ddi.Flags
      );
    GFX_TRACEPOINT(SIL_TRACE_INFO, "SIL: Flags 0x%x \n", DdiConfigData[index].Flags);
  }

}
