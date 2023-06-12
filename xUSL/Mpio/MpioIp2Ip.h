/**
 * @file  MpioIp2Ip.h
 * This file contains the definition of the MPIO Ip 2 Ip API.
 * This API provides other openSIL IPs with any MPIO functionality or data
 * they require while also abstracting revision specific differences.
 */
/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <SilCommon.h>
#include <NBIO/GnbDxio.h>

typedef uint32_t (*MPIO_SERVICE_REQUEST) (
  PCI_ADDR                 NbioPciAddress,
  uint32_t                 RequestId,
  uint32_t                 *RequestArgument,
  uint32_t                 AccessFlags
  );

typedef void (*MPIO_WRITE_PCIE_STRAP) (
  GNB_HANDLE     *GnbHandle,
  uint16_t       StrapIndex,
  uint32_t       Value,
  uint8_t        Wrapper
  );

typedef uint16_t (*MPIO_GET_PORT_STRAP_INDEX) (
  uint16_t Strap,
  uint16_t Port
  );

typedef uint16_t (*GET_DPC_CAPABILITY_STRAP) (void);

typedef uint16_t (*GET_TPH_SUPPORT_STRAP) (void);

typedef SIL_STATUS (*MPIO_VER_INFO) (
    uint8_t        *buffer      // Caller's location to place the version string
  );

// Define the Ip2Ip API as a struct containing pointers to the above functions

typedef struct {
  MPIO_SERVICE_REQUEST                MpioServiceRequest;
  MPIO_WRITE_PCIE_STRAP               MpioWritePcieStrap;
  MPIO_GET_PORT_STRAP_INDEX           MpioGetPortStrapIndex;
  GET_DPC_CAPABILITY_STRAP            MpioGetDpcCapabilityStrap;
  GET_TPH_SUPPORT_STRAP               MpioGetTphSupportStrap;
  MPIO_VER_INFO                       MpioGetVersionInfo;
} MPIO_IP2IP_API;
