/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT
/**
 * @file MpioClass-api.h
 * @brief openSIL-Host MPIO IP interface
 *
 * @details The <name> (MPIO) device API provides controls to handle the
 * PCIe root bridges and ports configuration. (This is still NOT the
 * endpoint 'PCI device enumeration').
 */

/** @cond API_Doc
 *  @ingroup MODULES_IP
 *  @page MPIO  MPIO IP api
 *
 *   The <name> (MPIO)device handles the PCIe lane allocations to Ports
 *   per the platform topology information provided by the Host.The MPIO
 *   also handles the broader system features, such as 'CXL'.
 *
 *
 *   See the 'Files - @ref MpioClass-api.h' section of this document for
 *   details.
 *
 *
 * @endcond
 */
#pragma once

#include <Mpio/Common/MpioInitLib.h>

#pragma pack(push, 1)

///  MPIO openSIL Input Block
typedef struct {
  MPIO_COMPLEX_DESCRIPTOR PlatformData[MAX_SOCKETS_SUPPORTED];
  MPIO_PORT_DESCRIPTOR    PortList[MAX_PORTS_SUPPORTED];
} PCIe_PLATFORM_TOPOLOGY;
typedef struct {
  bool        CfgDxioClockGating;  ///< Configure clock gating
  bool        PcieDxioTimingControlEnable;  ///< Enable pcie timing control
  uint32_t    PCIELinkReceiverDetectionPolling;  ///< Link polling control
  uint32_t    PCIELinkResetToTrainingTime;  ///< Link reset training control
  uint32_t    PCIELinkL0Polling;  ///< Link polling control
  bool        PCIeExactMatchEnable;  ///< Toggle exact pcie device match
  uint8_t     DxioPhyValid;  ///< Phy valid control
  uint8_t     DxioPhyProgramming;  ///< Phy program control
  uint8_t     CfgSkipPspMessage;  ///< Psp message control
  uint8_t     DxioSaveRestoreModes;  ///< Save restore control
  uint16_t    AmdAllowCompliance;  ///< Compilance settings
  uint8_t     AmdHotPlugHandlingMode;  ///< Configure hot plug mode
  uint8_t     SrisEnableMode;  ///< Mode setting for Separate Reference Clock with Independent Spread
  uint8_t     SrisSkipInterval;  ///< Interval control for Separate Reference Clock with Independent Spread
  uint8_t     SrisSkpIntervalSel;  ///< Interval selector for Separate Reference Clock with Independent Spread
  uint8_t     SrisCfgType;  ///< Configuration type for Separate Reference Clock with Independent Spread
  uint8_t     SrisAutoDetectMode;  ///< Auto detect mode for Separate Reference Clock with Independent Spread
  uint8_t     SrisAutodetectFactor;  ///< Auto detect factor Separate Reference Clock with Independent Spread
  uint8_t     SrisLowerSkpOsGenSup;  ///< Skip OS gen Separate Reference Clock with Independent Spread
  uint8_t     SrisLowerSkpOsRcvSup;  ///< Skip Os Rcv Separate Reference Clock with Independent Spread
  bool        AmdCxlOnAllPorts;  ///< Toggle Cxl on all ports
  bool        CxlCorrectableErrorLogging;  ///< Toggle correctable error logging
  bool        CxlUnCorrectableErrorLogging;  ///< Toggle unrecoverable error logging
  bool        CfgAEREnable;  ///< Toggle AER mode
  bool        CfgMcCapEnable;  ///< Toggle McCap mode
  bool        CfgRcvErrEnable;  ///< Toggle Rcv error
  bool        EarlyBmcLinkTraining;
  uint8_t     EarlyBmcLinkSocket;
  uint8_t     EarlyBmcLinkLaneNum;
  uint8_t     EarlyBmcLinkDie;
  bool        SurpriseDownFeature;
  bool        LcMultAutoSpdChgOnLastRateEnable;  ///< TBD
  bool        AmdRxMarginEnabled;  ///< Toggle Margin Rx
  uint8_t     CfgPcieCVTestWA;  ///< Control CV test workarounds
  bool        CfgPcieAriSupport;  ///< Toggle Ari Support
  bool        CfgNbioCTOtoSC;  ///< Toggle Nbio to SC
  bool        CfgNbioCTOIgnoreError;  ///< Toggle Nbio Ignore CTO error
  uint32_t    CfgNbioSsid;  ///< SSID Configuration
  uint32_t    CfgIommuSsid;  ///< SSID Configuration
  uint32_t    CfgPspccpSsid;  ///< SSID Configuration
  uint32_t    CfgNtbccpSsid;  ///< SSID Configuration
  uint32_t    CfgNbifF0Ssid;  ///< SSID Configuration
  uint32_t    CfgNtbSsid;  ///< SSID Configuration
  uint16_t    AmdPcieSubsystemDeviceID;  ///< Pcie device ID
  uint16_t    AmdPcieSubsystemVendorID;  ///< PCie vendor ID
  uint8_t     GppAtomicOps;  ///< TBD
  uint8_t     GfxAtomicOps;  ///< TBD
  bool        AmdNbioReportEdbErrors;  ///< Toggle reporting of EDB errors
  uint32_t    OpnSpare;
  uint32_t    AmdPreSilCtrl0;
  bool        MPIOAncDataSupport;
  uint16_t    AfterResetDelay;
  bool        CfgEarlyLink;
  uint8_t     AmdCfgExposeUnusedPciePorts;
  uint8_t     PortDevMap[MAX_INSTANCE_ID * MAX_PORT_DEVICE_MAP_SIZE];  ///< Possibly move to NBIO IP block
  MPIO_UBM_HFC_DESCRIPTOR     HfcDescriptor;
  uint8_t     CfgForcePcieGenSpeed;  ///< Force Pcie gen speed control
  uint8_t     CfgSataPhyTuning;  ///< Control for stat phy tuning
  bool        PcieLinkComplianceModeAllPorts;  ///< Toggle linking of compliance mode for all ports
  FW_ASK      AskBuffer[MAX_SOCKETS_SUPPORTED];
  uint8_t     AncillaryData[MPIO_MAX_ANCILLARY_DATA_SIZE];
  bool        MpioCompatibleV1Lib;
  bool        AmdMCTPEnable;  ///< Enables or disables MCTP support
  bool        SbrBrokenLaneAvoidanceSup;  ///< TBD
  bool        AutoFullMarginSup;  ///< TBD
  uint32_t    AmdPciePresetMask8GtAllPort;  ///< Value for the Gen3 PCIE LC Preset Mask Control for all ports
  uint32_t    AmdPciePresetMask16GtAllPort;  ///< Value for the Gen4 PCIE LC Preset Mask Control for all ports
  uint32_t    AmdPciePresetMask32GtAllPort;  ///< Value for the Gen5 PCIE LC Preset Mask Control for all ports
  uint8_t     PcieLinkAspmAllPort;  ///< Pcie LinkAspm
  uint8_t     AmdMCTPMasterSeg;  ///< Specifies segment of the PCI address of the MCTP master
  uint16_t    AmdMCTPMasterID;  ///< Specifies the PCI address of the MCTP master
  bool SyncHeaderByPass; ///< User configurable
  bool CxlTempGen5AdvertAltPtcl; ///< User configurable
  PCIe_DPC_STATUS_DATA    DpcStatusData;  ///< DPC status
  PCIe_PLATFORM_TOPOLOGY  PcieTopologyData; ///< PCIe Platform topology
} MPIOCLASS_INPUT_BLK;

#pragma pack(pop)

// Create a MPIO internal data block that does not get exposed to the host
// Change the instance number, so that the host can still get the data block (so that it will be allocated in the memory)
// MPIOCLASS_INTERNAL_INSTANCE 1

extern const MPIOCLASS_INPUT_BLK mMpioClassDflts;
