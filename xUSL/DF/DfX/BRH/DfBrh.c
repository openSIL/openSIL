/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  DfBrh.c
 * @brief Ip2Ip API and Cmn2Rev xfer table setting and entrypoints for BRH data fabric
 *
 */

#include <DF/DfIp2Ip.h>
#include <DF/Common/DfCmn2Rev.h>
#include <DF/Df.h>
#include <DF/DfX/DfX.h>
#include "DfBrh.h"
#include "DfInitBrh.h"

extern DF_COMMON_2_REV_XFER_BLOCK DfCmn2RevBrhXfer;
extern DF_IP2IP_API DfIp2IpApiBrh;

/**
 * DfSetInputBlkBrh
 *
 * @brief Establish DF input defaults for Brh
 *
 * @details This function is called by the xSIM core to populate the IP's input
 *          block with its default values. Each block established by the IP must
 *          have a unique header.ID and its header.size set to the single block
 *          only - so as to allow the FindStruct() to traverse the block chain.
 *
 *          This is an IP private function, not visible to the Host.
 *
 * @return SIL_STATUS.
 * @retval  SilPass - everything is OK
 * @retval  SilAbort - Something went wrong
 */
SIL_STATUS
DfSetInputBlkBrh (void)
{
  SIL_STATUS              Status;
  void                    *InfoBlkPtr;

  Status = DfXSetInputBlk ();
  assert(Status == SilPass);

  /*
   * Create Df domain info data block that will be used by xPRF services to build SRAT APIC entries.
   * This data block is an private data block for openSIL and is not exposed to Host-FW.
   */
  InfoBlkPtr = SilCreateInfoBlock(SilId_DfClass,
    sizeof (DF_DOMAIN_INFO_BLK),
    DF_DOMAIN_INSTANCE,
    DFCLASS_MAJOR_REV,
    DFCLASS_MINOR_REV
    );

  if (InfoBlkPtr == NULL) {
    Status = SilAborted;
  }

  return Status;
}

/**
 * DfInitApiBrh
 *
 * @brief Initialize Ip2Ip API and Cmn2Rev xfer table for BRH
 *
 */
SIL_STATUS
DfInitApiBrh (void)
{
  SIL_STATUS Status;

  // Set Cmn2Rev table for DF
  Status = SilInitCommon2RevXferTable(SilId_DfClass, &DfCmn2RevBrhXfer);
  if (Status != SilPass) {
    return Status;
  }

  // Set Ip2Ip API for DF
  return SilInitIp2IpApi(SilId_DfClass, (void *) &DfIp2IpApiBrh);
}

/**
 * InitializeDataFabricTp1Brh
 *
 * @brief   Initialize the Data Fabric IP
 *
 * See details in Df.h
 *
 * @details This function initializes the parent DF silicon block then proceeds to enable/disable the sub-ip components
 *          per config. Each sub-IP component is initialized separately.
 *
 * This is an IP private function, not visible to the Host
 *
 * @return SIL_STATUS
 * @retval  SilPass - everything is OK
 * @retval  SilAbort - Something went wrong
 */
SIL_STATUS
InitializeDataFabricTp1Brh (void)
{
  DFCLASS_INPUT_BLK            *LclInpBlk;    // Pointer to input blk
  SIL_STATUS                   Status = SilPass;
  SIL_RESERVED_STRUCT_0009       *DfTopoBlock;

  DF_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  /*
   * Create Df Topology info data block that will be used by Data Fabric Topology IP.
   * This data block is an private data block for openSIL and is not exposed to Host-FW.
   */
  DfTopoBlock = (SIL_RESERVED_STRUCT_0009 *)SilCreateInfoBlock(SilId_DfClass,
    sizeof (SIL_RESERVED_STRUCT_0009),
    SIL_RESERVED_0020,
    DFCLASS_MAJOR_REV,
    DFCLASS_MINOR_REV
    );

  /*
   * Populate Topology MAP
   */
  if (DfTopoBlock == NULL) {
    DF_TRACEPOINT(SIL_TRACE_ERROR, "DF Topology block not created!\n");
    Status = SilAborted;
  } else {
    Status = SilPrepareTopologyMap(DfTopoBlock);
    if (Status != SilPass) {
      DF_TRACEPOINT(SIL_TRACE_ERROR, "SilPrepareTopologyMap status = %d\n", Status);
    }

    LclInpBlk = (DFCLASS_INPUT_BLK *) xUslFindStructure(SilId_DfClass, 0); // Instance 0
    DF_TRACEPOINT(SIL_TRACE_INFO, "SIL DF found blk at: 0x%x \n", LclInpBlk);
    if (LclInpBlk == NULL) {
      // Could not find the IP input block
      return SilNotFound;
    }

    FabricSocInitBrh(LclInpBlk);
    Status = DfXBuildDomainInfo ();
  }

  DF_TRACEPOINT(SIL_TRACE_EXIT, "Status = [%x]\n", Status);
  return Status;
}

/**
 * InitializeDataFabricTp2Brh
 *
 * @brief   Initialize the Data Fabric IP
 *
 * See details in Df.h
 *
 * @details This function initializes the parent DF silicon block then proceeds to enable/disable the sub-ip components
 *          per config. Each sub-IP component is initialized separately.
 *
 * This is an IP private function, not visible to the Host
 *
 * @return SIL_STATUS
 * @retval  SilPass - everything is OK
 * @retval  SilAbort - Something went wrong
 */
SIL_STATUS
InitializeDataFabricTp2Brh (void)
{
  return SilPass;
}

/**
 * InitializeDataFabricTp3Brh
 *
 * @brief   Initialize the Data Fabric IP
 *
 * See details in Df.h
 *
 * @details This function initializes the parent DF silicon block then proceeds to enable/disable the sub-ip components
 *          per config. Each sub-IP component is initialized separately.
 *
 * This is an IP private function, not visible to the Host
 *
 * @return SIL_STATUS
 * @retval  SilPass - everything is OK
 * @retval  SilAbort - Something went wrong
 */
SIL_STATUS
InitializeDataFabricTp3Brh (void)
{
  return SilPass;
}
