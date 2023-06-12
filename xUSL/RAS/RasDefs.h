/* SPDX-License-Identifier: MIT */
/**
 * Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */
/**
 * @file    RasDefs.h
 * @details This file contains Structures and definitions used to define the RAS internal and ip-to-ip APIs
 */

#pragma once
#include <RAS/Common/RasClass-api.h>
#include <RAS/Common/RasReg-api.h>
#include <RAS/Common/RasReg.h>

/**
 * The number of Core MCA Bank
 *
 * The Core MCA banks (7 banks in total) are present in the address space of every logical core.
 */
#define MAX_CORE_MCA_BANK_COUNT (7)

#define IF_THR0_SMNADDR_BYTE1       0xAA
#define L2_THR0_SMNADDR_BYTE1       0x8A
#define DE_THR0_SMNADDR_BYTE1       0x96
#define EX_THR0_SMNADDR_BYTE1       0x9A
#define FP_THR0_SMNADDR_BYTE1       0x86
