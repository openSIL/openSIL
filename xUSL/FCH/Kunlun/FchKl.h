/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchKl.h
 * @brief OpenSIL FCH Kunlun FCH Class entry point declarations to be used in IpBlkListxxyy.h files
 *
 */

#pragma once

#include <Sil-api.h>
#include <FchClass-api.h>

// Define the amount of memory this IP block will need from the Host
// This value is used in the IP Block entry for this IP
#define FCHCLASS_INPBLK_SIZE_KL sizeof (FCHCLASS_INPUT_BLK)

SIL_STATUS InitializeFchApiKl (void);
SIL_STATUS FchClassSetInputBlkKl (void);
