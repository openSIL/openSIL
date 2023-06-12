/**
 * @file  CoreTechnologyService.c
 * @brief CoreTechnology service definition.
 *
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include <ApobCmn.h>

#include "CoreTopologyService.h"

/**
 * GetCoreTopologyOnDie
 *
 * @brief   This retrieves information about the layout of the cores on the given die.
 *
 * @param   Socket            Zero-based socket number to check.
 * @param   Die               The target die's identifier within Socket.
 * @param   NumberOfCcds      Pointer to the number of enabled core complex
 *                            dies on a given socket.
 * @param   NumberOfComplexes Pointer to the number of enabled complexes on
 *                            the given socket / die.
 * @param   NumberOfCores     Pointer to the number of enabled cores per
 *                            complex on the given socket / die.
 * @param   NumberOfThreads   Pointer to the number of enabled threads per
 *                            core on the given socket / die.
 *
 * @retval SilPass              The core topology information was successfully retrieved.
 * @retval SilInvalidParameter  Socket is non-existent.
 * @retval SilInvalidParameter  Die is non-existent.
 * @retval SilInvalidParameter  All output parameter pointers are NULL.
 *
 **/
SIL_STATUS
GetCoreTopologyOnDie (
  uint32_t Socket,
  uint32_t Die,
  uint32_t *NumberOfCcds,
  uint32_t *NumberOfComplexes,
  uint32_t *NumberOfCores,
  uint32_t *NumberOfThreads
  )
{
  return ApobGetCoreTopologyOnDie(Socket, Die, NumberOfCcds, NumberOfComplexes,
     NumberOfCores, NumberOfThreads);
}
