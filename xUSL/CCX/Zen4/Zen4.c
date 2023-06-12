/**
 * @file Zen4.c
 * @brief Defines Ccx Zen4 core initialization Entry Point
 *
 */

/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include <CommonLib/CpuLib.h>
#include <CcxClass-api.h>
#include <CCX/Common/Ccx.h>
#include <CcxCmn2Rev.h>
#include <CcxIp2Ip.h>
#include "Zen4.h"
#include "Zen4DownCoreInit.h"
#include <CCX/Zen4/Zen4Reg.h>

extern CCX_XFER_TABLE CcxXferZen4;

extern CCX_IP2IP_API  CcxIp2IpZen4;
/**
 * InitializeCcxZen4Tp1
 *
 * @brief Initialize CCX Zen4 IP data at timepoint 1
 *
 * @return SIL_STATUS
 */
SIL_STATUS
InitializeCcxZen4Tp1 (void)
{
  uint32_t CcdDisMask;
  uint32_t DesiredCcdCount;
  CCXCLASS_DATA_BLK *LclCcxDataBlk;

  LclCcxDataBlk = (CCXCLASS_DATA_BLK *) SilFindStructure (
    SilId_CcxClass,
    0
    );

  CCX_TRACEPOINT (SIL_TRACE_INFO,
    "Zen4 openSIL Ccx found blk at: 0x%x \n",
    LclCcxDataBlk);

  GetZen4DesiredCcdCount (
    LclCcxDataBlk->CcxInputBlock.AmdCcdMode,
    &CcdDisMask,
    &DesiredCcdCount
    );

  extern const REGISTER_TABLE_AT_GIVEN_TP CcxZen4RegTableListAtGivenTP[];
  return InitializeCcx (
    CcxZen4RegTableListAtGivenTP,
    CcdDisMask,
    DesiredCcdCount
    );
}

/**
 * InitializeCcxZen4Tp2
 *
 * @brief Initialize CCX Zen4 IP data at timepoint 2
 *
 * @return SIL_STATUS
 */
SIL_STATUS
InitializeCcxZen4Tp2 (void)
{
  CCX_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  CCX_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeCcxZen4Tp3
 *
 * @brief Initialize CCX Zen4 IP data at timepoint 3
 *
 * @return SIL_STATUS
 */
SIL_STATUS
InitializeCcxZen4Tp3 (void)
{
  CCX_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  CCX_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeApiZen4
 *
 * @brief   Initialize internal and external CCX APIs for Zen4
 *
 * @returns SIL_STATUS
 * @retval  SilPass     APIs initialized successfully
 *
 */
SIL_STATUS
InitializeApiZen4 (void)
{
  SIL_STATUS  Status;

  // Initialize Common to Rev specific transfer table first
  Status = SilInitCommon2RevXferTable (SilId_CcxClass, &CcxXferZen4);
  if (Status != SilPass) {
    CCX_TRACEPOINT (SIL_TRACE_INFO,"Zen4 InitCommon2RevXferTable failed \n");
    return Status;
  }

  // Initialize the openSIL IP-2-IP API
  Status = SilInitIp2IpApi (SilId_CcxClass, (void *)&CcxIp2IpZen4);

  return Status;
}
