/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file CcxBrh.h
 * @brief BRH Zen5 core specific headers and function declarations for use in
 *   the IP block list files (e.g.  \xSIM\SoC\F1AM00\IpBlkListF1AM00Tp1.c)

 */

#pragma once

#include <CCX/CcxClass-api.h>
#include <CCX/CcxZen5-api.h>

// Define the amount of memory this IP block will need from the Host
// This value is used in the IP Block entry for this IP
#define CCX_DATA_SIZE_ZEN5_BRH (sizeof (CCXCLASS_DATA_BLK) + \
        sizeof (SIL_RESERVED_STRUCT_0002))

/***************************************************************************
 * Declare Function prototypes
 *  These are the functions referenced in the IP block lists
 *  for initializing this IP
 */
SIL_STATUS CcxClassSetInputBlkBrh (void);
SIL_STATUS InitializeCcxZen5BrhTp1 (void);
SIL_STATUS InitializeCcxZen5BrhTp2 (void);
SIL_STATUS InitializeCcxZen5BrhTp3 (void);
SIL_STATUS InitializeApiZen5Brh (void);
