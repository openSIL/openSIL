/* SPDX-License-Identifier: MIT */
/**
 * Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */
/**
 * @file  Mca.h
 * @brief MCA common register descriptions and addresses that can be exposed to host firmware
 */

#pragma once

/**
 * @brief The MCA Hardware ID and MCA TYPE of DATA FABRIC Block
 *
 * @details Please refer to MCA_CPU_CORE_ID
 */
  #define CS_MCA_TYPE                   (0x0002)
#define CS_MCA_BANK_HARDWARE_ID         0x2E

typedef union {
  struct {
    uint64_t  McaX:1;
    uint64_t  TransparentErrorLoggingSupported:1;
    uint64_t  DeferredErrorLoggingSupported:1;
    uint64_t  :2;
    uint64_t  DeferredIntTypeSupported:1;
    uint64_t  :26;
    uint64_t  McaXEnable:1;
    uint64_t  TransparentErrorLoggingEnable:1;
    uint64_t  LogDeferredInMcaStat:1;
    uint64_t  :2;
    uint64_t  DeferredIntType:2;
    uint64_t  :25;
  } Field;
  uint64_t  Value;
} SIL_MCA_CONFIG_MSR;
