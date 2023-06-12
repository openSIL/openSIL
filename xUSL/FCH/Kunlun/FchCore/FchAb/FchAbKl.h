/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file FchAbKl.h
 * @brief Fch Ab Kunlun specific headers and function declarations for use in
 * the IP block list files (e.g.  \xSIM\SoC\F1AM00\IpBlkListF1AM00Tp1.c)
 */

#pragma once

#include <SilCommon.h>
#include <FchAb-api.h>

// Define the amount of memory this IP block will need from the Host
// This value is used in the IP Block entry for this IP
#define FCHAB_INPBLK_SIZE_KL sizeof (FCHAB_INPUT_BLK)

SIL_STATUS InitializeFchAbKlTp1 (void);
SIL_STATUS InitializeFchAbKlTp2 (void);
SIL_STATUS InitializeFchAbKlTp3 (void);
SIL_STATUS FchAbSetInputBlkKl (void);
SIL_STATUS InitializeApiFchAbKl (void);
