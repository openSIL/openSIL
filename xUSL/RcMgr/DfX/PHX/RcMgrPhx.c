/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file RcMgrPhx.c
 * @brief Defines RcMgr PHX core initialization Entry Point
 *
 */

#include <SilCommon.h>
#include <Utils.h>
#include <DF/Df.h>
#include <DF/DfIp2Ip.h>
#include <DF/DfX/PHX/DfSilFabricRegistersPhx.h>
#include <RcMgr/Common/FabricResourceManager.h>
#include <RcMgr/RcMgrIp2Ip.h>
#include "FabricRcInitPhx.h"
#include "FabricRcManagerPhx.h"
#include <RcMgr/DfX/RcManager-api.h>
#include <ProjSocConst.h>
#include <RcMgr/Common/RcMgrCmn2Rev.h>
#include <RcMgr/DfX/FabricRcManagerDfX.h>
#include <RcMgrPhx.h>
#include <APOB/ApobIp2Ip.h>
#include <APOB/Common/ApobCmn.h>

extern RCMGR_COMMON_2_REV_XFER_BLOCK mRcMgrXferPhx;
extern RCMGR_IP2IP_API mRcMgrApiPhx;

/**
 * RcMgrSetInputBlkPhx
 *
 * @brief Input block API that assigns host memory block to the IP.
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 */
SIL_STATUS
RcMgrSetInputBlkPhx (
  SIL_CONTEXT  *SilContext
  )
{
  void *InfoBlkPtr;

  InfoBlkPtr = SilCreateInfoBlock(SilContext,
    SilId_RcManager,
    sizeof (DFX_RCMGR_INPUT_BLK),
    RCMGR_INIT_INSTANCE,
    RCMGR_INIT_MAJOR_REV,
    RCMGR_INIT_MINOR_REV
    );

  if (InfoBlkPtr == NULL) {
    return SilAborted;
  }

  return SilPass;
}

/**
 * InitializeRcMgrPhxTp1
 *
 * @brief Initialize PHX resource registers for each RootBridge.
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 */
SIL_STATUS
InitializeRcMgrPhxTp1 (
  SIL_CONTEXT  *SilContext
  )
{
  DFX_RCMGR_INPUT_BLK *SilData;
  SIL_STATUS          Status;
  SIL_STATUS          CalledStatus;
  APOB_IP2IP_API      *ApobIp2IpApi;
  APOB_SOC_DIE_INFO   SocMaxDieInfo;

  // Locate the IP block
  SilData = (DFX_RCMGR_INPUT_BLK *) xUslFindStructure(SilContext, SilId_RcManager, 0);
  if (SilData == NULL) {
    RCMGR_TRACEPOINT(SIL_TRACE_INFO, "SIL RC Init memory block not found!\n");
    CalledStatus = SilNotFound; // Could not find the IP input block
  } else {
    RCMGR_TRACEPOINT(SIL_TRACE_INFO, "SIL RC Init memory block is found blk at: 0x%x \n", SilData);

    CalledStatus = SilPass;

    Status = SilGetIp2IpApi(SilContext, SilId_ApobClass, (void **) &ApobIp2IpApi);
    if ((Status != SilPass) || (ApobIp2IpApi == NULL)) {
      assert(Status == SilPass);
      return Status;
    }
    ApobIp2IpApi->ApobGetMaxDieInfo(SilContext, &SocMaxDieInfo);
    // These asserts prevent tainted scalar coverity warnings by checking values
    // are within permissible ranges
    RCMGR_TRACEPOINT(SIL_TRACE_INFO, "SilData->SocketNumber: 0x%x \n", SilData->SocketNumber);
    RCMGR_TRACEPOINT(SIL_TRACE_INFO, "SilData->RbsPerSocket: 0x%x \n", SilData->RbsPerSocket);
    assert((SilData->SocketNumber > 0) && (SilData->SocketNumber <= SocMaxDieInfo.MaxSocSocketsSupportedValue));
    assert((SilData->RbsPerSocket > 0) && (SilData->RbsPerSocket <= PROJ_MAX_RBS_PER_SOCKET));

    RCMGR_TRACEPOINT(SIL_TRACE_INFO, "SilData->MmioRcMgr: 0x%x \n", &SilData->MmioRcMgr);
    RCMGR_TRACEPOINT(SIL_TRACE_INFO, "SilData->IoRcMgr:   0x%x \n", &SilData->IoRcMgr);


    // Initialize MMIO
    if (SilData->SetRcBasedOnNv) {
      // Got NvVariable successfully, try to init MMIO based on it
      RCMGR_TRACEPOINT(SIL_TRACE_INFO, "  Init MMIO based on NV variable\n");
      CalledStatus = SilInitMmioBasedOnNvVariable4(SilContext, SilData, NULL, true);
    }

    if ((CalledStatus != SilPass) || (!SilData->SetRcBasedOnNv)) {
      RCMGR_TRACEPOINT(SIL_TRACE_INFO,
        " Can't get NV variable or init MMIO based on NV variable failed.\n"
        );
      RCMGR_TRACEPOINT(SIL_TRACE_INFO, " Init MMIO equally.\n");
      /* coverity[tainted_data:SUPPRESS] */
      CalledStatus = SilInitMmioEqually4(SilContext, SilData);
    }

    if (CalledStatus == SilPass) {
      // Initialize IO
      if (SilData->SetRcBasedOnNv) {
        // Get NvVariable successfully, try to init IO base on it
        RCMGR_TRACEPOINT(SIL_TRACE_INFO, "  Init IO based on NV variable\n");
        CalledStatus = SilInitIoBasedOnNvVariable4(SilContext, SilData, NULL, true);
      }

      if ((!SilData->SetRcBasedOnNv) || (CalledStatus != SilPass)) {
        RCMGR_TRACEPOINT(SIL_TRACE_INFO, " Can't get NV variable or init IO based on NV variable failed.\n");
        RCMGR_TRACEPOINT(SIL_TRACE_INFO, "                      Init IO equally.\n");
        /* coverity[tainted_data:SUPPRESS] */
        CalledStatus = SilInitIoEqually4(SilContext, SilData);
      }
    }
  }
  return SilPass;
}

/**
 * InitializeRcMgrPhxTp2
 *
 * @brief Initialize PHX resource manager at Tp2.
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 */
SIL_STATUS
InitializeRcMgrPhxTp2 (
  SIL_CONTEXT  *SilContext
  )
{
  RCMGR_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  RCMGR_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeRcMgrPhxTp3
 *
 * @brief Initialize PHX resource manager at Tp3.
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 */
SIL_STATUS
InitializeRcMgrPhxTp3 (
  SIL_CONTEXT  *SilContext
  )
{
  RCMGR_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  RCMGR_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeApiRcMgrPhx
 *
 * @brief   Initialize internal and external APIs for Resource Manager
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @retval  SilPass             API initialized successfully
 * @retval  SilInvalidParameter Id class is invalid
 *
 */
SIL_STATUS
InitializeApiRcMgrPhx (
  SIL_CONTEXT  *SilContext
  )
{
  SIL_STATUS  Status;

  // Initialize Common to Rev specific transfer table first
  Status = SilInitCommon2RevXferTable(SilContext, SilId_RcManager, (void *)&mRcMgrXferPhx);
  if (Status != SilPass) {
    return Status;
  }

  return SilInitIp2IpApi(SilContext, SilId_RcManager, (void *)&mRcMgrApiPhx);
}
