/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT
/**
 * @file SdciClass-api.h
 * @brief openSIL-Host SDCI IP interface
 *
 * @details The Smart Data Cache Injection (SDCI) API adds the capability
 * to perform certain fabric writes directly into cache rather than to the DRAM
 * first.
 *
 *  This file provides the structures details for the Host to configure the
 *  SDCI operations.
 */

/** @cond API_Doc
 *  @ingroup MODULES_IP
 *  @page SDCI  SDCI IP api
 * *   The Smart Data Cache Injection (SDCI) OpenSIL module provides an interface
 *   to configure SDCI on each PCIe controller.
 *
 *
 *   See the 'Files - @ref SdciClass-api.h' section of this document for
 *   details.
 *
 *
 * @endcond
 */
#pragma once

#include <stdbool.h>

///  SDCI openSIL Input Block
typedef struct {
  bool AmdFabricSdci; ///< User configurable
} SDCICLASS_INPUT_BLK;
