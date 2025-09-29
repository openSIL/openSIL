/* SPDX-License-Identifier: MIT */
/**
 * Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 * @file  IPC.c
 * @brief OpenSIL IPC functions
 *
 */

#include <SilCommon.h>
#include <IPC.h>

/**
 * @brief SendIPCFromx86ToASP - Send IPC From x86 CPU to ASP microcontroller
 * @param IPCCommand
 * @return int
 */
SIL_STATUS SendIPCFromx86ToASP (int IPCCommand)
{
  SIL_STATUS Status = SilPass;
  // Logic to send IPC and wait for IPC to complete.
  // Adequate hooks must be in place to allow for appropriate timeouts and must be reported as a Timeout error in such
  // a case.

  XUSL_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  UNUSED(IPCCommand);
  XUSL_TRACEPOINT(SIL_TRACE_EXIT, "\n");

  return Status;
}
