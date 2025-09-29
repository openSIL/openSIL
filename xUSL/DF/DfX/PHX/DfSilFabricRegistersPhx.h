/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  DfSilFabricRegistersPhx.h
 * @brief Data fabric register definitions which are specific to Phx
 *
 */

#pragma once

#include <xSIM.h>

#pragma pack (push, 1)

/* Fabric Block Instance Count Register */
#define FABRICBLOCKINSTANCECOUNT_FUNC 0x0
#define DFXFABx040  0x040


typedef union {
  struct {
    uint32_t BlkInstCount:10;
    uint32_t :6;
    uint32_t MinorRevision:8;
    uint32_t MajorRevision:4;
    uint32_t :4;
  } Field;
  uint32_t  Value;
} FABRIC_BLOCK_INSTANCE_COUNT_REGISTER;

#define CFGLIMITADDRESS_0_FUNC 0x0
#define CFGLIMITADDRESS_0_REG  0xC84

#define CFGLIMITADDRESS_1_REG  0xC8C


typedef union {
  struct {
    uint32_t DstFabricID:6;
    uint32_t :10;
    uint32_t BusNumLimit:8;
    uint32_t :8;
  } Field;
  uint32_t  Value;
} CFG_LIMIT_ADDRESS_REGISTER;

#define DFXFABx01 0x1
#define DFXFABx140  0x140

#define DFGLOBALCTRL_FUNC 0x3
#define DFGLOBALCTRL_REG  0x044

typedef union {
  struct {
    uint32_t McastMaskPerNodeCLV:1;
    uint32_t :2;
    uint32_t DisImmSyncFloodOnFatalErr:1;
    uint32_t WDTBaseSel:2;
    uint32_t :2;
    uint32_t PIEWDTCntSel:3;
    uint32_t :1;
    uint32_t IOMWDTCntSel:3;
    uint32_t :1;
    uint32_t CCMWDTCntSel:3;
    uint32_t :1;
    uint32_t DisWdtHwaSyndDbgBus:1;
    uint32_t :2;
    uint32_t Reserved_23:1;
    uint32_t :4;
    uint32_t GNCMWDTCntSel:3;
    uint32_t :1;
  } Field;
  uint32_t  Value;
} DF_GLOBAL_CTRL_REGISTER;

#define SYSTEMCFG_FUNC 0x4
#define SYSTEMCFG_REG  0x180

typedef union {
  struct {
    uint32_t MyDieType:2;
    uint32_t OtherDieType:2;
    uint32_t MyNodeMap:1;
    uint32_t :3;
    uint32_t OtherSocketPresent:1;
    uint32_t :7;
    uint32_t MyNodeId:6;
    uint32_t :10;
  } Field;
  uint32_t  Value;
} SYSTEM_CFG_REGISTER;

#define DRAMBASEADDRESS_0_FUNC 0x7
#define DFXFABx200  0x200
#define DFXFABx210  0x210

typedef union {
  struct {
    uint32_t DramBaseAddr:20;
    uint32_t :12;
  } Field;
  uint32_t  Value;
} DRAM_BASE_ADDRESS_REGISTER;
#define DRAMLIMITADDRESS_0_FUNC 0x7
#define DFXFABx204  0x204

#define DFXFABx214  0x214


typedef union {
  struct {
    uint32_t DramLimitAddr:20;
    uint32_t :12;
  } Field;
  uint32_t  Value;
} DRAM_LIMIT_ADDRESS_REGISTER;

#define DRAMADDRESSINTLV_0_FUNC 0x7
#define DFXFABx20C  0x20C
#define DFXFABx21C  0x21C

typedef union {
  struct {
    uint32_t IntLvAddrSel:3;
    uint32_t :1;
    uint32_t IntLvNumChan:5;
    uint32_t :3;
    uint32_t IntLvNumDies:2;
    uint32_t :4;
    uint32_t IntLvNumSockets:1;
    uint32_t :13;
  } Field;
  uint32_t  Value;
} DRAM_ADDRESS_INTLV_REGISTER;

#define DFXFABx218  0x218

#define DFXFABx144  0x144

#define DRAMADDRESSCTL0_FUNC 0x7
#define DFXFABx208  0x208

typedef union {
  struct {
    uint32_t field_bit_0:1;
    uint32_t field_bit_1:1;
    uint32_t field_bit_2:1;
    uint32_t :5;
    uint32_t field_bit_8:1;
    uint32_t field_bit_9:1;
    uint32_t field_bit_10:1;
    uint32_t :5;
    uint32_t field_bits_16_to_21:6;
    uint32_t :10;
  } Field;
  uint32_t  Value;
} SIL_RESERVED_UNION_0009;

#pragma pack (pop)
