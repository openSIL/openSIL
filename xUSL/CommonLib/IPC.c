/**
 * @file  IPC.c
 * @brief OpenSIL IPC functions
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include <IPC.h>

/**
 * @brief SendIPCFromx86ToASP - Send IPC From x86 CPU to ASP µC
 * @param IPCCommand
 * @return int
 */
SIL_STATUS SendIPCFromx86ToASP (int IPCCommand)
{
  SIL_STATUS Status = SilPass;
  // Logic to send IPC and wait for IPC to complete goes here
  // Adequate hooks must be in place to allow for appropriate timeouts and must be reported as a Timeout error in such
  // a case.

  XUSL_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
  ///< @todo : below is temporary fake use of formal parameters to avoid MS compiler warning C4100: unreferenced formal parameter
  if (IPCCommand == 0) return SilAborted;
  XUSL_TRACEPOINT (SIL_TRACE_EXIT, "\n");

  return Status;
}

