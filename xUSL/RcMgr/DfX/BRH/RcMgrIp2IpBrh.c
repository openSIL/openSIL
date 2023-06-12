/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  RcMgrIp2IpBrh.c
 * @brief OpenSIL Resource Manager BRH I2I API block
 *
 */

#include <RcMgrIp2Ip.h>
#include <FabricResourceManager.h>

RCMGR_IP2IP_API
  mRcMgrApiBrh = {
  .FabricReserveMmio = FabricReserveMmio
};
