/**
 *  @file MpioLib.h
 *  @brief Various top level defines / fuction prototypes for MPIO
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <xSIM.h>
#include <SilPcie.h>
#include <Mpio/MpioClass-api.h>

uint32_t
MpioServiceRequestCommon (
  PCI_ADDR                 NbioPciAddress,
  uint32_t                 RequestId,
  uint32_t                 *RequestArgument,
  uint32_t                 AccessFlags
  );

void
ParseTopologyForUbm (
  MPIOCLASS_INPUT_BLK       *SilData,
  MPIO_COMPLEX_DESCRIPTOR   **ComplexDescriptor,
  PCIe_PLATFORM_CONFIG      *Pcie
  );

SIL_STATUS
MpioEarlyInitV1 (
  MPIOCLASS_INPUT_BLK       *SilData,
  PCIe_PLATFORM_CONFIG      *Pcie,
  GNB_HANDLE                *StartHandle,
  MPIO_COMPLEX_DESCRIPTOR   *PlatformTopology
  );

SIL_STATUS
NbioInitializeDxio (
  PCIe_PLATFORM_CONFIG        *Pcie,
  MPIOCLASS_INPUT_BLK         *SilData
);

void
PcieAriInit (
  GNB_HANDLE  *GnbHandle
  );
