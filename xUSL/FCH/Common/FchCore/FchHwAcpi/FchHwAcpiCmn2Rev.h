/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file
 * This file is an EXAMPLE to the module owner for which to pattern
 * their actual init procedures.
 *
 * Each IP may supply services to the other IPs in the system. The provided
 * services are accessed via a standardized API table published by the IP.
 * The IP provides:
 *   - an Ip2Ipapi.h file for use by the other IPS so as to know what services
 *       are provided, along with parameters and returns for each service.
 *   - published structure containing function pointers to each service. This is
 *       communicated to the sil API handler functions.
 * The services listed in the transfer table may be implemented by the IP
 *     revision specific piece or the IP common functions piece.
 *     The transfer table provided by each IP revision MUST
 *     have the same structure.
 */


#pragma once

#include <SilCommon.h>
#include <FchClass-api.h>
#include <FchHwAcpi-api.h>

/**
 * FCH_INIT_PREPCIE_HWACPI_P
 *
 * @brief   Internal API type definition.
 *
 * @details Preliminary Config FCH ACPI Module before PCI enumeration
 *
 */
typedef void (*FCH_INIT_PREPCIE_HWACPI_P) (
  FCHCLASS_INPUT_BLK  *FchDataBlock,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

/**
 * FCH_INIT_PREPCIE_HWACPI
 *
 * @brief   Internal API type definition.
 *
 * @details Config FCH ACPI Module before PCI enumeration
 *
 */
typedef void (*FCH_INIT_PREPCIE_HWACPI) (
  FCHCLASS_INPUT_BLK  *FchDataBlock,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

/**
 * FCH_INIT_POSTPCIE_HWACPI_P
 *
 * @brief   Internal API type definition.
 *
 * @details Preliminary Config FCH ACPI Module after PCI enumeration
 *
 */
typedef void (*FCH_INIT_POSTPCIE_HWACPI_P) (
  FCHCLASS_INPUT_BLK  *FchDataBlock,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

/**
 * FCH_INIT_POSTPCIE_HWACPI
 *
 * @brief   Internal API type definition.
 *
 * @details Config FCH ACPI Module after PCI enumeration
 *
 */
typedef void (*FCH_INIT_POSTPCIE_HWACPI) (
  FCHCLASS_INPUT_BLK  *FchDataBlock,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

/**
 * FCH_INIT_PREOS_HWACPI_P
 *
 * @brief   Internal API type definition.
 *
 * @details Preliminary Config FCH ACPI Module before OS booting
 *
 */
typedef void (*FCH_INIT_PREOS_HWACPI_P) (
  FCHCLASS_INPUT_BLK  *FchDataBlock,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

/**
 * FCH_INIT_PREOS_HWACPI
 *
 * @brief   Internal API type definition.
 *
 * @details Config FCH ACPI Module before OS booting
 *
 */
typedef void (*FCH_INIT_PREOS_HWACPI) (
  FCHCLASS_INPUT_BLK  *FchDataBlock,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

typedef void (*FCH_HWACPI_RESET_RTC_EXT) (
  void
  );

typedef void (*FCH_HWACPI_ENABLE_IXC) (
  FCHCLASS_INPUT_BLK  *FchDataBlock
  );

typedef void (*FCH_HWACPI_UART_LEGACY_IO_INIT) (
  FCHCLASS_INPUT_BLK  *FchDataBlock
  );

typedef void (*FCH_HWACPI_AOAC_INIT) (
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

typedef void (*FCH_HWACPI_PROGRAMCPU_RST_BTMR) (
  void
  );

typedef struct {
  uint8_t   Version;
} FCH_HWACPI_INT_API_HEADER;

/**
 * Internal IP API
 *
 * This structure contains an API for internal IP specific functions that need
 * to run from IP common code.
 *
 * IP specific code should populate this structure of function pointers with
 * the appropriate ip version specific function.
 *
 */
typedef struct {
  FCH_HWACPI_INT_API_HEADER        Header;
  FCH_INIT_PREPCIE_HWACPI_P        FchInitPreliminaryPrePcieHwAcpi;
  FCH_INIT_PREPCIE_HWACPI          FchInitPrePcieHwAcpi;
  FCH_INIT_POSTPCIE_HWACPI_P       FchInitPreliminaryPostPcieHwAcpi;
  FCH_INIT_POSTPCIE_HWACPI         FchInitPostPcieHwAcpi;
  FCH_INIT_PREOS_HWACPI_P          FchInitPreliminaryPreOsHwAcpi;
  FCH_INIT_PREOS_HWACPI            FchInitPreOsHwAcpi;
  FCH_HWACPI_RESET_RTC_EXT         FchHwAcpiResetRtcExt;
  FCH_HWACPI_ENABLE_IXC            FchHwAcpiEnableIxC;
  FCH_HWACPI_UART_LEGACY_IO_INIT   FchHwAcpiUartLegacyIoInit;
  FCH_HWACPI_AOAC_INIT             FchHwAcpiAoacInit;
} FCH_HWACPI_XFER_TABLE;
