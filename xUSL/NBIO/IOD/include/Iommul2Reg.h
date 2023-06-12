/**
 *  @file Iommul2Reg.h
 *  @brief This file contains Iommul2 Register definitions
 */

/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

/***********************************************************
* Register Name : IOMMU_ADAPTER_ID_W
* Register Description :
*
* Visibility : 0x2
************************************************************/
#define SMN_IOHUB0NBIO0_IOMMU_ADAPTER_ID_W_ADDRESS    0x13f00078UL

/***********************************************************
* Register Name : IOMMU_CAP_BASE_LO
* Register Description :
*
* Visibility : 0x2
************************************************************/
#define PCICFG_NBIO0_IOHUB0_IOMMU_CAP_BASE_LO_OFFSET       0x44

/***********************************************************
* Register Name : IOMMU_CAP_MISC
* Register Description :
*
* Visibility : 0x2
************************************************************/
// Bitfield Description : 1=The HyperTransport? Address Translation address range for ATS responses is reserved and
//cannot be translated by the IOMMU. //0=The Address Translation address range can be translated by the IOMMU.
#define IOMMU_CAP_MISC_IOMMU_HT_ATS_RESV_OFFSET      22
#define IOMMU_CAP_MISC_IOMMU_HT_ATS_RESV_MASK        0x400000
#define SMN_IOHUB0NBIO0_IOMMU_CAP_MISC_ADDRESS    0x13f00050UL

/***********************************************************
* Register Name : IOMMU_CONTROL_W
* Register Description :
*
* Visibility : 0x2
************************************************************/
// Bitfield Description : Sets the INTERRUPT_PIN register in the configuration header. //0 = No legacy interrupts
//supported. //1 = INTA //2 = INTB //3 = INTC //4 = INTD
#define IOMMU_CONTROL_W_INTERRUPT_PIN_W_OFFSET      0
#define IOMMU_CONTROL_W_INTERRUPT_PIN_W_MASK        0x7
#define SMN_IOHUB0NBIO0_IOMMU_CONTROL_W_ADDRESS    0x13f0007cUL

/***********************************************************
* Register Name : IOMMU_MMIO_CONTROL0_W
* Register Description :
*
* Visibility : 0x2
************************************************************/
// Bitfield Description : 0x0=Hardware does not support AVIC in conjunction with SNP. //0x1 = Hardware supports AVIC and
//SNP enabled in conjunction.
#define IOMMU_MMIO_CONTROL0_W_SNPAVICSup_W_OFFSET      18
#define IOMMU_MMIO_CONTROL0_W_SNPAVICSup_W_MASK        0x1c0000
// Bitfield Description : Sets the GAM_SUP value in IOMMU Extended feature register.
#define IOMMU_MMIO_CONTROL0_W_GAM_SUP_W_OFFSET      21
#define IOMMU_MMIO_CONTROL0_W_GAM_SUP_W_MASK        0xe00000

typedef union {
  struct {
    uint32_t                            PREF_SUP_W:1;
    uint32_t                            PPR_SUP_W:1;
    uint32_t                            XT_SUP_W:1;
    uint32_t                            NX_SUP_W:1;
    uint32_t                            GT_SUP_W:1;
    uint32_t                            sATS_SUP_W:1;
    uint32_t                            IA_SUP_W:1;
    uint32_t                            GA_SUP_W:1;
    uint32_t                            HE_SUP_W:1;
    uint32_t                            PC_SUP_W:1;
    uint32_t                            HATS_W:2;
    uint32_t                            US_SUP_W:1;
    uint32_t                            GATS_W:2;
    uint32_t                            PageMigration_SUP_W:1;
    uint32_t                            GCR3TRPModeSup_W:1;
    uint32_t                            GstBufferTRPModeSup_W:1;
    uint32_t                            SNPAVICSup_W:3;
    uint32_t                            GAM_SUP_W:3;
    uint32_t                            PPRF_W:2;
    uint32_t                            CXLIO_SUP_W:1;
    uint32_t                            Reserved_27_27:1;
    uint32_t                            EVENTF_W:2;
    uint32_t                            GLX_SUP_W:2;
  } Field;
  uint32_t Value;
} SIL_RESERVED_3;

#define PCICFG_NBIO0_IOHUB0_IOMMU_MMIO_CONTROL0_W_OFFSET       0x80

#define SMN_IOMMU_MMIO_CONTROL0_W_ADDRESS    0x13f00080UL
#define SMN_IOHUB0NBIO0_IOMMU_MMIO_CONTROL0_W_ADDRESS    0x13f00080UL

/***********************************************************
* Register Name : IOMMU_MMIO_CONTROL1_W
* Register Description :
*
* Visibility : 0x2
************************************************************/
// Bitfield Description : 1 = Hardware supports invalidate IOTLB type.
#define IOMMU_MMIO_CONTROL1_W_InvIotlbTypeSup_W_OFFSET      19
#define IOMMU_MMIO_CONTROL1_W_InvIotlbTypeSup_W_MASK        0x80000
// Bitfield Description : 1 = Hardware supports SNP.
#define IOMMU_MMIO_CONTROL1_W_SNP_SUP_W_OFFSET      31
#define IOMMU_MMIO_CONTROL1_W_SNP_SUP_W_MASK        0x80000000

#define SMN_IOHUB0NBIO0_IOMMU_MMIO_CONTROL1_W_ADDRESS    0x13f00084UL

/***********************************************************
* Register Name : L2A_UPDATE_FILTER_CNTL
* Register Description :
*
* Visibility : 0x2
************************************************************/
// Bitfield Description : 1=Disable duplicate update filtering; //0=Enable the dropping of updates that are already in
//the l2a_update_filter or in the destination l2a cache.
#define L2A_UPDATE_FILTER_CNTL_L2a_Update_Filter_Bypass_OFFSET      0
#define L2A_UPDATE_FILTER_CNTL_L2a_Update_Filter_Bypass_MASK        0x1
#define SMN_IOHUB0NBIO0_L2A_UPDATE_FILTER_CNTL_ADDRESS    0x15700088UL

/***********************************************************
* Register Name : L2B_UPDATE_FILTER_CNTL
* Register Description :
*
* Visibility : 0x2
************************************************************/
// Bitfield Description : 1 = Disable duplicate update filtering; //0 = Enable the dropping of updates that are already
//in the l2b_update_filter or in the PDC.
#define L2B_UPDATE_FILTER_CNTL_L2b_Update_Filter_Bypass_OFFSET      0
#define L2B_UPDATE_FILTER_CNTL_L2b_Update_Filter_Bypass_MASK        0x1
#define SMN_IOHUB0NBIO0_L2B_UPDATE_FILTER_CNTL_ADDRESS    0x13f0114cUL

/***********************************************************
* Register Name : L2_CONTROL_5
* Register Description :
*
* Visibility : 0x2
************************************************************/
// Bitfield Description : Each bit controls which level of Guest Page Directory SPA pointer is allowed to be cached in
//PTC. Bit 3: SPA pointer to PTE. Bit 6: SPA pointer to PDE. All other bits are reserved
#define L2_CONTROL_5_GST_partial_ptc_cntrl_OFFSET      12
#define L2_CONTROL_5_GST_partial_ptc_cntrl_MASK        0x7f000
#define SMN_IOHUB0NBIO0_L2_CONTROL_5_ADDRESS    0x13f01130UL

/***********************************************************
* Register Name : L2_CP_CONTROL
* Register Description :
*
* Visibility : 0x2
************************************************************/
// Bitfield Description : 1=Command processor flushes out old requests on completion wait. //0=No flush is performed on
//completion wait.
#define L2_CP_CONTROL_CPFlushOnWait_OFFSET      1
#define L2_CP_CONTROL_CPFlushOnWait_MASK        0x2
// Bitfield Description : 1=Command processor flushes out old requests on every invalidation command. //0=No flush is
//performed during invalidations.
#define L2_CP_CONTROL_CPFlushOnInv_OFFSET      2
#define L2_CP_CONTROL_CPFlushOnInv_MASK        0x4
#define SMN_IOHUB0NBIO0_L2_CP_CONTROL_ADDRESS    0x13f01158UL

/***********************************************************
* Register Name : L2_ERR_RULE_CONTROL_0
* Register Description :
*
* Visibility : 0x2
************************************************************/
// Bitfield Description : This register is write-once. Setting this register bit locks the error detection rule set in
//ERRRuleDisable0/1/2.
#define L2_ERR_RULE_CONTROL_0_ERRRuleLock0_OFFSET      0
#define L2_ERR_RULE_CONTROL_0_ERRRuleLock0_MASK        0x1
#define SMN_IOHUB0NBIO0_L2_ERR_RULE_CONTROL_0_ADDRESS    0x13f01200UL

/***********************************************************
* Register Name : L2_ERR_RULE_CONTROL_3
* Register Description :
*
* Visibility : 0x2
************************************************************/
// Bitfield Description : This register is write-once. Setting this register bit locks the error detection rule set in
//ERRRuleDisable3/4/5.
#define L2_ERR_RULE_CONTROL_3_ERRRuleLock1_OFFSET      0
#define L2_ERR_RULE_CONTROL_3_ERRRuleLock1_MASK        0x1
#define SMN_IOHUB0NBIO0_L2_ERR_RULE_CONTROL_3_ADDRESS    0x157000c0UL

/***********************************************************
* Register Name : L2_L2A_CK_GATE_CONTROL
* Register Description :
*
* Visibility : 0x2
************************************************************/

// Bitfield Description : Disable the gating of the l2a register clock branch.
#define L2_L2A_CK_GATE_CONTROL_CKGateL2ARegsDisable_OFFSET      0
#define L2_L2A_CK_GATE_CONTROL_CKGateL2ARegsDisable_MASK        0x1
// Bitfield Description : Disable the gating of the l2a dynamic clock branch.
#define L2_L2A_CK_GATE_CONTROL_CKGateL2ADynamicDisable_OFFSET      1
#define L2_L2A_CK_GATE_CONTROL_CKGateL2ADynamicDisable_MASK        0x2
// Bitfield Description : Disable the gating of the l2a upper cache ways.
#define L2_L2A_CK_GATE_CONTROL_CKGateL2ACacheDisable_OFFSET      2
#define L2_L2A_CK_GATE_CONTROL_CKGateL2ACacheDisable_MASK        0x4
#define SMN_IOHUB0NBIO0_L2_L2A_CK_GATE_CONTROL_ADDRESS    0x157000ccUL

/***********************************************************
* Register Name : L2_L2A_PGSIZE_CONTROL
* Register Description :
*
* Visibility : 0x2
************************************************************/
// Bitfield Description : Controls the TLB search for different (final and partial) guest page sizes. //Bit 0 = 4k final
//TLB look up enabled(cannot be disabled) //Bit 3 = 2MB partial and final TLB look up enabled //Bit 6  = 1GB partial and
//final TLB look up enabled. All other bits are reserved. The register is used when L2AREG_PTCSCAN_MODE=000b
#define L2_L2A_PGSIZE_CONTROL_L2AREG_GST_PGSIZE_OFFSET      0
#define L2_L2A_PGSIZE_CONTROL_L2AREG_GST_PGSIZE_MASK        0x7f
// Bitfield Description : Controls the TLB search for different (final and partial) host page sizes. //Bit 0 = 4k final
//TLB look up enabled(cannot be disabled) //Bit 3 = 2MB partial and final TLB look up enabled //Bit 6 = 1GB partial and
//final TLB look up enabled //All other bits are reserved.The register is used when L2AREG_PTCSCAN_MODE=000b
#define L2_L2A_PGSIZE_CONTROL_L2AREG_HOST_PGSIZE_OFFSET      8
#define L2_L2A_PGSIZE_CONTROL_L2AREG_HOST_PGSIZE_MASK        0x7f00
#define SMN_IOHUB0NBIO0_L2_L2A_PGSIZE_CONTROL_ADDRESS    0x157000d0UL

/***********************************************************
* Register Name : L2_L2B_CK_GATE_CONTROL
* Register Description :
*
* Visibility : 0x2
************************************************************/
// Bitfield Description : Disable the gating of the l2b register clock branch.
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BRegsDisable_OFFSET      0
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BRegsDisable_MASK        0x1
// Bitfield Description : Disable the gating of the l2b dynamic clock branch.
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BDynamicDisable_OFFSET      1
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BDynamicDisable_MASK        0x2
// Bitfield Description : Disable the gating of the l2b miscellaneous clock branch.
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BMiscDisable_OFFSET      2
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BMiscDisable_MASK        0x4
// Bitfield Description : Disable the gating of the l2b upper cache ways.
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BCacheDisable_OFFSET      3
#define L2_L2B_CK_GATE_CONTROL_CKGateL2BCacheDisable_MASK        0x8
#define SMN_IOHUB0NBIO0_L2_L2B_CK_GATE_CONTROL_ADDRESS    0x13f01240UL

/***********************************************************
* Register Name : L2_L2B_PGSIZE_CONTROL
* Register Description :
*
* Visibility : 0x2
************************************************************/
// Bitfield Description : Controls the allowed Guest PTE sizes stored in PTC cache. Following are the valid programming
//bit values for this register //bit 0 = Enable 4k (4k is always on..cannot be disabled) //bit 3 = Enable 2M final guest
//page size caching //bit 6 = Enabled 1G final guest page size caching //If a given final page size is disabled, next
//enabled lowest page size will be cached. All other bits are reserved
#define L2_L2B_PGSIZE_CONTROL_L2BREG_GST_PGSIZE_OFFSET      0
#define L2_L2B_PGSIZE_CONTROL_L2BREG_GST_PGSIZE_MASK        0x7f
// Bitfield Description : Controls the allowed Host PTE sizes stored in PTC cache. Following are the valid programming
//bit values for this register //bit 0 = Enable 4k (4k is always on..cannot be disabled) //bit 3 = Enable 2M final host
//page size caching //bit 6 = Enabled 1G final host page size caching. //All other bits are reserved.
#define L2_L2B_PGSIZE_CONTROL_L2BREG_HOST_PGSIZE_OFFSET      8
#define L2_L2B_PGSIZE_CONTROL_L2BREG_HOST_PGSIZE_MASK        0x7f00
#define SMN_IOHUB0NBIO0_L2_L2B_PGSIZE_CONTROL_ADDRESS    0x13f01250UL

/***********************************************************
* Register Name : L2_MISC_CNTRL_3
* Register Description :
*
* Visibility : 0x2
************************************************************/
// Bitfield Description : Ignore Write permission checks for ATS requests.
#define L2_MISC_CNTRL_3_REG_ats_iw_OFFSET      0
#define L2_MISC_CNTRL_3_REG_ats_iw_MASK        0x1
// Bitfield Description : Value = 1 Mask il2_nw with L2PDE_L2TW_vaoffset[0].
#define L2_MISC_CNTRL_3_REG_mask_l2_nw_OFFSET      1
#define L2_MISC_CNTRL_3_REG_mask_l2_nw_MASK        0x2
// Bitfield Description : TW atomic filter enable.
#define L2_MISC_CNTRL_3_REG_atomic_filter_en_OFFSET      2
#define L2_MISC_CNTRL_3_REG_atomic_filter_en_MASK        0x4
#define SMN_IOHUB0NBIO0_L2_MISC_CNTRL_3_ADDRESS    0x13f0111cUL

/***********************************************************
* Register Name : L2_SB_LOCATION
* Register Description :  * Specifies which PCIE bridge is connected to the SB.  Register set to 32'b0 indicates the GNB
//is secondary and has no SB connected.
* Visibility : 0x2
************************************************************/
#define SMN_IOHUB0NBIO0_L2_SB_LOCATION_ADDRESS    0x13f0112cUL

/***********************************************************
* Register Name : L2_TW_CONTROL
* Register Description :
*
* Visibility : 0x2
************************************************************/
// Bitfield Description : 1=Table-walker always genereates coherent requests. The DTE SD bit is ignored when this bit is
//set to 1.
#define L2_TW_CONTROL_TWForceCoherent_OFFSET      6
#define L2_TW_CONTROL_TWForceCoherent_MASK        0x40
// Bitfield Description : Enable HW host prefetching in the table-walker.
#define L2_TW_CONTROL_TWPrefetchEn_OFFSET      8
#define L2_TW_CONTROL_TWPrefetchEn_MASK        0x100
// Bitfield Description : Selects the number of pages to prefetch.
#define L2_TW_CONTROL_TWPrefetchRange_OFFSET      12
#define L2_TW_CONTROL_TWPrefetchRange_MASK        0x7000
// Bitfield Description : 1-Enables storing Nested PTEs (2MB and larger) in PDC. 0-Disable storing Nested PTEs in PDC
//(default)
#define L2_TW_CONTROL_TWCacheNestedPTE_OFFSET      25
#define L2_TW_CONTROL_TWCacheNestedPTE_MASK        0x2000000
#define SMN_IOHUB0NBIO0_L2_TW_CONTROL_ADDRESS    0x13f01150UL
