/**
 * @file CcxMicrocodePatch.h
 * @brief OpenSIL CCX IP microcode patch specific function declarations.
 *
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once
#include <Utils.h>

/**********************************************************************************************************************
 * Module data structure
 *
 */
/// Microcode Patch Equivalent Processor Id
typedef struct {
  uint16_t   ProcessorRevisionID;        ///< Equivalent ID
  uint8_t    Chipset1RevisionID;         ///< Revision level of 1st Chipset device to match
  uint8_t    Chipset2RevisionID;         ///< Revision level of 2nd Chipset device to match
} MPB_REVISION;

/// MPB Load Control
typedef struct {
  uint8_t    SerializedLoad:1;
  uint8_t    LoadOnBothThreads:1;
  uint8_t    Reserved:6;
} LOAD_CONTROL;

/// Reserved Data
typedef struct {
		  uint8_t         Reserved[5512];
} RESERVED_DATA;

/// Microcode Patch Block
typedef struct {
		  uint32_t        MPB_DATE;
		  uint32_t        PatchLevel;
		  uint16_t        MPB_LOADER_ID;
		  uint32_t        Reserved1;
		  uint16_t        Reserved2;
		  uint32_t        MPB_NB_ID;
		  uint32_t        MPB_SB_ID;
		  MPB_REVISION    MPB_REVISION;
		  uint8_t         MPB_BIOS_REVISION;
		  LOAD_CONTROL    LoadControl;
		  uint16_t        Reserved3;
		  RESERVED_DATA   ReservedData;
} MPB; // Microcode Patch Block

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
bool LoadMicrocodePatch (uint64_t *UcodePatchAddr);
