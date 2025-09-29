/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchTc.h
 * @brief OpenSIL TC FCH Class entry point declarations to be used in IpBlkListxxyy.h files
 *
 */

#pragma once

#include <Sil-api.h>
#include <FchClass-api.h>

// Define the amount of memory this IP block will need from the Host
// This value is used in the IP Block entry for this IP

#define FCHCLASS_INPBLK_SIZE_TC sizeof (FCHCLASS_INPUT_BLK)

/***************************************************************************
 * Function Prototypes
 *  These are the functions referenced in the IP block lists
 *  for initializing this IP
 */
SIL_STATUS InitializeFchApiv (void);
SIL_STATUS FchClassSetInputBlkTc (
  SIL_CONTEXT  *SilContext
  );
SIL_STATUS InitializeFchApiTc (
  SIL_CONTEXT   *SilContext
  );
