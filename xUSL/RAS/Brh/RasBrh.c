/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  RasBrh.c
 * @brief openSIL RAS IP Brh specific initialization.
 *
 */

#include <xSIM-api.h>
#include <SilCommon.h>
#include <RAS/Common/Ras.h>
#include <RAS/Common/RasCmn2Rev.h>
#include <RAS/RasIp2Ip.h>
#include "RasBrh.h"
#include "RasInitBrh.h"

extern RAS_XFER_TABLE mRasXferBrh;

/**
 * RasSetDataBlkBrh
 *
 * @brief Establish RAS data block for BRH
 *
 * This is an IP private function, not visible to the Host
 *
 * @retval SIL_STATUS
 */
SIL_STATUS
RasSetDataBlkBrh (void)
{
  SIL_STATUS          Status;
  RAS_DATA_BLOCK_BRH  *RasDataBlock;

  RasDataBlock = (RAS_DATA_BLOCK_BRH *) SilCreateInfoBlock(SilId_RasClass,
    sizeof (RAS_DATA_BLOCK_BRH),
    SIL_RESERVED_1756,
    RASCLASS_MAJOR_REV,
    RASCLASS_MINOR_REV
    );
  if (RasDataBlock == NULL) {
    Status = SilAborted;
    RAS_TRACEPOINT(SIL_TRACE_INFO, "Failed to create RasDataBlock\n");
  } else {
    Status = SilPass;
    RAS_TRACEPOINT(SIL_TRACE_INFO, "SIL RasDataBlock at: 0x%x \n", RasDataBlock);
  }

  return Status;
}

/**
 * InitializeApiRasBrh
 *
 * @brief   Initialize internal and external APIs for RAS Brh
 *
 * @retval  SilPass             API initialized successfully
 * @retval  SilInvalidParameter Id class is invalid
 *
 */
SIL_STATUS
InitializeApiRasBrh (void)
{
  SIL_STATUS  Status;

  // Initialize Common to Rev specific transfer table first
  return Status = SilInitCommon2RevXferTable(SilId_RasClass, (void *)&mRasXferBrh);
}

/**
 * InitializeRasTp1Brh
 *
 * @brief   TP1 initialization for RAS
 *
 * @return  SIL_STATUS  initialization status
 *
 * @retval  SilNotFound Ras data block for BRH was not found
 * @retval  SilPass     Ras init success
 */
SIL_STATUS
InitializeRasTp1Brh (void)
{
  SIL_STATUS          Status;
  RAS_DATA_BLOCK_BRH  *RasDataBlk;

  RasDataBlk = (RAS_DATA_BLOCK_BRH *) xUslFindStructure(SilId_RasClass,
    SIL_RESERVED_1756
    );

  if (RasDataBlk == NULL) {
    RAS_TRACEPOINT(SIL_TRACE_INFO, "RAS data blk not found\n");
    Status = SilNotFound;
  } else {
    RAS_TRACEPOINT(SIL_TRACE_INFO, "RAS data blk found at: 0x%x \n", RasDataBlk);
    // Call RAS init
    Status = SetDimmPresenceBrh(RasDataBlk);
  }

  return Status;
}
