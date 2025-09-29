/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  DfPhx.c
 * @brief Ip2Ip API and Cmn2Rev xfer table setting and entrypoints for PHX data fabric
 *
 */


#include <DfIp2Ip.h>
#include <DF/Common/DfCmn2Rev.h>
#include <DF/Df.h>
#include "DfPhx.h"
#include <DF/DfX/DfX.h>
#include <DF/DfX/PHX/DfAcpiDomainInfoPhx.h>

extern DF_COMMON_2_REV_XFER_BLOCK DfCmn2RevPhxXfer;
extern DF_IP2IP_API DfIp2IpApiPhx;

/**
 * DfXSetInputBlkPhx
 *
 * @brief   Establish DF input defaults for Phx
 *
 * @details This function is called by the xSIM core to populate the IP's input
 *          block with its default values. Each block established by the IP must
 *          have a unique header.ID and its header.size set to the single block
 *          only - so as to allow the FindStruct() to traverse the block chain.
 *
 * This is an IP private function, not visible to the Host.
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @return SIL_STATUS.
 * @retval  SilPass - everything is OK
 * @retval  SilAbort - Something went wrong
 */
SIL_STATUS
DfXSetInputBlkPhx (
  SIL_CONTEXT  *SilContext
  )
{
  SIL_STATUS           Status;
  SIL_BLOCK_VARIABLES  *SilVars;
  void                 *InfoBlkPtr;

  Status = DfXSetInputBlk(SilContext);
  assert(Status == SilPass);

  SilVars = (SIL_BLOCK_VARIABLES *)(uintptr_t)SilContext->SilMemBaseAddress;
  SilVars->ActiveSoC.NumCcxPerCcd = 1;

  /*
   * Create Df domain info data block that will be used by xPRF services to build SRAT APIC entries.
   * This data block is an private data block for openSIL and is not exposed to Host-FW.
   */
  InfoBlkPtr = SilCreateInfoBlock(SilContext,
    SilId_DfClass,
    sizeof (DF_PHX_DOMAIN_INFO_BLK),
    DF_PHX_DOMAIN_INSTANCE,
    DFCLASS_MAJOR_REV,
    DFCLASS_MINOR_REV
    );

  if (InfoBlkPtr == NULL) {
    Status = SilAborted;
  }

  return Status;
}

/**
 * InitializeApiDfXPhx
 *
 * @brief Initialize Ip2Ip API and Cmn2Rev xfer table for PHX
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 */
SIL_STATUS
InitializeApiDfXPhx (
  SIL_CONTEXT  *SilContext
  )
{
  SIL_STATUS Status;

  // Set Cmn2Rev table for DF
  Status = SilInitCommon2RevXferTable(SilContext, SilId_DfClass, &DfCmn2RevPhxXfer);
  if (Status != SilPass) {
    return Status;
  }

  // Set Ip2Ip API for DF
  return SilInitIp2IpApi(SilContext, SilId_DfClass, (void *) &DfIp2IpApiPhx);
}

/**
 * InitializeDataFabricTp1Phx
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
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @return SIL_STATUS
 * @retval  SilPass - everything is OK
 * @retval  SilAbort - Something went wrong
 */
SIL_STATUS
InitializeDfXPhxTp1 (
  SIL_CONTEXT  *SilContext
  )
{
  DFCLASS_INPUT_BLK            *LclInpBlk;    // Pointer to input blk
  SIL_STATUS                   Status = SilPass;

  DF_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  LclInpBlk = (DFCLASS_INPUT_BLK *) xUslFindStructure(SilContext, SilId_DfClass, 0); // Instance 0
  DF_TRACEPOINT(SIL_TRACE_INFO, "SIL DF found blk at: 0x%x \n", LclInpBlk);
  if (LclInpBlk == NULL) {
    // Could not find the IP input block
    return SilNotFound;
  }

  FabricSocInitPhx(SilContext, LclInpBlk);
  Status = DfBuildDomainInfoPhx(SilContext);

  DF_TRACEPOINT(SIL_TRACE_EXIT, "Status = [%x]\n", Status);
  return Status;
}

/**
 * InitializeDfXPhxTp2
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
InitializeDfXPhxTp2 (void)
{
  DF_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  DF_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeDfXPhxTp3
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
InitializeDfXPhxTp3 (void)
{
  DF_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  DF_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}
