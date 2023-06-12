/**
 * @file  Utils.c
 * @brief OpenSIL utilities
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include <Utils.h>

/**
 * xUslDumpBuffer
 *
 * @brief This function will dump Size bytes of Buffer to the host supplied debug service routine.
 *
 * @param Buffer The buffer to dump
 * @param Size   The number of bytes in the buffer to dump
 */
void
xUslDumpBuffer (
  uint8_t     *Buffer,
  uint32_t    Size
)
{
  uint8_t   *Data8;
  uint32_t  Index;

  if (SIL_DEBUG_ENABLE) {
    XUSL_TRACEPOINT (SIL_TRACE_RAW, "-----------------------------------------------------------\n");

    XUSL_TRACEPOINT (SIL_TRACE_RAW, "0x00000000 ");
    Data8 = Buffer;
    for (Index = 0; Index < Size; Index++) {
      if ((Index != 0) && ((Index % 0x10) == 0)) {
        XUSL_TRACEPOINT (SIL_TRACE_RAW, "\n0x%08x ", Index);
      }
      XUSL_TRACEPOINT (SIL_TRACE_RAW, "%2x ", Data8[Index]);
    }
    XUSL_TRACEPOINT (SIL_TRACE_RAW, "\n");
    XUSL_TRACEPOINT (SIL_TRACE_RAW, "-----------------------------------------------------------\n");
  }
}

/**
 * @brief GetPointerToeSIMHOB - Get pointer to eSIM HOB object
 *
 * @return int
 */
/* SIL_STATUS GetPointerToeSIMHOB (eSIMHandOffBuffer* ptrToeSIMHandOffBuffer)
{
  SIL_STATUS Status;

  XUSL_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Status = SendIPCFromx86ToASP(SEND_eSIM_HOB_BASE_ADDRESS);
  if (Status != SilPass)
  {
    XUSL_TRACEPOINT(SIL_TRACE_ERROR, "GetPointerToeSIMHOB() function with a FATAL status\n");

    return Status;
  }

  // Read appropriate IPC mailbox registers and return back to the caller via the ptrToeSIMHandOffBuffer pointer
  ptrToeSIMHandOffBuffer = (eSIMHandOffBuffer*)0xC01DCAFE;

  XUSL_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return Status;
}
*/
/**
 * xUslLowBitSet32
 * @brief Returns the bit position of the lowest bit set in a 32-bit value.
 *        If Operand is zero, then -1 is returned.
 *
 * @param  Operand The 32-bit operand to evaluate.
 *
 * @retval 0..31  The lowest bit set in Operand was found.
 * @retval -1     Operand is zero.
 *
 */
int xUslLowBitSet32 (uint32_t Operand)
{
  int pos;
  if (Operand == 0) {
    return -1;
  }
  for (pos = 0; 0 == (Operand & 1); pos++, Operand >>= 1);
  return pos;
}

/**
 * xUslHighBitSet32
 * @brief Returns the bit position of the highest bit set in a 32-bit value.
 *
 * @details This function computes the bit position of the highest bit set in the 32-bit
 * value specified by Operand. If Operand is zero, then -1 is returned.
 * Otherwise, a value between 0 and 31 is returned.
 *
 * @param  Operand The 32-bit operand to evaluate.
 *
 * @retval 0..31  Position of the highest bit set in Operand if found.
 * @retval -1     Operand is zero.
 */
int xUslHighBitSet32 (uint32_t Operand)
{
  int pos;
  if (Operand == 0) {
    return -1;
  }
  for (pos = 31; (int)Operand > 0; pos--, Operand <<= 1);
  return pos;
}

/**
 * xUslBitWidth
 * @brief Returns the bit width of a given number.
 *
 * @details This function computes the bit width in value specified by Number.
 * bit width is always the 'width' from the left most '1' bit to bit '0'. It skips the 0's from leftmost set bit.
 *
 * @param  Number    number for which bitwidth is calculated.
 *
 * @return uint8_t   bit width if Number
 */
uint8_t xUslBitWidth (size_t Number)
{
  uint8_t Width;
  for (Width = 0; Number; Width++) {
    Number >>= 1;
  }
  return Width;
}

/**
 * xUslGetSetBitCount
 *
 * @brief This function returns the number of bits that are set in the input value.
 *
 * @retval uint32_t no. of set bits
 */
uint32_t xUslGetSetBitCount (uint32_t Value)
{
  uint32_t count = 0;
  while (Value) {
  Value &= (Value - 1);
    count++;
  }
  return count;
}
