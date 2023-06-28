/**
 * @file  NbioIod.c
 * @brief OpenSIL NBIO
 *
 */

/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <NBIO/NbioIp2Ip.h>
#include "NbioIod.h"
#include <NBIO/NbioPcieTopologyHelper.h>

static NBIO_IP2IP_API NbioIodApi = {
  .NbioGetHandle                            = NbioGetHandle,
  .GetGnbHandle                             = GetGnbHandle,
  .PcieConfigDebugDump                      = PcieConfigDebugDump,
  .GetHostPciAddress                        = NbioGetHostPciAddress,
  .PcieConfigGetChild                       = PcieConfigGetChild,
  .PcieConfigGetParent                      = PcieConfigGetParent,
  .PcieConfigRunProcForAllEngines           = PcieConfigRunProcForAllEngines,
  .PcieConfigRunProcForAllEnginesInWrapper  = PcieConfigRunProcForAllEnginesInWrapper,
  .PcieConfigRunProcForAllWrappers          = PcieConfigRunProcForAllWrappers,
  .PcieConfigRunProcForAllWrappersInNbio    = PcieConfigRunProcForAllWrappersInNbio,
  .PcieConfigCheckPortStatus                = PcieConfigCheckPortStatus,
  .GetVersionInfo                           = NULL
};

/**
 * InitializeApiNbioIod
 *
 * @brief   Initialize internal and external APIs for NBIO IOD
 *
 * @retval  SilPass             API initialized successfully
 * @retval  SilInvalidParameter Id class is invalid
 *
 */
SIL_STATUS
InitializeApiNbioIod (void)
{
  // Initialize NBIO IP-to-IP API
  return SilInitIp2IpApi (SilId_NbioClass, (void *)&NbioIodApi);
}
