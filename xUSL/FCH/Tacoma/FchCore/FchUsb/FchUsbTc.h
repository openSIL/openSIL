/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchUsbTc.h
 * @brief Tacoma FCH XHCI controller IP block list API header
 */

#pragma once

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
  );

/*
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
  );

SIL_STATUS
InitializeApiFchUsbTc (
  SIL_CONTEXT  *SilContext
  );


#define FCHUSB_INPBLK_SIZE_TC sizeof (FCHUSB_INPUT_BLK)
