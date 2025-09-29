/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 * @file  MpioPhxData.h
 * @brief MPIO input block definition for Phx
 *
 */

#pragma once

#include <SilCommon.h>

/**
 * MPIOCLASS_PHX_INSTANCE
 *
 * @details This instance value represents the MPIOCLASS_PHX_INPUT_BLK in the SIL IP block data region.
 *
 *          The MPIOCLASS_PHX_INPUT_BLK is not common across SOCs since elements are specific to Phx.
 */
#define MPIOCLASS_PHX_INSTANCE     1 // Instance number of MPIOCLASS_PHX_INPUT_BLK

#define NumberOfBridges            20

///  MPIO Phx openSIL Input Block
typedef struct {
  uint8_t     PsppPolicy;    ///< PSPP configuration
  uint32_t    CfgNbifRCSsid; ///< Defines the subsystem ID assigned to the Root Controllers
  uint32_t    AmdCfgGnbIGPUSSID;
  uint32_t    AmdCfgGnbIGPUAudioSSID; ///< Set the PCI Sub-System ID value reported by the iGPU audio controller
  bool        AcpController;          ///< Enable/Disable Acp Controller behind nbif
  bool        CfgSensorHubEnable;     ///< Enable/Disable Sensor Hub
  bool        CfgHdAudioEnable;       ///< Select whether or not the NBIO High Definition (HD) audio Device is active
  uint32_t    CfgAcpSsid;             ///< Assign ACP SSID
  uint8_t     BridgeIdRemappingTable[NumberOfBridges];
  bool        CfgDxioPCIeRSTGenericReset; ///< S0i3
  uint32_t    CfgDxioPCIeGPIOResetEP1;    ///< S0i3 GPIO Reset
  uint32_t    CfgDxioPCIeGPIOResetEP2;    ///< S0i3 GPIO Reset
  uint32_t    CfgDxioPCIeGPIOResetEP3;    ///< S0i3 GPIO Reset
  bool        CfgDxioRefClkShutDown;
  bool        CfgDxioPmaPowerGating;
  bool        CfgDxioPmaClockGating;
  bool        CfgDxioStaticPowerGating;
  bool        CfgPcieLoopbackMode;
  bool        AmdLcLoopbackWaitForAllActiveLanes;
  bool        CfgDxioAllowCompPass;
  uint8_t     CfgDynamicLanesPowerState;
  uint8_t     AmdEnableKPXShallowPstate; ///< Apply ASPM L1.0 p-state change
  bool        CfgTbtCompleterEn;
  bool        CfgTbtRequesterEn;
  uint8_t     PcieOBFF;
  uint8_t     PcieCoreMarginIgnoreCSkip;
  uint8_t     CfgPCIeTPowerOnValue;
  uint8_t     AmdDlfCapEnV2; ///< Data Link Feature capability enablement on all PCIe ports Gen4 or higher
  uint8_t     AmdDlfExEnV2;  ///< Data Link Feature Exchange enablement on all PCIe ports Gen4 or higher
  bool        CfgPcieTbtSupport;
  bool        CfgACSEnable; ///< Enable/disable Access Control Services (ACS)
  bool        CfgPCIeLTREnable;
  bool        PcieEcrcEnablement; ///< Specifies the state of the NBIO ECRC feature
} MPIOCLASS_PHX_INPUT_BLK;
