/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file CcxPhx.h
 * @brief PHX Zen4 core specific headers and function declarations for use in
 *   the IP block list files (e.g.  \xSIM\SoC\F19M70\IpBlkListF19M70Tp1.c)

 */

#pragma once

#include <CCX/CcxClass-api.h>
#include <CCX/CcxZen4-api.h>
#include <CCX/Common/Ccx.h>

// Define the amount of memory this IP block will need from the Host
// This value is used in the IP Block entry for this IP
#define CCX_DATA_SIZE_ZEN4_PHX (sizeof (CCXCLASS_DATA_BLK) + \
        sizeof (CCX_DATA_ZEN4) + sizeof (CCX_DATA_BLOCK))

/***************************************************************************
 * Function Prototypes
 *  These are the functions referenced in the IP block lists
 *  for initializing this IP
 */
SIL_STATUS CcxSetInputBlkPhx (SIL_CONTEXT  *SilContext);
SIL_STATUS InitializeCcxZen4PhxTp1 (SIL_CONTEXT  *SilContext);
SIL_STATUS InitializeCcxZen4PhxTp2 (SIL_CONTEXT  *SilContext);
SIL_STATUS InitializeCcxZen4PhxTp3 (SIL_CONTEXT  *SilContext);
SIL_STATUS InitializeApiZen4Phx (SIL_CONTEXT  *SilContext);
