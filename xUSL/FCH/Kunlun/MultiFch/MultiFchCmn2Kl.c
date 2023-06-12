/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MultiFchCmn2Kl.c
 * @brief Kunlun FCH multi-die Common to Kunlun Transfer functions
 *
 */

#include <SilCommon.h>
#include <FCH/Common/MultiFch/MultiFchCmn2Rev.h>
#include "MultiFchCmn2Kl.h"

SIL_RESERVED_STRUCT_0026 mMultiFchXferKl = {
  .field0 = {
    .field0 = 2
  },
  .field1 = FchMultiFchGetFchRbIndexOnDieKl,
  .field2 = FchMultiFchInitAbKl,
  .field3 = FchMultiFchInitUsbKl,
  .field4 = FchMultiFchInitSdKl,
};
