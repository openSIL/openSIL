/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2026 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  CxlDeviceInfo.h
 * @brief CXL endpoint discovery and HDM decoder reads for DXE consumers
 */

#pragma once

#include <SilCommon.h>

/// CXL device classes, as encoded in the CXL DVSEC capability field
#define CXL_DEVICE_TYPE_NONE   0  ///< Not a CXL device
#define CXL_DEVICE_TYPE_CACHE  1  ///< CXL.cache only
#define CXL_DEVICE_TYPE_BOTH   2  ///< CXL.cache and CXL.mem
#define CXL_DEVICE_TYPE_MEM    3  ///< CXL.mem only

/// A device exposes at most two HDM decoder ranges through its DVSEC
#define CXL_MAX_HDM_RANGES  2

/// Words CxlGetSwitchEndpoints writes per endpoint
#define CXL_SWITCH_ENDPOINT_WORDS  3

/// What one CXL endpoint reports about itself through its DVSEC
typedef struct {
  uint16_t  DvsecPtr;                        ///< Config offset of the CXL DVSEC, zero if absent
  uint8_t   CxlType;                         ///< One of the CXL_DEVICE_TYPE values
  uint8_t   HdmCount;                        ///< HDM decoder ranges the device implements
  uint8_t   MemHwInitMode;                   ///< Device sets Memory_Active itself after reset
  uint8_t   BaseClassCode;                   ///< PCI base class, identifies dual mode cards
  uint32_t  RangeSizeHi[CXL_MAX_HDM_RANGES];
  uint32_t  RangeSizeLo[CXL_MAX_HDM_RANGES];
} CXL_ENDPOINT_INFO;

/**
 * CxlDecodeDvsecCapability
 *
 * @brief Split the CXL DVSEC capability word into the fields callers act on.
 *
 * @details Separate from the register read so the decode can be exercised
 *          without a device present.
 *
 * @param[in]  Capability     Contents of the DVSEC capability register
 * @param[out] CxlType        One of the CXL_DEVICE_TYPE values
 * @param[out] HdmCount       HDM decoder ranges, clamped to CXL_MAX_HDM_RANGES
 * @param[out] MemHwInitMode  Device sets Memory_Active itself after reset
 */
void
CxlDecodeDvsecCapability (
  uint16_t  Capability,
  uint8_t   *CxlType,
  uint8_t   *HdmCount,
  uint8_t   *MemHwInitMode
  );

/**
 * CxlGetDeviceInfo
 *
 * @brief Read a CXL endpoint's DVSEC and HDM decoder state.
 *
 * @param[in]  Address  Encoded PCI address of the endpoint
 * @param[out] Info     Receives the endpoint description
 *
 * @retval SilPass              Info is populated, DvsecPtr zero if not a CXL device
 * @retval SilInvalidParameter  Info is NULL
 */
SIL_STATUS
CxlGetDeviceInfo (
  uint32_t         Address,
  CXL_ENDPOINT_INFO  *Info
  );

/**
 * CxlGetRangeSizeLow
 *
 * @brief Re-read one HDM range's size-low register.
 *
 * @details Split out from CxlGetDeviceInfo so a caller waiting for a device to
 *          set Memory_Active can poll it without knowing the register layout.
 *          The wait itself is left to the caller, which owns the timer.
 *
 * @param[in]  Address     Encoded PCI address of the endpoint
 * @param[in]  DvsecPtr    Config offset of the CXL DVSEC
 * @param[in]  RangeIndex  Zero based HDM range
 * @param[out] Value       Receives the register contents
 *
 * @retval SilPass              Value is populated
 * @retval SilInvalidParameter  RangeIndex out of range or Value is NULL
 */
SIL_STATUS
CxlGetRangeSizeLow (
  uint32_t  Address,
  uint16_t  DvsecPtr,
  uint32_t  RangeIndex,
  uint32_t  *Value
  );

/**
 * CxlGetLinkPortNumber
 *
 * @brief Read the port number a device reports in its PCIe link capabilities.
 *
 * @param[in] Address  Encoded PCI address
 *
 * @retval uint8_t     Port number, zero when the device has no PCIe capability
 */
uint8_t
CxlGetLinkPortNumber (
  uint32_t  Address
  );

/**
 * CxlGetSwitchEndpoints
 *
 * @brief Walk the buses behind a CXL switch and list the CXL endpoints found.
 *
 * @param[in]  SwitchAddress  Encoded PCI address of the switch upstream port
 * @param[in]  MaxEndpoints   Capacity of Buffer, in endpoints
 * @param[out] Count          Endpoints written
 * @param[out] Buffer         Receives CXL_SWITCH_ENDPOINT_WORDS uint32_t per
 *                            endpoint: endpoint address, downstream port
 *                            address, port number
 *
 * @retval SilPass              Walk completed, Count may be zero
 * @retval SilInvalidParameter  Count or Buffer is NULL
 * @retval SilOutOfBounds       More endpoints present than MaxEndpoints
 */
SIL_STATUS
CxlGetSwitchEndpoints (
  uint32_t  SwitchAddress,
  uint32_t  MaxEndpoints,
  uint32_t  *Count,
  uint32_t  *Buffer
  );
