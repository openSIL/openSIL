/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  SmuCmn2Rs.c
 * @brief OpenSIL SMU RS transfer block functions
 *
 */

#pragma once

#include <xSIM.h>
#include <stdint.h>
#include <SMU/SmuDefs.h>
#include <Pci.h>

SMC_RESULT
SmuServiceRequestBrh (
  PCI_ADDR PciAddress,
  uint32_t RequestId,
  uint32_t *RequestArgument,
  uint32_t AccessFlags
  );
