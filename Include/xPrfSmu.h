/**
 * @file  xPrfSmu.h
 * @brief xPRF SMU, SMN and PCI-config service prototypes.
 *
 * PCI addresses encode segment:bus:device:function:offset in bits
 * 31:28, 27:20, 19:15, 14:12 and 11:0 respectively.
 * PCI access widths are 1, 2, 3 and 4 for 8, 16, 32 and 64 bits;
 * the corresponding S3-save encodings are 0x81 through 0x84.
 */

/* SPDX-License-Identifier: MIT
 * Copyright (C) 2021 - 2026 Advanced Micro Devices, Inc. All rights reserved.
 */

#pragma once

#include <stdint.h>
#include <Sil-api.h>

/// Returns the raw SMU response code: 1 on success, 0xff on failure to locate the SMU API.
uint32_t
xPrfSmuServiceRequest (
  uint32_t  NbioPciAddress,
  uint32_t  RequestId,
  uint32_t  *RequestArgument,
  uint32_t  AccessFlags
  );

SIL_STATUS
xPrfSmuServiceRequestByInstance (
  uint32_t  InstanceId,
  uint32_t  RequestId,
  uint32_t  *RequestArgument
  );

SIL_STATUS
xPrfSmuServiceInitArguments (
  uint32_t  *RequestArgument
  );

SIL_STATUS
xPrfSmuRegisterRead (
  uint32_t  InstanceId,
  uint32_t  RegisterIndex,
  uint32_t  *RegisterValue
  );

SIL_STATUS
xPrfSmuRegisterWrite (
  uint32_t  InstanceId,
  uint32_t  RegisterIndex,
  uint32_t  *RegisterValue
  );

SIL_STATUS
xPrfSmuRegisterRMW (
  uint32_t  InstanceId,
  uint32_t  RegisterIndex,
  uint32_t  RegisterANDValue,
  uint32_t  RegisterORValue
  );

SIL_STATUS
xPrfSmuReadBrandString (
  uint32_t  InstanceId,
  uint32_t  BrandStringLength,
  uint8_t   *BrandString
  );

SIL_STATUS
xPrfSmuReadCacWeights (
  uint32_t  MaxNumWeights,
  uint64_t  *ApmWeights
  );

SIL_STATUS
xPrfSmuEnableNvmSelfRefresh (void);

uint32_t
xPrfSmnRead (
  uint32_t  SegmentNumber,
  uint32_t  IohcBus,
  uint32_t  SmnAddress
  );

void
xPrfSmnWrite (
  uint32_t  SegmentNumber,
  uint32_t  IohcBus,
  uint32_t  SmnAddress,
  uint32_t  Value
  );

void
xPrfSmnReadModifyWrite (
  uint32_t  SegmentNumber,
  uint32_t  IohcBus,
  uint32_t  SmnAddress,
  uint32_t  AndMask,
  uint32_t  OrMask
  );

void
xPrfPciRead (
  uint32_t      Address,
  uint32_t      Width,
  void          *Value
  );

void
xPrfPciWrite (
  uint32_t      Address,
  uint32_t      Width,
  void          *Value
  );

void
xPrfPciRmw (
  uint32_t      Address,
  uint32_t      Width,
  uint32_t      Mask,
  uint32_t      OrValue
  );
