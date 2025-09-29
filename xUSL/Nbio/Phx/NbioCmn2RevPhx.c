/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  NbioCmn2RevPhx.c
 * @brief PHX NBIO Client specific initialization
 *
 */

#include <SilCommon.h>
#include <Nbio/Common/GnbDxio.h>
#include <Nbio/Common/NbioCmn2Rev.h>
#include "NbioCmn2RevPhx.h"


NBIO_COMMON_2_REV_XFER_BLOCK NbioXferPhx = {
  .NbioIoApicMmioAddress = NbioIoApicMmioAddressPhx,
  .NbioIoApicPreDefId = NbioIoApicPreDefIdPhx,
  .NbioGetPcieComplexSize = NbioGetPcieComplexSizePhx,
  .NbioGetPcieComplexPointer = NbioGetPcieComplexPointerPhx,
  .NbioEnumerateHarvestWrappers = NbioEnumerateHarvestWrappersPhx,
  .NbioPcieFixupPlatformConfig = NbioPcieFixupPlatformConfigPhx,
  .NbioPcieFixupComplexDefaults = NbioPcieFixupComplexDefaultsPhx
};
