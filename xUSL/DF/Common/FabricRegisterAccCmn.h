/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */
/**
 * @file  FabricRegisterAccCmn.h
 * @brief Common definitions for Df fabric register access
 *
 */

#pragma once

#include <SilCommon.h>
#include <ProjSocConst.h>

#define FABRIC_REG_ACC_BC    (0xFFFFFFFF)

uint32_t
DfFabricRegisterAccRMW (
  uint32_t Socket,
  uint32_t Die,
  uint32_t Function,
  uint32_t Offset,
  uint32_t Instance,
  uint32_t NandValue,
  uint32_t OrValue
  );
uint32_t
DfFabricRegisterAccGetPciDeviceNumberOfDie (
  uint32_t Socket,
  uint32_t Die
  );
