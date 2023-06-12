/**
 * @file SdciClassDflts.c
 * @brief This file holds the platform default values for the SDCI Input Block
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <Sdci/SdciClass-api.h>

const SDCICLASS_INPUT_BLK SdciClassDflts = {
    /*
	 * This is where you declare all input block vars/values you want to share with the Host.
     * This becomes part of the IP API for the Host.
	 */
    .AmdFabricSdci = CONFIG_SDCI_SMART_DATA_CACHE_INJECTION_ENABLE
};
