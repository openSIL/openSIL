/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 * @file  Utils.c
 * @brief OpenSIL utilities
 */

#include <SilCommon.h>
#include <Utils.h>

/**
 *  @brief Line width for xUslDumpBuffer function.
 *  @details This macro defines the number of data items printed per line by the xUslDumpBuffer function.
 */
#define LINE_WIDTH    16

/**
 *  @brief Enumerator to define the number of bytes in each data type.
 *  @details This enumerator is used to increment the buffer pointer in xUslDumpBuffer function, according to the
 *           specified data width.
 */
typedef enum {
  BYTE_SIZE = 1,
  WORD_SIZE = 2,
  DWORD_SIZE = 4,
  QWORD_SIZE = 8,
} DATAWIDTH_INCREMENT;

/**
 * xUslDumpBuffer
 *
 * @brief This function will dump Size bytes of Buffer to the host supplied debug service routine.
 *
 * @param Buffer      The buffer to dump
 * @param Count       The count of data elements
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
  uint8_t  Increment;

  if (!SIL_DEBUG_ENABLE) {
    return;
  } else {
    assert(DataWidth >= 1 && DataWidth <= 4);

    if (DataWidth == 3) {
      Increment = DWORD_SIZE;
    } else if (DataWidth == 4) {
      Increment = QWORD_SIZE;
    } else {
      Increment = DataWidth;
    }

    XUSL_TRACEPOINT(SIL_TRACE_RAW, "-----------------------------------------------------------\n");
    XUSL_TRACEPOINT(SIL_TRACE_RAW, "0x00000000 ");

    for (Index = 0; Index < Count; Index += Increment) {
      if ((Index != 0) && ((Index) % (LINE_WIDTH * Increment) == 0)) {
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
        XUSL_TRACEPOINT(SIL_TRACE_RAW, "%016llx ", *(uint64_t *) ((uint8_t *) Buffer + Index));
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
