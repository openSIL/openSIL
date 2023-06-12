/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MultiFchCmn2Kl.h
 * @brief Kunlun FCH multi-die Common to Kunlun Transfer functions
 */

#pragma once

SIL_STATUS
FchMultiFchGetFchRbIndexOnDieKl (
  uint32_t *RbIndex
  );

SIL_STATUS
FchMultiFchInitAbKl (
  uint32_t SocketNum,
  uint32_t DieNum,
  uint32_t RbNum,
  uint64_t AcpiMmioBaseAddr,
  uint32_t IohcBusNumber,
  uint8_t  HasFchModule
  );

SIL_STATUS
FchMultiFchInitUsbKl (
  uint32_t SocketNum,
  uint32_t DieNum,
  uint32_t RbNum,
  uint64_t AcpiMmioBaseAddr,
  uint32_t IohcBusNumber,
  uint8_t  HasFchModule
  );

SIL_STATUS
FchMultiFchInitSdKl (
  uint32_t SocketNum,
  uint32_t DieNum,
  uint32_t RbNum,
  uint64_t AcpiMmioBaseAddr,
  uint32_t IohcBusNumber,
  uint8_t  HasFchModule
  );
