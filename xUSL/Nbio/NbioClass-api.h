/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.  */
/**
 * @file  NbioClass-api.h
 * @brief openSIL-Host NBIO IP interface
 *
 * @details The NorthBridge Input Output (NBIO) device provides some controls
 * for handling the PCIe root bridges.
 *
 * This file provides the structures details for the Host to configure the
 * NBIO operations.
 */

/** @cond API_Doc
 *  @ingroup MODULES_IP
 *  @page IP_NBIO  NBIO IP api
 *
 *   The NorthBridge Input Output (NBIO) device, in coordination with
 *   the MPIO device handle the PCIe internal devices (root bridges,
 *   Ports, Lanes)
 *
 *   See the 'Files - @ref NbioClass-api.h' section of this document for
 *   further details.
 *
 * @endcond
 */
#pragma once

#include <xSIM.h>
/**
 * NBIO_CONFIG_DATA
 *
 * This structure holds NBIO IP private configuration data. This data is not shared
 * with host. Please add configs that are not to be configured by host firmware,
 * and initialized to IP default as per spec recommendation.
 */
typedef struct {
  bool IOHCClkGatingSupport;            ///< Enable/Disable IOHC Clock Gating support // GN/RS/BRH
  bool CfgNbifMgcgClkGating;            ///< Enable/Disable NBIF mid-grain Clock Gating support // GN/RS/BRH
  bool CfgSstunlClkGating;              ///< Enable/Disable SST Clock Gating support
  bool CfgSyshubMgcgClkGating;          ///< Enable/Disable SysHub mid-grain Clock Gating support
  uint8_t TPHCompleterEnable;           ///< Enable/Disable TPH completer/requester capability
  bool IoApicMMIOAddressReservedEnable; ///< Enable Ioapic MMIO reserved from GNB driver. 0:Disable
  bool IoApicIdPreDefineEn;             ///< Enable assign IOAPIC ID
  uint8_t IoApicIdBase;                 ///< Base NBIO IOAPIC ID. ID assigned start from this value */
  uint8_t NbifMgcgHysteresis;           ///< NBIF MGCG HYSTERESIS for gating count
  uint8_t SyshubMgcgHysteresis;         ///< NBIF MGCG HYSTERESIS for gating count
  bool IohcNonPCIBarInitSmu;            ///< Configure non pci device bar for SMU
  bool IohcNonPCIBarInitDbg;            ///< Configure non pci device bar for DBG
  bool IohcNonPCIBarInitFastReg;        ///< Configure non pci device bar for FastReg
  bool IohcNonPCIBarInitFastRegCtl;     ///< Configure non pci device bar for FastRegCtl
  bool IommuMMIOAddressReservedEnable;  ///< 1: Enable Iommu MMIO reserved from GNB driver. 0:Disable
  bool IohcNonPCIBarInitIommuVf;        ///< 1: Enable Iommu VF MMIO reserved from GNB driver. 0:Disable
  bool IohcNonPCIBarInitIommuVfCntl;    ///< 1: Enable Iommu VF CNTL MMIO reserved from GNB driver. 0:Disable
  /* Apic Mode
   * 0xFF (auto),
   * 0x00 (CompatibilityMode) - threads below 255 run in xAPIC with xAPIC ACPI structures and threads 255 & above run in
   *                            x2 mode with x2 ACPI structures
   * 0x01 (xApicMode) - force legacy xApic mode (downcores if enabled threads are >= 255)
   * 0x02 (x2ApicMode) - force x2Apic mode independent of thread count
   */
  uint8_t AmdApicMode;
  bool IommuAvicSupport;                ///< IOMMU General AVIC modes support
  bool IommuL2ClockGatingEnable;        ///< Enable/Disable IOMMU L2 clock gating support
  bool IommuL1ClockGatingEnable;        ///< Enable/Disable IOMMU L1 clock gating support
  bool IOHCPgEnable;
  uint8_t NbioGlobalCgOverride;
  bool IommuSupport;
  bool CfgSMIFiltering;

  bool CfgACSEnable;
  bool CfgPCIeLTREnable;
  bool CfgPcieAriSupport;
  bool AmdMaskDpcCapability;
  bool CfgAEREnable;
  bool PcieEcrcEnablement;
  bool CfgAutoSpeedChangeEnable;
  bool EsmEnableAllRootPorts;
  uint8_t EsmTargetSpeed;
  bool CfgRxMarginPersistenceMode;

  bool CfgSriovEnDev0F1;
  bool CfgAriEnDev0F1;
  bool CfgAerEnDev0F1;
  bool CfgAcsEnDev0F1;
  bool CfgAtsEnDev0F1;
  bool CfgPasidEnDev0F1;
  bool CfgPwrEnDev0F1;
  bool CfgRtrEnDev0F1;
  bool CfgPriEnDev0F1;
  bool AtcEnable;
  bool AcsEnRccDev0;
  bool AerEnRccDev0;
  bool AcsSourceValStrap5;
  bool AcsTranslationalBlockingStrap5;
  bool AcsP2pReqStrap5;
  bool AcsP2pCompStrap5;
  bool AcsUpstreamFwdStrap5;
  bool AcsP2PEgressStrap5;
  bool AcsDirectTranslatedStrap5;
  bool AcsSsidEnStrap5;
  bool AcsAtomicRoutingEnStrap5;
  bool DlfEnStrap1;
  bool Phy16gtStrap1;
  bool MarginEnStrap1;
  bool PriEnPageReq;
  bool PriResetPageReq;
  bool AcsSourceVal;
  bool AcsTranslationalBlocking;
  bool AcsP2pReq;
  bool AcsP2pComp;
  bool AcsUpstreamFwd;
  bool AcsP2PEgress;
  bool RccDev0E2EPrefix;
  bool RccDev0ExtendedFmtSupported;

  bool DlfCapEn;
  bool DlfExEn;
  bool PrecodeRequestEnable;
  uint8_t PcieSpeedControl;
  bool AdvertiseEqToHighRateSupport;
  bool FabricSdxi;
  bool SevSnpSupport;
  uint8_t AmdGnbNbifDdrInitEn;  // NBIF DDR initial sequence enable
  uint8_t Gen5CfgPrecodeRequestEnable;
  uint8_t PcieGen3LaneEqUsTxPreset;       ///< Gen3 Upstream Tx Preset
  uint8_t PcieGen3LaneEqDsTxPreset;       ///< Gen3 Downstream Tx Preset
  uint8_t PcieLaneEqPresetMask8GtConfig;  ///< Configuration for Gen3 Preset Mask for all ports
  uint32_t PcieLaneEqPresetMask8Gt;       ///< Gen3 Preset Mask
  uint8_t PcieGen4LaneEqUsTxPreset;       ///< Upstream Tx Preset (Gen4)
  uint8_t PcieGen4LaneEqDsTxPreset;       ///< Downstream Tx Preset (Gen4)
  uint8_t PcieLaneEqPresetMask16GtConfig; ///< Configuration for Gen4 Preset Mask for all ports
  uint32_t PcieLaneEqPresetMask16Gt;      ///< Gen4 Preset Mask
  uint8_t PcieGen5LaneEqUsTxPreset;       ///< Upstream Tx Preset (Gen5)
  uint8_t PcieGen5LaneEqDsTxPreset;       ///< Downstream Tx Preset (Gen5)
  uint8_t PcieLaneEqPresetMask32GtConfig; ///< Configuration for Gen5 Preset Mask for all ports
  uint32_t PcieLaneEqPresetMask32Gt;      ///< Gen5 Preset Mask
  bool NbifDev0F1AtomicRequestEn;         ///< NBIF Endpoint device
  uint8_t AmdHotPlugSettleTime;           ///< Hot plug port settling time in ms
  bool AmdHotPlugPDSettle;                ///< Includes presence detect state in hot plug settling time
  uint8_t AmdPresenceDetectSelectMode;    ///< Selects hot plug presence detection mode
  uint16_t FchAcpiDeviceInvisibeMapEx;    ///< Selects which  ACPI FCH devices is invisible in ACPI
  uint16_t AmdAllowCompliance;            ///< Allows PCIe RP to enter polling compliance state
  bool AmdAllowComplianceForHpPort;       ///< Enable PCIe Hot-plug port to enter polling compliance state
  bool CfgIommuL2AtsCntlEn;
  bool AtomicRoutingEnStrap5; ///< NBIF DEV0 Enable AtomicOp Routing support in Downstream Port
  bool MappingDramIntoHtHole; ///< Enables or disables mapping DRAM into HT hole
  bool CfgIommuSocket0Nbio0Enable;
  bool CfgIommuSocket0Nbio1Enable;
  bool CfgIommuSocket0Nbio2Enable;
  bool CfgIommuSocket0Nbio3Enable;
  bool CfgIommuSocket1Nbio0Enable;
  bool CfgIommuSocket1Nbio1Enable;
  bool CfgIommuSocket1Nbio2Enable;
  bool CfgIommuSocket1Nbio3Enable;
  bool CfgIommuErrReportingWA;
  bool CfgSevTioSupport;
  bool AmdCxlOnAllPorts;    ///< Toggle Cxl on all ports
} NBIO_CONFIG_DATA;

typedef struct {
  //Fixed configs and NBIO private data goes here.
  NBIO_CONFIG_DATA    NbioConfigData;
} NBIOCLASS_DATA_BLOCK;
