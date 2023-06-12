/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file ProjSocConst.h
 * @brief Project summary of SoC characteristic constants
 */

#pragma once

/*
 * These Max values are used to define array sizes and associated loop
 * counts in the code. They reflect the maximum values that openSIL
 * currently supports (based on enabled SoCs) and does not necessarily
 * reflect the hardware capabilities or configuration.
 */

/* **********
   Clear the common constants
 *************/
/* SoC construction constants */
#define PROJ_MAX_SOCKETS_SUPPORTED       0   ///< Max sockets supported in a system
#define PROJ_MAX_CCD_DIES_PER_SOCKET     0   ///< Max number of CCX dies per socket
#define PROJ_MAX_MEM_DIES_PER_SOCKET     0   ///< Max number of MEM dies per socket
#define PROJ_MAX_IO_DIES_PER_SOCKET      0   ///< Max number of IO dies per socket

/* Processor Cores sub-system constants */
#define PROJ_MAX_COMPLEXES_PER_CCD       0   ///< Program dependent
#define PROJ_MAX_CCX_CORES_PER_COMPLEX   0
#define PROJ_MAX_CCX_THREADS_PER_CORE    0
#define PROJ_CCX_MAX_DIES_PER_SOCKET     0   ///< Max dies per socket

/* Memory sub-system constants */
#define PROJ_MAX_UMCS_PER_MEM_DIE        0   ///< Max Channels per die
#define PROJ_MAX_CHANNELS_PER_UMC        0   ///< Max Channels per sockets
#define PROJ_MAX_DIMMS_PER_CHANNEL       0   ///< Max DIMMs on a memory channel (independent of platform)

/* IO Hub sub-system constants */
#define PROJ_MAX_RBS_PER_SOCKET          0  ///< Max number of root bridges per socket
#define PROJ_MAX_HOST_BRIDGES_PER_SOCKET 0  ///< Max number of host bridges per socket

/* Data Fabric sub-system constants */
#define PROJ_NUM_CS_CMP_BLOCKS           0  ///< Number of CMP data fabric components
#define PROJ_NUM_CS_UMC_BLOCKS           0  ///< Number of UMC data fabric components


/* **********
   Now call each included SoC to find the max value
 *************/
#if defined (CONFIG_SOC_F1AM00) && CONFIG_SOC_F1AM00
  #include "SoC/F1AM00/F1AM00Charz.h"
#endif

//... Add new SoCs here...

/* **********
   Now the common constants contain the max value
 *************/

/* **********
   Optional: verify that the values got set
 *************/
#if (PROJ_MAX_SOCKETS_SUPPORTED == 0)
  #error SoC failed to define PROJ_MAX_SOCKETS_SUPPORTED
#endif
#if (PROJ_MAX_CCD_DIES_PER_SOCKET == 0)
  #error SoC failed to define PROJ_MAX_CCD_DIES_PER_SOCKET
#endif
#if (PROJ_MAX_MEM_DIES_PER_SOCKET == 0)
  #error SoC failed to define PROJ_MAX_MEM_DIES_PER_SOCKET
#endif
#if (PROJ_MAX_IO_DIES_PER_SOCKET == 0)
  #error SoC failed to define PROJ_MAX_IO_DIES_PER_SOCKET
#endif
#if (PROJ_MAX_COMPLEXES_PER_CCD == 0)
  #error SoC failed to define PROJ_MAX_COMPLEXES_PER_CCD
#endif
#if (PROJ_MAX_CCX_CORES_PER_COMPLEX == 0)
  #error SoC failed to define PROJ_MAX_CCX_CORES_PER_COMPLEX
#endif
#if (PROJ_MAX_CCX_THREADS_PER_CORE == 0)
  #error SoC failed to define PROJ_MAX_CCX_THREADS_PER_CORE
#endif
#if (PROJ_CCX_MAX_DIES_PER_SOCKET == 0)
  #error SoC failed to define PROJ_CCX_MAX_DIES_PER_SOCKET
#endif
#if (PROJ_MAX_UMCS_PER_MEM_DIE == 0)
  #error SoC failed to define PROJ_MAX_UMCS_PER_MEM_DIE
#endif
#if (PROJ_MAX_CHANNELS_PER_UMC == 0)
  #error SoC failed to define PROJ_MAX_CHANNELS_PER_UMC
#endif
#if (PROJ_MAX_DIMMS_PER_CHANNEL == 0)
  #error SoC failed to define PROJ_MAX_DIMMS_PER_CHANNEL
#endif
#if (PROJ_MAX_RBS_PER_SOCKET == 0)
  #error SoC failed to define PROJ_MAX_RBS_PER_SOCKET
#endif
#if (PROJ_MAX_HOST_BRIDGES_PER_SOCKET == 0)
  #error SoC failed to define PROJ_MAX_HOST_BRIDGES_PER_SOCKET
#endif
