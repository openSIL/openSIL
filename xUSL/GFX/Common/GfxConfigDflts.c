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
  .CfgDisableAllNumAudioEndpoints = false,
  .DpHBR2Disable0 = false,
  .DpHBR3Disable0 = false,
  .DpHBR2Disable1 = false,
  .DpHBR3Disable1 = false,
  .DpHBR2Disable2 = false,
  .DpHBR3Disable2 = false,
  .DpHBR2Disable3 = false,
  .DpHBR3Disable3 = false,
  .HDMI2Disable0 = false,
  .HDMIRetimerCaps0 = false,
  .HDMI2Disable1 = false,
  .HDMIRetimerCaps1 = false,
  .HDMI2Disable2 = false,
  .HDMIRetimerCaps2 = false,
  .HDMI2Disable3 = false,
  .HDMIRetimerCaps3 = false,
  .PeiGopEnable = true,
  .SysInfoTconInstantOnLogoSupport = false,
  .CfgSysInfoGpuCapsDdsSupport = false,
  .CfgSysInfoGpuCapsBr3SdrSupport = false,
  .Usb4Rt0En = true,
  .Usb4Rt0DpTnlEn = true,
  .Usb4Rt1En = true,
  .Usb4Rt1DpTnlEn = true,
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
  .CfgPcieRefClkSpreadSpectrum = 375,
  .AmdDpPhyOverride = 0,
  .BackLightPwmHz = 200,
  .CfgMaxNumAudioEndpoints = 4,
  .CfgIgpuControl = 1,
  .DisplayFixVoltageSwing = 2,
  .PwrOnVaryBlToBlon = 0,
  .PwrDownBloffToVaryBlOff = 0,
  .PwrOffDelay = 0,
  .Usb4DpiaDisable = 0,
  .UmaMode = 2,
  .AmdUmaCarveoutIndexMax = 1
};
