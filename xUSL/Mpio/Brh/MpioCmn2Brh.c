/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MpioCmn2Brh.c
 * @brief OpenSIL MPIO Breithorn transfer block functions
 *
 */

#include <Mpio/Common/MpioCmn2Rev.h>
#include "MpioCmn2Brh.h"

MPIO_COMMON_2_REV_XFER_BLOCK MpioXferBrh = {
  .MpioCfgGlobalConfig = MpioCfgGlobalConfigBrh,
  .MpioCleanUpEarlyInitSP5 = MpioCleanUpEarlyInitSP5Brh,
  .MpioControlPorts = MpioControlPortsBrh,
  .MpioEnablePort = MpioEnablePortBrh,
  .MpioTopologyWorkarounds = MpioTopologyWorkaroundsBrh,
  .MpioCfgBeforeReconfigWrapper = MpioCfgWrapperBeforeBifurcationBrh,
  .MpioCfgAfterReconfigWrapper = MpioCfgWrapperAfterBifurcationBrh,
  .PcieGetDeviceMappingSize = PcieGetDeviceMappingSizeBrh,
  .PcieGetDeviceMapping = PcieGetDeviceMappingBrh,
  .PcieGetDevFn = PcieGetDevFnBrh,
  .PcieSetPortPciAddressMap = PcieSetPortPciAddressMapBrh,
  .PcieHotplugPreInit = PcieHotplugPreInitBrh,
  .PcieHotplugInit = PcieHotplugInitBrh,
  .MpioIsEverTriedTraining = MpioIsEverTriedTrainingBrh,
  .MpioCfgAfterDxioInit = MpioCfgAfterDxioInitBrh,
  .MpioGetC2pmsgDoorbellAddress = MpioGetC2pmsgDoorbellAddressBrh
};
