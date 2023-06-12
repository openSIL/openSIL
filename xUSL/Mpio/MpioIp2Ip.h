/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */
/**
 * @file  MpioIp2Ip.h
 * This file contains the definition of the MPIO Ip 2 Ip API.
 * This API provides other openSIL IPs with any MPIO functionality or data
 * they require while also abstracting revision specific differences.
 */

#pragma once

#include <SilCommon.h>
#include <Nbio/Common/GnbDxio.h>

typedef enum {
  MPIO_MSG_CXL_INITIALIZE = 0x0D,   ///< 0x0D - Initialize the list of CXL devices for MPIO firmware
} CXL_MESSAGES;

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

typedef uint16_t (*GET_MEMBAR0_SIZE_STRAP) (void);

typedef uint16_t (*GET_CXL_MODE_STRAP) (void);

typedef void (*MPIO_SMN_PRIVATE_REG_READ) (
  GNB_HANDLE             *GnbHandle,
  uint32_t               RegisterIndex,
  uint32_t               *RegisterValue
  );

typedef void (*MPIO_SMN_PRIVATE_REG_RMW) (
  GNB_HANDLE         *GnbHandle,
  uint32_t           RegisterIndex,
  uint32_t           AndMask,
  uint32_t           OrValue,
  uint32_t           Flags
  );

// Define the Ip2Ip API as a struct containing pointers to the above functions

typedef struct {
  MPIO_SERVICE_REQUEST                MpioServiceRequest;
  MPIO_WRITE_PCIE_STRAP               MpioWritePcieStrap;
  MPIO_GET_PORT_STRAP_INDEX           MpioGetPortStrapIndex;
  GET_DPC_CAPABILITY_STRAP            MpioGetDpcCapabilityStrap;
  GET_TPH_SUPPORT_STRAP               MpioGetTphSupportStrap;
  GET_MEMBAR0_SIZE_STRAP              MpioGetMembar0SizeStrap;
  GET_CXL_MODE_STRAP                  MpioGetCxlModeStrap;
  MPIO_SMN_PRIVATE_REG_READ           MpioSmnPrivateRegisterRead;
  MPIO_SMN_PRIVATE_REG_RMW            MpioSmnPrivateRegisterRMW;
} MPIO_IP2IP_API;
