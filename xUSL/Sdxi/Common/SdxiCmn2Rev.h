/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 * @file SdxiCmn2Rev.h
 * @brief Common-2-Rev core specific functions
 * @details  This files declares the structure for the common-2-Rev
 *  transfer table and the types of all functions contained within
 *  the SDXI Cmn2Rev transfer table.
 *
 */

#pragma once

#include <Nbio/Common/GnbDxio.h>

typedef void (*FINAL_NBIF_REG_CONFIG) (
  GNB_HANDLE            *GnbHandle,
  PCIe_PLATFORM_CONFIG  *Pcie
  );

// Internal Common-2-Rev Transfer Block for SDXI
typedef struct {
  FINAL_NBIF_REG_CONFIG               FinalNbifRegConfig;
} SDXI_COMMON_2_REV_XFER_BLOCK;
