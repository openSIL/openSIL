/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  SilFabricRegistersBrh.h
 * @brief Data fabric register definitions which are specific to BRH
 *
 */

#pragma once

#include <xSIM.h>

#pragma pack (push, 1)

/* Fabric Block Instance Count Register */
#define FABRICBLOCKINSTANCECOUNT_FUNC 0x0
#define FABRICBLOCKINSTANCECOUNT_REG  0x040

/// Fabric Block Instance Count Register
typedef union {
  struct {
    uint32_t BlkInstCount:10;                 ///< Provides the total number of instances of all the blocks in
                                              ///  the Data Fabric.
    uint32_t :6;                              ///< Reserved
    uint32_t MinorRevision:8;                 ///< Specifies the minor revision of the Data Fabric hardware,
                                              ///  MajorRevision.MinorRevision.
    uint32_t MajorRevision:4;                 ///< Specifies the major revision of the Data Fabric hardware,
                                              ///  MajorRevision.MinorRevision.
    uint32_t :4;                              ///< Reserved
  } Field;
  uint32_t  Value;
} FABRIC_BLOCK_INSTANCE_COUNT_REGISTER;

/* Fabric Block Instance Information 3 Register */
#define FABRICBLOCKINSTANCEINFORMATION3_CSNCSPIEALLM_FUNC 0x0
#define FABRICBLOCKINSTANCEINFORMATION3_CSNCSPIEALLM_REG  0x050

/// Fabric Block Instance Information 3 Register
typedef union {
  struct {
    uint32_t BlockInstanceID:8;               ///< This component's Instance ID
    uint32_t BlockFabricID:8;                 ///< This component's Fabric ID. This field may be updated by PSP through
                                              ///  SMN after boot up if there are no pins allocated on the package
                                              ///  to identify the socket and die. Allocation of bits in this field for
                                              ///  socket, die, and component is specified in registers
                                              ///  DF::SystemFabricIdMask0
                                              ///  and DF::SystemFabricIdMask1.
    uint32_t :16;                             ///< Reserved
  } Field;
  uint32_t  Value;
} FABRIC_BLOCK_INSTANCE_INFORMATION3__CSNCSPIEALLM_REGISTER;

/* Configuration Address Map Control Register */
#define CFGLIMITADDRESS_0_FUNC 0x0
#define CFGLIMITADDRESS_0_REG  0xC84

#define CFGLIMITADDRESS_1_FUNC 0x0
#define CFGLIMITADDRESS_1_REG  0xC8C

/// Configuration Address Map Control Register
typedef union {
  struct {
    uint32_t DstFabricID:8;                   ///< For transactions which hit in this range, this field specifies the
                                              ///  destination FabricID which may be local or on a remote die.
    uint32_t :8;                              ///< Reserved
    uint32_t BusNumLimit:8;                   ///< Bus Number Limit Bits[7:0] for this IO Space.
    uint32_t :8;                              ///< Reserved
  } Field;
  uint32_t  Value;
} CFG_LIMIT_ADDRESS_REGISTER;

/* IO Space Limit Address Register */
#define X86IOLIMITADDRESS_0_FUNC 0x0

/// IO Space Limit Address Register
typedef union {
  struct {
    uint32_t DstFabricID:8;                   ///< For transactions which hit in this IO Space range, this field
                                              ///  specifies the destination FabricID which may be local to the die
                                              ///  or on a remote die.
    uint32_t :8;                              ///< Reserved
    uint32_t IOLimit:13;                      ///< IO Limit address[24:12] for this register's IO Space.
    uint32_t :3;                              ///< Reserved
  } Field;
  uint32_t  Value;
} X86_IOLIMIT_ADDRESS_REGISTER;

/// MMIO Address Control Register
typedef union {
  struct {
    uint32_t RE:1;                            ///< 1: Reads to this address range are enabled.
    uint32_t WE:1;                            ///< 1: Writes to this address range are enabled.
    uint32_t CpuDis:1;                        ///< Control whether CPU accesses get redirected to IO compatibility
                                              ///  space. IO accesses ignore this field.
    uint32_t Np:1;                            ///< Select whether CPU writes to this MMIO range go through posted
                                              ///  or non-posted channel.
    uint32_t :12;                             ///< Reserved
    uint32_t DstFabricID:8;                   ///< For transactions which hit in this range, this field specifies
                                              ///  the destination FabricID of the IOS, which may be local or on a
                                              ///  remote die.
    uint32_t :8;                              ///< Reserved
  } Field;
  uint32_t  Value;
} MMIO_ADDRESS_CONTROL_REGISTER;

/* CCD Enable Register Register */
#define CCDENABLE_FUNC 0x1
#define CCDENABLE_REG  0x104

/// CCD Enable Register Register
typedef union {
  struct {
    uint32_t CCDEn:2;                         ///< Bit position == 1: associated CCD enabled;Bit position == 0:
                                              ///  associated CCD disabled.
    uint32_t :29;                             ///< Reserved
    uint32_t SdpWideEn:1;                     ///< Enables both SDP ports to be connected to one CCD (in wide mode)
  } Field;
  uint32_t  Value;
} CCD_ENABLE_REGISTER;

#define SIL_RESERVED_0267                  0x4
#define SIL_RESERVED_0268                   0x1A8
#define SIL_RESERVED_0269 0x0
#define SIL_RESERVED_0270  0x050
/* Fabric Block Instance Information 0 Register */
#define FABRICBLOCKINSTANCEINFORMATION0_FUNC 0x0
#define FABRICBLOCKINSTANCEINFORMATION0_REG  0x044

/// System Configuration Register
typedef union {
  struct {
    uint32_t MyDieType:2;                     ///< Die Type [1:0] of the die this instance of the register is on.
    uint32_t OtherDieType:2;                  ///< Die Type [1:0] of other die(s) in this system, which is/are not the
                                              ///  same type as this die.
    uint32_t MyNodeMap:1;                     ///< Node Map Type for this node. 0=Local Map Type, 1=Remote Map Type.
    uint32_t :3;                              ///< Reserved
    uint32_t OtherSocketPresent:1;            ///< The other socket is present and enabled in the system. This field
                                              ///  is used by the probe/broadcast MultiCast Mask.
    uint32_t :7;                              ///< Reserved
    uint32_t MyNodeId:8;                      ///< Node Id of the die this instance of the register is on.
    uint32_t :8;                              ///< Reserved
  } Field;
  uint32_t  Value;
} SYSTEM_CFG_REGISTER;

/// CS Target Remap 0 Register A Register
typedef union {
  struct {
    uint32_t RemapCS0:5;                      ///< Physical CS mapped to logical CS0
    uint32_t RemapCS1:5;                      ///< Physical CS mapped to logical CS1
    uint32_t RemapCS2:5;                      ///< Physical CS mapped to logical CS2
    uint32_t RemapCS3:5;                      ///< Physical CS mapped to logical CS3
    uint32_t RemapCS4:5;                      ///< Physical CS mapped to logical CS4
    uint32_t RemapCS5:5;                      ///< Physical CS mapped to logical CS5
    uint32_t :2;                              ///< Reserved
  } Field;
  uint32_t  Value;
} CS_TARGET_REMAP0_A_REGISTER;

/// CS Target Remap 0 Register B Register
typedef union {
  struct {
    uint32_t RemapCS6:5;                      ///< Physical CS mapped to logical CS6
    uint32_t RemapCS7:5;                      ///< Physical CS mapped to logical CS7
    uint32_t RemapCS8:5;                      ///< Physical CS mapped to logical CS8
    uint32_t RemapCS9:5;                      ///< Physical CS mapped to logical CS9
    uint32_t RemapCS10:5;                     ///< Physical CS mapped to logical CS10
    uint32_t RemapCS11:5;                     ///< Physical CS mapped to logical CS11
    uint32_t :2;                              ///< Reserved
  } Field;
  uint32_t  Value;
} CS_TARGET_REMAP0_B_REGISTER;

/* DRAM Base Address Register */
#define DRAMBASEADDRESS_0_FUNC 0x7
#define DRAMBASEADDRESS_0_REG  0x200

/// DRAM Base Address Register
typedef union {
  struct {
    uint32_t DramBaseAddr:28;                 ///< Dram Base address[55:28]
    uint32_t :4;                              ///< Reserved
  } Field;
  uint32_t  Value;
} DRAM_BASE_ADDRESS_REGISTER;

/* DRAM Limit Address Register */
#define DRAMLIMITADDRESS_0_FUNC 0x7
#define DRAMLIMITADDRESS_0_REG  0x204

/// DRAM Limit Address Register
typedef union {
  struct {
    uint32_t DramLimitAddr:28;                ///< DRAM limit address[55:28]
    uint32_t :4;                              ///< Reserved
  } Field;
  uint32_t  Value;
} DRAM_LIMIT_ADDRESS_REGISTER;

/* DRAM Address Control Register */
#define DRAMADDRESSCTL_0_FUNC 0x7
#define DRAMADDRESSCTL_0_REG  0x208

/// DRAM Address Control Register
typedef union {
  struct {
    uint32_t AddrRngVal:1;                    ///< Address range valid (enabled/disabled)
    uint32_t LgcyMmioHoleEn:1;                ///< This field is only permitted to be set in at most one DRAM range.
                                              ///  BIOS sets this bit for an address range that spans
    //   the 4GB boundary and contains a hole for addresses used by legacy
    ///  MMIO. Additional information needs to be programmed;
    //   see DF::DramHoleControl.
    uint32_t SCM:1;                           ///< If set, this field indicates that the addresses described by this
                                              ///  address map are stored in Storage Class Memory.
    uint32_t :1;                              ///< Reserved
    uint32_t RemapEn:1;                       ///< Remap Enable
    uint32_t RemapSel:2;                      ///< Remap Select
    uint32_t HashIntlvCtl4K:1;                ///< Enables inclusion of address bits on the 4KB boundary
                                              ///  (bits 12, 13, 14) in a hashed interleave computation.
    uint32_t HashIntlvCtl64K:1;               ///< Enables inclusion of address bits on the 64KB boundary
                                              ///  (bits 16, 17, 18) in a hashed interleave computation.
    uint32_t HashIntlvCtl2M:1;                ///< Enables inclusion of address bits on the 2MB boundary
                                              ///  (bits 21, 22, 23) in a hashed interleave computation.
    uint32_t HashIntlvCtl1G:1;                ///< Enables inclusion of address bits on the 1GB boundary
                                              ///  (bits 30, 31, 32) in a hashed interleave computation.
    uint32_t DramColSwizzle:1;                ///< Enables swizzle mode which groups 1KB memory blocks to
                                              ///  column select bits
    uint32_t :3;                              ///< Reserved
    uint32_t HashIntlvCtl1T:1;                ///< Enables inclusion of address bits on the 1TB boundary
                                              ///  (bits 40, 41, 42) in a hashed interleave computation.
    uint32_t DstFabricID:8;                   ///< When CS interleaving is disabled (see the DramAddressIntlv
                                              ///  register and its IntLvNumChan field) which provides the
                                              ///  target CS FabricId for this address map.When CS interleaving is
                                              ///  enabled, this field provides the CS Fabric ID where
                                              ///  interleave starts.See 13.7.2.1
                                              ///  [DRAM Address Normalization and Interleave].
    uint32_t :8;                              ///< Reserved
  } Field;
  uint32_t  Value;
} DRAM_ADDRESS_CTL_REGISTER;

#pragma pack (pop)
