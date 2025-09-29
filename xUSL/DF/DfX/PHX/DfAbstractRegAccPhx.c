/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  DfAbstractRegAccPhx.c
 * @brief Abstract register access functionality and the Phx specific register data
 *        used in abstract register accesses on Phx
 *
 */

#include <SilCommon.h>
#include "DfAbstractRegAccPhx.h"
#include <CpuLib.h>
#include <DF/Df.h>
#include <DF/DfX/DfXFabricRegisterAcc.h>
#include <DF/DfX/SilFabricRegistersDfX.h>

#define NUM_REG_TABLE_ENTRIES 13

static const RegStruct RegTable[NUM_REG_TABLE_ENTRIES] = {
  {DF_FUNC0, DFABSTRACTx40},
  {DF_FUNC0, DFABSTRACTx50},
  {DF_FUNC0, DFABSTRACTxC84},
  {DF_FUNC0, DFABSTRACTxC8C},
  {DF_FUNC0, DFABSTRACTxD04},
  {DF_FUNC0, DFABSTRACTxD0C},
  {DF_FUNC0, DFABSTRACTxD88},
  {DF_FUNC0, DFABSTRACTxD98},
  {DF_FUNC3, DFABSTRACTx44},
  {DF_FUNC4, DFABSTRACTx180},
  {DF_FUNC7, DFABSTRACTx200},
  {DF_FUNC7, DFABSTRACTx204},
  {DF_FUNC7, DFABSTRACTx208}
};

static const FieldStruct FieldTable[NUM_FIELD_TABLE_ENTRIES] = {
  {BITS_0_TO_9_FIELD_AAAACBI, 0},
  {BITS_16_TO_23_FIELD_AAAACBL, 0},
  {BITS_24_TO_27_FIELD_AAAACBK, 0},
  NOT_SUPPORTED,
  {BITS_8_TO_13_FIELD_AAAACBN, 1},
  {BITS_0_TO_5_FIELD_AAAACAC, 2},
  {BITS_16_TO_23_FIELD_AAAACAB, 2},
  {BITS_0_TO_5_FIELD_AAAACAG, 3},
  {BITS_16_TO_23_FIELD_AAAACAF, 3},
  {BITS_0_TO_5_FIELD_AAAACCL, 4},
  {BITS_16_TO_28_FIELD_AAAACCN, 4},
  {BITS_0_TO_5_FIELD_AAAACCP, 5},
  {BITS_16_TO_28_FIELD_AAAACCR, 5},
  {BIT0_FIELD_AAAACBU, 6},
  {BIT1_FIELD_AAAACBW, 6},
  {BIT2_FIELD_AAAACBQ, 6},
  {BIT3_FIELD_AAAACBT, 6},
  {BITS_16_TO_21_FIELD_AAAACBR, 6},
  {BIT0_FIELD_AAAACCB, 7},
  {BIT1_FIELD_AAAACCD, 7},
  {BIT2_FIELD_AAAACBX, 7},
  {BIT3_FIELD_AAAACCA, 7},
  {BITS_16_TO_21_FIELD_AAAACBY, 7},
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  {BIT0_FIELD_AAAACAP, 8},
  {BIT3_FIELD_AAAACAK, 8},
  {BITS_4_TO_5_FIELD_AAAACAS, 8},
  {BITS_8_TO_10_FIELD_AAAACAQ, 8},
  {BITS_12_TO_14_FIELD_AAAACAO, 8},
  {BITS_16_TO_18_FIELD_AAAACAJ, 8},
  {BIT20_FIELD_AAAACAL, 8},
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  {BITS_28_TO_30_FIELD_AAAACAN, 8},
  {BITS_0_TO_1_FIELD_AAAACCF, 9},
  {BITS_2_TO_3_FIELD_AAAACCI, 9},
  {BIT4_FIELD_AAAACCH, 9},
  {BIT8_FIELD_AAAACCJ, 9},
  {BITS_16_TO_21_FIELD_AAAACCG, 9},
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  {BITS_0_TO_19_FIELD_AAAACBC, 10},
  {BITS_0_TO_19_FIELD_AAAACBF, 11},
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  NOT_SUPPORTED,
  {BIT0_FIELD_AAAACAT, 12},
  {BIT1_FIELD_AAAACAZ, 12},
  {BIT2_FIELD_AAAACBB, 12},
  {BIT8_FIELD_AAAACAY, 12},
  {BIT9_FIELD_AAAACAX, 12},
  {BIT10_FIELD_AAAACAW, 12},
  {BITS_16_TO_21_FIELD_AAAACAU, 12}
};

/**
 * MaskAndShift
 *
 * @brief Computes the shift given a mask then shifts Value by that amount,
 *        returning the new value
 */
static
uint32_t
MaskAndShift (
  uint32_t Value,
  uint32_t Mask
  )
{
  uint32_t  Shift;

  Value = Value & Mask;
  Shift = xUslCountTrailingZeroes(Mask);
  return (Value >> Shift);
}

/**
 * SetBits
 *
 * @brief Sets bits specified by Mask in Value to ValueToSet,
 *        returning the new value
 */
static
uint32_t
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
      DFABSTRACTxC84 + Index * (DFABSTRACTxC8C - DFABSTRACTxC84);
    break;
  case MMIO_ADDRESS_CONTROL0_RE:
  case MMIO_ADDRESS_CONTROL0_WE:
  case MMIO_ADDRESS_CONTROL0_DstFabricID:
    RetOffset =
      DFABSTRACTxD88 + Index * (DFABSTRACTxD98 - DFABSTRACTxD88);
    break;
  case X86_IOLIMIT_ADDRESS0_DstFabricID:
  case X86_IOLIMIT_ADDRESS0_IOLimit:
    RetOffset =
      DFABSTRACTxD04 + Index * (DFABSTRACTxD0C - DFABSTRACTxD04);
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
 * PhxAbstractRegAcc
 *
 * @brief   Accesses the register field specified by Field with Phx specific values
 *          when called from the xfer table
 * @details Accesses a fabric register in a way which is abstract to the current program.
 *          Given parameter Field, the FieldTable defined above is indexed to determine the mask
 *          of the field (that is, which bits the field refers to), and a second value which is used
 *          to index the RegTable, which provides the Func and Reg values needed to access the register.
 *          The actual register reading and writing is handled by the DfXFabricRegisterAcc functions.
 *          Error checking is performed to ensure accesses to fields not present on a given program are
 *          attempted.
 *
 * @param   SilContext    A context structure through which host firmware defined data
 *                        can be passed to openSIL. The host firmware is responsible
 *                        for initializing the SIL_CONTEXT structure.
 * @param   Field         The register field to access, specified by an enum in SilFabricRegistersDfX.h
 * @param   Socket        Processor socket to read from
 * @param   OffsetIndex   Offset applied to abstracted register
 * @param   InstanceId    Instance ID of the target fabric device
 * @param   Die           Number of Dies
 * @param   Rw            Read or write: 0 => read, else write
 * @param   WriteValue    Value to write (if writing)
 * @param   ReadValue     Location to return read value (if reading)
 *
 * @retval SilPass unless unsupported register field specified in Field, then SilUnsupported
 */
SIL_STATUS
PhxAbstractRegAcc (
  SIL_CONTEXT             *SilContext,
  uint8_t                 Field,
  uint32_t                Socket,
  uint32_t                OffsetIndex,
  uint32_t                InstanceId,
  uint8_t                 Die,
  uint8_t                 Rw,
  uint32_t                WriteValue,
  uint32_t                *ReadValue
  )
{
  uint32_t      Value;
  uint32_t      OffsetValue;
  SIL_STATUS    Status;

  if (Field >= NUM_FIELD_TABLE_ENTRIES) {
    DF_TRACEPOINT(SIL_TRACE_ERROR,
      "Field provided (%d) is invalid - out of bounds.\n",
      Field
      );
    Status = SilOutOfBounds;
  } else if (FieldTable[Field].Bits == 0) {
    // Field specified maps to NOT_SUPPORTED in the FieldTable
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

    // Read the value, edit the bitfield specified, then write back
    Value = DfXFabricRegisterAccRead(SilContext,
      Socket,
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

      DfXFabricRegisterAccWrite(SilContext,
        Socket,
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
