/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 *  @file Iommul1Reg.h
 *  @brief This file contains Iommul1 Register definitions
 */

#pragma once


#define L1_CLKCNTRL_0_L1_DMA_CLKGATE_EN_OFFSET      4
#define L1_CLKCNTRL_0_L1_DMA_CLKGATE_EN_MASK        0x10

#define L1_CLKCNTRL_0_L1_CACHE_CLKGATE_EN_OFFSET      5
#define L1_CLKCNTRL_0_L1_CACHE_CLKGATE_EN_MASK        0x20

#define L1_CLKCNTRL_0_L1_CPSLV_CLKGATE_EN_OFFSET      6
#define L1_CLKCNTRL_0_L1_CPSLV_CLKGATE_EN_MASK        0x40

#define L1_CLKCNTRL_0_L1_PERF_CLKGATE_EN_OFFSET      8
#define L1_CLKCNTRL_0_L1_PERF_CLKGATE_EN_MASK        0x100

#define L1_CLKCNTRL_0_L1_MEMORY_CLKGATE_EN_OFFSET      9
#define L1_CLKCNTRL_0_L1_MEMORY_CLKGATE_EN_MASK        0x200

#define L1_CLKCNTRL_0_L1_REG_CLKGATE_EN_OFFSET      10
#define L1_CLKCNTRL_0_L1_REG_CLKGATE_EN_MASK        0x400

#define L1_CLKCNTRL_0_L1_HOSTREQ_CLKGATE_EN_OFFSET      11
#define L1_CLKCNTRL_0_L1_HOSTREQ_CLKGATE_EN_MASK        0x800

#define L1_CLKCNTRL_0_L1_DMARSP_CLKGATE_EN_OFFSET      12
#define L1_CLKCNTRL_0_L1_DMARSP_CLKGATE_EN_MASK        0x1000

#define L1_CLKCNTRL_0_L1_HOSTRSP_CLKGATE_EN_OFFSET      13
#define L1_CLKCNTRL_0_L1_HOSTRSP_CLKGATE_EN_MASK        0x2000

#define L1_CLKCNTRL_0_L1_SION_PERF_CLKGATE_EN_OFFSET      14
#define L1_CLKCNTRL_0_L1_SION_PERF_CLKGATE_EN_MASK        0x4000

#define L1_CLKCNTRL_0_L1_CLKGATE_HYSTERESIS_OFFSET      15
#define L1_CLKCNTRL_0_L1_CLKGATE_HYSTERESIS_MASK        0x7f8000

#define L1_CLKCNTRL_0_L1_L2_CLKGATE_EN_OFFSET      31
#define L1_CLKCNTRL_0_L1_L2_CLKGATE_EN_MASK        0x80000000

#define SIL_RSVD_ADDR_153000CC             0x153000ccUL
#define SIL_RSVD_ADDR_147000CC             0x147000ccUL
#define SIL_RSVD_ADDR_14B000CC          0x14b000ccUL

#define L1_CNTRL_2_CPD_RESP_MODE_OFFSET      24
#define L1_CNTRL_2_CPD_RESP_MODE_MASK        0x7000000

#define L1_CNTRL_2_L1NonConsumedDataErrorSignalEn_OFFSET      27
#define L1_CNTRL_2_L1NonConsumedDataErrorSignalEn_MASK        0x8000000

#define L1_CNTRL_2_L1ConsumedDataErrorSignalEn_OFFSET      28
#define L1_CNTRL_2_L1ConsumedDataErrorSignalEn_MASK        0x10000000

#define SIL_RSVD_ADDR_15300038                0x15300038UL
#define SIL_RSVD_ADDR_14700038                0x14700038UL
#define SIL_RSVD_ADDR_14B00038                0x14b00038UL

#define L1_CNTRL_4_ATS_multiple_resp_en_OFFSET      0
#define L1_CNTRL_4_ATS_multiple_resp_en_MASK        0x1

#define L1_CNTRL_4_Timeout_pulse_ext_En_OFFSET      2
#define L1_CNTRL_4_Timeout_pulse_ext_En_MASK        0x4

#define SIL_RSVD_ADDR_153000C8                0x153000c8UL
#define SIL_RSVD_ADDR_147000C8                0x147000c8UL
#define SIL_RSVD_ADDR_14B000C8                0x14b000c8UL


#define L1_FEATURE_CNTRL_PMR_lock_bit_OFFSET      1
#define L1_FEATURE_CNTRL_PMR_lock_bit_MASK        0x2

#define L1_FEATURE_CNTRL_EXE_lock_bit_OFFSET      2
#define L1_FEATURE_CNTRL_EXE_lock_bit_MASK        0x4

#define SIL_RSVD_ADDR_1530009C          0x1530009cUL
#define SIL_RSVD_ADDR_1470009C          0x1470009cUL
#define SIL_RSVD_ADDR_14B0009C          0x14b0009cUL


#define L1_FEATURE_SUP_CNTRL_L1_XT_SUP_W_OFFSET      5
#define L1_FEATURE_SUP_CNTRL_L1_XT_SUP_W_MASK        0x20

#define SIL_RSVD_ADDR_153000DC                              0x153000dcUL
#define SIL_RSVD_ADDR_15300274      0x15300274UL
#define SIL_RSVD_ADDR_14700274      0x14700274UL
#define SIL_RSVD_ADDR_14B00274      0x14b00274UL

#define SIL_RESERVED_0762                0
#define SIL_RESERVED_0761                  0x1ff

// Bitfield Description : Read-write. Reset: 0. Sets the HTR_IGN_SUP value in IOMMU Extended feature registers
#define SIL_RESERVED_0760                30
#define SIL_RESERVED_0759                  0x40000000

#define SIL_RSVD_ADDR_15300288               0x15300288UL
#define SIL_RSVD_ADDR_14700288               0x14700288UL
#define SIL_RSVD_ADDR_14B00288               0x14b00288UL

#define SIL_RSVD_ADDR_15300024                             0x15300024UL
#define SIL_RESERVED_0775           0x1530001cUL

#define L1_SDP_CLKREQ_CNTRL_HW_PG_WAKEUP_EN_DMA_OFFSET        0
#define L1_SDP_CLKREQ_CNTRL_HW_PG_WAKEUP_EN_DMA_MASK          0x1
#define SIL_RESERVED_0766        1
#define SIL_RESERVED_0765          0x2
#define SIL_RESERVED_0764     2
#define SIL_RESERVED_0763       0x4
#define SIL_RESERVED_0768    3
#define SIL_RESERVED_0767      0x8
#define SIL_RESERVED_0770        4
#define SIL_RESERVED_0769          0x10

#define SIL_RSVD_ADDR_153000D4       0x153000d4UL
#define SIL_RSVD_ADDR_147000D4       0x147000d4UL
#define SIL_RSVD_ADDR_14B000D4       0x14b000d4UL

#define SIL_RESERVED_0772    8
#define SIL_RESERVED_0771      0xff00

#define L1_SDP_DMA_DATA_CREDIT_1_VC7_DMA_DATA_CREDIT_OFFSET      24
#define L1_SDP_DMA_DATA_CREDIT_1_VC7_DMA_DATA_CREDIT_MASK        0xff000000

#define SIL_RSVD_ADDR_15300120    0x15300120UL

#define L1_SDP_DMA_RDRSP_CREDIT_VC4_DMA_RDRSP_CREDIT_OFFSET      16
#define L1_SDP_DMA_RDRSP_CREDIT_VC4_DMA_RDRSP_CREDIT_MASK        0xf0000

#define SIL_RSVD_ADDR_15300124     0x15300124UL

#define SIL_RESERVED_0774      8
#define SIL_RESERVED_0773        0xff00
#define L1_SDP_DMA_REQ_CREDIT_1_VC7_DMA_REQ_CREDIT_OFFSET      24
#define L1_SDP_DMA_REQ_CREDIT_1_VC7_DMA_REQ_CREDIT_MASK        0xff000000

#define SIL_RSVD_ADDR_15300118   0x15300118UL

#define L1_SDP_DMA_WRRSP_CREDIT_VC4_DMA_WRRSP_CREDIT_OFFSET      16
#define L1_SDP_DMA_WRRSP_CREDIT_VC4_DMA_WRRSP_CREDIT_MASK        0xf0000

#define L1_SDP_DMA_WRRSP_CREDIT_VC6_DMA_WRRSP_CREDIT_OFFSET      24
#define L1_SDP_DMA_WRRSP_CREDIT_VC6_DMA_WRRSP_CREDIT_MASK        0xf000000

#define SIL_RSVD_ADDR_15300128     0x15300128UL


#define L1_SDP_MAXCRED_0_L1_DYNAMIC_CRED_RELOCATION_EN_OFFSET      31
#define L1_SDP_MAXCRED_0_L1_DYNAMIC_CRED_RELOCATION_EN_MASK        0x80000000

#define SIL_RSVD_ADDR_1530010C          0x1530010cUL
#define SIL_RSVD_ADDR_1470010C          0x1470010cUL
#define SIL_RSVD_ADDR_14B0010C          0x14b0010cUL

#define SIL_RSVD_ADDR_15304018          0x15304018UL
#define SIL_RSVD_ADDR_14704018          0x14704018UL
#define SIL_RSVD_ADDR_14b04018          0x14b04018UL

#define SIL_RESERVED_1768      5
#define SIL_RESERVED_1767        0xe0
