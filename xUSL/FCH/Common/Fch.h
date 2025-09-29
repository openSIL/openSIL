/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file Fch.h
 * @brief FCH Initialization data
 *
 */

#pragma once

#include <Sil-api.h>

#define FCH_TRACEPOINT(MsgLevel, Message, ...)        \
        do {                \
          if (DEBUG_FILTER_FCH & SIL_DEBUG_MODULE_FILTER) {    \
            XUSL_TRACEPOINT(MsgLevel, Message, ## __VA_ARGS__);  \
          } \
        } while (0)

#define FCHCLASS_MAJOR_REV 0
#define FCHCLASS_MINOR_REV 1
#define FCHCLASS_INSTANCE  0

/**
 * Establish FCH input defaults
 *
 * This function is called by the xSIM core to populate the IP's input
 * block with its default values. Each block established by the IP must
 * have a unique header.ID and its header.size set to the single block
 * only - so as to allow the FindStruct() to traverse the block chain.
 *
 * This is an IP private function, not visible to the Host
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @return  SIL_STATUS
 * @retval  SilPass - everything is OK
 * @retval  SilAbort - Something went wrong
 */

SIL_STATUS
FchClassSetInputBlk (
  SIL_CONTEXT  *SilContext
  );
