/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT
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
 *    Include into this file ONLY those items the Host needs to access
 *    and work with your data structure(s)
 */

#pragma once
#pragma pack(push,1)

/**
 * @cond FullCode_Doc
 * @ingroup DF_group
 * @endcond
 *
 * Definition for the Data FAbric (DF) module's Input Block
 *
 * Note: This struct contains the vars shared between the
 *    Host API and the openSIL DF IP module.
 *    Include into this file ONLY those items the Host needs to access
 *    and work with your IP.
 *
 */
typedef struct {
      uint32_t  AmdFabricWdtCfg;
      uint32_t  AmdFabricWdtCntSel;
      const uint32_t  *DfGlblCtrlInstanceIds;
      bool      AmdFabricImmSyncFloodOnFatalErrCtrl;
      uint32_t  AmdFabricNumOfScrubber;
      uint32_t  AmdFabricNumSeqScrubsCstateExit;
      uint32_t  AmdFabricEccScrubRedirection;
      uint32_t  FabricEccScrubPoisoned;
      uint32_t  AmdFabricDramScrubTime;
      uint32_t  AmdFabricEccScrubRedirectionLimit;
      bool      AmdFabricPeriodicDirRinse;

} DFCLASS_INPUT_BLK;

#pragma pack (pop)

