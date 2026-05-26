/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  SmuClassDflts.c
 * @brief SMU Class IP input defaults
 *
 * This file holds the platform default values for the SMU class module.
 */

#include <SMU/SmuClass-api.h>

const SMUCLASS_INPUT_BLK mSmuClassDflts = {
  .CfgAgmLogDramSize = 4, // TODO: Add CONFIG value
  .CtrlUnusedTileClkGating = false,
  .BalanceAlphaTempFilter = false,
  .AmdSmuDsmClkCtrl = false,
  .SmuFeatureControl = 0x7adb4fff,
  .SmuFeatureControlExt = 0x00000001,
  .SmuFeatureControl64 = 0x00000000,
  .FllBtcEnable = true,
  .AmdBoostFmax = 0,
  .SyncFloodToApml = true,
  .CfgHSMPSupport = 0x1,
  .MocPBOLimitScalar = 0,
  .AmdNbioLclkDpmLevel = 0xffffffff,
  .DfCstateEnable = 0xf,
  .CfgCPPCMode = 0xf,
  .AmdGmiFolding = 0xf,
  .AmdPowerSupplyIdleControl = 0xf,
  .CfgDiagnosticMode = 0xff,
  .SocDcBtcEnable = true,
  // PP table settings
  .AmdcTDP = 0,
  .CfgPPT = 0,
  .CfgTDC = 0,
  .CfgPlatformTDP = 0,
  .CfgPlatformPPT = 0,
  .CfgPlatformTDC = 0,
  .CfgPlatformEDC = 0,
  .AmdDeterminismMode = 0,
  .AmdDeterminismControl = false,
  .XgmiPstateControl = 0xff,
  .XgmiPstateSelection = 0x00,
  .xGMIForceLinkWidthEn = 0x00,
  .xGMIForceLinkWidth = 0x00,
  .xGMIMaxLinkWidthEn = 0x00,
  .xGMIMaxLinkWidth = 0x02,
  .xGMIMinLinkWidth = 0x00,
  .CfgApbDis = 0,
  .CfgFixedSocPstate = 0,
  .PowerProfileSelect = 0,
  .DFFODisable = 0,
  .AmdSvi3SvcSpeed = 3,
  .AmdFchI3c0SdaHold = 0x2,
  .AmdFchI3c1SdaHold = 0x2,
  .AmdFchI3c2SdaHold = 0x2,
  .AmdFchI3c3SdaHold = 0x2,
  .FchI3cPPHcnt = 0x8,
  .FchI3cSpeed = 0x2,
  .AmdMemPostPackageRepairConfigInitiator = 0x00,
  .DfPstateRangeSupportEn = 0,
  .DfPstateRangeMax = 0,
  .DfPstateRangeMin = 0,
  .ThrottlerMode = 0xF,
  .CfgPerRailFreqControl = 1,
  .CfgAdjustGB = 0,
  .CfgOneCppcMax = 0
};
