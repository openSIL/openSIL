/**
 * @file  SocLib.c
 * @brief RAS SoC Library functions for Genoa
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include <CommonLib/SmnAccess.h>
#include <SMU/V13/SmuV13Reg.h>
#include <RAS/Genoa/RasGenoa.h>
#include <RAS/RasDefs.h>
#include <RAS/Common/RasReg.h>
#include <DF/Common/SilBaseFabricTopologyLib.h>
#include <MsrReg.h>

/**
 * ProgramCoreMcaConfigUmcGenoa
 *
 * @brief Enable/Disable FRU text for UMC MCA.  When enabled the FRU text is reported in McaSynd1/McaSynd2 MSRs.
 *
 * @param EnableFruText Input flag to enable/disable FRU text in MCA.
 *
 */
void
ProgramCoreMcaConfigUmcGenoa (
  bool  EnableFruText
)
{
  MCA_CONFIG_UMC_STRUCT McaConfigUmc;

  McaConfigUmc.Value = xUslRdMsr (MSR_MCA_CONFIG_UMC_EVEN_INST_ADDRESS);
  McaConfigUmc.Field.McaFruTextInMca = EnableFruText;
  xUslWrMsr (MSR_MCA_CONFIG_UMC_EVEN_INST_ADDRESS, McaConfigUmc.Value);

  McaConfigUmc.Value = xUslRdMsr (MSR_MCA_CONFIG_UMC_ODD_INST_ADDRESS);
  McaConfigUmc.Field.McaFruTextInMca = EnableFruText;
  xUslWrMsr (MSR_MCA_CONFIG_UMC_ODD_INST_ADDRESS, McaConfigUmc.Value);
}

/**
 * GetUmcPerDieGenoa
 *
 * @brief Get the Umc Per Die object for Genoa
 *
 * @return uint8_t  The number of unified memory controllers per die
 */
uint8_t
GetUmcPerDieGenoa (void)
{
  return (uint8_t) UMC_PER_DIE;
}

/**
 * GetUmcHarvestFuseGenoa
 *
 * @brief Get the Umc Harvest Fuse object
 *
 * @param   BusNumberBase The bus base to read the fuse from
 *
 * @return  uint32_t      The fuse register value
 */
uint32_t
GetUmcHarvestFuseGenoa (
  size_t    BusNumberBase
  )
{
  UMC_HARVEST SmuFuse;

  SmuFuse.Value = xUSLSmnRead (0, (uint32_t)BusNumberBase, SIL_RESERVED_819);
  RAS_TRACEPOINT (
    SIL_TRACE_INFO,
    "BusNumberBase = %x, SIL_RESERVED_819 = %x\n",
    BusNumberBase,
    SmuFuse.Value
    );

  return SmuFuse.Field.Mp0UmcCtrlXHarvest;
}

/**
 * UpdateFruTextToUmcGenoa
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
UpdateFruTextToUmcGenoa (
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
  RAS_TRACEPOINT (
    SIL_TRACE_INFO,
    "  Iohc Bus: 0x%x, UMC base address: 0x%08x\n",
    IohcBus,
    UmcBaseAddress
    );

  xUSLSmnWrite (
    (IohcBus / MAX_PCI_BUS_NUMBER_PER_SEGMENT),
    IohcBus,
    UmcBaseAddress,
    AmdFruTextSEntry->AmdFrutextStr.McaFruTextDW0
    );
  xUSLSmnWrite (
    (IohcBus / MAX_PCI_BUS_NUMBER_PER_SEGMENT),
    IohcBus,
    UmcBaseAddress + 0x04,
    AmdFruTextSEntry->AmdFrutextStr.McaFruTextDW1
    );
  xUSLSmnWrite (
    (IohcBus / MAX_PCI_BUS_NUMBER_PER_SEGMENT),
    IohcBus,
    UmcBaseAddress + 0x08,
    AmdFruTextSEntry->AmdFrutextStr.McaFruTextDW2
    );
  xUSLSmnWrite (
    (IohcBus / MAX_PCI_BUS_NUMBER_PER_SEGMENT),
    IohcBus,
    UmcBaseAddress + 0x0C,
    AmdFruTextSEntry->AmdFrutextStr.McaFruTextDW3
    );

  return SilPass;
}
