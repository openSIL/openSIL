/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file CcxMicrocodePatch.h
 * @brief OpenSIL CCX IP microcode patch specific function declarations.
 *
 */

#pragma once
#include <Utils.h>

/**********************************************************************************************************************
 * Module data structure
 *
 */
/// Patch Loader Register
typedef struct {
  uint64_t PatchBase:32;                ///< Linear address of patch header address block
  uint64_t Sbz:32;                      ///< Should be zero
} PATCH_LOADER_MSR;

/// Patch Loader
typedef union {
  uint64_t           RawData;
  PATCH_LOADER_MSR BitFields;
} PATCH_LOADER;

/**********************************************************************************************************************
 * Function prototypes
 *
 */
bool LoadMicrocodePatch (
  SIL_CONTEXT   *SilContext,
  uint64_t      *UcodePatchAddr
  );
