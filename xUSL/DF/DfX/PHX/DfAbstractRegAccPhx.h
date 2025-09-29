/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  DfAbstractRegAccPhx.h
 * @brief Phx specific fabric register data used in abstract register access
 *
 */

#pragma once

#include <SilCommon.h>
#include <stdint.h>
#include <DF/DfX/SilFabricRegistersDfX.h>

#define ABSTRACT_REG_DEBUG_ENABLE 1

#ifdef ABSTRACT_REG_DEBUG_ENABLE
  #define ABSTRACT_REG_DF_TRACEPOINT DF_TRACEPOINT
#else
  #define ABSTRACT_REG_DF_TRACEPOINT
#endif

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
  );

typedef struct {
  uint32_t Bits;             ///< Specifies what bits of the register belong to the bitfield
  uint32_t RegIndex;         ///< Index to the entry in the RegTable which has the register details for the bitfield
} FieldStruct;

typedef struct {
  uint32_t Func;             ///< Function vale for the register
  uint32_t Reg;              ///< Offset of the register
} RegStruct;

#define DF_FUNC0    0
#define DF_FUNC3    3
#define DF_FUNC4    4
#define DF_FUNC7    7

#define DFABSTRACTx40  0x40

#define BITS_0_TO_9_FIELD_AAAACBI 0x3FF
#define BITS_16_TO_23_FIELD_AAAACBL 0xFF0000
#define BITS_24_TO_27_FIELD_AAAACBK 0xF000000

#define DFABSTRACTx50  0x50

#define BITS_8_TO_13_FIELD_AAAACBN 0x3F00

#define CFG_LIMIT_ADDRESS0_FUNC 0x0
#define DFABSTRACTxC84  0xC84

#define BITS_0_TO_5_FIELD_AAAACAC 0x3F
#define BITS_16_TO_23_FIELD_AAAACAB 0xFF0000

#define DFABSTRACTxC8C  0xC8C

#define BITS_0_TO_5_FIELD_AAAACAG 0x3F
#define BITS_16_TO_23_FIELD_AAAACAF 0xFF0000

#define DFABSTRACTxD04  0xD04

#define BITS_0_TO_5_FIELD_AAAACCL 0x3F
#define BITS_16_TO_28_FIELD_AAAACCN 0x1FFF0000

#define DFABSTRACTxD0C  0xD0C

#define BITS_0_TO_5_FIELD_AAAACCP 0x3F
#define BITS_16_TO_28_FIELD_AAAACCR 0x1FFF0000

#define DFABSTRACTxD88  0xD88

#define BIT0_FIELD_AAAACBU 0x1
#define BIT1_FIELD_AAAACBW 0x2
#define BIT2_FIELD_AAAACBQ 0x4
#define BIT3_FIELD_AAAACBT 0x8
#define BITS_16_TO_21_FIELD_AAAACBR 0x3F0000

#define DFABSTRACTxD98  0xD98

#define BIT0_FIELD_AAAACCB 0x1
#define BIT1_FIELD_AAAACCD 0x2
#define BIT2_FIELD_AAAACBX 0x4
#define BIT3_FIELD_AAAACCA 0x8
#define BITS_16_TO_21_FIELD_AAAACBY 0x3F0000

#define DFABSTRACTx44  0x44

#define BIT0_FIELD_AAAACAP 0x1
#define BIT3_FIELD_AAAACAK 0x8
#define BITS_4_TO_5_FIELD_AAAACAS 0x30
#define BITS_8_TO_10_FIELD_AAAACAQ 0x700
#define BITS_12_TO_14_FIELD_AAAACAO 0x7000
#define BITS_16_TO_18_FIELD_AAAACAJ 0x70000
#define BIT20_FIELD_AAAACAL 0x100000
#define BITS_28_TO_30_FIELD_AAAACAN 0x70000000

#define DFABSTRACTx180  0x180

#define BITS_0_TO_1_FIELD_AAAACCF 0x3
#define BITS_2_TO_3_FIELD_AAAACCI 0xC
#define BIT4_FIELD_AAAACCH 0x10
#define BIT8_FIELD_AAAACCJ 0x100
#define BITS_16_TO_21_FIELD_AAAACCG 0x3F0000

#define DFABSTRACTx200  0x200

#define BITS_0_TO_19_FIELD_AAAACBC 0xFFFFF

#define DFABSTRACTx204  0x204

#define BITS_0_TO_19_FIELD_AAAACBF 0xFFFFF

#define DFABSTRACTx208  0x208

#define BIT0_FIELD_AAAACAT 0x1
#define BIT1_FIELD_AAAACAZ 0x2
#define BIT2_FIELD_AAAACBB 0x4
#define BIT8_FIELD_AAAACAY 0x100
#define BIT9_FIELD_AAAACAX 0x200
#define BIT10_FIELD_AAAACAW 0x400
#define BITS_16_TO_21_FIELD_AAAACAU 0x3F0000
