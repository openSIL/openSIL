/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 *  @file Iommul2Reg.h
 *  @brief This file contains Iommul2 Register definitions
 */

#pragma once

#define SIL_RESERVED_0791                     0x44

#define SIL_RESERVED_1763                     0xd0
#define SIL_RESERVED_1764                     0xd4

#define SIL_RESERVED_1765                     0xd8
#define SIL_RESERVED_1766                     0xdc

#define SIL_REG_ADDR_157000c4                 0x157000c4UL
#define SIL_REG_ADDR_157000c8                 0x157000c8UL
#define SIL_REG_ADDR_1570011c                 0x1570011cUL

#define IOMMU_CAP_MISC_IOMMU_HT_ATS_RESV_OFFSET      22
#define IOMMU_CAP_MISC_IOMMU_HT_ATS_RESV_MASK        0x400000
#define SIL_RSVD_ADDR_13F00050                                    0x13f00050UL

#define IOMMU_CONTROL_W_INTERRUPT_PIN_W_OFFSET      0
#define IOMMU_CONTROL_W_INTERRUPT_PIN_W_MASK        0x7
#define SIL_RSVD_ADDR_13F0007C                 0x13f0007cUL

#define SIL_RESERVED_0779                5
#define SIL_RESERVED_0778                  0x20
#define SIL_RESERVED_0777     17
#define SIL_RESERVED_0776       0x20000
#define IOMMU_MMIO_CONTROL0_W_GAM_SUP_W_OFFSET      21
#define IOMMU_MMIO_CONTROL0_W_GAM_SUP_W_MASK        0xe00000

typedef union {
  struct {
    uint32_t                            field_bit_0:1;
    uint32_t                            field_bit_1:1;
    uint32_t                            field_bit_2:1;
    uint32_t                            field_bit_3:1;
    uint32_t                            field_bit_4:1;
    uint32_t                            field_bit_5:1;
    uint32_t                            field_bit_6:1;
    uint32_t                            field_bit_7:1;
    uint32_t                            field_bit_8:1;
    uint32_t                            field_bit_9:1;
    uint32_t                            field_bits_10_to_11:2;
    uint32_t                            field_bit_12:1;
    uint32_t                            field_bits_13_to_14:2;
    uint32_t                            field_bit_15:1;
    uint32_t                            field_bit_16:1;
    uint32_t                            field_bit_17:1;
    uint32_t                            field_bits_18_to_20:3;
    uint32_t                            field_bits_21_to_23:3;
    uint32_t                            field_bits_24_to_25:2;
    uint32_t                            field_bit_26:1;
    uint32_t                            field_bit_27:1;
    uint32_t                            field_bits_28_to_29:2;
    uint32_t                            field_bits_30_to_31:2;
  } Field;
  uint32_t Value;
} SIL_RESERVED_UNION_0022;

#define SIL_RESERVED_0792      0x80

#define SMI_FILTER_REGISTER_0_0_SmiDID_0_OFFSET                0
#define SMI_FILTER_REGISTER_0_0_SmiDID_0_MASK                  0xffff

#define SMI_FILTER_REGISTER_0_0_SmiDV_0_OFFSET                 16
#define SMI_FILTER_REGISTER_0_0_SmiDV_0_MASK                   0x10000

#define SMI_FILTER_REGISTER_0_0_SmiFLock_0_OFFSET              17
#define SMI_FILTER_REGISTER_0_0_SmiFLock_0_MASK                0x20000

#define SMI_FILTER_REGISTER_0_0_Reserved_31_18_OFFSET          18
#define SMI_FILTER_REGISTER_0_0_Reserved_31_18_MASK            0xfffc0000

#define SMN_SMI_FILTER_REGISTER_0_0_ADDRESS                           0x2400060UL

typedef union {
  struct {
    uint32_t     PREF_SUP:1;
    uint32_t      PPR_SUP:1;
    uint32_t       XT_SUP:1;
    uint32_t       NX_SUP:1;
    uint32_t       GT_SUP:1;
    uint32_t    GAPPI_SUP:1;
    uint32_t       IA_SUP:1;
    uint32_t       GA_SUP:1;
    uint32_t       HE_SUP:1;
    uint32_t       PC_SUP:1;
    uint32_t         HATS:2;
    uint32_t         GATS:2;
    uint32_t      GLX_SUP:2;
    uint32_t     SMIF_SUP:2;
    uint32_t      SMIF_RC:3;
    uint32_t      GAM_SUP:3;
    uint32_t         PPRF:2;
    uint32_t          GAF:2;
    uint32_t       EVENTF:2;
    uint32_t  DVM_ERR_SUP:1;
    uint32_t     sATS_SUP:1;
  } Field;
  uint32_t Value;
} IOMMU_MMIO_EFR_0_STRUCT;

#define SMN_IOMMU_MMIO_EFR_0_ADDRESS                     0x2400030UL
#define SMN_IOMMU_MMIO_CNTRL_0_ADDRESS                   0x2400018UL
#define IOMMU_MMIO_CNTRL_0_SMIF_EN_OFFSET                22
#define IOMMU_MMIO_CNTRL_0_SMIF_EN_MASK                  0x400000
#define IOMMU_MMIO_CNTRL_0_SMIF_LOG_EN_OFFSET            24
#define IOMMU_MMIO_CNTRL_0_SMIF_LOG_EN_MASK              0x1000000

#define SMN_IOMMU_MMIO_CONTROL0_W_ADDRESS                0x13f00080UL
#define SMN_IOHUB0NBIO0_IOMMU_MMIO_CONTROL0_W_ADDRESS    0x13f00080UL

#define IOMMU_MMIO_CONTROL1_W_InvIotlbTypeSup_W_OFFSET      19
#define IOMMU_MMIO_CONTROL1_W_InvIotlbTypeSup_W_MASK        0x80000
#define SIL_RESERVED_0785               28
#define SIL_RESERVED_0784                 0x10000000
#define SIL_RESERVED_0783            29
#define SIL_RESERVED_0782              0x20000000
#define SIL_RESERVED_0781         30
#define SIL_RESERVED_0780           0x40000000
#define IOMMU_MMIO_CONTROL1_W_SNP_SUP_W_OFFSET      31
#define IOMMU_MMIO_CONTROL1_W_SNP_SUP_W_MASK        0x80000000

#define SIL_RSVD_ADDR_13F00084                             0x13f00084UL
#define SIL_RSVD_ADDR_SMN_IOMMU_MMIO_CONTROL1_W_ADDRESS                 SIL_RSVD_ADDR_13F00084

#define SIL_RESERVED_0787             2
#define SIL_RESERVED_0786               0x4

#define SIL_RESERVED_1769             5
#define SIL_RESERVED_1770               0x20

#define SIL_RSVD_ADDR_13F000E8                      0x13f000e8UL

#define L2A_UPDATE_FILTER_CNTL_L2a_Update_Filter_Bypass_OFFSET      0
#define L2A_UPDATE_FILTER_CNTL_L2a_Update_Filter_Bypass_MASK        0x1
#define SIL_RSVD_ADDR_15700088                          0x15700088UL

#define L2B_UPDATE_FILTER_CNTL_L2b_Update_Filter_Bypass_OFFSET      0
#define L2B_UPDATE_FILTER_CNTL_L2b_Update_Filter_Bypass_MASK        0x1
#define SIL_RSVD_ADDR_13F0114C                          0x13f0114cUL

#define L2_CONTROL_5_GST_partial_ptc_cntrl_OFFSET      12
#define L2_CONTROL_5_GST_partial_ptc_cntrl_MASK        0x7f000
#define SIL_RSVD_ADDR_13F01130                0x13f01130UL

#define L2_CP_CONTROL_CPFlushOnWait_OFFSET      1
#define L2_CP_CONTROL_CPFlushOnWait_MASK        0x2
#define L2_CP_CONTROL_CPFlushOnInv_OFFSET      2
#define L2_CP_CONTROL_CPFlushOnInv_MASK        0x4
#define SIL_RSVD_ADDR_13F01158                0x13f01158UL

#define L2_ERR_RULE_CONTROL_0_ERRRuleLock0_OFFSET      0
#define L2_ERR_RULE_CONTROL_0_ERRRuleLock0_MASK        0x1

#define SIL_RSVD_ADDR_13F01200                0x13f01200UL

#define L2_ERR_RULE_CONTROL_3_ERRRuleLock1_OFFSET      0
#define L2_ERR_RULE_CONTROL_3_ERRRuleLock1_MASK        0x1
#define SIL_RSVD_ADDR_157000C0                0x157000c0UL

#define L2_L2A_CK_GATE_CONTROL_CKGateL2ARegsDisable_OFFSET      0
#define L2_L2A_CK_GATE_CONTROL_CKGateL2ARegsDisable_MASK        0x1
// Bitfield Description : Disable the gating of the l2a dynamic clock branch.
#define L2_L2A_CK_GATE_CONTROL_CKGateL2ADynamicDisable_OFFSET      1
#define L2_L2A_CK_GATE_CONTROL_CKGateL2ADynamicDisable_MASK        0x2
// Bitfield Description : Disable the gating of the l2a upper cache ways.
#define L2_L2A_CK_GATE_CONTROL_CKGateL2ACacheDisable_OFFSET      2
#define L2_L2A_CK_GATE_CONTROL_CKGateL2ACacheDisable_MASK        0x4
#define SIL_RSVD_ADDR_157000CC                0x157000ccUL

#define L2_L2A_PGSIZE_CONTROL_L2AREG_GST_PGSIZE_OFFSET      0
#define L2_L2A_PGSIZE_CONTROL_L2AREG_GST_PGSIZE_MASK        0x7f
#define L2_L2A_PGSIZE_CONTROL_L2AREG_HOST_PGSIZE_OFFSET      8
#define L2_L2A_PGSIZE_CONTROL_L2AREG_HOST_PGSIZE_MASK        0x7f00
#define SIL_RSVD_ADDR_157000D0                0x157000d0UL

#define L2_L2B_CK_GATE_CONTROL_CKGateL2BRegsDisable_OFFSET      0
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BRegsDisable_MASK        0x1
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BDynamicDisable_OFFSET      1
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BDynamicDisable_MASK        0x2
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BMiscDisable_OFFSET      2
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BMiscDisable_MASK        0x4
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BCacheDisable_OFFSET      3
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BCacheDisable_MASK        0x8
#define SIL_RSVD_ADDR_13F01240                0x13f01240UL

#define L2_L2B_PGSIZE_CONTROL_L2BREG_GST_PGSIZE_OFFSET      0
#define L2_L2B_PGSIZE_CONTROL_L2BREG_GST_PGSIZE_MASK        0x7f
#define L2_L2B_PGSIZE_CONTROL_L2BREG_HOST_PGSIZE_OFFSET      8
#define L2_L2B_PGSIZE_CONTROL_L2BREG_HOST_PGSIZE_MASK        0x7f00
#define SIL_RSVD_ADDR_13F01250                0x13f01250UL

#define L2_MISC_CNTRL_3_REG_ats_iw_OFFSET      0
#define L2_MISC_CNTRL_3_REG_ats_iw_MASK        0x1
#define L2_MISC_CNTRL_3_REG_mask_l2_nw_OFFSET      1
#define L2_MISC_CNTRL_3_REG_mask_l2_nw_MASK        0x2
#define L2_MISC_CNTRL_3_REG_atomic_filter_en_OFFSET      2
#define L2_MISC_CNTRL_3_REG_atomic_filter_en_MASK        0x4
#define SIL_RSVD_ADDR_13F0111C                0x13f0111cUL

#define SIL_RSVD_ADDR_13F0112C                0x13f0112cUL

#define L2_TW_CONTROL_TWForceCoherent_OFFSET      6
#define L2_TW_CONTROL_TWForceCoherent_MASK        0x40
#define L2_TW_CONTROL_TWPrefetchEn_OFFSET      8
#define L2_TW_CONTROL_TWPrefetchEn_MASK        0x100
#define L2_TW_CONTROL_TWPrefetchRange_OFFSET      12
#define L2_TW_CONTROL_TWPrefetchRange_MASK        0x7000
#define L2_TW_CONTROL_TWCacheNestedPTE_OFFSET      25
#define L2_TW_CONTROL_TWCacheNestedPTE_MASK        0x2000000
#define SIL_RSVD_ADDR_13F01150                0x13f01150UL

#define SIL_RESERVED_0798               31
#define SIL_RESERVED_0797                 0x80000000
#define SIL_RSVD_ADDR_15704400                     0x15704400UL

#define SIL_RESERVED_0794      21
#define SIL_RESERVED_0793        0xe00000
#define SIL_RESERVED_0801           0x15704330UL

#define SIL_RESERVED_0796     2
#define SIL_RESERVED_0795       0x4


#define SIL_RSVD_ADDR_15704398              0x15704398UL

//translated memory requests.
#define SIL_RESERVED_0800               31
#define SIL_RESERVED_0799                 0x80000000
#define SIL_RSVD_ADDR_13F04400                     0x13f04400UL

#define SIL_RESERVED_0788              0
#define BITS_0_TO_27_MASK_0003                0xfffffff

#define SIL_RSVD_ADDR_15700154                        0x15700154UL

#define SIL_RESERVED_0790              0
#define SIL_RESERVED_0789                0x1f

#define SIL_RSVD_ADDR_13F00308                        0x13f00308UL
