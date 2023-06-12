/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchIsa.c
 * @brief FCH ISA sub-controller functions
 *
 *
 */

#include <SilCommon.h>
#include <xSIM.h>
#include <string.h>
#include <FchClass-api.h>
#include <FCH/Common/Fch.h>
#include "FchSpi.h"
#include "FchIsa.h"

static const FCHISA_INPUT_BLK FchIsaDefaults = {
  .LpcConfig = {
    .LpcEnable = CONFIG_LPC_BUS_ENABLE,
    .LpcClk0 = true,
    .LpcClk1 = true
  },
  .SpiConfig = {
    .SpiSpeed = CONFIG_SPI_STD_SPEED,
    .WriteSpeed = CONFIG_SPI_WRT_SPEED,
    .SpiTpmSpeed = CONFIG_SPI_TPM_SPEED,
  }
};

/**
 * FchIsaSetInputBlk
 * @brief Establish FCH ISA input defaults
 *
 * This is an IP private function, not visible to the Host
 *
 * @retval SIL_STATUS
 */
SIL_STATUS FchIsaSetInputBlk (void)
{
  FCHISA_INPUT_BLK *FchIsaInput;

  FchIsaInput = (FCHISA_INPUT_BLK *) SilCreateInfoBlock(SilId_FchIsa,
    sizeof (FCHISA_INPUT_BLK),
    SIL_RESERVED_0283,
    FCHISA_MAJOR_REV,
    FCHISA_MINOR_REV
    );

  if (FchIsaInput == NULL) {
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "SIL Fch ISA input block was not found.\n");
    return SilAborted;
  }
  FCH_TRACEPOINT(SIL_TRACE_INFO, "SIL FCH ISA input block is at: 0x%x \n", FchIsaInput);

  // Fill Fch ISA structure with defaults
  memcpy((void *)FchIsaInput, &FchIsaDefaults, sizeof (FCHISA_INPUT_BLK));

  return SilPass;
}


/**
 * InitializeFchIsaTp1
 *
 * @brief Config ISA controller during timepoint 1 (Pre-Pcie phase)
 *
 */
SIL_STATUS
InitializeFchIsaTp1 (void)
{
  FCHCLASS_INPUT_BLK *LclInpFchBlk; //pointer to Fch input blk
  FCHISA_INPUT_BLK   *LclInpIsaBlk; //pointer to input blk

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  LclInpFchBlk = (FCHCLASS_INPUT_BLK *) xUslFindStructure(SilId_FchClass, 0);
  if (LclInpFchBlk == NULL) {
    // Could not find the IP input block
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "SIL FCH Class was not found\n");
    return SilNotFound;
  }
  FCH_TRACEPOINT(SIL_TRACE_INFO, "SIL FCH Class found blk at: 0x%x \n", LclInpFchBlk);

  LclInpIsaBlk = (FCHISA_INPUT_BLK *) xUslFindStructure(SilId_FchIsa, 0);
  if (LclInpIsaBlk == NULL) {
    // Could not find the IP input block
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "SIL FCH ISA was not found.\n");
    return SilNotFound;
  }
  FCH_TRACEPOINT(SIL_TRACE_INFO, "SIL FCH ISA found blk at: 0x%x \n", LclInpIsaBlk);

  FchIsaInitSpi(LclInpFchBlk, &(LclInpIsaBlk->SpiConfig));

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeFchIsaTp2
 *
 * @brief Config ISA controller during timepoint 2 (Post-Pcie phase)
 *
 */
SIL_STATUS
InitializeFchIsaTp2 (void)
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeFchIsaTp3
 *
 * @brief Config ISA controller during timepoint 3 (Pre-OS)
 *
 */
SIL_STATUS
InitializeFchIsaTp3 (void)
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}
