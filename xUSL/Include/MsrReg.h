/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MsrReg.h
 * @brief Define Msr registers
 *
 */

#pragma once

/**********************************************************************************************************************
 * variable declaration
 *
 */

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
} SIL_RESERVED_UNION_0005;

/// C-state Address MSR Register
typedef union {
  struct {                               ///< Bitfields of C-state Address MSR Register
    uint64_t CstateAddr:16;              ///< C-state address
    uint64_t :48;                        ///< Reserved
  } Field;
  uint64_t Value;
} CSTATE_ADDRESS_MSR;

/// C-state Configuration 2 MSR
typedef union {
  struct {
    uint64_t  field_bits_0_to_5:6;
    uint64_t  field_bit_6:1;
    uint64_t  :1;
    uint64_t  field_bits_8_to_13:6;
    uint64_t  field_bit_14:1;
    uint64_t  :1;
    uint64_t  field_bits_16_to_21:6;
    uint64_t  field_bit_22:1;
    uint64_t  :1;
    uint64_t  field_bits_24_to_29:6;
    uint64_t  field_bit_30:1;
    uint64_t  :33;
  } Field;
  uint64_t  Value;
} SIL_RESERVED_UNION_0004;

#define SIL_RESERVED_0378                  0xC0010298ul

/// Pstate Current Limit MSR Register
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

/// Pstate Control MSR Register
typedef union {
  struct {                              ///< Bitfields of Pstate Current Limit
                                        ///< MSR Register
    uint64_t PstateCmd:3;               ///< Pstate change command
    uint64_t :61;                       ///< Reserved
  } Field;
  uint64_t  Value;
} PSTATE_CTL_STRUCT;

/// Pstate Status MSR Register
typedef union {
  struct {                              ///< Bitfields of Pstate Status MSR
                                        ///<  Register
    uint64_t CurPstate:3;               ///< Current Pstate
    uint64_t :61;                       ///< Reserved
  } Field;
  uint64_t  Value;
} PSTATE_STATUS_STRUCT;

/// CPU Watchdog Timer MSR Register
typedef union {
  struct {
    uint64_t field_bit_0:1;
    uint64_t field_bits_1_to_2:2;
    uint64_t field_bits_3_to_6:4;
    uint64_t field_bits_7_to_9:3;
    uint64_t :54;
  } Field;
  uint64_t  Value;
} SIL_RESERVED_UNION_0003;

/// Hardware PState Status MSR
typedef union {
  struct {
    uint64_t field_bits_0_to_7:8;
    uint64_t field_bits_8_to_13:6;
    uint64_t field_bits_14_to_21:8;
    uint64_t field_bits_22_to_24:3;
    uint64_t :39;
  } Field;
  uint64_t  Value;
} SIL_RESERVED_UNION_0021;

/// CS Machine Check Control Mask (MCA::CS::MCA_CTL_MASK_CS)
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

/// L3 Machine Check Control Mask (MCA::L3::MCA_CTL_MASK_L3)
typedef union {
  struct {
    uint32_t  ShadowTag:1;
    uint32_t  MultiHitShadowTag:1;
    uint32_t  Tag:1;
    uint32_t  MultiHitTag:1;
    uint32_t  DataArray:1;
    uint32_t  SdpParity:1;
    uint32_t  XiVictimQueue:1;
    uint32_t  Hwa:1;
    uint64_t  Reserved_63_8:56;
  } Field;
  uint64_t Value;
} MCA_CTL_MASK_L3_STRUCT;

/// NBIO Machine Check Address (MCA::NBIO::MCA_ADDR_NBIO)
typedef union {
  struct {
    uint64_t                            ErrorAddr:64;
  } Field;
  uint64_t Value;
} MCA_ADDR_NBIO_STRUCT;

/// NBIO Machine Check Configuration (MCA::NBIO::MCA_CONFIG_NBIO)
typedef union {
  struct {
    uint64_t                            field_bit_0:1;
    uint64_t                            :1;
    uint64_t                            field_bit_2:1;
    uint64_t                            field_bit_3:1;
    uint64_t                            field_bit_4:1;
    uint64_t                            field_bit_5:1;
    uint64_t                            field_bit_6:1;
    uint64_t                            field_bit_7:1;
    uint64_t                            field_bit_8:1;
    uint64_t                            field_bits_9_to_31:23;
    uint64_t                            field_bit_32:1;
    uint64_t                            field_bit_33:1;
    uint64_t                            field_bit_34:1;
    uint64_t                            field_bit_35:1;
    uint64_t                            field_bit_36:1;
    uint64_t                            field_bits_37_to_38:2;
    uint64_t                            field_bit_39:1;
    uint64_t                            field_bits_40_to_63:24;
  } Field;
  uint64_t Value;
} SIL_RESERVED_UNION_0024;

/// NBIO Machine Check Control Mask (MCA::NBIO::MCA_CTL_MASK_NBIO)
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

/// NBIO Machine Check Control (MCA::NBIO::MCA_CTL_NBIO)
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
} MCA_CTL_NBIO_STRUCT;

/// MCA LS IP Identification MSR
typedef union {
  struct {
    uint32_t  InstanceIdLo:32;
    uint32_t  HardwareID:12; ///< Hardware ID of the IP associated with this MCA bank.
    uint32_t  InstanceIdHi:4;
    uint32_t  McaType:16;
  } Field;
  uint64_t Value;
} MCA_IPID_LS_STRUCT;

/// Secure Nested Paging Reverse Map Table Base.
typedef union {
  struct {
    uint64_t             :13;                 ///< Reserved
    uint64_t RmpTableBase:39;                 ///< Secure Nested Paging Reverse Map Table Base.
    uint64_t             :12;                 ///< Reserved
  } Field;
  uint64_t  Value;
} SECURE_RMPTABLE_BASE;

/// Secure Nested Paging Reverse Map Table Limit.
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
    uint32_t Count:8;          ///<
    uint32_t McgCtlP:1;        ///<
    uint64_t Reserved_63_9:55; ///<
  } Field;
  uint64_t Value;
} MCG_CAP_STRUCT;

/// Smm Base Address MSR (Core::X86::Msr::SMM_BASE)
typedef union {
  struct {
    uint32_t  SmmBase:32;         ///< Base address of SMM memory region
    uint32_t  Reserved_63_32:32;  ///<
  } Field;
  uint64_t Value;
} SMM_BASE_STRUCT;

typedef union {
  struct {
    uint64_t McaX:1;                              ///< This bank provides Machine Check Architecture Extensions
    uint64_t :1;                                  ///< Reserved
    uint64_t DeferredErrorLoggingSupported:1;     ///< Deferred errors are supported in this MCA bank
    uint64_t :2;                                  ///< Reserved
    uint64_t DeferredIntTypeSupported:1;          ///< Controls the type of interrupt generated on a deferred error
    uint64_t :2;                                  ///< Reserved
    uint64_t McaLsbInStatusSupported:1;           ///< Indicates that AddrLsb is located in McaStatus registers
    uint64_t McaFruTextInMca:1;                   ///< Configure if FruText is reported McaSynd1/McaSynd2 registers
    uint64_t :22;                                 ///< Reserved
    uint64_t McaXEnable:1;                        ///< Software Ack for MCAX feature set
    uint64_t :1;                                  ///< Reserved
    uint64_t LogDeferredInMcaStat:1;              ///< Log deferred errors in MCA_STATUS_UMC and MCA_ADDR_UMC
    uint64_t :2;                                  ///< Reserved
    uint64_t DeferredIntType:2;                   ///< Specifies the type of interrupt signaled when a deferred error is
                                                  ///< logged
    uint64_t :25;                                 ///< Reserved
  } Field;
  uint64_t  Value;
} MCA_CONFIG_UMC_STRUCT;

/******************************************************************************
 * Declare macros here
 *
 */

#define TW_CFG_COMBINED_CR0_CD              BIT_64(49) ///< Core::X86::Msr::TW_CFG::CombineCr0Cd.
                                                       ///< Combine H_CR0.CD of all threads in memtype.
#define SYS_CFG_MTRR_FIX_DRAM_EN            BIT_64(18) ///< Core::X86::Msr::SYS_CFG::MtrrFixDramEn.
                                                       ///< MTRR fixed RdDram and WrDram attributes enable.
#define SYS_CFG_MTRR_FIX_DRAM_MOD_EN        BIT_64(19) ///< Core::X86::Msr::SYS_CFG::MtrrFixDramModEn.
                                                       ///< MTRR fixed RdDram and WrDram modification enable.
#define SYS_CFG_MTRR_VAR_DRAM_EN            BIT_64(20) ///< Core::X86::Msr::SYS_CFG::MtrrVarDramEn.
                                                       ///< MTRR variable DRAM enable.
#define SYS_CFG_MTRR_TOM2_EN                BIT_64(21) ///< Core::X86::Msr::SYS_CFG::MtrrTom2En. MTRR
                                                       ///< top of memory 2 enable.
#define SYS_CFG_TOM2_FORCE_MEM_TYPE_WB      BIT_64(22) ///< Core::X86::Msr::SYS_CFG::Tom2ForceMemTypeWB.
                                                       ///< top of memory 2 memory type write back.

#define MCA_BANKS_VISIBLE_MASK              0xFF ///< Mask to determine the
                                                 ///< number of error reporting
                                                 ///< banks visible
#define MCA_BANK_SIZE                       0x10 ///< Size of each MCA Bank

#define MSR_APIC_BAR                        0x0000001Bul
#define MSR_PATCH_LEVEL                     0x0000008Bul
#define MSR_SYS_CFG                         0xC0010010ul
#define MSR_PSTATE_CURLIM_ADDRESS           0xC0010061ul ///< P-State Current
                                                         ///< Limit
#define MSR_PSTATE_CTL_ADDRESS              0xC0010062ul ///< P-State Control
#define MSR_PSTATE_STATUS_ADDRESS           0xC0010063ul ///< P-State Status
#define MSR_PSTATE_DEF_ADDRESS              0xC0010064ul ///< P-State MSR
#define MSR_CPU_WDT_CFG                     0xC0010074ul ///< CPU Watchdog Timer
#define MSR_PMGT_MISC_ADDRESS               0xC0010292ul ///< Power Management
                                                         ///< Misc
#define SIL_RESERVED_0382        0xC0010293ul
#define SIL_RESERVED_0385               0xC0011000ul
#define MSR_CPUID_FN_00000007_FEATURES      0xC0011002ul
#define MSR_LS_CFG                          0xC0011020ul
#define MSR_IC_CFG                          0xC0011021ul
#define MSR_DC_CFG                          0xC0011022ul
#define SIL_RESERVED_0387                          0xC0011023ul
#define MSR_DE_CFG                          0xC0011029ul
#define MSR_L2_CFG                          0xC001102Aul
#define MSR_L2_PREFETCH_CFG                 0xC001102Bul ///< L2 Prefetch Configuration (Core::X86::Msr::ChL2PfCfg)
#define MSR_LS_CFG2                         0xC001102Dul

#define MSR_SVM_REV_FEAT_ID                 0xC00110DDul

#define MSR_EXTENDED_FEATURE_EDX0_ECX0      0xC00110DFul
#define MSR_L2_CFG1                         0xC00110E2ul

#define MSR_LS_CFG3                         0xC00110E5ul
#define MSR_LS_CFG4                         0xC00110E6ul

#define MSR_DC_PF_CFG                       0xC00110E7ul
#define MSR_L2_AA_PAIR_CFG0                 0xC00110E9ul ///< ChL2AaPairCfg0 L2 Adaptive Allocation Configuration
                                                         ///< Register for 0 or Base of Dueling Pair
#define MSR_L2_AA_PAIR_CFG1                 0xC00110EAul ///< ChL2AaPairCfg1 L2 Adaptive Allocation Configuration
                                                         ///< Register for 1 or New of Dueling Pair

#define SIL_RESERVED_0383                         0xC00110EBul
#define SIL_RESERVED_0384                         0xC00110ECul
#define SIL_RESERVED_0377                      0xC0010296ul
#define MSR_CSTATE_ADDRESS                  0xC0010073ul       /// C-state Base Address Register 0xC0010073
#define SIL_RESERVED_0379                         0xC0011074ul
#define SIL_RESERVED_0380               0xC0011076ul
#define SIL_RESERVED_0381                    0xC0011077ul
#define MSR_LS_RMP_BASE                     0xC0010132ul ///< Secure Nested Paging Reverse Map Table Base
                                                         ///< Core::X86::Msr::LS_RMP_BASE
#define MSR_LS_RMP_END                      0xC0010133ul ///< Secure Nested Paging Reverse Map Table Limit
                                                         ///< Core::X86::Msr::LS_RMP_END
#define SIL_RESERVED_0386                         0xC001100Cul

// MSRs
// ------------------------
#define MSR_APIC_BAR                        0x0000001Bul
#define MSR_PATCH_LEVEL                     0x0000008Bul
#define MSR_SYS_CFG                         0xC0010010ul
#define MSR_TOM                             0xC001001Aul
#define MSR_TOM2                            0xC001001Dul
#define MSR_EXTENDED_FEATURE_EN             0xC0000080ul
#define MSR_PATCH_LOADER                    0xC0010020ul
#define MSR_SYS_CFG                         0xC0010010ul    // SYSCFG
#define MSR_TOM                             0xC001001Aul    // TOP_MEM
#define MSR_TOM2                            0xC001001Dul    // TOP_MEM2
#define MSR_VM_CR                           0xC0010114ul    // Virtual Machine Control
#define MSR_SVM_LOCK_KEY                    0xC0010118ul    // SVM Lock Key
#define MSR_CPUID_FEATS                     0xC0011004ul    // CPUID Features
#define MSR_CPUID_NAME_STRING0              0xC0010030ul    // First CPUID namestring register
#define MSR_CPUID_NAME_STRING1              0xC0010031ul
#define MSR_CPUID_NAME_STRING2              0XC0010032ul
#define MSR_CPUID_NAME_STRING3              0xC0010033ul
#define MSR_CPUID_NAME_STRING4              0xC0010034ul
#define MSR_CPUID_NAME_STRING5              0xC0010035ul    // Last CPUID namestring register

// RAS MSRs
#define MSR_MCA_INTR_CFG                    0xC0000410ul ///< MCA Interrupt
                                                         ///< Configuration
#define MSR_MCA_CFG_BANK0                   0xC0002004ul ///< MCA Config
                                                         ///< Register 0xC0002xx4
#define MCA_IPID_LS_ADDRESS                 0xC0002005UL ///< MCA LS IP
                                                         ///< Identification
                                                         ///< Register
#define MCA_CTL_MASK_LS_ADDRESS             0xC0010400ul
#define MCA_CTL_MASK_IF_ADDRESS             0xC0010401ul
#define MCA_CTL_MASK_L2_ADDRESS             0xC0010402ul
#define MCA_CTL_MASK_FP_ADDRESS             0xC0010406ul

#define SIL_RESERVED_0376            0xC001071Eul

/**
 * @brief Global Machine Check Capabilities register
 *
 * @details The Global Machine Check Capabilities register (Core::X86::Msr::MCG_CAP) is located in MSR address 0x179.
 */
#define MSR_MCG_CAP                     (0x00000179UL)

/**
 * @brief Hardware Configuration register
 *
 * @details The Hardware Configuration register (Core::X86::Msr::HWCR) is located in MSR address 0xC001_0015.
 */
#define MSR_HWCR                        (0xC0010015UL)

/**
 * @brief MMIO Configuration Base Address register
 *
 * @details The MMIO Configuration Base Address register (Core::X86::Msr::MmioCfgBaseAddr)
 *          is located in MSR address 0xC001_0058.
 */
#define MSR_MMIO_CFG_BASE               (0xC0010058UL)

/**
 * @brief SMM Base Address register
 *
 * @details The SMM Base Address register (Core::X86::Msr::SMM_BASE)
 *          is located in MSR address 0xC001_0111.
 */
#define MSR_SMM_BASE                    (0xC0010111UL)

/**
 * @brief Legacy MCA MSR base address
 *
 * @details The legacy MCA MSRs are MSR0000_04[7F:00].
 *          The legacy MCA MSR space contains 32 banks of 4 registers per bank.
 */
#define MCA_LEGACY_BASE                 (0x00000400UL)

/**
 * @brief The maximum number of MCA banks in the legacy MCA MSR space
 *
 * @details The legacy MCA MSRs are MSR0000_04[7F:00].
 *          The legacy MCA MSR space contains '32' banks of 4 registers per bank.
 */
#define MCA_LEGACY_MAX_BANK             (32)

/**
 * @brief The number of Legacy MCA MSR registers per bank
 *
 * @details The legacy MCA MSRs are MSR0000_04[7F:00].
 *          The legacy MCA MSR space contains 32 banks of '4' registers per bank.
 */
#define MCA_LEGACY_REG_PER_BANK         (1 << 2)

/**
 * @brief The MSR address just beyond the maximum legacy MCA MSR address in the legacy MCA MSR space
 *
 * @details The maximum legacy MCA MSR address in the legacy MCA MSR space is MSR 0x47F.
 *          The MSR address just beyond the maximum legacy MCA MSR address in the legacy MCA MSR space is 0x480.
 *          MCA_LEGACY_TOP_ADDR along with MCA_LEGACY_BASE can be used to check whether the specified MSR address is
 *          located in the legacy MCA MSR space (i.e. MCA_LEGACY_BASE <= specified MSR address < MCA_LEGACY_TOP_ADDR)
 */
#define MCA_LEGACY_TOP_ADDR             (MCA_LEGACY_BASE + (MCA_LEGACY_MAX_BANK * MCA_LEGACY_REG_PER_BANK))

/**
 * @brief Used to get the MCA register offset from the legacy MCA MSR address.
 *
 * @details Any legacy MCA MSR address BitwiseAnd MCA_REG_OFFSET_MASK can get the offset of MCA register
 *          0: MCA_CTL, 1: MCA_STATUS, 2: MCA_ADDR, 3: MCA_MISC0
 */
#define MCA_REG_OFFSET_MASK             (0x00000003)

/**
 * @brief The MSR address of the first MCA_CTL_MASK register
 *
 * @details MCA_CTL_MASK MSRs are present at MSRC001_04[3F:00]. corresponding to MCA Bank 0:63
 *          Uncorrected errors cannot be corrected by hardware. Uncorrected errors update the status
 *          and address registers if not masked from logging in MCA_CTL_MASK.
 */
#define MCA_CTL_MASK_BASE               (0xC0010400UL)

/**
 * @brief The MSR address of the first MCAX register
 *
 * @details Machine Check Architecture Extensions (MCAX) is AMD's x86-64 extension to the Machine Check Architecture.
 *          MCAX supports up to 64 MCA banks per logical core. MSRC000_2000 for the beginning of MCA Bank 0
 *          MCAX MSRs are present at MSRC000_2[3FF:000]. This MSR address range contains space for 64 banks of 16
 *          registers each. MSRC000_2[FFF:400] are Reserved for future use. The MCAX MSR address range allows access
 *          to both legacy MCA registers and MCAX registers in each MCA bank.
 */
#define MCA_EXTENSION_BASE              (0xC0002000UL)

/**
 * @brief The total number of MCA registers in an MCAX Bank.
 *
 * @details MCAX MSRs are present at MSRC000_2[3FF:000]. This MSR address range contains space for 64 banks
 * of 16 registers each.
 */
#define SMCA_REG_PER_BANK               (1 << 4)
