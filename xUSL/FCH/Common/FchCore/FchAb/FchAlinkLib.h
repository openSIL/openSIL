/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchAlinkLib.h
 * @brief FCH Service routines for Alink register access
 */

#pragma once

///
/// _ABTblEntry - AB link register table R/W structure
///
typedef struct {
  uint32_t RegIndex; // RegIndex : AB Register Index
  uint32_t RegMask;  // RegMask  : AB Register Mask
  uint32_t RegData;  // RegData  : AB Register Data
} AB_TBL_ENTRY;

uint32_t ReadAlink (uint32_t Index);
void WriteAlink (uint32_t Index, uint32_t Data);
void RwAlink (uint32_t Index, uint32_t AndMask, uint32_t OrMask);
