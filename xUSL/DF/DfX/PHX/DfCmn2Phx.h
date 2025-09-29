/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2024 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  DfCmn2Phx.h
 * @brief Data Fabric common to PHX definitions
 *
 */

#pragma once

#include <SilCommon.h>
#include <DF/Common/SilBaseFabricTopologyLib.h>

bool
PhxHasFch (
  SIL_CONTEXT       *SilContext,
  uint32_t          Socket,
  uint32_t          Die,
  uint32_t          Index
  );

bool
PhxHasSmu (
  SIL_CONTEXT       *SilContext,
  uint32_t          Socket,
  uint32_t          Die,
  uint32_t          Index
  );

bool
PhxGetSystemComponentRootBridgeLocation (
  SIL_CONTEXT           *SilContext,
  COMPONENT_TYPE        Component,
  ROOT_BRIDGE_LOCATION  *Location
  );
