/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <xSIM.h>

static void testHostDebugService(size_t MsgLevel,
                                 const char *SilPrefix,
                                 const char *Message,
                                 const char *Function,
                                 size_t Line,
                                 ...)
{
  UNUSED (MsgLevel);
  UNUSED (SilPrefix);
  UNUSED (Message);
  UNUSED (Function);
  UNUSED (Line);
  printf("%s\n", Message);
}

int main (void)
{
  size_t MemReq = xSimQueryMemoryRequirements();
  void *MemBuf = malloc(MemReq);
  xSimAssignMemoryTp1(MemBuf, MemReq);
  SilDebugSetup(testHostDebugService);
  InitializeSiTp1();
  return 0;
}
