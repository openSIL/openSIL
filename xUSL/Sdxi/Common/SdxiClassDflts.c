/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 * @file SdxiClassDflts.c
 * @brief This file holds the platform default values for the SDXI Input Block
 */

#include <Sdxi/SdxiClass-api.h>

const SDXICLASS_INPUT_BLK SdxiClassDflts = {
  /*
   * This is where you declare all input block vars/values you want to share with the Host.
   * This becomes part of the IP API for the Host.
   */
  .AmdFabricSdxi = CONFIG_SDXI_SMART_DATA_CACHE_INJECTION_ENABLE
};
