/**
 * @file CpuOps.c
 * @brief Defines cpu specific common operations
 *
 */

/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <CpuLib.h>

/**
 * xUslMsrOr
 * @brief Reads a 64-bit MSR, performs a bitwise OR, and writes the result
 * back to the 64-bit MSR.
 *
 * @details Reads the 64-bit MSR specified by Index, performs a bitwise OR
 * between the read result and the value specified by OrData, and writes the
 * result to the 64-bit MSR specified by Index.
 *
 * @param  Index   The 32-bit MSR index to write.
 * @param  OrData  The value to OR with the read value from the MSR.
 *
 * @return None
 *
 **/
void xUslMsrOr (uint32_t Index, uint64_t OrData)
{
  xUslWrMsr (Index, xUslRdMsr (Index) | OrData);
}

/**
 * xUslMsrAnd
 * @brief Reads a 64-bit MSR, performs a bitwise AND, and writes the result back to the
 * 64-bit MSR.
 *
 * @details Reads the 64-bit MSR specified by Index, performs a bitwise AND between the
 * read result and writes the result to the 64-bit MSR specified by Index.
 *
 * @param  Index   The 32-bit MSR index to write.
 * @param  AndData The value to AND with the read value from the MSR.
 *
 * @return None
 *
 **/
void xUslMsrAnd (uint32_t Index, uint64_t AndData)
{
  xUslWrMsr (Index, xUslRdMsr (Index) & AndData);
}

/**
 * xUslMsrAndThenOr
 * @brief Reads a 64-bit MSR, performs a bitwise AND followed by a bitwise
 * OR, and writes the result back to the 64-bit MSR.
 *
 * @details Reads the 64-bit MSR specified by Index, performs a bitwise AND
 * between read result and the value specified by AndData, performs a bitwise OR
 * between the result of the AND operation and the value specified by OrData,
 * and writes the result to the 64-bit MSR specified by Index.
 *
 * @param  Index   The 32-bit MSR index to write.
 * @param  AndData The value to AND with the read value from the MSR.
 * @param  OrData  The value to OR with the result of the AND operation.
 *
 * @return None.
 *
 **/
void
xUslMsrAndThenOr (
  uint32_t Index,
  uint64_t AndData,
  uint64_t OrData
  )
{
  xUslWrMsr (Index, (xUslRdMsr (Index) & AndData) | OrData);
}
