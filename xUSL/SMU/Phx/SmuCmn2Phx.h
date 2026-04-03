/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  SmuCmn2Phx.h
 * @brief OpenSIL SMU Phx transfer block functions
 *
 */
#pragma once

#include <xSIM.h>
#include <stdint.h>
#include <SMU/SmuDefs.h>
#include <Pci.h>

SMC_RESULT
SmuServiceRequestPhx (
  PCI_ADDR PciAddress,
  uint32_t RequestId,
  uint32_t *RequestArgument,
  uint32_t AccessFlags
  );

SIL_STATUS
SmuInitAfterCcxDonePhx (
  SIL_CONTEXT                   *SilContext
  );

SIL_STATUS
SmuInitAfterPcieTrainingDonePhx (
  SIL_CONTEXT                   *SilContext
  );
