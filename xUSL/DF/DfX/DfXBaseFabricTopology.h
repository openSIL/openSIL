/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  DfXBaseFabricTopology.h
 * @brief DfX specific declarations of BaseFabricTopology functions
 */


#pragma once

uint32_t
DfXGetNumberOfSystemDies (
  SIL_CONTEXT          *SilContext
  );
uint32_t DfXGetNumberOfSystemRootBridges (
  SIL_CONTEXT          *SilContext
  );
uint32_t
DfXGetNumberOfRootBridgesOnSocket (
  SIL_CONTEXT          *SilContext,
  uint32_t             Socket
  );
uint32_t
DfXGetNumberOfRootBridgesOnDie (
  SIL_CONTEXT          *SilContext,
  uint32_t             Socket,
  uint32_t             Die
  );
