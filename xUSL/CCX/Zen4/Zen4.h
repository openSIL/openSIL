/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT
/**
 * @file Zen4.h
 * @brief Zen4 core specific headers and function declarations for use in
 *   the IP block list files (e.g.  \xSIM\SoC\F19M10\IpBlkListF19M10Tp1.c)

 */

#pragma once

#include <CcxClass-api.h>
#include <CcxZen4-api.h>

// Define the amount of memory this IP block will need from the Host
// This value is used in the IP Block entry for this IP
#define CCX_DATA_SIZE_ZEN4 (sizeof(CCXCLASS_DATA_BLK)  + \
                            sizeof(CCX_DATA_ZEN4) )

/***************************************************************************
 * Declare Function prototypes
 *  These are the functions referenced in the IP block lists
 *  for initializing this IP
 */
extern SIL_STATUS CcxClassSetInputBlk (void); // This is a common function

SIL_STATUS InitializeCcxZen4Tp1 (void);
SIL_STATUS InitializeCcxZen4Tp2 (void);
SIL_STATUS InitializeCcxZen4Tp3 (void);

SIL_STATUS InitializeApiZen4 (void);


