/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved. */
// SPDX-License-Identifier: MIT
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
/*
 * Note: This file is the Host API definitions.
 *    Include into this file ONLY those items the Host needs to access
 *    and work with your data structure(s)
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

#pragma pack(push, 1)

/**
 * NBIO_CONFIG_DATA
 *
 * This structure holds NBIO IP private configuration data. This data is not shared
 * with host. Please add configs that are not to be configured by host firmware,
 * and initialized to IP default as per spec recommendation.
 */
typedef struct {
  bool IOHCClkGatingSupport;            ///< Enable/Disable IOHC Clock Gating support
  bool CfgNbifMgcgClkGating;            ///< Enable/Disable NBIF mid-grain Clock Gating support
  bool CfgSstunlClkGating;              ///< Enable/Disable SST Clock Gating support
  bool CfgSyshubMgcgClkGating;          ///< Enable/Disable SysHub mid-grain Clock Gating support
  bool PcieMemoryPowerDeepSleep;        ///< Enable/Disable Deep Sleep Power Saving mode for the Completion static
                                        ///< memories.
  bool PcieMemoryPowerShutDown;         ///< Enable/Disable Shut Down Power Saving mode for the Completion static
                                        ///< memories.
  uint8_t TPHCompleterEnable;           ///< Enable/Disable TPH completer/requester capability
  bool IoApicMMIOAddressReservedEnable; ///< Enable Ioapic MMIO reserved from GNB driver. 0:Disable
  bool IoApicIdPreDefineEn;             ///< Enable assign IOAPIC ID
  uint8_t IoApicIdBase;                 ///< Base NBIO IOAPIC ID. ID assigned start from this value */
  bool NTBP0P0;
  bool NTBP0P1;
  bool NTBP0P2;
  bool NTBP0P3;
  uint8_t NTBModeP0P0;
  uint8_t NTBModeP0P1;
  uint8_t NTBModeP0P2;
  uint8_t NTBModeP0P3;
  uint8_t NbifMgcgHysteresis;           ///< NBIF MGCG HYSTERESIS for gating count
  uint8_t SyshubMgcgHysteresis;         ///< NBIF MGCG HYSTERESIS for gating count
  bool NTBClockGatingEnable;            ///< Enable/Disable NTB Clock Gating support
  uint32_t AzaliaCodecVerbTable;        ///< Azalia verb table address
  bool IohcNonPCIBarInitSmu;            ///< Configure non pci device bar for SMU
  bool IohcNonPCIBarInitDbg;            ///< Configure non pci device bar for DBG
  bool IohcNonPCIBarInitFastReg;        ///< Configure non pci device bar for FastReg
  bool IohcNonPCIBarInitFastRegCtl;     ///< Configure non pci device bar for FastRegCtl
  uint32_t OpnSpare;
  uint8_t OpnFuseConfig;
  uint32_t GnbIoapicAddress;            ///< Configure non pci device bar for FastRegCtl
  bool IommuMMIOAddressReservedEnable;  ///< 1: Enable Iommu MMIO reserved from GNB driver. 0:Disable
  bool IommuSocket0Nbio0Enable;         ///< 1: Enable Iommu on Socket 0, NBIO 0
  bool IommuSocket0Nbio1Enable;         ///< 1: Enable Iommu on Socket 0, NBIO 1
  bool IommuSocket0Nbio2Enable;         ///< 1: Enable Iommu on Socket 0, NBIO 2
  bool IommuSocket0Nbio3Enable;         ///< 1: Enable Iommu on Socket 0, NBIO 3
  bool IommuSocket1Nbio0Enable;         ///< 1: Enable Iommu on Socket 1, NBIO 0
  bool IommuSocket1Nbio1Enable;         ///< 1: Enable Iommu on Socket 1, NBIO 1
  bool IommuSocket1Nbio2Enable;         ///< 1: Enable Iommu on Socket 1, NBIO 2
  bool IommuSocket1Nbio3Enable;         ///< 1: Enable Iommu on Socket 1, NBIO 3
  /* Apic Mode
   * 0xFF (auto),
   * 0x00 (CompatibilityMode) - threads below 255 run in xAPIC with xAPIC ACPI structures and threads 255 & above run in
   *                            x2 mode with x2 ACPI structures
   * 0x01 (xApicMode) - force legacy xApic mode (downcores if enabled threads are >= 255)
   * 0x02 (x2ApicMode) - force x2Apic mode independent of thread count
   */
  bool AmdApicMode;
  bool IommuAvicSupport;                ///< IOMMU General AVIC modes support
  bool TWFilterDis;                     ///< 0=Enabled. 1=Disabled. Disables filtering within table-walker of memory
  bool IommuL2ClockGatingEnable;        ///< Enable/Disable IOMMU L2 clock gating support
  bool IommuL1ClockGatingEnable;        ///< Enable/Disable IOMMU L1 clock gating support
  bool IOHCPgEnable;
  uint8_t NbioGlobalCgOverride;
  bool IommuSupport;

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
  bool FabricSdci;
} NBIO_CONFIG_DATA;

/**
 *  Establish the NBIO class module's Input Block
 */
typedef struct {
  uint8_t   AmdPeer2PeerMode;             ///< P2PMode 1:Local 2:Remote
  bool      CfgHdAudioEnable;             ///< Enable/disable audio
  bool      EsmEnableAllRootPorts;        ///< If set PCIe ESM sequence is attempted on all root ports
  uint8_t   EsmTargetSpeed;               ///< Initial PCIe ESM Target Speed for all cards in the system
  uint8_t   CfgRxMarginPersistenceMode;   ///< 1: Enable, 0: Disable
  bool      CfgDxioFrequencyVetting;
  uint8_t   CfgSkipPspMessage;            ///< 1: Skip
  bool      CfgEarlyTrainTwoPcieLinks;
  bool      EarlyBmcLinkTraining;
  uint8_t   EarlyBmcLinkSocket;
  uint8_t   EarlyBmcLinkLaneNum;
  uint8_t   EarlyBmcLinkDie;
  uint8_t   PcieAerReportMechanism;       ///<  MCA reporting; (1) OS First; (2) FW First
  uint8_t   EdpcEnable;                   ///< (0) Disabled; (1) Enabled; (3) Auto
  bool      SevSnpSupport;                ///< SEV-SNP Support
  bool      IommuSupport;                 ///< Enable/Disable IOMMU
  uint8_t   PresenceDetectSelectMode;     ///< Hotplug
  uint8_t   HotPlugHandlingMode;          ///< Hotplug
  uint8_t   HotplugPortReset;             ///< Hotplug
  uint8_t   HotPlugNVMEDefaultMaxPayload; ///< Hotplug
} NBIOCLASS_INPUT_BLK;

typedef struct {
  NBIO_CONFIG_DATA    NbioConfigData;
  // This would point to the host memory for NBIO Input block
  NBIOCLASS_INPUT_BLK NbioInputBlk;
} NBIOCLASS_DATA_BLOCK;

#pragma pack(pop)
