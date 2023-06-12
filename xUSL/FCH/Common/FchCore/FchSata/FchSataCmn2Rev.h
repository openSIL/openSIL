/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchSataCmn2Rev.h
 * This file defines the types of all functions contained within SMU's Cmn2Rev
 * xfer table, as well as the table itself as a struct containing pointers to
 * these functions
 */

#pragma once

#include <FCH/FchSata-api.h>

// Common function type definitions of Cmn2Rev functions

/**
 * FCH_SATA_INIT_HIDE_PCI
 *
 * @brief   Internal API type definition.
 *
 * @details Disable Sata Controller PCI configuration space.
 *
 */
typedef void (*FCH_SATA_INIT_HIDE_PCI) (
  uint32_t  DieBusNum,
  uint32_t  Controller
  );

/**
 * FCH_SATA_INIT_MSI
 *
 * @brief   Internal API type definition.
 *
 * @details Enable/Disable Sata MSI capabilities
 *
 */
typedef void (*FCH_SATA_INIT_MSI) (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  );

/**
 * FCH_SATA_SET_BISTL_COMP_MODE
 *
 * @brief   Internal API type definition.
 *
 * @details Set Sata port BIST-L Compliance mode
 *
 */
typedef void (*FCH_SATA_SET_BISTL_COMP_MODE) (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  );

/**
 * FCH_SATA_INIT_ENV_AHCI
 *
 * @brief
 *
 * @details
 *
 */
typedef void (*FCH_SATA_INIT_ENV_AHCI) (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  );

typedef void (*FCH_SATA_INIT_DEVICE_SLEEP) (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  );

/**
 * FCH_SATA_INIT_ENV_RAID
 *
 * @brief
 *
 * @details
 *
 */
typedef void (*FCH_SATA_INIT_ENV_RAID) (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  );

typedef void (*FCH_SATA_AUTO_SHUTDOWN) (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  );

typedef struct {
  uint8_t   Version;
} FCH_SATA_INT_API_HEADER;

/*
 * Fch SATA IP Transfer (xfer) Table
 *
 * This structure contains an API for internal IP specific functions that need
 * to run from IP common code.
 *
 * IP specific code should populate this structure of function pointers with
 * the appropriate ip version specific function.
 *
 */
typedef struct {
  FCH_SATA_INT_API_HEADER           Header;
  FCH_SATA_INIT_HIDE_PCI            FchSataInitHideSataPci;
  FCH_SATA_INIT_MSI                 FchSataInitMsi;
  FCH_SATA_SET_BISTL_COMP_MODE      FchSataSetBistLComplianceMode;
  FCH_SATA_INIT_ENV_AHCI            FchSataInitEnvAhci;
  FCH_SATA_INIT_ENV_RAID            FchSataInitEnvRaid;
  FCH_SATA_INIT_DEVICE_SLEEP        FchSataInitDevSlp;
  FCH_SATA_AUTO_SHUTDOWN            FchSataAutoShutdown;
} FCH_SATA_XFER_TABLE;
