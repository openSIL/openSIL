/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */
/**
 * @file  SilFabricRegistersDfX.h
 * @brief Data fabric register definitions common between SoCs specified
 *        as DfX (currently RS and BRH)
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
  CCD_ENABLE_CCXEn,     // RS
  CCD_ENABLE_SdpWideEn, // BRH
  DF_GLOBAL_CTRL_McastMaskPerNodeCLV,
  DF_GLOBAL_CTRL_DisImmSyncFloodOnFatalErr,
  DF_GLOBAL_CTRL_WDTBaseSel,
  DF_GLOBAL_CTRL_PIEWDTCntSel,
  DF_GLOBAL_CTRL_IOMWDTCntSel,
  DF_GLOBAL_CTRL_CCMWDTCntSel,
  DF_GLOBAL_CTRL_DisWdtHwaSyndDbgBus,
  DF_GLOBAL_CTRL_DisProbeChannelHash, // BRH
  DF_GLOBAL_CTRL_VicBlkClnHasAck,     // BRH
  DF_GLOBAL_CTRL_EarlyWrRspVicBlkCln, // BRH
  DF_GLOBAL_CTRL_EnSpf1KRegion,       // BRH
  DF_GLOBAL_CTRL_MaxNumCorePerCCX,    // BRH
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
  CS_TARGET_REMAP0_A_RemapCS6, // RS
  CS_TARGET_REMAP0_A_RemapCS7, // RS
  CS_TARGET_REMAP0_B_RemapCS6, // BRH
  CS_TARGET_REMAP0_B_RemapCS7, // BRH
  CS_TARGET_REMAP0_B_RemapCS8,
  CS_TARGET_REMAP0_B_RemapCS9,
  CS_TARGET_REMAP0_B_RemapCS10,
  CS_TARGET_REMAP0_B_RemapCS11,
  CS_TARGET_REMAP0_B_RemapCS12, // RS
  CS_TARGET_REMAP0_B_RemapCS13, // RS
  CS_TARGET_REMAP0_B_RemapCS14, // RS
  CS_TARGET_REMAP0_B_RemapCS15, // RS
  DRAM_BASE_ADDRESS_DramBaseAddr,
  DRAM_LIMIT_ADDRESS_DramLimitAddr,
  DRAM_ADDRESS_CTL_AddrRngVal,
  DRAM_ADDRESS_CTL_LgcyMmioHoleEn,
  DRAM_ADDRESS_CTL_SCM,
  DRAM_ADDRESS_CTL_RemapEn,
  DRAM_ADDRESS_CTL_RemapSel,
  DRAM_ADDRESS_CTL_HashIntlvCtl4K, // BRH
  DRAM_ADDRESS_CTL_HashIntlvCtl64K,
  DRAM_ADDRESS_CTL_HashIntlvCtl2M,
  DRAM_ADDRESS_CTL_HashIntlvCtl1G,
  DRAM_ADDRESS_CTL_DramColSwizzle, // BRH
  DRAM_ADDRESS_CTL_HashIntlvCtl1T, // BRH
  DRAM_ADDRESS_CTL_DstFabricID
} DF_FIELD_TABLE_ENTRIES;

// Size of Field Table in Register Abstraction
#define NUM_FIELD_TABLE_ENTRIES 76

// Registers that are not supported in a specific program in register abstraction
#define NOT_SUPPORTED {0, 0}

#define DF_ABSTRACT_REG_READ   0
#define DF_ABSTRACT_REG_WRITE  1

#define SIL_RESERVED_0271 0x5
#define SIL_RESERVED_0272 0x104

/// DF Global Clock Gater Register
typedef union {
  struct {                                ///< Bitfields of DF Global Clock Gater Register
    uint32_t field_bits_0_to_3:4;                  ///< Mid-grain clock gating mode
    uint32_t :28;                         ///< Reserved
  } Field;
  uint32_t Value;
} SIL_RESERVED_UNION_0011;

/* Fabric Block Instance Information 0 Register */
#define FABRICBLOCKINSTANCEINFORMATION0_FUNC 0x0
#define FABRICBLOCKINSTANCEINFORMATION0_REG  0x044

/// Fabric Block Instance Information 0 Register
typedef union {
  struct {
    uint32_t InstanceType:4;                  ///< Instance type register.
    uint32_t SdpDataBusWidth:2;               ///< SDP data bus width.
    uint32_t Enabled:1;                       ///< Indicates component is enabled for responding to register accesses.
    uint32_t :1;                              ///< Reserved
    uint32_t FtiDataBusWidth:2;               ///< FTI data bus width.
    uint32_t :2;                              ///< Reserved
    uint32_t SdpPortCnt:2;                    ///< Number of SDP ports connected to the instance.
    uint32_t SdpRdRespBuses:1;                ///< Number of SDP Read response buses.
    uint32_t :1;                              ///< Reserved
    uint32_t FtiPortCnt:3;                    ///< Number of Fabric transport interfaces connected to instance.
    uint32_t :1;                              ///< Reserved
    uint32_t FtiPortDatBusCnt:2;              ///< Number of data buses attached to an FTI port.
    uint32_t :1;                              ///< Reserved
    uint32_t McaBankPresent:1;                ///< Set by hardware if the Instance has an MCA bank.
    uint32_t InstanceSubType:3;               ///< Component sub-types.
    uint32_t :5;                              ///< Reserved
  } Field;
  uint32_t  Value;
} FABRIC_BLOCK_INSTANCE_INFORMATION0_REGISTER;

/// Core Coherent Master Configuration Access Control Register
typedef union {
  struct {
    uint32_t EnableCf8ExtCfg:1;               ///< Enable PCI extended configuration register access for CFC/CF8
                                              ///  accesses.
    uint32_t DisPciCfgReg:1;                  ///< Disable CFC/CF8 Accesses to IO space. When set to 1, CFC/CF8
                                              ///  accesses are treated as PCI IO space accesses and not PCI
                                              ///  configuration accesses.
    uint32_t :30;                             ///< Reserved
  } Field;
  uint32_t  Value;
} CORE_MASTER_ACCESS_CTRL_REGISTER;

/* Config Address Control Register */
#define CFGADDRESSCNTL_FUNC 0x0
#define CFGADDRESSCNTL_REG  0xC04

/// Config Address Control Register
typedef union {
  struct {
    uint32_t SecBusNum:8;                     ///< Specifies the configuration-space bus number of the IO Link.
    uint32_t :24;                             ///< Reserved
  } Field;
  uint32_t  Value;
} CFG_ADDRESS_CNTL_REGISTER;

/* Configuration Base Address Maps Register */
#define CFGBASEADDRESS_0_FUNC 0x0
#define CFGBASEADDRESS_0_REG  0xC80

#define CFGBASEADDRESS_1_FUNC 0x0
#define CFGBASEADDRESS_1_REG  0xC88

/// Configuration Base Address Maps Register
typedef union {
  struct {
    uint32_t RE:1;                            ///< Read Enable.
    uint32_t WE:1;                            ///< Write Enable.
    uint32_t :6;                              ///< Reserved
    uint32_t SegmentNum:8;                    ///< Segment Number for this configuration-space address map.
    uint32_t BusNumBase:8;                    ///< Bus Number Base Bits[7:0] for this configuration-space address map.
                                              ///  See DF::CfgAddressCntl[SecBusNum].
    uint32_t :8;                              ///< Reserved
  } Field;
  uint32_t  Value;
} CFG_BASE_ADDRESS_REGISTER;

/* IO Space Base Address Register */
#define X86IOBASEADDRESS_0_FUNC 0x0
#define X86IOBASEADDRESS_0_REG  0xD00

#define X86IOBASEADDRESS_1_REG  0xD08

/// IO Space Base Address Register
typedef union {
  struct {
    uint32_t RE:1;                            ///< Read Enable.
    uint32_t WE:1;                            ///< Write Enable.
    uint32_t :3;                              ///< Reserved
    uint32_t IE:1;                            ///< PCI IO space address window ISA Enable.
    uint32_t :10;                             ///< Reserved
    uint32_t IOBase:13;                       ///< IO Base address[24:12] for this IO Space range.
    uint32_t :3;                              ///< Reserved
  } Field;
  uint32_t  Value;
} X86_IOBASE_ADDRESS_REGISTER;

/* MMIO Base Address Register */
#define MMIOBASEADDRESS_0_FUNC 0x0
#define MMIOBASEADDRESS_0_REG  0xD80

#define MMIOBASEADDRESS_1_REG  0xD90

/// MMIO Base Address Register
typedef union {
  struct {
    uint32_t MmioBaseAddr:32;                 ///< MMIO Base address[47:16].
  } Field;
  uint32_t  Value;
} MMIO_BASE_ADDRESS_REGISTER;

/* MMIO Limit Address Register */
#define MMIOLIMITADDRESS_0_FUNC 0x0
#define MMIOLIMITADDRESS_0_REG  0xD84

#define MMIOLIMITADDRESS_1_REG  0xD94

/// MMIO Limit Address Register
typedef union {
  struct {
    uint32_t MmioLimitAddr:32;                ///< MMIO Limit address[47:16].
  } Field;
  uint32_t  Value;
} MMIO_LIMIT_ADDRESS_REGISTER;

/* Hardware Assert Status Low Register */
#define HARDWAREASSERTSTATUSLOW_FUNC 0x3
#define HARDWAREASSERTSTATUSLOW_REG  0x8F0

/// Hardware Assert Status Low Register
typedef union {
  struct {
    uint32_t HWAssert0:1;                     ///< Hardware Assert 0.
    uint32_t HWAssert1:1;                     ///< Hardware Assert 1.
    uint32_t HWAssert2:1;                     ///< Hardware Assert 2.
    uint32_t HWAssert3:1;                     ///< Hardware Assert 3.
    uint32_t HWAssert4:1;                     ///< Hardware Assert 4.
    uint32_t HWAssert5:1;                     ///< Hardware Assert 5.
    uint32_t HWAssert6:1;                     ///< Hardware Assert 6.
    uint32_t HWAssert7:1;                     ///< Hardware Assert 7.
    uint32_t HWAssert8:1;                     ///< Hardware Assert 8.
    uint32_t HWAssert9:1;                     ///< Hardware Assert 9.
    uint32_t HWAssert10:1;                    ///< Hardware Assert 10.
    uint32_t HWAssert11:1;                    ///< Hardware Assert 11.
    uint32_t HWAssert12:1;                    ///< Hardware Assert 12.
    uint32_t HWAssert13:1;                    ///< Hardware Assert 13.
    uint32_t HWAssert14:1;                    ///< Hardware Assert 14.
    uint32_t HWAssert15:1;                    ///< Hardware Assert 15.
    uint32_t HWAssert16:1;                    ///< Hardware Assert 16.
    uint32_t HWAssert17:1;                    ///< Hardware Assert 17.
    uint32_t HWAssert18:1;                    ///< Hardware Assert 18.
    uint32_t HWAssert19:1;                    ///< Hardware Assert 19.
    uint32_t HWAssert20:1;                    ///< Hardware Assert 20.
    uint32_t HWAssert21:1;                    ///< Hardware Assert 21.
    uint32_t HWAssert22:1;                    ///< Hardware Assert 22.
    uint32_t HWAssert23:1;                    ///< Hardware Assert 23.
    uint32_t HWAssert24:1;                    ///< Hardware Assert 24.
    uint32_t HWAssert25:1;                    ///< Hardware Assert 25.
    uint32_t HWAssert26:1;                    ///< Hardware Assert 26.
    uint32_t HWAssert27:1;                    ///< Hardware Assert 27.
    uint32_t HWAssert28:1;                    ///< Hardware Assert 28.
    uint32_t HWAssert29:1;                    ///< Hardware Assert 29.
    uint32_t HWAssert30:1;                    ///< Hardware Assert 30.
    uint32_t HWAssert31:1;                    ///< Hardware Assert 31.
  } Field;
  uint32_t  Value;
} HARDWARE_ASSERT_STATUS_LOW_REGISTER;

/* Hardware Assert Status High Register */
#define HARDWAREASSERTSTATUSHIGH_FUNC 0x3
#define HARDWAREASSERTSTATUSHIGH_REG  0x8F4

/// Hardware Assert Status High Register
typedef union {
  struct {
    uint32_t HWAssert0:1;                     ///< Hardware Assert 32.
    uint32_t HWAssert1:1;                     ///< Hardware Assert 33.
    uint32_t HWAssert2:1;                     ///< Hardware Assert 34.
    uint32_t HWAssert3:1;                     ///< Hardware Assert 35.
    uint32_t HWAssert4:1;                     ///< Hardware Assert 36.
    uint32_t HWAssert5:1;                     ///< Hardware Assert 37.
    uint32_t HWAssert6:1;                     ///< Hardware Assert 38.
    uint32_t HWAssert7:1;                     ///< Hardware Assert 39.
    uint32_t HWAssert8:1;                     ///< Hardware Assert 40.
    uint32_t HWAssert9:1;                     ///< Hardware Assert 41.
    uint32_t HWAssert10:1;                    ///< Hardware Assert 42.
    uint32_t HWAssert11:1;                    ///< Hardware Assert 43.
    uint32_t HWAssert12:1;                    ///< Hardware Assert 44.
    uint32_t HWAssert13:1;                    ///< Hardware Assert 45.
    uint32_t HWAssert14:1;                    ///< Hardware Assert 46.
    uint32_t HWAssert15:1;                    ///< Hardware Assert 47.
    uint32_t HWAssert16:1;                    ///< Hardware Assert 48.
    uint32_t HWAssert17:1;                    ///< Hardware Assert 49.
    uint32_t HWAssert18:1;                    ///< Hardware Assert 50.
    uint32_t HWAssert19:1;                    ///< Hardware Assert 51.
    uint32_t HWAssert20:1;                    ///< Hardware Assert 52.
    uint32_t HWAssert21:1;                    ///< Hardware Assert 53.
    uint32_t HWAssert22:1;                    ///< Hardware Assert 54.
    uint32_t HWAssert23:1;                    ///< Hardware Assert 55.
    uint32_t HWAssert24:1;                    ///< Hardware Assert 56.
    uint32_t HWAssert25:1;                    ///< Hardware Assert 57.
    uint32_t HWAssert26:1;                    ///< Hardware Assert 58.
    uint32_t HWAssert27:1;                    ///< Hardware Assert 59.
    uint32_t HWAssert28:1;                    ///< Hardware Assert 60.
    uint32_t HWAssert29:1;                    ///< Hardware Assert 61.
    uint32_t HWAssert30:1;                    ///< Hardware Assert 62.
    uint32_t HWAssert31:1;                    ///< Hardware Assert 63.
  } Field;
  uint32_t  Value;
} HARDWARE_ASSERT_STATUS_HIGH_REGISTER;

/* Hardware Assert Mask Low Register */
#define HARDWAREASSERTMASKLOW_FUNC 0x3
#define HARDWAREASSERTMASKLOW_REG  0x8F8

/// Hardware Assert Mask Low Register
typedef union {
  struct {
    uint32_t HWAssertMsk0:1;                  ///< Hardware Assert Mask 0
    uint32_t HWAssertMsk1:1;                  ///< Hardware Assert Mask 1
    uint32_t HWAssertMsk2:1;                  ///< Hardware Assert Mask 2
    uint32_t HWAssertMsk3:1;                  ///< Hardware Assert Mask 3
    uint32_t HWAssertMsk4:1;                  ///< Hardware Assert Mask 4
    uint32_t HWAssertMsk5:1;                  ///< Hardware Assert Mask 5
    uint32_t HWAssertMsk6:1;                  ///< Hardware Assert Mask 6
    uint32_t HWAssertMsk7:1;                  ///< Hardware Assert Mask 7
    uint32_t HWAssertMsk8:1;                  ///< Hardware Assert Mask 8
    uint32_t HWAssertMsk9:1;                  ///< Hardware Assert Mask 9
    uint32_t HWAssertMsk10:1;                 ///< Hardware Assert Mask 10
    uint32_t HWAssertMsk11:1;                 ///< Hardware Assert Mask 11
    uint32_t HWAssertMsk12:1;                 ///< Hardware Assert Mask 12
    uint32_t HWAssertMsk13:1;                 ///< Hardware Assert Mask 13
    uint32_t HWAssertMsk14:1;                 ///< Hardware Assert Mask 14
    uint32_t HWAssertMsk15:1;                 ///< Hardware Assert Mask 15
    uint32_t HWAssertMsk16:1;                 ///< Hardware Assert Mask 16
    uint32_t HWAssertMsk17:1;                 ///< Hardware Assert Mask 17
    uint32_t HWAssertMsk18:1;                 ///< Hardware Assert Mask 18
    uint32_t HWAssertMsk19:1;                 ///< Hardware Assert Mask 19
    uint32_t HWAssertMsk20:1;                 ///< Hardware Assert Mask 20
    uint32_t HWAssertMsk21:1;                 ///< Hardware Assert Mask 21
    uint32_t HWAssertMsk22:1;                 ///< Hardware Assert Mask 22
    uint32_t HWAssertMsk23:1;                 ///< Hardware Assert Mask 23
    uint32_t HWAssertMsk24:1;                 ///< Hardware Assert Mask 24
    uint32_t HWAssertMsk25:1;                 ///< Hardware Assert Mask 25
    uint32_t HWAssertMsk26:1;                 ///< Hardware Assert Mask 26
    uint32_t HWAssertMsk27:1;                 ///< Hardware Assert Mask 27
    uint32_t HWAssertMsk28:1;                 ///< Hardware Assert Mask 28
    uint32_t HWAssertMsk29:1;                 ///< Hardware Assert Mask 29
    uint32_t HWAssertMsk30:1;                 ///< Hardware Assert Mask 30
    uint32_t HWAssertMsk31:1;                 ///< Hardware Assert Mask 31
  } Field;
  uint32_t  Value;
} HARDWARE_ASSERT_MASK_LOW_REGISTER;

/* Hardware Assert Mask High Register */
#define HARDWAREASSERTMASKHIGH_FUNC 0x3
#define HARDWAREASSERTMASKHIGH_REG  0x8FC

/// Hardware Assert Mask High Register
typedef union {
  struct {
    uint32_t HWAssertMsk0:1;                  ///< Hardware Assert Mask 32.
    uint32_t HWAssertMsk1:1;                  ///< Hardware Assert Mask 33.
    uint32_t HWAssertMsk2:1;                  ///< Hardware Assert Mask 34.
    uint32_t HWAssertMsk3:1;                  ///< Hardware Assert Mask 35.
    uint32_t HWAssertMsk4:1;                  ///< Hardware Assert Mask 36.
    uint32_t HWAssertMsk5:1;                  ///< Hardware Assert Mask 37.
    uint32_t HWAssertMsk6:1;                  ///< Hardware Assert Mask 38.
    uint32_t HWAssertMsk7:1;                  ///< Hardware Assert Mask 39.
    uint32_t HWAssertMsk8:1;                  ///< Hardware Assert Mask 40.
    uint32_t HWAssertMsk9:1;                  ///< Hardware Assert Mask 41.
    uint32_t HWAssertMsk10:1;                 ///< Hardware Assert Mask 42.
    uint32_t HWAssertMsk11:1;                 ///< Hardware Assert Mask 43.
    uint32_t HWAssertMsk12:1;                 ///< Hardware Assert Mask 44.
    uint32_t HWAssertMsk13:1;                 ///< Hardware Assert Mask 45.
    uint32_t HWAssertMsk14:1;                 ///< Hardware Assert Mask 46.
    uint32_t HWAssertMsk15:1;                 ///< Hardware Assert Mask 47.
    uint32_t HWAssertMsk16:1;                 ///< Hardware Assert Mask 48.
    uint32_t HWAssertMsk17:1;                 ///< Hardware Assert Mask 49.
    uint32_t HWAssertMsk18:1;                 ///< Hardware Assert Mask 50.
    uint32_t HWAssertMsk19:1;                 ///< Hardware Assert Mask 51.
    uint32_t HWAssertMsk20:1;                 ///< Hardware Assert Mask 52.
    uint32_t HWAssertMsk21:1;                 ///< Hardware Assert Mask 53.
    uint32_t HWAssertMsk22:1;                 ///< Hardware Assert Mask 54.
    uint32_t HWAssertMsk23:1;                 ///< Hardware Assert Mask 55.
    uint32_t HWAssertMsk24:1;                 ///< Hardware Assert Mask 56.
    uint32_t HWAssertMsk25:1;                 ///< Hardware Assert Mask 57.
    uint32_t HWAssertMsk26:1;                 ///< Hardware Assert Mask 58.
    uint32_t HWAssertMsk27:1;                 ///< Hardware Assert Mask 59.
    uint32_t HWAssertMsk28:1;                 ///< Hardware Assert Mask 60.
    uint32_t HWAssertMsk29:1;                 ///< Hardware Assert Mask 61.
    uint32_t HWAssertMsk30:1;                 ///< Hardware Assert Mask 62.
    uint32_t HWAssertMsk31:1;                 ///< Hardware Assert Mask 63.
  } Field;
  uint32_t  Value;
} HARDWARE_ASSERT_MASK_HIGH_REGISTER;

/* Fabric Indirect Configuration Access Address (FICAA) Register */
#define FABRICINDIRECTCONFIGACCESSADDRESS_3_FUNC 0x4
#define FABRICINDIRECTCONFIGACCESSADDRESS_3_REG  0x08C

/// Fabric Indirect Configuration Access Address (FICAA) Register
/*
 * The exact layout of this bitfield can differ across programs, namely the width of certain bitfields. However,
 * within the scope of OpenSIL, this particular layout fits all applications, so it is kept common.
 */
typedef union {
  struct {
    uint32_t CfgRegInstAccEn:1;               ///< Configuration Register Instance Access Enable.
    uint32_t IndCfgAccRegNum:10;              ///< Register number of the local DF Configuration register to be
                                              ///  accessed through DF::FabricIndirectConfigAccessDataLo and
                                              ///  DF::FabricIndirectConfigAccessDataHi.The register offset address is
                                              ///  {IndCfgAccRegNum[11:2], 00b}.
    uint32_t IndCfgAccFuncNum:3;              ///< Function number of the local DF configuration register to be
                                              ///  accessed through DF::FabricIndirectConfigAccessDataLo and
                                              ///  DF::FabricIndirectConfigAccessDataHi.
    uint32_t SixtyFourBitRegEn:1;             ///< Specifies 64-bit register access using both the upper and lower
                                              ///  32 bit data registers. When set to 1, this field causes
                                              ///  PIE to hold arbitration until two transactions have been made
                                              ///  through the associated DF::FabricIndirectConfigAccessDataLo,
                                              ///  DF::FabricIndirectConfigAccessDataHi register pair. This bit ensures
                                              ///  integrity of operations to 64-bit registers
                                              ///  (implemented in Data Fabric as two contiguous-offset 32-bit
                                              ///  registers) by preventing FCI arbitration in the PIE, as that
                                              ///  arbitration could result in an unrelated access getting inserted
                                              ///  between the two 32-bit accesses that must be back-to-back to the
                                              ///  64-bit register.
    uint32_t :1;                              ///< Reserved
    uint32_t CfgRegInstID:8;                  ///< Identifies the instanceID of the register to access.
    uint32_t :8;                              ///< Reserved
  } Field;
  uint32_t  Value;
} FABRIC_INDIRECT_CONFIG_ACCESS_ADDRESS_REGISTER;

/* Fabric Indirect Configuration Access Data Lo (FICAD) Register */
#define FABRICINDIRECTCONFIGACCESSDATALO_3_FUNC 0x4
#define FABRICINDIRECTCONFIGACCESSDATALO_3_REG  0x0B8

/// Fabric Indirect Configuration Access Data Lo (FICAD) Register
typedef union {
  struct {
    uint32_t Data:32;                         ///< Indirect Data Lo
  } Field;
  uint32_t  Value;
} FABRIC_INDIRECT_CONFIG_ACCESS_DATA_LO_REGISTER;

/// DRAM Hole Control Register
typedef union {
  struct {
    uint32_t DramHoleValid:1;                 ///< Dram Hole Valid.
    uint32_t :23;                             ///< Reserved
    uint32_t DramHoleBase:8;                  ///< Dram Hole Base[31:24]. Specifies the base address of the IO hole
                                              ///  below the 4GB address level for legacy 32-bit devices.
                                              ///  MMIO hole cannot spam multiple DRAM ranges; it must lie within
                                              ///  a single DRAM address range. DRAM range which spans the
                                              ///  IO hole needs to be hoisted (added) to compensate for the hole.
  } Field;
  uint32_t  Value;
} DRAM_HOLE_CONTROL_REGISTER;

/// DRAM Offset Register
typedef union {
  struct {
    uint32_t HiAddrOffsetEn:1;                ///< Control addition of HiAddrOffset when forming normalized address.
                                              ///  This field must be set to one when HiAddrOffset is non-zero.
    uint32_t HiAddrOffset:24;                 ///< Offset address[51:28] for CS DRAM Address range 1. Specifies the
                                              ///  normalized (DRAM) address at the base of the associated range.
    uint32_t :7;                              ///< Reserved
  } Field;
  uint32_t  Value;
} DRAM_OFFSET_REGISTER;

#pragma pack (pop)
