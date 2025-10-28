/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  SmuBrh.c
 * @brief OpenSIL SMU BRH specific initialization
 *
 */

#pragma once

#include <SMU/SmuClass-api.h>

SIL_STATUS
InitializeSmuApiBrh (void);
SIL_STATUS
SmuSetInputBlkBrh (void);
SIL_STATUS
InitializeSmuTp1Brh (void);
SIL_STATUS
InitializeSmuTp2Brh (void);
SIL_STATUS
InitializeSmuTp3Brh (void);
