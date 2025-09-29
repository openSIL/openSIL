/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  xSIM.h
 * @brief xSIM Internal definitions
 * This file is used to declare the functions, variables and macros used
 * by inside the xSIM core. These are not intended for Host access.
 */

/**
 * @ingroup xSIM_group
 * @defgroup xSIMcore_group xSIMcoregroup: Early Init operations center
 * The xSIM library is comprised of IP blocks that each work with their
 * corresponding silicon IP block within the Soc. The core xSIM routines
 * provide the host entry points for initialization of all the various
 * IP components.
 * The primary entry points are:
 * - @ref xSimQueryMemoryRequirements
 * - @ref xSimAssignMemoryTp1
 * - @ref xSimAssignMemoryTp2
 * - @ref xSimAssignMemoryTp3
 * - @ref SilFindStructure
 * - @ref xSimDebugSetup
 * - @ref InitializeAMDSiTp1
 * - @ref InitializeAMDSiTp2
 * - @ref InitializeAMDSiTp3
 *
 * Please refer to the API document for information on how these fit
 * in the design.
 * Additional related service functions for use by the xSIM components are:
 *  - @ref SilCreateInfoBlock
 */

#pragma once

#include <SilCommon.h>
#include <xSIM-api.h>

/* *************************************************************************
 *  openSIL Common Structures
 *
 */
#define KILOBYTE 1024

/* *************************************************************************
 *  Common Variables
 *
 */

/*********************************************************************
 * private Function prototypes
 *********************************************************************/

void *
SilCreateInfoBlock (
  SIL_CONTEXT   *SilContext,
  uint32_t      BlockTag,
  size_t        BlockSize,
  uint16_t      Block_Instance,
  uint8_t       Block_MajorRev,
  uint8_t       Block_MinorRev
  );

// The Host API functions are declared in xSIM-api.h
