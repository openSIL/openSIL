/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file CxlIp2IpBrh.c
 * @brief Defines I2I Table for CXL Brh
 *
 */

#include <Cxl/CxlIp2Ip.h>
#include "CxlIp2IpBrh.h"

/** ---------------------------- Ip2Ip API Table ---------------------------------
 * @details This is the internal common-2-Rev transfer table for BRH
 */
CXL_IP2IP_API CxlBrhApi = {
  .RemoveCxlLinksFromTopology = RemoveCxlLinksFromTopologyBrh,
  .GetCxlLinkSpeed = GetCxlLinkSpeedBrh
};
