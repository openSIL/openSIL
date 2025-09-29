/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  NbioPhx.c
 * @brief OpenSIL NBIO specific initialization
 *
 */

#include <Nbio/NbioIp2Ip.h>
#include "NbioPhx.h"

extern NBIO_COMMON_2_REV_XFER_BLOCK NbioXferPhx;
extern NBIO_IP2IP_API NbioPhxApi;

/**
 * NbioSetInputBlkPhx
 * @brief Establish NBIO config data
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @return SIL_STATUS
 */
SIL_STATUS
NbioSetInputBlkPhx (
  SIL_CONTEXT  *SilContext
  )
{
  SIL_STATUS Status;
  Status = NbioClassSetInputBlk(SilContext);

  return (Status);
}

/**
 * InitializeNbioRsTp1
 *
 * @brief Initialize NBIO IP for TimePoint 1
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @return SIL_STATUS
 */
SIL_STATUS
InitializeNbioPhxTp1 (
  SIL_CONTEXT  *SilContext
  )
{
  NBIOCLASS_DATA_BLOCK *NbioIpBlockData;
  NBIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  NbioIpBlockData = GetNbioBlockData(SilContext);
  if (NbioIpBlockData == NULL) {
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "ERROR : Could Not Get NBIO Data Block\n");
    return SilAborted;
  }

  // Create and Initialize PCIe Topology Structure
  PcieConfigurationInit(SilContext);

  NBIO_TRACEPOINT(SIL_TRACE_INFO, "Enabled Base Iommu Ioapic in OpenSIL \n");
  NbioBaseConfigurationPhx(SilContext, NbioIpBlockData);

  NbioIommuInit(SilContext, NbioIpBlockData);
  NbioIoApicInit(SilContext, NbioIpBlockData);
  NbioSetTopOfMemory(SilContext);

  NBIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");

  return SilPass;
}

/**
 * InitializeNbioPhxTp2
 *
 * @brief Initialize NBIO IP in timepoint 2
 *
 * @return SIL_STATUS
 */
SIL_STATUS
InitializeNbioPhxTp2 (
  void
  )
{
  NBIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  NBIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeNbioPhxTp3
 *
 * @brief Initialize NBIO IP in timepoint 3
 *
 * @return SIL_STATUS
 */
SIL_STATUS
InitializeNbioPhxTp3 (
  void
  )
{
  NBIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  NBIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeApiNbioPhx
 *
 * @brief   Initialize internal and external APIs for NBIO
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @retval  SilPass             API initialized successfully
 * @retval  SilInvalidParameter Id class is invalid
 *
 */
SIL_STATUS
InitializeApiNbioPhx (
  SIL_CONTEXT  *SilContext
  )
{
  SIL_STATUS  Status;

  // Initialize Common to Rev specific transfer table first
  Status = SilInitCommon2RevXferTable(SilContext, SilId_NbioClass, (void *)&NbioXferPhx);
  if (Status != SilPass) {
    return Status;
  }

  // Initialize NBIO IP-to-IP API
  return SilInitIp2IpApi(SilContext, SilId_NbioClass, (void *)&NbioPhxApi);
}
