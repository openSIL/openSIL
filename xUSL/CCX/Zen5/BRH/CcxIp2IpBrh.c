/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file CcxIp2IpBrh.c
 * @brief BRH Zen5 core specific I2I API table functions
 *
 */

#include <CcxIp2Ip.h>
#include "CcxIp2IpBrh.h"

/** ------------------------ Table ------------------------------
 * @details This is the Ip2Ip Table for Zen5
 */
CCX_IP2IP_API CcxIp2IpZen5Brh = {
  .CalcLocalApic = CalcLocalApicBrh,
  .GetPstateInfo = Zen5GetPstateInfo,
  .ReOrderLogicalCcdWithNumaDomainOrder = Zen5ReOrderLogicalCcdWithNumaDomainOrder
};
