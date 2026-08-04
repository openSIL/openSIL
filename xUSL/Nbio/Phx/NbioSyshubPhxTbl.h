/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  NbioNbifPhxTbl.h
 * @brief This file contains the Nbif device configurations
 *
 */

#pragma once

#include <includePHX/PHX_NBIFMM.h>
#include <includePHX/PHX_NBIFEPFCFG.h>
#include <includePHX/PHX_PCIERCCFG.h>
#include <includePHX/PHX_SYSHUBMM.h>

  #define NBIO_SYSHUB_NGDC_MGCG_CTR_CLOCK_GATING_TBL \
    SMN_ENTRY_RMW ( \
      SYSHUBMMx1403ba8, \
      (0x1 << 0), \
      (0x1 << 0) \
      ),

  #define NBIO_SYSHUB_MGCG_CTRL_HSP_CLOCK_GATING_TBL \
    SMN_ENTRY_RMW ( \
      SYSHUBMMx1412020, \
      (0x1 << 0), \
      (0x1 << 0) \
      ),
