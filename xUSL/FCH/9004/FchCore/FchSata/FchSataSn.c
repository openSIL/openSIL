/**
 * @file  FchSataSn.c
 * @brief 9004 FCH SATA controller functions
 *
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include <Fch.h>
#include <FCH/Common/FchCore/FchSata/FchSataCmn2Rev.h>
#include <FCH/Common/FchCore/FchSata/FchSataDef.h>
#include <FCH/FchSataIp2Ip.h>
#include <FCH/9004/FchCore/FchSata/FchSataCmn2Sn.h>
#include "FchSataSn.h"

extern FCH_SATA_XFER_TABLE mFchSataXferSn;

static FCHSATA_IP2IP_API mFchSataApiSn = {
  .Header = {
    .IpId         = SilId_FchSata,
    .Version    = 1,
  },
  .FchInitResetSataProgram  = FchInitResetSataProgramSn
};

/**
 * InitializeFchSataSnTp1
 *
 * @brief Config FCH Sata controller during timepoint 1 (Pre-Pcie phase)
 *
 */
SIL_STATUS
InitializeFchSataSnTp1 (void)
{
  FCHSATA_INPUT_BLK  *LclInpSataBlk; //pointer to Sata input blk

  FCH_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  LclInpSataBlk = (FCHSATA_INPUT_BLK *) xUslFindStructure (SilId_FchSata, 0);
  FCH_TRACEPOINT (SIL_TRACE_INFO,
    "openSIL FCH Sata found blk at: 0x%x \n",
    LclInpSataBlk
    );
  if (LclInpSataBlk == NULL) {
    // Could not find the IP input block
    return SilNotFound;
  }

  FchSataPrePcieInit (LclInpSataBlk);

  FCH_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeFchSataSnTp2
 *
 * @brief Config FCH Sata controller during timepoint 2 (Post-Pcie phase)
 *
 */
SIL_STATUS
InitializeFchSataSnTp2 (void)
{
  FCH_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeFchSataSnTp3
 *
 * @brief Config FCH Sata controller during timepoint 3 (Pre-OS)
 *
 */
SIL_STATUS
InitializeFchSataSnTp3 (void)
{
  FCH_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeApiFchSataSn
 *
 * @brief   Initialize internal APIs
 *
 * @returns SIL_STATUS
 * @retval  SilPass     APIs initialized successfully
 *
 */
SIL_STATUS
InitializeApiFchSataSn (void)
{
  SIL_STATUS Status;

  // Initialize Common to Rev specific transfer table first
  Status = SilInitCommon2RevXferTable (SilId_FchSata, (void *)&mFchSataXferSn);
  if (Status != SilPass) {
    return Status;
  }

  // Initialize SMU IP-to-IP API
  return SilInitIp2IpApi (SilId_FchSata, (void *)&mFchSataApiSn);
}
