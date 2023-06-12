/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  Utils.c
 * @brief OpenSIL utilities
 *
 */
/**
 * Copyright (c) 2006 - 2008, Intel Corporation.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <SilCommon.h>
#include <Utils.h>

/**
 * xUslDumpBuffer
 *
 * @brief This function will dump Size bytes of Buffer to the host supplied debug service routine.
 *
 * @param Buffer      The buffer to dump
 * @param Size        The number of bytes in the buffer to dump
 * @param DataWidth   Width of each element in the buffer (1 - Byte; 2 - Word; 3 - DWORD; 4 - QWORD)
 */
void
xUslDumpBuffer (
  void        *Buffer,
  uint32_t    Count,
  uint8_t     DataWidth
  )
{
  uint32_t Index;
  if (!SIL_DEBUG_ENABLE) {
    return;
  } else {
    assert(DataWidth >= 1 && DataWidth <= 4);

    XUSL_TRACEPOINT(SIL_TRACE_RAW, "-----------------------------------------------------------\n");
    XUSL_TRACEPOINT(SIL_TRACE_RAW, "0x00000000 ");

    for (Index = 0; Index < Count; Index += DataWidth) {
      if ((Index != 0) && ((Index + DataWidth - 1) % (16 * DataWidth) == 0)) {
        XUSL_TRACEPOINT(SIL_TRACE_RAW, "\n0x%08x ", Index);
      }
      switch (DataWidth) {
      case 1:
        XUSL_TRACEPOINT(SIL_TRACE_RAW, "%02x ", *((uint8_t *) Buffer + Index));
        break;
      case 2:
        XUSL_TRACEPOINT(SIL_TRACE_RAW, "%04x ", *(uint16_t *) ((uint8_t *) Buffer + Index));
        break;
      case 3:
        XUSL_TRACEPOINT(SIL_TRACE_RAW, "%08x ", *(uint32_t *) ((uint8_t *) Buffer + Index));
        break;
      case 4:
        XUSL_TRACEPOINT(SIL_TRACE_RAW, "%016llx", *(uint64_t *) ((uint8_t *) Buffer + Index));
        break;
      default:
        XUSL_TRACEPOINT(SIL_TRACE_RAW, "ERROR! Incorrect Data Width\n");
        return;
      }
    }
    XUSL_TRACEPOINT(SIL_TRACE_RAW, "\n");
    XUSL_TRACEPOINT(SIL_TRACE_RAW, "-----------------------------------------------------------\n");
  }
}

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
  for (pos = 0; 0 == (Operand & 1); pos++, Operand >>= 1) {;
  }
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
  for (pos = 31; (int)Operand > 0; pos--, Operand <<= 1) {;
  }
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

/**
 * xUslSwapBytes16
 *
 * @brief  Switches the endianness of a 16-bit unsigned integer.
 *
 * @details This function swaps the bytes in a 16-bit unsigned value to switch the value
 * from little endian to big endian or vice versa. The byte swapped value is
 * returned.
 *
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 * @param  Value A 16-bit unsigned value.
 *
 * @return The byte swapped Value.
 */
uint16_t
xUslSwapBytes16 (
  uint16_t                    Value
  )
{
  return (uint16_t) ((Value << 8) | (Value >> 8));
}

/**
 * xUslSwapBytes32
 *
 * @brief  Switches the endianness of a 32-bit unsigned integer.
 *
 * @details This function swaps the bytes in a 32-bit unsigned value to switch the value
 * from little endian to big endian or vice versa. The byte swapped value is
 * returned.
 *
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 * @param  Value A 32-bit unsigned value.
 *
 * @return The byte swapped Value.
 */
uint32_t
xUslSwapBytes32 (
  uint32_t                    Value
  )
{
  uint32_t  LowerBytes;
  uint32_t  HigherBytes;

  LowerBytes = (uint32_t) xUslSwapBytes16((uint16_t) Value);
  HigherBytes = (uint32_t) xUslSwapBytes16((uint16_t) (Value >> 16));
  return (LowerBytes << 16 | HigherBytes);
}

/**
 * xUslSwapBytes64
 *
 * @brief  Switches the endianness of a 64-bit unsigned integer.
 *
 * @details This function swaps the bytes in a 64-bit unsigned value to switch the value
 * from little endian to big endian or vice versa. The byte swapped value is
 * returned.
 *
 * SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 * @param  Value A 64-bit unsigned value.
 *
 * @return The byte swapped Value.
 */
uint64_t
xUslSwapBytes64 (
  uint64_t                    Value
  )
{
  uint64_t  LowerBytes;
  uint64_t  HigherBytes;

  LowerBytes = (uint64_t) xUslSwapBytes32((uint32_t) Value);
  HigherBytes = (uint64_t) xUslSwapBytes32((uint32_t) (Value >> 32));

  return (LowerBytes << 32 | HigherBytes);
}
