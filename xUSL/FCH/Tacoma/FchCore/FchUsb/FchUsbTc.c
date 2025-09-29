/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchUsbTc.c
 * @brief Tacoma FCH XHCI Module functions
 *
 */

#include <SilCommon.h>
#include <FCH/FchClass-api.h>
#include <FCH/Common/FchCore/FchUsb/FchXhci.h>
#include <FCH/Common/Fch.h>
#include <IP/FchXhciIp.h>
#include "FchUsbTc.h"
#include "FchUsbCmn2Tc.h"
#include "FchXhciInitTc.h"
#include "FchCioInitTc.h"

void
FchInitPrePcieXhciTc (
  SIL_CONTEXT          *SilContext,
  FCHUSB_INPUT_BLK     *FchUsbData
  );

/*
 * InitializeFchUsbTcTp1
 *
 * @brief Config Usb controller during timepoint 1 (pre-pcie)
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 */
SIL_STATUS
InitializeFchUsbTcTp1 (
  SIL_CONTEXT  *SilContext
  )
{
  FCHUSB_INPUT_BLK  *LclInpUsbBlk; //pointer to Usb input blk

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  LclInpUsbBlk = (FCHUSB_INPUT_BLK *) xUslFindStructure(SilContext, SilId_FchUsb, 0);
  FCH_TRACEPOINT(SIL_TRACE_INFO, "SIL FCH Usb found blk at: 0x%x \n", LclInpUsbBlk);
  if (LclInpUsbBlk == NULL) {
    // Could not find the IP input block
    return SilNotFound;
  }

  FchInitPrePcieXhciTc(SilContext, LclInpUsbBlk);
  FchInitPrePcieCioTc(SilContext, LclInpUsbBlk);

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * FchUsbSetInputBlkTc
 *
 * @brief Establish FCH Usb input defaults for Tacoma
 *
 * This is an IP private function, not visible to the Host
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @retval SIL_STATUS
 */
SIL_STATUS FchUsbSetInputBlkTc (
  SIL_CONTEXT  *SilContext
  )
{
  SIL_STATUS Status;

  Status = FchUsbSetInputBlk(SilContext, &FchBiosSmcMsgIp);
  return Status;
}

/**
 * InitializeApiFchUsbTc
 *
 * @brief   Initialize internal APIs for Tacoma USB
 *
 * @returns SIL_STATUS
 * @retval  SilPass     APIs initialized successfully
 *
 */
SIL_STATUS
InitializeApiFchUsbTc (
  SIL_CONTEXT  *SilContext
  )
{
  SIL_STATUS  Status;

  Status = SilInitCommon2RevXferTable(SilContext, SilId_FchUsb, &mFchUsbXferTc);
  return Status;
}
