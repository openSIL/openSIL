/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file CcxIp2IpPhx.c
 * @brief PHX Zen4 core specific transfer table functions
 *
 */
#include <CcxIp2Ip.h>
#include "CcxIp2IpPhx.h"

/** ------------------------ Table ------------------------------
 * @details This is the Ip2Ip Table for PHX/Zen4
 */
CCX_IP2IP_API CcxIp2IpZen4Phx = {
  .CalcLocalApic = CalcLocalApicPhx,
  .GetPstateInfo = Zen4GetPstateInfo,
  .ReOrderLogicalCcdWithNumaDomainOrder = Zen4ReOrderLogicalCcdWithNumaDomainOrder
};
