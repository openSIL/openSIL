/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  DfXFabricRegisterAcc.h
 * @brief Data fabric register access functionality header
 *
 */

#pragma once

#include <DF/Common/FabricRegisterAccCmn.h>

void
DfXFabricRegisterAccWrite (
  SIL_CONTEXT         *SilContext,
  uint32_t            Socket,
  uint32_t            Die,
  uint32_t            Function,
  uint32_t            Offset,
  uint32_t            Instance,
  uint32_t            Value
  );
uint32_t
DfXFabricRegisterAccRead (
  SIL_CONTEXT         *SilContext,
  uint32_t            Socket,
  uint32_t            Die,
  uint32_t            Function,
  uint32_t            Offset,
  uint32_t            Instance
  );
