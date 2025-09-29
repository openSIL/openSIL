/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file CcxZen4-api.h
 * @brief Zen4 core specific data declarations
 * @details This file contains the Host API definitions for the IP revision
 * contained in the SoC.
 */

#pragma once


/**
 *  Define a structure for IP version specific 'public' variables
 *   These variables are an extension to the IP common variables and
 *   are visible to the Host.
 */
typedef struct {
  CcxRevisions    CcxIpRev;       ///< Revision of the IP for use by Host.

} CCX_DATA_ZEN4;
