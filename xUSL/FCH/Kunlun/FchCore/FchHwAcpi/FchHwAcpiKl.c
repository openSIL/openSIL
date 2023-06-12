/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchHwAcpiKl.c
 * @brief Kunlun FCH ACPI Module functions
 *
 */

#include <SilCommon.h>
#include <xSIM.h>
#include <Fch.h>
#include <FchHwAcpi-api.h>
#include <FchHwAcpi.h>
#include "FchHwAcpiKl.h"
#include "FchHwAcpiCmn2Kl.h"

/**
 * InitializeFchHwAcpiPreliminaryKlTp1
 *
 * @brief Called before InitializeFchHwAcpiKlTp1
 *
 */
SIL_STATUS
InitializeFchHwAcpiPreliminaryKlTp1 (
  void
  )
{
  FCHCLASS_INPUT_BLK *LclInpFchBlk;     //pointer to Fch input blk
  FCHHWACPI_INPUT_BLK *LclInpHwAcpiBlk; //pointer to HwAcpi  input blk

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  LclInpHwAcpiBlk = (FCHHWACPI_INPUT_BLK *) xUslFindStructure(SilId_FchHwAcpiP, 0);
  if (LclInpHwAcpiBlk == NULL) {
    // Could not find the IP input block
    return SilNotFound;
  }
  FCH_TRACEPOINT(SIL_TRACE_INFO, "SIL FCH HwAcpi found blk at: 0x%x \n", LclInpHwAcpiBlk);

  LclInpFchBlk = (FCHCLASS_INPUT_BLK *) xUslFindStructure(SilId_FchClass, 0);
  if (LclInpFchBlk == NULL) {
    // Could not find the IP input block
    return SilNotFound;
  }
  FCH_TRACEPOINT(SIL_TRACE_INFO, "SIL FCH found blk at: 0x%x \n", LclInpFchBlk);

  FchHwAcpiPreliminaryPrePcieInit(LclInpFchBlk, LclInpHwAcpiBlk);

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeFchHwAcpiKlTp1
 *
 * @brief Configures Kunlun FCH ACPI during timepoint 1 (Pre-Pcie)
 *
 */
SIL_STATUS
InitializeFchHwAcpiKlTp1 (
  void
  )
{
  FCHCLASS_INPUT_BLK  *LclInpFchBlk;
  FCHHWACPI_INPUT_BLK *LclInpHwAcpiBlk;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  LclInpHwAcpiBlk = (FCHHWACPI_INPUT_BLK *) xUslFindStructure(SilId_FchHwAcpiP, 0);
  if (LclInpHwAcpiBlk == NULL) {
    return SilNotFound;
  }
  FCH_TRACEPOINT(SIL_TRACE_INFO, "SIL FCH HwAcpi found blk at: 0x%x \n", LclInpHwAcpiBlk);

  LclInpFchBlk = (FCHCLASS_INPUT_BLK *) xUslFindStructure(SilId_FchClass, 0);
  if (LclInpFchBlk == NULL) {
    return SilNotFound;
  }
  FCH_TRACEPOINT(SIL_TRACE_INFO, "SIL FCH Class found blk at: 0x%x \n", LclInpFchBlk);

  FchHwAcpiPrePcieInit(LclInpFchBlk, LclInpHwAcpiBlk);

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeFchHwAcpiPreliminaryKlTp2
 *
 * @brief Called before InitializeFchHwAcpiKlTp2
 *
 */
SIL_STATUS
InitializeFchHwAcpiPreliminaryKlTp2 (
  void
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeFchHwAcpiKlTp2
 *
 * @brief Configures Kunlun FCH ACPI during timepoint 2 (Post-Pcie)
 *
 */
SIL_STATUS
InitializeFchHwAcpiKlTp2 (
  void
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeFchHwAcpiPreliminaryKlTp3
 *
 * @brief Called before InitializeFchHwAcpiKlTp3
 *
 */
SIL_STATUS
InitializeFchHwAcpiPreliminaryKlTp3 (
  void
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeFchHwAcpiKlTp3
 *
 * @brief Configures Kunlun FCH ACPI during timepoint 3 (Pre-OS)
 *
 */
SIL_STATUS
InitializeFchHwAcpiKlTp3 (
  void
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * FchHwAcpiPreliminarySetInputBlkKl
 * @brief Sets FCH ACPI Default Input Data
 *
 * @retval SIL_STATUS
 */
SIL_STATUS
FchHwAcpiPreliminarySetInputBlkKl (
  void
  )
{
  return FchHwAcpiPreliminarySetInputBlk ();
}

/**
 * InitializeApiFchHwAcpiKl
 *
 * @brief   Initialize FCH Kunlun ACPI Cmn2Rev and IP2IP APIs
 *
 * @returns SIL_STATUS
 * @retval  SilPass     APIs initialized successfully
 *
 */
SIL_STATUS
InitializeApiFchHwAcpiKl (
  void
  )
{
  // Initialize Common to Rev specific transfer table first
  return SilInitCommon2RevXferTable(SilId_FchHwAcpiP, (void *)&mFchHwAcpiXferKl);
}
