/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  NbioCmn2RevBrh.h
 * @brief OpenSIL NBIO initialization
 * This file contain the rev specific functions prototypes.
 */

#pragma once

#include <Nbio/Common/GnbDxio.h>

void
NbioPcieFixupComplexDefaultsBrh (
  PCIe_PLATFORM_CONFIG     *Pcie
  );

void
NbioPcieFixupPlatformConfigBrh (
  PCIe_PLATFORM_CONFIG *Pcie
  );

void
NbioEnumerateHarvestWrappersBrh (
  PCIe_PLATFORM_CONFIG *Pcie
  );

void
NbioPcieConfigAttachComplexesBrh (
  PCIe_COMPLEX_CONFIG *Base,
  PCIe_COMPLEX_CONFIG *New
  );

void *
NbioGetPcieComplexPointerBrh (
  uint32_t   SocketIndex,
  uint32_t   DieIndex,
  uint32_t   RootBridgeIndex
  );

uint32_t
NbioGetPcieComplexSizeBrh (
  uint32_t   SocketIndex,
  uint32_t   DieIndex,
  uint32_t   RootBridgeIndex
  );

void
NbioIoApicMmioAddressBrh (
  GNB_HANDLE                    *GnbHandle,
  uint64_t                      IoapicMmioBase
  );

void
NbioIoApicPreDefIdBrh (
  GNB_HANDLE                     *GnbHandle,
  uint8_t                        IoApicIdBase
  );
