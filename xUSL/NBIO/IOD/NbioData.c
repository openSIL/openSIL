/**
 * @file  NbioData.c
 * OpenSIL NBIO data definitions
 */

/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include "NbioData.h"

// Global NbioBlockData
NBIOCLASS_DATA mNbioIpBlockData;

/*
 * This is where you declare all configuration data that is private to Nbio.
 * Not shared with host.
 */
NBIO_CONFIG_DATA mNbioConfigDataDflts = {
  .IOHCClkGatingSupport                 = CONFIG_IOHC_CLK_GATING_SUPPORT,
  .CfgNbifMgcgClkGating                 = CONFIG_NTB_CLOCK_GATING_ENABLE,
  .CfgSstunlClkGating                   = CONFIG_SSTUNL_CLK_GATING,
  .CfgSyshubMgcgClkGating               = CONFIG_SYSHUB_MGCG_CLK_GATING,
  .PcieMemoryPowerDeepSleep             = true, // no usage found in code
  .PcieMemoryPowerShutDown              = true, // no usage found in code
  .TPHCompleterEnable                   = CONFIG_TPH_COMPLETER_ENABLE,
  .IoApicMMIOAddressReservedEnable      = CONFIG_IOAPIC_MMIO_ADDRESS_RESERVED_ENABLE,
  .IoApicIdPreDefineEn                  = CONFIG_IOAPIC_ID_PREDEFINE_EN,
  .IoApicIdBase                         = CONFIG_IOAPIC_ID_BASE,
  .NTBP0P0                              = false, // no usage found in code
  .NTBP0P1                              = false, // no usage found in code
  .NTBP0P2                              = false, // no usage found in code
  .NTBP0P3                              = false, // no usage found in code
  .NTBModeP0P0                          = 0, // no usage found in code
  .NTBModeP0P1                          = 0, // no usage found in code
  .NTBModeP0P2                          = 0, // no usage found in code
  .NTBModeP0P3                          = 0, // no usage found in code
  .NbifMgcgHysteresis                   = CONFIG_NBIF_MGCG_HYSTERESIS,
  .SyshubMgcgHysteresis                 = CONFIG_SYSHUB_MGCG_HYSTERESIS,
  .NTBClockGatingEnable                 = false, // no usage found in code
  .AzaliaCodecVerbTable                 = 0,     // no usage found in code
  .IohcNonPCIBarInitSmu                 = true,  // no usage found in code
  .IohcNonPCIBarInitDbg                 = CONFIG_IOHC_NONPCI_BAR_INIT_DBG,
  .IohcNonPCIBarInitFastReg             = CONFIG_IOHC_NONPCI_BAR_INIT_FAST_REG,
  .IohcNonPCIBarInitFastRegCtl          = CONFIG_IOHC_NONPCI_BAR_INIT_FAST_REGCTL,
  .OpnSpare                             = 0,   // no usage found in code
  .OpnFuseConfig                        = 0,   // no usage found in code
  .GnbIoapicAddress                     = 0,   // no usage found in code
  .IommuMMIOAddressReservedEnable       = CONFIG_IOMMU_MMIO_ADDRESS_RESERVED_ENABLE,
  .IommuSocket0Nbio0Enable              = false,   // no usage found in code
  .IommuSocket0Nbio1Enable              = false,   // no usage found in code
  .IommuSocket0Nbio2Enable              = false,   // no usage found in code
  .IommuSocket0Nbio3Enable              = false,   // no usage found in code
  .IommuSocket1Nbio0Enable              = false,   // no usage found in code
  .IommuSocket1Nbio1Enable              = false,   // no usage found in code
  .IommuSocket1Nbio2Enable              = false,   // no usage found in code
  .IommuSocket1Nbio3Enable              = false,   // no usage found in code
  .AmdApicMode                          = CONFIG_CCX_APIC_MODE,
  .IommuAvicSupport                     = CONFIG_IOMMU_AVIC_SUPPORT,
  .TWFilterDis                          = false,   // no usage found in code
  .IommuL2ClockGatingEnable             = CONFIG_IOMMU_L2_CLOCK_GATING_EN,
  .IommuL1ClockGatingEnable             = CONFIG_IOMMU_L1_CLOCK_GATING_EN,
  .IOHCPgEnable                         = CONFIG_IOHC_PG_ENABLE,
  .NbioGlobalCgOverride                 = CONFIG_NBIO_GLOBAL_CG_OVERRIDE,
  .IommuSupport                         = CONFIG_IOMMU_SUPPORT,
  .CfgACSEnable                         = CONFIG_ACS_ENABLE,
  .CfgPCIeLTREnable                     = CONFIG_PCIE_LTR_ENABLE,
  .CfgPcieAriSupport                    = CONFIG_PCIE_ARI_SUPPORT,
  .AmdMaskDpcCapability                 = CONFIG_AMD_MASK_DPC_CAPABILITY,
  .CfgAEREnable                         = CONFIG_AER_ENABLE,    // moved to MPIO
  .PcieEcrcEnablement                   = CONFIG_PCIE_ECRC_ENABLEMENT,
  .CfgAutoSpeedChangeEnable             = CONFIG_AUTO_SPEED_CHANGE_EN,
  .EsmEnableAllRootPorts                = CONFIG_ESM_EN_ALL_ROOT_PORTS,
#if CONFIG_ESM_EN_ALL_ROOT_PORTS
  .EsmTargetSpeed                       = CONFIG_ESM_TARGET_SPEED,
#endif
  .CfgRxMarginPersistenceMode           = CONFIG_RX_MARGIN_PERSISTENCE_MODE,
  .CfgSriovEnDev0F1                     = CONFIG_SRIOV_EN_DEV0F1,
  .CfgAriEnDev0F1                       = CONFIG_ARI_EN_DEV0F1,
  .CfgAerEnDev0F1                       = CONFIG_AER_EN_DEV0F1,
  .CfgAcsEnDev0F1                       = CONFIG_ACS_EN_DEV0F1,
  .CfgAtsEnDev0F1                       = CONFIG_ATS_EN_DEV0F1,
  .CfgPasidEnDev0F1                     = CONFIG_PASID_EN_DEV0F1,
  .CfgPwrEnDev0F1                       = CONFIG_PWR_EN_DEV0F1,
  .CfgRtrEnDev0F1                       = CONFIG_RTR_EN_DEV0F1,
  .CfgPriEnDev0F1                       = CONFIG_PRI_EN_DEV0F1,
  .AtcEnable                            = CONFIG_ATC_ENABLE,
  .AcsEnRccDev0                         = CONFIG_ACS_EN_RCC_DEV0,
  .AerEnRccDev0                         = CONFIG_AER_EN_RCC_DEV0,
  .AcsSourceValStrap5                   = CONFIG_ACS_SOURCE_VAL_STRAP5,
  .AcsTranslationalBlockingStrap5       = CONFIG_ACS_TRANSLATIONAL_BLOCKING_STRAP5,
  .AcsP2pReqStrap5                      = CONFIG_ACS_P2P_REQ_STRAP5,
  .AcsP2pCompStrap5                     = CONFIG_ACS_P2P_COMP_STRAP5,
  .AcsUpstreamFwdStrap5                 = CONFIG_ACS_UPSTREAM_FWD_STRAP5,
  .AcsP2PEgressStrap5                   = CONFIG_ACS_P2P_EGRESS_STRAP5,
  .AcsDirectTranslatedStrap5            = CONFIG_ACS_DIRECT_TRANSLATED_STRAP5,
  .AcsSsidEnStrap5                      = CONFIG_ACS_SSID_EN_STRAP5,
  .DlfEnStrap1                          = CONFIG_DLF_EN_STRAP1,
  .Phy16gtStrap1                        = CONFIG_PHY_16GT_STRAP1,
  .MarginEnStrap1                       = CONFIG_MARGIN_EN_STRAP1,
  .PriEnPageReq                         = CONFIG_PRI_EN_PAGE_REQ,
  .PriResetPageReq                      = CONFIG_PRI_RESET_PAGE_REQ,
  .AcsSourceVal                         = CONFIG_ACS_SOURCE_VAL,
  .AcsTranslationalBlocking             = CONFIG_ACS_TRANSLATIONAL_BLOCKING,
  .AcsP2pReq                            = CONFIG_ACS_P2P_REQ,
  .AcsP2pComp                           = CONFIG_ACS_P2P_COMP,
  .AcsUpstreamFwd                       = CONFIG_ACS_UPSTREAM_FWD,
  .AcsP2PEgress                         = CONFIG_ACS_P2P_EGRESS,
  .RccDev0E2EPrefix                     = CONFIG_TLP_PREFIX_SETTING,
  .RccDev0ExtendedFmtSupported          = CONFIG_RCC_DEV0_EXTENDED_FMT_SUPPORTED,
  .DlfCapEn                             = CONFIG_DLF_CAP_EN,
  .DlfExEn                              = CONFIG_DL_FEX_EN,
  .PrecodeRequestEnable                 = CONFIG_PRE_CODE_REQUEST_ENABLE,
  .PcieSpeedControl                     = CONFIG_PCIE_SPEED_CONTROL,
  .AdvertiseEqToHighRateSupport         = CONFIG_ADVERTISE_EQ_TO_HIGH_RATE_SUPPORT,
  .FabricSdci                           = CONFIG_FABRIC_SDCI
};


/*
 * This is where you declare all input block vars/values you want to share with the Host.
 * This becomes part of the IP API for the Host.
 */

NBIOCLASS_INPUT_BLK mNbioClassDflts = {
  .AmdPeer2PeerMode = 1,
  .CfgHdAudioEnable = true,
  .PcieAerReportMechanism = 1,
  .EdpcEnable = 0
};

/**
 * InitNbioConfigData
 * @brief Init global NbioBlockData data members
 */

void InitNbioConfigData (void)
{
  uint32_t Value32;

  Value32 = xUSLSmnRead (0, 0, 0x5D484);
  Value32 = (Value32 >> 24) & 0xFF;
  mNbioIpBlockData.NbioConfigData->OpnSpare = Value32;
}

/**
 * GetNbioBlockData
 * @brief Get NbioBlockData instance
 *
 * @retval pointer to global mNbioIpBlockData structure.
 */

NBIOCLASS_DATA *GetNbioBlockData (void)
{
    return &mNbioIpBlockData;
}
