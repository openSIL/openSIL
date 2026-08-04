/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  PHX_IOHC.h
 * @brief This file contains IOHC Register definitions
 */

#pragma once

/** IOHC_PCIE_CRS_Count
 * Reg(IOHC::IOHC_PCIE_CRS_Count)
 * Classification(NDA)
 *   Disposition(TBD)
 */
#define IOHC_PCIE_CRS_Count_CrsDelayCount_OFFSET               0
#define BITS_0_TO_15_MASK_AAAAAVD                 0xffff

#define IOHC_PCIE_CRS_Count_CrsLimitCount_OFFSET               16
#define IOHC_PCIE_CRS_Count_CrsLimitCount_MASK                 0xfff0000

#define IOHCx13b10028                               0x13b10028UL

/** CFG_IOHC_PCI
 * Reg(IOHC::CFG_IOHC_PCI)
 * Classification(NDA)
 *   Disposition(TBD)
 */
#define BIT0_OFFSET_AAAAATC          0
#define BIT0_MASK_AAAAATB            0x1

#define IOHCx13b10034                                      0x13b10034UL

/** CFG_ZSC_IDLE_MASK
 * Reg(IOHC::CFG_ZSC_IDLE_MASK)
 * Classification(NDA)
 *   Disposition(TBD)
 */
#define BIT22_OFFSET_AAAAAUI     22
#define BIT22_MASK_AAAAAUH       0x400000

#define BIT23_OFFSET_AAAAAUG     23
#define BIT23_MASK_AAAAAUF       0x800000

#define BIT24_OFFSET_AAAAAUE     24
#define IOHC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK7_MASK       0x1000000

#define BIT25_OFFSET_AAAAAUC     25
#define BIT25_MASK_AAAAAUB       0x2000000

#define BIT26_OFFSET_AAAAAUA     26
#define BIT26_MASK_AAAAATZ       0x4000000

#define BIT27_OFFSET_AAAAATY     27
#define BIT27_MASK_AAAAATX       0x8000000

#define BIT28_OFFSET_AAAAATW     28
#define BIT28_MASK_AAAAATV       0x10000000

#define BIT29_OFFSET_AAAAATU     29
#define BIT29_MASK_AAAAATT       0x20000000

#define BIT30_OFFSET_AAAAATS     30
#define BIT30_MASK_AAAAATR       0x40000000

#define BIT31_OFFSET_AAAAATQ     31
#define BIT31_MASK_AAAAATP       0x80000000

#define IOHCx13b10088                          0x13b10088UL

/** IOHC_GLUE_CG_LCLK_CTRL_1
 * Reg(IOHC::IOHC_GLUE_CG_LCLK_CTRL_1)
 * Classification(NDA)
 *   Disposition(TBD)
 */
#define IOHC_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK9_OFFSET     22
#define BIT22_MASK_AAAAAVB       0x400000

#define BIT23_OFFSET_AAAAAVA     23
#define BIT23_MASK_AAAAAUZ       0x800000

#define BIT24_OFFSET_AAAAAUY     24
#define BIT24_MASK_AAAAAUX       0x1000000

#define BIT25_OFFSET_AAAAAUW     25
#define BIT25_MASK_AAAAAUV       0x2000000

#define BIT26_OFFSET_AAAAAUU     26
#define BIT26_MASK_AAAAAUT       0x4000000

#define BIT27_OFFSET_AAAAAUS     27
#define BIT27_MASK_AAAAAUR       0x8000000

#define BIT28_OFFSET_AAAAAUQ     28
#define BIT28_MASK_AAAAAUP       0x10000000

#define BIT29_OFFSET_AAAAAUO     29
#define BIT29_MASK_AAAAAUN       0x20000000

#define BIT30_OFFSET_AAAAAUM     30
#define IOHC_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK1_MASK       0x40000000

#define BIT31_OFFSET_AAAAAUK     31
#define BIT31_MASK_AAAAAUJ       0x80000000

#define IOHCx13b1008c                          0x13b1008cUL

/** IOHC_PERF_CNTL
 * Reg(IOHC::IOHC_PERF_CNTL)
 * Classification(NDA)
 *   Disposition(TBD)
 */
#define IOHC_FEATURE_CNTL_P2P_mode_OFFSET                      1
#define BITS_1_TO_2_MASK_AAAAATN                        0x6

#define IOHC_FEATURE_CNTL_IOHC_ARI_SUPPORTED_OFFSET            22
#define IOHC_FEATURE_CNTL_IOHC_ARI_SUPPORTED_MASK              0x400000

#define IOHCx13b10118                                 0x13b10118UL

/** PCIE_VDM_CNTL2
 * Reg(IOHC::PCIE_VDM_CNTL2)
 * Classification(NDA)
 *   Disposition(TBD)
 */
#define PCIE_VDM_CNTL2_VdmP2pMode_OFFSET                       0
#define PCIE_VDM_CNTL2_VdmP2pMode_MASK                         0x3

#define IOHCx13b10230                                    0x13b10230UL

/** CCP_BASE_ADDR_LO
 * Reg(IOHC::CCP_BASE_ADDR_LO)
 * Classification(NDA)
 *   Disposition(TBD)
 */
#define IOHCx13b102d8                                  0x13b102d8UL

/** CCP_BASE_ADDR_LO
 * Reg(IOHC::CCP_BASE_ADDR_HI)
 * Classification(NDA)
 *   Disposition(TBD)
 */
#define IOHCx13b102dc                                  0x13b102dcUL

/** PSP_BASE_ADDR_LO
 * Reg(IOHC::PSP_BASE_ADDR_LO)
 * Classification(NDA)
 *   Disposition(TBD)
 */
#define IOHCx13b102e0                                  0x13b102e0UL

/** PSP_BASE_ADDR_LO
 * Reg(IOHC::PSP_BASE_ADDR_HI)
 * Classification(NDA)
 *   Disposition(TBD)
 */
#define IOHCx13b102e4                                  0x13b102e4UL

/** SMU_BASE_ADDR_LO
 * Reg(IOHC::SMU_BASE_ADDR_LO)
 * Classification(NDA)
 *   Disposition(TBD)
 */
#define IOHCx13b102e8                                  0x13b102e8UL

/** SMU_BASE_ADDR_HI
 * Reg(IOHC::SMU_BASE_ADDR_HI)
 * Classification(NDA)
 *   Disposition(TBD)
 */
#define IOHCx13b102ec                                  0x13b102ecUL

/** IOAPIC_BASE_ADDR_LO
 * Reg(IOHC::IOAPIC_BASE_ADDR_LO)
 * Classification(NDA)
 *   Disposition(TBD)
 */
#define IOHCx13b102fc                                  0x13b102fcUL

/** FASTREG_BASE_ADDR_LO
 * Reg(IOHC::FASTREG_BASE_ADDR_LO)
 * Classification(NDA)
 *   Disposition(TBD)
 */
#define IOHCx13b10300                              0x13b10300UL

/** FASTREG_BASE_ADDR_HI
 * Reg(IOHC::FASTREG_BASE_ADDR_HI)
 * Classification(NDA)
 *   Disposition(TBD)
 */
#define IOHCx13b10304                              0x13b10304UL

/** FASTREGCNTL_BASE_ADDR_LO
 * Reg(IOHC::FASTREGCNTL_BASE_ADDR_LO)
 * Classification(NDA)
 *   Disposition(TBD)
 */
#define IOHCx13b10308                          0x13b10308UL

/** FASTREGCNTL_BASE_ADDR_HI
 * Reg(IOHC::FASTREGCNTL_BASE_ADDR_HI)
 * Classification(NDA)
 *   Disposition(TBD)
 */
#define IOHCx13b1030c                          0x13b1030cUL

/** MPIO_BASE_ADDR_LO
 * Reg(IOHC::MPIO_BASE_ADDR_LO)
 * Classification(NDA)
 *   Disposition(TBD)
 */
#define BIT8_OFFSET_AAAAAVI                       8
#define BIT8_MASK_AAAAAVH                         0x100

#define IOHCx13b10340                                   0x13b10340UL

/** IOHC_EARLY_WAKE_UP_EN
 * Reg(IOHC::IOHC_EARLY_WAKE_UP_EN)
 * Classification(NDA)
 *   Disposition(TBD)
 */
#define BIT0_OFFSET_AAAAATK      0
#define BITS_0_TO_1_MASK_AAAAATJ        0x3

#define IOHCx13b10348                             0x13b10348UL

/** IOHC_QOS_CONTROL
 * Reg(IOHC::IOHC_QOS_CONTROL)
 * Classification(NDA)
 *   Disposition(TBD)
 */
#define IOHC_QOS_CONTROL_VC0QoSPriority_OFFSET                 0
#define BITS_0_TO_3_MASK_AAAAAVJ                   0xf

#define BIT4_OFFSET_AAAAAVM                 4
#define IOHC_QOS_CONTROL_VC1QoSPriority_MASK                   0xf0

#define BIT8_OFFSET_AAAAAVO                 8
#define BITS_8_TO_11_MASK_AAAAAVN                   0xf00

#define BIT12_OFFSET_AAAAAVQ                 12
#define BITS_12_TO_15_MASK_AAAAAVP                   0xf000

#define BIT16_OFFSET_AAAAAVS                 16
#define BITS_16_TO_19_MASK_AAAAAVR                   0xf0000

#define BIT20_OFFSET_AAAAAVU                 20
#define IOHC_QOS_CONTROL_VC5QoSPriority_MASK                   0xf00000

#define BIT24_OFFSET_AAAAAVW                 24
#define BITS_24_TO_27_MASK_AAAAAVV                   0xf000000

#define BIT28_OFFSET_AAAAAVY                 28
#define BITS_28_TO_31_MASK_AAAAAVX                   0xf0000000

#define IOHCx13b14040                                  0x13b14040UL

/** POISON_ACTION_CONTROL
 * Reg(IOHC::POISON_ACTION_CONTROL)
 * Classification(NDA)
 *   Disposition(TBD)
 */
#define BIT0_OFFSET_AAAAAWC        0
#define POISON_ACTION_CONTROL_IntPoisonAPMLErrEn_MASK          0x1

#define BIT3_OFFSET_AAAAAWE        3
#define BIT3_MASK_AAAAAWD          0x8

#define BIT4_OFFSET_AAAAAWG      4
#define BIT4_MASK_AAAAAWF        0x10

#define IOHCx13b20814                             0x13b20814UL

/** IOHC_Bridge_CNTL
 * Reg(IOHC::IOHC_Bridge_CNTL)
 * Classification(NDA)
 *   Disposition(TBD)
 */
#define BIT0_OFFSET_AAAAATE                      0
#define BIT0_MASK_AAAAATD                        0x1

#define BIT2_OFFSET_AAAAATG                         2
#define BIT2_MASK_AAAAATF                           0x4

#define BIT18_OFFSET_AAAAATI                      18
#define BIT18_MASK_AAAAATH                        0x40000

#define IOHCx13b31004           0x13b31004UL
#define IOHCx13b33004           0x13b33004UL

#define IOHCx13b38404           0x13b38404UL

#define IOHCx13b1007c           0x13b1007cUL

/** NB_PROG_DEVICE_REMAP
 * Reg(IOHC::NB_PROG_DEVICE_REMAP)
 * Classification(NDA)
 *   Disposition(TBD)
 */
typedef union {
  struct {
    uint32_t                            DevFnMap:8;
    uint32_t                            Reserved_31_8:24;
  } Field;
  uint32_t Value;
} NB_PROG_DEVICE_REMAP_STRUCT;

#define IOHCx13b100b8                    0x13b100b8UL

/** PCIE_VDM_NODE_CTRL4
 * Reg(IOHC::PCIE_VDM_NODE_CTRL4)
 * Classification(NDA)
 *   Disposition(TBD)
 */
typedef union {
  struct {
    uint32_t                            BUS_RANGE_BASE:8;
    uint32_t                            BUS_RANGE_LIMIT:8;
    uint32_t                            Reserved_30_16:15;
    uint32_t                            NODE_PRESENT:1;
  } Field;
  uint32_t Value;
} PCIE_VDM_NODE_CTRL4_STRUCT;

#define IOHCx13b10190                     0x13b10190UL



/** NB_ADAPTER_ID_W
 * Reg(IOHC::NB_ADAPTER_ID_W)
 * Classification(NDA)
 *   Disposition(TBD)
 */
#define IOHCx13b00050    0x13b00050UL

/** NB_TOP_OF_DRAM_SLOT1
 * Reg(IOHC::NB_TOP_OF_DRAM_SLOT1)
 * Classification(NDA)
 *   Disposition(TBD)
 */
typedef union {
  struct {
    uint32_t                            TOP_OF_DRAM_BIT_32:1;
    uint32_t                            Reserved_22_1:22;
    uint32_t                            TOP_OF_DRAM:9;
  } Field;
  uint32_t Value;
} NB_TOP_OF_DRAM_SLOT1_STRUCT;

#define IOHCx13b00090                        0x13b00090UL

/***********************************************************
 * Register Name : NB_BUS_NUM_CNTL
 * Register Description :
 * GNB Bus Number Control.
 * Visibility : 0x2
 ************************************************************/
#define IOHCx13b10044    0x13b10044UL

/***********************************************************
 * Register Name : NB_LOWER_TOP_OF_DRAM2
 * Register Description :
 * Top of Memory 2 Low.
 * Visibility : 0x2
 ************************************************************/
typedef union {
  struct {
    uint32_t                            ENABLE:1;
    uint32_t                            Reserved_22_1:22;
    uint32_t                            LOWER_TOM2:9;
  } Field;
  uint32_t Value;
} NB_LOWER_TOP_OF_DRAM2_STRUCT;

#define IOHCx13b10064    0x13b10064UL


/***********************************************************
 * Register Name : NB_UPPER_TOP_OF_DRAM2
 * Register Description :
 * Top of Memory 2 High.
 * Visibility : 0x2
 ************************************************************/
typedef union {
  struct {
    uint32_t                            UPPER_TOM2:9;
    uint32_t                            Reserved_31_9:23;
  } Field;
  uint32_t Value;
} NB_UPPER_TOP_OF_DRAM2_STRUCT;

#define IOHCx13b10068    0x13b10068UL

/***********************************************************
 * Register Name : NB_TOP_OF_DRAM3
 * Register Description :
 * Top of Memory 3.
 * Visibility : 0x2
 ************************************************************/
typedef union {
  struct {
    uint32_t                            TOM3_LIMIT:30;
    uint32_t                            Reserved_30_30:1;
    uint32_t                            TOM3_ENABLE:1;
  } Field;
  uint32_t Value;
} NB_TOP_OF_DRAM3_STRUCT;

#define IOHCx13b10138    0x13b10138UL

/***********************************************************
 * Register Name : IOAPIC_BASE_ADDR_HI
 * Register Description :
 * IOAPIC MMIO base address.
 * Visibility : 0x2
 ************************************************************/
#define IOHCx13b102f4    0x13b102f4UL

/***********************************************************
 * Register Name : IOAPIC_BASE_ADDR_LO
 * Register Description :
 * IOAPIC MMIO base address.
 * Visibility : 0x2
 ************************************************************/
#define IOHCx13b102f0    0x13b102f0UL

/***********************************************************
 * Register Name : FEATURES_ENABLE
 * Register Description :
 * IOAPIC feature control bits.
 * Visibility : 0x2
 ************************************************************/
#define IOHCx14300000    0x14300000UL

#define IOHCx13b0004c    0x13b0004cul
#define IOHCx13b1005c    0x13b1005cul
#define IOHCx13b10060    0x13b10060ul
#define IOHCx13b00084    0x13b00084ul
