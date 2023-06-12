/**
 * @file  CoreTechnologyService.h
 * @brief CoreTechnology service definition.
 *
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

SIL_STATUS
GetCoreTopologyOnDie (
  uint32_t Socket,
  uint32_t Die,
  uint32_t *NumberOfCcds,
  uint32_t *NumberOfComplexes,
  uint32_t *NumberOfCores,
  uint32_t *NumberOfThreads
  );
