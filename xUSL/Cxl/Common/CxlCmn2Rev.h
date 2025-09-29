/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 * @file CxlCmn2Rev.h
 * @brief Common-2-Rev core specific functions
 * @details  This files declares the structure for the common-2-Rev
 *  transfer table and the types of all functions contained within
 *  the CXL Cmn2Rev transfer table.
 *
 */

#pragma once

#include <Cxl/Common/CxlInit.h>
#include <xSIM-api.h>

typedef void (*CXL_ASSIGN_RESOURCES) (
  SIL_CONTEXT           *SilContext,
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  );

typedef void (*CXL_FIND_PORTS) (
  SIL_CONTEXT           *SilContext,
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  );

typedef void (*CXL_DEV_LIST_GENERATE) (
  PCIe_PLATFORM_CONFIG  *Pcie,
  CXL_INFO_LIST     *CxlInfo
  );

// Internal Common-2-Rev Transfer Block for CXL
typedef struct {
  CXL_ASSIGN_RESOURCES     CxlAssignResources;
  CXL_FIND_PORTS           CxlFindPorts;
  CXL_DEV_LIST_GENERATE    CxlDevListGenerate;
} CXL_COMMON_2_REV_XFER_BLOCK;
