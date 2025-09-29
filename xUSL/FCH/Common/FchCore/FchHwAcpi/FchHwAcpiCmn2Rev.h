/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchHwAcpiCmn2Rev.h
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
 * FCH_INIT_PREPCIE_HWACPI
 *
 * @brief   Internal API type definition.
 *
 * @details Config FCH ACPI Module before PCI enumeration
 *
 */
typedef void (*FCH_HWACPI_PREPCIE_INIT) (
  FCHCLASS_INPUT_BLK  *FchDataBlock,
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

typedef void (*FCH_HWACPI_AOAC_INIT) (
  FCHHWACPI_INPUT_BLK *FchHwAcpi
  );

/**
 * FCH_HWACPI_SPREAD_SPECTRUM
 *
 * @brief Spread Spectrum Clock Programming
 *
 * @details The rev-specific function should implement SSC programming
 * sequences for both enabling and disabling SSC. The function must execute
 * either the enabling or disabling sequence based on the Enable argument.
 * The check for whether the sequence needs to be called is done in the common
 * function.
 */
typedef void (*FCH_HWACPI_SPREAD_SPECTRUM) (
  bool Enable
  );

typedef void (*FCH_HWACPI_ZSTATE_TIMING) (
  void
  );

typedef void (*FCH_HWACPI_I2C_I3C_INIT) (
  FCHCLASS_INPUT_BLK *FchDataPtr
  );

typedef void (*FCH_HWACPI_UART_INIT) (
  FCHCLASS_INPUT_BLK *FchDataPtr
  );

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
  FCH_HWACPI_PREPCIE_INIT          FchHwAcpiPrePcieInit;
  FCH_HWACPI_AOAC_INIT             FchHwAcpiAoacInit;
  FCH_HWACPI_UART_INIT             FchHwAcpiUartInit;
  FCH_HWACPI_I2C_I3C_INIT          FchHwAcpiI2cI3cInit;
  FCH_HWACPI_SPREAD_SPECTRUM       FchHwAcpiSpreadSpectrum;
  FCH_HWACPI_ZSTATE_TIMING         FchHwAcpiZstateTiming;
} FCH_HWACPI_XFER_TABLE;
