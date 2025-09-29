/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 *  @file MpioLib.h
 *  @brief Various top level defines / function prototypes for MPIO
 */

#pragma once

#include <xSIM.h>
#include <SilPcie.h>
#include <Mpio/MpioClass-api.h>

uint32_t
MpioServiceRequestCommon (
  SIL_CONTEXT              *SilContext,
  PCI_ADDR                 NbioPciAddress,
  uint32_t                 RequestId,
  uint32_t                 *RequestArgument,
  uint32_t                 AccessFlags
  );

SIL_STATUS
MpioEarlyInitV1 (
  SIL_CONTEXT                      *SilContext,
  MPIOCLASS_COMMON_INPUT_BLK       *SilDataCommon,
  PCIe_PLATFORM_CONFIG             *Pcie,
  GNB_HANDLE                       *StartHandle,
  MPIO_COMPLEX_DESCRIPTOR          *PlatformTopology
  );

SIL_STATUS
NbioInitializeDxio (
  SIL_CONTEXT                 *SilContext,
  PCIe_PLATFORM_CONFIG        *Pcie,
  MPIOCLASS_COMMON_INPUT_BLK  *SilDataCommon
  );

void
MpioSmnPrivateRegWrite (
  SIL_CONTEXT            *SilContext,
  GNB_HANDLE             *GnbHandle,
  uint32_t               RegisterIndex,
  uint32_t               *RegisterValue,
  uint32_t               Flags
  );

void
MpioSmnPrivateRegRead (
  SIL_CONTEXT            *SilContext,
  GNB_HANDLE             *GnbHandle,
  uint32_t               RegisterIndex,
  uint32_t               *RegisterValue
  );

void
MpioSmnPrivateRegRMW (
  SIL_CONTEXT        *SilContext,
  GNB_HANDLE         *GnbHandle,
  uint32_t           RegisterIndex,
  uint32_t           AndMask,
  uint32_t           OrValue,
  uint32_t           Flags
  );
