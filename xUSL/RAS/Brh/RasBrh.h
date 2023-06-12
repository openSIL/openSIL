/* SPDX-License-Identifier: MIT */
/**
 * Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */
/**
 * @file
 * @brief OpenSIL RAS IP Brh specific initialization function declaration.
 *
 */

#pragma once

#include <Sil-api.h>
#include <RAS/Brh/RasInitBrh.h>

#define RAS_DATA_SIZE_BRH  sizeof (RAS_DATA_BLOCK_BRH)

/*******************************************************************************
 * variable declaration
 *
 */

/*******************************************************************************
 * Declare Function prototypes
 *
 */
SIL_STATUS
RasSetDataBlkBrh (void);

SIL_STATUS
InitializeApiRasBrh (void);

SIL_STATUS
InitializeRasTp1Brh (void);
