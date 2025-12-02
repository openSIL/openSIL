/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */
/**
 * @file  CxlIp2Ip.h
 * This file contains the definition of the CXL Ip 2 Ip API.
 * This API provides other openSIL IPs with any CXL functionality or data
 * they require while also abstracting revision specific differences.
 */

#pragma once

#include <SilCommon.h>
#include <Mpio/Common/MpioStructs.h>
#include <Nbio/Common/GnbDxio.h>

typedef void (*CXL_REMOVE_LINKS_FROM_TOPOLOGY) (
  MPIO_COMPLEX_DESCRIPTOR **CurrentComplexConfig,
  PCIe_PLATFORM_CONFIG    *Pcie
  );

typedef void (*CXL_GET_LINK_SPEED) (
  PCIe_PLATFORM_CONFIG  *Pcie,
  uint32_t              *CxlMsgBuffer
);

// Define the Ip2Ip API as a struct containing pointers to the above functions

typedef struct {
  CXL_REMOVE_LINKS_FROM_TOPOLOGY      RemoveCxlLinksFromTopology;
  CXL_GET_LINK_SPEED                  GetCxlLinkSpeed;
} CXL_IP2IP_API;
