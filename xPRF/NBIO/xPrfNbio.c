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
 * @param   SilContext  A context structure through which host firmware defined data
 *                      can be passed to openSIL. The host firmware is responsible for initializing
 *                      the SIL_CONTEXT structure.
 * @param   *InfoBlockDataSize   - Output block size
 *
 * @return  void* The address from openSIL Instance.
 *
 */
void *
xPrfGetNbiotopologyStructure (
  SIL_CONTEXT   *SilContext,
  uint32_t      *InfoBlockDataSize
  )
{
  *InfoBlockDataSize = xUslFindStructureSize(SilContext, SilId_NbioClass, NBIOPCIECLASS_INSTANCE);
  return (void *)xUslFindStructure(SilContext, SilId_NbioClass, NBIOPCIECLASS_INSTANCE);
}

/**
 * xPrfGetGnbHandle
 *
 * @brief   Get base address to NBIO IOHC config data
 *
 * @details This data returned by this function is only valid after openSIL time point 1.
 *          Prior to this time point execution, the data is not available.
 *
 * @param   SilContext  A context structure through which host firmware defined data
 *                      can be passed to openSIL. The host firmware is responsible for initializing
 *                      the SIL_CONTEXT structure.
 * @param   GnbHandle   On output, a pointer to the NBIO IOHC config data.
 *
 * @returns SIL_STATUS
 *
 * @retval  SilPass         A pointer to the IOHC config data is returned in GnbHandle
 * @retval  SilNotFound     NBIO API was not found in the API list
 * @retval  SilDeviceError  The GnbHandle is not ready
 *
 */
SIL_STATUS
xPrfGetGnbHandle (
  SIL_CONTEXT     *SilContext,
  SIL_GNB_HANDLE  **GnbHandle
  )
{
  NBIO_IP2IP_API                *NbioIp2Ip;

  if (SilGetIp2IpApi(SilContext, SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    return SilNotFound;
  }

  *GnbHandle = (SIL_GNB_HANDLE *)NbioIp2Ip->GetGnbHandle(SilContext);
  if (*GnbHandle == NULL) {
    return SilDeviceError;
  }

  return SilPass;
}
