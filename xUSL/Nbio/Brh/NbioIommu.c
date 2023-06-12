/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file NbioIommu.c
 * @brief OpenSIL NBIO Iommu initialization module.
 *
 */

#include <Nbio/NbioIp2Ip.h>
#include <CCX/Common/CcxApic.h>
#include <CCX/Common/CcxReg.h>
#include <RcMgr/RcMgrIp2Ip.h>
#include <Nbio/Brh/NbioSmnTable.h>
#include <Nbio/Brh/include/IohcReg.h>
#include <Nbio/Brh/include/Iommul2Reg.h>
#include <Nbio/Brh/include/NbioBaseReg.h>
#include <Nbio/Brh/include/Iommul1Reg.h>
#include <Nbio/Brh/include/IommuMmioReg.h>
#include "GnbRegistersBrh.h"

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           T A B L E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

//
//
//
// --------------------------------------------------
// 15.3.1.2.1 : This table initializes IOMMU
// --------------------------------------------------

// Disable INTx in IOMMU
// IOMMU::L2::IOMMU_CAP_MISC[IOMMU_HT_ATS_RESV] = 0h
SMN_TABLE NbioIommuInitTbl [] = {
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_13F0007C,
    IOMMU_CONTROL_W_INTERRUPT_PIN_W_MASK,
    0x0 << IOMMU_CONTROL_W_INTERRUPT_PIN_W_OFFSET
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_13F00050,
    IOMMU_CAP_MISC_IOMMU_HT_ATS_RESV_MASK,
    (0 << IOMMU_CAP_MISC_IOMMU_HT_ATS_RESV_OFFSET)
    ),
  SMN_ENTRY_TERMINATE
};
// End of NBIO_IOMMU_INIT_TBL

// --------------------------------------------------
// 15.3.1.2.2       IOMMU L1 Initialization
// --------------------------------------------------
// Program these values on every IOMMU
//
SMN_TABLE NbioIommuL1InitTbl [] = {
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_153000C8,
    L1_CNTRL_4_ATS_multiple_resp_en_MASK |
    L1_CNTRL_4_Timeout_pulse_ext_En_MASK,
    (0x1 << L1_CNTRL_4_ATS_multiple_resp_en_OFFSET) |
    (0x1 << L1_CNTRL_4_Timeout_pulse_ext_En_OFFSET)
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_147000C8,
    L1_CNTRL_4_ATS_multiple_resp_en_MASK |
    L1_CNTRL_4_Timeout_pulse_ext_En_MASK,
    (0x1 << L1_CNTRL_4_ATS_multiple_resp_en_OFFSET) |
    (0x1 << L1_CNTRL_4_Timeout_pulse_ext_En_OFFSET)
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_14B000C8,
    L1_CNTRL_4_ATS_multiple_resp_en_MASK |
    L1_CNTRL_4_Timeout_pulse_ext_En_MASK,
    (0x1 << L1_CNTRL_4_ATS_multiple_resp_en_OFFSET) |
    (0x1 << L1_CNTRL_4_Timeout_pulse_ext_En_OFFSET)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_0775,
    SIL_RESERVED_1242 |
    SIL_RESERVED_1244 |
    SIL_RESERVED_1248,
    (0x0 << SIL_RESERVED_1243) |
    (0x1 << SIL_RESERVED_1245) |
    (0x1 << SIL_RESERVED_1249)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1495,
    SIL_RESERVED_1242 |
    SIL_RESERVED_1244 |
    SIL_RESERVED_1248,
    (0x0 << SIL_RESERVED_1243) |
    (0x1 << SIL_RESERVED_1245) |
    (0x1 << SIL_RESERVED_1249)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1496,
    SIL_RESERVED_1242 |
    SIL_RESERVED_1244 |
    SIL_RESERVED_1248,
    (0x0 << SIL_RESERVED_1243) |
    (0x1 << SIL_RESERVED_1245) |
    (0x1 << SIL_RESERVED_1249)
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_1530009C,
    L1_FEATURE_CNTRL_EXE_lock_bit_MASK |
    L1_FEATURE_CNTRL_PMR_lock_bit_MASK,
    1 << L1_FEATURE_CNTRL_EXE_lock_bit_OFFSET |
      1 << L1_FEATURE_CNTRL_PMR_lock_bit_OFFSET
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_1470009C,
    L1_FEATURE_CNTRL_EXE_lock_bit_MASK |
    L1_FEATURE_CNTRL_PMR_lock_bit_MASK,
    1 << L1_FEATURE_CNTRL_EXE_lock_bit_OFFSET |
      1 << L1_FEATURE_CNTRL_PMR_lock_bit_OFFSET
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_14B0009C,
    L1_FEATURE_CNTRL_EXE_lock_bit_MASK |
    L1_FEATURE_CNTRL_PMR_lock_bit_MASK,
    1 << L1_FEATURE_CNTRL_EXE_lock_bit_OFFSET |
      1 << L1_FEATURE_CNTRL_PMR_lock_bit_OFFSET
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_15300038,
    L1_CNTRL_2_CPD_RESP_MODE_MASK |
    L1_CNTRL_2_L1NonConsumedDataErrorSignalEn_MASK |
    L1_CNTRL_2_L1ConsumedDataErrorSignalEn_MASK,
    1 << L1_CNTRL_2_CPD_RESP_MODE_OFFSET |
      1 << L1_CNTRL_2_L1NonConsumedDataErrorSignalEn_OFFSET |
      1 << L1_CNTRL_2_L1ConsumedDataErrorSignalEn_OFFSET
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_14700038,
    L1_CNTRL_2_CPD_RESP_MODE_MASK |
    L1_CNTRL_2_L1NonConsumedDataErrorSignalEn_MASK |
    L1_CNTRL_2_L1ConsumedDataErrorSignalEn_MASK,
    1 << L1_CNTRL_2_CPD_RESP_MODE_OFFSET |
      1 << L1_CNTRL_2_L1NonConsumedDataErrorSignalEn_OFFSET |
      1 << L1_CNTRL_2_L1ConsumedDataErrorSignalEn_OFFSET
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_14B00038,
    L1_CNTRL_2_CPD_RESP_MODE_MASK |
    L1_CNTRL_2_L1NonConsumedDataErrorSignalEn_MASK |
    L1_CNTRL_2_L1ConsumedDataErrorSignalEn_MASK,
    1 << L1_CNTRL_2_CPD_RESP_MODE_OFFSET |
      1 << L1_CNTRL_2_L1NonConsumedDataErrorSignalEn_OFFSET |
      1 << L1_CNTRL_2_L1ConsumedDataErrorSignalEn_OFFSET
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_153000D4,
    (L1_SDP_CLKREQ_CNTRL_HW_PG_WAKEUP_EN_DMA_MASK |
    SIL_RESERVED_0765 |
    SIL_RESERVED_0763 |
    SIL_RESERVED_0767 |
    SIL_RESERVED_0769),
    ((1 << L1_SDP_CLKREQ_CNTRL_HW_PG_WAKEUP_EN_DMA_OFFSET) |
    (1 << SIL_RESERVED_0766) |
    (1 << SIL_RESERVED_0764) |
    (1 << SIL_RESERVED_0768) |
    (1 << SIL_RESERVED_0770))
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_147000D4,
    (L1_SDP_CLKREQ_CNTRL_HW_PG_WAKEUP_EN_DMA_MASK |
    SIL_RESERVED_0765 |
    SIL_RESERVED_0763 |
    SIL_RESERVED_0767 |
    SIL_RESERVED_0769),
    ((1 << L1_SDP_CLKREQ_CNTRL_HW_PG_WAKEUP_EN_DMA_OFFSET) |
    (1 << SIL_RESERVED_0766) |
    (1 << SIL_RESERVED_0764) |
    (1 << SIL_RESERVED_0768) |
    (1 << SIL_RESERVED_0770))
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_14B000D4,
    (L1_SDP_CLKREQ_CNTRL_HW_PG_WAKEUP_EN_DMA_MASK |
    SIL_RESERVED_0765 |
    SIL_RESERVED_0763 |
    SIL_RESERVED_0767 |
    SIL_RESERVED_0769),
    ((1 << L1_SDP_CLKREQ_CNTRL_HW_PG_WAKEUP_EN_DMA_OFFSET) |
    (1 << SIL_RESERVED_0766) |
    (1 << SIL_RESERVED_0764) |
    (1 << SIL_RESERVED_0768) |
    (1 << SIL_RESERVED_0770))
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_1530010C,
    L1_SDP_MAXCRED_0_L1_DYNAMIC_CRED_RELOCATION_EN_MASK,
    (1 << L1_SDP_MAXCRED_0_L1_DYNAMIC_CRED_RELOCATION_EN_OFFSET)
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_1470010C,
    L1_SDP_MAXCRED_0_L1_DYNAMIC_CRED_RELOCATION_EN_MASK,
    (1 << L1_SDP_MAXCRED_0_L1_DYNAMIC_CRED_RELOCATION_EN_OFFSET)
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_14B0010C,
    L1_SDP_MAXCRED_0_L1_DYNAMIC_CRED_RELOCATION_EN_MASK,
    (1 << L1_SDP_MAXCRED_0_L1_DYNAMIC_CRED_RELOCATION_EN_OFFSET)
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_15300118,
    L1_SDP_DMA_REQ_CREDIT_1_VC7_DMA_REQ_CREDIT_MASK |
    SIL_RESERVED_0773,
    (1 << L1_SDP_DMA_REQ_CREDIT_1_VC7_DMA_REQ_CREDIT_OFFSET) |
    (3 << SIL_RESERVED_0774)
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_15300120,
    L1_SDP_DMA_DATA_CREDIT_1_VC7_DMA_DATA_CREDIT_MASK |
    SIL_RESERVED_0771,
    (1 << L1_SDP_DMA_DATA_CREDIT_1_VC7_DMA_DATA_CREDIT_OFFSET) |
    (3 << SIL_RESERVED_0772)
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_15300124,
    L1_SDP_DMA_RDRSP_CREDIT_VC4_DMA_RDRSP_CREDIT_MASK,
    (1 << L1_SDP_DMA_RDRSP_CREDIT_VC4_DMA_RDRSP_CREDIT_OFFSET)
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_15300128,
    L1_SDP_DMA_WRRSP_CREDIT_VC4_DMA_WRRSP_CREDIT_MASK |
    L1_SDP_DMA_WRRSP_CREDIT_VC6_DMA_WRRSP_CREDIT_MASK,
    (1 << L1_SDP_DMA_WRRSP_CREDIT_VC4_DMA_WRRSP_CREDIT_OFFSET) |
    (1 << L1_SDP_DMA_WRRSP_CREDIT_VC6_DMA_WRRSP_CREDIT_OFFSET)
    ),
  SMN_ENTRY_PROPERTY_RMW(SIL_RESERVED_1754,
    SIL_RSVD_ADDR_15300288,
    SIL_RESERVED_0759 |
    SIL_RESERVED_0761,
    1 << SIL_RESERVED_0760 |
      0xC << SIL_RESERVED_0762
    ),
  SMN_ENTRY_PROPERTY_RMW(SIL_RESERVED_1754,
    SIL_RSVD_ADDR_14700288,
    SIL_RESERVED_0759 |
    SIL_RESERVED_0761,
    1 << SIL_RESERVED_0760 |
      0xC << SIL_RESERVED_0762
    ),
  SMN_ENTRY_PROPERTY_RMW(SIL_RESERVED_1754,
    SIL_RSVD_ADDR_14B00288,
    SIL_RESERVED_0759 |
    SIL_RESERVED_0761,
    1 << SIL_RESERVED_0760 |
      0xC << SIL_RESERVED_0762
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_1530010C,
    L1_SDP_MAXCRED_0_L1_DYNAMIC_CRED_RELOCATION_EN_MASK,
    (0 << L1_SDP_MAXCRED_0_L1_DYNAMIC_CRED_RELOCATION_EN_OFFSET)
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_1470010C,
    L1_SDP_MAXCRED_0_L1_DYNAMIC_CRED_RELOCATION_EN_MASK,
    (0 << L1_SDP_MAXCRED_0_L1_DYNAMIC_CRED_RELOCATION_EN_OFFSET)
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_14B0010C,
    L1_SDP_MAXCRED_0_L1_DYNAMIC_CRED_RELOCATION_EN_MASK,
    (0 << L1_SDP_MAXCRED_0_L1_DYNAMIC_CRED_RELOCATION_EN_OFFSET)
    ),
  SMN_ENTRY_TERMINATE
};
// End of NBIO_IOMMU_L1_INIT_TBL

// --------------------------------------------------
// 15.3.1.2.3  : IOMMU L2 Initialization
// --------------------------------------------------
SMN_TABLE NbioIommuL2InitTbl [] = {
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_13F01200,
    L2_ERR_RULE_CONTROL_0_ERRRuleLock0_MASK,
    0x1 << L2_ERR_RULE_CONTROL_0_ERRRuleLock0_OFFSET
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_157000C0,
    L2_ERR_RULE_CONTROL_3_ERRRuleLock1_MASK,
    0x1 << L2_ERR_RULE_CONTROL_3_ERRRuleLock1_OFFSET
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_157000D0,
    L2_L2A_PGSIZE_CONTROL_L2AREG_HOST_PGSIZE_MASK |
    L2_L2A_PGSIZE_CONTROL_L2AREG_GST_PGSIZE_MASK,
    (0x49 << L2_L2A_PGSIZE_CONTROL_L2AREG_HOST_PGSIZE_OFFSET) |
    (0x49 << L2_L2A_PGSIZE_CONTROL_L2AREG_GST_PGSIZE_OFFSET)
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_13F01250,
    L2_L2B_PGSIZE_CONTROL_L2BREG_GST_PGSIZE_MASK |
    L2_L2B_PGSIZE_CONTROL_L2BREG_HOST_PGSIZE_MASK,
    (0x49 << L2_L2B_PGSIZE_CONTROL_L2BREG_GST_PGSIZE_OFFSET) |
    (0x49 << L2_L2B_PGSIZE_CONTROL_L2BREG_HOST_PGSIZE_OFFSET)
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_13F01130,
    L2_CONTROL_5_GST_partial_ptc_cntrl_MASK,
    (0x49 << L2_CONTROL_5_GST_partial_ptc_cntrl_OFFSET)
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_13F0111C,
    L2_MISC_CNTRL_3_REG_atomic_filter_en_MASK |
    L2_MISC_CNTRL_3_REG_mask_l2_nw_MASK |
    L2_MISC_CNTRL_3_REG_ats_iw_MASK,
    (0x1 << L2_MISC_CNTRL_3_REG_atomic_filter_en_OFFSET) |
    (0x0 << L2_MISC_CNTRL_3_REG_mask_l2_nw_OFFSET) |
    (0x1 << L2_MISC_CNTRL_3_REG_ats_iw_OFFSET)
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_13F01158,
    L2_CP_CONTROL_CPFlushOnWait_MASK |
    L2_CP_CONTROL_CPFlushOnInv_MASK,
    (0x1 << L2_CP_CONTROL_CPFlushOnWait_OFFSET) |
    (0x0 << L2_CP_CONTROL_CPFlushOnInv_OFFSET)
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_13F0114C,
    L2B_UPDATE_FILTER_CNTL_L2b_Update_Filter_Bypass_MASK,
    (0x0 << L2B_UPDATE_FILTER_CNTL_L2b_Update_Filter_Bypass_OFFSET)
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_15700088,
    L2A_UPDATE_FILTER_CNTL_L2a_Update_Filter_Bypass_MASK,
    (0x0 << L2A_UPDATE_FILTER_CNTL_L2a_Update_Filter_Bypass_OFFSET)
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_13F01150,
    L2_TW_CONTROL_TWPrefetchRange_MASK |
    L2_TW_CONTROL_TWPrefetchEn_MASK |
    L2_TW_CONTROL_TWCacheNestedPTE_MASK |
    L2_TW_CONTROL_TWForceCoherent_MASK,
    (0x1 << L2_TW_CONTROL_TWPrefetchRange_OFFSET) |
    (0x1 << L2_TW_CONTROL_TWPrefetchEn_OFFSET) |
    (0x1 << L2_TW_CONTROL_TWCacheNestedPTE_OFFSET) |
    (0 << L2_TW_CONTROL_TWForceCoherent_OFFSET)
    ),
  SMN_ENTRY_RMW(SIL_RSVD_ADDR_13F00084,
    IOMMU_MMIO_CONTROL1_W_InvIotlbTypeSup_W_MASK,
    (0x0 << IOMMU_MMIO_CONTROL1_W_InvIotlbTypeSup_W_OFFSET)
    ),
  SMN_ENTRY_PROPERTY_RMW(SIL_RESERVED_1754,
    SIL_RSVD_ADDR_13F000E8,
    SIL_RESERVED_0786,
    (0x1 << SIL_RESERVED_0787)
    ),
  SMN_ENTRY_PROPERTY_RMW(SIL_RESERVED_1754,
    SIL_RSVD_ADDR_15704398,
    SIL_RESERVED_0795,
    (0x1 << SIL_RESERVED_0796)
    ),
  SMN_ENTRY_PROPERTY_RMW(SIL_RESERVED_1754,
    SIL_RSVD_ADDR_13F00308,
    SIL_RESERVED_0789,
    ( 0x1F << SIL_RESERVED_0790)
    ),
  SMN_ENTRY_PROPERTY_RMW(SIL_RESERVED_1754,
    SIL_RSVD_ADDR_15700154,
    BITS_0_TO_27_MASK_0003,
    (0xFFFFFF << SIL_RESERVED_0788)
    ),
  SMN_ENTRY_TERMINATE
};
// --------------------------------------------------
// PPR:       IOMMU Clock Gating
// --------------------------------------------------
SMN_TABLE NbioIommuClockGatingTbl [] = {
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_IOMMU_L2CLKGATING_ENABLED,
    SIL_RSVD_ADDR_157000CC,
    L2_L2A_CK_GATE_CONTROL_CKGateL2ARegsDisable_MASK | L2_L2A_CK_GATE_CONTROL_CKGateL2ADynamicDisable_MASK |
    L2_L2A_CK_GATE_CONTROL_CKGateL2ACacheDisable_MASK,
    (0x0 << L2_L2A_CK_GATE_CONTROL_CKGateL2ARegsDisable_OFFSET) |
    (0x0 << L2_L2A_CK_GATE_CONTROL_CKGateL2ADynamicDisable_OFFSET) |
    (0x0 << L2_L2A_CK_GATE_CONTROL_CKGateL2ACacheDisable_OFFSET)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_IOMMU_L2CLKGATING_DISABLED,
    SIL_RSVD_ADDR_157000CC,
    L2_L2A_CK_GATE_CONTROL_CKGateL2ARegsDisable_MASK | L2_L2A_CK_GATE_CONTROL_CKGateL2ADynamicDisable_MASK |
    L2_L2A_CK_GATE_CONTROL_CKGateL2ACacheDisable_MASK,
    (0x1 << L2_L2A_CK_GATE_CONTROL_CKGateL2ARegsDisable_OFFSET) |
    (0x1 << L2_L2A_CK_GATE_CONTROL_CKGateL2ADynamicDisable_OFFSET) |
    (0x1 << L2_L2A_CK_GATE_CONTROL_CKGateL2ACacheDisable_OFFSET)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_IOMMU_L2CLKGATING_ENABLED,
    SIL_RSVD_ADDR_13F01240,
    L2_L2B_CK_GATE_CONTROL_CKGateL2BRegsDisable_MASK | L2_L2B_CK_GATE_CONTROL_CKGateL2BDynamicDisable_MASK |
    L2_L2B_CK_GATE_CONTROL_CKGateL2BMiscDisable_MASK | L2_L2B_CK_GATE_CONTROL_CKGateL2BCacheDisable_MASK,
    (0x0 << L2_L2B_CK_GATE_CONTROL_CKGateL2BRegsDisable_OFFSET) |
    (0x0 << L2_L2B_CK_GATE_CONTROL_CKGateL2BDynamicDisable_OFFSET) |
    (0x0 << L2_L2B_CK_GATE_CONTROL_CKGateL2BMiscDisable_OFFSET) |
    (0x0 << L2_L2B_CK_GATE_CONTROL_CKGateL2BCacheDisable_OFFSET)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_IOMMU_L2CLKGATING_DISABLED,
    SIL_RSVD_ADDR_13F01240,
    L2_L2B_CK_GATE_CONTROL_CKGateL2BRegsDisable_MASK | L2_L2B_CK_GATE_CONTROL_CKGateL2BDynamicDisable_MASK |
    L2_L2B_CK_GATE_CONTROL_CKGateL2BMiscDisable_MASK | L2_L2B_CK_GATE_CONTROL_CKGateL2BCacheDisable_MASK,
    (0x1 << L2_L2B_CK_GATE_CONTROL_CKGateL2BRegsDisable_OFFSET) |
    (0x1 << L2_L2B_CK_GATE_CONTROL_CKGateL2BDynamicDisable_OFFSET) |
    (0x1 << L2_L2B_CK_GATE_CONTROL_CKGateL2BMiscDisable_OFFSET) |
    (0x1 << L2_L2B_CK_GATE_CONTROL_CKGateL2BCacheDisable_OFFSET)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_IOMMU_L1CLKGATING_ENABLED,
    SIL_RSVD_ADDR_153000CC,
    L1_CLKCNTRL_0_L1_CLKGATE_HYSTERESIS_MASK |
    L1_CLKCNTRL_0_L1_DMA_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_CACHE_CLKGATE_EN_MASK |
    L1_CLKCNTRL_0_L1_CPSLV_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_PERF_CLKGATE_EN_MASK |
    L1_CLKCNTRL_0_L1_MEMORY_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_REG_CLKGATE_EN_MASK |
    L1_CLKCNTRL_0_L1_HOSTREQ_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_DMARSP_CLKGATE_EN_MASK |
    L1_CLKCNTRL_0_L1_HOSTRSP_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_SION_PERF_CLKGATE_EN_MASK |
    L1_CLKCNTRL_0_L1_L2_CLKGATE_EN_MASK,
    (0x20 << L1_CLKCNTRL_0_L1_CLKGATE_HYSTERESIS_OFFSET) |
    (0x1 << L1_CLKCNTRL_0_L1_DMA_CLKGATE_EN_OFFSET) |
    (0x1 << L1_CLKCNTRL_0_L1_CACHE_CLKGATE_EN_OFFSET) |
    (0x1 << L1_CLKCNTRL_0_L1_CPSLV_CLKGATE_EN_OFFSET) |
    (0x1 << L1_CLKCNTRL_0_L1_PERF_CLKGATE_EN_OFFSET) |
    (0x1 << L1_CLKCNTRL_0_L1_MEMORY_CLKGATE_EN_OFFSET) |
    (0x1 << L1_CLKCNTRL_0_L1_REG_CLKGATE_EN_OFFSET) |
    (0x1 << L1_CLKCNTRL_0_L1_HOSTREQ_CLKGATE_EN_OFFSET) |
    (0x1 << L1_CLKCNTRL_0_L1_DMARSP_CLKGATE_EN_OFFSET) |
    (0x1 << L1_CLKCNTRL_0_L1_HOSTRSP_CLKGATE_EN_OFFSET) |
    (0x1 << L1_CLKCNTRL_0_L1_SION_PERF_CLKGATE_EN_OFFSET) |
    (0x1 << L1_CLKCNTRL_0_L1_L2_CLKGATE_EN_OFFSET)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_IOMMU_L1CLKGATING_DISABLED,
    SIL_RSVD_ADDR_147000CC,
    L1_CLKCNTRL_0_L1_DMA_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_CACHE_CLKGATE_EN_MASK |
    L1_CLKCNTRL_0_L1_CPSLV_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_PERF_CLKGATE_EN_MASK |
    L1_CLKCNTRL_0_L1_MEMORY_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_REG_CLKGATE_EN_MASK |
    L1_CLKCNTRL_0_L1_HOSTREQ_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_DMARSP_CLKGATE_EN_MASK |
    L1_CLKCNTRL_0_L1_HOSTRSP_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_SION_PERF_CLKGATE_EN_MASK |
    L1_CLKCNTRL_0_L1_L2_CLKGATE_EN_MASK,
    (0x0 << L1_CLKCNTRL_0_L1_DMA_CLKGATE_EN_OFFSET) |
    (0x0 << L1_CLKCNTRL_0_L1_CACHE_CLKGATE_EN_OFFSET) |
    (0x0 << L1_CLKCNTRL_0_L1_CPSLV_CLKGATE_EN_OFFSET) |
    (0x0 << L1_CLKCNTRL_0_L1_PERF_CLKGATE_EN_OFFSET) |
    (0x0 << L1_CLKCNTRL_0_L1_MEMORY_CLKGATE_EN_OFFSET) |
    (0x0 << L1_CLKCNTRL_0_L1_REG_CLKGATE_EN_OFFSET) |
    (0x0 << L1_CLKCNTRL_0_L1_HOSTREQ_CLKGATE_EN_OFFSET) |
    (0x0 << L1_CLKCNTRL_0_L1_DMARSP_CLKGATE_EN_OFFSET) |
    (0x0 << L1_CLKCNTRL_0_L1_HOSTRSP_CLKGATE_EN_OFFSET) |
    (0x0 << L1_CLKCNTRL_0_L1_SION_PERF_CLKGATE_EN_OFFSET) |
    (0x0 << L1_CLKCNTRL_0_L1_L2_CLKGATE_EN_OFFSET)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_IOMMU_L1CLKGATING_ENABLED,
    SIL_RSVD_ADDR_14B000CC,
    L1_CLKCNTRL_0_L1_CLKGATE_HYSTERESIS_MASK |
    L1_CLKCNTRL_0_L1_DMA_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_CACHE_CLKGATE_EN_MASK |
    L1_CLKCNTRL_0_L1_CPSLV_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_PERF_CLKGATE_EN_MASK |
    L1_CLKCNTRL_0_L1_MEMORY_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_REG_CLKGATE_EN_MASK |
    L1_CLKCNTRL_0_L1_HOSTREQ_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_DMARSP_CLKGATE_EN_MASK |
    L1_CLKCNTRL_0_L1_HOSTRSP_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_SION_PERF_CLKGATE_EN_MASK |
    L1_CLKCNTRL_0_L1_L2_CLKGATE_EN_MASK,
    (0x20 << L1_CLKCNTRL_0_L1_CLKGATE_HYSTERESIS_OFFSET) |
    (0x1 << L1_CLKCNTRL_0_L1_DMA_CLKGATE_EN_OFFSET) |
    (0x1 << L1_CLKCNTRL_0_L1_CACHE_CLKGATE_EN_OFFSET) |
    (0x1 << L1_CLKCNTRL_0_L1_CPSLV_CLKGATE_EN_OFFSET) |
    (0x1 << L1_CLKCNTRL_0_L1_PERF_CLKGATE_EN_OFFSET) |
    (0x1 << L1_CLKCNTRL_0_L1_MEMORY_CLKGATE_EN_OFFSET) |
    (0x1 << L1_CLKCNTRL_0_L1_REG_CLKGATE_EN_OFFSET) |
    (0x1 << L1_CLKCNTRL_0_L1_HOSTREQ_CLKGATE_EN_OFFSET) |
    (0x1 << L1_CLKCNTRL_0_L1_DMARSP_CLKGATE_EN_OFFSET) |
    (0x1 << L1_CLKCNTRL_0_L1_HOSTRSP_CLKGATE_EN_OFFSET) |
    (0x1 << L1_CLKCNTRL_0_L1_SION_PERF_CLKGATE_EN_OFFSET) |
    (0x1 << L1_CLKCNTRL_0_L1_L2_CLKGATE_EN_OFFSET)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_IOMMU_L1CLKGATING_DISABLED,
    SIL_RSVD_ADDR_153000CC,
    L1_CLKCNTRL_0_L1_DMA_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_CACHE_CLKGATE_EN_MASK |
    L1_CLKCNTRL_0_L1_CPSLV_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_PERF_CLKGATE_EN_MASK |
    L1_CLKCNTRL_0_L1_MEMORY_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_REG_CLKGATE_EN_MASK |
    L1_CLKCNTRL_0_L1_HOSTREQ_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_DMARSP_CLKGATE_EN_MASK |
    L1_CLKCNTRL_0_L1_HOSTRSP_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_SION_PERF_CLKGATE_EN_MASK |
    L1_CLKCNTRL_0_L1_L2_CLKGATE_EN_MASK,
    (0x0 << L1_CLKCNTRL_0_L1_DMA_CLKGATE_EN_OFFSET) |
    (0x0 << L1_CLKCNTRL_0_L1_CACHE_CLKGATE_EN_OFFSET) |
    (0x0 << L1_CLKCNTRL_0_L1_CPSLV_CLKGATE_EN_OFFSET) |
    (0x0 << L1_CLKCNTRL_0_L1_PERF_CLKGATE_EN_OFFSET) |
    (0x0 << L1_CLKCNTRL_0_L1_MEMORY_CLKGATE_EN_OFFSET) |
    (0x0 << L1_CLKCNTRL_0_L1_REG_CLKGATE_EN_OFFSET) |
    (0x0 << L1_CLKCNTRL_0_L1_HOSTREQ_CLKGATE_EN_OFFSET) |
    (0x0 << L1_CLKCNTRL_0_L1_DMARSP_CLKGATE_EN_OFFSET) |
    (0x0 << L1_CLKCNTRL_0_L1_HOSTRSP_CLKGATE_EN_OFFSET) |
    (0x0 << L1_CLKCNTRL_0_L1_SION_PERF_CLKGATE_EN_OFFSET) |
    (0x0 << L1_CLKCNTRL_0_L1_L2_CLKGATE_EN_OFFSET)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_IOMMU_L1CLKGATING_ENABLED,
    SIL_RSVD_ADDR_147000CC,
    L1_CLKCNTRL_0_L1_CLKGATE_HYSTERESIS_MASK |
    L1_CLKCNTRL_0_L1_DMA_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_CACHE_CLKGATE_EN_MASK |
    L1_CLKCNTRL_0_L1_CPSLV_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_PERF_CLKGATE_EN_MASK |
    L1_CLKCNTRL_0_L1_MEMORY_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_REG_CLKGATE_EN_MASK |
    L1_CLKCNTRL_0_L1_HOSTREQ_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_DMARSP_CLKGATE_EN_MASK |
    L1_CLKCNTRL_0_L1_HOSTRSP_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_SION_PERF_CLKGATE_EN_MASK |
    L1_CLKCNTRL_0_L1_L2_CLKGATE_EN_MASK,
    (0x20 << L1_CLKCNTRL_0_L1_CLKGATE_HYSTERESIS_OFFSET) |
    (0x1 << L1_CLKCNTRL_0_L1_DMA_CLKGATE_EN_OFFSET) |
    (0x1 << L1_CLKCNTRL_0_L1_CACHE_CLKGATE_EN_OFFSET) |
    (0x1 << L1_CLKCNTRL_0_L1_CPSLV_CLKGATE_EN_OFFSET) |
    (0x1 << L1_CLKCNTRL_0_L1_PERF_CLKGATE_EN_OFFSET) |
    (0x1 << L1_CLKCNTRL_0_L1_MEMORY_CLKGATE_EN_OFFSET) |
    (0x1 << L1_CLKCNTRL_0_L1_REG_CLKGATE_EN_OFFSET) |
    (0x1 << L1_CLKCNTRL_0_L1_HOSTREQ_CLKGATE_EN_OFFSET) |
    (0x1 << L1_CLKCNTRL_0_L1_DMARSP_CLKGATE_EN_OFFSET) |
    (0x1 << L1_CLKCNTRL_0_L1_HOSTRSP_CLKGATE_EN_OFFSET) |
    (0x1 << L1_CLKCNTRL_0_L1_SION_PERF_CLKGATE_EN_OFFSET) |
    (0x1 << L1_CLKCNTRL_0_L1_L2_CLKGATE_EN_OFFSET)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_IOMMU_L1CLKGATING_DISABLED,
    SIL_RSVD_ADDR_14B000CC,
    L1_CLKCNTRL_0_L1_DMA_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_CACHE_CLKGATE_EN_MASK |
    L1_CLKCNTRL_0_L1_CPSLV_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_PERF_CLKGATE_EN_MASK |
    L1_CLKCNTRL_0_L1_MEMORY_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_REG_CLKGATE_EN_MASK |
    L1_CLKCNTRL_0_L1_HOSTREQ_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_DMARSP_CLKGATE_EN_MASK |
    L1_CLKCNTRL_0_L1_HOSTRSP_CLKGATE_EN_MASK | L1_CLKCNTRL_0_L1_SION_PERF_CLKGATE_EN_MASK |
    L1_CLKCNTRL_0_L1_L2_CLKGATE_EN_MASK,
    (0x0 << L1_CLKCNTRL_0_L1_DMA_CLKGATE_EN_OFFSET) |
    (0x0 << L1_CLKCNTRL_0_L1_CACHE_CLKGATE_EN_OFFSET) |
    (0x0 << L1_CLKCNTRL_0_L1_CPSLV_CLKGATE_EN_OFFSET) |
    (0x0 << L1_CLKCNTRL_0_L1_PERF_CLKGATE_EN_OFFSET) |
    (0x0 << L1_CLKCNTRL_0_L1_MEMORY_CLKGATE_EN_OFFSET) |
    (0x0 << L1_CLKCNTRL_0_L1_REG_CLKGATE_EN_OFFSET) |
    (0x0 << L1_CLKCNTRL_0_L1_HOSTREQ_CLKGATE_EN_OFFSET) |
    (0x0 << L1_CLKCNTRL_0_L1_DMARSP_CLKGATE_EN_OFFSET) |
    (0x0 << L1_CLKCNTRL_0_L1_HOSTRSP_CLKGATE_EN_OFFSET) |
    (0x0 << L1_CLKCNTRL_0_L1_SION_PERF_CLKGATE_EN_OFFSET) |
    (0x0 << L1_CLKCNTRL_0_L1_L2_CLKGATE_EN_OFFSET)
    ),
  SMN_ENTRY_TERMINATE
};
// NBIO_IOMMU_CLOCK_GATING_TBL
// --------------------------------------------------
// PPR: IOMMU SMI Filtering
// --------------------------------------------------
SMN_TABLE NbioIommuSmiFilteringTbl [] = {
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_IOMMU_DISABLED,
    SIL_RSVD_ADDR_13B10034,
    SIL_RESERVED_0600,
    0x0
    ),
  SMN_ENTRY_TERMINATE
};

// If X2 APIC is enabled,
// enable XT_SUP in IOMMU

SMN_TABLE NbioIommuXtSupTbl [] = {
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_XAPIC_MODE,
    SIL_RSVD_ADDR_153000DC,
    L1_FEATURE_SUP_CNTRL_L1_XT_SUP_W_MASK,
    (0x1 << L1_FEATURE_SUP_CNTRL_L1_XT_SUP_W_OFFSET)
    ),
  SMN_ENTRY_TERMINATE
};

SMN_TABLE_ENTRY GnbIommuEnvInitTable [] = {
  SMN_ENTRY_TABLE_64(NbioIommuInitTbl),         // 15.3.1.2.1  This table initializes IOMMU
  SMN_ENTRY_TABLE_64(NbioIommuL1InitTbl),       // 15.3.1.2.2  IOMMU L1 Initialization
  SMN_ENTRY_TABLE_64(NbioIommuL2InitTbl),       // 15.3.1.2.3  IOMMU L2 Initialization
  SMN_ENTRY_TABLE_64(NbioIommuClockGatingTbl),  // 15.3.1.2.4  IOMMU Clock Gating
  SMN_ENTRY_TABLE_64(NbioIommuSmiFilteringTbl), // 15.3.1.2.5  IOMMU SMI Filtering
  SMN_ENTRY_TABLE_64(NbioIommuXtSupTbl),
  SMN_ENTRY_TERMINATE_TABLE
};

/*----------------------------------------------------------------------------------------*/
/**
 * NbioIommuInit
 *
 * Initialize IOMMU. Program NBIO IOMMU tables for features such as
 * L1/L2 clock gating, TW Filter Disable, Disable IOMMU if IOMMU enable
 * config is set to false. Allocate MMIO space for IOMMU controller.
 * Read modify Write IOMMU register based on a NBIO input parameter.
 *
 * @param  NBIOCLASS_DATA_BLOCK  Pointer to NBIO Data Block
 *
 * @return SIL_STATUS
 * @retval SilPass - everything is OK
 * @retval SilAborted- Failed to allocate MMIO resources or program IOMMU NBIO tables.
 *
 */
SIL_STATUS
NbioIommuInit (
  NBIOCLASS_DATA_BLOCK  *NbioIpBlockData
  )
{

  FABRIC_TARGET                      MmioTarget = {0};
  FABRIC_MMIO_ATTRIBUTE              MmioAttr;
  uint64_t                           IommMmioSize;
  uint64_t                           IommMmioBase;
  GNB_HANDLE                         *GnbHandle;
  SIL_STATUS                         Status;
  PCI_ADDR                           IommuPciAddress;
  uint32_t                           Value;
  bool                               ReserveIommuBar;
  uint32_t                           Property;
  SIL_RESERVED_UNION_0022       MmioControl0;
  SECURE_ENCRYPTION_EAX              SecureEncryptionEax;
  bool                               SnpSupported;
  RCMGR_IP2IP_API                    *RcMgrIp2Ip;

  Property = NBIO_TABLE_PROPERTY_DEFAULT;
  Status = SilPass;

  if (NbioIpBlockData == NULL) {
    assert(false);
    return SilInvalidParameter;
  }

  if (NbioIpBlockData->NbioConfigData.IommuL1ClockGatingEnable) {
    Property |= PROPERTY_IOMMU_L1CLKGATING_ENABLED;
  } else {
    Property |= PROPERTY_IOMMU_L1CLKGATING_DISABLED;
  }
  if (NbioIpBlockData->NbioConfigData.IommuL2ClockGatingEnable) {
    Property |= PROPERTY_IOMMU_L2CLKGATING_ENABLED;
  } else {
    Property |= PROPERTY_IOMMU_L2CLKGATING_DISABLED;
  }
  if (xApicMode == NbioIpBlockData->NbioConfigData.AmdApicMode) {
    Property |= PROPERTY_XAPIC_MODE;
  }
  if (false == NbioIpBlockData->NbioConfigData.IommuSupport) {
    Property |= PROPERTY_IOMMU_DISABLED;
  }
  if (false == NbioIpBlockData->NbioConfigData.MappingDramIntoHtHole) {
    Property |= SIL_RESERVED_1754;
  }
  NBIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (NbioIpBlockData->NbioConfigData.IommuMMIOAddressReservedEnable == 0x00) {
    NBIO_TRACEPOINT(SIL_TRACE_INFO, " We don't need reserved IOMMU MMIO space from GNB module \n");
    ReserveIommuBar = false;
  } else {
    NBIO_TRACEPOINT(SIL_TRACE_INFO, " Will reserve IOMMU Bars \n");
    ReserveIommuBar = true;
  }

  SnpSupported = false;
  if (NbioIpBlockData->NbioConfigData.SevSnpSupport) {
    SecureEncryptionEax.Value = 0;
    SecureEncryptionEax.Value = xUslGetSecureEncryption ();
    if (SecureEncryptionEax.Field.SNP != 0) {
      SnpSupported = true;
    }
  }

  GnbHandle = GetGnbHandle ();

  if (SilGetIp2IpApi(SilId_RcManager, (void **)(&RcMgrIp2Ip)) != SilPass) {
    NBIO_TRACEPOINT(SIL_TRACE_ERROR, " MMIO allocator API is not found.\n");
    return SilNotFound;
  }

  // Loop to program each GNB Handle appropriately

  while (GnbHandle != NULL) {
    if (GnbHandle->RBIndex < 4) {
      NBIO_TRACEPOINT(SIL_TRACE_INFO, " GnbHandle = 0x%x\n", GnbHandle);

      // Allocate BAR for IOMMU
      if (ReserveIommuBar) {
        MmioTarget.TgtType = TARGET_PCI_BUS;
        MmioTarget.SocketNum = GnbHandle->SocketId;
        MmioTarget.PciBusNum = (uint16_t) GnbHandle->Address.Address.Bus;
        MmioTarget.PciSegNum = (uint16_t) GnbHandle->Address.Address.Segment;
        MmioTarget.RbNum = GnbHandle->RBIndex;

        NBIO_TRACEPOINT(SIL_TRACE_INFO,
          "iommu rb_index: %d/%08x\n",
          MmioTarget.RbNum,
          MmioTarget.PciBusNum
          );

        IommMmioSize = SIZE_512KB;
        MmioAttr.MmioType = NON_PCI_DEVICE_BELOW_4G;
        Status = RcMgrIp2Ip->FabricReserveMmio(&IommMmioBase, &IommMmioSize, ALIGN_512K, MmioTarget, &MmioAttr);
        NBIO_TRACEPOINT(SIL_TRACE_INFO,
          " IOMMU MMIO at address 0x%x for Socket 0x%x Silicon 0x%x\n",
          IommMmioBase,
          GnbHandle->SocketId,
          GnbHandle->DieNumber
          );

        if (Status != SilPass) {
          NBIO_TRACEPOINT(SIL_TRACE_INFO, "Failed to allocate IoApic mmio space\n");
          return SilAborted;
        }
        Value = (uint32_t)IommMmioBase;
        IommuPciAddress = NbioGetHostPciAddress(GnbHandle);
        IommuPciAddress.Address.Function = 0x2;
        xUSLPciWrite32(IommuPciAddress.AddressValue | SIL_RESERVED_0791, Value);
      }

      // Program up IOMMU NBIO Tables
      ProgramNbioSmnTable(GnbHandle, (SMN_TABLE *)GnbIommuEnvInitTable, NBIO_SPACE(GnbHandle, 0), Property);

      // Disable IOMMUs
      if (GnbHandle->SocketId == 0) {
        if (GnbHandle->RBIndex == 0 && !(NbioIpBlockData->NbioConfigData.CfgIommuSocket0Nbio0Enable)) {
          xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
            GnbHandle->Address.Address.Bus,
            NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_13B10034),
            (uint32_t) ~(SIL_RESERVED_0600),
            0 << SIL_RESERVED_0601
            );
        } else if (GnbHandle->RBIndex == 1 && !(NbioIpBlockData->NbioConfigData.CfgIommuSocket0Nbio1Enable)) {
          xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
            GnbHandle->Address.Address.Bus,
            NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_13B10034),
            (uint32_t) ~(SIL_RESERVED_0600),
            0 << SIL_RESERVED_0601
            );
        } else if (GnbHandle->RBIndex == 2 && !(NbioIpBlockData->NbioConfigData.CfgIommuSocket0Nbio2Enable)) {
          xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
            GnbHandle->Address.Address.Bus,
            NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_13B10034),
            (uint32_t) ~(SIL_RESERVED_0600),
            0 << SIL_RESERVED_0601
            );
        } else if (GnbHandle->RBIndex == 3 && !(NbioIpBlockData->NbioConfigData.CfgIommuSocket0Nbio3Enable)) {
          xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
            GnbHandle->Address.Address.Bus,
            NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_13B10034),
            (uint32_t) ~(SIL_RESERVED_0600),
            0 << SIL_RESERVED_0601
            );
        }
      } else if (GnbHandle->SocketId == 1) {
        if (GnbHandle->RBIndex == 0 && !(NbioIpBlockData->NbioConfigData.CfgIommuSocket1Nbio0Enable)) {
          xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
            GnbHandle->Address.Address.Bus,
            NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_13B10034),
            (uint32_t) ~(SIL_RESERVED_0600),
            0 << SIL_RESERVED_0601
            );
        } else if (GnbHandle->RBIndex == 1 && !(NbioIpBlockData->NbioConfigData.CfgIommuSocket1Nbio1Enable)) {
          xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
            GnbHandle->Address.Address.Bus,
            NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_13B10034),
            (uint32_t) ~(SIL_RESERVED_0600),
            0 << SIL_RESERVED_0601
            );
        } else if (GnbHandle->RBIndex == 2 && !(NbioIpBlockData->NbioConfigData.CfgIommuSocket1Nbio2Enable)) {
          xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
            GnbHandle->Address.Address.Bus,
            NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_13B10034),
            (uint32_t) ~(SIL_RESERVED_0600),
            0 << SIL_RESERVED_0601
            );
        } else if (GnbHandle->RBIndex == 3 && !(NbioIpBlockData->NbioConfigData.CfgIommuSocket1Nbio3Enable)) {
          xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
            GnbHandle->Address.Address.Bus,
            NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_13B10034),
            (uint32_t) ~(SIL_RESERVED_0600),
            0 << SIL_RESERVED_0601
            );
        }
      }
      if (NbioIpBlockData->NbioConfigData.AmdApicMode != xApicMode) {
        IommuPciAddress = NbioGetHostPciAddress(GnbHandle);
        IommuPciAddress.Address.Function = 0x2;
        MmioControl0.Value = xUSLPciRead32(IommuPciAddress.AddressValue |
          SIL_RESERVED_0792
          );
        MmioControl0.Field.field_bit_2 = 1;
        MmioControl0.Field.field_bit_7 = 1;
        xUSLPciWrite32(IommuPciAddress.AddressValue |
          SIL_RESERVED_0792,
          MmioControl0.Value
          );
      }

      // IOMMU configuration for SEV-SNP enabled
      if (NbioIpBlockData->NbioConfigData.SevSnpSupport == true) {
        xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          NBIO_SPACE(GnbHandle, SMN_IOMMU_MMIO_CONTROL0_W_ADDRESS),
          (uint32_t) ~(IOMMU_MMIO_CONTROL0_W_GAM_SUP_W_MASK |
          SIL_RESERVED_0778 |
          SIL_RESERVED_0776),
          (1 << IOMMU_MMIO_CONTROL0_W_GAM_SUP_W_OFFSET) |
          (1 << SIL_RESERVED_0779) |
          (1 << SIL_RESERVED_0777)
          );
        xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          NBIO_SPACE(GnbHandle, SIL_RESERVED_0801),
          (uint32_t) ~(SIL_RESERVED_0793),
          (1 << SIL_RESERVED_0794)
          );
        xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_2400018),
          (uint32_t) ~(SIL_RESERVED_0802),
          (1 << SIL_RESERVED_0803)
          );
        xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_2400034),
          (uint32_t) ~(SIL_RESERVED_0818),
          (uint32_t)(1 << SIL_RESERVED_0819)
          );
        xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_SMN_IOMMU_MMIO_CONTROL1_W_ADDRESS),
          (uint32_t) ~(SIL_RESERVED_0780 |
          SIL_RESERVED_0784 |
          SIL_RESERVED_0782),
          (uint32_t)((0 << SIL_RESERVED_0781) |
          (1 << SIL_RESERVED_0785) |
          (1 << SIL_RESERVED_0783))
          );
        xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_240001C),
          (uint32_t) ~(SIL_RESERVED_0806 |
          SIL_RESERVED_0810 |
          SIL_RESERVED_0812 |
          SIL_RESERVED_0816 |
          SIL_RESERVED_0808 |
          SIL_RESERVED_0814 |
          SIL_RESERVED_0804),
          (uint32_t)((1 << SIL_RESERVED_0807) |
          (1 << SIL_RESERVED_0811) |
          (1 << SIL_RESERVED_0813) |
          (1 << SIL_RESERVED_0817) |
          (1 << SIL_RESERVED_0809) |
          (1 << SIL_RESERVED_0815) |
          ((NbioIpBlockData->NbioConfigData.AmdCxlOnAllPorts)? 1 : 0) << SIL_RESERVED_0805)
          );
      } else {
        // IOMMU General AVIC modes support
        xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          NBIO_SPACE(GnbHandle, SMN_IOHUB0NBIO0_IOMMU_MMIO_CONTROL0_W_ADDRESS),
          (uint32_t) ~(IOMMU_MMIO_CONTROL0_W_GAM_SUP_W_MASK),
          ((NbioIpBlockData->NbioConfigData.IommuAvicSupport ? 1 : 0) <<
            IOMMU_MMIO_CONTROL0_W_GAM_SUP_W_OFFSET)
          );
        NBIO_TRACEPOINT(SIL_TRACE_INFO,
          "Enable SNPAVICSup to %d\n",
          (NbioIpBlockData->NbioConfigData.IommuAvicSupport? 1 : 0)
          );
        xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          NBIO_SPACE(GnbHandle, SIL_RESERVED_0801),
          (uint32_t) ~(SIL_RESERVED_0793),
          ((NbioIpBlockData->NbioConfigData.IommuAvicSupport ? 1 : 0) <<
            SIL_RESERVED_0794)
          );
      }
      // IOMMU configuration for SEV-SNP disabled
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_SMN_IOMMU_MMIO_CONTROL1_W_ADDRESS),
        (uint32_t) ~(IOMMU_MMIO_CONTROL1_W_SNP_SUP_W_MASK),
        ((SnpSupported ? 1 : 0) << IOMMU_MMIO_CONTROL1_W_SNP_SUP_W_OFFSET)
        );
      Value = xUSLSmnRead(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_SMN_IOMMU_MMIO_CONTROL1_W_ADDRESS)
        );

      NBIO_TRACEPOINT(SIL_TRACE_INFO, "Read back from IOMM_MMIO_CONTRO1_W 0x%x\n", Value);

      Value = 0x1FFFF;
      xUSLSmnWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_15300274),
        Value
        );
      xUSLSmnWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_14700274),
        Value
        );
      xUSLSmnWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_14B00274),
        Value
        );

      // Sets the GAM_SUP value in IOMMU Extended feature register.
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, SMN_IOMMU_MMIO_CONTROL0_W_ADDRESS),
        (uint32_t) ~(IOMMU_MMIO_CONTROL0_W_GAM_SUP_W_MASK),
        (1 << IOMMU_MMIO_CONTROL0_W_GAM_SUP_W_OFFSET)
        );
      // IOMMU configuration for ATS enabled
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_15704400),
        (uint32_t) ~(SIL_RESERVED_0797),
        ((NbioIpBlockData->NbioConfigData.CfgIommuL2AtsCntlEn? 1 : 0) << SIL_RESERVED_0798)
        );
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_13F04400),
        (uint32_t) ~(SIL_RESERVED_0799),
        ((NbioIpBlockData->NbioConfigData.CfgIommuL2AtsCntlEn? 1 : 0) << SIL_RESERVED_0800)
        );
    }
    GnbHandle = GnbGetNextHandle(GnbHandle);
  }

  NBIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}
