/**
 * @file  FabricRegisterAccCmn.h
 * @brief Common definitions for Df fabric register access
 *
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <SilCommon.h>

#define FABRIC_REG_ACC_BC    (0xFF)

uint32_t
DfFabricRegisterAccRMW (
  uint32_t Socket,
  uint32_t Function,
  uint32_t Offset,
  uint32_t Instance,
  uint32_t NandValue,
  uint32_t OrValue
  );
uint32_t
DfFabricRegisterAccGetPciDeviceNumberOfDie (
  uint32_t Socket
  );
