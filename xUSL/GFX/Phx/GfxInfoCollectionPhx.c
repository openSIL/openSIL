/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2024 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file GfxInfoCollectionPhx.c
 *  @brief Phoenix GFX device information collection
 */

#include <SilCommon.h>
#include <DF/DfIp2Ip.h>
#include <DF/DfX/PHX/DfSilFabricInfoPhx.h>
#include <DF/DfX/PHX/DfSilFabricRegistersPhx.h>
#include <GFX/Common/Gfx.h>
#include <GFX/Common/GfxDisplayTypeSettings.h>
#include <GFX/GfxClass-api.h>
#include <string.h>
#include "GfxInitPhx.h"
#include <GFX/Common/GfxV4.h>

static bool
IsNonInterleavingRegionExist (
  SIL_CONTEXT *SilContext
  )
{
  uint32_t                     Index;
  SIL_STATUS                   Status;
  SIL_RESERVED_UNION_0009      DramAddressCtl;
  DRAM_ADDRESS_INTLV_REGISTER  DramAddressIntlv;
  DF_IP2IP_API                 *DfIp2IpApi;

  Status = SilGetIp2IpApi(SilContext, SilId_DfClass, (void **) &DfIp2IpApi);
  assert(Status == SilPass);

  for (Index = 0; Index < 2; Index++) {
    DramAddressCtl.Value = DfIp2IpApi->DfFabricRegisterAccRead(SilContext,
                             0,
                             0,
                             DRAMADDRESSCTL0_FUNC,
                             (DFXFABx208 + (Index * (DFXFABx218 - DFXFABx208))),
                             PHX_IOM0_INSTANCE_ID
                             );
    DramAddressIntlv.Value = DfIp2IpApi->DfFabricRegisterAccRead(SilContext,
                               0,
                               0,
                               DRAMADDRESSINTLV_0_FUNC,
                               (DFXFABx20C + (Index * (DFXFABx21C - DFXFABx20C))),
                               PHX_IOM0_INSTANCE_ID
                               );
    if ((DramAddressCtl.Field.field_bit_0 == 1) && (DramAddressIntlv.Field.IntLvNumChan == 0)) {
      GFX_TRACEPOINT(SIL_TRACE_INFO, "  non-interleaving region exist: TRUE\n");
      return true;
    }
  }

  GFX_TRACEPOINT(SIL_TRACE_INFO, "  non-interleaving region exist: FALSE\n");
  return false;
}

/**
 * PopulateSystemInfoTablePhx
 * 
 * @brief Populate SystemInfoTable with values from input block
 *
 * @param SilContext       openSIL context
 * @param InputBlk         Pointer to the GFX iP block input data
 * @param SystemInfoTable  Pointer to a SystemInfoTable
 */
void
PopulateSystemInfoTablePhx (
  SIL_CONTEXT                     *SilContext,
  GFXCLASS_INPUT_BLK              *InputBlk,
  ATOM_FUSION_SYSTEM_INFO_V6      *SystemInfoTable
  )
{
  uint8_t                         Index;
  uint32_t                        PackageType;

  GFX_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  memset(SystemInfoTable, 0, sizeof (ATOM_FUSION_SYSTEM_INFO_V6));

  SystemInfoTable->sIntegratedSysInfo.table_header.structuresize = sizeof (ATOM_INTEGRATED_SYSTEM_INFO_V2_2);
  SystemInfoTable->sIntegratedSysInfo.table_header.format_revision = 2;
  SystemInfoTable->sIntegratedSysInfo.table_header.content_revision = 2;
  GFX_TRACEPOINT(SIL_TRACE_INFO, "ATOM_INTEGRATED_SYSTEM_INFO_V2_2 size[%x] Ver%d.%d\n",
                              SystemInfoTable->sIntegratedSysInfo.table_header.structuresize,
                              SystemInfoTable->sIntegratedSysInfo.table_header.format_revision,
                              SystemInfoTable->sIntegratedSysInfo.table_header.content_revision
                              );

  SystemInfoTable->sIntegratedSysInfo.vbios_misc = 0;
  if (IsNonInterleavingRegionExist (SilContext)) {
    SystemInfoTable->sIntegratedSysInfo.vbios_misc |= BIT_32(7);
  }

  SystemInfoTable->sIntegratedSysInfo.UMACarveoutIndexMax = InputBlk->AmdUmaCarveoutIndexMax;
  SystemInfoTable->sIntegratedSysInfo.UMACarveoutIndexDefault = (InputBlk->UmaMode == 3) ? 2 : 1;
  SystemInfoTable->sIntegratedSysInfo.UMACarveoutID[0] = 1;
  SystemInfoTable->sIntegratedSysInfo.UMACarveoutID[1] = 2;

  if (InputBlk->UmaMode == 1) {
    SystemInfoTable->sIntegratedSysInfo.UMACarveoutIndexMax = 0;
  }

  SystemInfoTable->sIntegratedSysInfo.gpucapinfo = 0;
  SystemInfoTable->sIntegratedSysInfo.system_config = 0;

  SystemInfoTable->sIntegratedSysInfo.cpucapinfo = 0;
  SystemInfoTable->sIntegratedSysInfo.gpuclk_ss_percentage = InputBlk->CfgPcieRefClkSpreadSpectrum;
  SystemInfoTable->sIntegratedSysInfo.gpuclk_ss_type = 0;
  SystemInfoTable->sIntegratedSysInfo.dpphy_override = InputBlk->AmdDpPhyOverride;

  SystemInfoTable->sIntegratedSysInfo.htc_hyst_limit = 0;
  SystemInfoTable->sIntegratedSysInfo.htc_tmp_limit = 0;

  SystemInfoTable->sIntegratedSysInfo.extdispconninfo.sHeader.structuresize = sizeof (ATOM_EXTERNAL_DISPLAY_CONNECTION_INFO);
  SystemInfoTable->sIntegratedSysInfo.extdispconninfo.sHeader.format_revision = 1;
  SystemInfoTable->sIntegratedSysInfo.extdispconninfo.sHeader.content_revision = 1;
  SystemInfoTable->sIntegratedSysInfo.extdispconninfo.stereopinid = 0;
  SystemInfoTable->sIntegratedSysInfo.extdispconninfo.ucRemoteDisplayConfig = 0;
  SystemInfoTable->sIntegratedSysInfo.extdispconninfo.ucFixDPVoltageSwing = InputBlk->DisplayFixVoltageSwing;

  SystemInfoTable->sIntegratedSysInfo.edp1_info.edp_backlight_pwm_hz = InputBlk->BackLightPwmHz;
  SystemInfoTable->sIntegratedSysInfo.edp1_info.edp_pwr_on_vary_bl_to_blon = InputBlk->PwrOnVaryBlToBlon;
  SystemInfoTable->sIntegratedSysInfo.edp1_info.edp_pwr_down_bloff_to_vary_bloff = InputBlk->PwrDownBloffToVaryBlOff;
  SystemInfoTable->sIntegratedSysInfo.edp1_info.edp_pwr_on_off_delay = InputBlk->PwrOffDelay;

  SystemInfoTable->sIntegratedSysInfo.edp2_info.edp_backlight_pwm_hz = InputBlk->BackLightPwmHz;
  SystemInfoTable->sIntegratedSysInfo.edp2_info.edp_pwr_on_vary_bl_to_blon = InputBlk->PwrOnVaryBlToBlon;
  SystemInfoTable->sIntegratedSysInfo.edp2_info.edp_pwr_down_bloff_to_vary_bloff = InputBlk->PwrDownBloffToVaryBlOff;
  SystemInfoTable->sIntegratedSysInfo.edp2_info.edp_pwr_on_off_delay =InputBlk->PwrOffDelay;

  if (InputBlk->SysInfoTconInstantOnLogoSupport) {
    SystemInfoTable->sIntegratedSysInfo.gpucapinfo |= SYS_INFO_GPUCAPS__TCON_INSTANT_ON_LOGO;
  }
  if (InputBlk->CfgSysInfoGpuCapsDdsSupport) {
    SystemInfoTable->sIntegratedSysInfo.gpucapinfo |= SYS_INFO_GPUCAPS__DDS_SUPPORT;
  }
  if (InputBlk->CfgSysInfoGpuCapsBr3SdrSupport) {
    SystemInfoTable->sIntegratedSysInfo.gpucapinfo |= SYS_INFO_GPUCAPS__BR3_SDR_SUPPORT;
  }

  PackageType = xUSLGetPackageType();
  if ((1 << ZEN4_PKG_FP8) == PackageType) {
    SystemInfoTable->sIntegratedSysInfo.gpu_package_id = ZEN4_PKG_FP8;
  } else if ((1 << ZEN4_PKG_FP7) == PackageType) {
    SystemInfoTable->sIntegratedSysInfo.gpu_package_id = ZEN4_PKG_FP7;
  } else if ((1 << ZEN4_PKG_FP7r2) == PackageType) {
    SystemInfoTable->sIntegratedSysInfo.gpu_package_id = ZEN4_PKG_FP7r2;
  }

  GfxIntegratedEnumerateAllConnectorsPhx(SilContext,
    InputBlk,
    &SystemInfoTable->sIntegratedSysInfo.extdispconninfo.sPath[0]
    );

  for (Index = 0; Index < NUM_DDI_PORTS; Index++) {
    GFX_TRACEPOINT(SIL_TRACE_INFO, "sPath[%x] address  0x%x\n",
                   Index, &SystemInfoTable->sIntegratedSysInfo.extdispconninfo.sPath[Index]);
    GfxIntegratedDebugDumpDisplayPath(
      &SystemInfoTable->sIntegratedSysInfo.extdispconninfo.sPath[Index]
      );
  }

  GFX_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}
