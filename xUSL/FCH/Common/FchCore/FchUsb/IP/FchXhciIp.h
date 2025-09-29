/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchXhciIp.h
 * @brief FCH xHCI functions
 *
 */

#include <SilCommon.h>
#include <FchUsb-api.h>

extern const FCH_BIOSSMC_MSG_INPUT_BLK FchBiosSmcMsgIp;

bool
FchXhciCheckUsbControllerSkip (
  uint32_t DieBusNum
  );

bool
FchXhciCheckUsbPhySkip (
  uint32_t DieBusNum
  );

void
FchXhciIohcPmeDisable (
  uint32_t DieBusNum,
  bool PMEDis
  );
