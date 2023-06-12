/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file FchUsbCmn2Rev.h
 * This file defines the types of all functions contained within FCH USB Cmn2Rev
 * transfer table as well as a definition of the transfer table itself.
 *
 */


#pragma once

#include <SilCommon.h>
#include <FCH/FchUsb-api.h>


/* First define the prototypes for each published function */
/**
 * FCH_INIT_PREPCIE_XHCI
 *
 * @brief   Internal API type definition.
 *
 * @details Config FCH XHCI Module before PCI enumeration
 *
 */
typedef void (*FCH_INIT_PREPCIE_XHCI) (
  FCHUSB_INPUT_BLK *FchUsbData
  );

/**
 * FCH_INIT_POSTPCIE_XHCI
 *
 * @brief   Internal API type definition.
 *
 * @details Config FCH XHCI Module after PCI enumeration
 *
 */
typedef void (*FCH_INIT_POSTPCIE_XHCI) (
  FCHUSB_INPUT_BLK *FchUsbData
  );

/**
 * FCH_INIT_PREOS_XHCI
 *
 * @brief   Internal API type definition.
 *
 * @details Config FCH XHCI Module before OS booting
 *
 */
typedef void (*FCH_INIT_PREOS_XHCI) (
  FCHUSB_INPUT_BLK *FchUsbData
  );

typedef struct {
  uint8_t   Version;
} FCH_XHCI_INT_API_HEADER;

/**
 * Common to Revision specific transfer table
 *
 * This structure contains an API for internal IP specific functions that need
 * to run from IP common code.
 *
 * IP specific code should populate this structure of function pointers with
 * the appropriate ip version specific function.
 *
 */
typedef struct {
  FCH_XHCI_INT_API_HEADER            Header;
  FCH_INIT_PREPCIE_XHCI              FchInitPrePcieXhci;
  FCH_INIT_POSTPCIE_XHCI             FchInitPostPcieXhci;
  FCH_INIT_PREOS_XHCI                FchInitPreOsXhci;
} FCH_XHCI_XFER_TABLE;
