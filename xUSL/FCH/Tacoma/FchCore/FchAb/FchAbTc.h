/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file FchAbTc.h
 * @brief Fch Ab specific headers and function declarations for use in
 *   the IP block list files (e.g.  \xSIM\SoC\F1AM00\IpBlkListF19M10Tp1.c)
 */

#pragma once

// Define the amount of memory this IP block will need from the Host
// This value is used in the IP Block entry for this IP
#define FCH_AB_INPBLK_SIZE_TC sizeof (FCHAB_INPUT_BLK)

/***************************************************************************
 * Function Prototypes
 *  These are the functions referenced in the IP block lists
 *  for initializing this IP
 */

SIL_STATUS InitializeFchAbTcTp1 (
  SIL_CONTEXT  *SilContext
  );
SIL_STATUS FchAbSetInputBlkTc (
  SIL_CONTEXT  *SilContext
  );
