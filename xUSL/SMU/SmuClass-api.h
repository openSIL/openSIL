/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  SmuClass-api.h
 * @brief openSIL-Host SMU IP interface
 *
 * @details The SMU API provides some controls for SMU initialization.
 *
 *  This file provides the structures details for the Host to configure the
 *  SMU operations.
 */

/*
 * Note: This file is the Host API definitions.
 *    Include into this file ONLY those items the Host needs to access
 *    and work with your data structure(s)
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

#define SMUCLASS_MAJOR_REV   0
#define SMUCLASS_MINOR_REV   1
#define SMUCLASS_INSTANCE    0

/**
 * @cond FullCode_Doc
 * @ingroup SMU_group
 * @endcond
 *
 * Definition for the SMU module's Input Block
 *
 * Note: This struct contains the vars shared between the
 *    Host API and the openSIL SMU IP module.
 *    Include into this file ONLY those items the Host needs to access
 *    and work with your IP.
 *
 */
typedef struct {
  uint16_t  CfgAgmLogDramSize;
  bool      CtrlUnusedTileClkGating;
  bool      AmdSmuDsmClkCtrl;
  uint32_t  SmuFeatureControl;
  uint32_t  SmuFeatureControlExt;
  uint32_t  SmuFeatureControl64;
  bool      FllBtcEnable;
  uint32_t  AmdBoostFmax;
  bool      SyncFloodToApml;
  uint8_t   CfgHSMPSupport;
  uint32_t  MocPBOLimitScalar;
  uint32_t  AmdNbioLclkDpmLevel;
  uint8_t   DfCstateEnable;
  uint8_t   CfgCPPCMode;
  uint8_t   AmdGmiFolding;
  uint8_t   AmdPowerSupplyIdleControl;
  uint8_t   CfgDiagnosticMode;
  bool      SocDcBtcEnable;
  // PP table settings
  uint32_t  AmdcTDP;
  uint32_t  CfgPPT;
  uint32_t  CfgTDC;
  uint32_t  CfgPlatformTDP;
  uint32_t  CfgPlatformPPT;
  uint32_t  CfgPlatformTDC;
  uint32_t  CfgPlatformEDC;
  uint8_t   AmdDeterminismMode;
  bool      AmdDeterminismControl;
  uint8_t   XgmiPstateControl;
  uint8_t   XgmiPstateSelection;
  uint8_t   xGMIForceLinkWidthEn;
  uint8_t   xGMIForceLinkWidth;
  uint8_t   xGMIMaxLinkWidthEn;
  uint8_t   xGMIMaxLinkWidth;
  uint8_t   xGMIMinLinkWidth;
  uint8_t   CfgApbDis;
  uint8_t   CfgFixedSocPstate;
  uint8_t   PowerProfileSelect;
  uint8_t   DFFODisable;
  uint8_t   AmdSvi3SvcSpeed;
  uint8_t   AmdFchI3c0SdaHold;
  uint8_t   AmdFchI3c1SdaHold;
  uint8_t   AmdFchI3c2SdaHold;
  uint8_t   AmdFchI3c3SdaHold;
  uint8_t   FchI3cPPHcnt;
  uint8_t   FchI3cSpeed;
  uint8_t   AmdMemPostPackageRepairConfigInitiator;
  uint8_t   DfPstateRangeSupportEn;
  uint8_t   DfPstateRangeMax;
  uint8_t   DfPstateRangeMin;
  uint8_t   ThrottlerMode;
  uint8_t   CfgPerRailFreqControl;
} SMUCLASS_INPUT_BLK;
