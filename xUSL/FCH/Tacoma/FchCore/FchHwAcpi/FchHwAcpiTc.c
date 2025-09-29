/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchHwAcpiTc.c
 * @brief TC FCH HwAcpi Module functions
 *
 */

#include <SilCommon.h>
#include <xSIM.h>
#include <Fch.h>
#include <FchHwAcpi-api.h>
#include <FchHwAcpi.h>
#include <IP/FchHwAcpiIp.h>
#include "FchHwAcpiTc.h"
#include "FchHwAcpiInitTc.h"
#include "FchHwAcpiCmn2Tc.h"

/**
 * InitializeFchHwAcpiPreliminaryTcTp1
 *
 * @brief Called before InitializeFchHwAcpiTcTp1
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 */
SIL_STATUS
InitializeFchHwAcpiPreliminaryTcTp1 (
  SIL_CONTEXT  *SilContext
  )
{
  FCHCLASS_INPUT_BLK *LclInpFchBlk;     //pointer to Fch input blk
  FCHHWACPI_INPUT_BLK *LclInpHwAcpiBlk; //pointer to HwAcpi  input blk

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  LclInpHwAcpiBlk = (FCHHWACPI_INPUT_BLK *) xUslFindStructure(SilContext, SilId_FchHwAcpi, 0);
  if (LclInpHwAcpiBlk == NULL) {
    return SilNotFound;
  }
  FCH_TRACEPOINT(SIL_TRACE_INFO, "SIL FCH HwAcpi found blk at: 0x%x \n", LclInpHwAcpiBlk);

  LclInpFchBlk = (FCHCLASS_INPUT_BLK *) xUslFindStructure(SilContext, SilId_FchClass, 0);
  if (LclInpFchBlk == NULL) {
    return SilNotFound;
  }
  FCH_TRACEPOINT(SIL_TRACE_INFO, "SIL FCH Class found blk at: 0x%x \n", LclInpFchBlk);

  FchHwAcpiPreliminaryPrePcieInit(SilContext, LclInpFchBlk, LclInpHwAcpiBlk);
  FchHwAcpiPreliminaryPrePcieInitTc ();

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeFchHwAcpiTcTp1
 *
 * @brief Configures TC FCH ACPI during timepoint 1 (Pre-Pcie)
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 */
SIL_STATUS
InitializeFchHwAcpiTcTp1 (
  SIL_CONTEXT  *SilContext
  )
{
  FCHCLASS_INPUT_BLK *LclInpFchBlk;     //pointer to Fch input blk
  FCHHWACPI_INPUT_BLK *LclInpHwAcpiBlk; //pointer to HwAcpi  input blk

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  LclInpHwAcpiBlk = (FCHHWACPI_INPUT_BLK *) xUslFindStructure(SilContext, SilId_FchHwAcpi, 0);
  FCH_TRACEPOINT(SIL_TRACE_INFO, "SIL FCH HwAcpi found blk at: 0x%x \n", LclInpHwAcpiBlk);
  if (LclInpHwAcpiBlk == NULL) {
    // Could not find the IP input block
    return SilNotFound;
  }

  LclInpFchBlk = (FCHCLASS_INPUT_BLK *) xUslFindStructure(SilContext, SilId_FchClass, 0);
  FCH_TRACEPOINT(SIL_TRACE_INFO, "SIL FCH found blk at: 0x%x \n", LclInpFchBlk);
  if (LclInpFchBlk == NULL) {
    // Could not find the IP input block
    return SilNotFound;
  }

  FchHwAcpiPrePcieInit(SilContext, LclInpFchBlk, LclInpHwAcpiBlk);
  FchHwAcpiPrePcieInitIp(LclInpFchBlk, LclInpHwAcpiBlk);

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeFchHwAcpiTcTp3
 *
 * @brief Configures TC FCH ACPI during timepoint 1 (Pre-OS)
 *
 */
SIL_STATUS
InitializeFchHwAcpiTcTp3 (
  void
  )
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * FchHwAcpiPreliminarySetInputBlkTc
 * @brief Sets FCH ACPI Default Input Data
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @retval SIL_STATUS
 */
SIL_STATUS
FchHwAcpiSetInputBlkTc (
  SIL_CONTEXT  *SilContext
  )
{
  return FchHwAcpiSetInputBlk(SilContext);
}

/**
 * InitializeApiFchHwAcpiTc
 *
 * @brief   Initialize FCH TC ACPI Cmn2Rev and IP2IP APIs
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @returns SIL_STATUS
 * @retval  SilPass     APIs initialized successfully
 *
 */
SIL_STATUS
InitializeApiFchHwAcpiTc (
  SIL_CONTEXT  *SilContext
  )
{
  // Initialize Common to Rev specific transfer table first
  return SilInitCommon2RevXferTable(SilContext, SilId_FchHwAcpi, (void *)&mFchHwAcpiXferTc);
}
