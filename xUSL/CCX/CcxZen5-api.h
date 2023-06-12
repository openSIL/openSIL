/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file CcxZen5-api.h
 * @brief Zen5 core specific data declarations
 * @details This file contains the Host API definitions for the IP revision
 * contained in the SoC.
 *    Include into this file ONLY those items the Host needs to access
 *    and work with your data structure(s)
 */

#pragma once
#include <CCX/CcxClass-api.h>

/**
 *  Define a structure for IP version specific 'public' variables
 *   These variables are an extension to the IP common variables and
 *   are visible to the Host.
 */
typedef struct {
  CcxRevisions    field0;       ///< Revision of the IP for use by Host.
} SIL_RESERVED_STRUCT_0002;
