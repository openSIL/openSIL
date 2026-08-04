/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MsrReg.h
 * @brief Define Msr registers
 *
 */

#pragma once

typedef union {
  struct {
    uint64_t field_bits_0_to_5:6;
    uint64_t field_bit_6:1;
    uint64_t :1;
    uint64_t field_bits_8_to_13:6;
    uint64_t field_bit_14:1;
    uint64_t :1;
    uint64_t field_bits_16_to_21:6;
    uint64_t field_bit_22:1;
    uint64_t :1;
    uint64_t  field_bits_24_to_29:6;
    uint64_t  field_bit_30:1;
    uint64_t  :33;
  } Field;
  uint64_t Value;
} SIL_RESERVED_UNION_0006;

typedef union {
  struct {                               ///< Bitfields of C-state Address MSR Register
    uint64_t CstateAddr:16;              ///< C-state address
    uint64_t :48;                        ///< Reserved
  } Field;
  uint64_t Value;
} CSTATE_ADDRESS_MSR;

typedef union {
  struct {                              ///< Bitfields of Pstate Current Limit
                                        ///< MSR Register
    uint64_t CurPstateLimit:3;          ///< Current Pstate Limit
    uint64_t :1;                        ///< Reserved
    uint64_t PstateMaxVal:3;            ///< Pstate Max Value
    uint64_t :57;                       ///< Reserved
  } Field;
  uint64_t  Value;
} PSTATE_CURLIM_STRUCT;

typedef union {
  struct {                              ///< Bitfields of Pstate Current Limit
                                        ///< MSR Register
    uint64_t PstateCmd:3;               ///< Pstate change command
    uint64_t :61;                       ///< Reserved
  } Field;
  uint64_t  Value;
} PSTATE_CTL_STRUCT;

typedef union {
  struct {                              ///< Bitfields of Pstate Status MSR
                                        ///<  Register
    uint64_t CurPstate:3;               ///< Current Pstate
    uint64_t :61;                       ///< Reserved
  } Field;
  uint64_t  Value;
} PSTATE_STATUS_STRUCT;

typedef union {
  struct {
    uint64_t field_bit_0:1;
    uint64_t field_bits_1_to_2:2;
    uint64_t field_bits_3_to_6:4;
    uint64_t field_bits_7_to_9:3;
    uint64_t :54;
  } Field;
  uint64_t  Value;
} SIL_RESERVED_UNION_0005;

typedef union {
  struct {
    uint64_t field_bits_0_to_7:8;
    uint64_t field_bits_8_to_13:6;
    uint64_t field_bits_14_to_21:8;
    uint64_t field_bits_22_to_24:3;
    uint64_t :39;
  } Field;
  uint64_t  Value;
} SIL_RESERVED_UNION_0026;

typedef union {
  struct {
    uint32_t  FTI_ILL_REQ:1;
    uint32_t  FTI_ADDR_VIOL:1;
    uint32_t  FTI_SEC_VIOL:1;
    uint32_t  FTI_ILL_RSP:1;
    uint32_t  FTI_RSP_NO_MTCH:1;
    uint32_t  FTI_PAR_ERR:1;
    uint32_t  SDP_PAR_ERR:1;
    uint32_t  ATM_PAR_ERR:1;
    uint32_t  SDP_RSP_NO_MTCH:1;
    uint32_t  SPF_PRT_ERR:1;
    uint32_t  SPF_ECC_ERR:1;
    uint32_t  SDP_UNEXP_RETRY:1;
    uint32_t  CNTR_OVFL:1;
    uint32_t  CNTR_UNFL:1;
    uint64_t  Reserved_63_14:50;
  } Field;
  uint64_t Value;
} MCA_CTL_MASK_CS_STRUCT;

typedef union {
  struct {
    uint64_t                            EccParityError:1;
    uint64_t                            PCIE_Sideband:1;
    uint64_t                            Ext_ErrEvent:1;
    uint64_t                            Egress_Poison:1;
    uint64_t                            IOHC_Internal_Poison:1;
    uint64_t                            Int_ErrEvent:1;
    uint64_t                            Reserved_63_6:58;
  } Field;
  uint64_t Value;
} MCA_CTL_MASK_NBIO_STRUCT;

typedef union {
  struct {
    uint32_t  InstanceIdLo:32;
    uint32_t  HardwareID:12; ///< Hardware ID of the IP associated with this MCA bank.
    uint32_t  InstanceIdHi:4;
    uint32_t  McaType:16;
  } Field;
  uint64_t Value;
} MCA_IPID_LS_STRUCT;

typedef union {
  struct {
    uint64_t             :13;                 ///< Reserved
    uint64_t RmpTableBase:39;                 ///< Secure Nested Paging Reverse Map Table Base.
    uint64_t             :12;                 ///< Reserved
  } Field;
  uint64_t  Value;
} SECURE_RMPTABLE_BASE;

typedef union {
  struct {
    uint64_t             :13;                 ///< Reserved
    uint64_t RmpTableEnd :39;                 ///< Secure Nested Paging Reverse Map Table Limit.
    uint64_t             :12;                 ///< Reserved
  } Field;
  uint64_t  Value;
} SECURE_RMPTABLE_END;

typedef union {
  struct {
    uint32_t Count:8;
    uint32_t McgCtlP:1;
    uint64_t Reserved_63_9:55;
  } Field;
  uint64_t Value;
} MCG_CAP_STRUCT;

/// Smm Base Address
typedef union {
  struct {
    uint32_t  SmmBase:32;
    uint32_t  Reserved_63_32:32;
  } Field;
  uint64_t Value;
} SMM_BASE_STRUCT;

#define TW_CFG_COMBINED_CR0_CD              BIT_64(49)
#define SYS_CFG_MTRR_FIX_DRAM_EN            BIT_64(18)
#define SYS_CFG_MTRR_FIX_DRAM_MOD_EN        BIT_64(19)
#define SYS_CFG_MTRR_VAR_DRAM_EN            BIT_64(20)
#define SYS_CFG_MTRR_TOM2_EN                BIT_64(21)
#define MTRR_VARIABLE_MASK0_VALID           BIT_64(11)
#define SYS_CFG_TOM2_FORCE_MEM_TYPE_WB      BIT_64(22)
#define MCA_BANKS_VISIBLE_MASK              0xFF
#define MCA_BANK_SIZE                       0x10
#define MSR_PATCH_LEVEL                     0x0000008Bul
#define MTRR_VARIABLE_BASE0                 0x200ul
#define BIT_11_AAAABHA                 0x201ul
#define MSR_PSTATE_CURLIM_ADDRESS           0xC0010061ul
#define MSR_PSTATE_CTL_ADDRESS              0xC0010062ul
#define MSR_PSTATE_STATUS_ADDRESS           0xC0010063ul
#define MSR_PSTATE_DEF_ADDRESS              0xC0010064ul
#define MSR_CPU_WDT_CFG                     0xC0010074ul
#define MSRxC0010293        0xC0010293ul
#define MSRxC0011000               0xC0011000ul
#define MSRxC0011002      0xC0011002ul
#define MSR_LS_CFG                          0xC0011020ul
#define MSR_IC_CFG                          0xC0011021ul
#define MSR_DC_CFG                          0xC0011022ul
#define MSRxC0011023                          0xC0011023ul
#define MSR_FP_CFG                          0xC0011028ul
#define MSR_DE_CFG                          0xC0011029ul
#define MSR_L2_CFG                          0xC001102Aul
#define MSR_L2_PREFETCH_CFG                 0xC001102Bul
#define MSRxC001102E                          0xC001102Eul
#define MSRxC0011093                         0xC0011093ul
#define MSRxC00110DC  0xC00110DCul

#define MSRxC00110DD                 0xC00110DDul

#define MSRxC00110DF      0xC00110DFul
#define MSRxC00110E2                         0xC00110E2ul
#define MSRxC00110E3                         0xC00110E3ul

#define MSRxC00110E5                         0xC00110E5ul
#define MSRxC00110E6                         0xC00110E6ul
#define MSRxC00110E7                         0xC00110E7ul

#define MSRxC00110EC                         0xC00110ECul
#define MSRxC0010296                      0xC0010296ul
#define MSR_CSTATE_ADDRESS                  0xC0010073ul
#define MSRxC0011074                         0xC0011074ul
#define MSRxC0011076               0xC0011076ul
#define MSRxC0011077                    0xC0011077ul
#define MSR_LS_RMP_BASE                     0xC0010132ul
#define MSR_LS_RMP_END                      0xC0010133ul
#define MSRxC001100C                         0xC001100Cul

// MSRs
// ------------------------
#define MSR_APIC_BAR                        0x0000001Bul
#define MSR_TOM                             0xC001001Aul
#define MSR_TOM2                            0xC001001Dul
#define MSR_PATCH_LOADER                    0xC0010020ul
#define MSR_SYS_CFG                         0xC0010010ul
#define MSR_SMM_ADDR                        0xC0010112ul
#define MSR_VM_CR                           0xC0010114ul
#define MSR_SVM_LOCK_KEY                    0xC0010118ul
#define MSR_CPUID_NAME_STRING0              0xC0010030ul
#define MSR_CPUID_NAME_STRING1              0xC0010031ul
#define MSR_CPUID_NAME_STRING2              0XC0010032ul
#define MSR_CPUID_NAME_STRING3              0xC0010033ul
#define MSR_CPUID_NAME_STRING4              0xC0010034ul
#define MSR_CPUID_NAME_STRING5              0xC0010035ul
#define MSR_OSVW_ID_Length                  0xC0010140ul
#define MSR_OSVW_Status                     0xC0010141ul

#define MSR_MCA_CFG_BANK0                   0xC0002004ul
#define MCA_IPID_LS_ADDRESS                 0xC0002005UL
#define MCA_CTL_MASK_LS_ADDRESS             0xC0010400ul
#define MCA_CTL_MASK_IF_ADDRESS             0xC0010401ul
#define MCA_CTL_MASK_L2_ADDRESS             0xC0010402ul
#define MCA_CTL_MASK_FP_ADDRESS             0xC0010406ul

#define MSR_MCG_CAP                     (0x00000179UL)
#define MSR_HWCR                        (0xC0010015UL)
#define MSR_MMIO_CFG_BASE               (0xC0010058UL)
