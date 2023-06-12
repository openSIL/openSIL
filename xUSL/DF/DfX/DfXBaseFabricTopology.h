/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */
/**
 * @file  DfXBaseFabricTopology.h
 * @brief DfX specific declarations of BaseFabricTopology functions
 */

#pragma once

uint32_t DfXGetNumberOfSystemDies (void);
uint32_t DfXGetNumberOfSystemRootBridges (void);
uint32_t
DfXGetNumberOfRootBridgesOnSocket (
  uint32_t Socket
  );
uint32_t
DfXGetNumberOfRootBridgesOnDie (
  uint32_t Socket,
  uint32_t  Die
  );
