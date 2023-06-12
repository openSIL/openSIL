/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  BrhAbstractRegAcc.h
 * @brief Brh specific fabric register data used in abstract register access
 *
 */

#pragma once

#include <SilCommon.h>
#include <stdint.h>
#include <DF/DfX/SilFabricRegistersDfX.h>

#define ABSTRACT_REG_DEBUG_ENABLE 0

#ifdef ABSTRACT_REG_DEBUG_ENABLE
  #define ABSTRACT_REG_DF_TRACEPOINT DF_TRACEPOINT
#else
  #define ABSTRACT_REG_DF_TRACEPOINT
#endif

SIL_STATUS
BrhAbstractRegAcc (
  uint8_t                  Field,       ///< Value coming from the abstract register enum list specifying which register
  uint32_t                 Socket,      ///< Processor socket to read from
  uint32_t                 OffsetIndex, ///< Offset Value, 0 for no offset
  uint32_t                 InstanceId,  ///< Instance ID of target fabric device
  uint8_t                  Die,         ///< Number of Dies
  uint8_t                  Rw,          ///< Read/write, 0 for read
  uint32_t                 WriteValue,  ///< Value to be written if writing to a register
  uint32_t                 *ReadValue   ///< Value read from register is returned here when reading
  );

typedef struct {
  uint32_t Bits;             ///< Specifies what bits of the register belong to the bitfield
  uint32_t RegIndex;         ///< Index to the entry in the RegTable which has the register details for the bitfield
} FieldStruct;

typedef struct {
  uint32_t Func;             ///< Function vale for the register
  uint32_t Reg;              ///< Offset of the register
} RegStruct;

#define SIL_RESERVED_0084 0x0
#define SIL_RESERVED_0087  0x40

#define SIL_RESERVED_0083 0x3FF
#define SIL_RESERVED_0086 0xFF0000
#define SIL_RESERVED_0085 0xF000000

#define SIL_RESERVED_0090 0x0
#define SIL_RESERVED_0091  0x50

#define SIL_RESERVED_0089 0xFF
#define SIL_RESERVED_0088 0xFF00

#define CFG_LIMIT_ADDRESS0_FUNC 0x0
#define SIL_RESERVED_0027  0xC84

#define SIL_RESERVED_0026 0xFF
#define SIL_RESERVED_0025 0xFF0000

#define SIL_RESERVED_0030 0x0
#define SIL_RESERVED_0031  0xC8C

#define SIL_RESERVED_0029 0xFF
#define SIL_RESERVED_0028 0xFF0000

#define SIL_RESERVED_0114 0x0
#define SIL_RESERVED_0116  0xD04

#define SIL_RESERVED_0113 0xFF
#define SIL_RESERVED_0115 0x1FFF0000

#define SIL_RESERVED_0118 0x0
#define SIL_RESERVED_0120  0xD0C

#define SIL_RESERVED_0117 0xFF
#define SIL_RESERVED_0119 0x1FFF0000

#define SIL_RESERVED_0094 0x0
#define SIL_RESERVED_0097  0xD88

#define SIL_RESERVED_0096 0x1
#define SIL_RESERVED_0098 0x2
#define SIL_RESERVED_0092 0x4
#define SIL_RESERVED_0095 0x8
#define SIL_RESERVED_0093 0xFF0000

#define SIL_RESERVED_0101 0x0
#define SIL_RESERVED_0104  0xD98

#define SIL_RESERVED_0103 0x1
#define SIL_RESERVED_0105 0x2
#define SIL_RESERVED_0099 0x4
#define SIL_RESERVED_0102 0x8
#define SIL_RESERVED_0100 0xFF0000

#define SIL_RESERVED_0022 0x1
#define SIL_RESERVED_0023  0x104

#define SIL_RESERVED_0021 0x3
#define SIL_RESERVED_0024 0x80000000

#define SIL_RESERVED_0054 0x3
#define SIL_RESERVED_0060  0x44

#define SIL_RESERVED_0058 0x1
#define SIL_RESERVED_0049 0x8
#define SIL_RESERVED_0062 0x30
#define SIL_RESERVED_0059 0x700
#define SIL_RESERVED_0056 0x7000
#define SIL_RESERVED_0048 0x70000
#define SIL_RESERVED_0051 0x100000
#define SIL_RESERVED_0050 0x200000
#define SIL_RESERVED_0061 0x400000
#define SIL_RESERVED_0052 0x800000
#define SIL_RESERVED_0053 0x2000000
#define SIL_RESERVED_0057 0x8000000
#define SIL_RESERVED_0055 0x70000000

#define SIL_RESERVED_0106 0x4
#define SIL_RESERVED_0112  0x180

#define SIL_RESERVED_0107 0x3
#define SIL_RESERVED_0110 0xC
#define SIL_RESERVED_0109 0x10
#define SIL_RESERVED_0111 0x100
#define SIL_RESERVED_0108 0xFF0000

#define SIL_RESERVED_0032 0x7
#define SIL_RESERVED_0033  0x180

#define SIL_RESERVED_0034 0x1F
#define SIL_RESERVED_0035 0x3E0
#define SIL_RESERVED_0036 0x7C00
#define SIL_RESERVED_0037 0xF8000
#define SIL_RESERVED_0038 0x1F00000
#define SIL_RESERVED_0039 0x3E000000

#define SIL_RESERVED_0040 0x7
#define SIL_RESERVED_0041  0x184

#define SIL_RESERVED_0044 0x1F
#define SIL_RESERVED_0045 0x3E0
#define SIL_RESERVED_0046 0x7C00
#define SIL_RESERVED_0047 0xF8000
#define SIL_RESERVED_0042 0x1F00000
#define SIL_RESERVED_0043 0x3E000000

#define SIL_RESERVED_0078 0x7
#define SIL_RESERVED_0079  0x200

#define SIL_RESERVED_0077 0xFFFFFFF

#define SIL_RESERVED_0081 0x7
#define SIL_RESERVED_0082  0x204

#define SIL_RESERVED_0080 0xFFFFFFF

#define SIL_RESERVED_0066 0x7
#define SIL_RESERVED_0073  0x208

#define SIL_RESERVED_0063 0x1
#define SIL_RESERVED_0072 0x2
#define SIL_RESERVED_0076 0x4
#define SIL_RESERVED_0074 0x10
#define SIL_RESERVED_0075 0x60
#define SIL_RESERVED_0070 0x80
#define SIL_RESERVED_0071 0x100
#define SIL_RESERVED_0069 0x200
#define SIL_RESERVED_0067 0x400
#define SIL_RESERVED_0064 0x800
#define SIL_RESERVED_0068 0x8000
#define SIL_RESERVED_0065 0xFF0000
