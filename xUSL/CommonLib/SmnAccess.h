/* SPDX-License-Identifier: MIT */
/**
 * @file  SmnAccess.h
 * @brief OpenSIL SMN access functions prototype
 *
 * Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */

#pragma once

#define SMNx00B8  0x00B8
#define SMNx00BC   0x00BC

/**********************************************************************************************************************
 * @brief Function prototypes
 *
 */

uint32_t xUSLSmnRead (uint32_t SegmentNumber, uint32_t IohcBus, uint32_t SmnAddress);
void xUSLSmnWrite (uint32_t SegmentNumber, uint32_t IohcBus, uint32_t SmnAddress, uint32_t Value);
void xUSLSmnReadModifyWrite (uint32_t SegmentNumber, uint32_t IohcBus, uint32_t SmnAddress, uint32_t AndMask,
  uint32_t OrMask);
uint8_t xUSLSmnRead8 (uint32_t SegmentNumber, uint32_t IohcBus, uint32_t SmnAddress);
void xUSLSmnWrite8 (uint32_t SegmentNumber, uint32_t IohcBus, uint32_t SmnAddress, uint8_t Value8);
void xUSLSmnReadModifyWrite8 (uint32_t SegmentNumber, uint32_t IohcBus, uint32_t SmnAddress, uint8_t AndMask,
  uint8_t OrMask);
