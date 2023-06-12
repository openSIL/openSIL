/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file CcxBrh.c
 * @brief Defines Ccx BRH Zen5 core initialization Entry Point
 *
 */

#include <SilCommon.h>
#include <CommonLib/CpuLib.h>
#include <Ccx.h>
#include <CcxClass-api.h>
#include <CcxCmn2Rev.h>
#include <CcxIp2Ip.h>
#include "CcxBrh.h"

extern CCX_XFER_TABLE CcxXferZen5Brh;
extern CCX_IP2IP_API  CcxIp2IpZen5Brh;

extern const REGISTER_TABLE_AT_GIVEN_TP CcxZen5RegTableListAtGivenTP[];

/**--------------------------------------------------------------------
 * CcxClassSetInputBlkBrh
 *
 * @brief Initialize CCX BRH specific input block configs.
 *
 * @return SIL_STATUS
 */
SIL_STATUS
CcxClassSetInputBlkBrh (void)
{
  CCX_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  CcxClassSetInputBlk ();
  CCX_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**--------------------------------------------------------------------
 * InitializeCcxZen5BrhTp1
 *
 * @brief Initialize CCX Zen5 IP data at timepoint 1
 *
 * @return SIL_STATUS
 */
SIL_STATUS
InitializeCcxZen5BrhTp1 (void)
{
  CCXCLASS_DATA_BLK *LclCcxDataBlk;

  LclCcxDataBlk = (CCXCLASS_DATA_BLK *) xUslFindStructure(SilId_CcxClass,
    0
    );

  CCX_TRACEPOINT(SIL_TRACE_INFO,
    "Zen5 SIL Ccx found blk at: 0x%x \n",
    LclCcxDataBlk
    );

  return InitializeCcx(CcxZen5RegTableListAtGivenTP
    );
}

/**--------------------------------------------------------------------
 * InitializeCcxZen5BrhTp2
 *
 * @brief Initialize CCX Zen5 IP data at timepoint 2
 *
 * @return SIL_STATUS
 */
SIL_STATUS
InitializeCcxZen5BrhTp2 (void)
{
  return SilPass;
}

/**--------------------------------------------------------------------
 * InitializeCcxZen5BrhTp3
 *
 * @brief Initialize CCX Zen5 IP data at timepoint 3
 *
 * @return SIL_STATUS
 */
SIL_STATUS
InitializeCcxZen5BrhTp3 (void)
{
  return SilPass;
}

/**--------------------------------------------------------------------
 * InitializeApiZen5Brh
 *
 * @brief   Initialize internal and external CCX APIs for Zen5
 *
 * @returns SIL_STATUS
 * @retval  SilPass     APIs initialized successfully
 *
 */
SIL_STATUS
InitializeApiZen5Brh (void)
{
  SIL_STATUS  Status;

  // Initialize Common to Rev specific transfer table first
  Status = SilInitCommon2RevXferTable(SilId_CcxClass, &CcxXferZen5Brh);
  if (Status != SilPass) {
    CCX_TRACEPOINT(SIL_TRACE_INFO, "Zen5 BRH InitCommon2RevXferTable failed \n");
    return Status;
  }

  // Initialize the SIL IP-2-IP API
  Status = SilInitIp2IpApi(SilId_CcxClass, (void *)&CcxIp2IpZen5Brh);

  return Status;
}
