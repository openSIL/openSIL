/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2024 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file GfxPhx.h
 *  @brief GFX function prototypes, structs, and defines
 */

#pragma once

#include <xSIM.h>
#include <GFX/GfxClass-api.h>
#include <GFX/Common/GfxDisplayPhySettings.h>
#include <GFX/Common/GfxDisplayTypeSettings.h>

extern const GFXCLASS_INPUT_BLK GfxClassDflts;

#define GFX_DATA_SIZE_PHX sizeof (GFXCLASS_INPUT_BLK)


/***************************************************************************
 * Function prototypes
 *  These are the functions referenced in the IP block lists
 *  for initializing this IP
 */

SIL_STATUS
GfxSetInputBlkPhx (
  SIL_CONTEXT  *SilContext
  );

SIL_STATUS
InitializeGfxPhxTp1 (
  SIL_CONTEXT  *SilContext
  );

SIL_STATUS
InitializeGfxPhxTp2 (
  SIL_CONTEXT  *SilContext
  );

SIL_STATUS
InitializeGfxPhxTp3 (
  SIL_CONTEXT  *SilContext
  );
