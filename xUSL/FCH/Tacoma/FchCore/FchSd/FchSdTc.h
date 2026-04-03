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
#define FCH_SD_INPBLK_SIZE_TC sizeof (FCHSD_INPUT_BLK)

/***************************************************************************
 * Function Prototypes
 *  These are the functions referenced in the IP block lists
 *  for initializing this IP
 */

SIL_STATUS InitializeFchSdTcTp1 (
  SIL_CONTEXT  *SilContext
  );
SIL_STATUS FchSdSetInputBlkTc (
  SIL_CONTEXT  *SilContext
  );
