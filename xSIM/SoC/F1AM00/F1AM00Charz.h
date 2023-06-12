/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file F1AM00Charz.h
 * @brief SoC characteristics constants
 */

#pragma once

/* SoC construction constants */
#if      (PROJ_MAX_SOCKETS_SUPPORTED < 2)
  #undef  PROJ_MAX_SOCKETS_SUPPORTED
  #define PROJ_MAX_SOCKETS_SUPPORTED      2
#endif
#if      (PROJ_MAX_CCD_DIES_PER_SOCKET < 16)
  #undef  PROJ_MAX_CCD_DIES_PER_SOCKET
  #define PROJ_MAX_CCD_DIES_PER_SOCKET      16
#endif
#if      (PROJ_MAX_MEM_DIES_PER_SOCKET < 1)
  #undef  PROJ_MAX_MEM_DIES_PER_SOCKET
  #define PROJ_MAX_MEM_DIES_PER_SOCKET      1
#endif
#if      (PROJ_MAX_IO_DIES_PER_SOCKET < 1)
  #undef  PROJ_MAX_IO_DIES_PER_SOCKET
  #define PROJ_MAX_IO_DIES_PER_SOCKET      1
#endif

/* Processor Cores sub-system constants */
#if      (PROJ_MAX_COMPLEXES_PER_CCD < 1)
  #undef  PROJ_MAX_COMPLEXES_PER_CCD
  #define PROJ_MAX_COMPLEXES_PER_CCD      1
#endif
#if      (PROJ_MAX_CCX_CORES_PER_COMPLEX < 16)
  #undef  PROJ_MAX_CCX_CORES_PER_COMPLEX
  #define PROJ_MAX_CCX_CORES_PER_COMPLEX      16
#endif
#if      (PROJ_MAX_CCX_THREADS_PER_CORE < 2)
  #undef  PROJ_MAX_CCX_THREADS_PER_CORE
  #define PROJ_MAX_CCX_THREADS_PER_CORE      2
#endif
#if      (PROJ_CCX_MAX_DIES_PER_SOCKET < 1)
  #undef  PROJ_CCX_MAX_DIES_PER_SOCKET
  #define PROJ_CCX_MAX_DIES_PER_SOCKET      1
#endif

/* Memory sub-system constants */
#if      (PROJ_MAX_UMCS_PER_MEM_DIE < 12)
  #undef  PROJ_MAX_UMCS_PER_MEM_DIE
  #define PROJ_MAX_UMCS_PER_MEM_DIE      12
#endif
#if      (PROJ_MAX_CHANNELS_PER_UMC < 1)
  #undef  PROJ_MAX_CHANNELS_PER_UMC
  #define PROJ_MAX_CHANNELS_PER_UMC      1
#endif
#if      (PROJ_MAX_DIMMS_PER_CHANNEL < 2)
  #undef  PROJ_MAX_DIMMS_PER_CHANNEL
  #define PROJ_MAX_DIMMS_PER_CHANNEL      2
#endif

/* IO Hub sub-system constants */
#if      (PROJ_MAX_RBS_PER_SOCKET < 8)
  #undef  PROJ_MAX_RBS_PER_SOCKET
  #define PROJ_MAX_RBS_PER_SOCKET      8
#endif
#if      (PROJ_MAX_HOST_BRIDGES_PER_SOCKET < 8)
  #undef  PROJ_MAX_HOST_BRIDGES_PER_SOCKET
  #define PROJ_MAX_HOST_BRIDGES_PER_SOCKET      8
#endif

/* Data Fabric sub-system constants */
#if      (PROJ_NUM_CS_CMP_BLOCKS < 4)
  #undef  PROJ_NUM_CS_CMP_BLOCKS
  #define PROJ_NUM_CS_CMP_BLOCKS        4
#endif
#if      (PROJ_NUM_CS_UMC_BLOCKS < 12)
  #undef  PROJ_NUM_CS_UMC_BLOCKS
  #define PROJ_NUM_CS_UMC_BLOCKS        12
#endif
