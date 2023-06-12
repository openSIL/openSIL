/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file CxlCmn2Brh.c
 * @brief Defines C2R Table for CXL Brh
 *
 */

#pragma once

#include <SilCommon.h>
#include <Cxl/CxlClass-api.h>
#include <Cxl/Common/CxlInit.h>

SIL_STATUS
SetCxlApiBrh (void);

/**************** Common-2-Rev Transfer Table for BRH ******************/
/*
 *  prototypes for forward references of functions used in the table
 */
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

void
RemoveCxlLinksFromTopologyBrh (
  MPIO_COMPLEX_DESCRIPTOR **CurrentComplexConfig,
  PCIe_PLATFORM_CONFIG    *Pcie
  );

SIL_STATUS
InitializeBrhCxlTp1 (void);
