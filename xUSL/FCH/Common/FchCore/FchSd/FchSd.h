/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchAb.h
 * @brief FCH AB Entry point definitions
 *
 * This file holds the declarations for the AB sub-component entry points for
 * use in the IpBlkListFxxMxx.c files.
 * Only the information needed by for the entry point is included here.
 */

#pragma once

#include <FCH/FchClass-api.h>

void
FchInitEnvSd (
  FCH_SD *FchSd
  );
