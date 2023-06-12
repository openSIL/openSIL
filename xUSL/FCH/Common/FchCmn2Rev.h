/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file FchCmn2Rev.h
 * @brief Declares Fch Class Cmn2Rev Xfer Table
 */

#pragma once

typedef void (*FCH_FUNC_SAMPLE) (void);

/*
 * FCH_COMMON_2_REV_XFER_BLOCK
 *
 * @brief  A temporary Common2Rev Transfer Block for Fch Class
 *
 * @details  This is a temporary Common2Rev Transfer Block for Fch Class
 *           intended to help IP owners to have a template for Cmn2Rev
 *           table if needed in their development
 *
 */
typedef struct {
  FCH_FUNC_SAMPLE              FchCmn2RevSample; ///< The Info function
  /* additional public functions here... */
} FCH_COMMON_2_REV_XFER_BLOCK;
