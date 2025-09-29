/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file RcMgrIp2IpPhx.h
 * @brief Prototypes for PHX RCMGR core specific transfer table functions
 *
 */

#pragma once
#include <RcMgr/RcMgrIp2Ip.h>

SIL_STATUS FabricReserveMmio (
  SIL_CONTEXT           *SilContext,
  uint64_t              *BaseAddress,
  uint64_t              *Length,
  uint64_t              Alignment,
  FABRIC_TARGET         Target,
  FABRIC_MMIO_ATTRIBUTE *Attributes
  );
