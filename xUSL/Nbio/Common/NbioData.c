/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  NbioData.c
 * OpenSIL NBIO data definitions
 */

#include <Nbio/Common/Nbio.h>
#include <xSIM.h>
#include <string.h>

/*
 * This is where you declare all configuration data that is private to Nbio.
 * Not shared with host.
 */
const NBIO_CONFIG_DATA mNbioConfigDataDflts = {
  .IOHCClkGatingSupport = CONFIG_IOHC_CLK_GATING_SUPPORT,
  .CfgNbifMgcgClkGating = CONFIG_NTB_CLOCK_GATING_ENABLE,
  .CfgSstunlClkGating = CONFIG_SSTUNL_CLK_GATING,
  .CfgSyshubMgcgClkGating = CONFIG_SYSHUB_MGCG_CLK_GATING,
  .TPHCompleterEnable = CONFIG_TPH_COMPLETER_ENABLE,
  .IoApicMMIOAddressReservedEnable = CONFIG_IOAPIC_MMIO_ADDRESS_RESERVED_ENABLE,
  .IoApicIdPreDefineEn = CONFIG_IOAPIC_ID_PREDEFINE_EN,
  .IoApicIdBase = CONFIG_IOAPIC_ID_BASE,
  .NbifMgcgHysteresis = CONFIG_NBIF_MGCG_HYSTERESIS,
  .SyshubMgcgHysteresis = CONFIG_SYSHUB_MGCG_HYSTERESIS,
  .IohcNonPCIBarInitSmu = CONFIG_IOHC_NONPCI_BAR_INIT_SMU,
  .IohcNonPCIBarInitDbg = CONFIG_IOHC_NONPCI_BAR_INIT_DBG,
  .IohcNonPCIBarInitFastReg = CONFIG_IOHC_NONPCI_BAR_INIT_FAST_REG,
  .IohcNonPCIBarInitFastRegCtl = CONFIG_IOHC_NONPCI_BAR_INIT_FAST_REGCTL,
  .IommuMMIOAddressReservedEnable = CONFIG_IOMMU_MMIO_ADDRESS_RESERVED_ENABLE,
  .IohcNonPCIBarInitIommuVf = CONFIG_IOMMU_VF_MMIO_ADDRESS_RESERVED_ENABLE,
  .IohcNonPCIBarInitIommuVfCntl = CONFIG_IOMMU_VFCNTL_MMIO_ADDRESS_RESERVED_ENABLE,
  .AmdApicMode = CONFIG_CCX_APIC_MODE,
  .IommuAvicSupport = CONFIG_IOMMU_AVIC_SUPPORT,
  .IommuL2ClockGatingEnable = CONFIG_IOMMU_L2_CLOCK_GATING_EN,
  .IommuL1ClockGatingEnable = CONFIG_IOMMU_L1_CLOCK_GATING_EN,
  .IOHCPgEnable = CONFIG_IOHC_PG_ENABLE,
  .NbioGlobalCgOverride = CONFIG_NBIO_GLOBAL_CG_OVERRIDE,
  .IommuSupport = CONFIG_IOMMU_SUPPORT,
  .CfgACSEnable = CONFIG_ACS_ENABLE,
  .CfgPCIeLTREnable = CONFIG_PCIE_LTR_ENABLE,
  .CfgPcieAriSupport = CONFIG_PCIE_ARI_SUPPORT,
  .AmdMaskDpcCapability = CONFIG_AMD_MASK_DPC_CAPABILITY,
  .CfgAEREnable = CONFIG_AER_ENABLE,
  .PcieEcrcEnablement = CONFIG_PCIE_ECRC_ENABLEMENT,
  .CfgAutoSpeedChangeEnable = CONFIG_AUTO_SPEED_CHANGE_EN,
  .EsmEnableAllRootPorts = CONFIG_ESM_EN_ALL_ROOT_PORTS,
#if CONFIG_ESM_EN_ALL_ROOT_PORTS
  .EsmTargetSpeed = CONFIG_ESM_TARGET_SPEED,
#endif
  .CfgRxMarginPersistenceMode = CONFIG_RX_MARGIN_PERSISTENCE_MODE,
  .CfgSriovEnDev0F1 = CONFIG_SRIOV_EN_DEV0F1,
  .CfgAriEnDev0F1 = CONFIG_ARI_EN_DEV0F1,
  .CfgAerEnDev0F1 = CONFIG_AER_EN_DEV0F1,
  .CfgAcsEnDev0F1 = CONFIG_ACS_EN_DEV0F1,
  .CfgAtsEnDev0F1 = CONFIG_ATS_EN_DEV0F1,
  .CfgPasidEnDev0F1 = CONFIG_PASID_EN_DEV0F1,
  .CfgPwrEnDev0F1 = CONFIG_PWR_EN_DEV0F1,
  .CfgRtrEnDev0F1 = CONFIG_RTR_EN_DEV0F1,
  .CfgPriEnDev0F1 = CONFIG_PRI_EN_DEV0F1,
  .AtcEnable = CONFIG_ATC_ENABLE,
  .AcsEnRccDev0 = CONFIG_ACS_EN_RCC_DEV0,
  .AerEnRccDev0 = CONFIG_AER_EN_RCC_DEV0,
  .AcsSourceValStrap5 = CONFIG_ACS_SOURCE_VAL_STRAP5,
  .AcsTranslationalBlockingStrap5 = CONFIG_ACS_TRANSLATIONAL_BLOCKING_STRAP5,
  .AcsP2pReqStrap5 = CONFIG_ACS_P2P_REQ_STRAP5,
  .AcsP2pCompStrap5 = CONFIG_ACS_P2P_COMP_STRAP5,
  .AcsUpstreamFwdStrap5 = CONFIG_ACS_UPSTREAM_FWD_STRAP5,
  .AcsP2PEgressStrap5 = CONFIG_ACS_P2P_EGRESS_STRAP5,
  .AcsDirectTranslatedStrap5 = CONFIG_ACS_DIRECT_TRANSLATED_STRAP5,
  .AcsSsidEnStrap5 = CONFIG_ACS_SSID_EN_STRAP5,
  .AtomicRoutingEnStrap5 = CONFIG_ATOMIC_ROUTING_EN_STRAP5,
  .DlfEnStrap1 = CONFIG_DLF_EN_STRAP1,
  .Phy16gtStrap1 = CONFIG_PHY_16GT_STRAP1,
  .MarginEnStrap1 = CONFIG_MARGIN_EN_STRAP1,
  .PriEnPageReq = CONFIG_PRI_EN_PAGE_REQ,
  .PriResetPageReq = CONFIG_PRI_RESET_PAGE_REQ,
  .AcsSourceVal = CONFIG_ACS_SOURCE_VAL,
  .AcsTranslationalBlocking = CONFIG_ACS_TRANSLATIONAL_BLOCKING,
  .AcsP2pReq = CONFIG_ACS_P2P_REQ,
  .AcsP2pComp = CONFIG_ACS_P2P_COMP,
  .AcsUpstreamFwd = CONFIG_ACS_UPSTREAM_FWD,
  .AcsP2PEgress = CONFIG_ACS_P2P_EGRESS,
  .RccDev0E2EPrefix = CONFIG_TLP_PREFIX_SETTING,
  .RccDev0ExtendedFmtSupported = CONFIG_RCC_DEV0_EXTENDED_FMT_SUPPORTED,
  .DlfCapEn = CONFIG_DLF_CAP_EN,
  .DlfExEn = CONFIG_DL_FEX_EN,
  .PrecodeRequestEnable = CONFIG_PRE_CODE_REQUEST_ENABLE,
  .PcieSpeedControl = CONFIG_PCIE_SPEED_CONTROL,
  .AdvertiseEqToHighRateSupport = CONFIG_ADVERTISE_EQ_TO_HIGH_RATE_SUPPORT,
  .FabricSdxi = CONFIG_FABRIC_SDXI,
  .SevSnpSupport = CONFIG_SEV_SNP_SUPPORT,
  // NBIF DDR initial sequence enable
  .AmdGnbNbifDdrInitEn = 0xFF,
  .PcieGen3LaneEqUsTxPreset = 0xFF,
  .PcieGen3LaneEqDsTxPreset = 0xFF,
  .PcieLaneEqPresetMask8GtConfig = 0xFF,
  .PcieLaneEqPresetMask8Gt = 0xFF,
  .PcieGen4LaneEqUsTxPreset = 0xFF,
  .PcieGen4LaneEqDsTxPreset = 0xFF,
  .PcieLaneEqPresetMask16GtConfig = 0xFF,
  .PcieLaneEqPresetMask16Gt = 0xFF,
  .PcieGen5LaneEqUsTxPreset = 0xFF,
  .PcieGen5LaneEqDsTxPreset = 0xFF,
  .PcieLaneEqPresetMask32GtConfig = 0xFF,
  .PcieLaneEqPresetMask32Gt = 0xFF,
  .NbifDev0F1AtomicRequestEn = 0x01,
  .AmdHotPlugSettleTime = 0xFF,
  .AmdHotPlugPDSettle = 0x00,
  .AmdPresenceDetectSelectMode = 0xFF,
  .FchAcpiDeviceInvisibeMapEx = 0x00000000,
  .AmdAllowCompliance = 0x0F,
  .AmdAllowComplianceForHpPort = 0x01,
  .CfgIommuL2AtsCntlEn = 0x00,
  .MappingDramIntoHtHole = 0x01,
  .CfgIommuSocket0Nbio0Enable = 0x01,
  .CfgIommuSocket0Nbio1Enable = 0x01,
  .CfgIommuSocket0Nbio2Enable = 0x01,
  .CfgIommuSocket0Nbio3Enable = 0x01,
  .CfgIommuSocket1Nbio0Enable = 0x01,
  .CfgIommuSocket1Nbio1Enable = 0x01,
  .CfgIommuSocket1Nbio2Enable = 0x01,
  .CfgIommuSocket1Nbio3Enable = 0x01,
  .CfgIommuErrReportingWA = 0x00,
  .CfgSMIFiltering = 0x01,
  .CfgSevTioSupport = 0x00,
  .AmdCxlOnAllPorts = 0x01
};

/*----------------------------------------------------------------------------------------*/
/**
 * GetNbioBlockData
 *
 * @brief Get NbioBlockData instance
 *
 * @param None
 *
 * @retval pointer to NBIOCLASS_DATA_BLOCK.
 */
NBIOCLASS_DATA_BLOCK *
GetNbioBlockData (
  void
  )
{
  NBIOCLASS_DATA_BLOCK  *NbioIpBlockData;

  NbioIpBlockData = (NBIOCLASS_DATA_BLOCK *) xUslFindStructure(SilId_NbioClass, 0);
  if (NbioIpBlockData == NULL) {
    // Could not find the IP input block
    NBIO_TRACEPOINT(SIL_TRACE_ERROR, "SIL NBIO Data block not found.\n");
  }
  return NbioIpBlockData;
}

/*----------------------------------------------------------------------------------------*/
/**
 * NbioClassSetInputBlk
 *
 * @brief    Establish NBIO input defaults
 *
 * @details  Establish NBIO input defaults
 *
 * @return  SIL_STATUS  initialization status
 *
 * @retval  success      for successful initialization
 * @retval  SilAborted   for failure
 */
SIL_STATUS
NbioClassSetInputBlk (
  void
  )
{
  NBIOCLASS_DATA_BLOCK   *NbioInput;

  NbioInput = (NBIOCLASS_DATA_BLOCK *)SilCreateInfoBlock(SilId_NbioClass,
    sizeof (NBIOCLASS_DATA_BLOCK),
    NBIOCLASS_INSTANCE,
    NBIOCLASS_MAJOR_REV,
    NBIOCLASS_MINOR_REV
    );
  if (NbioInput == NULL) {
    NBIO_TRACEPOINT(SIL_TRACE_ERROR, "SIL Failed to create NBIO Data Block\n");
    return SilAborted;
  }
  NBIO_TRACEPOINT(SIL_TRACE_INFO, "SIL NBIO Data Block at: 0x%x \n", NbioInput);
  // Initialize the NBIO config block data.
  memcpy((void *)(&NbioInput->NbioConfigData), &mNbioConfigDataDflts, sizeof (NBIO_CONFIG_DATA));

  return SilPass;
}
