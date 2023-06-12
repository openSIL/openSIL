/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */
/**
 * @file  FabricResourceInit.c
 * @brief OpenSIL DataFabric MMIO and IO resource map initialization.
 *
 */

#include <SilCommon.h>
#include <DF/DfIp2Ip.h>
#include <RcMgr/Common/FabricResourceManager.h>

/**
 * SilGetPrimaryRb
 *
 * @brief Returns primary RB socket number and RB number
 *
 * @param[out]  SocketNum     Primary RB socket number
 * @param[out]  DieNum        Primary RB die number
 * @param[out]  RootBridgeNum Primary RB number
 *
 */
void
SilGetPrimaryRb (
  uint32_t *SocketNum,
  uint32_t *DieNum,
  uint32_t *RootBridgeNum
  )
{
  ROOT_BRIDGE_LOCATION Location;
  DF_IP2IP_API *DfIp2IpApi;
  SIL_STATUS           Status;

  Status = SilGetIp2IpApi(SilId_DfClass, (void **) &DfIp2IpApi);
  assert(Status == SilPass);

  if (DfIp2IpApi->DfGetSystemComponentRootBridgeLocation(PrimaryFch, &Location)) {
    *SocketNum = Location.Socket;
    *DieNum = Location.Die;
    *RootBridgeNum = Location.Index;
  } else {
    *SocketNum = 0;
    *DieNum = 0;
    *RootBridgeNum = 0;
  }
}
