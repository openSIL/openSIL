/**
 * @file ProjSktConst.h
 * @brief Project summary of Socket characteristic constants
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

/* **********
   Clear the common constants
*************/
/* Socket pinnout constants */
#define PROJ_SKT_SOCKETS            1   // Num sockets permitted by pinnout
#define PROJ_SKT_MEM_CHANNELS       0   // Num channels available
#define PROJ_SKT_PCIE_LANES         0   // Num lanes available
#define PROJ_SKT_USB2_PORTS         0   // Num pinned out ports

/* **********
   Now call each Socket to find the right one for this project
*************/
#if defined(CONFIG__SKT_TYPE_SP5) && CONFIG__SKT_TYPE_SP5
  #include "Skt/Sp5Charz.h"
#endif

//... Add new Sockets here...

/* **********
   Now the common constants contain the correct value for this platform
*************/
