/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchLpc.h
 * @brief FCH LPC header
 *
 *
 */

#pragma once

#include <FchIsa-api.h>
#include <FchClass-api.h>

void
FchInitEnvLpc (
  FCHCLASS_INPUT_BLK *FchDataPtr,
  FCH_LPC            *FchLpc
  );
