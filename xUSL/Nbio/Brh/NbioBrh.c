/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  NbioBrh.c
 * @brief OpenSIL NBIO initialization
 *
 */

#include <Nbio/NbioIp2Ip.h>
#include "NbioCmn2RevBrh.h"
#include "NbioBrh.h"

NBIO_COMMON_2_REV_XFER_BLOCK NbioXferBrh = {
  .NbioIoApicMmioAddress = NbioIoApicMmioAddressBrh,
  .NbioIoApicPreDefId = NbioIoApicPreDefIdBrh,
  .NbioGetPcieComplexSize = NbioGetPcieComplexSizeBrh,
  .NbioGetPcieComplexPointer = NbioGetPcieComplexPointerBrh,
  .NbioEnumerateHarvestWrappers = NbioEnumerateHarvestWrappersBrh,
  .NbioPcieFixupPlatformConfig = NbioPcieFixupPlatformConfigBrh,
  .NbioPcieFixupComplexDefaults = NbioPcieFixupComplexDefaultsBrh
};

static NBIO_IP2IP_API NbioBrhApi = {
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

/**
 * InitializeNbioBrhTp1
 *
 * @brief Initialize NBIO IP for TimePoint 1
 *
 * @return SIL_STATUS
 */
SIL_STATUS
InitializeNbioBrhTp1 (
  void
  )
{
  NBIOCLASS_DATA_BLOCK *NbioIpBlockData;
  NBIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  NbioIpBlockData = GetNbioBlockData ();
  if (NbioIpBlockData == NULL) {
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "ERROR : Could Not Get NBIO Data Block\n");
    return SilAborted;
  }

  // Create and Initialize PCIe Topology Structure
  PcieConfigurationInit ();

  NBIO_TRACEPOINT(SIL_TRACE_INFO, "Enabled Base Iommu Ioapic in OpenSIL \n");
  NbioBaseConfigurationBrh(NbioIpBlockData);

  NbioIommuInit(NbioIpBlockData);
  NbioIoApicInit(NbioIpBlockData);
  NBIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");

  return SilPass;
}

/**
 * InitializeNbioBrhTp2
 *
 * @brief Initialize NBIO IP in timepoint 2
 *
 * @return SIL_STATUS
 */
SIL_STATUS
InitializeNbioBrhTp2 (
  void
  )
{
  return SilPass;
}

/**
 * InitializeNbioBrhTp3
 *
 * @brief Initialize NBIO IP in timepoint 3
 *
 * @return SIL_STATUS
 */
SIL_STATUS
InitializeNbioBrhTp3 (
  void
  )
{
  return SilPass;
}

/**
 * InitializeApiNbioBrh
 *
 * @brief   Initialize internal and external APIs for NBIO
 *
 * @retval  SilPass             API initialized successfully
 * @retval  SilInvalidParameter Id class is invalid
 *
 */
SIL_STATUS
InitializeApiNbioBrh (void)
{
  SIL_STATUS  Status;

  // Initialize Common to Rev specific transfer table first
  Status = SilInitCommon2RevXferTable(SilId_NbioClass, (void *)&NbioXferBrh);
  if (Status != SilPass) {
    return Status;
  }

  // Initialize NBIO IP-to-IP API
  return SilInitIp2IpApi(SilId_NbioClass, (void *)&NbioBrhApi);
}
