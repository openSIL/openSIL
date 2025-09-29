/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  PkgTypeFixups.c
 * @brief Package Specific PCIe Configuration
 *
 */

#include <SilCommon.h>
#include <xSIM.h>
#include <CommonLib/SmnAccess.h>
#include <CommonLib/CpuLib.h>
#include <Nbio/NbioIp2Ip.h>
#include "NbioCmn2RevPhx.h"

/**
 * PcieFixupPlatformConfig
 * @brief Routine for Pcie Platform Configuration for Phx
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   Pcie                PCIe wrapper configuration info
 */
static SIL_STATUS PcieFixupPlatformConfig (
  SIL_CONTEXT           *SilContext,
  PCIe_WRAPPER_CONFIG   *Wrapper,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  NBIO_TRACEPOINT(SIL_TRACE_INFO, "Lane Mapping for PHX1 and HPT1\n");

  if (Wrapper->WrapId == 0) {
    Wrapper->StartDxioLane = 0;
    Wrapper->EndDxioLane = 19;
    Wrapper->StartPhyLane = 0;
    Wrapper->EndPhyLane = 19;
    Wrapper->Features.CoreReversed = 0;

  } else if (Wrapper->WrapId == 1) {
    Wrapper->StartDxioLane = 20;
    Wrapper->EndDxioLane = 27;
    Wrapper->StartPhyLane = 20;
    Wrapper->EndPhyLane = 27;
    Wrapper->Features.CoreReversed = 0;
  }

  return SilPass;
}

/**
 * PcieEnginePlatformConfigDefault
 * @brief Routine to program the Pcie Platform Configuration defaults
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   PcieEngine          PCIe engine configuration info
 */
static void PcieEnginePlatformConfigDefault (
  SIL_CONTEXT           *SilContext,
  PCIe_ENGINE_CONFIG    *PcieEngine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  )
{

  NBIO_CONFIG_DATA NbioConfigData = ((NBIOCLASS_DATA_BLOCK *)Buffer)->NbioConfigData;

  PcieEngine->Type.Port.EqSearchMode = 3;
  PcieEngine->Type.Port.EqSearchModeGen4 = 3;
  PcieEngine->Type.Port.PortFeatures.DynLanesPwrState = 0xFF;
  PcieEngine->Type.Port.PortFeatures.L1PowerDown = 1;
  PcieEngine->Type.Port.PortFeatures.L11PowerDown = 0xFF;
  PcieEngine->Type.Port.PortFeatures.L12PowerDown = 0xFF;
  PcieEngine->Type.Port.PortFeatures.AutoSpdChngEn = 0xFF;
  PcieEngine->Type.Port.PortFeatures.TurnOffUnusedLanes = 1;
  if (NbioConfigData.CfgPCIeLTREnable) {
    PcieEngine->Type.Port.PortFeatures.LTRControl = 1;
    PcieEngine->Type.Port.PortCapabilities.LtrSupport = 1;
  }
  if (NbioConfigData.CfgPcieTbtSupport) {
    PcieEngine->Type.Port.PortCapabilities.TenBitTagSupport |= 0x2;
  }
  if (NbioConfigData.CfgTbtCompleterEn) {
    PcieEngine->Type.Port.PortCapabilities.TenBitTagSupport |= 0x1;
    PcieEngine->Type.Port.PortFeatures.TenBitTagControl = 1;
  }
  PcieEngine->Type.Port.ClkReqFilterEn = 1;
}

/**
 * NbioPcieFixupPlatformConfigPhx
 * @brief Routine to fixup Pcie Platform Configuration for package specific values
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   Pcie                PCIe platform configuration info
 */
void
NbioPcieFixupPlatformConfigPhx (
  SIL_CONTEXT          *SilContext,
  PCIe_PLATFORM_CONFIG *Pcie
  )
{
  NBIOCLASS_DATA_BLOCK *NbioBlockData;

  NbioBlockData = GetNbioBlockData(SilContext);
  if (NbioBlockData == NULL) {
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "ERROR : Could Not Get NBIO Data Block\n");
    assert(false);
  }
  PcieConfigRunProcForAllEngines(SilContext,
    DESCRIPTOR_ALL_ENGINES,
    PcieEnginePlatformConfigDefault,
    (void *)NbioBlockData,
    Pcie
    );
  PcieConfigRunProcForAllWrappers(SilContext,
    DESCRIPTOR_ALL_WRAPPERS,
    PcieFixupPlatformConfig,
    (void *)NbioBlockData,
    Pcie
    );
}

/**
 * NbioPcieFixupComplexDefaultsPhx
 * @brief Initialize PcieComplexData defaults for all engines and wrappers
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   Pcie                Pointer to PCIe complex data
 **/
void
NbioPcieFixupComplexDefaultsPhx (
  SIL_CONTEXT          *SilContext,
  PCIe_PLATFORM_CONFIG *Pcie
  )
{
  PcieConfigRunProcForAllEngines(SilContext,
    DESCRIPTOR_ALL_ENGINES,
    PcieEngineConfigDefaults,
    NULL,
    Pcie
    );
  PcieConfigRunProcForAllWrappers(SilContext,
    DESCRIPTOR_ALL_WRAPPERS,
    PcieWrapperConfigDefaults,
    NULL,
    Pcie
    );
}
