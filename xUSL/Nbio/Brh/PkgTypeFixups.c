/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  PkgTypeFixups.c
 * @brief Package Specific PCIe Configuration
 *
 */

#include <xSIM.h>
#include <Nbio/NbioIp2Ip.h>
#include "NbioPcieComplexDataBrh.h"
#include "NbioCmn2RevBrh.h"
#include <DF/DfIp2Ip.h>
#include <Nbio/Brh/include/IohcReg.h>

static const uint8_t StartDxioLaneSP5 [] = {
  0,         // P0
  96,        // G0
  48,        // P2
  112,       // G2
  64,        // G1
  32,        // P1
  80,        // G3
  16         // P3
};

static const uint8_t CoreReversedSP5 [] = {
  0,        // P0
  1,        // G0
  1,        // P2
  0,        // G2
  1,        // G1
  0,        // P1
  0,        // G3
  1         // P3
};

static const uint8_t StartPhyLaneSP5 [] = {
  0,         // P0
  96,        // G0
  48,        // P2
  112,       // G2
  64,        // G1
  32,        // P1
  80,        // G3
  16         // P3
};


/**
 * PcieFixupPlatformConfigSP5
 * @brief Routine for Pcie Platform Configuration for SP5
 *
 * @param[in]     Wrapper      PCIe wrapper configuration info
 * @param[in]     Buffer       Nothing here, will be NULL
 * @param[in]     Pcie         PCIe platform configuration info
 */
static SIL_STATUS PcieFixupPlatformConfigSP5 (
  PCIe_WRAPPER_CONFIG   *Wrapper,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  )

{
  GNB_HANDLE *GnbHandle;
  uint8_t Index;

  NBIO_CONFIG_DATA NbioConfigData = ((NBIOCLASS_DATA_BLOCK *)Buffer)->NbioConfigData;

  GnbHandle = (GNB_HANDLE *) PcieConfigGetParent(DESCRIPTOR_SILICON, &(Wrapper->Header));
  if (Wrapper->WrapId == 0) {
    Index = GnbHandle->RBIndex;
    Wrapper->StartDxioLane = StartDxioLaneSP5[Index];
    Wrapper->EndDxioLane = Wrapper->StartDxioLane + 15;
    Wrapper->StartPhyLane = StartPhyLaneSP5[Index];
    Wrapper->EndPhyLane = Wrapper->StartPhyLane + 15;
    Wrapper->Features.CoreReversed = CoreReversedSP5[Index];
  } else if (Wrapper->WrapId == SIL_RESERVED_1733) {
    Wrapper->StartDxioLane = SIL_RESERVED_1731;
    Wrapper->EndDxioLane = SIL_RESERVED_1729;
    Wrapper->StartPhyLane = SIL_RESERVED_1731;
    Wrapper->EndPhyLane = SIL_RESERVED_1729;
  } else if (Wrapper->WrapId == SIL_RESERVED_1753) {
    if (GnbHandle->RBIndex == 1) {
      Wrapper->StartDxioLane = SIL_RESERVED_1747;
      Wrapper->EndDxioLane = SIL_RESERVED_1746;
      Wrapper->StartPhyLane = SIL_RESERVED_1747;
      Wrapper->EndPhyLane = SIL_RESERVED_1746;
    } else {
      Wrapper->StartDxioLane = SIL_RESERVED_1749;
      Wrapper->EndDxioLane = SIL_RESERVED_1748;
      Wrapper->StartPhyLane = SIL_RESERVED_1749;
      Wrapper->EndPhyLane = SIL_RESERVED_1748;
    }
  }
  // PCD based settings
  Wrapper->AcsSupport = NbioConfigData.CfgACSEnable? 1 : 0;
  Wrapper->LtrSupport = NbioConfigData.CfgPCIeLTREnable? 1 : 0;
  Wrapper->AriForwarding = NbioConfigData.CfgPcieAriSupport? 1 : 0;
  Wrapper->LaneMargining = 1;
  Wrapper->NativePCIeEnclosureManagement = 1;
  Wrapper->DownstreamPortContainment = NbioConfigData.AmdMaskDpcCapability? 1 : 0;
  Wrapper->AdvancedErrorReporting = NbioConfigData.CfgAEREnable? 1 : 0;
  Wrapper->ECRCSupport = NbioConfigData.PcieEcrcEnablement? 3 : 0;
  return SilPass;
}


/**
 * PcieEnginePlatformConfigDefault
 * @brief Routine to program the Pcie Platform Configuration defaults
 *
 * @param[in]     PcieEngine   PCIe engine configuration info
 * @param[in]     Buffer       Nothing here, will be NULL
 * @param[in]     Pcie         PCIe platform configuration info
 */
static void PcieEnginePlatformConfigDefault (
  PCIe_ENGINE_CONFIG    *PcieEngine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  uint16_t  Value16;
  NBIO_CONFIG_DATA NbioConfigData = ((NBIOCLASS_DATA_BLOCK *)Buffer)->NbioConfigData;

  PcieEngine->Type.Port.EsmControl = 0;                                                  ///< Esm does not get used in Turin
  PcieEngine->Type.Port.RxMarginPersistence = NbioConfigData.CfgRxMarginPersistenceMode; ///< RxMargin persistence mode
  PcieEngine->Type.Port.PortCapabilities.NativePCIeEnclosureManagement = 1;
  PcieEngine->Type.Port.PortFeatures.L1PowerDown = 0;
  PcieEngine->Type.Port.EqSearchMode = 3;
  PcieEngine->Type.Port.EqSearchModeGen4 = 3;
  PcieEngine->Type.Port.EqSearchModeGen5 = 3;
  PcieEngine->Type.Port.PortFeatures.LTRControl = 1;
  PcieEngine->Type.Port.PortFeatures.TenBitTagControl = 1;
  PcieEngine->Type.Port.PortCapabilities.DataLinkFeature = 0;
  PcieEngine->Type.Port.PortFeatures.AutoSpdChngEn = NbioConfigData.CfgAutoSpeedChangeEnable;
  if (NbioConfigData.PcieSpeedControl != 0xF) { // Disables EQ bypass if PCIe speed controller is enabled
    PcieEngine->Type.Port.Gen5LaneEqualizationCntl.AdvertiseEqToHiRate = 0;
  } else {
    PcieEngine->Type.Port.Gen5LaneEqualizationCntl.AdvertiseEqToHiRate = NbioConfigData.AdvertiseEqToHighRateSupport;
  }
  PcieEngine->Type.Port.Gen5LaneEqualizationCntl.PrecodeRequest =
    NbioConfigData.Gen5CfgPrecodeRequestEnable == 0xFF ? 0 : NbioConfigData.Gen5CfgPrecodeRequestEnable;
  PcieEngine->Type.Port.LaneEqualizationCntl.UsRxPresetHint = 3;
  PcieEngine->Type.Port.LaneEqualizationCntl.UsTxPreset =
    NbioConfigData.PcieGen3LaneEqUsTxPreset == 0xFF ? 4 : NbioConfigData.PcieGen3LaneEqUsTxPreset;
  PcieEngine->Type.Port.LaneEqualizationCntl.DsRxPresetHint = 3;
  PcieEngine->Type.Port.LaneEqualizationCntl.DsTxPreset =
    NbioConfigData.PcieGen3LaneEqDsTxPreset == 0xFF ? 7 : NbioConfigData.PcieGen3LaneEqDsTxPreset;

  PcieEngine->Type.Port.Gen4LaneEqualizationCntl.UsTxPreset =
    NbioConfigData.PcieGen4LaneEqUsTxPreset == 0xFF ?
    4 : NbioConfigData.PcieGen4LaneEqUsTxPreset;
  PcieEngine->Type.Port.Gen4LaneEqualizationCntl.DsTxPreset =
    NbioConfigData.PcieGen4LaneEqDsTxPreset == 0xFF ? 7 : NbioConfigData.PcieGen4LaneEqDsTxPreset;

  PcieEngine->Type.Port.Gen5LaneEqualizationCntl.UsTxPreset =
    NbioConfigData.PcieGen5LaneEqUsTxPreset == 0xFF ? 4 : NbioConfigData.PcieGen5LaneEqUsTxPreset;
  PcieEngine->Type.Port.Gen5LaneEqualizationCntl.DsTxPreset =
    NbioConfigData.PcieGen5LaneEqDsTxPreset == 0xFF ? 7 : NbioConfigData.PcieGen5LaneEqDsTxPreset;

  PcieEngine->Type.Port.LaneEqualizationCntl.LcPresetMask8Gt =
    NbioConfigData.PcieLaneEqPresetMask8GtConfig == 0xFF ? 0x370 : (NbioConfigData.PcieLaneEqPresetMask8Gt) & 0x3FF;
  PcieEngine->Type.Port.Gen4LaneEqualizationCntl.LcPresetMask16Gt =
    NbioConfigData.PcieLaneEqPresetMask16GtConfig == 0xFF ? 0x370 : (NbioConfigData.PcieLaneEqPresetMask16Gt) & 0x3FF;
  PcieEngine->Type.Port.Gen5LaneEqualizationCntl.LcPresetMask32Gt =
    NbioConfigData.PcieLaneEqPresetMask32GtConfig == 0xFF ?  0x78 : (NbioConfigData.PcieLaneEqPresetMask32Gt) & 0x3FF;
  PcieEngine->Type.Port.PortFeatures.DataLinkFeatureExchangeControl = 1;

  if (NbioConfigData.AmdHotPlugSettleTime == 0xFF) {  //Disable Hot plug port settling
    NbioConfigData.AmdHotPlugSettleTime = 0;
    NbioConfigData.AmdHotPlugPDSettle = false;
  }

  Value16 = NbioConfigData.FchAcpiDeviceInvisibeMapEx;
  if (!(Value16 & BIT_16(4))) {  // Hiding I2C 4 in ACPI
    Value16 |= BIT_16(4);
    NbioConfigData.FchAcpiDeviceInvisibeMapEx = Value16;
  }

  if (NbioConfigData.AmdAllowCompliance != 0) {  //Disable Hot plug port to enter Compliance state
    NbioConfigData.AmdAllowComplianceForHpPort = false;
  }
}
/**
 * NbioPcieFixupPlatformConfigBrh
 * @brief Routine to fixup Pcie Platform Configuration for package specific values
 *
 * @param[in]     Pcie    PCIe platform configuration info
 */
void
NbioPcieFixupPlatformConfigBrh (
  PCIe_PLATFORM_CONFIG *Pcie
  )
{
  NBIOCLASS_DATA_BLOCK          *NbioBlockData;
  DF_IP2IP_API                  *DfIp2IpApi;
  uint32_t                      TotalNumberOfDie;
  uint32_t                      TotalNumberOfRootBridges;
  GNB_HANDLE                    *GnbHandle;
  uint8_t                       RbValue;


  if (SilGetIp2IpApi(SilId_DfClass, (void **)(&DfIp2IpApi)) != SilPass) {
    assert(false);
  }

  DfIp2IpApi->DfGetProcessorInfo(0, &TotalNumberOfDie, &TotalNumberOfRootBridges);


  GnbHandle = NbioGetHandle(Pcie);
  while (GnbHandle != NULL) {
    RbValue = xUSLPciRead8(GnbHandle->Address.AddressValue | SIL_RESERVED_0705);
    GnbHandle->RBIndex = RbValue;
    if ((RbValue == 0) || (RbValue == 2) || (RbValue == 5) || (RbValue == 7)) {
      GnbHandle->Header.DescriptorFlags |= SILICON_CXL_CAPABLE;
    }
    GnbHandle = GnbGetNextHandle(GnbHandle);
  }

  NbioBlockData = GetNbioBlockData ();
  if (NbioBlockData == NULL) {
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "ERROR : Could Not Get NBIO Data Block\n");
    assert(false);
  }
  PcieConfigRunProcForAllEngines(DESCRIPTOR_ALL_ENGINES,
    PcieEnginePlatformConfigDefault,
    (void *)NbioBlockData,
    Pcie
    );
  PcieConfigRunProcForAllWrappers(DESCRIPTOR_ALL_WRAPPERS, PcieFixupPlatformConfigSP5, (void *)NbioBlockData, Pcie);
}

/**
 * NbioPcieFixupComplexDefaultsBrh
 * @brief Initialize PcieComplexData defaults for all engines and wrappers
 *
 * @param Pcie     Pointer to PCIe complex data
 **/
void
NbioPcieFixupComplexDefaultsBrh (
  PCIe_PLATFORM_CONFIG *Pcie
  )
{
  PcieConfigRunProcForAllEngines(DESCRIPTOR_ALL_ENGINES,
    PcieEngineConfigDefaults,
    NULL,
    Pcie
    );
  PcieConfigRunProcForAllWrappers(DESCRIPTOR_ALL_WRAPPERS,
    PcieWrapperConfigDefaults,
    NULL,
    Pcie
    );
}
