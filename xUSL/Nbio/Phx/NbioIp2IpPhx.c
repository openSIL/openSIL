/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file NbioIp2IpPhx.c
 * @brief PHX Nbio core specific transfer table functions
 *
 */

#include <Nbio/NbioIp2Ip.h>

NBIO_IP2IP_API NbioPhxApi = {
  .NbioGetHandle = NbioGetHandle,
  .GetGnbHandle = GetGnbHandle,
  .PcieConfigDebugDump = PcieConfigDebugDump,
  .GetHostPciAddress = NbioGetHostPciAddress,
  .PcieConfigGetChild = PcieConfigGetChild,
  .PcieConfigGetParent = PcieConfigGetParent,
  .PcieConfigRunProcForAllEngines = PcieConfigRunProcForAllEngines,
  .PcieConfigRunProcForAllEnginesInWrapper = PcieConfigRunProcForAllEnginesInWrapper,
  .PcieConfigRunProcForAllWrappers = PcieConfigRunProcForAllWrappers,
  .PcieConfigRunProcForAllWrappersInNbio = PcieConfigRunProcForAllWrappersInNbio,
  .PcieConfigCheckPortStatus = PcieConfigCheckPortStatus
};
