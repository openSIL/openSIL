/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  BrhAbstractRegAcc.c
 * @brief Abstract register access functionality and the Brh specific register data
 *        used in abstract register accesses on Brh
 *
 */

#include "BrhAbstractRegAcc.h"
#include <CpuLib.h>
#include <DF/Df.h>
#include <DF/DfX/DfXFabricRegisterAcc.h>
#include <DF/DfX/SilFabricRegistersDfX.h>

#define NUM_REG_TABLE_ENTRIES 16

static const RegStruct RegTable[NUM_REG_TABLE_ENTRIES] = {
  {SIL_RESERVED_0084, SIL_RESERVED_0087},
  {SIL_RESERVED_0090, SIL_RESERVED_0091},
  {CFG_LIMIT_ADDRESS0_FUNC, SIL_RESERVED_0027},
  {SIL_RESERVED_0030, SIL_RESERVED_0031},
  {SIL_RESERVED_0114, SIL_RESERVED_0116},
  {SIL_RESERVED_0118, SIL_RESERVED_0120},
  {SIL_RESERVED_0094, SIL_RESERVED_0097},
  {SIL_RESERVED_0101, SIL_RESERVED_0104},
  {SIL_RESERVED_0022, SIL_RESERVED_0023},
  {SIL_RESERVED_0054, SIL_RESERVED_0060},
  {SIL_RESERVED_0106, SIL_RESERVED_0112},
  {SIL_RESERVED_0032, SIL_RESERVED_0033},
  {SIL_RESERVED_0040, SIL_RESERVED_0041},
  {SIL_RESERVED_0078, SIL_RESERVED_0079},
  {SIL_RESERVED_0081, SIL_RESERVED_0082},
  {SIL_RESERVED_0066, SIL_RESERVED_0073}
};

static const FieldStruct FieldTable[NUM_FIELD_TABLE_ENTRIES] = {
  {SIL_RESERVED_0083, 0},
  {SIL_RESERVED_0086, 0},
  {SIL_RESERVED_0085, 0},
  {SIL_RESERVED_0089, 1},
  {SIL_RESERVED_0088, 1},
  {SIL_RESERVED_0026, 2},
  {SIL_RESERVED_0025, 2},
  {SIL_RESERVED_0029, 3},
  {SIL_RESERVED_0028, 3},
  {SIL_RESERVED_0113, 4},
  {SIL_RESERVED_0115, 4},
  {SIL_RESERVED_0117, 5},
  {SIL_RESERVED_0119, 5},
  {SIL_RESERVED_0096, 6},
  {SIL_RESERVED_0098, 6},
  {SIL_RESERVED_0092, 6},
  {SIL_RESERVED_0095, 6},
  {SIL_RESERVED_0093, 6},
  {SIL_RESERVED_0103, 7},
  {SIL_RESERVED_0105, 7},
  {SIL_RESERVED_0099, 7},
  {SIL_RESERVED_0102, 7},
  {SIL_RESERVED_0100, 7},
  {SIL_RESERVED_0021, 8},
  NOT_SUPPORTED,
  {SIL_RESERVED_0024, 8},
  {SIL_RESERVED_0058, 9},
  {SIL_RESERVED_0049, 9},
  {SIL_RESERVED_0062, 9},
  {SIL_RESERVED_0059, 9},
  {SIL_RESERVED_0056, 9},
  {SIL_RESERVED_0048, 9},
  {SIL_RESERVED_0051, 9},
  {SIL_RESERVED_0050, 9},
  {SIL_RESERVED_0061, 9},
  {SIL_RESERVED_0052, 9},
  {SIL_RESERVED_0053, 9},
  {SIL_RESERVED_0057, 9},
  {SIL_RESERVED_0055, 9},
  {SIL_RESERVED_0107, 10},
  {SIL_RESERVED_0110, 10},
  {SIL_RESERVED_0109, 10},
  {SIL_RESERVED_0111, 10},
  {SIL_RESERVED_0108, 10},
  {SIL_RESERVED_0034, 11},
  {SIL_RESERVED_0035, 11},
  {SIL_RESERVED_0036, 11},
  {SIL_RESERVED_0037, 11},
  {SIL_RESERVED_0038, 11},
  {SIL_RESERVED_0039, 11},
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  {SIL_RESERVED_0044, 12},
  {SIL_RESERVED_0045, 12},
  {SIL_RESERVED_0046, 12},
  {SIL_RESERVED_0047, 12},
  {SIL_RESERVED_0042, 12},
  {SIL_RESERVED_0043, 12},
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  {SIL_RESERVED_0077, 13},
  {SIL_RESERVED_0080, 14},
  {SIL_RESERVED_0063, 15},
  {SIL_RESERVED_0072, 15},
  {SIL_RESERVED_0076, 15},
  {SIL_RESERVED_0074, 15},
  {SIL_RESERVED_0075, 15},
  {SIL_RESERVED_0070, 15},
  {SIL_RESERVED_0071, 15},
  {SIL_RESERVED_0069, 15},
  {SIL_RESERVED_0067, 15},
  {SIL_RESERVED_0064, 15},
  {SIL_RESERVED_0068, 15},
  {SIL_RESERVED_0065, 15}
};

/**
 * MaskAndShift
 *
 * @brief Computes the shift given a mask then shifts Value by that amount,
 *        returning the new value
 */
static uint32_t
MaskAndShift (
  uint32_t Value,
  uint32_t Mask
  )
{
  uint32_t Shift;

  Value = Value & Mask;
  Shift = xUslCountTrailingZeroes(Mask);

  ABSTRACT_REG_DF_TRACEPOINT(SIL_TRACE_INFO,
    "Value: %x, Mask: %x, Shift: %x, Result: %x\n",
    Value,
    Mask,
    Shift,
    (Value >> Shift)
    );

  return (Value >> Shift);
}

/**
 * SetBits
 *
 * @brief Sets bits specified by Mask in Value to ValueToSet,
 *        returning the new value
 */
static uint32_t
SetBits (
  uint32_t Value,
  uint32_t ValueToSet,
  uint32_t Mask
  )
{
  uint32_t Shift;
  uint32_t i;

  // Clear the bits specified by mask
  i = ~Mask;
  i = Value & i;

  // Shift the value to set
  Shift = xUslCountTrailingZeroes(Mask);
  ValueToSet = ValueToSet << Shift;

  // Set bits
  i |= ValueToSet;

  ABSTRACT_REG_DF_TRACEPOINT(SIL_TRACE_INFO,
    "Value: %x, ValueToSet: %x, Mask: %x, Shift: %x, Result: %x\n",
    Value,
    ValueToSet,
    Mask,
    Shift,
    i
    );

  return i;
}

/**
 * GetOffsetVal
 *
 * @brief   Provides the offset needed for register field specified by Field
 *
 * @details Enables access to register blocks given the index of the specific
 *          register that wants to be accessed. This function only supports certain
 *          block accesses needed in OpenSIL and not all block accesses are available.
 *
 * @param[in]   Field      The register field to access, specified by an enum in SilFabricRegistersDfX.h
 * @param[in]   Index      The index for the offset calculations
 *
 * @retval      Register Offset Values
 */
static
uint32_t
GetOffsetVal (
  DF_FIELD_TABLE_ENTRIES   Field,
  uint32_t                 Index
  )
{
  uint32_t RetOffset = 0;

  /*
   * To add more register block access options, input the register field as a case into
   * the switch statement then assign the RetOffset with the offset calculation with the
   * index included.
   */
  switch (Field) {
  case CFG_LIMIT_ADDRESS0_DstFabricID:
  case CFG_LIMIT_ADDRESS0_BusNumLimit:
    RetOffset =
      SIL_RESERVED_0027 + Index * (SIL_RESERVED_0031 - SIL_RESERVED_0027);
    break;
  case MMIO_ADDRESS_CONTROL0_RE:
  case MMIO_ADDRESS_CONTROL0_WE:
  case MMIO_ADDRESS_CONTROL0_DstFabricID:
    RetOffset =
      SIL_RESERVED_0097 + Index * (SIL_RESERVED_0104 - SIL_RESERVED_0097);
    break;
  case X86_IOLIMIT_ADDRESS0_DstFabricID:
  case X86_IOLIMIT_ADDRESS0_IOLimit:
    RetOffset =
      SIL_RESERVED_0116 + Index * (SIL_RESERVED_0120 - SIL_RESERVED_0116);
    break;
  default:
    DF_TRACEPOINT(SIL_TRACE_ERROR, "Offset is not found.\n");
    assert(RetOffset != 0);
    break;
  }

  DF_TRACEPOINT(SIL_TRACE_INFO,
    "Abstract register offset value: %x\n",
    RetOffset
    );

  return RetOffset;
}

/**
 * BrhAbstractRegAcc
 *
 * @brief   Accesses the register field specified by Field with Brh specific values
 *          when called from the xfer table
 * @details Accesses a fabric register in a way which is abstract to the current program.
 *          Given parameter Field, the FieldTable defined above is indexed to determine the mask
 *          of the field (that is, which bits the field refers to), and a second value which is used
 *          to index the RegTable, which provides the Func and Reg values needed to access the register.
 *          The actual register reading and writing is handled by the DfXFabricRegisterAcc functions.
 *          Error checking is performed to ensure accesses to fields not present on a given program are
 *          attempted.
 *
 * @param[in]   Field       The register field to access, specified by an enum in SilFabricRegistersDfX.h
 * @param[in]   Socket      Processor socket to read from
 * @param[in]   OffsetIndex Offset Value, 0 for no offset
 * @param[in]   InstanceId  Instance ID of the target fabric device
 * @param[in]   Die         Number of Dies
 * @param[in]   Rw          Read or write: 0 => read, else write
 * @param[in]   WriteValue  Value to write (if writing)
 * @param[out]  ReadValue   Location to return read value (if reading)
 *
 * @retval SilPass unless unsupported register field specified in Field, then SilUnsupported
 */
SIL_STATUS
BrhAbstractRegAcc (
  uint8_t                  Field,
  uint32_t                 Socket,
  uint32_t                 OffsetIndex,
  uint32_t                 InstanceId,
  uint8_t                  Die,
  uint8_t                  Rw,
  uint32_t                 WriteValue,
  uint32_t                 *ReadValue
  )
{
  uint32_t      Value;
  uint32_t      OffsetValue;
  SIL_STATUS    Status;

  // Check that Field is within bounds of FieldTable
  if (Field >= NUM_FIELD_TABLE_ENTRIES) {
    DF_TRACEPOINT(SIL_TRACE_ERROR,
      "Field provided (%d) is invalid - out of bounds.\n",
      Field
      );

    Status = SilOutOfBounds;
  } else if (FieldTable[Field].Bits == 0) { // Field specified maps to NOT_SUPPORTED in the FieldTable,
    // A register not supported on current platform is being accessed
    DF_TRACEPOINT(SIL_TRACE_ERROR,
      "Attempted to access invalid register field (%d) for current program.\n",
      Field
      );

    Status = SilUnsupported;
  } else {
    OffsetValue = RegTable[FieldTable[Field].RegIndex].Reg;

    if (OffsetIndex != 0) {
      OffsetValue = GetOffsetVal(Field, OffsetIndex);
    }

    Value = DfXFabricRegisterAccRead(Socket,
      Die,
      RegTable[FieldTable[Field].RegIndex].Func,
      OffsetValue,
      InstanceId
      );

    if (Rw == 0) { // Rw == 0 indicates a read, Rw == 1 a write
      Value = MaskAndShift(Value, FieldTable[Field].Bits);
      ABSTRACT_REG_DF_TRACEPOINT(SIL_TRACE_INFO,
        "Abstract register read: read %x from func %x reg %x mask %x.\n",
        Value,
        RegTable[FieldTable[Field].RegIndex].Func,
        RegTable[FieldTable[Field].RegIndex].Reg,
        FieldTable[Field].Bits
        );

      *ReadValue = Value;
      Status = SilPass;
    } else {
      ABSTRACT_REG_DF_TRACEPOINT(SIL_TRACE_INFO,
        "Abstract register write: read %x from func %x reg %x mask %x.\n",
        Value,
        RegTable[FieldTable[Field].RegIndex].Func,
        RegTable[FieldTable[Field].RegIndex].Reg,
        FieldTable[Field].Bits
        );

      Value = SetBits(Value, WriteValue, FieldTable[Field].Bits);

      ABSTRACT_REG_DF_TRACEPOINT(SIL_TRACE_INFO,
        "Abstract register write: now writing back %x (%x was set).\n",
        Value,
        WriteValue
        );

      DfXFabricRegisterAccWrite(Socket,
        Die,
        RegTable[FieldTable[Field].RegIndex].Func,
        OffsetValue,
        InstanceId,
        Value
        );

      Status = SilPass;
    }
  }

  return Status;
}
