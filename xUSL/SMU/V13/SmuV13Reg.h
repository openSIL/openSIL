/**
 * @file SmuRegV13.h
 * @brief openSIL SMUV13 register definitions
 *
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <stdint.h>

#define SIL_RESERVED_819    0x5D288

typedef union {
  struct {
    uint32_t :22; // Reserved
    uint32_t CcdPresent :10;
  } Field;
  uint32_t Value;
} DFX_FUSE_CCD_PRESENT_1;

typedef union {
  struct {
    uint32_t CcdPresent :2;
    uint32_t :30; // Reserved
  } Field;
  uint32_t Value;
} DFX_CCD_PRESENT_2;

typedef union {
  struct {
    uint32_t :2; // Reserved
    uint32_t CcdDown :12;
    uint32_t :18; // Reserved
  } Field;
  uint32_t Value;
} DFX_FUSE_CCD_DOWN;

typedef union {
  struct {
    uint32_t Mp5CoreDisable :8;
    uint32_t Mp5SmtEnable :1;
    uint32_t :8; // Reserved
    uint32_t Mp5Ccx1Disable :8;
    uint32_t :7; // Reserved
  } Field;
  uint32_t Value;
} DFX_CORE_FUSE;

///
typedef union {
  struct {
    uint32_t :13; // Reserved
    uint32_t Mp0UmcCtrlXHarvest :12;
    uint32_t :7; // Reserved
  } Field;
  uint32_t Value;
} UMC_HARVEST;
