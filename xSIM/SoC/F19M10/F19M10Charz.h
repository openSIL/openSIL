/**
 * @file F19M10Charz.h
 * @brief SoC characteristics constants
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once



/* SoC construction constants */
#if      (PROJ_MAX_SOCKETS_SUPPORTED    < 2)
  #undef  PROJ_MAX_SOCKETS_SUPPORTED
  #define PROJ_MAX_SOCKETS_SUPPORTED      2
#endif
#if      (PROJ_MAX_CCD_DIES_PER_SOCKET    < 12)
  #undef  PROJ_MAX_CCD_DIES_PER_SOCKET
  #define PROJ_MAX_CCD_DIES_PER_SOCKET      12
#endif
#if      (PROJ_MAX_MEM_DIES_PER_SOCKET    < 1)
  #undef  PROJ_MAX_MEM_DIES_PER_SOCKET
  #define PROJ_MAX_MEM_DIES_PER_SOCKET      1
#endif
#if      (PROJ_MAX_IO_DIES_PER_SOCKET    < 1)
  #undef  PROJ_MAX_IO_DIES_PER_SOCKET
  #define PROJ_MAX_IO_DIES_PER_SOCKET      1
#endif

/* Processor Cores sub-system constants */
#if      (PROJ_MAX_COMPLEXES_PER_CCD    < 1)
  #undef  PROJ_MAX_COMPLEXES_PER_CCD
  #define PROJ_MAX_COMPLEXES_PER_CCD      1
#endif
#if      (PROJ_MAX_CCX_CORES_PER_COMPLEX    < 8)
  #undef  PROJ_MAX_CCX_CORES_PER_COMPLEX
  #define PROJ_MAX_CCX_CORES_PER_COMPLEX      8
#endif
#if      (PROJ_MAX_CCX_THREADS_PER_CORE    < 2)
  #undef  PROJ_MAX_CCX_THREADS_PER_CORE
  #define PROJ_MAX_CCX_THREADS_PER_CORE      2
#endif

/* Memory sub-system constants */
#if      (PROJ_MAX_UMCS_PER_MEM_DIE    < 12)
  #undef  PROJ_MAX_UMCS_PER_MEM_DIE
  #define PROJ_MAX_UMCS_PER_MEM_DIE      12
#endif
#if      (PROJ_MAX_CHANNELS_PER_UMC    < 1)
  #undef  PROJ_MAX_CHANNELS_PER_UMC
  #define PROJ_MAX_CHANNELS_PER_UMC      1
#endif
#if      (PROJ_MAX_DIMMS_PER_CHANNEL    < 2)
  #undef  PROJ_MAX_DIMMS_PER_CHANNEL
  #define PROJ_MAX_DIMMS_PER_CHANNEL      1
#endif

/* IO Hub sub-system constants */
#if      (PROJ_MAX_RBS_PER_SOCKET    < 12)
  #undef  PROJ_MAX_RBS_PER_SOCKET
  #define PROJ_MAX_RBS_PER_SOCKET      12
#endif





