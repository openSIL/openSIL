/* SPDX-License-Identifier: MIT */
/*********************************************************************
 *  Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 ********************************************************************/
/**
 * @file SdxiClass-api.h
 * @brief openSIL-Host SDXI IP interface
 *
 * @details The Cacheable Direct Memory Access (SDXI) API adds the capability
 * to perform certain fabric writes directly into cache rather than to the DRAM
 * first.
 *
 *  This file provides the structures details for the Host to configure the
 *  SDXI operations.
 */

/** @cond API_Doc
 *  @ingroup MODULES_IP
 *  @page SDXI  SDXI IP api
 *
 *   The Cacheable Direct Memory Access (SDXI) OpenSIL module provides an interface
 *   to configure SDXI on each PCIe controller.
 *
 *
 *   See the 'Files - @ref SdxiClass-api.h' section of this document for
 *   details.
 *
 *
 * @endcond
 */
#pragma once

#include <stdbool.h>

///  SDXI openSIL Input Block
typedef struct {
  bool AmdFabricSdxi; ///< User configurable
} SDXICLASS_INPUT_BLK;
