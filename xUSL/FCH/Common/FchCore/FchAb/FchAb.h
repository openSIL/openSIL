/**
 * @file  FchAb.h
 * @brief FCH AB definitions
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <FCH/Common/FchCommonCfg.h>

/*******************************************************************************
 * @brief Function prototypes
 *
 */

uint32_t ReadAlink (uint32_t Index);
void WriteAlink (uint32_t Index, uint32_t Data);
void RwAlink (uint32_t Index, uint32_t AndMask, uint32_t OrMask);
void FchAbPrePcieInit(FCHAB_INPUT_BLK *LclInpFchAbBlk);
SIL_STATUS FchAbSetInputBlk (void);
SIL_STATUS InitializeFchAbTp1 (void);
SIL_STATUS InitializeFchAbTp2 (void);
SIL_STATUS InitializeFchAbTp3 (void);

/*******************************************************************************
 * @brief Module data structure
 *
 */

/*******************************************************************************
 * @brief Declare macros here
 *
 */
#define FCHAB_MAJOR_REV 0
#define FCHAB_MINOR_REV 1
#define FCHAB_INSTANCE  0
