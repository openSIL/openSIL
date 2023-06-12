/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file VirtualMemory.h
 * @brief x64 Long Mode Virtual Memory Management Definitions
 *
 */

#pragma once

// Structure for a GDT segment descriptor.
#pragma pack(1)
typedef union {
  struct {
    uint16_t LimitLow;
    uint16_t BaseLow;
    uint8_t BaseMid;
    struct {
      uint8_t Type: 4;
      uint8_t System: 1;
      uint8_t DescriptorPrivilegeLevel: 2;
      uint8_t Present: 1;
    } AccessByte;
    struct {
      uint8_t LimitHigh: 4;
      uint8_t Available: 1;
      uint8_t Reserved: 1;
      uint8_t DefaultOperationSize: 1;
      uint8_t Granularity: 1;
    } Flags;
    uint8_t BaseHigh;
  } Bits;
  uint64_t value;
} SegmentDescriptor;

// Structure for the GDTR descriptors.
typedef struct {
  uint16_t Limit;
  uintptr_t BaseAddress;
} DescriptorReg;
#pragma pack ()
