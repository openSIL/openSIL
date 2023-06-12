/**
 *  @file IohcReg.h
 *  @brief This file contains Iohc Register definitions
 */

/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#define SIL_RESERVED_53        0x1

#define SMN_IOHUB0NBIO0_CFG_IOHC_PCI_ADDRESS               0x13b10034UL

/***********************************************************
* Register Name : DBG_BASE_ADDR_HI
* Register Description :
* DBG MMIO base address.
* Visibility : 0x2
************************************************************/
#define SMN_IOHUB0NBIO0_DBG_BASE_ADDR_HI_ADDRESS           0x13b102fcUL

/***********************************************************
* Register Name : DBG_BASE_ADDR_LO
* Register Description :
* DBG MMIO base address.
* Visibility : 0x2
************************************************************/
#define SMN_IOHUB0NBIO0_DBG_BASE_ADDR_LO_ADDRESS           0x13b102f8UL

/***********************************************************
* Register Name : FASTREGCNTL_BASE_ADDR_HI
* Register Description :
* FastRegsCntl MMIO base address.
* Visibility : 0x2
************************************************************/
#define SMN_IOHUB0NBIO0_FASTREGCNTL_BASE_ADDR_HI_ADDRESS   0x13b1030cUL

/***********************************************************
* Register Name : FASTREGCNTL_BASE_ADDR_LO
* Register Description :
* FastRegsCntl MMIO base address.
* Visibility : 0x2
************************************************************/
#define SMN_IOHUB0NBIO0_FASTREGCNTL_BASE_ADDR_LO_ADDRESS   0x13b10308UL

/***********************************************************
* Register Name : FASTREG_BASE_ADDR_HI
* Register Description :
* FastRegs MMIO base address.
* Visibility : 0x2
************************************************************/
#define SMN_IOHUB0NBIO0_FASTREG_BASE_ADDR_HI_ADDRESS       0x13b10304UL

/***********************************************************
* Register Name : FASTREG_BASE_ADDR_LO
* Register Description :
* FastRegs MMIO base address.
* Visibility : 0x2
************************************************************/
#define SMN_IOHUB0NBIO0_FASTREG_BASE_ADDR_LO_ADDRESS       0x13b10300UL

/***********************************************************
* Register Name : IOAPIC_BASE_ADDR_HI
* Register Description :
* IOAPIC MMIO base address.
* Visibility : 0x2
************************************************************/
#define SMN_IOHUB0NBIO0_IOAPIC_BASE_ADDR_HI_ADDRESS        0x13b102f4UL

/***********************************************************
* Register Name : IOAPIC_BASE_ADDR_LO
* Register Description :
* IOAPIC MMIO base address.
* Visibility : 0x2
************************************************************/
#define SMN_IOHUB0NBIO0_IOAPIC_BASE_ADDR_LO_ADDRESS        0x13b102f0UL

#define SIL_RESERVED_116      0
#define SIL_RESERVED_115      0x1
#define SIL_RESERVED_118      2
#define SIL_RESERVED_117      0x4
#define SIL_RESERVED_120      18
#define SIL_RESERVED_119      0x40000
#define SIL_RESERVED_746      0x13b31004UL

/***********************************************************
* Register Name : IOHC_FEATURE_CNTL
* Register Description :
* Control bits for misc IOHC features.
* Visibility : 0x2
************************************************************/
// Bitfield Description : Peer to peer mode: //00 -  Mode 0 : DMA writes not matching a valid P2P range are dropped
//(Legacy operation) //01 - Mode 1 : DMA writes not matching a valid P2P range are forwarded to the DF //10 - Mode 2 : all
//DMA writes are forwarded to the DF //11 - Mode 3: Disable P2P
#define IOHC_FEATURE_CNTL_P2P_mode_OFFSET      1
// Bitfield Description : Enables the per-port ARI_EN signals from the IOHC shadow registers. This is used for decoding
//external configuration requests to the bridges.
#define IOHC_FEATURE_CNTL_IOHC_ARI_SUPPORTED_OFFSET      22
#define IOHC_FEATURE_CNTL_IOHC_ARI_SUPPORTED_MASK        0x400000
#define SMN_IOHUB0NBIO0_IOHC_FEATURE_CNTL_ADDRESS        0x13b10118UL

#define SIL_RESERVED_155      22
#define SIL_RESERVED_154      0x400000
#define SIL_RESERVED_153      23
#define SIL_RESERVED_152      0x800000
#define SIL_RESERVED_151      24
#define IOHC_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK7_MASK        0x1000000
#define SIL_RESERVED_150      25
#define SIL_RESERVED_149      0x2000000
#define SIL_RESERVED_148      26
#define SIL_RESERVED_147      0x4000000
#define SIL_RESERVED_146      27
#define SIL_RESERVED_145      0x8000000
#define SIL_RESERVED_144      28
#define SIL_RESERVED_143      0x10000000
#define SIL_RESERVED_142      29
#define SIL_RESERVED_141      0x20000000
#define SIL_RESERVED_140      30
#define SIL_RESERVED_139      0x40000000
#define SIL_RESERVED_138      31
#define SIL_RESERVED_137      0x80000000

#define SIL_RESERVED_732    0x13b10088UL

#define IOHC_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK9_OFFSET      22
#define SIL_RESERVED_172      0x400000
#define SIL_RESERVED_171      23
#define SIL_RESERVED_170      0x800000
#define SIL_RESERVED_169      24
#define SIL_RESERVED_168      0x1000000
#define SIL_RESERVED_167      25
#define SIL_RESERVED_166      0x2000000
#define SIL_RESERVED_165      26
#define SIL_RESERVED_164      0x4000000
#define SIL_RESERVED_163      27
#define SIL_RESERVED_162      0x8000000
#define SIL_RESERVED_161      28
#define SIL_RESERVED_160      0x10000000
#define SIL_RESERVED_159      29
#define SIL_RESERVED_158      0x20000000
#define SIL_RESERVED_177      30
#define SIL_RESERVED_157      0x40000000
#define SIL_RESERVED_156      31
#define IOHC_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK0_MASK        0x80000000

#define SIL_RESERVED_733    0x13b1008cUL

#define SIL_RESERVED_190      22
#define SIL_RESERVED_189      0x400000
#define SIL_RESERVED_188      23
#define SIL_RESERVED_187      0x800000
#define SIL_RESERVED_186      24
#define IOHC_GLUE_CG_LCLK_CTRL_2_SOFT_OVERRIDE_CLK7_MASK        0x1000000
#define IOHC_GLUE_CG_LCLK_CTRL_2_SOFT_OVERRIDE_CLK6_OFFSET      25
#define SIL_RESERVED_185      0x2000000
#define SIL_RESERVED_184      26
#define SIL_RESERVED_183      0x4000000
#define SIL_RESERVED_182      27
#define SIL_RESERVED_181      0x8000000
#define SIL_RESERVED_180      28
#define SIL_RESERVED_179      0x10000000
#define SIL_RESERVED_178      29
#define IOHC_GLUE_CG_LCLK_CTRL_2_SOFT_OVERRIDE_CLK2_MASK        0x20000000
#define SIL_RESERVED_176      30
#define SIL_RESERVED_175      0x40000000
#define SIL_RESERVED_174      31
#define SIL_RESERVED_173      0x80000000

#define SIL_RESERVED_734      0x13b10090UL

#define IOHC_PCIE_CRS_Count_CrsDelayCount_OFFSET      0
#define IOHC_PCIE_CRS_Count_CrsDelayCount_MASK        0xffff
#define IOHC_PCIE_CRS_Count_CrsLimitCount_OFFSET      16
#define IOHC_PCIE_CRS_Count_CrsLimitCount_MASK        0xfff0000
#define SMN_IOHUB0NBIO0_IOHC_PCIE_CRS_Count_ADDRESS   0x13b10028UL

#define SIL_RESERVED_192      8
#define SIL_RESERVED_191      0x100
#define SIL_RESERVED_735      0x13b10340UL

#define IOHC_QOS_CONTROL_VC0QoSPriority_OFFSET      0
#define SIL_RESERVED_193      0xf
#define SIL_RESERVED_194      4
#define IOHC_QOS_CONTROL_VC1QoSPriority_MASK        0xf0
#define SIL_RESERVED_196      8
#define SIL_RESERVED_195      0xf00
#define SIL_RESERVED_198      12
#define SIL_RESERVED_197      0xf000
#define SIL_RESERVED_200      16
#define SIL_RESERVED_199      0xf0000
#define SIL_RESERVED_201      20
#define IOHC_QOS_CONTROL_VC5QoSPriority_MASK        0xf00000
#define SIL_RESERVED_203      24
#define SIL_RESERVED_202      0xf000000
#define SIL_RESERVED_205      28
#define SIL_RESERVED_204      0xf0000000

#define SIL_RESERVED_736      0x13b14040UL

#define SIL_RESERVED_208      0
#define SIL_RESERVED_206      0x1
#define SIL_RESERVED_209      1
#define SIL_RESERVED_207      0x2
#define SIL_RESERVED_211      2
#define SIL_RESERVED_210      0x4
#define SIL_RESERVED_737      0x13b10020UL

#define SIL_RESERVED_546      0x13b14410UL
#define SMN_IOHUB0_N1NBIO0_IOHC_SION_S0_CLIENT_RDRSP_BURSTTARGET_LOWER_ADDRESS    0x13b14814UL
#define SMN_IOHUB0_N2NBIO0_IOHC_SION_S0_CLIENT_RDRSP_BURSTTARGET_LOWER_ADDRESS    0x13b14c18UL
#define SIL_RESERVED_652    0x13b1501cUL
#define SIL_RESERVED_683    0x13b15420UL
#define SMN_IOHUB0_N5NBIO0_IOHC_SION_S0_CLIENT_RDRSP_BURSTTARGET_LOWER_ADDRESS    0x13b15824UL
#define SIL_RESERVED_720    0x13b15c28UL

#define SIL_RESERVED_547    0x13b14414UL
#define SIL_RESERVED_581    0x13b14818UL
#define SMN_IOHUB0_N2NBIO0_IOHC_SION_S0_CLIENT_RDRSP_BURSTTARGET_UPPER_ADDRESS    0x13b14c1cUL
#define SIL_RESERVED_653    0x13b15020UL
#define SIL_RESERVED_684    0x13b15424UL
#define SIL_RESERVED_711    0x13b15828UL
#define SIL_RESERVED_721    0x13b15c2cUL

#define SIL_RESERVED_548    0x13b14400UL
#define SIL_RESERVED_582    0x13b14804UL
#define SIL_RESERVED_619    0x13b14c08UL
#define SIL_RESERVED_654    0x13b1500cUL
#define SIL_RESERVED_685    0x13b15410UL
#define SIL_RESERVED_712    0x13b15814UL
#define SIL_RESERVED_722    0x13b15c18UL

#define SMN_IOHUB0_N0NBIO0_IOHC_SION_S0_CLIENT_REQ_BURSTTARGET_UPPER_ADDRESS    0x13b14404UL
#define SIL_RESERVED_583    0x13b14808UL
#define SIL_RESERVED_620    0x13b14c0cUL
#define SMN_IOHUB0_N3NBIO0_IOHC_SION_S0_CLIENT_REQ_BURSTTARGET_UPPER_ADDRESS    0x13b15010UL
#define SIL_RESERVED_686    0x13b15414UL
#define SIL_RESERVED_713    0x13b15818UL
#define SIL_RESERVED_723    0x13b15c1cUL

#define SIL_RESERVED_549    0x13b14408UL
#define SIL_RESERVED_584    0x13b1480cUL
#define SIL_RESERVED_621    0x13b14c10UL
#define SIL_RESERVED_655    0x13b15014UL
#define SIL_RESERVED_687    0x13b15418UL
#define SIL_RESERVED_714    0x13b1581cUL

#define SIL_RESERVED_550    0x13b1440cUL
#define SIL_RESERVED_585    0x13b14810UL
#define SIL_RESERVED_622    0x13b14c14UL
#define SIL_RESERVED_656    0x13b15018UL
#define SIL_RESERVED_688    0x13b1541cUL
#define SIL_RESERVED_715    0x13b15820UL

#define SIL_RESERVED_551    0x13b14440UL
#define SIL_RESERVED_586    0x13b14844UL
#define SMN_IOHUB0_N2NBIO0_IOHC_SION_S1_CLIENT_RDRSP_BURSTTARGET_LOWER_ADDRESS    0x13b14c48UL
#define SIL_RESERVED_657    0x13b1504cUL
#define SIL_RESERVED_689    0x13b15450UL
#define SIL_RESERVED_716    0x13b15854UL
#define SIL_RESERVED_724    0x13b15c58UL

#define SIL_RESERVED_552    0x13b14444UL
#define SIL_RESERVED_587    0x13b14848UL
#define SIL_RESERVED_623    0x13b14c4cUL
#define SIL_RESERVED_658    0x13b15050UL
#define SIL_RESERVED_690    0x13b15454UL
#define SIL_RESERVED_717    0x13b15858UL
#define SIL_RESERVED_725    0x13b15c5cUL

#define SIL_RESERVED_553    0x13b14430UL
#define SIL_RESERVED_588    0x13b14834UL
#define SIL_RESERVED_624    0x13b14c38UL
#define SIL_RESERVED_659    0x13b1503cUL
#define SIL_RESERVED_691    0x13b15440UL
#define SIL_RESERVED_718    0x13b15844UL
#define SIL_RESERVED_726    0x13b15c48UL

#define SIL_RESERVED_554    0x13b14434UL
#define SIL_RESERVED_589    0x13b14838UL
#define SMN_IOHUB0_N2NBIO0_IOHC_SION_S1_CLIENT_REQ_BURSTTARGET_UPPER_ADDRESS    0x13b14c3cUL
#define SIL_RESERVED_660    0x13b15040UL
#define SIL_RESERVED_692    0x13b15444UL
#define SMN_IOHUB0_N5NBIO0_IOHC_SION_S1_CLIENT_REQ_BURSTTARGET_UPPER_ADDRESS    0x13b15848UL
#define SIL_RESERVED_727    0x13b15c4cUL

/***********************************************************
* Register Name : NB_BUS_NUM_CNTL
* Register Description :
* GNB Bus Number Control.
* Visibility : 0x2
************************************************************/
typedef union {
  struct {
    uint32_t                            NB_BUS_NUM:8;
    uint32_t                            NB_BUS_LAT_Mode:1;
    uint32_t                            Reserved_15_9:7;
    uint32_t                            NB_SEGMENT:8;
    uint32_t                            Reserved_31_24:8;
  } Field;
  uint32_t Value;
} NB_BUS_NUM_CNTL_STRUCT;

#define SMN_IOHUB0NBIO0_NB_BUS_NUM_CNTL_ADDRESS    0x13b10044UL

/***********************************************************
* Register Name : NB_CXL_CFG1
* Register Description :
* CXL Configuration Register 1
* Visibility : 0x2
************************************************************/

#define SMN_NB_CXL_CFG1_ADDRESS    0x13b15cc8UL

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

#define SMN_IOHUB0NBIO0_NB_LOWER_TOP_OF_DRAM2_ADDRESS    0x13b10064UL

/***********************************************************
* Register Name : NB_PCIE0_PORTA_CXL_RCRB_BASE_ADDR_HI
* Register Description :
* PCIE0 PORTA CXL RCRB Base Register High.
* Visibility : 0x2
************************************************************/
typedef union {
  struct {
    uint32_t                            PCIE0_PORTA_CXL_RCRB_BASE_ADDR_HI:32;
  } Field;
  uint32_t Value;
} NB_PCIE0_PORTA_CXL_RCRB_BASE_ADDR_HI_STRUCT;

#define SMN_IOHUB0NBIO0_NB_PCIE0_PORTA_CXL_RCRB_BASE_ADDR_HI_ADDRESS    0x13b15cd4UL

/***********************************************************
* Register Name : NB_PCIE0_PORTA_CXL_RCRB_BASE_ADDR_LO
* Register Description :
* PCIE0 PORTA CXL RCRB Base Register Low.
* Visibility : 0x2
************************************************************/
typedef union {
  struct {
    uint32_t                            PCIE0_PORTA_CXL_RCRB_ENABLE:1;
    uint32_t                            Reserved_12_1:12;
    uint32_t                            PCIE0_PORTA_CXL_RCRB_BASE_ADDR_LO_31_13:19;
  } Field;
  uint32_t Value;
} NB_PCIE0_PORTA_CXL_RCRB_BASE_ADDR_LO_STRUCT;

#define SMN_IOHUB0NBIO0_NB_PCIE0_PORTA_CXL_RCRB_BASE_ADDR_LO_ADDRESS    0x13b15cd0UL

/***********************************************************
* Register Name : NB_PROG_DEVICE_REMAP
* Register Description :
* Software Device Remapping Register.
* Visibility : 0x2
************************************************************/
typedef union {
  struct {
    uint32_t                            DevFnMap:8;
    uint32_t                            Reserved_31_8:24;
  } Field;
  uint32_t Value;
} NB_PROG_DEVICE_REMAP_STRUCT;

#define SMN_IOHUB0_N0NBIO0_NB_PROG_DEVICE_REMAP_ADDRESS    0x13b100b8UL

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

#define SMN_IOHUB0NBIO0_NB_TOP_OF_DRAM3_ADDRESS    0x13b10138UL

/***********************************************************
* Register Name : NB_TOP_OF_DRAM_SLOT1
* Register Description :
* North Bridge Memory Base Register
* Visibility : 0x2
************************************************************/
typedef union {
  struct {
    uint32_t                            TOP_OF_DRAM_BIT_32:1;
    uint32_t                            Reserved_22_1:22;
    uint32_t                            TOP_OF_DRAM:9;
  } Field;
  uint32_t Value;
} NB_TOP_OF_DRAM_SLOT1_STRUCT;

#define SMN_IOHUB0NBIO0_NB_TOP_OF_DRAM_SLOT1_ADDRESS    0x13b00090UL

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

#define SMN_IOHUB0NBIO0_NB_UPPER_TOP_OF_DRAM2_ADDRESS    0x13b10068UL

/***********************************************************
* Register Name : PCIE_VDM_CNTL2
* Register Description :
* PCIE VDM / MCTP VDM control register.
* Visibility : 0x2
************************************************************/

// Bitfield Description : This register controls the routing of ID-routed vendor defined messages 0 - Local VDM P2P only
//1 - Local VDM P2P + Remote VDM P2P 2 - Remote P2P only. VDMs to local targets are reflected off the Data Fabric. 3 - VDM
//ID-routing is disabled. All ID-routed VDMs are terminated.
#define PCIE_VDM_CNTL2_VdmP2pMode_OFFSET      0
#define PCIE_VDM_CNTL2_VdmP2pMode_MASK        0x3

#define SMN_IOHUB0NBIO0_PCIE_VDM_CNTL2_ADDRESS    0x13b10230UL

/***********************************************************
* Register Name : PCIE_VDM_NODE_CTRL4
* Register Description :
* PCIE VDM controls for node0.
* Visibility : 0x2
************************************************************/
typedef union {
  struct {
    uint32_t                            BUS_RANGE_BASE:8;
    uint32_t                            BUS_RANGE_LIMIT:8;
    uint32_t                            Reserved_30_16:15;
    uint32_t                            NODE_PRESENT:1;
  } Field;
  uint32_t Value;
} PCIE_VDM_NODE_CTRL4_STRUCT;

#define SMN_IOHUB0_N0NBIO0_PCIE_VDM_NODE_CTRL4_ADDRESS    0x13b10190UL

#define SIL_RESERVED_444      0
#define POISON_ACTION_CONTROL_IntPoisonAPMLErrEn_MASK        0x1
#define SIL_RESERVED_446      3
#define SIL_RESERVED_445        0x8
#define SIL_RESERVED_448      4
#define SIL_RESERVED_447        0x10
#define SIL_RESERVED_740    0x13b20814UL

/***********************************************************
* Register Name : SB_LOCATION
* Register Description :  * South Bridge Location Register. Register set 0 indicates the GNB is secondary and there is
//no SB/FCH connected.
* Visibility : 0x2
************************************************************/

#define SMN_IOHUB0NBIO0_SB_LOCATION_ADDRESS    0x13b1007cUL

/***********************************************************
* Register Name : SMU_BASE_ADDR_HI
* Register Description :
* SMU MMIO base address
* Visibility : 0x2
************************************************************/

#define SMN_IOHUB0NBIO0_SMU_BASE_ADDR_HI_ADDRESS    0x13b102ecUL

/***********************************************************
* Register Name : SMU_BASE_ADDR_LO
* Register Description :
* SMU MMIO base address.
* Visibility : 0x2
************************************************************/

#define SMN_IOHUB0NBIO0_SMU_BASE_ADDR_LO_ADDRESS    0x13b102e8UL

#define SIL_RESERVED_774      0
#define SIL_RESERVED_773        0x7f
#define SIL_RESERVED_777      8
#define SIL_RESERVED_776        0xf00
#define USB_QoS_CNTL_UnitID0Enable_OFFSET      12
#define SIL_RESERVED_775        0x1000
#define SIL_RESERVED_779      16
#define SIL_RESERVED_778        0x7f0000
#define SIL_RESERVED_782      24
#define USB_QoS_CNTL_UnitID1QoSPriority_MASK        0xf000000
#define SIL_RESERVED_781      28
#define SIL_RESERVED_780        0x10000000
#define SIL_RESERVED_743    0x13b14044UL
