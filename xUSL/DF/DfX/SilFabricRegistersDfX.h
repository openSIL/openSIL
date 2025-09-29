/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  SilFabricRegistersDfX.h
 * @brief Data fabric register definitions
 *
 */

#pragma once

#include <xSIM.h>

#pragma pack (push, 1)

/* DfX Abstract Register Access Enum */
typedef enum {
  FABRIC_BLOCK_INSTANCE_COUNT_BlkInstCount,
  FABRIC_BLOCK_INSTANCE_COUNT_MinorRevision,
  FABRIC_BLOCK_INSTANCE_COUNT_MajorRevision,
  FABRIC_BLOCK_INSTANCE_INFORMATION3_CSNCSPIEALLM_BlockInstanceID,
  FABRIC_BLOCK_INSTANCE_INFORMATION3_CSNCSPIEALLM_BlockFabricID,
  CFG_LIMIT_ADDRESS0_DstFabricID,
  CFG_LIMIT_ADDRESS0_BusNumLimit,
  CFG_LIMIT_ADDRESS1_DstFabricID,
  CFG_LIMIT_ADDRESS1_BusNumLimit,
  X86_IOLIMIT_ADDRESS0_DstFabricID,
  X86_IOLIMIT_ADDRESS0_IOLimit,
  X86_IOLIMIT_ADDRESS1_DstFabricID,
  X86_IOLIMIT_ADDRESS1_IOLimit,
  MMIO_ADDRESS_CONTROL0_RE,
  MMIO_ADDRESS_CONTROL0_WE,
  MMIO_ADDRESS_CONTROL0_CpuDis,
  MMIO_ADDRESS_CONTROL0_Np,
  MMIO_ADDRESS_CONTROL0_DstFabricID,
  MMIO_ADDRESS_CONTROL1_RE,
  MMIO_ADDRESS_CONTROL1_WE,
  MMIO_ADDRESS_CONTROL1_CpuDis,
  MMIO_ADDRESS_CONTROL1_Np,
  MMIO_ADDRESS_CONTROL1_DstFabricID,
  CCD_ENABLE_CCDEn,
  DF_GLOBAL_CTRL_McastMaskPerNodeCLV,
  DF_GLOBAL_CTRL_DisImmSyncFloodOnFatalErr,
  DF_GLOBAL_CTRL_WDTBaseSel,
  DF_GLOBAL_CTRL_PIEWDTCntSel,
  DF_GLOBAL_CTRL_IOMWDTCntSel,
  DF_GLOBAL_CTRL_CCMWDTCntSel,
  DF_GLOBAL_CTRL_DisWdtHwaSyndDbgBus,
  DF_GLOBAL_CTRL_GNCMWDTCntSel,
  SYSTEM_CFG_MyDieType,
  SYSTEM_CFG_OtherDieType,
  SYSTEM_CFG_MyNodeMap,
  SYSTEM_CFG_OtherSocketPresent,
  SYSTEM_CFG_MyNodeId,
  CS_TARGET_REMAP0_A_RemapCS0,
  CS_TARGET_REMAP0_A_RemapCS1,
  CS_TARGET_REMAP0_A_RemapCS2,
  CS_TARGET_REMAP0_A_RemapCS3,
  CS_TARGET_REMAP0_A_RemapCS4,
  CS_TARGET_REMAP0_A_RemapCS5,
  CS_TARGET_REMAP0_B_RemapCS8,
  CS_TARGET_REMAP0_B_RemapCS9,
  CS_TARGET_REMAP0_B_RemapCS10,
  CS_TARGET_REMAP0_B_RemapCS11,
  DRAM_BASE_ADDRESS_DramBaseAddr,
  DRAM_LIMIT_ADDRESS_DramLimitAddr,
  DRAM_ADDRESS_CTL_AddrRngVal,
  DRAM_ADDRESS_CTL_LgcyMmioHoleEn,
  DRAM_ADDRESS_CTL_SCM,
  DRAM_ADDRESS_CTL_RemapEn,
  DRAM_ADDRESS_CTL_RemapSel,
  DRAM_ADDRESS_CTL_HashIntlvCtl64K,
  DRAM_ADDRESS_CTL_HashIntlvCtl2M,
  DRAM_ADDRESS_CTL_HashIntlvCtl1G,
  DRAM_ADDRESS_CTL_DstFabricID,
  DRAM_ADDRESS_CTL0_AddrRngVal,
  DRAM_ADDRESS_CTL0_LgcyMmioHoleEn,
  DRAM_ADDRESS_CTL0_SCM,
  DRAM_ADDRESS_CTL0_HashIntlvCtl64K,
  DRAM_ADDRESS_CTL0_HashIntlvCtl2M,
  DRAM_ADDRESS_CTL0_HashIntlvCtl1G,
  DRAM_ADDRESS_CTL0_DstFabricID
} DF_FIELD_TABLE_ENTRIES;

// Size of Field Table in Register Abstraction
#define NUM_FIELD_TABLE_ENTRIES 83

// Registers that are not supported in a specific program in register abstraction
#define NOT_SUPPORTED {0, 0}

#define DF_ABSTRACT_REG_READ   0
#define DF_ABSTRACT_REG_WRITE  1

#define DFXFABFUNCx5 0x5
#define DFXFABFUNC5x104 0x104

typedef union {
  struct {
    uint32_t field_bits_0_to_3:4;
    uint32_t :28;
  } Field;
  uint32_t Value;
} SIL_RESERVED_UNION_0008;

#define CFGADDRESSCNTL_FUNC 0x0
#define CFGADDRESSCNTL_REG  0xC04

typedef union {
  struct {
    uint32_t SecBusNum:8;
    uint32_t :24;
  } Field;
  uint32_t  Value;
} CFG_ADDRESS_CNTL_REGISTER;

#define CFGBASEADDRESS_0_FUNC 0x0
#define CFGBASEADDRESS_0_REG  0xC80

#define CFGBASEADDRESS_1_REG  0xC88

typedef union {
  struct {
    uint32_t RE:1;
    uint32_t WE:1;
    uint32_t :6;
    uint32_t SegmentNum:8;
    uint32_t BusNumBase:8;
    uint32_t :8;
  } Field;
  uint32_t  Value;
} CFG_BASE_ADDRESS_REGISTER;

#define X86IOBASEADDRESS_0_FUNC 0x0
#define X86IOBASEADDRESS_0_REG  0xD00

#define X86IOBASEADDRESS_1_REG  0xD08

typedef union {
  struct {
    uint32_t RE:1;
    uint32_t WE:1;
    uint32_t :3;
    uint32_t IE:1;
    uint32_t :10;
    uint32_t IOBase:13;
    uint32_t :3;
  } Field;
  uint32_t  Value;
} X86_IOBASE_ADDRESS_REGISTER;

#define MMIOBASEADDRESS_0_FUNC 0x0
#define MMIOBASEADDRESS_0_REG  0xD80

#define MMIOBASEADDRESS_1_REG  0xD90

#define MMIOLIMITADDRESS_0_FUNC 0x0
#define MMIOLIMITADDRESS_0_REG  0xD84

#define MMIOLIMITADDRESS_1_REG  0xD94

#define HARDWAREASSERTSTATUSLOW_FUNC 0x3
#define HARDWAREASSERTSTATUSLOW_REG  0x8F0

typedef union {
  struct {
    uint32_t HWAssert0:1;
    uint32_t HWAssert1:1;
    uint32_t HWAssert2:1;
    uint32_t HWAssert3:1;
    uint32_t HWAssert4:1;
    uint32_t HWAssert5:1;
    uint32_t HWAssert6:1;
    uint32_t HWAssert7:1;
    uint32_t HWAssert8:1;
    uint32_t HWAssert9:1;
    uint32_t HWAssert10:1;
    uint32_t HWAssert11:1;
    uint32_t HWAssert12:1;
    uint32_t HWAssert13:1;
    uint32_t HWAssert14:1;
    uint32_t HWAssert15:1;
    uint32_t HWAssert16:1;
    uint32_t HWAssert17:1;
    uint32_t HWAssert18:1;
    uint32_t HWAssert19:1;
    uint32_t HWAssert20:1;
    uint32_t HWAssert21:1;
    uint32_t HWAssert22:1;
    uint32_t HWAssert23:1;
    uint32_t HWAssert24:1;
    uint32_t HWAssert25:1;
    uint32_t HWAssert26:1;
    uint32_t HWAssert27:1;
    uint32_t HWAssert28:1;
    uint32_t HWAssert29:1;
    uint32_t HWAssert30:1;
    uint32_t HWAssert31:1;
  } Field;
  uint32_t  Value;
} HARDWARE_ASSERT_STATUS_LOW_REGISTER;

#define HARDWAREASSERTSTATUSHIGH_FUNC 0x3
#define HARDWAREASSERTSTATUSHIGH_REG  0x8F4

typedef union {
  struct {
    uint32_t HWAssert0:1;
    uint32_t HWAssert1:1;
    uint32_t HWAssert2:1;
    uint32_t HWAssert3:1;
    uint32_t HWAssert4:1;
    uint32_t HWAssert5:1;
    uint32_t HWAssert6:1;
    uint32_t HWAssert7:1;
    uint32_t HWAssert8:1;
    uint32_t HWAssert9:1;
    uint32_t HWAssert10:1;
    uint32_t HWAssert11:1;
    uint32_t HWAssert12:1;
    uint32_t HWAssert13:1;
    uint32_t HWAssert14:1;
    uint32_t HWAssert15:1;
    uint32_t HWAssert16:1;
    uint32_t HWAssert17:1;
    uint32_t HWAssert18:1;
    uint32_t HWAssert19:1;
    uint32_t HWAssert20:1;
    uint32_t HWAssert21:1;
    uint32_t HWAssert22:1;
    uint32_t HWAssert23:1;
    uint32_t HWAssert24:1;
    uint32_t HWAssert25:1;
    uint32_t HWAssert26:1;
    uint32_t HWAssert27:1;
    uint32_t HWAssert28:1;
    uint32_t HWAssert29:1;
    uint32_t HWAssert30:1;
    uint32_t HWAssert31:1;
  } Field;
  uint32_t  Value;
} HARDWARE_ASSERT_STATUS_HIGH_REGISTER;

#define HARDWAREASSERTMASKLOW_FUNC 0x3
#define HARDWAREASSERTMASKLOW_REG  0x8F8

typedef union {
  struct {
    uint32_t HWAssertMsk0:1;
    uint32_t HWAssertMsk1:1;
    uint32_t HWAssertMsk2:1;
    uint32_t HWAssertMsk3:1;
    uint32_t HWAssertMsk4:1;
    uint32_t HWAssertMsk5:1;
    uint32_t HWAssertMsk6:1;
    uint32_t HWAssertMsk7:1;
    uint32_t HWAssertMsk8:1;
    uint32_t HWAssertMsk9:1;
    uint32_t HWAssertMsk10:1;
    uint32_t HWAssertMsk11:1;
    uint32_t HWAssertMsk12:1;
    uint32_t HWAssertMsk13:1;
    uint32_t HWAssertMsk14:1;
    uint32_t HWAssertMsk15:1;
    uint32_t HWAssertMsk16:1;
    uint32_t HWAssertMsk17:1;
    uint32_t HWAssertMsk18:1;
    uint32_t HWAssertMsk19:1;
    uint32_t HWAssertMsk20:1;
    uint32_t HWAssertMsk21:1;
    uint32_t HWAssertMsk22:1;
    uint32_t HWAssertMsk23:1;
    uint32_t HWAssertMsk24:1;
    uint32_t HWAssertMsk25:1;
    uint32_t HWAssertMsk26:1;
    uint32_t HWAssertMsk27:1;
    uint32_t HWAssertMsk28:1;
    uint32_t HWAssertMsk29:1;
    uint32_t HWAssertMsk30:1;
    uint32_t HWAssertMsk31:1;
  } Field;
  uint32_t  Value;
} HARDWARE_ASSERT_MASK_LOW_REGISTER;

/* Hardware Assert Mask High Register */
#define HARDWAREASSERTMASKHIGH_FUNC 0x3
#define HARDWAREASSERTMASKHIGH_REG  0x8FC


typedef union {
  struct {
    uint32_t HWAssertMsk0:1;
    uint32_t HWAssertMsk1:1;
    uint32_t HWAssertMsk2:1;
    uint32_t HWAssertMsk3:1;
    uint32_t HWAssertMsk4:1;
    uint32_t HWAssertMsk5:1;
    uint32_t HWAssertMsk6:1;
    uint32_t HWAssertMsk7:1;
    uint32_t HWAssertMsk8:1;
    uint32_t HWAssertMsk9:1;
    uint32_t HWAssertMsk10:1;
    uint32_t HWAssertMsk11:1;
    uint32_t HWAssertMsk12:1;
    uint32_t HWAssertMsk13:1;
    uint32_t HWAssertMsk14:1;
    uint32_t HWAssertMsk15:1;
    uint32_t HWAssertMsk16:1;
    uint32_t HWAssertMsk17:1;
    uint32_t HWAssertMsk18:1;
    uint32_t HWAssertMsk19:1;
    uint32_t HWAssertMsk20:1;
    uint32_t HWAssertMsk21:1;
    uint32_t HWAssertMsk22:1;
    uint32_t HWAssertMsk23:1;
    uint32_t HWAssertMsk24:1;
    uint32_t HWAssertMsk25:1;
    uint32_t HWAssertMsk26:1;
    uint32_t HWAssertMsk27:1;
    uint32_t HWAssertMsk28:1;
    uint32_t HWAssertMsk29:1;
    uint32_t HWAssertMsk30:1;
    uint32_t HWAssertMsk31:1;
  } Field;
  uint32_t  Value;
} HARDWARE_ASSERT_MASK_HIGH_REGISTER;

#define FABRICINDIRECTCONFIGACCESSADDRESS_3_FUNC 0x4
#define FABRICINDIRECTCONFIGACCESSADDRESS_3_REG  0x08C

typedef union {
  struct {
    uint32_t CfgRegInstAccEn:1;
    uint32_t IndCfgAccRegNum:10;
    uint32_t IndCfgAccFuncNum:3;
    uint32_t SixtyFourBitRegEn:1;
    uint32_t :1;
    uint32_t CfgRegInstID:8;
    uint32_t :8;
  } Field;
  uint32_t  Value;
} FABRIC_INDIRECT_CONFIG_ACCESS_ADDRESS_REGISTER;

#define FABRICINDIRECTCONFIGACCESSDATALO_3_FUNC 0x4
#define FABRICINDIRECTCONFIGACCESSDATALO_3_REG  0x0B8

#pragma pack (pop)
