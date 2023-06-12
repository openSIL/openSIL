/**
 * @file  MpioCmn2.c
 * @brief OpenSIL MPIO transfer block functions
 *
 */

/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT


#include <Mpio/Common/MpioCmn2Rev.h>
#include <Mpio/Common/MpioLib.h>
#include <Mpio/MpioIp2Ip.h>
#include "MpioPcieStraps.h"
#include "MpioCmn2.h"

MPIO_COMMON_2_REV_XFER_BLOCK MpioXfer = {
  .MpioCfgGlobalConfig           = MpioCfgGlobalConfig,
  .MpioCleanUpEarlyInitSP5       = MpioCleanUpEarlyInitSP5,
  .MpioControlPorts              = MpioControlPorts,
  .MpioEnablePort                = MpioEnablePort,
  .MpioTopologyWorkarounds       = MpioTopologyWorkarounds,
  .MpioCfgBeforeReconfigWrapper  = MpioCfgWrapperBeforeBifurcation,
  .MpioCfgAfterReconfigWrapper   = MpioCfgWrapperAfterBifurcation,
  .SubsystemIdSetting            = SubsystemIdSetting,
  .GetCcdInfo                    = GetCcdInfo,
  .PcieCommonCoreConfiguration   = PcieCommonCoreConfiguration,
  .PcieCommonEngineGetDeliInfo   = PcieCommonEngineGetDeliInfo,
  .PcieGetDeviceMappingSize      = PcieGetDeviceMappingSize,
  .PcieGetDeviceMapping          = PcieGetDeviceMapping,
  .PcieGetDevFn                  = PcieGetDevFn,
  .PcieSetPortPciAddressMap      = PcieSetPortPciAddressMap,
  .PcieCommonEngineConfiguration = PcieCommonEngineConfiguration,
  .InitBusRangesAndAri           = InitBusRangesAndAri,
  .MpioIsEverTriedTraining       = MpioIsEverTriedTraining
};

MPIO_IP2IP_API MpioApi = {
  .MpioServiceRequest                  = MpioServiceRequestCommon,
  .MpioWritePcieStrap                  = WritePcieStrap,
  .MpioGetPortStrapIndex               = PcieGetPortStrapIndex,
  .MpioGetDpcCapabilityStrap           = PcieGetPortDpcCapabilityStrap,
  .MpioGetTphSupportStrap              = PcieGetTphSupportStrap,
  .MpioGetVersionInfo                  = NULL
};
