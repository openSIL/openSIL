/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 *  @file MpioPortVisiblity.c
 *  @brief Set visibility of ports based on status
 */


#include <xSIM.h>
#include <Nbio/NbioIp2Ip.h>
#include <CommonLib/SmnAccess.h>
#include "MpioInitLib.h"
#include "MpioPcie.h"
#include "MpioCmn2Rev.h"

/*----------------------------------------------------------------------------------------*/
/**
 * MpioEnablePorts
 *
 * @brief Enable visibility of links that trained or support hotplug
 *
 * @param[in]  GnbHandle     Pointer to GNB_HANDLE
 */
static
void
MpioEnablePorts (
  GNB_HANDLE              *GnbHandle
  )
{
  PCIe_ENGINE_CONFIG            *Engine;
  MPIO_COMMON_2_REV_XFER_BLOCK  *MpioXferTable;
  NBIO_IP2IP_API                *NbioIp2Ip;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  if (SilGetCommon2RevXferTable(SilId_MpioClass, (void **)(&MpioXferTable)) != SilPass) {
    return;
  }

  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }
  Engine = (PCIe_ENGINE_CONFIG *) (NbioIp2Ip->PcieConfigGetChild(DESCRIPTOR_ALL_ENGINES, &(GnbHandle->Header)));
  MpioXferTable->MpioEnablePort(Engine, GnbHandle);

  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * MpioVisibilityControl
 *
 * @brief Hide unused ports and keep active ports visible
 *
 */
void
MpioVisibilityControl (void)
{
  GNB_HANDLE                    *GnbHandle;
  MPIO_COMMON_2_REV_XFER_BLOCK  *MpioXferTable;
  NBIO_IP2IP_API                *NbioIp2Ip;

  if (SilGetCommon2RevXferTable(SilId_MpioClass, (void **)(&MpioXferTable)) != SilPass) {
    return;
  }

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }
  GnbHandle = NbioIp2Ip->GetGnbHandle ();
  while (GnbHandle != NULL) {
    MpioXferTable->MpioControlPorts(HidePorts, GnbHandle);
    MpioEnablePorts(GnbHandle);
    GnbHandle = GnbGetNextHandle(GnbHandle);
  }
  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}
