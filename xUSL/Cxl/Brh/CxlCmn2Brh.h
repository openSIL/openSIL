/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file CxlCmn2Brh.h
 * @brief Prototypes for CXL Brh specific C2R table functions
 *
 */

#pragma once

#include <Mpio/Common/MpioInitLib.h>

void
CxlAssignResourcesBrh (
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  );

void
CxlFindPortsBrh (
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  );

void
CxlDevListGenerateBrh (
  PCIe_PLATFORM_CONFIG  *Pcie
  );
