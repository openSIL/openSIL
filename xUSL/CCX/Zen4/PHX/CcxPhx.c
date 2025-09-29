/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file CcxPhx.c
 * @brief Defines Ccx PHX Zen4 core initialization Entry Point
 *
 */

#include <SilCommon.h>
#include <CommonLib/CpuLib.h>
#include <CCX/CcxClass-api.h>
#include <CCX/Common/Ccx.h>
#include <CCX/Common/CcxCmn2Rev.h>
#include <CCX/CcxIp2Ip.h>
#include "CcxPhx.h"

extern CCX_XFER_TABLE CcxXferZen4Phx;
extern CCX_IP2IP_API  CcxIp2IpZen4Phx;

extern const REGISTER_TABLE_AT_GIVEN_TP CcxZen4RegTableListAtGivenTP[];

/**
 * CcxSetInputBlkPhx
 * @brief Establish CCX config data
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @return SIL_STATUS
 */
SIL_STATUS
CcxSetInputBlkPhx (
  SIL_CONTEXT  *SilContext
  )
{
  SIL_STATUS Status;
  Status = CcxClassSetInputBlk(SilContext);

  return Status;
}

/**
 * InitializeCcxZen4PhxTp1
 *
 * @brief Initialize CCX Zen4 IP data at timepoint 1
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @return SIL_STATUS
 */
SIL_STATUS
InitializeCcxZen4PhxTp1 (
  SIL_CONTEXT  *SilContext
  )
{
  CCXCLASS_DATA_BLK *LclCcxDataBlk;

  LclCcxDataBlk = (CCXCLASS_DATA_BLK *) xUslFindStructure(SilContext,
    SilId_CcxClass,
    0
    );

  CCX_TRACEPOINT(SIL_TRACE_INFO,
    "Zen4 SIL Ccx found blk at: 0x%x \n",
    LclCcxDataBlk
    );

  return InitializeCcx(SilContext,
    CcxZen4RegTableListAtGivenTP
    );
}

/**
 * InitializeCcxZen4PhxTp2
 *
 * @brief Initialize CCX Zen4 IP data at timepoint 2
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @return SIL_STATUS
 */
SIL_STATUS
InitializeCcxZen4PhxTp2 (
  SIL_CONTEXT  *SilContext
  )
{
  CCX_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  CCX_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeCcxZen4PhxTp3
 *
 * @brief Initialize CCX Zen4 IP data at timepoint 3
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @return SIL_STATUS
 */
SIL_STATUS
InitializeCcxZen4PhxTp3 (
  SIL_CONTEXT  *SilContext
  )
{
  CCX_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  CCX_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeApiZen4Phx
 *
 * @brief   Initialize internal and external CCX APIs for Zen4
 *
 * @param   SilContext    A context structure through which host firmware defined data
 *                        can be passed to openSIL. The host firmware is responsible
 *                        for initializing the SIL_CONTEXT structure.
 * @returns SIL_STATUS
 * @retval  SilPass       APIs initialized successfully
 *
 */
SIL_STATUS
InitializeApiZen4Phx (
  SIL_CONTEXT  *SilContext
  )
{
  SIL_STATUS  Status;

  // Initialize Common to Rev specific transfer table first
  Status = SilInitCommon2RevXferTable(SilContext, SilId_CcxClass, &CcxXferZen4Phx);
  if (Status != SilPass) {
    CCX_TRACEPOINT(SIL_TRACE_INFO, "Zen4 PHX InitCommon2RevXferTable failed \n");
    return Status;
  }

  // Initialize the SIL IP-2-IP API
  Status = SilInitIp2IpApi(SilContext, SilId_CcxClass, (void *)&CcxIp2IpZen4Phx);

  return Status;
}
