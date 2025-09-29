/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file SmuInitPhx.h
 * @brief Prototypes for PHX SMU core specific transfer table functions
 *
 */

#pragma once

/// BIOS to SMU Message Definitions for Phoenix
#define SMC_MSG_GetNameString_PHX                       0x4  ///< Get CPU name string
#define SMC_MSG_ReadCoreCacWeightRegister_PHX           0xA  ///< Read Core CAC weights
#define SMC_MSG_GetCoreBistPF_PHX                       0x3F ///< To get CCX memory build in test result
#define SMC_Message_Count_PHX                           0x69 ///< Total number of BIOS messages
