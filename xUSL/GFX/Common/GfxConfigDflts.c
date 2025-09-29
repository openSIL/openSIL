/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2024 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 * @file GfxConfigDflts.c
 * @brief This file holds the platform default values for the GFX Input Block
 */

#include <GFX/GfxClass-api.h>

const GFXCLASS_INPUT_BLK GfxClassDflts = {
  /*
   * Input block vars/values shared with the Host.
   * This becomes part of the IP API for the Host.
   */
  .AmdGfxOpenSilEnable = 1,
  .CfgDisableAllNumAudioEndpoints = 0,
  .DpHBR2Disable0 = 0,
  .DpHBR3Disable0 = 0,
  .DpHBR2Disable1 = 0,
  .DpHBR3Disable1 = 0,
  .DpHBR2Disable2 = 0,
  .DpHBR3Disable2 = 0,
  .DpHBR2Disable3 = 0,
  .DpHBR3Disable3 = 0,
  .HDMI2Disable0 = 0,
  .HDMIRetimerCaps0 = 0,
  .HDMI2Disable1 = 0,
  .HDMIRetimerCaps1 = 0,
  .HDMI2Disable2 = 0,
  .HDMIRetimerCaps2 = 0,
  .HDMI2Disable3 = 0,
  .HDMIRetimerCaps3 = 0,
  .PeiGopEnable = 0,
  .SysInfoTconInstantOnLogoSupport = 0,
  .CfgSysInfoGpuCapsDdsSupport = 0,
  .CfgSysInfoGpuCapsBr3SdrSupport = 0,
  .Usb4Rt0En = 0,
  .Usb4Rt0DpTnlEn = 0,
  .Usb4Rt1En = 0,
  .Usb4Rt1DpTnlEn = 0,
  .AmdPreSilCtrl1 = 0,
  .AmdDisplayPhyTuningSettingTableHeader = 0,
  .AmdDisplayPhyTuningSettingTableContent = 0,
  .AmdDdiContent = 0,
  .PeiGopConfigMemsize = 0,
  .PeiGopVmFbOffset = 0,
  .PeiGopVmFbLocationTop = 0,
  .BootMode = 0,
  .DisplayCapDdi0 = 0,
  .DisplayCapDdi1 = 0,
  .DisplayCapDdi2 = 0,
  .DisplayCapDdi3 = 0,
  .DisplayCapDdi4 = 0,
  .AmdBitMapDisaplyOnlyController = 0,
  .CfgPcieRefClkSpreadSpectrum = 0,
  .AmdDpPhyOverride = 0,
  .BackLightPwmHz = 0,
  .CfgMaxNumAudioEndpoints = 0,
  .CfgIgpuControl = 0,
  .DisplayFixVoltageSwing = 0,
  .PwrOnVaryBlToBlon = 0,
  .PwrDownBloffToVaryBlOff = 0,
  .PwrOffDelay = 0,
  .Usb4DpiaDisable = 0,
};
