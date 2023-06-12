/**
 * @file  FchSataCmn2Rev.h
 * This file defines the types of all functions contained within SMU's Cmn2Rev
 * xfer table, as well as the table itself as a struct containing pointers to
 * these functions
 */
/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <FCH/Common/FchCore/FchSata/FchSataDef.h>

// Common function type definitions of Cmn2Rev functions

/**
 * INIT_RESET_SATA_PROG
 *
 * @brief   Internal API type definition.
 *
 * @details Config Sata controller during Power-On
 *
 */
typedef void (*INIT_RESET_SATA_PROG) (
  uint32_t          DieBusNum,
  FCHSATA_INPUT_BLK *FchSata
  );

/**
 * SATA_INIT_HIDE_SATA_PCI
 *
 * @brief   Internal API type definition.
 *
 * @details Disable Sata Controller PCI configuration space.
 *
 */
typedef void (*SATA_INIT_HIDE_SATA_PCI) (
  uint32_t  DieBusNum,
  uint32_t  Controller
  );

/**
 * SATA_INIT_MSI
 *
 * @brief   Internal API type definition.
 *
 * @details Enable/Disable Sata Msi capabilities
 *
 */
typedef void (*SATA_INIT_MSI) (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  );

/**
 * SET_BISTL_COMP_MODE
 *
 * @brief   Internal API type definition.
 *
 * @details Set Sata port BIST-L Compliance mode
 *
 */
typedef void (*SET_BISTL_COMP_MODE) (
  uint32_t          DieBusNum,
  uint32_t          Controller,
  FCHSATA_INPUT_BLK *FchSata
  );

/**
 * SGPIO_TO_MPIO
 *
 * @brief   Internal API type definition.
 *
 * @details Set to MPIO mode
 *
 */
typedef void (*SGPIO_TO_MPIO) (
  uint32_t   DieBusNum,
  uint32_t   Controller
  );

/**
 * INIT_ENV_SATA
 *
 * @brief   Internal API type definition.
 *
 * @details Config SATA controller before PCI emulation
 *
 */
typedef void (*INIT_ENV_SATA) (
  FCHSATA_INPUT_BLK *FchSata
  );

/**
 * SATA_INIT_MMC
 *
 * @brief   Internal API type definition.
 *
 * @details Sata MSI MMC setting
 *
 */
typedef void (*SATA_INIT_MMC) (
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
  FCH_SATA_INT_API_HEADER   Header;
  INIT_RESET_SATA_PROG      FchInitResetSataProgram;
  SATA_INIT_HIDE_SATA_PCI   FchSataInitHideSataPci;
  SATA_INIT_MSI             FchSataInitMsi;
  SET_BISTL_COMP_MODE       FchSataSetBistLComplianceMode;
  SGPIO_TO_MPIO             FchSgpioToMpio;
  INIT_ENV_SATA             FchInitEnvSata;
  SATA_INIT_MMC             FchSataInitMmc;
} FCH_SATA_XFER_TABLE;

