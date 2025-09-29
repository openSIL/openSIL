/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MpioCmn2Phx.c
 * @brief OpenSIL MPIO transfer block functions
 *
 */

#include <Mpio/Common/MpioCmn2Rev.h>
#include "MpioCmn2Phx.h"

MPIO_COMMON_2_REV_XFER_BLOCK MpioXferPhx = {
  .MpioCfgGlobalConfig = MpioCfgGlobalConfigPhx,
  .MpioControlPorts = MpioControlPortsPhx,
  .MpioEnablePort = MpioEnablePortPhx,
  .MpioTopologyWorkarounds = MpioTopologyWorkaroundsPhx,
  .MpioCfgBeforeReconfigWrapper = MpioCfgWrapperBeforeBifurcationPhx,
  .MpioCfgAfterReconfigWrapper = MpioCfgWrapperAfterBifurcationPhx,
  .PcieGetDeviceMappingSize = PcieGetDeviceMappingSizePhx,
  .PcieGetDeviceMapping = PcieGetDeviceMappingPhx,
  .PcieGetDevFn = PcieGetDevFnPhx,
  .PcieSetPortPciAddressMap = PcieSetPortPciAddressMapPhx,
  .PcieHotplugPreInit = PcieHotplugPreInitPhx,
  .PcieHotplugInit = PcieHotplugInitPhx,
  .MpioIsEverTriedTraining = MpioIsEverTriedTrainingPhx,
  .MpioCfgAfterDxioInit = MpioCfgAfterDxioInitPhx,
  .MpioGetC2pmsgDoorbellAddress = MpioGetC2pmsgDoorbellAddressPhx,
  .MpioConfigurePspp = MpioConfigurePsppPhx,
  .PcieEarlyTrainFixups = PcieEarlyTrainFixupsPhx,
  .IommuInitAfterMpioInit = IommuInitAfterMpioInitPhx,
  .MpioHotplugConfigureUSB4 = MpioHotplugConfigureUSB4Phx,
  .Usb4LaneNumberCfg = Usb4LaneNumberCfgPhx,
  .SwapLogicalBridgeId = SwapLogicalBridgeIdPhx,
  .MpioLaneMappingFixups = MpioLaneMappingFixupsPhx,
  .NbifDeviceEarlyControl = NbifDeviceEarlyControlPhx,
  .MpioCfgAddStrapList = MpioCfgAddStrapListPhx,
  .MpioCfgGetStrapListSize = MpioCfgGetStrapListSizePhx,
  .MpioReleasePort = MpioReleasePortPhx,
  .MpioGetPortId = MpioGetPortIdPhx,
  .MpioRemoveCxlLinks = MpioRemoveCxlLinksPhx
};
