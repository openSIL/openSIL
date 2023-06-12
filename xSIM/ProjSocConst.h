/**
 * @file ProjSocConst.h
 * @brief Project summary of SoC characteristic constants
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include "SoC/F19M10/F19M10Charz.h"

/* **********
   Clear the common constants
*************/
/* SoC construction constants */
#define PROJ_MAX_SOCKETS_SUPPORTED   0   ///< Max sockets supported in a system
#define PROJ_MAX_CCD_DIES_PER_SOCKET 0   ///< Max number of CCX dies per socket
#define PROJ_MAX_MEM_DIES_PER_SOCKET 0   ///< Max number of MEM dies per socket
#define PROJ_MAX_IO_DIES_PER_SOCKET  0   ///< Max number of IO dies per socket

/* Processor Cores sub-system constants */
#define PROJ_MAX_COMPLEXES_PER_CCD   0   ///< Program dependent
#define PROJ_MAX_CCX_CORES_PER_COMPLEX  0
#define PROJ_MAX_CCX_THREADS_PER_CORE   0

/* Memory sub-system constants */
#define PROJ_MAX_UMCS_PER_MEM_DIE  0  ///< Max Channels per die
#define PROJ_MAX_CHANNELS_PER_UMC  0  ///< Max Channels per sockets
#define PROJ_MAX_DIMMS_PER_CHANNEL 0  ///< Max DIMMs on a memory channel (independent of platform)

/* IO Hub sub-system constants */
#define PROJ_MAX_RBS_PER_SOCKET    0  ///< Max number of root bridges per socket

/* **********
   Optional: verify that the values got set
*************/
#if (PROJ_MAX_SOCKETS_SUPPORTED     == 0)
  #error SoC failed to define PROJ_MAX_SOCKETS_SUPPORTED
#endif
#if (PROJ_MAX_CCD_DIES_PER_SOCKET   == 0)
  #error SoC failed to define PROJ_MAX_CCD_DIES_PER_SOCKET
#endif
#if (PROJ_MAX_MEM_DIES_PER_SOCKET   == 0)
  #error SoC failed to define PROJ_MAX_MEM_DIES_PER_SOCKET
#endif
#if (PROJ_MAX_IO_DIES_PER_SOCKET    == 0)
  #error SoC failed to define PROJ_MAX_IO_DIES_PER_SOCKET
#endif
#if (PROJ_MAX_COMPLEXES_PER_CCD     == 0)
  #error SoC failed to define PROJ_MAX_COMPLEXES_PER_CCD
#endif
#if (PROJ_MAX_CCX_CORES_PER_COMPLEX == 0)
  #error SoC failed to define PROJ_MAX_CCX_CORES_PER_COMPLEX
#endif
#if (PROJ_MAX_CCX_THREADS_PER_CORE  == 0)
  #error SoC failed to define PROJ_MAX_CCX_THREADS_PER_CORE
#endif
#if (PROJ_MAX_UMCS_PER_MEM_DIE      == 0)
  #error SoC failed to define PROJ_MAX_UMCS_PER_MEM_DIE
#endif
#if (PROJ_MAX_CHANNELS_PER_UMC      == 0)
  #error SoC failed to define PROJ_MAX_CHANNELS_PER_UMC
#endif
#if (PROJ_MAX_DIMMS_PER_CHANNEL     == 0)
  #error SoC failed to define PROJ_MAX_DIMMS_PER_CHANNEL
#endif
#if (PROJ_MAX_RBS_PER_SOCKET        == 0)
  #error SoC failed to define PROJ_MAX_RBS_PER_SOCKET
#endif
