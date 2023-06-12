/**
 *  @file Iommul1Reg.h
 *  @brief This file contains Iommul1 Register definitions
 */

/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

/***********************************************************
* Register Name : L1_CLKCNTRL_0
* Register Description :
*
* Visibility : 0x2
************************************************************/
// Bitfield Description : Enables clock gating for internal DMA path blocks.
#define L1_CLKCNTRL_0_L1_DMA_CLKGATE_EN_OFFSET      4
#define L1_CLKCNTRL_0_L1_DMA_CLKGATE_EN_MASK        0x10
// Bitfield Description : Enables clock gating for internal Cache blocks.
#define L1_CLKCNTRL_0_L1_CACHE_CLKGATE_EN_OFFSET      5
#define L1_CLKCNTRL_0_L1_CACHE_CLKGATE_EN_MASK        0x20
// Bitfield Description : Enables clock gating for internal Command Process Secondary blocks.
#define L1_CLKCNTRL_0_L1_CPSLV_CLKGATE_EN_OFFSET      6
#define L1_CLKCNTRL_0_L1_CPSLV_CLKGATE_EN_MASK        0x40
// Bitfield Description : Enables clock gating for internal Performance blocks.
#define L1_CLKCNTRL_0_L1_PERF_CLKGATE_EN_OFFSET      8
#define L1_CLKCNTRL_0_L1_PERF_CLKGATE_EN_MASK        0x100
// Bitfield Description : Enables clock gating for internal memory blocks.
#define L1_CLKCNTRL_0_L1_MEMORY_CLKGATE_EN_OFFSET      9
#define L1_CLKCNTRL_0_L1_MEMORY_CLKGATE_EN_MASK        0x200
// Bitfield Description : Enables clock gating for internal register blocks.
#define L1_CLKCNTRL_0_L1_REG_CLKGATE_EN_OFFSET      10
#define L1_CLKCNTRL_0_L1_REG_CLKGATE_EN_MASK        0x400
// Bitfield Description : Enables clock gating for internal HOST path blocks.
#define L1_CLKCNTRL_0_L1_HOSTREQ_CLKGATE_EN_OFFSET      11
#define L1_CLKCNTRL_0_L1_HOSTREQ_CLKGATE_EN_MASK        0x800
// Bitfield Description : Enables clock gating for internal DMA Response path blocks.
#define L1_CLKCNTRL_0_L1_DMARSP_CLKGATE_EN_OFFSET      12
#define L1_CLKCNTRL_0_L1_DMARSP_CLKGATE_EN_MASK        0x1000
// Bitfield Description : Enables clock gating for internal HOST Response path blocks.
#define L1_CLKCNTRL_0_L1_HOSTRSP_CLKGATE_EN_OFFSET      13
#define L1_CLKCNTRL_0_L1_HOSTRSP_CLKGATE_EN_MASK        0x2000
// Bitfield Description : Enables clock gating for sion Performance blocks.
#define L1_CLKCNTRL_0_L1_SION_PERF_CLKGATE_EN_OFFSET      14
#define L1_CLKCNTRL_0_L1_SION_PERF_CLKGATE_EN_MASK        0x4000
// Bitfield Description : IOMMU L1 Clock branch Hysteresis used in all the branches - Reset to 16 clycles.
#define L1_CLKCNTRL_0_L1_CLKGATE_HYSTERESIS_OFFSET      15
#define L1_CLKCNTRL_0_L1_CLKGATE_HYSTERESIS_MASK        0x7f8000
// Bitfield Description : 0x0 - L1 does not wait for L2toL1ClkGrant before sending request, 0x1-L1 needs to receive
//L2toL1ClkGrant before sending request.  This register must be set if L2 clock gating or L2 power gating is enabled.
#define L1_CLKCNTRL_0_L1_L2_CLKGATE_EN_OFFSET      31
#define L1_CLKCNTRL_0_L1_L2_CLKGATE_EN_MASK        0x80000000

#define SMN_IOAGR_IOHUB0NBIO0_L1_CLKCNTRL_0_ADDRESS    0x153000ccUL
#define SMN_PCIE0_IOHUB0NBIO0_L1_CLKCNTRL_0_ADDRESS    0x147000ccUL
#define SMN_PCIE1_IOHUB0NBIO0_L1_CLKCNTRL_0_ADDRESS    0x14b000ccUL

/***********************************************************
* Register Name : L1_CNTRL_2
* Register Description :
*
* Visibility : 0x2
************************************************************/
// Bitfield Description : 000 = No response taken when IOMMU consumes poisoned data. //001 = ErrEvent issued when IOMMU
//consumes poisoned data. //010 = Target Abort issued when IOMMU consumes poisoned data. //011 = Master Abort issued when
//IOMMU consumes poisoned data.
#define L1_CNTRL_2_CPD_RESP_MODE_OFFSET      24
#define L1_CNTRL_2_CPD_RESP_MODE_MASK        0x7000000
// Bitfield Description : Enables asserting Data Error on egress sdp request when parity check fails on IOMMU non-
//consumed data.
#define L1_CNTRL_2_L1NonConsumedDataErrorSignalEn_OFFSET      27
#define L1_CNTRL_2_L1NonConsumedDataErrorSignalEn_MASK        0x8000000
// Bitfield Description : Enables asserting Data Error on egress sdp request when parity check fails on IOMMU consumed
//data.
#define L1_CNTRL_2_L1ConsumedDataErrorSignalEn_OFFSET      28
#define L1_CNTRL_2_L1ConsumedDataErrorSignalEn_MASK        0x10000000

#define SMN_IOAGR_IOHUB0NBIO0_L1_CNTRL_2_ADDRESS    0x15300038UL
#define SMN_PCIE0_IOHUB0NBIO0_L1_CNTRL_2_ADDRESS    0x14700038UL
#define SMN_PCIE1_IOHUB0NBIO0_L1_CNTRL_2_ADDRESS    0x14b00038UL

/***********************************************************
* Register Name : L1_CNTRL_4
* Register Description :
*
* Visibility : 0x2
************************************************************/

// Bitfield Description : Allow multiple ATS responses for a large sized ATS request.
#define L1_CNTRL_4_ATS_multiple_resp_en_OFFSET      0
#define L1_CNTRL_4_ATS_multiple_resp_en_MASK        0x1
// Bitfield Description : Use external pulse for Invalidation Timeout.
#define L1_CNTRL_4_Timeout_pulse_ext_En_OFFSET      2
#define L1_CNTRL_4_Timeout_pulse_ext_En_MASK        0x4

#define SMN_IOAGR_IOHUB0NBIO0_L1_CNTRL_4_ADDRESS    0x153000c8UL
#define SMN_PCIE0_IOHUB0NBIO0_L1_CNTRL_4_ADDRESS    0x147000c8UL
#define SMN_PCIE1_IOHUB0NBIO0_L1_CNTRL_4_ADDRESS    0x14b000c8UL

/***********************************************************
* Register Name : L1_FEATURE_CNTRL
* Register Description :
*
* Visibility : 0x2
************************************************************/
// Bitfield Description : Bits programmed 1 remain 1 until reset. Sticky lock bits for the PMR permission checks.
#define L1_FEATURE_CNTRL_PMR_lock_bit_OFFSET      1
#define L1_FEATURE_CNTRL_PMR_lock_bit_MASK        0x2
// Bitfield Description : Bits programmed 1 remain 1 until reset. Sticky lock bits for the EXE permission checks.
#define L1_FEATURE_CNTRL_EXE_lock_bit_OFFSET      2
#define L1_FEATURE_CNTRL_EXE_lock_bit_MASK        0x4

#define SMN_IOAGR_IOHUB0NBIO0_L1_FEATURE_CNTRL_ADDRESS    0x1530009cUL
#define SMN_PCIE0_IOHUB0NBIO0_L1_FEATURE_CNTRL_ADDRESS    0x1470009cUL
#define SMN_PCIE1_IOHUB0NBIO0_L1_FEATURE_CNTRL_ADDRESS    0x14b0009cUL

/***********************************************************
* Register Name : L1_FEATURE_SUP_CNTRL
* Register Description :
*
* Visibility : 0x2
************************************************************/
// Bitfield Description : Must be programmed to the same value as in IOMMUL2::IOMMU_MMIO_CONTROL0_W[XT_SUP_W].
#define L1_FEATURE_SUP_CNTRL_L1_XT_SUP_W_OFFSET      5
#define L1_FEATURE_SUP_CNTRL_L1_XT_SUP_W_MASK        0x20

#define SMN_IOAGR_IOHUB0NBIO0_L1_FEATURE_SUP_CNTRL_ADDRESS    0x153000dcUL

/***********************************************************
* Register Name : L1_SB_LOCATION
* Register Description :
*
* Visibility : 0x2
************************************************************/

#define SMN_IOAGR_IOHUB0NBIO0_L1_SB_LOCATION_ADDRESS    0x15300024UL

/***********************************************************
* Register Name : L1_SDP_CLKREQ_CNTRL
* Register Description :
*
* Visibility : 0x2
************************************************************/
// Bitfield Description : Connect the ingress OrigClkReq signal on the client SDP interface to the OrigClkReq on the
//L1_IOHC interface, and the L1_L2_req_clkreq to the L2.
#define L1_SDP_CLKREQ_CNTRL_HW_PG_WAKEUP_EN_DMA_OFFSET      0
#define L1_SDP_CLKREQ_CNTRL_HW_PG_WAKEUP_EN_DMA_MASK        0x1

#define SMN_IOAGR_IOHUB0NBIO0_L1_SDP_CLKREQ_CNTRL_ADDRESS    0x153000d4UL
#define SMN_PCIE0_IOHUB0NBIO0_L1_SDP_CLKREQ_CNTRL_ADDRESS    0x147000d4UL
#define SMN_PCIE1_IOHUB0NBIO0_L1_SDP_CLKREQ_CNTRL_ADDRESS    0x14b000d4UL

/***********************************************************
* Register Name : L1_SDP_DMA_DATA_CREDIT_1
* Register Description :
* Register used to control the credit assignment to VC for dma data.
* Visibility : 0x2
************************************************************/
// Bitfield Description : Credits on VC7 for dma data
#define L1_SDP_DMA_DATA_CREDIT_1_VC7_DMA_DATA_CREDIT_OFFSET      24
#define L1_SDP_DMA_DATA_CREDIT_1_VC7_DMA_DATA_CREDIT_MASK        0xff000000

#define SMN_IOAGR_IOHUB0NBIO0_L1_SDP_DMA_DATA_CREDIT_1_ADDRESS    0x15300120UL

/***********************************************************
* Register Name : L1_SDP_DMA_RDRSP_CREDIT
* Register Description :
* Register used to control the credit assignment to VC for dma read response.
* Visibility : 0x2
************************************************************/
// Bitfield Description : Credits on VC4 for dma read response
#define L1_SDP_DMA_RDRSP_CREDIT_VC4_DMA_RDRSP_CREDIT_OFFSET      16
#define L1_SDP_DMA_RDRSP_CREDIT_VC4_DMA_RDRSP_CREDIT_MASK        0xf0000

#define SMN_IOAGR_IOHUB0NBIO0_L1_SDP_DMA_RDRSP_CREDIT_ADDRESS    0x15300124UL

/***********************************************************
* Register Name : L1_SDP_DMA_REQ_CREDIT_1
* Register Description :
* Register used to control the credit assignment to VC for dma req.
* Visibility : 0x2
************************************************************/
// Bitfield Description : Credits on VC7 for dma requests
#define L1_SDP_DMA_REQ_CREDIT_1_VC7_DMA_REQ_CREDIT_OFFSET      24
#define L1_SDP_DMA_REQ_CREDIT_1_VC7_DMA_REQ_CREDIT_MASK        0xff000000

#define SMN_IOAGR_IOHUB0NBIO0_L1_SDP_DMA_REQ_CREDIT_1_ADDRESS    0x15300118UL

/***********************************************************
* Register Name : L1_SDP_DMA_WRRSP_CREDIT
* Register Description :
* Register used to control the credit assignment to VC for dma write response.
* Visibility : 0x2
************************************************************/
// Bitfield Description : Credits on VC4 for dma write response
#define L1_SDP_DMA_WRRSP_CREDIT_VC4_DMA_WRRSP_CREDIT_OFFSET      16
#define L1_SDP_DMA_WRRSP_CREDIT_VC4_DMA_WRRSP_CREDIT_MASK        0xf0000
// Bitfield Description : Credits on VC6 for dma write response
#define L1_SDP_DMA_WRRSP_CREDIT_VC6_DMA_WRRSP_CREDIT_OFFSET      24
#define L1_SDP_DMA_WRRSP_CREDIT_VC6_DMA_WRRSP_CREDIT_MASK        0xf000000
#define SMN_IOAGR_IOHUB0NBIO0_L1_SDP_DMA_WRRSP_CREDIT_ADDRESS    0x15300128UL

/***********************************************************
* Register Name : L1_SDP_MAXCRED_0
* Register Description :
* Register used to set the max credits on the L1 SDP interfaces.
* Visibility : 0x2
************************************************************/
// Bitfield Description : Programing to 1 enables IOMMU credit count blocks to dynamicly change number of reserved
//credits per VC without presense of traffic.
#define L1_SDP_MAXCRED_0_L1_DYNAMIC_CRED_RELOCATION_EN_OFFSET      31
#define L1_SDP_MAXCRED_0_L1_DYNAMIC_CRED_RELOCATION_EN_MASK        0x80000000

#define SMN_IOAGR_IOHUB0NBIO0_L1_SDP_MAXCRED_0_ADDRESS    0x1530010cUL
