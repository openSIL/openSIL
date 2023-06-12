/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT
/**
 * @file  MultiFchCmn2Rev.h
 * This file defines the types of all functions contained within Fch's Cmn2Rev
 * xfer table, as well as the table itself as a struct containing pointers to
 * these functions
 */

#pragma once
#include <Sil-api.h>
#include <stdint.h>

// Common function type definitions of Cmn2Rev functions

typedef SIL_STATUS (*FCH_EN_IO_CXX_ON_DIEN) (
  uint32_t          *IoBaseRegValOrg,
  uint32_t          *IoLimitRegValOrg
);

typedef SIL_STATUS (*FCH_DIS_IO_CXX_ON_DIEN) (
  uint32_t          IoBaseRegValOrg,
  uint32_t          IoLimitRegValOrg
);

typedef SIL_STATUS (*FCH_GET_SEC_RB_NUMBER) (
  uint8_t   *RootBridgeNumber
);

typedef SIL_STATUS (*FCH_GET_SEC_BUS_NUMBER) (
  uint8_t   *FchBusNum
);

// Define the Cmn2Rev xfer table containing pointers to these functions

typedef struct {
  FCH_EN_IO_CXX_ON_DIEN   EnableIoCxxOnDieN;
  FCH_DIS_IO_CXX_ON_DIEN  DisableIoCxxOnDieN;
  FCH_GET_SEC_RB_NUMBER   GetSecFchRootBridgeNumber;
  FCH_GET_SEC_BUS_NUMBER  GetSecFchBusNumber;
} MULTI_FCH_COMMON_2_REV_XFER_BLOCK;
