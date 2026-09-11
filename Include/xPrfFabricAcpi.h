/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2026 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  xPrfFabricAcpi.h
 * @brief Fabric locality xPRF services behind SRAT, CRAT, SLIT, CDIT and MSCT.
 *
 * @details Host firmware formats the tables and sources the fabric data through
 *          these services.
 *
 *          Every service writes into a caller supplied buffer and takes only
 *          scalars, so no structure has to keep a matching layout on both sides
 *          of the boundary. Nothing is cached, so a caller decides when the
 *          data is final rather than depending on a time point having run.
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <Sil-api.h>

SIL_STATUS
xPrfFabricAcpiGetDomainCounts (
  uint32_t  *ReportedDomainCount,
  uint32_t  *PhysicalDomainCount,
  uint32_t  *PhysNodesPerSocket,
  uint32_t  *SystemCxlCount,
  uint32_t  *MaxDomains,
  bool      *CcxAsNuma
  );

SIL_STATUS
xPrfFabricAcpiDomainXlat (
  uint32_t  Socket,
  uint32_t  Die,
  uint32_t  Ccd,
  uint32_t  Ccx,
  uint32_t  *Domain
  );

SIL_STATUS
xPrfFabricAcpiGetReportedDomains (
  uint32_t  MaxDomains,
  uint32_t  *Count,
  uint32_t  *Buffer
  );

SIL_STATUS
xPrfFabricAcpiGetPhysicalDomains (
  uint32_t  MaxDomains,
  uint32_t  *Count,
  uint32_t  *Buffer
  );

SIL_STATUS
xPrfFabricAcpiGetMemoryEntries (
  uint32_t  MaxEntries,
  uint32_t  *MemEntryCount,
  uint32_t  *MemEntryBuffer,
  uint32_t  *MemInfoCount,
  uint32_t  *MemInfoBuffer
  );

SIL_STATUS
xPrfFabricAcpiGetDistanceInfo (
  uint32_t  DomainCount,
  uint32_t  BufferSize,
  uint8_t   *Distance
  );

SIL_STATUS
xPrfFabricAcpiGetPxmDomains (
  uint32_t  BusBase,
  uint32_t  MaxCount,
  uint32_t  *Count,
  uint32_t  *Domains
  );

uint32_t
xPrfFabricRegisterAccRead (
  uint32_t  Socket,
  uint32_t  Die,
  uint32_t  Function,
  uint32_t  Offset,
  uint32_t  Instance
  );

void
xPrfFabricRegisterAccWrite (
  uint32_t  Socket,
  uint32_t  Die,
  uint32_t  Function,
  uint32_t  Offset,
  uint32_t  Instance,
  uint32_t  Value
  );
