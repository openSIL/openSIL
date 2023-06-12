/**
 * @file  xPrfNbio.c
 * @brief Platform Reference Firmware - exposes Platform specific features for
 *        Nbio
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include "xPrfNbio.h"
#include "xPRF-api.h"
#include "xSIM.h"

/**
 * xPrfGetNbiotopologyStructure
 *
 * @brief   Returns the base address of Pcie Topology Structure
 *
 */
void* xPrfGetNbiotopologyStructure(void)
{
  return ((void *)SilFindStructure(SilId_NorthBridgePcie, 0));
}
