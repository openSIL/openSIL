/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  DfBrh.h
 * @brief Brh entrypoint and API init function declarations
 */

#pragma once

#include <xSIM.h>
#include <DF/DfClass-api.h>
#include <DF/DfX/DfXAcpiDomainInfo.h>

#define DF_DATA_SIZE_BRH sizeof (DFCLASS_INPUT_BLK) + sizeof (DF_DOMAIN_INFO_BLK) + sizeof (SIL_RESERVED_STRUCT_0009)

SIL_STATUS
DfSetInputBlkBrh (void);
SIL_STATUS
DfInitApiBrh (void);
SIL_STATUS
InitializeDataFabricTp1Brh (void);
SIL_STATUS
InitializeDataFabricTp2Brh (void);
SIL_STATUS
InitializeDataFabricTp3Brh (void);
