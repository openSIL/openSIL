/**
 *  @file PciecoreReg.h
 *  @brief This file contains Pcie Core Register definitions
 */

/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

typedef union {
  struct {
    uint32_t                            LC_PREV_STATE40:6;
    uint32_t                            Reserved_7_6:2;
    uint32_t                            LC_PREV_STATE41:6;
    uint32_t                            Reserved_15_14:2;
    uint32_t                            LC_PREV_STATE42:6;
    uint32_t                            Reserved_23_22:2;
    uint32_t                            LC_PREV_STATE43:6;
    uint32_t                            Reserved_31_30:2;
  } Field;
  uint32_t Value;
} PCIE_LC_STATE10_STRUCT;
