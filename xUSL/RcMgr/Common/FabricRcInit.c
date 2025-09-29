/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FabricRcInit.c
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
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   SocketNum           Primary RB socket number
 * @param   RootBridgeNum       Primary RB number
 *
 */
void
SilGetPrimaryRb (
  SIL_CONTEXT      *SilContext,
  uint32_t         *SocketNum,
  uint32_t         *DieNum,
  uint32_t         *RootBridgeNum
  )
{
  ROOT_BRIDGE_LOCATION Location;
  DF_IP2IP_API *DfIp2IpApi;
  SIL_STATUS           Status;

  Status = SilGetIp2IpApi(SilContext, SilId_DfClass, (void **) &DfIp2IpApi);
  if (Status != SilPass) {
    assert(Status == SilPass);
    return;
  }

  if (DfIp2IpApi->DfGetSystemComponentRootBridgeLocation(SilContext, PrimaryFch, &Location)) {
    *SocketNum = Location.Socket;
    *DieNum = Location.Die;
    *RootBridgeNum = Location.Index;
  } else {
    *SocketNum = 0;
    *DieNum = 0;
    *RootBridgeNum = 0;
  }
}
