/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file CxlIp2IpBrh.h
 * @brief Prototypes for CXL Brh specific I2I API table functions
 *
 */

#pragma once

#include <Mpio/Common/MpioInitLib.h>

void
RemoveCxlLinksFromTopologyBrh (
  MPIO_COMPLEX_DESCRIPTOR **CurrentComplexConfig,
  PCIe_PLATFORM_CONFIG    *Pcie
  );
