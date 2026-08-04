/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file RcMgrIp2IpPhx.c
 * @brief PHX RCMGR core specific transfer table functions
 *
 */
#include <RcMgrIp2Ip.h>
#include <RcMgr/Common/FabricResourceManager.h>
#include "RcMgrIp2IpPhx.h"

RCMGR_IP2IP_API mRcMgrApiPhx = {
  .FabricReserveMmio = FabricReserveMmio,
  .FabricEnableVgaMmio = FabricEnableVgaMmio
};
