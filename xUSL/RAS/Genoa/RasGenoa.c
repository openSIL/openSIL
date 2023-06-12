/**
 * @file  RasGenoa.c
 * @brief openSIL RAS IP  specific initialization.
 *
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <xSIM.h>
#include <MsrReg.h>
#include <stdint.h>
#include <RAS/Common/Ras.h>
#include <RAS/Common/RasReg.h>
#include <RAS/Common/RasCmn2Rev.h>
#include <RAS/RasIp2Ip.h>
#include <DF/Common/SilBaseFabricTopologyLib.h>
#include <DF/DfIp2Ip.h>
#include "RasGenoa.h"

extern RAS_XFER_TABLE mRasXferGenoa;

/**
 * mRasGenoaApi
 *
 * @brief RAS Ip-to-Ip API declaration for Genoa
 *
 */
static RAS_IP2IP_API mRasGenoaApi = {
  .Header = {
    .IpId           = SilId_RasClass,
    .Version        = 1,
  },
  .GetUmcPerDie                 = GetUmcPerDieGenoa,
  .GetUmcHarvestFuse            = GetUmcHarvestFuseGenoa,
  .UpdateFruTextToUmc           = UpdateFruTextToUmcGenoa,
  .SetIpMcaCtlMask              = SetIpMcaCtlMask,
  .ProgramCoreMcaIpIdInstanceId = ProgramCoreMcaIpIdInstanceId,
  .ProgramCoreMcaConfigUmc      = ProgramCoreMcaConfigUmcGenoa
};

/**
 * InitializeApiRas
 *
 * @brief   Initialize internal and external APIs for RAS
 *
 * @retval  SilPass             API initialized successfully
 * @retval  SilInvalidParameter Id class is invalid
 *
 */
SIL_STATUS
InitializeApiRasGenoa (void)
{
  SIL_STATUS  Status;

  // Initialize Common to Rev specific transfer table first
  Status = SilInitCommon2RevXferTable (SilId_RasClass, (void *)&mRasXferGenoa);
  if (Status != SilPass) {
    return Status;
  }

  // Initialize RAS IP-to-IP API
  return SilInitIp2IpApi (SilId_RasClass, (void *)&mRasGenoaApi);
}

/**
 * InitializeRasGenoa
 *
 * @brief   TBD
 *
 * @return  SIL_STATUS  initialization status
 *
 * @retval  TBD
 */
SIL_STATUS
InitializeRasGenoa (void)
{
  SIL_STATUS        Status;
  RASCLASS_DATA_BLK *RasDataBlk;

  RasDataBlk = (RASCLASS_DATA_BLK *) SilFindStructure (
    SilId_RasClass,
    0
    );

  RAS_TRACEPOINT (SIL_TRACE_INFO,
    "RAS data blk found at: 0x%x \n",
    RasDataBlk
    );

  // Call RAS common code
  Status = InitializeRas (RasDataBlk);

  return Status;
}
