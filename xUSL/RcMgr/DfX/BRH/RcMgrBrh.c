/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  RcMgrBrh.c
 * @brief OpenSIL Resource Manager Silicon Init. Functions
 *
 */
#include <SilCommon.h>
#include <RcMgrIp2Ip.h>
#include <ProjSocConst.h>
#include <RcMgr/Common/RcMgrCmn2Rev.h>
#include <RcMgr/Common/FabricResourceManager.h>
#include "FabricRcManagerDfX.h"
#include "RcMgrBrh.h"
#include "FabricRcInitBrh.h"
#include <string.h>
#include <APOB/ApobIp2Ip.h>
#include <APOB/Common/ApobCmn.h>


extern RCMGR_COMMON_2_REV_XFER_BLOCK mRcMgrXferBrh;
extern RCMGR_IP2IP_API mRcMgrApiBrh;
extern const DFX_RCMGR_INPUT_BLK mRcMgrClassDflts;

/**--------------------------------------------------------------------
 * RcMgrSetInputBlkBrh
 *
 * @brief Input block API that assigns host memory block to the IP.
 *
 */
SIL_STATUS
RcMgrSetInputBlkBrh (void)
{
  DFX_RCMGR_INPUT_BLK *RcMgrInput;

  RcMgrInput = SilCreateInfoBlock(SilId_RcManager,
    sizeof (DFX_RCMGR_INPUT_BLK),
    RCMGR_INIT_INSTANCE,
    RCMGR_INIT_MAJOR_REV,
    RCMGR_INIT_MINOR_REV
    );

  if (RcMgrInput == NULL) {
    return SilAborted;
  }

  // fill RcMgr IP data structure with defaults
  memcpy((void *)RcMgrInput, &mRcMgrClassDflts, sizeof (DFX_RCMGR_INPUT_BLK));
  return SilPass;
}

/**--------------------------------------------------------------------
 * InitializeResourceManagerBrhTp1
 *
 * @brief Initialize BRH resource registers for each RootBridge.
 *
 */
SIL_STATUS
InitializeResourceManagerBrhTp1 (void)
{
  DFX_RCMGR_INPUT_BLK *SilData;
  SIL_STATUS          Status;
  APOB_IP2IP_API      *ApobIp2IpApi;
  APOB_SOC_DIE_INFO   SocMaxDieInfo;

  // Locate the IP block
  SilData = (DFX_RCMGR_INPUT_BLK *) xUslFindStructure(SilId_RcManager, 0);
  if (SilData == NULL) {
    RCMGR_TRACEPOINT(SIL_TRACE_INFO, "SIL RC Init memory block not found!\n");
    Status = SilNotFound; // Could not find the IP input block
  } else {
    RCMGR_TRACEPOINT(SIL_TRACE_INFO, "SIL RC Init memory block is found blk at: 0x%x \n", SilData);

    Status = SilPass;

    Status = SilGetIp2IpApi(SilId_ApobClass, (void **) &ApobIp2IpApi);
    if ((Status != SilPass) || (ApobIp2IpApi == NULL)) {
      assert(Status == SilPass);
      return Status;
    }

    ApobIp2IpApi->ApobGetMaxDieInfo(&SocMaxDieInfo);

    // These asserts prevent tainted scalar coverity warnings by checking values
    // are within permissible ranges
    RCMGR_TRACEPOINT(SIL_TRACE_INFO, "SilData->SocketNumber: 0x%x \n", SilData->SocketNumber);
    RCMGR_TRACEPOINT(SIL_TRACE_INFO, "SilData->RbsPerSocket: 0x%x \n", SilData->RbsPerSocket);
    assert((SilData->SocketNumber > 0) && (SilData->SocketNumber <=
      SocMaxDieInfo.MaxSocSocketsSupportedValue)
      );
    assert((SilData->RbsPerSocket > 0) && (SilData->RbsPerSocket <= PROJ_MAX_RBS_PER_SOCKET));

    // Initialize MMIO
    if (SilData->SetRcBasedOnNv) {
      // Got NvVariable successfully, try to init MMIO based on it
      RCMGR_TRACEPOINT(SIL_TRACE_INFO, "  Init MMIO based on NV variable\n");
      Status = SilInitMmioBasedOnNvVariable4(SilData, NULL, true);
    }

    if ((!SilData->SetRcBasedOnNv) || (Status != SilPass)) {
      RCMGR_TRACEPOINT(SIL_TRACE_INFO, " Can't get NV variable or init MMIO based on NV variable failed.\n");
      RCMGR_TRACEPOINT(SIL_TRACE_INFO, " Init MMIO equally.\n");
      Status = SilInitMmioEqually4(SilData);
    }

    #ifdef RCMGR_TEST
    RCMGR_TRACEPOINT(SIL_TRACE_INFO, "Dump MmioRcMgr struct from openSIL:\n");
    xUslDumpBuffer((void *)&SilData->MmioRcMgr, sizeof (SilData->MmioRcMgr), 1);
    #endif

    if (Status == SilPass) {
      #ifdef RCMGR_TEST
      RCMGR_TRACEPOINT(SIL_TRACE_INFO, "MMIO Rc Manager struct setup status = SilPass\n");
      #endif
      // Initialize IO
      if (SilData->SetRcBasedOnNv) {
        // Get NvVariable successfully, try to init IO base on it
        RCMGR_TRACEPOINT(SIL_TRACE_INFO, "  Init IO based on NV variable\n");
        Status = SilInitIoBasedOnNvVariable4(SilData, NULL, true);
      }

      if ((!SilData->SetRcBasedOnNv) || (Status != SilPass)) {
        RCMGR_TRACEPOINT(SIL_TRACE_INFO, " Can't get NV variable or init IO based on NV variable failed.\n");
        RCMGR_TRACEPOINT(SIL_TRACE_INFO, "                      Init IO equally.\n");
        Status = SilInitIoEqually4(SilData);
      }

      if (Status == SilPass) {
        if (SilData->SetRcBasedOnNv) {
          Status = SilInitPciBusBasedOnNvVariableBrh(SilData);
        }
      }

      #ifdef RCMGR_TEST
      RCMGR_TRACEPOINT(SIL_TRACE_INFO, "Dump IoRcMgr struct from openSIL:\n");
      xUslDumpBuffer((void *)&SilData->IoRcMgr, sizeof (SilData->IoRcMgr), 1);
      #endif
    }
  }

  return Status;
}

/**--------------------------------------------------------------------
 * InitializeResourceManagerDfXTp2
 *
 * @brief Initialize BRH resource manager at Tp2.
 *
 */
SIL_STATUS
InitializeResourceManagerBrhTp2 (void)
{
  return SilPass;
}

/**--------------------------------------------------------------------
 * InitializeResourceManagerDfXTp3
 *
 * @brief Initialize DFX at Tp3.
 *
 */
SIL_STATUS
InitializeResourceManagerBrhTp3 (void)
{
  return SilPass;
}

/**--------------------------------------------------------------------
 * InitializeRcMgrApiBrh
 *
 * @brief   Initialize internal and external APIs for Resource Manager
 *
 * @retval  SilPass             API initialized successfully
 * @retval  SilInvalidParameter Id class is invalid
 *
 */
SIL_STATUS
InitializeRcMgrApiBrh (void)
{
  return SilInitIp2IpApi(SilId_RcManager, (void *)&mRcMgrApiBrh);
}
