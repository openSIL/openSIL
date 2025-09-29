/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file FchIsaCmn2Rev.h
 * @brief This file defines the types of all functions contained within FCH ISA Cmn2Rev
 * transfer table as well as a definition of the transfer table itself.
 *
 */

#pragma once

#include <SilCommon.h>
#include <FchIsa-api.h>
#include <FchClass-api.h>

typedef void (*FCH_ISA_RELOCATE_ROM3) (
  SIL_CONTEXT *SilContext
  );

typedef void (*FCH_ISA_TPM_INIT) (
  FCH_SPI *FchSpiData
  );

typedef struct {
  uint8_t   Version;
} FCH_ISA_INT_API_HEADER;

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
  FCH_ISA_INT_API_HEADER  Header;
  FCH_ISA_RELOCATE_ROM3   FchIsaRelocateRom3;
  FCH_ISA_TPM_INIT        FchIsaTpmInit;
} FCH_ISA_XFER_TABLE;
