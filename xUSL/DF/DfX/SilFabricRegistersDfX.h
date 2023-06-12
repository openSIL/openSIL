/**
 * @file  SilFabricRegistersDfX.h
 * @brief OpenSIL AMD Data Fabric registers definition
 *
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <xSIM.h>

#pragma pack (push, 1)

#define SIL_RESERVED_821 0x5
#define SIL_RESERVED_820 0x104
#define DF_GLBL_CLK_GATER_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, SIL_RESERVED_821, SIL_RESERVED_820))

typedef union {
  struct {
    uint32_t MGCGMode:4;
    uint32_t :28;
  } Field;
  uint32_t Value;
} SIL_RESERVED_825;

/* DF Global Control Register */
#define DFGLOBALCTRL_FUNC 0x3
#define DFGLOBALCTRL_REG  0x044
#define DFGLOBALCTRL_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DFGLOBALCTRL_FUNC, DFGLOBALCTRL_REG))

/* Fabric Block Instance Count Register */
#define FABRICBLOCKINSTANCECOUNT_FUNC 0x0
#define FABRICBLOCKINSTANCECOUNT_REG  0x040
#define FABRICBLOCKINSTANCECOUNT_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FABRICBLOCKINSTANCECOUNT_FUNC, FABRICBLOCKINSTANCECOUNT_REG))

/// Fabric Block Instance Count Register
typedef union {
  struct {
    uint32_t BlkInstCount:8;                  ///< Provides the total number of instances of all the blocks in the Data Fabric.
    uint32_t :8;                              ///< Reserved
    uint32_t MinorRevision:8;                 ///< Specifies the minor revision of the Data Fabric hardware, MajorRevision.MinorRevision.
    uint32_t MajorRevision:4;                 ///< Specifies the major revision of the Data Fabric hardware, MajorRevision.MinorRevision.
    uint32_t :4;                              ///< Reserved
  } Field;
  uint32_t  Value;
} FABRIC_BLOCK_INSTANCE_COUNT_REGISTER;

/* Fabric Block Instance Information 0 Register */
#define FABRICBLOCKINSTANCEINFORMATION0_FUNC 0x0
#define FABRICBLOCKINSTANCEINFORMATION0_REG  0x044
#define FABRICBLOCKINSTANCEINFORMATION0_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FABRICBLOCKINSTANCEINFORMATION0_FUNC, FABRICBLOCKINSTANCEINFORMATION0_REG))

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

/* Fabric Block Instance Information 1 Register */
#define FABRICBLOCKINSTANCEINFORMATION1_FUNC 0x0
#define FABRICBLOCKINSTANCEINFORMATION1_REG  0x048
#define FABRICBLOCKINSTANCEINFORMATION1_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FABRICBLOCKINSTANCEINFORMATION1_FUNC, FABRICBLOCKINSTANCEINFORMATION1_REG))

/// Fabric Block Instance Information 1 Register
typedef union {
  struct {
    uint32_t FtiPort0NeighborInstId:8;        ///< Valid if FabricBlockInstanceInformation0[FtiPortCnt] > 0
    uint32_t FtiPort1NeighborInstId:8;        ///< Valid if FabricBlockInstanceInformation0[FtiPortCnt] > 1
    uint32_t FtiPort2NeighborInstId:8;        ///< Valid if FabricBlockInstanceInformation0[FtiPortCnt] > 2
    uint32_t FtiPort3NeighborInstId:8;        ///< Valid if FabricBlockInstanceInformation0[FtiPortCnt] > 3
  } Field;
  uint32_t  Value;
} FABRIC_BLOCK_INSTANCE_INFORMATION1_REGISTER;

/* Fabric Block Instance Information 2 Register */
#define FABRICBLOCKINSTANCEINFORMATION2_FUNC 0x0
#define FABRICBLOCKINSTANCEINFORMATION2_REG  0x04C
#define FABRICBLOCKINSTANCEINFORMATION2_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FABRICBLOCKINSTANCEINFORMATION2_FUNC, FABRICBLOCKINSTANCEINFORMATION2_REG))

/// Fabric Block Instance Information 2 Register
typedef union {
  struct {
    uint32_t FtiPort4NeighborInstId:8;        ///< Valid if FabricBlockInstanceInformation0[FtiPortCnt] > 4
    uint32_t FtiPort5NeighborInstId:8;        ///< Valid if FabricBlockInstanceInformation0[FtiPortCnt] > 5
    uint32_t :16;                             ///< Reserved
  } Field;
  uint32_t  Value;
} FABRIC_BLOCK_INSTANCE_INFORMATION2_REGISTER;

/* Fabric Block Instance Information 3 Register */
#define FABRICBLOCKINSTANCEINFORMATION3_CSNCSPIEALLM_FUNC 0x0
#define FABRICBLOCKINSTANCEINFORMATION3_CSNCSPIEALLM_REG  0x050
#define FABRICBLOCKINSTANCEINFORMATION3_CSNCSPIEALLM_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FABRICBLOCKINSTANCEINFORMATION3_CSNCSPIEALLM_FUNC, FABRICBLOCKINSTANCEINFORMATION3_CSNCSPIEALLM_REG))

/// Fabric Block Instance Information 3 Register
typedef union {
  struct {
    uint32_t BlockInstanceID:8;               ///< This component's Instance ID
    uint32_t BlockFabricID:12;                ///< This component's Fabric ID. This field may be updated by PSP through SMN after boot up if there are no pins allocated on the package to identify the socket and die. Allocation of bits in this field for socket, die, and component is specified in registers DF::SystemFabricIdMask0 and DF::SystemFabricIdMask1.
    uint32_t :12;                             ///< Reserved
  } Field;
  uint32_t  Value;
} FABRIC_BLOCK_INSTANCE_INFORMATION3__CSNCSPIEALLM_REGISTER;

/* Fabric Block Instance Information 3 Register */
#define FABRICBLOCKINSTANCEINFORMATION3_IOS_FUNC 0x0
#define FABRICBLOCKINSTANCEINFORMATION3_IOS_REG  0x050
#define FABRICBLOCKINSTANCEINFORMATION3_IOS_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FABRICBLOCKINSTANCEINFORMATION3_IOS_FUNC, FABRICBLOCKINSTANCEINFORMATION3_IOS_REG))

/// Fabric Block Instance Information 3 Register
typedef union {
  struct {
    uint32_t BlockInstanceID:8;               ///< This component's Instance ID
    uint32_t BlockFabricID:12;                ///< This component's Fabric ID. This field may be updated by PSP through SMN after boot up if there are no pins allocated on the package to identify the socket and die. Allocation of bits in this field for socket, die, and component is specified in registers DF::SystemFabricIdMask0 and DF::SystemFabricIdMask1.
    uint32_t :12;                             ///< Reserved
  } Field;
  uint32_t  Value;
} FABRIC_BLOCK_INSTANCE_INFORMATION3__IOS_REGISTER;

/* Fabric Block Instance Information 3 Register */
#define FABRICBLOCKINSTANCEINFORMATION3_TCDXPFXCAKE_FUNC 0x0
#define FABRICBLOCKINSTANCEINFORMATION3_TCDXPFXCAKE_REG  0x050
#define FABRICBLOCKINSTANCEINFORMATION3_TCDXPFXCAKE_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FABRICBLOCKINSTANCEINFORMATION3_TCDXPFXCAKE_FUNC, FABRICBLOCKINSTANCEINFORMATION3_TCDXPFXCAKE_REG))

/// Fabric Block Instance Information 3 Register
typedef union {
  struct {
    uint32_t BlockInstanceID:8;               ///< This component's Instance ID
    uint32_t BlockFabricID:12;                ///< TCDX and CAKE do not have Fabric IDs
    uint32_t :12;                             ///< Reserved
  } Field;
  uint32_t  Value;
} FABRIC_BLOCK_INSTANCE_INFORMATION3__TCDXPFXCAKE_REGISTER;

/* DF Capability Register */
#define DFCAPABILITY_FUNC 0x0
#define DFCAPABILITY_REG  0x090
#define DFCAPABILITY_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DFCAPABILITY_FUNC, DFCAPABILITY_REG))

/// DF Capability Register
typedef union {
  struct {
    uint32_t DataPoison:1;                    ///< Software uncorrectable error containment and recovery capability (SUCCOR).
    uint32_t Spf:1;                           ///< Probe filter capability.
    uint32_t :1;                              ///< Reserved
    uint32_t Spare:29;                        ///< Reserved for spare capabilities.
  } Field;
  uint32_t  Value;
} DF_CAPABILITY_REGISTER;

/* Secure Loader Configuration Register */
#define SECURELOADERCONFIG_FUNC 0x0
#define SECURELOADERCONFIG_REG  0x384
#define SECURELOADERCONFIG_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, SECURELOADERCONFIG_FUNC, SECURELOADERCONFIG_REG))

/// Secure Loader Configuration Register
typedef union {
  struct {
    uint32_t SLRgnEnable:1;                   ///< Secure Loader Region Enable
    uint32_t :15;                             ///< Reserved
    uint32_t SLAddr:16;                       ///< Secure Loader Region Address [31:16]. Address [55:32] are defined to be 0.
  } Field;
  uint32_t  Value;
} SECURE_LOADER_CONFIG_REGISTER;

/* Core Coherent Master Configuration Access Control Register */
#define COREMASTERACCESSCTRL_FUNC 0x0
#define COREMASTERACCESSCTRL_REG  0xC00
#define COREMASTERACCESSCTRL_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, COREMASTERACCESSCTRL_FUNC, COREMASTERACCESSCTRL_REG))

/// Core Coherent Master Configuration Access Control Register
typedef union {
  struct {
    uint32_t EnableCf8ExtCfg:1;               ///< Enable PCI extended configuration register access for CFC/CF8 accesses.
    uint32_t DisPciCfgReg:1;                  ///< Disable CFC/CF8 Accesses to IO space. When set to 1, CFC/CF8 accesses are treated as PCI IO space accesses and not PCI configuration accesses.
    uint32_t :30;                             ///< Reserved
  } Field;
  uint32_t  Value;
} CORE_MASTER_ACCESS_CTRL_REGISTER;

/* Config Address Control Register */
#define CFGADDRESSCNTL_FUNC 0x0
#define CFGADDRESSCNTL_REG  0xC04
#define CFGADDRESSCNTL_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CFGADDRESSCNTL_FUNC, CFGADDRESSCNTL_REG))

/// Config Address Control Register
typedef union {
  struct {
    uint32_t SecBusNum:8;                     ///< Specifies the configuration-space bus number of the IO Link.
    uint32_t :24;                             ///< Reserved
  } Field;
  uint32_t  Value;
} CFG_ADDRESS_CNTL_REGISTER;

/* VGA Enable Register */
#define VGAEN_FUNC 0x0
#define VGAEN_REG  0xC08
#define VGAEN_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, VGAEN_FUNC, VGAEN_REG))

/// VGA Enable Register
typedef union {
  struct {
    uint32_t VgaEn_VE:1;                      ///< VGA Enable. The VGA-compatible address space is:(1) the MMIO range A_0000h through B_FFFFh(2) IO space accesses in which address bits[9:0] range from 3B0h to 3BBh or 3C0h to 3DFh. Address bits[15:10] are not decoded; this only applies to the first 64 KB of IO space (i.e., address bits[24:16] must be zero.)
    uint32_t VgaEn_NP:1;                      ///< VGA Enable register Non-Posted configuration bit. Steers CPU Write requests to posted or non-posted write channel.
    uint32_t VgaEn_CpuDis:1;                  ///< VGA MMIO access by CPU control.
    uint32_t :1;                              ///< Reserved
    uint32_t VgaEn_DstFabricID:12;            ///< For transactions which hit in this range, this field specifies the destination FabricID which may be local or on a remote die.
    uint32_t :16;                             ///< Reserved
  } Field;
  uint32_t  Value;
} VGAEN_REGISTER;

/* Configuration Bus-0 Type-1 Destination Fabric ID Register-0 Register */
#define CFGBUS0TYPE1DSTFABRICIDREG0_FUNC 0x0
#define CFGBUS0TYPE1DSTFABRICIDREG0_REG  0xC20
#define CFGBUS0TYPE1DSTFABRICIDREG0_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CFGBUS0TYPE1DSTFABRICIDREG0_FUNC, CFGBUS0TYPE1DSTFABRICIDREG0_REG))

/// Configuration Bus-0 Type-1 Destination Fabric ID Register-0 Register
typedef union {
  struct {
    uint32_t CfgBus0Type1_Dev24DstFabricID:12;///< Specifies the destination for Device Number 24, which can be PIE or IOMS Fabric ID for the Node
    uint32_t :4;                              ///< Reserved
    uint32_t CfgBus0Type1_Dev25DstFabricID:12;///< Specifies the destination for Device Number 25, which can be PIE or IOMS Fabric ID for the Node
    uint32_t :4;                              ///< Reserved
  } Field;
  uint32_t  Value;
} CFG_BUS0_TYPE1_DST_FABRIC_IDREG0_REGISTER;

/* Configuration Bus0 Type1 Destination Fabric ID Register-1 Register */
#define CFGBUS0TYPE1DSTFABRICIDREG1_FUNC 0x0
#define CFGBUS0TYPE1DSTFABRICIDREG1_REG  0xC24
#define CFGBUS0TYPE1DSTFABRICIDREG1_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CFGBUS0TYPE1DSTFABRICIDREG1_FUNC, CFGBUS0TYPE1DSTFABRICIDREG1_REG))

/// Configuration Bus0 Type1 Destination Fabric ID Register-1 Register
typedef union {
  struct {
    uint32_t CfgBus0Type1_Dev26DstFabricID:12;///< Specifies the destination for Device Number 26, which can be PIE or IOMS Fabric ID for the Node
    uint32_t :4;                              ///< Reserved
    uint32_t CfgBus0Type1_Dev27DstFabricID:12;///< Specifies the destination for Device Number 27, which can be PIE or IOMS Fabric ID for the Node
    uint32_t :4;                              ///< Reserved
  } Field;
  uint32_t  Value;
} CFG_BUS0_TYPE1_DST_FABRIC_IDREG1_REGISTER;

/* Configuration Bus-0 Type-1 Destination Fabric ID Register-2 Register */
#define CFGBUS0TYPE1DSTFABRICIDREG2_FUNC 0x0
#define CFGBUS0TYPE1DSTFABRICIDREG2_REG  0xC28
#define CFGBUS0TYPE1DSTFABRICIDREG2_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CFGBUS0TYPE1DSTFABRICIDREG2_FUNC, CFGBUS0TYPE1DSTFABRICIDREG2_REG))

/// Configuration Bus-0 Type-1 Destination Fabric ID Register-2 Register
typedef union {
  struct {
    uint32_t CfgBus0Type1_Dev28DstFabricID:12;///< Specifies the destination for Device Number 28, which can be PIE or IOMS Fabric ID for the Node
    uint32_t :4;                              ///< Reserved
    uint32_t CfgBus0Type1_Dev29DstFabricID:12;///< Specifies the destination for Device Number 29, which can be PIE or IOMS Fabric ID for the Node
    uint32_t :4;                              ///< Reserved
  } Field;
  uint32_t  Value;
} CFG_BUS0_TYPE1_DST_FABRIC_IDREG2_REGISTER;

/* Configuration Bus0 Type1 Destination Fabric ID Register-3 Register */
#define CFGBUS0TYPE1DSTFABRICIDREG3_FUNC 0x0
#define CFGBUS0TYPE1DSTFABRICIDREG3_REG  0xC2C
#define CFGBUS0TYPE1DSTFABRICIDREG3_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CFGBUS0TYPE1DSTFABRICIDREG3_FUNC, CFGBUS0TYPE1DSTFABRICIDREG3_REG))

/// Configuration Bus0 Type1 Destination Fabric ID Register-3 Register
typedef union {
  struct {
    uint32_t CfgBus0Type1_Dev30DstFabricID:12;///< Specifies the destination for Device Number 30, which can be PIE or IOMS Fabric ID for the Node
    uint32_t :4;                              ///< Reserved
    uint32_t CfgBus0Type1_Dev31DstFabricID:12;///< Specifies the destination for Device Number 31, which can be PIE or IOMS Fabric ID for the Node
    uint32_t :4;                              ///< Reserved
  } Field;
  uint32_t  Value;
} CFG_BUS0_TYPE1_DST_FABRIC_IDREG3_REGISTER;

/* Configuration Base Address Maps Register */
#define CFGBASEADDRESS_0_FUNC 0x0
#define CFGBASEADDRESS_0_REG  0xC80
#define CFGBASEADDRESS_0_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CFGBASEADDRESS_0_FUNC, CFGBASEADDRESS_0_REG))

#define CFGBASEADDRESS_1_FUNC 0x0
#define CFGBASEADDRESS_1_REG  0xC88
#define CFGBASEADDRESS_1_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CFGBASEADDRESS_1_FUNC, CFGBASEADDRESS_1_REG))

#define CFGBASEADDRESS_2_FUNC 0x0
#define CFGBASEADDRESS_2_REG  0xC90
#define CFGBASEADDRESS_2_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CFGBASEADDRESS_2_FUNC, CFGBASEADDRESS_2_REG))

#define CFGBASEADDRESS_3_FUNC 0x0
#define CFGBASEADDRESS_3_REG  0xC98
#define CFGBASEADDRESS_3_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CFGBASEADDRESS_3_FUNC, CFGBASEADDRESS_3_REG))

#define CFGBASEADDRESS_4_FUNC 0x0
#define CFGBASEADDRESS_4_REG  0xCA0
#define CFGBASEADDRESS_4_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CFGBASEADDRESS_4_FUNC, CFGBASEADDRESS_4_REG))

#define CFGBASEADDRESS_5_FUNC 0x0
#define CFGBASEADDRESS_5_REG  0xCA8
#define CFGBASEADDRESS_5_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CFGBASEADDRESS_5_FUNC, CFGBASEADDRESS_5_REG))

#define CFGBASEADDRESS_6_FUNC 0x0
#define CFGBASEADDRESS_6_REG  0xCB0
#define CFGBASEADDRESS_6_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CFGBASEADDRESS_6_FUNC, CFGBASEADDRESS_6_REG))

#define CFGBASEADDRESS_7_FUNC 0x0
#define CFGBASEADDRESS_7_REG  0xCB8
#define CFGBASEADDRESS_7_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CFGBASEADDRESS_7_FUNC, CFGBASEADDRESS_7_REG))

/// Configuration Base Address Maps Register
typedef union {
  struct {
    uint32_t RE:1;                            ///< Read Enable.
    uint32_t WE:1;                            ///< Write Enable.
    uint32_t :6;                              ///< Reserved
    uint32_t SegmentNum:8;                    ///< Segment Number for this configuration-space address map.
    uint32_t BusNumBase:8;                    ///< Bus Number Base Bits[7:0] for this configuration-space address map. See DF::CfgAddressCntl[SecBusNum].
    uint32_t :8;                              ///< Reserved
  } Field;
  uint32_t  Value;
} CFG_BASE_ADDRESS_REGISTER;

/* Configuration Address Map Control Register */
#define CFGLIMITADDRESS_0_FUNC 0x0
#define CFGLIMITADDRESS_0_REG  0xC84
#define CFGLIMITADDRESS_0_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CFGLIMITADDRESS_0_FUNC, CFGLIMITADDRESS_0_REG))

#define CFGLIMITADDRESS_1_FUNC 0x0
#define CFGLIMITADDRESS_1_REG  0xC8C
#define CFGLIMITADDRESS_1_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CFGLIMITADDRESS_1_FUNC, CFGLIMITADDRESS_1_REG))

#define CFGLIMITADDRESS_2_FUNC 0x0
#define CFGLIMITADDRESS_2_REG  0xC94
#define CFGLIMITADDRESS_2_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CFGLIMITADDRESS_2_FUNC, CFGLIMITADDRESS_2_REG))

#define CFGLIMITADDRESS_3_FUNC 0x0
#define CFGLIMITADDRESS_3_REG  0xC9C
#define CFGLIMITADDRESS_3_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CFGLIMITADDRESS_3_FUNC, CFGLIMITADDRESS_3_REG))

#define CFGLIMITADDRESS_4_FUNC 0x0
#define CFGLIMITADDRESS_4_REG  0xCA4
#define CFGLIMITADDRESS_4_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CFGLIMITADDRESS_4_FUNC, CFGLIMITADDRESS_4_REG))

#define CFGLIMITADDRESS_5_FUNC 0x0
#define CFGLIMITADDRESS_5_REG  0xCAC
#define CFGLIMITADDRESS_5_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CFGLIMITADDRESS_5_FUNC, CFGLIMITADDRESS_5_REG))

#define CFGLIMITADDRESS_6_FUNC 0x0
#define CFGLIMITADDRESS_6_REG  0xCB4
#define CFGLIMITADDRESS_6_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CFGLIMITADDRESS_6_FUNC, CFGLIMITADDRESS_6_REG))

#define CFGLIMITADDRESS_7_FUNC 0x0
#define CFGLIMITADDRESS_7_REG  0xCBC
#define CFGLIMITADDRESS_7_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CFGLIMITADDRESS_7_FUNC, CFGLIMITADDRESS_7_REG))

/// Configuration Address Map Control Register
typedef union {
  struct {
    uint32_t DstFabricID:12;                  ///< For transactions which hit in this range, this field specifies the destination FabricID which may be local or on a remote die.
    uint32_t :4;                              ///< Reserved
    uint32_t BusNumLimit:8;                   ///< Bus Number Limit Bits[7:0] for this IO Space.
    uint32_t :8;                              ///< Reserved
  } Field;
  uint32_t  Value;
} CFG_LIMIT_ADDRESS_REGISTER;

/* IO Space Base Address Register */
#define X86IOBASEADDRESS_0_FUNC 0x0
#define X86IOBASEADDRESS_0_REG  0xD00
#define X86IOBASEADDRESS_0_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, X86IOBASEADDRESS_0_FUNC, X86IOBASEADDRESS_0_REG))

#define X86IOBASEADDRESS_1_FUNC 0x0
#define X86IOBASEADDRESS_1_REG  0xD08
#define X86IOBASEADDRESS_1_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, X86IOBASEADDRESS_1_FUNC, X86IOBASEADDRESS_1_REG))

#define X86IOBASEADDRESS_2_FUNC 0x0
#define X86IOBASEADDRESS_2_REG  0xD10
#define X86IOBASEADDRESS_2_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, X86IOBASEADDRESS_2_FUNC, X86IOBASEADDRESS_2_REG))

#define X86IOBASEADDRESS_3_FUNC 0x0
#define X86IOBASEADDRESS_3_REG  0xD18
#define X86IOBASEADDRESS_3_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, X86IOBASEADDRESS_3_FUNC, X86IOBASEADDRESS_3_REG))

#define X86IOBASEADDRESS_4_FUNC 0x0
#define X86IOBASEADDRESS_4_REG  0xD20
#define X86IOBASEADDRESS_4_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, X86IOBASEADDRESS_4_FUNC, X86IOBASEADDRESS_4_REG))

#define X86IOBASEADDRESS_5_FUNC 0x0
#define X86IOBASEADDRESS_5_REG  0xD28
#define X86IOBASEADDRESS_5_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, X86IOBASEADDRESS_5_FUNC, X86IOBASEADDRESS_5_REG))

#define X86IOBASEADDRESS_6_FUNC 0x0
#define X86IOBASEADDRESS_6_REG  0xD30
#define X86IOBASEADDRESS_6_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, X86IOBASEADDRESS_6_FUNC, X86IOBASEADDRESS_6_REG))

#define X86IOBASEADDRESS_7_FUNC 0x0
#define X86IOBASEADDRESS_7_REG  0xD38
#define X86IOBASEADDRESS_7_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, X86IOBASEADDRESS_7_FUNC, X86IOBASEADDRESS_7_REG))

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

/* IO Space Limit Address Register */
#define X86IOLIMITADDRESS_0_FUNC 0x0
#define X86IOLIMITADDRESS_0_REG  0xD04
#define X86IOLIMITADDRESS_0_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, X86IOLIMITADDRESS_0_FUNC, X86IOLIMITADDRESS_0_REG))

#define X86IOLIMITADDRESS_1_FUNC 0x0
#define X86IOLIMITADDRESS_1_REG  0xD0C
#define X86IOLIMITADDRESS_1_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, X86IOLIMITADDRESS_1_FUNC, X86IOLIMITADDRESS_1_REG))

#define X86IOLIMITADDRESS_2_FUNC 0x0
#define X86IOLIMITADDRESS_2_REG  0xD14
#define X86IOLIMITADDRESS_2_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, X86IOLIMITADDRESS_2_FUNC, X86IOLIMITADDRESS_2_REG))

#define X86IOLIMITADDRESS_3_FUNC 0x0
#define X86IOLIMITADDRESS_3_REG  0xD1C
#define X86IOLIMITADDRESS_3_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, X86IOLIMITADDRESS_3_FUNC, X86IOLIMITADDRESS_3_REG))

#define X86IOLIMITADDRESS_4_FUNC 0x0
#define X86IOLIMITADDRESS_4_REG  0xD24
#define X86IOLIMITADDRESS_4_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, X86IOLIMITADDRESS_4_FUNC, X86IOLIMITADDRESS_4_REG))

#define X86IOLIMITADDRESS_5_FUNC 0x0
#define X86IOLIMITADDRESS_5_REG  0xD2C
#define X86IOLIMITADDRESS_5_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, X86IOLIMITADDRESS_5_FUNC, X86IOLIMITADDRESS_5_REG))

#define X86IOLIMITADDRESS_6_FUNC 0x0
#define X86IOLIMITADDRESS_6_REG  0xD34
#define X86IOLIMITADDRESS_6_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, X86IOLIMITADDRESS_6_FUNC, X86IOLIMITADDRESS_6_REG))

#define X86IOLIMITADDRESS_7_FUNC 0x0
#define X86IOLIMITADDRESS_7_REG  0xD3C
#define X86IOLIMITADDRESS_7_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, X86IOLIMITADDRESS_7_FUNC, X86IOLIMITADDRESS_7_REG))

/// IO Space Limit Address Register
typedef union {
  struct {
    uint32_t DstFabricID:12;                  ///< For transactions which hit in this IO Space range, this field specifies the destination FabricID which may be local to the die or on a remote die.
    uint32_t :4;                              ///< Reserved
    uint32_t IOLimit:13;                      ///< IO Limit address[24:12] for this register's IO Space.
    uint32_t :3;                              ///< Reserved
  } Field;
  uint32_t  Value;
} X86_IOLIMIT_ADDRESS_REGISTER;

/* MMIO Base Address Register */
#define MMIOBASEADDRESS_0_FUNC 0x0
#define MMIOBASEADDRESS_0_REG  0xD80
#define MMIOBASEADDRESS_0_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOBASEADDRESS_0_FUNC, MMIOBASEADDRESS_0_REG))

#define MMIOBASEADDRESS_1_FUNC 0x0
#define MMIOBASEADDRESS_1_REG  0xD90
#define MMIOBASEADDRESS_1_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOBASEADDRESS_1_FUNC, MMIOBASEADDRESS_1_REG))

#define MMIOBASEADDRESS_2_FUNC 0x0
#define MMIOBASEADDRESS_2_REG  0xDA0
#define MMIOBASEADDRESS_2_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOBASEADDRESS_2_FUNC, MMIOBASEADDRESS_2_REG))

#define MMIOBASEADDRESS_3_FUNC 0x0
#define MMIOBASEADDRESS_3_REG  0xDB0
#define MMIOBASEADDRESS_3_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOBASEADDRESS_3_FUNC, MMIOBASEADDRESS_3_REG))

#define MMIOBASEADDRESS_4_FUNC 0x0
#define MMIOBASEADDRESS_4_REG  0xDC0
#define MMIOBASEADDRESS_4_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOBASEADDRESS_4_FUNC, MMIOBASEADDRESS_4_REG))

#define MMIOBASEADDRESS_5_FUNC 0x0
#define MMIOBASEADDRESS_5_REG  0xDD0
#define MMIOBASEADDRESS_5_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOBASEADDRESS_5_FUNC, MMIOBASEADDRESS_5_REG))

#define MMIOBASEADDRESS_6_FUNC 0x0
#define MMIOBASEADDRESS_6_REG  0xDE0
#define MMIOBASEADDRESS_6_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOBASEADDRESS_6_FUNC, MMIOBASEADDRESS_6_REG))

#define MMIOBASEADDRESS_7_FUNC 0x0
#define MMIOBASEADDRESS_7_REG  0xDF0
#define MMIOBASEADDRESS_7_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOBASEADDRESS_7_FUNC, MMIOBASEADDRESS_7_REG))

#define MMIOBASEADDRESS_8_FUNC 0x0
#define MMIOBASEADDRESS_8_REG  0xE00
#define MMIOBASEADDRESS_8_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOBASEADDRESS_8_FUNC, MMIOBASEADDRESS_8_REG))

#define MMIOBASEADDRESS_9_FUNC 0x0
#define MMIOBASEADDRESS_9_REG  0xE10
#define MMIOBASEADDRESS_9_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOBASEADDRESS_9_FUNC, MMIOBASEADDRESS_9_REG))

#define MMIOBASEADDRESS_10_FUNC 0x0
#define MMIOBASEADDRESS_10_REG  0xE20
#define MMIOBASEADDRESS_10_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOBASEADDRESS_10_FUNC, MMIOBASEADDRESS_10_REG))

#define MMIOBASEADDRESS_11_FUNC 0x0
#define MMIOBASEADDRESS_11_REG  0xE30
#define MMIOBASEADDRESS_11_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOBASEADDRESS_11_FUNC, MMIOBASEADDRESS_11_REG))

#define MMIOBASEADDRESS_12_FUNC 0x0
#define MMIOBASEADDRESS_12_REG  0xE40
#define MMIOBASEADDRESS_12_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOBASEADDRESS_12_FUNC, MMIOBASEADDRESS_12_REG))

#define MMIOBASEADDRESS_13_FUNC 0x0
#define MMIOBASEADDRESS_13_REG  0xE50
#define MMIOBASEADDRESS_13_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOBASEADDRESS_13_FUNC, MMIOBASEADDRESS_13_REG))

#define MMIOBASEADDRESS_14_FUNC 0x0
#define MMIOBASEADDRESS_14_REG  0xE60
#define MMIOBASEADDRESS_14_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOBASEADDRESS_14_FUNC, MMIOBASEADDRESS_14_REG))

#define MMIOBASEADDRESS_15_FUNC 0x0
#define MMIOBASEADDRESS_15_REG  0xE70
#define MMIOBASEADDRESS_15_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOBASEADDRESS_15_FUNC, MMIOBASEADDRESS_15_REG))

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
#define MMIOLIMITADDRESS_0_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOLIMITADDRESS_0_FUNC, MMIOLIMITADDRESS_0_REG))

#define MMIOLIMITADDRESS_1_FUNC 0x0
#define MMIOLIMITADDRESS_1_REG  0xD94
#define MMIOLIMITADDRESS_1_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOLIMITADDRESS_1_FUNC, MMIOLIMITADDRESS_1_REG))

#define MMIOLIMITADDRESS_2_FUNC 0x0
#define MMIOLIMITADDRESS_2_REG  0xDA4
#define MMIOLIMITADDRESS_2_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOLIMITADDRESS_2_FUNC, MMIOLIMITADDRESS_2_REG))

#define MMIOLIMITADDRESS_3_FUNC 0x0
#define MMIOLIMITADDRESS_3_REG  0xDB4
#define MMIOLIMITADDRESS_3_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOLIMITADDRESS_3_FUNC, MMIOLIMITADDRESS_3_REG))

#define MMIOLIMITADDRESS_4_FUNC 0x0
#define MMIOLIMITADDRESS_4_REG  0xDC4
#define MMIOLIMITADDRESS_4_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOLIMITADDRESS_4_FUNC, MMIOLIMITADDRESS_4_REG))

#define MMIOLIMITADDRESS_5_FUNC 0x0
#define MMIOLIMITADDRESS_5_REG  0xDD4
#define MMIOLIMITADDRESS_5_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOLIMITADDRESS_5_FUNC, MMIOLIMITADDRESS_5_REG))

#define MMIOLIMITADDRESS_6_FUNC 0x0
#define MMIOLIMITADDRESS_6_REG  0xDE4
#define MMIOLIMITADDRESS_6_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOLIMITADDRESS_6_FUNC, MMIOLIMITADDRESS_6_REG))

#define MMIOLIMITADDRESS_7_FUNC 0x0
#define MMIOLIMITADDRESS_7_REG  0xDFX
#define MMIOLIMITADDRESS_7_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOLIMITADDRESS_7_FUNC, MMIOLIMITADDRESS_7_REG))

#define MMIOLIMITADDRESS_8_FUNC 0x0
#define MMIOLIMITADDRESS_8_REG  0xE04
#define MMIOLIMITADDRESS_8_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOLIMITADDRESS_8_FUNC, MMIOLIMITADDRESS_8_REG))

#define MMIOLIMITADDRESS_9_FUNC 0x0
#define MMIOLIMITADDRESS_9_REG  0xE14
#define MMIOLIMITADDRESS_9_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOLIMITADDRESS_9_FUNC, MMIOLIMITADDRESS_9_REG))

#define MMIOLIMITADDRESS_10_FUNC 0x0
#define MMIOLIMITADDRESS_10_REG  0xE24
#define MMIOLIMITADDRESS_10_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOLIMITADDRESS_10_FUNC, MMIOLIMITADDRESS_10_REG))

#define MMIOLIMITADDRESS_11_FUNC 0x0
#define MMIOLIMITADDRESS_11_REG  0xE34
#define MMIOLIMITADDRESS_11_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOLIMITADDRESS_11_FUNC, MMIOLIMITADDRESS_11_REG))

#define MMIOLIMITADDRESS_12_FUNC 0x0
#define MMIOLIMITADDRESS_12_REG  0xE44
#define MMIOLIMITADDRESS_12_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOLIMITADDRESS_12_FUNC, MMIOLIMITADDRESS_12_REG))

#define MMIOLIMITADDRESS_13_FUNC 0x0
#define MMIOLIMITADDRESS_13_REG  0xE54
#define MMIOLIMITADDRESS_13_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOLIMITADDRESS_13_FUNC, MMIOLIMITADDRESS_13_REG))

#define MMIOLIMITADDRESS_14_FUNC 0x0
#define MMIOLIMITADDRESS_14_REG  0xE64
#define MMIOLIMITADDRESS_14_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOLIMITADDRESS_14_FUNC, MMIOLIMITADDRESS_14_REG))

#define MMIOLIMITADDRESS_15_FUNC 0x0
#define MMIOLIMITADDRESS_15_REG  0xE74
#define MMIOLIMITADDRESS_15_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOLIMITADDRESS_15_FUNC, MMIOLIMITADDRESS_15_REG))

/// MMIO Limit Address Register
typedef union {
  struct {
    uint32_t MmioLimitAddr:32;                ///< MMIO Limit address[47:16].
  } Field;
  uint32_t  Value;
} MMIO_LIMIT_ADDRESS_REGISTER;

/* MMIO Address Control Register */
#define MMIOADDRESSCONTROL_0_FUNC 0x0
#define MMIOADDRESSCONTROL_0_REG  0xD88
#define MMIOADDRESSCONTROL_0_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOADDRESSCONTROL_0_FUNC, MMIOADDRESSCONTROL_0_REG))

#define MMIOADDRESSCONTROL_1_FUNC 0x0
#define MMIOADDRESSCONTROL_1_REG  0xD98
#define MMIOADDRESSCONTROL_1_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOADDRESSCONTROL_1_FUNC, MMIOADDRESSCONTROL_1_REG))

#define MMIOADDRESSCONTROL_2_FUNC 0x0
#define MMIOADDRESSCONTROL_2_REG  0xDA8
#define MMIOADDRESSCONTROL_2_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOADDRESSCONTROL_2_FUNC, MMIOADDRESSCONTROL_2_REG))

#define MMIOADDRESSCONTROL_3_FUNC 0x0
#define MMIOADDRESSCONTROL_3_REG  0xDB8
#define MMIOADDRESSCONTROL_3_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOADDRESSCONTROL_3_FUNC, MMIOADDRESSCONTROL_3_REG))

#define MMIOADDRESSCONTROL_4_FUNC 0x0
#define MMIOADDRESSCONTROL_4_REG  0xDC8
#define MMIOADDRESSCONTROL_4_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOADDRESSCONTROL_4_FUNC, MMIOADDRESSCONTROL_4_REG))

#define MMIOADDRESSCONTROL_5_FUNC 0x0
#define MMIOADDRESSCONTROL_5_REG  0xDD8
#define MMIOADDRESSCONTROL_5_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOADDRESSCONTROL_5_FUNC, MMIOADDRESSCONTROL_5_REG))

#define MMIOADDRESSCONTROL_6_FUNC 0x0
#define MMIOADDRESSCONTROL_6_REG  0xDE8
#define MMIOADDRESSCONTROL_6_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOADDRESSCONTROL_6_FUNC, MMIOADDRESSCONTROL_6_REG))

#define MMIOADDRESSCONTROL_7_FUNC 0x0
#define MMIOADDRESSCONTROL_7_REG  0xDF8
#define MMIOADDRESSCONTROL_7_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOADDRESSCONTROL_7_FUNC, MMIOADDRESSCONTROL_7_REG))

#define MMIOADDRESSCONTROL_8_FUNC 0x0
#define MMIOADDRESSCONTROL_8_REG  0xE08
#define MMIOADDRESSCONTROL_8_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOADDRESSCONTROL_8_FUNC, MMIOADDRESSCONTROL_8_REG))

#define MMIOADDRESSCONTROL_9_FUNC 0x0
#define MMIOADDRESSCONTROL_9_REG  0xE18
#define MMIOADDRESSCONTROL_9_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOADDRESSCONTROL_9_FUNC, MMIOADDRESSCONTROL_9_REG))

#define MMIOADDRESSCONTROL_10_FUNC 0x0
#define MMIOADDRESSCONTROL_10_REG  0xE28
#define MMIOADDRESSCONTROL_10_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOADDRESSCONTROL_10_FUNC, MMIOADDRESSCONTROL_10_REG))

#define MMIOADDRESSCONTROL_11_FUNC 0x0
#define MMIOADDRESSCONTROL_11_REG  0xE38
#define MMIOADDRESSCONTROL_11_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOADDRESSCONTROL_11_FUNC, MMIOADDRESSCONTROL_11_REG))

#define MMIOADDRESSCONTROL_12_FUNC 0x0
#define MMIOADDRESSCONTROL_12_REG  0xE48
#define MMIOADDRESSCONTROL_12_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOADDRESSCONTROL_12_FUNC, MMIOADDRESSCONTROL_12_REG))

#define MMIOADDRESSCONTROL_13_FUNC 0x0
#define MMIOADDRESSCONTROL_13_REG  0xE58
#define MMIOADDRESSCONTROL_13_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOADDRESSCONTROL_13_FUNC, MMIOADDRESSCONTROL_13_REG))

#define MMIOADDRESSCONTROL_14_FUNC 0x0
#define MMIOADDRESSCONTROL_14_REG  0xE68
#define MMIOADDRESSCONTROL_14_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOADDRESSCONTROL_14_FUNC, MMIOADDRESSCONTROL_14_REG))

#define MMIOADDRESSCONTROL_15_FUNC 0x0
#define MMIOADDRESSCONTROL_15_REG  0xE78
#define MMIOADDRESSCONTROL_15_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOADDRESSCONTROL_15_FUNC, MMIOADDRESSCONTROL_15_REG))

/// MMIO Address Control Register
typedef union {
  struct {
    uint32_t RE:1;                            ///< 1: Reads to this address range are enabled.
    uint32_t WE:1;                            ///< 1: Writes to this address range are enabled.
    uint32_t CpuDis:1;                        ///< Control whether CPU accesses get redirected to IO compatibility space. IO accesses ignore this field.
    uint32_t Np:1;                            ///< Select whether CPU writes to this MMIO range go through posted or non-posted channel.
    uint32_t :12;                             ///< Reserved
    uint32_t DstFabricID:12;                  ///< For transactions which hit in this range, this field specifies the destination FabricID of the IOS, which may be local or on a remote die.
    uint32_t :4;                              ///< Reserved
  } Field;
  uint32_t  Value;
} MMIO_ADDRESS_CONTROL_REGISTER;

/* MMIO Address Extension Register */
#define MMIOEXTADDRESS_0_FUNC 0x0
#define MMIOEXTADDRESS_0_REG  0xD8C
#define MMIOEXTADDRESS_0_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOEXTADDRESS_0_FUNC, MMIOEXTADDRESS_0_REG))

#define MMIOEXTADDRESS_1_FUNC 0x0
#define MMIOEXTADDRESS_1_REG  0xD9C
#define MMIOEXTADDRESS_1_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOEXTADDRESS_1_FUNC, MMIOEXTADDRESS_1_REG))

#define MMIOEXTADDRESS_2_FUNC 0x0
#define MMIOEXTADDRESS_2_REG  0xDAC
#define MMIOEXTADDRESS_2_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOEXTADDRESS_2_FUNC, MMIOEXTADDRESS_2_REG))

#define MMIOEXTADDRESS_3_FUNC 0x0
#define MMIOEXTADDRESS_3_REG  0xDBC
#define MMIOEXTADDRESS_3_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOEXTADDRESS_3_FUNC, MMIOEXTADDRESS_3_REG))

#define MMIOEXTADDRESS_4_FUNC 0x0
#define MMIOEXTADDRESS_4_REG  0xDCC
#define MMIOEXTADDRESS_4_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOEXTADDRESS_4_FUNC, MMIOEXTADDRESS_4_REG))

#define MMIOEXTADDRESS_5_FUNC 0x0
#define MMIOEXTADDRESS_5_REG  0xDDC
#define MMIOEXTADDRESS_5_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOEXTADDRESS_5_FUNC, MMIOEXTADDRESS_5_REG))

#define MMIOEXTADDRESS_6_FUNC 0x0
#define MMIOEXTADDRESS_6_REG  0xDEC
#define MMIOEXTADDRESS_6_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOEXTADDRESS_6_FUNC, MMIOEXTADDRESS_6_REG))

#define MMIOEXTADDRESS_7_FUNC 0x0
#define MMIOEXTADDRESS_7_REG  0xDFC
#define MMIOEXTADDRESS_7_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOEXTADDRESS_7_FUNC, MMIOEXTADDRESS_7_REG))

#define MMIOEXTADDRESS_8_FUNC 0x0
#define MMIOEXTADDRESS_8_REG  0xE0C
#define MMIOEXTADDRESS_8_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOEXTADDRESS_8_FUNC, MMIOEXTADDRESS_8_REG))

#define MMIOEXTADDRESS_9_FUNC 0x0
#define MMIOEXTADDRESS_9_REG  0xE1C
#define MMIOEXTADDRESS_9_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOEXTADDRESS_9_FUNC, MMIOEXTADDRESS_9_REG))

#define MMIOEXTADDRESS_10_FUNC 0x0
#define MMIOEXTADDRESS_10_REG  0xE2C
#define MMIOEXTADDRESS_10_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOEXTADDRESS_10_FUNC, MMIOEXTADDRESS_10_REG))

#define MMIOEXTADDRESS_11_FUNC 0x0
#define MMIOEXTADDRESS_11_REG  0xE3C
#define MMIOEXTADDRESS_11_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOEXTADDRESS_11_FUNC, MMIOEXTADDRESS_11_REG))

#define MMIOEXTADDRESS_12_FUNC 0x0
#define MMIOEXTADDRESS_12_REG  0xE4C
#define MMIOEXTADDRESS_12_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOEXTADDRESS_12_FUNC, MMIOEXTADDRESS_12_REG))

#define MMIOEXTADDRESS_13_FUNC 0x0
#define MMIOEXTADDRESS_13_REG  0xE5C
#define MMIOEXTADDRESS_13_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOEXTADDRESS_13_FUNC, MMIOEXTADDRESS_13_REG))

#define MMIOEXTADDRESS_14_FUNC 0x0
#define MMIOEXTADDRESS_14_REG  0xE6C
#define MMIOEXTADDRESS_14_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOEXTADDRESS_14_FUNC, MMIOEXTADDRESS_14_REG))

#define MMIOEXTADDRESS_15_FUNC 0x0
#define MMIOEXTADDRESS_15_REG  0xE7C
#define MMIOEXTADDRESS_15_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MMIOEXTADDRESS_15_FUNC, MMIOEXTADDRESS_15_REG))

/// MMIO Address Extension Register
typedef union {
  struct {
    uint32_t MmioBaseAddrExt:8;               ///< MMIO Base Address [55:48].
    uint32_t :8;                              ///< Reserved
    uint32_t MmioLimitAddrExt:8;              ///< MMIO Limit Address [55:48].
    uint32_t :8;                              ///< Reserved
  } Field;
  uint32_t  Value;
} MMIO_EXT_ADDRESS_REGISTER;

/* PCS Enable Register */
#define PCSENABLE_FUNC 0x1
#define PCSENABLE_REG  0x110
#define PCSENABLE_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, PCSENABLE_FUNC, PCSENABLE_REG))

/// PCS Enable Register
typedef union {
  struct {
    uint32_t CakePcsEnable:8;           ///< Each bit position associates with a PCS and associated
                                        ///< GMI or xGMI attached to a CAKE;
                                        ///< e.g., bit 0 associates with PCS0.Each bit position set
                                        ///< to 1 enables associated
                                        ///< PCS.Each bit position set to 0 disables the associated PCS.
    uint32_t CcmPcsEnable:12;           ///< Each bit position associates with a PCS and associated GMI2
                                        ///< attached to a CCM;
                                        ///< e.g., bit 0 associates with PCS0.Each bit position set to 1
                                        ///< enables associated
                                        ///< PCS.Each bit position set to 0 disables the associated PCS.
    uint32_t :12;                       ///< Reserved
  } Field;
  uint32_t  Value;
} PCS_ENABLE_REGISTER;

#define SIL_RESERVED_826  (0x0001C000ul)
#define MAKE_SMN_ADDR(FUN, OFF) (((uint32_t) SIL_RESERVED_826) | (((uint32_t)(FUN)) << 10) | ((uint32_t)(OFF)))

#define CCMCONFIG4_FUNC       0x3
#define CCMCONFIG4_REG      0x510
#define SIL_RESERVED_829 (MAKE_SMN_ADDR (CCMCONFIG4_FUNC, CCMCONFIG4_REG))
#define DF_CCMCONFIG4        SIL_RESERVED_829

/// CCM Configuration 4 Register
typedef union {
  struct {
    uint32_t :26;                       ///< Reserved
    uint32_t SdpWideEn:1;               ///< Enables both SDP ports to be connected to one CCD (in wide mode).
    uint32_t :5;                        ///< Reserved
  } Field;
  uint32_t  Value;
} CCM_CONFIG4_REGISTER;

/* CCD Enable Register Register */
#define CCDENABLE_FUNC       0x1
#define CCDENABLE_REG      0x104
#define CCDENABLE_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CCDENABLE_FUNC, CCDENABLE_REG))

/// CCD Enable Register Register
typedef union {
  struct {
    uint32_t CCDEn:2;                    ///< Bit position == 1: associated CCD enabled;Bit position == 0:
                                         ///< associated CCD disabled.
    uint32_t :14;                        ///< Reserved
    uint32_t CCXEn:2;                    ///< Bit position == 1: associated CCX enabled;Bit position == 0:
                                         ///< associated CCX disabled.
    uint32_t :14;                        ///< Reserved
  } Field;
  uint32_t  Value;
} CCD_ENABLE_REGISTER;

#define SIL_RESERVED_824  0x1
#define SIL_RESERVED_822  0x140
#define PHYSICALCOREENABLE0_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, SIL_RESERVED_824, SIL_RESERVED_822))

/// Physical Core Enable Register
typedef union {
  struct {
    uint32_t PhysicalCoreEn:32;               ///< Bit position == 1: associated CPU core enabled;Bit position == 0: associated CPU core disabled.
  } Field;
  uint32_t  Value;
} PHYSICAL_CORE_ENABLE0_REGISTER;

#define PHYSICALCOREENABLE1_FUNC 0x1
#define SIL_RESERVED_823         0x144
#define PHYSICALCOREENABLE1_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, PHYSICALCOREENABLE1_FUNC, SIL_RESERVED_823))

/// Physical Core Enable Register
typedef union {
  struct {
    uint32_t PhysicalCoreEn:32;               ///< Bit position == 1: associated CPU core enabled;Bit position == 0: associated CPU core disabled.
  } Field;
  uint32_t  Value;
} PHYSICAL_CORE_ENABLE1_REGISTER;

/* Physical Core Enable Register */
#define PHYSICALCOREENABLE2_FUNC 0x1
#define PHYSICALCOREENABLE2_REG  0x148
#define PHYSICALCOREENABLE2_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, PHYSICALCOREENABLE2_FUNC, PHYSICALCOREENABLE2_REG))

/// Physical Core Enable Register
typedef union {
  struct {
    uint32_t PhysicalCoreEn:32;               ///< Bit position == 1: associated CPU core enabled;Bit position == 0: associated CPU core disabled.
  } Field;
  uint32_t  Value;
} PHYSICAL_CORE_ENABLE2_REGISTER;

/* DRAM Scrubber Error Address Low Register */
#define DRAMSCRUBERRADDRLO_FUNC 0x2
#define DRAMSCRUBERRADDRLO_REG  0x8F0
#define DRAMSCRUBERRADDRLO_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMSCRUBERRADDRLO_FUNC, DRAMSCRUBERRADDRLO_REG))

/// DRAM Scrubber Error Address Low Register
typedef union {
  struct {
    uint32_t ErrInjEn:1;                      ///< A new command to inject error is triggered by writing a 1. Software should clear DF::DramScrubErrAddrLo[ErrInjDone] and DF::DramScrubErrAddrLo[ErrInjEn] if there are no more errors to inject.
    uint32_t ErrInjDone:1;                    ///< Set by hardware once the write is sent to CS. Software should poll this bit to ensure the error has been injected in the data. Software should clear this bit on a new request.
    uint32_t LockAndDisErrInj:1;              ///< Lock error injection widget. Once this field is set to 1, none of the fields in this register can be written.
    uint32_t :3;                              ///< Reserved
    uint32_t ErrInjAddrLo:26;                 ///< Error injection address low. Address bits[31:6] of the cache line where the error is injected. See DF::DramScrubErrAddrHi[ErrInjAddrHi].
  } Field;
  uint32_t  Value;
} DRAM_SCRUB_ERR_ADDR_LO_REGISTER;

/* DRAM Scrubber Error Address High Register */
#define DRAMSCRUBERRADDRHI_FUNC 0x2
#define DRAMSCRUBERRADDRHI_REG  0x8F4
#define DRAMSCRUBERRADDRHI_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMSCRUBERRADDRHI_FUNC, DRAMSCRUBERRADDRHI_REG))

/// DRAM Scrubber Error Address High Register
typedef union {
  struct {
    uint32_t ErrInjAddrHi:24;                 ///< Error injection address high. Address bits[55:32] of the cache line where the error is injected.
    uint32_t :8;                              ///< Reserved
  } Field;
  uint32_t  Value;
} DRAM_SCRUB_ERR_ADDR_HI_REGISTER;

/* DF Global Control Register */
#define DFGLOBALCTRL_FUNC 0x3
#define DFGLOBALCTRL_REG  0x044
#define DFGLOBALCTRL_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DFGLOBALCTRL_FUNC, DFGLOBALCTRL_REG))

/// DF Global Control Register
typedef union {
  struct {
    uint32_t McastMaskPerNodeCLV:1;           ///< Controls whether Mcast Mask is interpreted as per-node cluster valid bits or the legacy view.
    uint32_t :2;                              ///< Reserved
    uint32_t DisImmSyncFloodOnFatalErr:1;     ///< Determines whether a DF component immediately posts a sync packet to its SDP port when it detects a fatal error or only notifies PIE.
    uint32_t WDTBaseSel:2;                    ///< Data Fabric Watchdog Timer timebase select. The rates provided in the table below are based on a 100 MHz REFCLK, and will scale accordingly based on REFCLK changes.
    uint32_t :2;                              ///< Reserved
    uint32_t PIEWDTCntSel:3;                  ///< Data Fabric PIE SMN Watchdog Timer Count select.
    uint32_t :1;                              ///< Reserved
    uint32_t IOMWDTCntSel:3;                  ///< Data Fabric IOM Watchdog Timer Count select.
    uint32_t :1;                              ///< Reserved
    uint32_t CCMWDTCntSel:3;                  ///< Data Fabric CCM Watchdog Timer Count select.
    uint32_t :1;                              ///< Reserved
    uint32_t DisWdtHwaSyndDbgBus:1;           ///< Selects Debug bus information source on HW Assert/WDT error.
    uint32_t :2;                              ///< Reserved
    uint32_t Reserved_23:1;                   ///< Reserved field
    uint32_t :4;                              ///< Reserved
    uint32_t GNCMWDTCntSel:3;                 ///< Data Fabric GCM and NCM Watchdog Timer Count select.
    uint32_t :1;                              ///< Reserved
  } Field;
  uint32_t  Value;
} DF_GLOBAL_CTRL_REGISTER;

/* PIE Configuration Register */
#define PIECONFIG_FUNC 0x3
#define PIECONFIG_REG  0x180
#define PIECONFIG_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, PIECONFIG_FUNC, PIECONFIG_REG))

/// PIE Configuration Register
typedef union {
  struct {
    uint32_t Reserved_0:1;                    ///< Reserved field
    uint32_t Reserved_1:1;                    ///< Reserved field
    uint32_t Reserved_2:1;                    ///< Reserved field
    uint32_t EnMcaWakeOnSmnAcc:1;             ///< Specify PIE behavior on MCA register access from SMN during DF Cstate
    uint32_t :6;                              ///< Reserved
    uint32_t Reserved_10:1;                   ///< Reserved field
    uint32_t :8;                              ///< Reserved
    uint32_t DisSyncFloodProp:1;              ///< Configure sync flood propagation.
    uint32_t :4;                              ///< Reserved
    uint32_t Reserved_24:2;                   ///< Reserved field
    uint32_t Reserved_26:2;                   ///< Reserved field
    uint32_t Reserved_28:3;                   ///< Reserved field
    uint32_t Reserved_31:1;                   ///< Reserved field
  } Field;
  uint32_t  Value;
} PIECONFIG_REGISTER;

/* PIE Control Register */
#define PIECONTROL_FUNC 0x3
#define PIECONTROL_REG  0x1A0
#define PIECONTROL_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, PIECONTROL_FUNC, PIECONTROL_REG))

/// PIE Control Register
typedef union {
  struct {
    uint32_t :4;                              ///< Reserved
    uint32_t Reserved_4:8;                    ///< Reserved field
    uint32_t :12;                             ///< Reserved
    uint32_t MCADomainSelect:3;               ///< Selects the MCA Domain for the PIE MCA Bank.  Interrupts will be delivered to the MDOMRegisterMDOM
    uint32_t :1;                              ///< Reserved
    uint32_t DVMOpBcstIOSEn:1;                ///< Enables IOS in the DVM Op Broadcast McMask.
    uint32_t DvmSyncBcstIOSEn:1;              ///< Enables IOS in the DVM Sync Bcst McMask and in the expected completion count using SystemComponentCnt[IOMCount].
    uint32_t DvmSyncBcstCCMEn:1;              ///< Enables CCM in the DVM Sync Bcst McMask and in the expected completion count using the SysMgt Join/Leave count.
    uint32_t DvmCompBcstIOSEn:1;              ///< Enables IOS in the DVM Completion Bcst McMask.
  } Field;
  uint32_t  Value;
} PIECONTROL_REGISTER;

/* Hardware Assert Status Low Register */
#define HARDWAREASSERTSTATUSLOW_FUNC 0x3
#define HARDWAREASSERTSTATUSLOW_REG  0x8F0
#define HARDWAREASSERTSTATUSLOW_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, HARDWAREASSERTSTATUSLOW_FUNC, HARDWAREASSERTSTATUSLOW_REG))

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
#define HARDWAREASSERTSTATUSHIGH_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, HARDWAREASSERTSTATUSHIGH_FUNC, HARDWAREASSERTSTATUSHIGH_REG))

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
#define HARDWAREASSERTMASKLOW_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, HARDWAREASSERTMASKLOW_FUNC, HARDWAREASSERTMASKLOW_REG))

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
#define HARDWAREASSERTMASKHIGH_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, HARDWAREASSERTMASKHIGH_FUNC, HARDWAREASSERTMASKHIGH_REG))

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

/* RSPQ Watchdog Timer Log Register Low Register */
#define RSPQWDTIOTRANSLOGLOW_FUNC 0x3
#define RSPQWDTIOTRANSLOGLOW_REG  0x900
#define RSPQWDTIOTRANSLOGLOW_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, RSPQWDTIOTRANSLOGLOW_FUNC, RSPQWDTIOTRANSLOGLOW_REG))

/// RSPQ Watchdog Timer Log Register Low Register
typedef union {
  struct {
    uint32_t Valid:1;                         ///< RSP0 Entry 0 was a IO transaction when WDT for RSP Entry0 expired. Any Write to this field will reset DF::RSPQWDTIoTransLogLow and DF::RSPQWDTIoTransLogHi registers.
    uint32_t Certainty:1;                     ///< Address of transaction being timed out is certain.
    uint32_t IoAddrLow:30;                    ///< IO Transaction Address bit [31:2]
  } Field;
  uint32_t  Value;
} RSPQWDTIO_TRANS_LOG_LOW_REGISTER;

/* RSPQ Watchdog Timer Status Hi Register */
#define RSPQWDTIOTRANSLOGHI_FUNC 0x3
#define RSPQWDTIOTRANSLOGHI_REG  0x904
#define RSPQWDTIOTRANSLOGHI_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, RSPQWDTIOTRANSLOGHI_FUNC, RSPQWDTIOTRANSLOGHI_REG))

/// RSPQ Watchdog Timer Status Hi Register
typedef union {
  struct {
    uint32_t IoAddrHi:24;                     ///< IO Transaction Address bits [55:32]
    uint32_t :8;                              ///< Reserved
  } Field;
  uint32_t  Value;
} RSPQWDTIO_TRANS_LOG_HI_REGISTER;

/* Fabric Configuration Access Control (FCAC) Register */
#define FABRICCONFIGACCESSCONTROL_FUNC 0x4
#define FABRICCONFIGACCESSCONTROL_REG  0x040
#define FABRICCONFIGACCESSCONTROL_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FABRICCONFIGACCESSCONTROL_FUNC, FABRICCONFIGACCESSCONTROL_REG))

/// Fabric Configuration Access Control (FCAC) Register
typedef union {
  struct {
    uint32_t Reserved_0:1;                    ///< Reserved field
    uint32_t CfgRegInstAccRegLock:1;          ///< PSP must set this bit to 1 to lock this register after setting up address maps, routing tables, flow control, and all the fabric per-instance registers. When this bit is set to 1, Data Fabric configuration access operates in broadcast mode. This register can be updated only after a system wide reset; reads return OR-reduction of all instances of the addressed register. For information on per-instance access after this register is locked, see DF::FabricIndirectConfigAccessAddress.
    uint32_t :14;                             ///< Reserved
    uint32_t Reserved_16:8;                   ///< Reserved field
    uint32_t :8;                              ///< Reserved
  } Field;
  uint32_t  Value;
} FABRIC_CONFIG_ACCESS_CONTROL_REGISTER;

/* Fabric Indirect Configuration Access Address (FICAA) Register */
#define FABRICINDIRECTCONFIGACCESSADDRESS_0_FUNC 0x4
#define FABRICINDIRECTCONFIGACCESSADDRESS_0_REG  0x080
#define FABRICINDIRECTCONFIGACCESSADDRESS_0_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FABRICINDIRECTCONFIGACCESSADDRESS_0_FUNC, FABRICINDIRECTCONFIGACCESSADDRESS_0_REG))

#define FABRICINDIRECTCONFIGACCESSADDRESS_1_FUNC 0x4
#define FABRICINDIRECTCONFIGACCESSADDRESS_1_REG  0x084
#define FABRICINDIRECTCONFIGACCESSADDRESS_1_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FABRICINDIRECTCONFIGACCESSADDRESS_1_FUNC, FABRICINDIRECTCONFIGACCESSADDRESS_1_REG))

#define FABRICINDIRECTCONFIGACCESSADDRESS_2_FUNC 0x4
#define FABRICINDIRECTCONFIGACCESSADDRESS_2_REG  0x088
#define FABRICINDIRECTCONFIGACCESSADDRESS_2_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FABRICINDIRECTCONFIGACCESSADDRESS_2_FUNC, FABRICINDIRECTCONFIGACCESSADDRESS_2_REG))

#define FABRICINDIRECTCONFIGACCESSADDRESS_3_FUNC 0x4
#define FABRICINDIRECTCONFIGACCESSADDRESS_3_REG  0x08C
#define FABRICINDIRECTCONFIGACCESSADDRESS_3_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FABRICINDIRECTCONFIGACCESSADDRESS_3_FUNC, FABRICINDIRECTCONFIGACCESSADDRESS_3_REG))

#define FABRICINDIRECTCONFIGACCESSADDRESS_4_FUNC 0x4
#define FABRICINDIRECTCONFIGACCESSADDRESS_4_REG  0x090
#define FABRICINDIRECTCONFIGACCESSADDRESS_4_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FABRICINDIRECTCONFIGACCESSADDRESS_4_FUNC, FABRICINDIRECTCONFIGACCESSADDRESS_4_REG))

#define FABRICINDIRECTCONFIGACCESSADDRESS_5_FUNC 0x4
#define FABRICINDIRECTCONFIGACCESSADDRESS_5_REG  0x094
#define FABRICINDIRECTCONFIGACCESSADDRESS_5_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FABRICINDIRECTCONFIGACCESSADDRESS_5_FUNC, FABRICINDIRECTCONFIGACCESSADDRESS_5_REG))

/// Fabric Indirect Configuration Access Address (FICAA) Register
typedef union {
  struct {
    uint32_t CfgRegInstAccEn:1;               ///< Configuration Register Instance Access Enable.
    uint32_t IndCfgAccRegNum:10;              ///< Register number of the local DF Configuration register to be accessed through DF::FabricIndirectConfigAccessDataLo and DF::FabricIndirectConfigAccessDataHi.The register offset address is {IndCfgAccRegNum[11:2], 00b}.
    uint32_t IndCfgAccFuncNum:3;              ///< Function number of the local DF configuration register to be accessed through DF::FabricIndirectConfigAccessDataLo and DF::FabricIndirectConfigAccessDataHi.
    uint32_t SixtyFourBitRegEn:1;             ///< Specifies 64-bit register access using both the upper and lower 32 bit data registers. When set to 1, this field causes PIE to hold arbitration until two transactions have been made through the associated DF::FabricIndirectConfigAccessDataLo, DF::FabricIndirectConfigAccessDataHi register pair. This bit ensures integrity of operations to 64-bit registers (implemented in Data Fabric as two contiguous-offset 32-bit registers) by preventing FCI arbitration in the PIE, as that arbitration could result in an unrelated access getting inserted between the two 32-bit accesses that must be back-to-back to the 64-bit register.
    uint32_t :1;                              ///< Reserved
    uint32_t CfgRegInstID:8;                  ///< Identifies the instanceID of the register to access.
    uint32_t :8;                              ///< Reserved
  } Field;
  uint32_t  Value;
} FABRIC_INDIRECT_CONFIG_ACCESS_ADDRESS_REGISTER;

/* Fabric Indirect Configuration Access Data Lo (FICAD) Register */
#define FABRICINDIRECTCONFIGACCESSDATALO_0_FUNC 0x4
#define FABRICINDIRECTCONFIGACCESSDATALO_0_REG  0x0A0
#define FABRICINDIRECTCONFIGACCESSDATALO_0_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FABRICINDIRECTCONFIGACCESSDATALO_0_FUNC, FABRICINDIRECTCONFIGACCESSDATALO_0_REG))

#define FABRICINDIRECTCONFIGACCESSDATALO_1_FUNC 0x4
#define FABRICINDIRECTCONFIGACCESSDATALO_1_REG  0x0A8
#define FABRICINDIRECTCONFIGACCESSDATALO_1_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FABRICINDIRECTCONFIGACCESSDATALO_1_FUNC, FABRICINDIRECTCONFIGACCESSDATALO_1_REG))

#define FABRICINDIRECTCONFIGACCESSDATALO_2_FUNC 0x4
#define FABRICINDIRECTCONFIGACCESSDATALO_2_REG  0x0B0
#define FABRICINDIRECTCONFIGACCESSDATALO_2_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FABRICINDIRECTCONFIGACCESSDATALO_2_FUNC, FABRICINDIRECTCONFIGACCESSDATALO_2_REG))

#define FABRICINDIRECTCONFIGACCESSDATALO_3_FUNC 0x4
#define FABRICINDIRECTCONFIGACCESSDATALO_3_REG  0x0B8
#define FABRICINDIRECTCONFIGACCESSDATALO_3_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FABRICINDIRECTCONFIGACCESSDATALO_3_FUNC, FABRICINDIRECTCONFIGACCESSDATALO_3_REG))

#define FABRICINDIRECTCONFIGACCESSDATALO_4_FUNC 0x4
#define FABRICINDIRECTCONFIGACCESSDATALO_4_REG  0x0C0
#define FABRICINDIRECTCONFIGACCESSDATALO_4_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FABRICINDIRECTCONFIGACCESSDATALO_4_FUNC, FABRICINDIRECTCONFIGACCESSDATALO_4_REG))

#define FABRICINDIRECTCONFIGACCESSDATALO_5_FUNC 0x4
#define FABRICINDIRECTCONFIGACCESSDATALO_5_REG  0x0C8
#define FABRICINDIRECTCONFIGACCESSDATALO_5_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FABRICINDIRECTCONFIGACCESSDATALO_5_FUNC, FABRICINDIRECTCONFIGACCESSDATALO_5_REG))

/// Fabric Indirect Configuration Access Data Lo (FICAD) Register
typedef union {
  struct {
    uint32_t Data:32;                         ///< Indirect Data Lo
  } Field;
  uint32_t  Value;
} FABRIC_INDIRECT_CONFIG_ACCESS_DATA_LO_REGISTER;

/* Fabric Indirect Configuration Access Data Hi (FICAD) Register */
#define FABRICINDIRECTCONFIGACCESSDATAHI_0_FUNC 0x4
#define FABRICINDIRECTCONFIGACCESSDATAHI_0_REG  0x0A4
#define FABRICINDIRECTCONFIGACCESSDATAHI_0_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FABRICINDIRECTCONFIGACCESSDATAHI_0_FUNC, FABRICINDIRECTCONFIGACCESSDATAHI_0_REG))

#define FABRICINDIRECTCONFIGACCESSDATAHI_1_FUNC 0x4
#define FABRICINDIRECTCONFIGACCESSDATAHI_1_REG  0x0AC
#define FABRICINDIRECTCONFIGACCESSDATAHI_1_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FABRICINDIRECTCONFIGACCESSDATAHI_1_FUNC, FABRICINDIRECTCONFIGACCESSDATAHI_1_REG))

#define FABRICINDIRECTCONFIGACCESSDATAHI_2_FUNC 0x4
#define FABRICINDIRECTCONFIGACCESSDATAHI_2_REG  0x0B4
#define FABRICINDIRECTCONFIGACCESSDATAHI_2_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FABRICINDIRECTCONFIGACCESSDATAHI_2_FUNC, FABRICINDIRECTCONFIGACCESSDATAHI_2_REG))

#define FABRICINDIRECTCONFIGACCESSDATAHI_3_FUNC 0x4
#define FABRICINDIRECTCONFIGACCESSDATAHI_3_REG  0x0BC
#define FABRICINDIRECTCONFIGACCESSDATAHI_3_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FABRICINDIRECTCONFIGACCESSDATAHI_3_FUNC, FABRICINDIRECTCONFIGACCESSDATAHI_3_REG))

#define FABRICINDIRECTCONFIGACCESSDATAHI_4_FUNC 0x4
#define FABRICINDIRECTCONFIGACCESSDATAHI_4_REG  0x0C4
#define FABRICINDIRECTCONFIGACCESSDATAHI_4_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FABRICINDIRECTCONFIGACCESSDATAHI_4_FUNC, FABRICINDIRECTCONFIGACCESSDATAHI_4_REG))

#define FABRICINDIRECTCONFIGACCESSDATAHI_5_FUNC 0x4
#define FABRICINDIRECTCONFIGACCESSDATAHI_5_REG  0x0CC
#define FABRICINDIRECTCONFIGACCESSDATAHI_5_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FABRICINDIRECTCONFIGACCESSDATAHI_5_FUNC, FABRICINDIRECTCONFIGACCESSDATAHI_5_REG))

/// Fabric Indirect Configuration Access Data Hi (FICAD) Register
typedef union {
  struct {
    uint32_t Data:32;                         ///< Indirect Data Hi
  } Field;
  uint32_t  Value;
} FABRIC_INDIRECT_CONFIG_ACCESS_DATA_HI_REGISTER;

/* Master Request Control Register */
#define MASTERREQUESTCONTROL_FUNC 0x4
#define MASTERREQUESTCONTROL_REG  0x110
#define MASTERREQUESTCONTROL_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MASTERREQUESTCONTROL_FUNC, MASTERREQUESTCONTROL_REG))

/// Master Request Control Register
typedef union {
  struct {
    uint32_t :12;                             ///< Reserved
    uint32_t MstIoNpActReqLmt:6;              ///< Master to IO Non-Posted Active Request Limit.
    uint32_t :8;                              ///< Reserved
    uint32_t MstIoReqThrottle:5;              ///< IO request throttle rate of the Master components.
    uint32_t :1;                              ///< Reserved
  } Field;
  uint32_t  Value;
} MASTER_REQUEST_CONTROL_REGISTER;

/* Master Active Memory Request Limits Register */
#define MASTERACTIVEMEMREQLIMITS_FUNC 0x4
#define MASTERACTIVEMEMREQLIMITS_REG  0x12C
#define MASTERACTIVEMEMREQLIMITS_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, MASTERACTIVEMEMREQLIMITS_FUNC, MASTERACTIVEMEMREQLIMITS_REG))

/// Master Active Memory Request Limits Register
typedef union {
  struct {
    uint32_t MstActReqLmtLight:8;             ///< Number of outstanding memory requests before the Master enters the Light regime
    uint32_t :2;                              ///< Reserved
    uint32_t MstActReqLmtHeavy:8;             ///< Number of outstanding memory requests before the Master enters the Heavy regime
    uint32_t :2;                              ///< Reserved
    uint32_t MstActReqLmtStop:8;              ///< Number of outstanding memory requests before the Master enters the Stop regime
    uint32_t :4;                              ///< Reserved
  } Field;
  uint32_t  Value;
} MASTER_ACTIVE_MEM_REQ_LIMITS_REGISTER;

/* System Configuration Register */
#define SYSTEMCFG_FUNC 0x4
#define SYSTEMCFG_REG  0x180
#define SYSTEMCFG_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, SYSTEMCFG_FUNC, SYSTEMCFG_REG))

/// System Configuration Register
typedef union {
  struct {
    uint32_t MyDieType:2;                     ///< Die Type [1:0] of the die this instance of the register is on.
    uint32_t OtherDieType:2;                  ///< Die Type [1:0] of other die(s) in this system, which is/are not the same type as this die.
    uint32_t MyNodeMap:1;                     ///< Node Map Type for this node. 0=Local Map Type, 1=Remote Map Type.
    uint32_t :3;                              ///< Reserved
    uint32_t OtherSocketPresent:1;            ///< The other socket is present and enabled in the system. This field is used by the probe/broadcast MultiCast Mask.
    uint32_t :7;                              ///< Reserved
    uint32_t MyNodeId:12;                     ///< Node Id of the die this instance of the register is on.
    uint32_t :4;                              ///< Reserved
  } Field;
  uint32_t  Value;
} SYSTEM_CFG_REGISTER;

/* PIE Fabric Component Count Register */
#define PIEFABRICCOMPCNT_FUNC 0x4
#define PIEFABRICCOMPCNT_REG  0x18C
#define PIEFABRICCOMPCNT_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, PIEFABRICCOMPCNT_FUNC, PIEFABRICCOMPCNT_REG))

/// PIE Fabric Component Count Register
typedef union {
  struct {
    uint32_t SysBusLockNCMGrantCnt:8;         ///< Number of Non-Coherent Masters which send Grant in Bus Lock.
    uint32_t ICNGCount:8;                     ///< System count of present and enabled ICNGs
    uint32_t ACMCnt:8;                        ///< System count of present and enabled ACMs
    uint32_t :8;                              ///< Reserved
  } Field;
  uint32_t  Value;
} PIEFABRIC_COMP_CNT_REGISTER;

/* System Fabric ID Mask 0 Register */
#define SYSTEMFABRICIDMASK0_FUNC 0x4
#define SYSTEMFABRICIDMASK0_REG  0x1B0
#define SYSTEMFABRICIDMASK0_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, SYSTEMFABRICIDMASK0_FUNC, SYSTEMFABRICIDMASK0_REG))

/// System Fabric ID Mask 0 Register
typedef union {
  struct {
    uint32_t CompIdMask:16;                   ///< ComponentId Mask for the system. If a bit in this mask is set, then the corresponding bit in the FabricId is used to identify a component. ComponentId bits must be contiguous.
    uint32_t NodeIdMask:16;                   ///< NodeId Mask for the system. If a bit in this mask is set, then the corresponding bit in the FabricId is used to identify a node. NodeId bits must be contiguous.
  } Field;
  uint32_t  Value;
} SYSTEM_FABRIC_ID_MASK0_REGISTER;

/* System Fabric ID Mask 1 Register */
#define SYSTEMFABRICIDMASK1_FUNC 0x4
#define SYSTEMFABRICIDMASK1_REG  0x1B4
#define SYSTEMFABRICIDMASK1_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, SYSTEMFABRICIDMASK1_FUNC, SYSTEMFABRICIDMASK1_REG))

/// System Fabric ID Mask 1 Register
typedef union {
  struct {
    uint32_t NodeIdShft:4;                    ///< NodeId shift amount for the system. Tells how many bits to shift the NodeId by to align it to bit 0. In effect, this field identifies the LSB of the NodeId.
    uint32_t :4;                              ///< Reserved
    uint32_t SocketIdShft:4;                  ///< SocketId shift amount for the system. Tells how many bits to shift the SocketId by to align it to bit 0. In effect, this field identifies the LSB of the SocketId.
    uint32_t :20;                             ///< Reserved
  } Field;
  uint32_t  Value;
} SYSTEM_FABRIC_ID_MASK1_REGISTER;

/* System Fabric ID Mask 2 Register */
#define SYSTEMFABRICIDMASK2_FUNC 0x4
#define SYSTEMFABRICIDMASK2_REG  0x1B8
#define SYSTEMFABRICIDMASK2_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, SYSTEMFABRICIDMASK2_FUNC, SYSTEMFABRICIDMASK2_REG))

/// System Fabric ID Mask 2 Register
typedef union {
  struct {
    uint32_t DieIdMask:16;                    ///< DieId Mask for the system. If a bit in this mask is set, then the corresponding bit in the NodeId is used to identify a die. DieId bits must be contiguous.
    uint32_t SocketIdMask:16;                 ///< SocketId Mask for the system. If a bit in this mask is set, then the corresponding bit in the NodeId is used to identify a socket. SocketId bits must be contiguous.
  } Field;
  uint32_t  Value;
} SYSTEM_FABRIC_ID_MASK2_REGISTER;

/* Local Node Type Map Register */
#define LCLNODETYPEMAP_FUNC 0x4
#define LCLNODETYPEMAP_REG  0x1E0
#define LCLNODETYPEMAP_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, LCLNODETYPEMAP_FUNC, LCLNODETYPEMAP_REG))

/// Local Node Type Map Register
typedef union {
  struct {
    uint32_t BaseNodeId:12;                   ///< Lowest-numbered node ID of Node Type Local.
    uint32_t :4;                              ///< Reserved
    uint32_t NodeCount:12;                    ///< Count of Node Type Local nodes in the system.
    uint32_t :4;                              ///< Reserved
  } Field;
  uint32_t  Value;
} LCL_NODE_TYPE_MAP_REGISTER;

/* Remote Node Type Map Register */
#define RMTNODETYPEMAP_FUNC 0x4
#define RMTNODETYPEMAP_REG  0x1E4
#define RMTNODETYPEMAP_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, RMTNODETYPEMAP_FUNC, RMTNODETYPEMAP_REG))

/// Remote Node Type Map Register
typedef union {
  struct {
    uint32_t BaseNodeId:12;                   ///< Lowest-numbered node ID of Node Type Remote.
    uint32_t :4;                              ///< Reserved
    uint32_t NodeCount:12;                    ///< Count of Node Type Remote nodes in the system.
    uint32_t :4;                              ///< Reserved
  } Field;
  uint32_t  Value;
} RMT_NODE_TYPE_MAP_REGISTER;

/* CAKE Encryption Control Register */
#define CAKEENCRCNTL_FUNC 0x6
#define CAKEENCRCNTL_REG  0x0D8
#define CAKEENCRCNTL_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CAKEENCRCNTL_FUNC, CAKEENCRCNTL_REG))

/// CAKE Encryption Control Register
typedef union {
  struct {
    uint32_t EncrEn:1;                        ///< Encryption Enable control. Writing a 1 to this bit, even when already set, causes CAKE to restart encryption (reset counter; clear pipeline and prime with new nonce and key) and send an encryption_start command to the companion CAKE.
    uint32_t Reserved_1:1;                    ///< Reserved field
    uint32_t Reserved_2:1;                    ///< Reserved field
    uint32_t :1;                              ///< Reserved
    uint32_t Reserved_4:1;                    ///< Reserved field
    uint32_t :27;                             ///< Reserved
  } Field;
  uint32_t  Value;
} CAKE_ENCR_CNTL_REGISTER;

/* DRAM Hole Control Register */
#define DRAMHOLECONTROL_FUNC 0x7
#define DRAMHOLECONTROL_REG  0x104
#define DRAMHOLECONTROL_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMHOLECONTROL_FUNC, DRAMHOLECONTROL_REG))

/// DRAM Hole Control Register
typedef union {
  struct {
    uint32_t DramHoleValid:1;                 ///< Dram Hole Valid.
    uint32_t :23;                             ///< Reserved
    uint32_t DramHoleBase:8;                  ///< Dram Hole Base[31:24]. Specifies the base address of the IO hole below the 4GB address level for legacy 32-bit devices. MMIO hole cannot spam multiple DRAM ranges; it must lie within a single DRAM address range. DRAM range which spans the IO hole needs to be hoisted (added) to compensate for the hole.
  } Field;
  uint32_t  Value;
} DRAM_HOLE_CONTROL_REGISTER;

/* DRAM Offset Register */
#define DRAMOFFSET_1_FUNC 0x7
#define DRAMOFFSET_1_REG  0x140
#define DRAMOFFSET_1_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMOFFSET_1_FUNC, DRAMOFFSET_1_REG))

#define DRAMOFFSET_2_FUNC 0x7
#define DRAMOFFSET_2_REG  0x144
#define DRAMOFFSET_2_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMOFFSET_2_FUNC, DRAMOFFSET_2_REG))

#define DRAMOFFSET_3_FUNC 0x7
#define DRAMOFFSET_3_REG  0x148
#define DRAMOFFSET_3_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMOFFSET_3_FUNC, DRAMOFFSET_3_REG))

/// DRAM Offset Register
typedef union {
  struct {
    uint32_t HiAddrOffsetEn:1;                ///< Control addition of HiAddrOffset when forming normalized address. This field must be set to one when HiAddrOffset is non-zero.
    uint32_t HiAddrOffset:24;                 ///< Offset address[51:28] for CS DRAM Address range 1. Specifies the normalized (DRAM) address at the base of the associated range.
    uint32_t :7;                              ///< Reserved
  } Field;
  uint32_t  Value;
} DRAM_OFFSET_REGISTER;

/* CS Target Remap 0 Register A Register */
#define CSTARGETREMAP0A_FUNC 0x7
#define CSTARGETREMAP0A_REG  0x180
#define CSTARGETREMAP0A_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CSTARGETREMAP0A_FUNC, CSTARGETREMAP0A_REG))

/// CS Target Remap 0 Register A Register
typedef union {
  struct {
    uint32_t RemapCS0:4;                      ///< Physical CS mapped to logical CS0
    uint32_t RemapCS1:4;                      ///< Physical CS mapped to logical CS1
    uint32_t RemapCS2:4;                      ///< Physical CS mapped to logical CS2
    uint32_t RemapCS3:4;                      ///< Physical CS mapped to logical CS3
    uint32_t RemapCS4:4;                      ///< Physical CS mapped to logical CS4
    uint32_t RemapCS5:4;                      ///< Physical CS mapped to logical CS5
    uint32_t RemapCS6:4;                      ///< Physical CS mapped to logical CS6
    uint32_t RemapCS7:4;                      ///< Physical CS mapped to logical CS7
  } Field;
  uint32_t  Value;
} CS_TARGET_REMAP0_A_REGISTER;

/* CS Target Remap 0 Register B Register */
#define CSTARGETREMAP0B_FUNC 0x7
#define CSTARGETREMAP0B_REG  0x184
#define CSTARGETREMAP0B_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CSTARGETREMAP0B_FUNC, CSTARGETREMAP0B_REG))

/// CS Target Remap 0 Register B Register
typedef union {
  struct {
    uint32_t RemapCS8:4;                      ///< Physical CS mapped to logical CS8
    uint32_t RemapCS9:4;                      ///< Physical CS mapped to logical CS9
    uint32_t RemapCS10:4;                     ///< Physical CS mapped to logical CS10
    uint32_t RemapCS11:4;                     ///< Physical CS mapped to logical CS11
    uint32_t RemapCS12:4;                     ///< Physical CS mapped to logical CS12
    uint32_t RemapCS13:4;                     ///< Physical CS mapped to logical CS13
    uint32_t RemapCS14:4;                     ///< Physical CS mapped to logical CS14
    uint32_t RemapCS15:4;                     ///< Physical CS mapped to logical CS15
  } Field;
  uint32_t  Value;
} CS_TARGET_REMAP0_B_REGISTER;

/* CS Target Remap 1 Register A Register */
#define CSTARGETREMAP1A_FUNC 0x7
#define CSTARGETREMAP1A_REG  0x188
#define CSTARGETREMAP1A_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CSTARGETREMAP1A_FUNC, CSTARGETREMAP1A_REG))

/// CS Target Remap 1 Register A Register
typedef union {
  struct {
    uint32_t RemapCS0:4;                      ///< Physical CS mapped to logical CS0
    uint32_t RemapCS1:4;                      ///< Physical CS mapped to logical CS1
    uint32_t RemapCS2:4;                      ///< Physical CS mapped to logical CS2
    uint32_t RemapCS3:4;                      ///< Physical CS mapped to logical CS3
    uint32_t RemapCS4:4;                      ///< Physical CS mapped to logical CS4
    uint32_t RemapCS5:4;                      ///< Physical CS mapped to logical CS5
    uint32_t RemapCS6:4;                      ///< Physical CS mapped to logical CS6
    uint32_t RemapCS7:4;                      ///< Physical CS mapped to logical CS7
  } Field;
  uint32_t  Value;
} CS_TARGET_REMAP1_A_REGISTER;

/* CS Target Remap 1 Register B Register */
#define CSTARGETREMAP1B_FUNC 0x7
#define CSTARGETREMAP1B_REG  0x18C
#define CSTARGETREMAP1B_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CSTARGETREMAP1B_FUNC, CSTARGETREMAP1B_REG))

/// CS Target Remap 1 Register B Register
typedef union {
  struct {
    uint32_t RemapCS8:4;                      ///< Physical CS mapped to logical CS8
    uint32_t RemapCS9:4;                      ///< Physical CS mapped to logical CS9
    uint32_t RemapCS10:4;                     ///< Physical CS mapped to logical CS10
    uint32_t RemapCS11:4;                     ///< Physical CS mapped to logical CS11
    uint32_t RemapCS12:4;                     ///< Physical CS mapped to logical CS12
    uint32_t RemapCS13:4;                     ///< Physical CS mapped to logical CS13
    uint32_t RemapCS14:4;                     ///< Physical CS mapped to logical CS14
    uint32_t RemapCS15:4;                     ///< Physical CS mapped to logical CS15
  } Field;
  uint32_t  Value;
} CS_TARGET_REMAP1_B_REGISTER;

/* CS Target Remap 2 Register A Register */
#define CSTARGETREMAP2A_FUNC 0x7
#define CSTARGETREMAP2A_REG  0x190
#define CSTARGETREMAP2A_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CSTARGETREMAP2A_FUNC, CSTARGETREMAP2A_REG))

/// CS Target Remap 2 Register A Register
typedef union {
  struct {
    uint32_t RemapCS0:4;                      ///< Physical CS mapped to logical CS0
    uint32_t RemapCS1:4;                      ///< Physical CS mapped to logical CS1
    uint32_t RemapCS2:4;                      ///< Physical CS mapped to logical CS2
    uint32_t RemapCS3:4;                      ///< Physical CS mapped to logical CS3
    uint32_t RemapCS4:4;                      ///< Physical CS mapped to logical CS4
    uint32_t RemapCS5:4;                      ///< Physical CS mapped to logical CS5
    uint32_t RemapCS6:4;                      ///< Physical CS mapped to logical CS6
    uint32_t RemapCS7:4;                      ///< Physical CS mapped to logical CS7
  } Field;
  uint32_t  Value;
} CS_TARGET_REMAP2_A_REGISTER;

/* CS Target Remap 2 Register B Register */
#define CSTARGETREMAP2B_FUNC 0x7
#define CSTARGETREMAP2B_REG  0x194
#define CSTARGETREMAP2B_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CSTARGETREMAP2B_FUNC, CSTARGETREMAP2B_REG))

/// CS Target Remap 2 Register B Register
typedef union {
  struct {
    uint32_t RemapCS8:4;                      ///< Physical CS mapped to logical CS8
    uint32_t RemapCS9:4;                      ///< Physical CS mapped to logical CS9
    uint32_t RemapCS10:4;                     ///< Physical CS mapped to logical CS10
    uint32_t RemapCS11:4;                     ///< Physical CS mapped to logical CS11
    uint32_t RemapCS12:4;                     ///< Physical CS mapped to logical CS12
    uint32_t RemapCS13:4;                     ///< Physical CS mapped to logical CS13
    uint32_t RemapCS14:4;                     ///< Physical CS mapped to logical CS14
    uint32_t RemapCS15:4;                     ///< Physical CS mapped to logical CS15
  } Field;
  uint32_t  Value;
} CS_TARGET_REMAP2_B_REGISTER;

/* CS Target Remap 3 Register A Register */
#define CSTARGETREMAP3A_FUNC 0x7
#define CSTARGETREMAP3A_REG  0x198
#define CSTARGETREMAP3A_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CSTARGETREMAP3A_FUNC, CSTARGETREMAP3A_REG))

/// CS Target Remap 3 Register A Register
typedef union {
  struct {
    uint32_t RemapCS0:4;                      ///< Physical CS mapped to logical CS0
    uint32_t RemapCS1:4;                      ///< Physical CS mapped to logical CS1
    uint32_t RemapCS2:4;                      ///< Physical CS mapped to logical CS2
    uint32_t RemapCS3:4;                      ///< Physical CS mapped to logical CS3
    uint32_t RemapCS4:4;                      ///< Physical CS mapped to logical CS4
    uint32_t RemapCS5:4;                      ///< Physical CS mapped to logical CS5
    uint32_t RemapCS6:4;                      ///< Physical CS mapped to logical CS6
    uint32_t RemapCS7:4;                      ///< Physical CS mapped to logical CS7
  } Field;
  uint32_t  Value;
} CS_TARGET_REMAP3_A_REGISTER;

/* CS Target Remap 3 Register B Register */
#define CSTARGETREMAP3B_FUNC 0x7
#define CSTARGETREMAP3B_REG  0x19C
#define CSTARGETREMAP3B_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CSTARGETREMAP3B_FUNC, CSTARGETREMAP3B_REG))

/// CS Target Remap 3 Register B Register
typedef union {
  struct {
    uint32_t RemapCS8:4;                      ///< Physical CS mapped to logical CS8
    uint32_t RemapCS9:4;                      ///< Physical CS mapped to logical CS9
    uint32_t RemapCS10:4;                     ///< Physical CS mapped to logical CS10
    uint32_t RemapCS11:4;                     ///< Physical CS mapped to logical CS11
    uint32_t RemapCS12:4;                     ///< Physical CS mapped to logical CS12
    uint32_t RemapCS13:4;                     ///< Physical CS mapped to logical CS13
    uint32_t RemapCS14:4;                     ///< Physical CS mapped to logical CS14
    uint32_t RemapCS15:4;                     ///< Physical CS mapped to logical CS15
  } Field;
  uint32_t  Value;
} CS_TARGET_REMAP3_B_REGISTER;

/* CXL Memory Base Address Register */
#define CXLMEMORYBASEADDRESS_0_FUNC 0x7
#define CXLMEMORYBASEADDRESS_0_REG  0xD80
#define CXLMEMORYBASEADDRESS_0_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CXLMEMORYBASEADDRESS_0_FUNC, CXLMEMORYBASEADDRESS_0_REG))

#define CXLMEMORYBASEADDRESS_1_FUNC 0x7
#define CXLMEMORYBASEADDRESS_1_REG  0xD90
#define CXLMEMORYBASEADDRESS_1_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CXLMEMORYBASEADDRESS_1_FUNC, CXLMEMORYBASEADDRESS_1_REG))

#define CXLMEMORYBASEADDRESS_2_FUNC 0x7
#define CXLMEMORYBASEADDRESS_2_REG  0xDA0
#define CXLMEMORYBASEADDRESS_2_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CXLMEMORYBASEADDRESS_2_FUNC, CXLMEMORYBASEADDRESS_2_REG))

#define CXLMEMORYBASEADDRESS_3_FUNC 0x7
#define CXLMEMORYBASEADDRESS_3_REG  0xDB0
#define CXLMEMORYBASEADDRESS_3_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CXLMEMORYBASEADDRESS_3_FUNC, CXLMEMORYBASEADDRESS_3_REG))

/// CXL Memory Base Address Register
typedef union {
  struct {
    uint32_t AddrRngVal:1;                    ///< Address range valid (enabled/disabled)
    uint32_t NPAEn:1;                         ///< remove bits used for interleaving before sent out
    uint32_t CxlMemBaseAddr:24;               ///< CXL Memory Base address[51:28]
    uint32_t :6;                              ///< Reserved
  } Field;
  uint32_t  Value;
} CXL_MEMORY_BASE_ADDRESS_REGISTER;

/* CXL Memory Limit Address Register */
#define CXLMEMORYLIMITADDRESS_0_FUNC 0x7
#define CXLMEMORYLIMITADDRESS_0_REG  0xD84
#define CXLMEMORYLIMITADDRESS_0_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CXLMEMORYLIMITADDRESS_0_FUNC, CXLMEMORYLIMITADDRESS_0_REG))

#define CXLMEMORYLIMITADDRESS_1_FUNC 0x7
#define CXLMEMORYLIMITADDRESS_1_REG  0xD94
#define CXLMEMORYLIMITADDRESS_1_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CXLMEMORYLIMITADDRESS_1_FUNC, CXLMEMORYLIMITADDRESS_1_REG))

#define CXLMEMORYLIMITADDRESS_2_FUNC 0x7
#define CXLMEMORYLIMITADDRESS_2_REG  0xDA4
#define CXLMEMORYLIMITADDRESS_2_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CXLMEMORYLIMITADDRESS_2_FUNC, CXLMEMORYLIMITADDRESS_2_REG))

#define CXLMEMORYLIMITADDRESS_3_FUNC 0x7
#define CXLMEMORYLIMITADDRESS_3_REG  0xDB4
#define CXLMEMORYLIMITADDRESS_3_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CXLMEMORYLIMITADDRESS_3_FUNC, CXLMEMORYLIMITADDRESS_3_REG))

/// CXL Memory Limit Address Register
typedef union {
  struct {
    uint32_t CxlMemLimitAddr:24;              ///< CXL Memory limit address[51:28]
    uint32_t :8;                              ///< Reserved
  } Field;
  uint32_t  Value;
} CXL_MEMORY_LIMIT_ADDRESS_REGISTER;

/* CXL Memory Address Interleave Register */
#define CXLMEMADDRESSINTLV0_FUNC 0x7
#define CXLMEMADDRESSINTLV0_REG  0xD88
#define CXLMEMADDRESSINTLV0_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CXLMEMADDRESSINTLV0_FUNC, CXLMEMADDRESSINTLV0_REG))

/// CXL Memory Address Interleave Register
typedef union {
  struct {
    uint32_t IntLvAddrSel:1;                  ///< Encoded value for this address range which specifies the starting address bit used for interleaving. The number of address bits used for interleaving depends on the number of links across which they are interleaved. Values not listed are RESERVED
    uint32_t :3;                              ///< Reserved
    uint32_t IntLvLinkEn:4;                   ///< Enabled sub-links for this address range which specifies the CXL links across which addresses are interleaved. Non-zero if the adress range is enabled. 0xF means interleaving across all 4 sub-links
    uint32_t :24;                             ///< Reserved
  } Field;
  uint32_t  Value;
} CXL_MEM_ADDRESS_INTLV0_REGISTER;

/* CXL Memory Address Interleave Register */
#define CXLMEMADDRESSINTLV1_FUNC 0x7
#define CXLMEMADDRESSINTLV1_REG  0xD98
#define CXLMEMADDRESSINTLV1_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CXLMEMADDRESSINTLV1_FUNC, CXLMEMADDRESSINTLV1_REG))

/// CXL Memory Address Interleave Register
typedef union {
  struct {
    uint32_t IntLvAddrSel:1;                  ///< Encoded value for this address range which specifies the starting address bit used for interleaving. The number of address bits used for interleaving depends on the number of links across which they are interleaved. Values not listed are RESERVED
    uint32_t :3;                              ///< Reserved
    uint32_t IntLvLinkEn:4;                   ///< Enabled sub-links for this address range which specifies the CXL links across which addresses are interleaved. Non-zero if the adress range is enabled. 0xF means interleaving across all 4 sub-links
    uint32_t :24;                             ///< Reserved
  } Field;
  uint32_t  Value;
} CXL_MEM_ADDRESS_INTLV1_REGISTER;

/* CXL Memory Address Interleave Register */
#define CXLMEMADDRESSINTLV2_FUNC 0x7
#define CXLMEMADDRESSINTLV2_REG  0xDA8
#define CXLMEMADDRESSINTLV2_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CXLMEMADDRESSINTLV2_FUNC, CXLMEMADDRESSINTLV2_REG))

/// CXL Memory Address Interleave Register
typedef union {
  struct {
    uint32_t IntLvAddrSel:1;                  ///< Encoded value for this address range which specifies the starting address bit used for interleaving. The number of address bits used for interleaving depends on the number of links across which they are interleaved. Values not listed are RESERVED
    uint32_t :3;                              ///< Reserved
    uint32_t IntLvLinkEn:4;                   ///< Enabled sub-links for this address range which specifies the CXL links across which addresses are interleaved. Non-zero if the adress range is enabled. 0xF means interleaving across all 4 sub-links
    uint32_t :24;                             ///< Reserved
  } Field;
  uint32_t  Value;
} CXL_MEM_ADDRESS_INTLV2_REGISTER;

/* CXL Memory Address Interleave Register */
#define CXLMEMADDRESSINTLV3_FUNC 0x7
#define CXLMEMADDRESSINTLV3_REG  0xDB8
#define CXLMEMADDRESSINTLV3_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, CXLMEMADDRESSINTLV3_FUNC, CXLMEMADDRESSINTLV3_REG))

/// CXL Memory Address Interleave Register
typedef union {
  struct {
    uint32_t IntLvAddrSel:1;                  ///< Encoded value for this address range which specifies the starting address bit used for interleaving. The number of address bits used for interleaving depends on the number of links across which they are interleaved. Values not listed are RESERVED
    uint32_t :3;                              ///< Reserved
    uint32_t IntLvLinkEn:4;                   ///< Enabled sub-links for this address range which specifies the CXL links across which addresses are interleaved. Non-zero if the adress range is enabled. 0xF means interleaving across all 4 sub-links
    uint32_t :24;                             ///< Reserved
  } Field;
  uint32_t  Value;
} CXL_MEM_ADDRESS_INTLV3_REGISTER;

/* DRAM Base Address Register */
#define DRAMBASEADDRESS_0_FUNC 0x7
#define DRAMBASEADDRESS_0_REG  0xE00
#define DRAMBASEADDRESS_0_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMBASEADDRESS_0_FUNC, DRAMBASEADDRESS_0_REG))

#define DRAMBASEADDRESS_1_FUNC 0x7
#define DRAMBASEADDRESS_1_REG  0xE10
#define DRAMBASEADDRESS_1_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMBASEADDRESS_1_FUNC, DRAMBASEADDRESS_1_REG))

#define DRAMBASEADDRESS_2_FUNC 0x7
#define DRAMBASEADDRESS_2_REG  0xE20
#define DRAMBASEADDRESS_2_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMBASEADDRESS_2_FUNC, DRAMBASEADDRESS_2_REG))

#define DRAMBASEADDRESS_3_FUNC 0x7
#define DRAMBASEADDRESS_3_REG  0xE30
#define DRAMBASEADDRESS_3_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMBASEADDRESS_3_FUNC, DRAMBASEADDRESS_3_REG))

#define DRAMBASEADDRESS_4_FUNC 0x7
#define DRAMBASEADDRESS_4_REG  0xE40
#define DRAMBASEADDRESS_4_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMBASEADDRESS_4_FUNC, DRAMBASEADDRESS_4_REG))

#define DRAMBASEADDRESS_5_FUNC 0x7
#define DRAMBASEADDRESS_5_REG  0xE50
#define DRAMBASEADDRESS_5_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMBASEADDRESS_5_FUNC, DRAMBASEADDRESS_5_REG))

#define DRAMBASEADDRESS_6_FUNC 0x7
#define DRAMBASEADDRESS_6_REG  0xE60
#define DRAMBASEADDRESS_6_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMBASEADDRESS_6_FUNC, DRAMBASEADDRESS_6_REG))

#define DRAMBASEADDRESS_7_FUNC 0x7
#define DRAMBASEADDRESS_7_REG  0xE70
#define DRAMBASEADDRESS_7_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMBASEADDRESS_7_FUNC, DRAMBASEADDRESS_7_REG))

#define DRAMBASEADDRESS_8_FUNC 0x7
#define DRAMBASEADDRESS_8_REG  0xE80
#define DRAMBASEADDRESS_8_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMBASEADDRESS_8_FUNC, DRAMBASEADDRESS_8_REG))

#define DRAMBASEADDRESS_9_FUNC 0x7
#define DRAMBASEADDRESS_9_REG  0xE90
#define DRAMBASEADDRESS_9_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMBASEADDRESS_9_FUNC, DRAMBASEADDRESS_9_REG))

#define DRAMBASEADDRESS_10_FUNC 0x7
#define DRAMBASEADDRESS_10_REG  0xEA0
#define DRAMBASEADDRESS_10_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMBASEADDRESS_10_FUNC, DRAMBASEADDRESS_10_REG))

#define DRAMBASEADDRESS_11_FUNC 0x7
#define DRAMBASEADDRESS_11_REG  0xEB0
#define DRAMBASEADDRESS_11_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMBASEADDRESS_11_FUNC, DRAMBASEADDRESS_11_REG))

#define DRAMBASEADDRESS_12_FUNC 0x7
#define DRAMBASEADDRESS_12_REG  0xEC0
#define DRAMBASEADDRESS_12_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMBASEADDRESS_12_FUNC, DRAMBASEADDRESS_12_REG))

#define DRAMBASEADDRESS_13_FUNC 0x7
#define DRAMBASEADDRESS_13_REG  0xED0
#define DRAMBASEADDRESS_13_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMBASEADDRESS_13_FUNC, DRAMBASEADDRESS_13_REG))

#define DRAMBASEADDRESS_14_FUNC 0x7
#define DRAMBASEADDRESS_14_REG  0xEE0
#define DRAMBASEADDRESS_14_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMBASEADDRESS_14_FUNC, DRAMBASEADDRESS_14_REG))

#define DRAMBASEADDRESS_15_FUNC 0x7
#define DRAMBASEADDRESS_15_REG  0xEF0
#define DRAMBASEADDRESS_15_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMBASEADDRESS_15_FUNC, DRAMBASEADDRESS_15_REG))

#define DRAMBASEADDRESS_16_FUNC 0x7
#define DRAMBASEADDRESS_16_REG  0xF00
#define DRAMBASEADDRESS_16_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMBASEADDRESS_16_FUNC, DRAMBASEADDRESS_16_REG))

#define DRAMBASEADDRESS_17_FUNC 0x7
#define DRAMBASEADDRESS_17_REG  0xF10
#define DRAMBASEADDRESS_17_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMBASEADDRESS_17_FUNC, DRAMBASEADDRESS_17_REG))

#define DRAMBASEADDRESS_18_FUNC 0x7
#define DRAMBASEADDRESS_18_REG  0xF20
#define DRAMBASEADDRESS_18_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMBASEADDRESS_18_FUNC, DRAMBASEADDRESS_18_REG))

#define DRAMBASEADDRESS_19_FUNC 0x7
#define DRAMBASEADDRESS_19_REG  0xF30
#define DRAMBASEADDRESS_19_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMBASEADDRESS_19_FUNC, DRAMBASEADDRESS_19_REG))

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
#define DRAMLIMITADDRESS_0_REG  0xE04
#define DRAMLIMITADDRESS_0_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMLIMITADDRESS_0_FUNC, DRAMLIMITADDRESS_0_REG))

#define DRAMLIMITADDRESS_1_FUNC 0x7
#define DRAMLIMITADDRESS_1_REG  0xE14
#define DRAMLIMITADDRESS_1_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMLIMITADDRESS_1_FUNC, DRAMLIMITADDRESS_1_REG))

#define DRAMLIMITADDRESS_2_FUNC 0x7
#define DRAMLIMITADDRESS_2_REG  0xE24
#define DRAMLIMITADDRESS_2_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMLIMITADDRESS_2_FUNC, DRAMLIMITADDRESS_2_REG))

#define DRAMLIMITADDRESS_3_FUNC 0x7
#define DRAMLIMITADDRESS_3_REG  0xE34
#define DRAMLIMITADDRESS_3_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMLIMITADDRESS_3_FUNC, DRAMLIMITADDRESS_3_REG))

#define DRAMLIMITADDRESS_4_FUNC 0x7
#define DRAMLIMITADDRESS_4_REG  0xE44
#define DRAMLIMITADDRESS_4_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMLIMITADDRESS_4_FUNC, DRAMLIMITADDRESS_4_REG))

#define DRAMLIMITADDRESS_5_FUNC 0x7
#define DRAMLIMITADDRESS_5_REG  0xE54
#define DRAMLIMITADDRESS_5_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMLIMITADDRESS_5_FUNC, DRAMLIMITADDRESS_5_REG))

#define DRAMLIMITADDRESS_6_FUNC 0x7
#define DRAMLIMITADDRESS_6_REG  0xE64
#define DRAMLIMITADDRESS_6_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMLIMITADDRESS_6_FUNC, DRAMLIMITADDRESS_6_REG))

#define DRAMLIMITADDRESS_7_FUNC 0x7
#define DRAMLIMITADDRESS_7_REG  0xE74
#define DRAMLIMITADDRESS_7_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMLIMITADDRESS_7_FUNC, DRAMLIMITADDRESS_7_REG))

#define DRAMLIMITADDRESS_8_FUNC 0x7
#define DRAMLIMITADDRESS_8_REG  0xE84
#define DRAMLIMITADDRESS_8_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMLIMITADDRESS_8_FUNC, DRAMLIMITADDRESS_8_REG))

#define DRAMLIMITADDRESS_9_FUNC 0x7
#define DRAMLIMITADDRESS_9_REG  0xE94
#define DRAMLIMITADDRESS_9_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMLIMITADDRESS_9_FUNC, DRAMLIMITADDRESS_9_REG))

#define DRAMLIMITADDRESS_10_FUNC 0x7
#define DRAMLIMITADDRESS_10_REG  0xEA4
#define DRAMLIMITADDRESS_10_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMLIMITADDRESS_10_FUNC, DRAMLIMITADDRESS_10_REG))

#define DRAMLIMITADDRESS_11_FUNC 0x7
#define DRAMLIMITADDRESS_11_REG  0xEB4
#define DRAMLIMITADDRESS_11_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMLIMITADDRESS_11_FUNC, DRAMLIMITADDRESS_11_REG))

#define DRAMLIMITADDRESS_12_FUNC 0x7
#define DRAMLIMITADDRESS_12_REG  0xEC4
#define DRAMLIMITADDRESS_12_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMLIMITADDRESS_12_FUNC, DRAMLIMITADDRESS_12_REG))

#define DRAMLIMITADDRESS_13_FUNC 0x7
#define DRAMLIMITADDRESS_13_REG  0xED4
#define DRAMLIMITADDRESS_13_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMLIMITADDRESS_13_FUNC, DRAMLIMITADDRESS_13_REG))

#define DRAMLIMITADDRESS_14_FUNC 0x7
#define DRAMLIMITADDRESS_14_REG  0xEE4
#define DRAMLIMITADDRESS_14_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMLIMITADDRESS_14_FUNC, DRAMLIMITADDRESS_14_REG))

#define DRAMLIMITADDRESS_15_FUNC 0x7
#define DRAMLIMITADDRESS_15_REG  0xEF4
#define DRAMLIMITADDRESS_15_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMLIMITADDRESS_15_FUNC, DRAMLIMITADDRESS_15_REG))

#define DRAMLIMITADDRESS_16_FUNC 0x7
#define DRAMLIMITADDRESS_16_REG  0xF04
#define DRAMLIMITADDRESS_16_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMLIMITADDRESS_16_FUNC, DRAMLIMITADDRESS_16_REG))

#define DRAMLIMITADDRESS_17_FUNC 0x7
#define DRAMLIMITADDRESS_17_REG  0xF14
#define DRAMLIMITADDRESS_17_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMLIMITADDRESS_17_FUNC, DRAMLIMITADDRESS_17_REG))

#define DRAMLIMITADDRESS_18_FUNC 0x7
#define DRAMLIMITADDRESS_18_REG  0xF24
#define DRAMLIMITADDRESS_18_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMLIMITADDRESS_18_FUNC, DRAMLIMITADDRESS_18_REG))

#define DRAMLIMITADDRESS_19_FUNC 0x7
#define DRAMLIMITADDRESS_19_REG  0xF34
#define DRAMLIMITADDRESS_19_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMLIMITADDRESS_19_FUNC, DRAMLIMITADDRESS_19_REG))

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
#define DRAMADDRESSCTL_0_REG  0xE08
#define DRAMADDRESSCTL_0_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSCTL_0_FUNC, DRAMADDRESSCTL_0_REG))

#define DRAMADDRESSCTL_1_FUNC 0x7
#define DRAMADDRESSCTL_1_REG  0xE18
#define DRAMADDRESSCTL_1_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSCTL_1_FUNC, DRAMADDRESSCTL_1_REG))

#define DRAMADDRESSCTL_2_FUNC 0x7
#define DRAMADDRESSCTL_2_REG  0xE28
#define DRAMADDRESSCTL_2_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSCTL_2_FUNC, DRAMADDRESSCTL_2_REG))

#define DRAMADDRESSCTL_3_FUNC 0x7
#define DRAMADDRESSCTL_3_REG  0xE38
#define DRAMADDRESSCTL_3_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSCTL_3_FUNC, DRAMADDRESSCTL_3_REG))

#define DRAMADDRESSCTL_4_FUNC 0x7
#define DRAMADDRESSCTL_4_REG  0xE48
#define DRAMADDRESSCTL_4_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSCTL_4_FUNC, DRAMADDRESSCTL_4_REG))

#define DRAMADDRESSCTL_5_FUNC 0x7
#define DRAMADDRESSCTL_5_REG  0xE58
#define DRAMADDRESSCTL_5_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSCTL_5_FUNC, DRAMADDRESSCTL_5_REG))

#define DRAMADDRESSCTL_6_FUNC 0x7
#define DRAMADDRESSCTL_6_REG  0xE68
#define DRAMADDRESSCTL_6_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSCTL_6_FUNC, DRAMADDRESSCTL_6_REG))

#define DRAMADDRESSCTL_7_FUNC 0x7
#define DRAMADDRESSCTL_7_REG  0xE78
#define DRAMADDRESSCTL_7_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSCTL_7_FUNC, DRAMADDRESSCTL_7_REG))

#define DRAMADDRESSCTL_8_FUNC 0x7
#define DRAMADDRESSCTL_8_REG  0xE88
#define DRAMADDRESSCTL_8_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSCTL_8_FUNC, DRAMADDRESSCTL_8_REG))

#define DRAMADDRESSCTL_9_FUNC 0x7
#define DRAMADDRESSCTL_9_REG  0xE98
#define DRAMADDRESSCTL_9_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSCTL_9_FUNC, DRAMADDRESSCTL_9_REG))

#define DRAMADDRESSCTL_10_FUNC 0x7
#define DRAMADDRESSCTL_10_REG  0xEA8
#define DRAMADDRESSCTL_10_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSCTL_10_FUNC, DRAMADDRESSCTL_10_REG))

#define DRAMADDRESSCTL_11_FUNC 0x7
#define DRAMADDRESSCTL_11_REG  0xEB8
#define DRAMADDRESSCTL_11_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSCTL_11_FUNC, DRAMADDRESSCTL_11_REG))

#define DRAMADDRESSCTL_12_FUNC 0x7
#define DRAMADDRESSCTL_12_REG  0xEC8
#define DRAMADDRESSCTL_12_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSCTL_12_FUNC, DRAMADDRESSCTL_12_REG))

#define DRAMADDRESSCTL_13_FUNC 0x7
#define DRAMADDRESSCTL_13_REG  0xED8
#define DRAMADDRESSCTL_13_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSCTL_13_FUNC, DRAMADDRESSCTL_13_REG))

#define DRAMADDRESSCTL_14_FUNC 0x7
#define DRAMADDRESSCTL_14_REG  0xEE8
#define DRAMADDRESSCTL_14_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSCTL_14_FUNC, DRAMADDRESSCTL_14_REG))

#define DRAMADDRESSCTL_15_FUNC 0x7
#define DRAMADDRESSCTL_15_REG  0xEF8
#define DRAMADDRESSCTL_15_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSCTL_15_FUNC, DRAMADDRESSCTL_15_REG))

#define DRAMADDRESSCTL_16_FUNC 0x7
#define DRAMADDRESSCTL_16_REG  0xF08
#define DRAMADDRESSCTL_16_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSCTL_16_FUNC, DRAMADDRESSCTL_16_REG))

#define DRAMADDRESSCTL_17_FUNC 0x7
#define DRAMADDRESSCTL_17_REG  0xF18
#define DRAMADDRESSCTL_17_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSCTL_17_FUNC, DRAMADDRESSCTL_17_REG))

#define DRAMADDRESSCTL_18_FUNC 0x7
#define DRAMADDRESSCTL_18_REG  0xF28
#define DRAMADDRESSCTL_18_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSCTL_18_FUNC, DRAMADDRESSCTL_18_REG))

#define DRAMADDRESSCTL_19_FUNC 0x7
#define DRAMADDRESSCTL_19_REG  0xF38
#define DRAMADDRESSCTL_19_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSCTL_19_FUNC, DRAMADDRESSCTL_19_REG))

/// DRAM Address Control Register
typedef union {
  struct {
    uint32_t AddrRngVal:1;                    ///< Address range valid (enabled/disabled)
    uint32_t LgcyMmioHoleEn:1;                ///< This field is only permitted to be set in at most one
                                              ///< DRAM range.Host Firmware sets this bit for an address range that
                                              ///< spans the 4GB boundary and contains a hole for addresses
                                              ///< used by legacy MMIO. Additional information needs to be
                                              ///< programmed; see DF::DramHoleControl.
    uint32_t SCM:1;                           ///< If set, this field indicates that the addresses described
                                              ///< by this address map are stored in Storage Class Memory.
    uint32_t :1;                              ///< Reserved
    uint32_t RemapEn:1;                       ///< Remap Enable
    uint32_t RemapSel:3;                      ///< Remap Select
    uint32_t HashIntlvCtl64K:1;               ///< Enables inclusion of address bits on the 64KB boundary
                                              ///< (bits 16, 17, 18) in a hashed interleave computation.
    uint32_t HashIntlvCtl2M:1;                ///< Enables inclusion of address bits on the 2MB boundary
                                              ///< (bits 21, 22, 23) in a hashed interleave computation.
    uint32_t HashIntlvCtl1G:1;                ///< Enables inclusion of address bits on the 1GB boundary
                                              ///< (bits 30, 31, 32) in a hashed interleave computation.
    uint32_t :5;                              ///< Reserved
    uint32_t DstFabricID:12;                  ///< When CS interleaving is disabled
                                              ///< (see DF::DramAddressIntlv[IntLvNumChan])
                                              ///< this field provides the target CS FabricId for this
                                              ///< address map.When CS interleaving is enabled,
                                              ///< this field provides the CS Fabric ID where interleave starts.
                                              ///< See 13.6.2.1 [DRAM Address Normalization and Interleave].
    uint32_t :4;                              ///< Reserved
  } Field;
  uint32_t  Value;
} DRAM_ADDRESS_CTL_REGISTER;

/* DRAM Address Interleave Register */
#define DRAMADDRESSINTLV_0_FUNC 0x7
#define DRAMADDRESSINTLV_0_REG  0xE0C
#define DRAMADDRESSINTLV_0_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSINTLV_0_FUNC, DRAMADDRESSINTLV_0_REG))

#define DRAMADDRESSINTLV_1_FUNC 0x7
#define DRAMADDRESSINTLV_1_REG  0xE1C
#define DRAMADDRESSINTLV_1_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSINTLV_1_FUNC, DRAMADDRESSINTLV_1_REG))

#define DRAMADDRESSINTLV_2_FUNC 0x7
#define DRAMADDRESSINTLV_2_REG  0xE2C
#define DRAMADDRESSINTLV_2_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSINTLV_2_FUNC, DRAMADDRESSINTLV_2_REG))

#define DRAMADDRESSINTLV_3_FUNC 0x7
#define DRAMADDRESSINTLV_3_REG  0xE3C
#define DRAMADDRESSINTLV_3_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSINTLV_3_FUNC, DRAMADDRESSINTLV_3_REG))

#define DRAMADDRESSINTLV_4_FUNC 0x7
#define DRAMADDRESSINTLV_4_REG  0xE4C
#define DRAMADDRESSINTLV_4_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSINTLV_4_FUNC, DRAMADDRESSINTLV_4_REG))

#define DRAMADDRESSINTLV_5_FUNC 0x7
#define DRAMADDRESSINTLV_5_REG  0xE5C
#define DRAMADDRESSINTLV_5_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSINTLV_5_FUNC, DRAMADDRESSINTLV_5_REG))

#define DRAMADDRESSINTLV_6_FUNC 0x7
#define DRAMADDRESSINTLV_6_REG  0xE6C
#define DRAMADDRESSINTLV_6_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSINTLV_6_FUNC, DRAMADDRESSINTLV_6_REG))

#define DRAMADDRESSINTLV_7_FUNC 0x7
#define DRAMADDRESSINTLV_7_REG  0xE7C
#define DRAMADDRESSINTLV_7_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSINTLV_7_FUNC, DRAMADDRESSINTLV_7_REG))

#define DRAMADDRESSINTLV_8_FUNC 0x7
#define DRAMADDRESSINTLV_8_REG  0xE8C
#define DRAMADDRESSINTLV_8_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSINTLV_8_FUNC, DRAMADDRESSINTLV_8_REG))

#define DRAMADDRESSINTLV_9_FUNC 0x7
#define DRAMADDRESSINTLV_9_REG  0xE9C
#define DRAMADDRESSINTLV_9_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSINTLV_9_FUNC, DRAMADDRESSINTLV_9_REG))

#define DRAMADDRESSINTLV_10_FUNC 0x7
#define DRAMADDRESSINTLV_10_REG  0xEAC
#define DRAMADDRESSINTLV_10_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSINTLV_10_FUNC, DRAMADDRESSINTLV_10_REG))

#define DRAMADDRESSINTLV_11_FUNC 0x7
#define DRAMADDRESSINTLV_11_REG  0xEBC
#define DRAMADDRESSINTLV_11_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSINTLV_11_FUNC, DRAMADDRESSINTLV_11_REG))

#define DRAMADDRESSINTLV_12_FUNC 0x7
#define DRAMADDRESSINTLV_12_REG  0xECC
#define DRAMADDRESSINTLV_12_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSINTLV_12_FUNC, DRAMADDRESSINTLV_12_REG))

#define DRAMADDRESSINTLV_13_FUNC 0x7
#define DRAMADDRESSINTLV_13_REG  0xEDC
#define DRAMADDRESSINTLV_13_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSINTLV_13_FUNC, DRAMADDRESSINTLV_13_REG))

#define DRAMADDRESSINTLV_14_FUNC 0x7
#define DRAMADDRESSINTLV_14_REG  0xEEC
#define DRAMADDRESSINTLV_14_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSINTLV_14_FUNC, DRAMADDRESSINTLV_14_REG))

#define DRAMADDRESSINTLV_15_FUNC 0x7
#define DRAMADDRESSINTLV_15_REG  0xEFC
#define DRAMADDRESSINTLV_15_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSINTLV_15_FUNC, DRAMADDRESSINTLV_15_REG))

#define DRAMADDRESSINTLV_16_FUNC 0x7
#define DRAMADDRESSINTLV_16_REG  0xF0C
#define DRAMADDRESSINTLV_16_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSINTLV_16_FUNC, DRAMADDRESSINTLV_16_REG))

#define DRAMADDRESSINTLV_17_FUNC 0x7
#define DRAMADDRESSINTLV_17_REG  0xF1C
#define DRAMADDRESSINTLV_17_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSINTLV_17_FUNC, DRAMADDRESSINTLV_17_REG))

#define DRAMADDRESSINTLV_18_FUNC 0x7
#define DRAMADDRESSINTLV_18_REG  0xF2C
#define DRAMADDRESSINTLV_18_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSINTLV_18_FUNC, DRAMADDRESSINTLV_18_REG))

#define DRAMADDRESSINTLV_19_FUNC 0x7
#define DRAMADDRESSINTLV_19_REG  0xF3C
#define DRAMADDRESSINTLV_19_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, DRAMADDRESSINTLV_19_FUNC, DRAMADDRESSINTLV_19_REG))

/// DRAM Address Interleave Register
typedef union {
  struct {
    uint32_t IntLvAddrSel:3;                  ///< Encoded value for this address range which specifies the starting
                                              ///< address bit used for interleaving. The number of address bits
                                              ///< used for interleaving depends on the number of channels across which
                                              ///< they are interleaved. Values not listed are RESERVED
    uint32_t :1;                              ///< Reserved
    uint32_t IntLvNumChan:5;                  ///< Encoded value for this address range which specifies the number of
                                              ///< coherent stations across which addresses are interleaved.
                                              ///< Hash interleaves must program IntLvAddrSel to select address bit [8].
    uint32_t :3;                              ///< Reserved
    uint32_t IntLvNumDies:2;                  ///< Encoded value which specifies the number of dies across which
                                              ///< addresses are interleaved.
    uint32_t :4;                              ///< Reserved
    uint32_t IntLvNumSockets:1;               ///< Specifies the number of sockets across which addresses are interleaved.
    uint32_t :13;                             ///< Reserved
  } Field;
  uint32_t  Value;
} DRAM_ADDRESS_INTLV_REGISTER;

#pragma pack (pop)
