/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 * @file SdxiBrh.c
 * @brief Defines SDXI Breithorn core initialization Entry Point
 *
 */

#include <SilCommon.h>
#include <Nbio/Common/Nbio.h>
#include <Sdxi/Common/Sdxi.h>
#include <Sdxi/Common/SdxiCmn2Rev.h>
#include "SdxiBrh.h"

extern SDXI_COMMON_2_REV_XFER_BLOCK SdxiXferBrh;

/**--------------------------------------------------------------------
 * InitializeSdxiBrhTp1
 *
 * @brief This function initializes the SDXI silicon block.
 *
 * @details  This is an IP private function, not visible to the Host.
 *           The SDXI Silicon block is initialized by calling this function
 *           in the IP block list.
 * @return SIL_STATUS
 * @retval  SilPass - everything is OK
 * @retval  SilNotFound - Something went wrong
 */
SIL_STATUS InitializeSdxiBrhTp1 (void)
{
  SDXICLASS_INPUT_BLK           *SilData;
  NORTH_BRIDGE_PCIE_SIB         *NbPcieData;
  SIL_STATUS                    Status;

  Status = SilPass;
  SDXI_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  /*
   * Get IP block data
   */
  SilData = (SDXICLASS_INPUT_BLK *)xUslFindStructure(SilId_SdxiClass, 0);
  if (SilData == NULL) {
    // Could not find the IP input block
    Status = SilNotFound;
    SDXI_TRACEPOINT(SIL_TRACE_INFO, "SDXI IP block not found \n");
    return Status;
  }

  SDXI_TRACEPOINT(SIL_TRACE_INFO, "SDXI IP block is located at %x\n", SilData);
  /*
   * Get PCIe topology from platform BIOS
   */
  NbPcieData = (NORTH_BRIDGE_PCIE_SIB *)xUslFindStructure(SilId_NbioClass, NBIOPCIECLASS_INSTANCE);
  assert(NbPcieData);

  SdxiConfig(&NbPcieData->PciePlatformConfig);

  SDXI_TRACEPOINT(SIL_TRACE_EXIT, "\n");

  return Status;
}

/**--------------------------------------------------------------------
 * SetMSdxiApiBrh
 *
 * @brief Initialize internal and external APIs for SDXI BRH
 *
 * @returns SIL_STATUS
 * @retval  SilPass     APIs initialized successfully
 *
 */
SIL_STATUS
SetSdxiApiBrh (void)
{
  SIL_STATUS Status;

  // Initialize Common to Rev specific transfer table first
  Status = SilInitCommon2RevXferTable(SilId_SdxiClass, &SdxiXferBrh);

  if (Status != SilPass) {
    SDXI_TRACEPOINT(SIL_TRACE_INFO, "SDXI BRH InitCommon2RevXferTable failed \n");
    return Status;
  }

  // There are no IP-toIP APIs for SDXI

  return Status;
}
