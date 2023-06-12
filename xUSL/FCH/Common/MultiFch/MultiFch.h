/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MultiFch.h
 * @brief Secondary FCH initialization functions prototype
 *
 */

#pragma once

#include <Sil-api.h>
#include <FCH/FchClass-api.h>

#define SIL_RESERVED_0351  0x0
#define SIL_RESERVED_0352 0x0

SIL_STATUS
FchMultiFchGetAcpiMmioBaseAddress (
  uint32_t SocketNum,
  uint32_t DieNum,
  uint64_t *AcpiMmioBaseAddr
  );

SIL_STATUS
FchMultiFchGetFchIohcBusNumber (
  uint32_t SocketNum,
  uint32_t DieNum,
  uint32_t *IohcBaseAddr
  );

SIL_STATUS
FchMultiFchHasFchModule (
  uint32_t SocketNum,
  uint32_t DieNum,
  uint8_t  *HasFchModule
  );

SIL_STATUS
FchMultiFchGetFchRbIndexOnDieCmn (
  uint32_t *RbIndex
  );

SIL_STATUS
FchMultiFchReadIoBaseAddress (
  uint32_t SocketNum,
  uint32_t DieNum,
  uint32_t *IoBaseVal,
  uint32_t *IoLimitVal
  );

SIL_STATUS
FchMultiFchWriteIoBaseAddress (
  uint32_t SocketNum,
  uint32_t DieNum,
  uint32_t IoBaseVal,
  uint32_t IoLimitVal
  );

SIL_STATUS
FchMultiFchDispatchSecondaryInits (
  void
  );

SIL_STATUS
FchMultiFchInitAcpiMmioAllocation (
  void
  );

SIL_STATUS
FchMultiFchSetInputBlk (
  void
  );
