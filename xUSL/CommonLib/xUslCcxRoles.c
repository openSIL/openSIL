/**
 * @file xUslCcxRoles.c
 * @brief core complex helper functions.
 *
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include "xUslCcxRoles.h"
#include "MsrReg.h"
#include <CommonLib/CpuLib.h>

/**
 * xUslIsBsp
 * @brief Is this the Bootstrap Processor ?
 *
 *
 *  @retval  true      if it is BSP core
 *  @retval  false     if not BSP Core
 *
 */
bool xUslIsBsp (void) {
  size_t ApicBar;

  ApicBar = (size_t) xUslRdMsr (MSR_APIC_BAR);
  return (bool) ((ApicBar & BIT_32(8)) != 0);
}

/**
 * xUslIsComputeUnitPrimary
 * @brief Is the current core a primary core of it's compute unit?
 *
 *
 *  @retval  true       if it is Primary Core
 *  @retval  false      if it is not Primary Core
 *
 */
bool xUslIsComputeUnitPrimary (void) {
  uint32_t ThreadsPerCore;

  ThreadsPerCore = xUslGetThreadsPerCore ();
  if (ThreadsPerCore == 0) {
    ThreadsPerCore = 1;
  }
  return ((xUslGetInitialApicId () % ThreadsPerCore) == 0);
}

/**
 * xUslIsCoreComplexPrimary
 * @brief Is the current core a primary core of its complex?
 *
 *
 *  @retval        true     if it is Primary Core
 *  @retval        false    if it is not Primary Core
 *
 */
bool xUslIsCoreComplexPrimary (void) {
  uint32_t ComplexMask;

  ComplexMask = (xUslGetThreadsPerCore () == 1) ? 0x00000007 : 0x0000000F;
  return ((xUslGetInitialApicId () & ComplexMask) == 0);
}

/**
 * xUslIsDiePrimary
 * @brief Is the current core a primary core of it's CCD?
 *
 *
 *  @retval    true         if it is Primary Core
 *  @retval    false        if it is not Primary Core
 *
 */
bool xUslIsDiePrimary (void) {
  return xUslIsSocketPrimary ();
}

/**
 * xUslIsSocketPrimary
 * @brief Is the current core a primary core of it's socket?
 *
 *
 *  @retval    true         if it is Primary Core
 *  @retval    false        if it is not Primary Core
 *
 */
bool xUslIsSocketPrimary (void) {
  uint32_t SocketMask;

  SocketMask = (xUslGetThreadsPerCore () == 1) ? 0x0000003F : 0x0000007F;
  return ((xUslGetInitialApicId () & SocketMask) == 0);
}
