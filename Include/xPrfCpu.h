/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2026 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file xPrfCpu.h
 * @brief Enabled processor topology for host CPU startup and locality tables.
 */

#pragma once

#include <stdint.h>
#include <Sil-api.h>

/**
 * One enabled hardware thread. Coordinates are logical APOB map indices, not
 * physical CCD/CCX/core numbers. They are suitable for the fabric domain
 * translation service. ApicId comes from the CCX Ip-2-Ip APIC calculator.
 */
typedef struct {
  uint32_t ApicId;
  uint32_t Socket;
  uint32_t Die;
  uint32_t Ccd;
  uint32_t Complex;
  uint32_t Core;
  uint32_t Thread;
} XPRF_CPU_TOPOLOGY;

/**
 * Enumerate enabled threads from every installed socket's per-die APOB map.
 *
 * Call after DF/APOB/CCX services are initialized (after timepoint 1 on Turin).
 * The installed socket/die counts come from DF. Each die's logical-to-physical
 * map and thread enable flags come from APOB; compile-time maxima only bound
 * validation. A missing map or failed service is an error, never a reason to
 * reuse another die's map or report a partial population. No hardware is
 * initialized by this query and no provider pointers escape to the caller.
 *
 * The host must reconcile these calculated APIC IDs with the CPUs that actually
 * start before publishing CPU affinity tables. Output order is socket, die,
 * logical CCD, complex, core, then thread; it does not define an ACPI CPU UID.
 *
 * @param[in]  Capacity Number of records available in Cpus, not a byte count.
 * @param[out] Count    Complete enabled-thread count on SilPass; zero on every
 *                     error when Count is non-NULL. There is no sizing mode.
 * @param[out] Cpus     Caller-owned array. Only valid on SilPass. An error can
 *                     leave partial records in the buffer; discard them all.
 *
 * @retval SilPass             Complete nonempty topology was returned.
 * @retval SilInvalidParameter A required pointer is NULL or Capacity is zero.
 * @retval SilNotFound         A required Ip-2-Ip table is unavailable.
 * @retval SilUnsupported      A required Ip-2-Ip callback is unavailable.
 * @retval SilOutOfBounds      Capacity is insufficient or topology exceeds its
 *                            supported bounds.
 * @retval SilAborted          Topology is empty or inconsistent, including
 *                            duplicate physical locations or APIC IDs.
 * Other service failures are propagated unchanged.
 */
SIL_STATUS
xPrfGetEnabledCpuTopology (
  uint32_t           Capacity,
  uint32_t           *Count,
  XPRF_CPU_TOPOLOGY  *Cpus
  );
