/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2026 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  linktest.c
 * @brief Link test for the openSIL libraries.
 *
 * Meson builds this file as pseudo_host when the unit_test option is enabled.
 * Linking it confirms that the host entry points for all three timepoints, and
 * everything they reach, resolve in the combined library. It is a build check
 * only: running it calls into silicon initialization, which accesses hardware.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <xSIM.h>

/**
 * TestHostDebugService
 *
 * @brief  Minimal host debug service that prints each openSIL message.
 *
 * @param[in]  MsgLevel   Message level
 * @param[in]  SilPrefix  openSIL module prefix
 * @param[in]  Message    Message text
 * @param[in]  Function   Calling function name
 * @param[in]  Line       Calling line number
 */
static void
TestHostDebugService (
  size_t      MsgLevel,
  const char  *SilPrefix,
  const char  *Message,
  const char  *Function,
  size_t      Line,
  ...
  )
{
  UNUSED(MsgLevel);
  UNUSED(SilPrefix);
  UNUSED(Function);
  UNUSED(Line);
  printf("%s\n", Message);
}

int
main (void)
{
  size_t  MemReq;
  void    *MemBuf;
  int     Result;

  MemReq = xSimQueryMemoryRequirements ();
  MemBuf = malloc(MemReq);
  if (MemBuf == NULL) {
    return EXIT_FAILURE;
  }

  Result = EXIT_FAILURE;
  if ((xSimAssignMemoryTp1(MemBuf, MemReq) == SilPass) &&
    (SilDebugSetup(TestHostDebugService) == SilPass) &&
    (InitializeAMDSiTp1 () == SilPass) &&
    (xSimAssignMemoryTp2(MemBuf, MemReq) == SilPass) &&
    (InitializeAMDSiTp2 () == SilPass) &&
    (xSimAssignMemoryTp3(MemBuf, MemReq) == SilPass) &&
    (InitializeAMDSiTp3 () == SilPass)) {
    Result = EXIT_SUCCESS;
  }

  free(MemBuf);
  return Result;
}
