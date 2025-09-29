/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  NbioCmn2Rev.h
 * This file defines the types of all functions contained within Nbio's Cmn2Rev
 * xfer table, as well as the table itself as a struct containing pointers to
 * these functions
 */


#pragma once

// Common function type definitions of Cmn2Rev functions

typedef void (*NBIO_PCIE_FIXUP_COMPLEX_DEFAULTS) (
  SIL_CONTEXT               *SilContext,
  PCIe_PLATFORM_CONFIG      *Pcie
  );

typedef void (*NBIO_PCIE_FIXUP_PLATFORM_CONFIG) (
  SIL_CONTEXT          *SilContext,
  PCIe_PLATFORM_CONFIG *Pcie
  );

typedef void (*NBIO_ENUMERATE_HARVEST_WRAPPERS) (
  PCIe_PLATFORM_CONFIG *Pcie
  );

typedef void * (*NBIO_GET_PCIECOMPLEX_POINTER) (
  uint32_t   SocketIndex,
  uint32_t   DieIndex,
  uint32_t   RootBridgeInde
  );

typedef uint32_t (*NBIO_GET_PCIECOMPLEX_SIZE) (
  uint32_t   SocketIndex,
  uint32_t   DieIndex,
  uint32_t   RootBridgeIndex
  );

typedef void (*NBIO_IOAPIC_MMIO_ADDRESS) (
  GNB_HANDLE     *GnbHandle,
  uint64_t       IoapicMmioBase
  );

typedef void (*NBIO_IOAPIC_PREDEF_ID) (
  GNB_HANDLE     *GnbHandle,
  uint8_t        IoApicIdBase
  );

// Define the Cmn2Rev xfer table containing pointers to these functions

typedef struct {
  NBIO_IOAPIC_MMIO_ADDRESS          NbioIoApicMmioAddress;
  NBIO_IOAPIC_PREDEF_ID             NbioIoApicPreDefId;
  NBIO_GET_PCIECOMPLEX_SIZE         NbioGetPcieComplexSize;
  NBIO_GET_PCIECOMPLEX_POINTER      NbioGetPcieComplexPointer;
  NBIO_ENUMERATE_HARVEST_WRAPPERS   NbioEnumerateHarvestWrappers;
  NBIO_PCIE_FIXUP_PLATFORM_CONFIG   NbioPcieFixupPlatformConfig;
  NBIO_PCIE_FIXUP_COMPLEX_DEFAULTS  NbioPcieFixupComplexDefaults;
} NBIO_COMMON_2_REV_XFER_BLOCK;
