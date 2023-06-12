/**
 * @file  NbioData.h
 * OpenSIL NBIO data definitions
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <NBIO/NbioClass-api.h>
#include <xSIM.h>
#include <CommonLib/CpuLib.h>
#include <CommonLib/SmnAccess.h>

/**
 * NBIOCLASS_DATA
 *
 * Placeholder for all NBIO IP configuration data.
 * This would be comibination of NBIO private data +
 * NBIO host shared config block(NBIOCLASS_INPUT_BLK)
 *
 */
typedef struct {
  //Fixed configs and NBIO private data goes here.
  NBIO_CONFIG_DATA *NbioConfigData;
  // This would point to the host memory for NBIO Input block
  NBIOCLASS_INPUT_BLK *NbioInputBlk;
} NBIOCLASS_DATA;

/**
 * Declare function prototypes here
 *
 */
void InitNbioConfigData (void);
NBIOCLASS_DATA *GetNbioBlockData (void);
