/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchXhciKl.c
 * @brief Kunlun FCH XHCI Module functions
 *
 */

#include <SilCommon.h>
#include <FCH/FchClass-api.h>
#include <FCH/FchUsbIp2Ip.h>
#include <FCH/Common/FchCore/FchXhci/FchUsbCmn2Rev.h>
#include <FCH/Common/FchCore/FchXhci/FchXhci.h>
#include <FCH/Common/Fch.h>
#include "FchXhciKl.h"

extern FCH_XHCI_XFER_TABLE mFchXhciXferKl;
extern FCHUSB_IP2IP_API mFchUsbApiKl;

/**
 * InitializeFchUsbKlTp1
 *
 * @brief Config Usb controller during timepoint 1 (pre-pcie)
 *
 */
SIL_STATUS
InitializeFchUsbKlTp1 (void)
{
  FCHUSB_INPUT_BLK  *LclInpUsbBlk; //pointer to Usb input blk

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  LclInpUsbBlk = (FCHUSB_INPUT_BLK *) xUslFindStructure(SilId_FchUsb, 0);
  FCH_TRACEPOINT(SIL_TRACE_INFO, "SIL FCH Usb found blk at: 0x%x \n", LclInpUsbBlk);
  if (LclInpUsbBlk == NULL) {
    // Could not find the IP input block
    return SilNotFound;
  }

  FchXhciPrePcieInit(LclInpUsbBlk);

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeFchUsbKlTp2
 *
 * @brief Config Usb controller during timepoint 2
 *
 */
SIL_STATUS
InitializeFchUsbKlTp2 (void)
{
  return SilPass;
}

/**
 * InitializeFchUsbKlTp3
 *
 * @brief Config Usb controller during timepoint 3
 *
 */
SIL_STATUS
InitializeFchUsbKlTp3 (void)
{
  return SilPass;
}

/**
 * FchUsbSetInputBlkKl
 *
 * @brief Establish FCH Usb input defaults
 *
 * This is an IP private function, not visible to the Host
 *
 * @retval SIL_STATUS
 */
SIL_STATUS FchUsbSetInputBlkKl (void)
{
  SIL_STATUS Status;

  Status = FchUsbSetInputBlk ();
  return Status;
}

/**
 * InitializeApiFchUsbKl
 *
 * @brief   Initialize internal APIs for Kunlun Xhci
 *
 * @returns SIL_STATUS
 * @retval  SilPass     APIs initialized successfully
 *
 */
SIL_STATUS
InitializeApiFchUsbKl (void)
{
  SIL_STATUS  Status;

  // Initialize Common to Rev specific transfer table first
  Status = SilInitCommon2RevXferTable(SilId_FchUsb, &mFchXhciXferKl);
  if (Status != SilPass) {
    return Status;
  }

  // Initialize FCH USB IP-to-IP API
  return SilInitIp2IpApi(SilId_FchUsb, (void *)&mFchUsbApiKl);
}
