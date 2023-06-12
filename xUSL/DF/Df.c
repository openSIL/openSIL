/**
 * @file  Df.c
 * This file contains the Data Fabric (DF) IPclass init code.
 */
/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT
/**
 * @ingroup xSIM_group
 * @defgroup DF_group DFgroup:Data Fabric (DF) Class IP block
 * The DF is an IP block responsible for the connections between sockets, die
 * and internal silicon components.
 *
 * The 'class' element is the top most part of the silicon block and handles
 * the general controls.
 *
 * There are 2 primary entry points:
 * - @ref DfSetInputBlk
 * - @ref InitializeDataFabric
 *
 */

#include "Df.h"
#include "Common/DfCmn2Rev.h"
#include <SilCommon.h>
#include <string.h>

#define DFCLASS_MAJOR_REV   0
#define DFCLASS_MINOR_REV   1
#define DFCLASS_INSTANCE    0

extern const DFCLASS_INPUT_BLK  mDfClassDflts;    // module defaults

/**
 * Establish DF input defaults
 *
 * This function is called by the xSIM core to populate the IP's input
 * block with its default values. Each block established by the IP must
 * have a unique header.ID and its header.size set to the single block
 * only - so as to allow the FindStruct() to traverse the block chain.
 *
 * This is an IP private function, not visible to the Host.
 *
 * @return SIL_STATUS.
 * @retval  SilPass - everything is OK
 * @retval  SilAbort - Something went wrong
 */
SIL_STATUS
DfSetInputBlk (void)
{
  DFCLASS_INPUT_BLK *DfInput;

  DfInput = (DFCLASS_INPUT_BLK *)SilCreateInfoBlock (SilId_DfClass,
                                 sizeof(DFCLASS_INPUT_BLK),
                                 DFCLASS_INSTANCE,
                                 DFCLASS_MAJOR_REV,
                                 DFCLASS_MINOR_REV);
  DF_TRACEPOINT(SIL_TRACE_INFO, "openSIL DfSetInputBlk at: 0x%x \n", DfInput);
  if (DfInput == NULL) {
    return SilAborted;
  }
  // fill DF structure with defaults
  memcpy( (void *)DfInput, &mDfClassDflts ,sizeof(DFCLASS_INPUT_BLK));
  // fill SILcommon with platform info ( #sockets #RB/Skt, #RB, CPI-ID, #cores, etc....)

  return SilPass;
}


/**
 * InitializeDataFabricTp1
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
InitializeDataFabricTp1 (void)
{
  DFCLASS_INPUT_BLK*           LclInpBlk;    // Pointer to input blk
  DF_COMMON_2_REV_XFER_BLOCK*  DfXfer;
  SIL_STATUS                   Status = SilPass;

  DF_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Status = SilGetCommon2RevXferTable (SilId_DfClass, (void**) &DfXfer);
  assert (Status == SilPass);

  DfXfer->DfBaseFabricTopologyConstructor();
  LclInpBlk = (DFCLASS_INPUT_BLK *)SilFindStructure( SilId_DfClass,  0); //Instance 0
  DF_TRACEPOINT (SIL_TRACE_INFO, "openSIL DF found blk at: 0x%x \n", LclInpBlk);
  if (LclInpBlk == NULL) {
    // Could not find the IP input block
    return SilNotFound;
  }

  FabricSocInit (LclInpBlk);
  DfXfer->DfBuildDomainInfo ();
  DF_TRACEPOINT (SIL_TRACE_EXIT, "Status = [%x]\n", Status);
  return Status;
}

/**
 * InitializeDataFabricTp2
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
InitializeDataFabricTp2 (void)
{
  DF_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  DF_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeDataFabricTp3
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
InitializeDataFabricTp3 (void)
{
  DF_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  DF_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SilPass;
}
