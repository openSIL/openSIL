/**
 * @file  FchSataKl.c
 * @brief 9004 FCH SATA controller functions
 *
 */
/* Copyright 2025 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include <Fch.h>
#include <FCH/Common/FchCore/FchSata/FchSataCmn2Rev.h>
#include <FCH/Common/FchCore/FchSata/FchSataReg.h>
#include <FCH/Kunlun/FchCore/FchSata/FchSataCmn2Kl.h>
#include "FchSataKl.h"

extern FCH_SATA_XFER_TABLE mFchSataXferKl;

/**
 * InitializeFchSataKlTp1
 *
 * @brief Config FCH Sata controller during timepoint 1 (Pre-Pcie phase)
 *
 */
SIL_STATUS
InitializeFchSataKlTp1 (void)
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
 * InitializeFchSataKlTp2
 *
 * @brief Config FCH Sata controller during timepoint 2 (Post-Pcie phase)
 *
 */
SIL_STATUS
InitializeFchSataKlTp2 (void)
{
  FCH_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeFchSataKlTp3
 *
 * @brief Config FCH Sata controller during timepoint 3 (Pre-OS)
 *
 */
SIL_STATUS
InitializeFchSataKlTp3 (void)
{
  FCH_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeApiFchSataKl
 *
 * @brief   Initialize internal APIs
 *
 * @returns SIL_STATUS
 * @retval  SilPass     APIs initialized successfully
 *
 */
SIL_STATUS
InitializeApiFchSataKl (void)
{
  // Initialize Common to Rev specific transfer table first
  return SilInitCommon2RevXferTable (SilId_FchSata, (void *)&mFchSataXferKl);
}
