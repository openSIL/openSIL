/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 *  @file MpioPortVisibility.c
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
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   GnbHandle           Pointer to GNB_HANDLE
 */
static
void
MpioEnablePorts (
  SIL_CONTEXT             *SilContext,
  GNB_HANDLE              *GnbHandle
  )
{
  PCIe_ENGINE_CONFIG            *Engine;
  MPIO_COMMON_2_REV_XFER_BLOCK  *MpioXferTable;
  NBIO_IP2IP_API                *NbioIp2Ip;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (SilGetCommon2RevXferTable(SilContext, SilId_MpioClass, (void **)(&MpioXferTable)) != SilPass) {
    return;
  }

  if (SilGetIp2IpApi(SilContext, SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }
  Engine = (PCIe_ENGINE_CONFIG *) (NbioIp2Ip->PcieConfigGetChild(DESCRIPTOR_ALL_ENGINES, &(GnbHandle->Header)));
  MpioXferTable->MpioEnablePort(Engine, GnbHandle);

  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * MpioVisibilityControl
 *
 * @brief Hide unused ports and keep active ports visible
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 */
void
MpioVisibilityControl (
  SIL_CONTEXT  *SilContext
  )
{
  GNB_HANDLE                    *GnbHandle;
  MPIO_COMMON_2_REV_XFER_BLOCK  *MpioXferTable;
  NBIO_IP2IP_API                *NbioIp2Ip;

  if (SilGetCommon2RevXferTable(SilContext, SilId_MpioClass, (void **)(&MpioXferTable)) != SilPass) {
    return;
  }

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  if (SilGetIp2IpApi(SilContext, SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }
  GnbHandle = NbioIp2Ip->GetGnbHandle(SilContext);
  while (GnbHandle != NULL) {
    MpioXferTable->MpioControlPorts(HidePorts, GnbHandle);
    MpioEnablePorts(SilContext, GnbHandle);
    GnbHandle = GnbGetNextHandle(GnbHandle);
  }
  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}
