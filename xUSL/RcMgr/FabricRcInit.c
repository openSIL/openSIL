/**
 * @file  FabricResourceInit.c
 * @brief OpenSIL DataFabric MMIO and IO resource map initialization.
 *
 */
 /* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include "RcMgrIp2Ip.h"
#include <DF/DfIp2Ip.h>
#include <RcMgr/FabricResourceManager.h>

RCMGR_IP2IP_API RcMgrApi = {
    .FabricReserveMmio    = FabricReserveMmio
};

/**
 * SilGetPrimaryRb
 *
 * @brief Returns primary RB socket number and RB number
 *
 * @param[out]  SocketNum     Primary RB socket number
 * @param[out]  RootBridgeNum Primary RB number
 *
 */
void
SilGetPrimaryRb (
  uint32_t *SocketNum,
  uint32_t *RootBridgeNum
  )
{
  ROOT_BRIDGE_LOCATION Location;
  DF_IP2IP_API*        DfIp2IpApi;
  SIL_STATUS           Status;

  Status = SilGetIp2IpApi (SilId_DfClass, (void**) &DfIp2IpApi);
  assert (Status == SilPass);

  if (DfIp2IpApi->DfGetSystemComponentRootBridgeLocation (PrimaryFch, &Location)) {
    *SocketNum = Location.Socket;
    *RootBridgeNum = Location.Index;
  } else {
    *SocketNum = 0;
    *RootBridgeNum = 0;
  }
}

/**
 * InitializeRcMgrApi
 *
 * @brief   Initialize internal and external APIs for Resource Manager
 *
 * @retval  SilPass             API initialized successfully
 * @retval  SilInvalidParameter Id class is invalid
 *
 */
SIL_STATUS
InitializeRcMgrApi (void)
{
  return SilInitIp2IpApi (SilId_RcManager, (void *)&RcMgrApi);
}
