/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  NbioCmn2RevPhx.h
 * @brief PHX NBIO core specific transfer table functions prototypes
 */

#pragma once

#include <Nbio/Common/GnbDxio.h>

void
NbioPcieFixupComplexDefaultsPhx (
  SIL_CONTEXT              *SilContext,
  PCIe_PLATFORM_CONFIG     *Pcie
  );

void
NbioPcieFixupPlatformConfigPhx (
  SIL_CONTEXT          *SilContext,
  PCIe_PLATFORM_CONFIG *Pcie
  );

void
NbioEnumerateHarvestWrappersPhx (
  PCIe_PLATFORM_CONFIG *Pcie
  );

void *
NbioGetPcieComplexPointerPhx (
  uint32_t   SocketIndex,
  uint32_t   DieIndex,
  uint32_t   RootBridgeIndex
  );

uint32_t
NbioGetPcieComplexSizePhx (
  uint32_t   SocketIndex,
  uint32_t   DieIndex,
  uint32_t   RootBridgeIndex
  );

void
NbioIoApicMmioAddressPhx (
  GNB_HANDLE                    *GnbHandle,
  uint64_t                      IoapicMmioBase
  );

void
NbioIoApicPreDefIdPhx (
  GNB_HANDLE                     *GnbHandle,
  uint8_t                        IoApicIdBase
  );
