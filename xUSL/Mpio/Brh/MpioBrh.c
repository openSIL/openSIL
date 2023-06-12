/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MpioBrh.c
 * @brief OpenSIL Breithorn specific initialization
 *
 */

#include <string.h>
#include <Mpio/Common/MpioCmn2Rev.h>
#include <Mpio/Common/MpioLib.h>
#include <Mpio/MpioIp2Ip.h>
#include "MpioBrh.h"

extern MPIO_COMMON_2_REV_XFER_BLOCK MpioXferBrh;
extern MPIO_IP2IP_API MpioBrhApi;

/**
 * InitializeMpioBrhTp1
 *
 * Initialize the MPIO IP during timepoint 1 (pre-Pcie phase)
 *
 * This is an IP private function, not visible to the Host
 *
 * @return SIL_STATUS
 * @retval  SilPass - everything is OK
 * @retval  SilAbort - Something went wrong
 */
SIL_STATUS
InitializeMpioBrhTp1 (void)
{
  MPIOCLASS_INPUT_BLK             *SilData;
  NORTH_BRIDGE_PCIE_SIB           *NbPcieData;
  SIL_STATUS                      Status;
  NBIO_IP2IP_API                  *NbioIp2Ip;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  /*
   * Get IP block data
   */
  SilData = (MPIOCLASS_INPUT_BLK *)xUslFindStructure(SilId_MpioClass, 0);
  assert(SilData != NULL);
  MPIO_TRACEPOINT(SIL_TRACE_INFO, "MPIO IP block is located at %x\n", SilData);
  /*
   * Get PCIe topology from platform BIOS
   */
  NbPcieData = (NORTH_BRIDGE_PCIE_SIB *)xUslFindStructure(SilId_NbioClass, NBIOPCIECLASS_INSTANCE);
  assert(NbPcieData);

  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return SilNotFound;
  }

  // Debug output to view complex
  NbioIp2Ip->PcieConfigDebugDump(&NbPcieData->PciePlatformConfig);

  Status = NbioInitializeDxio(&NbPcieData->PciePlatformConfig, SilData);

  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");

  return Status;
}

/**
 * InitializeMpioBrhTp2
 *
 * Initialize the MPIO IP during timepoint 2 (post-Pcie phase)
 *
 * This is an IP private function, not visible to the Host
 *
 * @return SIL_STATUS
 * @retval  SilPass - everything is OK
 * @retval  SilAbort - Something went wrong
 */
SIL_STATUS
InitializeMpioBrhTp2 (void)
{
  return SilPass;
}

/**
 * InitializeMpioBrhTp3
 *
 * Initialize the MPIO IP during timepoint 3 (pre-OS)
 *
 * This is an IP private function, not visible to the Host
 *
 * @return SIL_STATUS
 * @retval  SilPass - everything is OK
 * @retval  SilAbort - Something went wrong
 */
SIL_STATUS
InitializeMpioBrhTp3 (void)
{
  return SilPass;
}

/**--------------------------------------------------------------------
 * SetMpioApiBrh
 *
 * @brief   Initialize internal and external APIs for MPIO BRH
 *
 * @retval  SilPass             API initialized successfully
 * @retval  SilInvalidParameter Id class is invalid
 *
 */
SIL_STATUS
SetMpioApiBrh (void)
{
  SIL_STATUS Status;

  // Initialize MPIO Common to Rev specific transfer table first
  Status = SilInitCommon2RevXferTable(SilId_MpioClass, (void *)&MpioXferBrh);
  if (Status != SilPass) {
    return Status;
  }

  // Initialize MPIO IP-to-IP API
  return SilInitIp2IpApi(SilId_MpioClass, (void *)&MpioBrhApi);
}
