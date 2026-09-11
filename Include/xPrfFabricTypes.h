/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2026 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  xPrfFabricTypes.h
 * @brief Host-visible data fabric topology types.
 */

#pragma once

#include <stdint.h>

typedef enum {
  XprfFabricCcm,
  XprfFabricGcm,
  XprfFabricNcs,
  XprfFabricNcm,
  XprfFabricPie,
  XprfFabricIoms,
  XprfFabricCs,
  XprfFabricTcdx,
  XprfFabricCake,
  XprfFabricCsUmc,
  XprfFabricCsCcix,
  XprfFabricCsCmp,
  XprfFabricAcm,
  XprfFabricIom,
  XprfFabricIos,
  XprfFabricIcng,
  XprfFabricCnli,
  XprfFabricPfx,
  XprfFabricSpf,
  XprfFabricNcmIommu,
  XprfFabricGiom,
  XprfFabricHtdm,
  XprfFabricHtds,
  XprfFabricXgmi,
  XprfFabricDeviceTypeMax
} XPRF_FABRIC_DEVICE_TYPE;

#pragma pack (push, 1)

typedef struct {
  uint32_t  Socket;
  uint32_t  Die;
  uint32_t  Index;
} XPRF_ROOT_BRIDGE_LOCATION;

typedef struct {
  uint32_t  FabricID;
  uint32_t  InstanceID;
} XPRF_FABRIC_DEVICE_IDS;

/**
 * A die's map ends at an entry with Type == XprfFabricDeviceTypeMax.
 * The map and each Count-element IDs array belong to openSIL. They are
 * read-only and valid until topology is rebuilt or the firmware phase ends.
 * Hosts with native-width fields must convert each field before use.
 */
typedef struct {
  uint32_t                      Type;  ///< XPRF_FABRIC_DEVICE_TYPE value
  uint32_t                      Count;
  const XPRF_FABRIC_DEVICE_IDS  *IDs;
} XPRF_FABRIC_DEVICE_MAP;

#pragma pack (pop)
