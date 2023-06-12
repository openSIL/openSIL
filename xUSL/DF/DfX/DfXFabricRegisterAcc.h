/**
 * @file  DfXFabricRegisterAcc.h
 * @brief OpenSIL DataFabric access functions header.
 *
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <DF/Common/FabricRegisterAccCmn.h>


void
DfXFabricRegisterAccWrite (
  uint32_t Socket,
  uint32_t Function,
  uint32_t Offset,
  uint32_t Instance,
  uint32_t Value
  );
uint32_t
DfXFabricRegisterAccRead (
  uint32_t Socket,
  uint32_t Function,
  uint32_t Offset,
  uint32_t Instance
  );
