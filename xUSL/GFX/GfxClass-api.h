/* SPDX-License-Identifier: MIT */
/*********************************************************************
 *  Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 ********************************************************************/
/**
 * @file GfxClass-api.h
 * @brief openSIL-Host GFX IP interface
 *
 * @details GFX API adds the capability
 * to perform certain fabric writes directly into cache rather than to the DRAM
 * first.
 *
 *  This file provides the structures details for the Host to configure the
 *  Gfx operations.
 */

/** @cond API_Doc
 *  @ingroup MODULES_IP
 *  @page GFX  GFX IP api
 *
 *   The GFX OpenSIL module provides an interface
 *   to configure GFX on each PCIe controller.
 *
 *
 *   See the 'Files - @ref GfxClass-api.h' section of this document for
 *   details.
 *
 *
 * @endcond
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>

#define GFXCLASS_MAJOR_REV      0
#define GFXCLASS_MINOR_REV      1
#define GFXCLASS_INSTANCE       0   ///< This is the instance number of GFXCLASS_INPUT_BLK
#define GFXCLASS_N6_INSTANCE    1   ///< This instance will hold the Gfx N6 Information Block
#define GFXCLASS_DDI_INSTANCE   2   ///< This instance will hold the Gfx DDI Information Block

///  GFX openSIL Input Block
typedef struct {
  bool AmdGfxOpenSilEnable; ///< User configurable
  bool CfgDisableAllNumAudioEndpoints;
  bool DpHBR2Disable0;
  bool DpHBR3Disable0;
  bool DpHBR2Disable1;
  bool DpHBR3Disable1;
  bool DpHBR2Disable2;
  bool DpHBR3Disable2;
  bool DpHBR2Disable3;
  bool DpHBR3Disable3;
  bool HDMI2Disable0;
  bool HDMIRetimerCaps0;
  bool HDMI2Disable1;
  bool HDMIRetimerCaps1;
  bool HDMI2Disable2;
  bool HDMIRetimerCaps2;
  bool HDMI2Disable3;
  bool HDMIRetimerCaps3;
  bool PeiGopEnable;
  bool SysInfoTconInstantOnLogoSupport;
  bool CfgSysInfoGpuCapsDdsSupport;
  bool CfgSysInfoGpuCapsBr3SdrSupport;
  bool Usb4Rt0En;
  bool Usb4Rt0DpTnlEn;
  bool Usb4Rt1En;
  bool Usb4Rt1DpTnlEn;
  uint32_t  AmdPreSilCtrl1;
  uint64_t  AmdDisplayPhyTuningSettingTableHeader;
  uint64_t  AmdDisplayPhyTuningSettingTableContent;
  uint64_t  AmdDdiContent;
  uint64_t  PeiGopConfigMemsize;
  uint64_t  PeiGopVmFbOffset;
  uint64_t  PeiGopVmFbLocationTop;
  uint32_t  BootMode;
  uint32_t  DisplayCapDdi0;
  uint32_t  DisplayCapDdi1;
  uint32_t  DisplayCapDdi2;
  uint32_t  DisplayCapDdi3;
  uint32_t  DisplayCapDdi4;
  uint32_t  AmdBitMapDisaplyOnlyController;
  uint16_t  CfgPcieRefClkSpreadSpectrum;
  uint16_t  AmdDpPhyOverride;
  uint16_t  BackLightPwmHz;
  uint8_t   CfgMaxNumAudioEndpoints;
  uint8_t   CfgIgpuControl;
  uint8_t   DisplayFixVoltageSwing;
  uint8_t   PwrOnVaryBlToBlon;
  uint8_t   PwrDownBloffToVaryBlOff;
  uint8_t   PwrOffDelay;
  uint8_t   Usb4DpiaDisable;
  uint8_t   UmaMode;
  uint32_t  AmdUmaCarveoutIndexMax;
} GFXCLASS_INPUT_BLK;
