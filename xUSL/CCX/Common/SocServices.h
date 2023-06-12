/**
 * @file SocServices.h
 * @brief Soc specific services interface
 *
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

uint32_t
SocGetSoftDowncoreAddress (
  uint32_t CcdIndex
  );

bool SocIsOneCcdModeSupported (void);
bool SocIsS3Supported (void);
bool SocIsGameModeSupported (void);
