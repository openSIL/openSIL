/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  xPrfNbio.c
 * @brief Platform Reference Firmware - exposes Platform specific features for
 *        Nbio
 */

#include "xPRF-api.h"
#include "xSIM.h"
#include <Nbio/NbioIp2Ip.h>

/**
 * xPrfGetNbiotopologyStructure
 *
 * @brief   Returns the base address of Pcie Topology Structure and its size
 *
 * @param   *InfoBlockDataSize   - Output block size
 *
 * @return  void* The address from openSIL Instance.
 *
 */
void *
xPrfGetNbiotopologyStructure (
  uint32_t  *InfoBlockDataSize
  )
{
  *InfoBlockDataSize = xUslFindStructureSize(SilId_NbioClass, NBIOPCIECLASS_INSTANCE);
  return (void *)xUslFindStructure(SilId_NbioClass, NBIOPCIECLASS_INSTANCE);
}
