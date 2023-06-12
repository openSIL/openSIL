/**
 * @file Sp5Charz.h
 * @brief Socket 'SP5' characteristic constants
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once



/* Socket pinnout constants */
#ifdef    PROJ_SKT_MEM_CHANNELS
  #undef  PROJ_SKT_MEM_CHANNELS
#endif
#define   PROJ_SKT_MEM_CHANNELS      12   // Num channels available

#ifdef    PROJ_SKT_PCIE_LANES
  #undef  PROJ_SKT_PCIE_LANES
#endif
#define   PROJ_SKT_PCIE_LANES         0   // Num lanes available

#ifdef    PROJ_SKT_USB2_PORTS
  #undef  PROJ_SKT_USB2_PORTS
#endif
#define   PROJ_SKT_USB2_PORTS         0   // Num pinned out ports




