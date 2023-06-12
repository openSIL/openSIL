/**
 * @file SdciCmn2Rev.h
 * @brief Common-2-Rev core specific functions
 * @details  This files declares the structure for the common-2-Rev
 *  transfer table and the types of all functions contained within
 *  the SDCI Cmn2Rev transfer table.
 *
 */
/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <NBIO/GnbDxio.h>

typedef void (*SDCI_FINAL_REG_INIT) (
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  );

typedef void (*SDCI_PCIE_ROOT_PORT_CONFIG_WRAPPER) (
  PCIe_WRAPPER_CONFIG               *Wrapper,
  void                              *Buffer,
  GNB_HANDLE                        *GnbHandle
  );

typedef void (*INIT_MPDMA_ENGINE) (
  PCIe_PLATFORM_CONFIG       *Pcie
  );

// Internal Common-2-Rev Transfer Block for SDCI

typedef struct {
  SDCI_FINAL_REG_INIT                 SdciFinalRegisterInit;
  SDCI_PCIE_ROOT_PORT_CONFIG_WRAPPER  CmdaConfigPcieRootPortWrapper;
  INIT_MPDMA_ENGINE                   InitMpdmaEngine;
} SDCI_COMMON_2_REV_XFER_BLOCK;
