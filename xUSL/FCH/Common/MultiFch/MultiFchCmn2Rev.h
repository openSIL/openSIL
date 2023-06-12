/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MultiFchCmn2Rev.h
 * This file defines the types of all functions contained within Fch's Cmn2Rev
 * xfer table, as well as the table itself as a struct containing pointers to
 * these functions
 */

#pragma once
#include <Sil-api.h>
#include <stdint.h>

typedef SIL_STATUS (*FCH_MULTI_FCH_GET_FCH_RB_INDEX_ON_DIE) (
  uint32_t *RbIndex
  );

typedef SIL_STATUS (*FCH_MULTI_FCH_INIT_AB) (
  uint32_t SocketNum,
  uint32_t DieNum,
  uint32_t RbNum,
  uint64_t AcpiMmioBaseAddr,
  uint32_t IohcBusNumber,
  uint8_t  HasFchModule
  );

typedef SIL_STATUS (*FCH_MULTI_FCH_INIT_SATA) (
  uint32_t SocketNum,
  uint32_t DieNum,
  uint32_t RbNum,
  uint64_t AcpiMmioBaseAddr,
  uint32_t IohcBusNumber,
  uint8_t  HasFchModule
  );

typedef SIL_STATUS (*FCH_MULTI_FCH_INIT_USB) (
  uint32_t SocketNum,
  uint32_t DieNum,
  uint32_t RbNum,
  uint64_t AcpiMmioBaseAddr,
  uint32_t IohcBusNumber,
  uint8_t  HasFchModule
  );

typedef SIL_STATUS (*FCH_MULTI_FCH_INIT_SD) (
  uint32_t SocketNum,
  uint32_t DieNum,
  uint32_t RbNum,
  uint64_t AcpiMmioBaseAddr,
  uint32_t IohcBusNumber,
  uint8_t  HasFchModule
  );

typedef struct {
  uint8_t   field0;
} SIL_RESERVED_STRUCT_0025;

// Define the Cmn2Rev xfer table containing pointers to these functions
typedef struct {
  SIL_RESERVED_STRUCT_0025                      field0;
  FCH_MULTI_FCH_GET_FCH_RB_INDEX_ON_DIE     field1;
  FCH_MULTI_FCH_INIT_AB                     field2;
  FCH_MULTI_FCH_INIT_USB                    field3;
  FCH_MULTI_FCH_INIT_SD                     field4;
  FCH_MULTI_FCH_INIT_SATA                   field5;
} SIL_RESERVED_STRUCT_0026;
