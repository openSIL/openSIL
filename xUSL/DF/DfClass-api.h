/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  DfClass-api.h
 * @brief openSIL-Host Data Fabric (DF) IP interface
 *
 * @details The Data Fabric API provides some controls for system wide data
 * handling.
 *
 *  This file provides the structures details for the Host to configure the
 *  Data Fabric operations.
 */
/**
 * @cond API_Doc
 *  @ingroup MODULES_IP
 *  @page IP_DF  Data Fabric (DF) api
 *   The Data Fabric is the portion of the SoC that routes data to the
 *   various IP blocks in the silicon. Most of the DF configuration has already
 *   been done in the ABL embedded firmware. Items for this module to do
 *   are system wide watchdog timers and memory scrubbers.
 *
 *   See the 'Files - @ref DfClass-api.h' section of this document for
 *   further details.
 * @endcond
 */
/*
 * Note: This file is the Host API definitions.
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

#define DFCLASS_MAJOR_REV   0
#define DFCLASS_MINOR_REV   1
#define DFCLASS_INSTANCE    0

/**
 * @cond FullCode_Doc
 * @ingroup DF_group
 * @endcond
 *
 * Definition for the Data FAbric (DF) module's Input Block
 *
 * Note: This struct contains the vars shared between the
 *    Host API and the openSIL DF IP module.
 */
typedef struct {
  uint32_t  AmdFabricWdtCfg;
  uint32_t  AmdFabricWdtCntSel;
  bool      AmdFabricImmSyncFloodOnFatalErrCtrl;
  bool      AmdFabricCcxAsNumaDomain;
  uint64_t  AmdPciExpressBaseAddress;
  uint32_t  AmdLongModePageTables;
  bool      AmdFabricMixedDimmConfigNumaDomain;   // PHX
} DFCLASS_INPUT_BLK;

/**
 * @brief Root Bridge location
 *
 */
typedef struct {
  uint32_t  Socket;                               // Socket number
  uint32_t  Die;                                  // Die number
  uint32_t  Index;                                // Root bridge number
} SIL_ROOT_BRIDGE_LOCATION;
