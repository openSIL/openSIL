/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2026 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  xPrfCxl.h
 * @brief CXL endpoint discovery xPRF services.
 *
 * @details Host firmware manages CXL memory in DXE: it reconciles what the
 *          devices report against the APCB, converts ranges into the GCD and
 *          decides when a warm reset is needed. Reading what the devices report
 *          is device access, and that is what these services do.
 *
 *          Everything crosses as scalars and flat uint32_t arrays so that no
 *          structure needs a matching layout on both sides.
 */

#pragma once

#include <stdint.h>
#include <Sil-api.h>

/// Words xPrfCxlGetDeviceInfo writes: dvsec pointer, type, hdm count, hw init
/// mode, base class, then size high and size low for each of two HDM ranges
#define XPRF_CXL_DEVICE_INFO_WORDS  9

SIL_STATUS
xPrfCxlGetDeviceInfo (
  uint32_t  Address,
  uint32_t  BufferWords,
  uint32_t  *Buffer
  );

SIL_STATUS
xPrfCxlGetRangeSizeLow (
  uint32_t  Address,
  uint32_t  DvsecPtr,
  uint32_t  RangeIndex,
  uint32_t  *Value
  );

uint32_t
xPrfCxlGetLinkPortNumber (
  uint32_t  Address
  );

uint32_t
xPrfCxlFindPciCapability (
  uint32_t  Address,
  uint32_t  CapabilityId
  );

uint32_t
xPrfCxlFindPcieExtendedCapability (
  uint32_t  Address,
  uint32_t  StartCapabilityPtr,
  uint32_t  ExtendedCapabilityId
  );

uint32_t
xPrfCxlGetPcieDeviceType (
  uint32_t  Address
  );

SIL_STATUS
xPrfCxlGetSwitchEndpoints (
  uint32_t  SwitchAddress,
  uint32_t  MaxEndpoints,
  uint32_t  *Count,
  uint32_t  *Buffer
  );
