/**
 * @file  MsrReg.h
 * @brief Define Msr registers
 *
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

/**********************************************************************************************************************
 * variable declaration
 *
 */
/// C-state Configuration MSR
typedef union {
  struct {                              ///< Bitfields of C-state Configuration MSR
    uint64_t Reserved1:6;           ///< CCR0 CC1 DFS ID
    uint64_t Reserved2:1;              ///< CCR0 CC6 Enable
    uint64_t :1;                        ///< Reserved
    uint64_t Reserved3:6;           ///< CCR1 CC1 DFS ID
    uint64_t Reserved4:1;              ///< CCR1 CC6 Enable
    uint64_t :1;                        ///< Reserved
    uint64_t Reserved5:6;           ///< CCR2 CC1 DFS ID
    uint64_t Reserved6:1;              ///< CCR2 CC6 Enable
    uint64_t :41;                       ///< Reserved
  } Field;
  uint64_t Value;
} SIL_RESERVED2_912_STRUCT;

/// C-state Address MSR Register
typedef union {
  struct {                               ///< Bitfields of C-state Address MSR Register
    uint64_t CstateAddr:16;              ///< C-state address
    uint64_t :48;                        ///< Reserved
  } Field;
  uint64_t Value;
} CSTATE_ADDRESS_MSR;

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

/// P-state MSR
typedef union {
  struct {                              ///< Bitfields of P-state MSR
    uint64_t CpuFid:8;                  ///< Core Frequency Multiplier
    uint64_t CpuDfsId:6;                ///< Core Frequency Divisor
    uint64_t CpuVid:8;                  ///< Core Voltage ID
    uint64_t IddValue:8;                ///< Electical Current value
    uint64_t IddDiv:2;                  ///< Electical Current divisor
    uint64_t :31;                       ///< Reserved
    uint64_t PstateEn:1;                ///< Pstate Enable
  } Field;
  uint64_t  Value;
} PSTATE_DEF_STRUCT;

typedef union {
  struct {
    uint64_t Reserved1:1;
    uint64_t Reserved2:2;
    uint64_t Reserved3:4;
    uint64_t Reserved4:3;
    uint64_t :54;
  } Field;
  uint64_t  Value;
} SIL_RESERVED2_907_STRUCT;

/// Power Management Miscellaneous MSR
typedef union {
  struct {                              ///< Bitfields of Power Management
                                        ///< Miscellaneous MSR
    uint64_t CurHwPstateLimit:3;        ///< Current HW Pstate limit
    uint64_t StartupPstate:3;           ///< Start up Pstate
    uint64_t DFPstateDis:1;             ///< DF Pstate disable
    uint64_t CurDFVid:8;                ///< Current DF VID
    uint64_t MaxCpuCof:6;               ///< Maximum CPU COF
    uint64_t MaxDFCof:5;                ///< Maximum DF COF
    uint64_t CpbCap:3;                  ///< CPB capability
    uint64_t :3;                        ///< Reserved
    uint64_t PC6En:1;                   ///< PC6 enable
    uint64_t :31;                       ///< Reserved
  } Field;
  uint64_t  Value;
} PMGT_MISC_STRUCT;

typedef union {
  struct {
    uint64_t Reserved1:8;
    uint64_t Reserved2:6;
    uint64_t Reserved3:8;
    uint64_t Reserved4:3;
    uint64_t :39;
  } Field;
  uint64_t  Value;
} SIL_RESERVED2_913_STRUCT;

/// CS Machine Check Control Mask (MCA::CS::MCA_CTL_MASK_CS)
typedef union {
  struct {
    uint32_t  FTI_ILL_REQ:1 ;
    uint32_t  FTI_ADDR_VIOL:1 ;
    uint32_t  FTI_SEC_VIOL:1 ;
    uint32_t  FTI_ILL_RSP:1 ;
    uint32_t  FTI_RSP_NO_MTCH:1;
    uint32_t  FTI_PAR_ERR:1 ;
    uint32_t  SDP_PAR_ERR:1 ;
    uint32_t  ATM_PAR_ERR:1 ;
    uint32_t  SDP_RSP_NO_MTCH:1 ;
    uint32_t  SPF_PRT_ERR:1 ;
    uint32_t  SPF_ECC_ERR:1 ;
    uint32_t  SDP_UNEXP_RETRY:1 ;
    uint32_t  CNTR_OVFL:1 ;
    uint32_t  CNTR_UNFL:1 ;
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
    uint32_t Count:8 ;          ///<
    uint32_t McgCtlP:1 ;        ///<
    uint64_t Reserved_63_9:55;  ///<
  } Field;
  uint64_t Value;
} MCG_CAP_STRUCT;

typedef union {
  struct {
    uint32_t GenerateDeferredLvtOnExit:1 ; ///<
    uint32_t GenerateThresholdLvtOnExit:1 ; ///<
    uint32_t GenerateMceOnExit:1 ; ///<
    uint64_t Reserved_63_4:60; ///<
  } Field;
  uint64_t Value;
} SIL_RESERVED_1;

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
 * Defines here
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
#define MCA_BANKS_VISIBLE_MASK              0xFF       ///< Mask to determine the
                                                       ///< number of error reporting
                                                       ///< banks visible
#define MCA_BANK_SIZE                       0x10       ///< Size of each MCA Bank
#define MSR_APIC_BAR                        0x0000001Bul
#define MSR_SPEC_CTRL                       0x00000048ul
#define MSR_PATCH_LEVEL                     0x0000008Bul
#define MSR_SYS_CFG                         0xC0010010ul
#define MSR_PSTATE_CURLIM_ADDRESS           0xC0010061ul    ///< P-State Current Limit
#define MSR_PSTATE_CTL_ADDRESS              0xC0010062ul    ///< P-State Control
#define MSR_PSTATE_STATUS_ADDRESS           0xC0010063ul    ///< P-State Status
#define MSR_PSTATE_DEF_ADDRESS              0xC0010064ul    ///< P-State MSR
#define SIL_RESERVED2_907                   0xC0010074ul
#define MSR_PMGT_MISC_ADDRESS               0xC0010292ul    ///< Power Management Misc
#define SIL_RESERVED2_913                   0xC0010293ul
#define SIL_RESERVED2_919                   0xC0011000ul
#define SIL_RESERVED2_926                   0xC0011022ul
#define SIL_RESERVED_9                      0xC0011023ul
#define SIL_RESERVED2_899                   0xC001102Bul
#define MSR_EX_CFG                          0xC001102Cul
#define SIL_RESERVED2_908                   0xC0011093ul
#define SIL_RESERVED2_922                   0xC0011097ul
#define SIL_RESERVED2_918                   0xC00110DCul
#define SIL_RESERVED2_917                   0xC00110DEul
#define MSR_L2_AA_PAIR_CFG0                 0xC00110E9ul    ///< ChL2AaPairCfg0 L2 Adaptive Allocation Configuration
#define SIL_RESERVED2_912                   0xC0010296ul
#define MSR_CSTATE_ADDRESS                  0xC0010073ul    /// C-state Base Address Register 0xC0010073
#define SIL_RESERVED2_909                   0xC0011074ul
#define SIL_RESERVED2_915                   0xC0011076ul
#define SIL_RESERVED2_901                   0xC0011077ul
#define MSR_LS_RMP_BASE                     0xC0010132ul    ///< Secure Nested Paging Reverse Map Table Base
#define MSR_LS_RMP_END                      0xC0010133ul    ///< Secure Nested Paging Reverse Map Table Limit
#define SIL_RESERVED2_925                   0xC001100Cul
#define MCG_CTL_P                           0x00000100ul    ///< bit 8  for MCG_CTL_P under MSRR
#define DBG_CTL_MSR                         0x000001D9ul
#define MSR_EXTENDED_FEATURE_EN             0xC0000080ul
#define MSR_MC_MISC_L3_THRESHOLD            0xC0000409ul
#define MSR_PATCH_LOADER                    0xC0010020ul
#define MSR_TOM                             0xC001001Aul    ///< TOP_MEM
#define MSR_TOM2                            0xC001001Dul    ///< TOP_MEM2
#define MSR_VM_CR                           0xC0010114ul    ///< Virtual Machine Control
#define MSR_SVM_LOCK_KEY                    0xC0010118ul    ///< SVM Lock Key
#define MSR_CPUID_EXT_FEATS                 0xC0011005ul    ///< CPUID Extended Features
#define MSR_NB_CFG                          0xC001001Ful    ///< NB Config
#define MSR_HTC                             0xC001003Eul
#define MSR_CU_PFTCFG                       0xC001102Ful
#define MSR_IBS_OP_DATA3                    0xC0011037ul
#define MSR_C001_1070                       0xC0011070ul    ///< F15 Shared
#define MSR_CU_CBBCFG                       0xC00110A1ul    ///< F15 CZ
#define MSR_CPUID_NAME_STRING0              0xC0010030ul    ///< First CPUID namestring register
#define MSR_CPUID_NAME_STRING1              0xC0010031ul
#define MSR_CPUID_NAME_STRING2              0XC0010032ul
#define MSR_CPUID_NAME_STRING3              0xC0010033ul
#define MSR_CPUID_NAME_STRING4              0xC0010034ul
#define MSR_CPUID_NAME_STRING5              0xC0010035ul    ///< Last CPUID namestring register
#define MSR_MMIO_Cfg_Base                   0xC0010058ul    // MMIO Configuration Base Address Register
#define MSR_BIST                            0xC0010060ul    ///< BIST Results register
#define MSR_OSVW_ID_Length                  0xC0010140ul
#define MSR_OSVW_Status                     0xC0010141ul
#define MSR_NB_PERF_CTL0                    0xC0010240ul
#define MSR_NB_PERF_CTR0                    0xC0010241ul
#define MSR_NB_PERF_CTL1                    0xC0010242ul
#define MSR_NB_PERF_CTR1                    0xC0010243ul
#define MSR_NB_PERF_CTL2                    0xC0010244ul
#define MSR_NB_PERF_CTR2                    0xC0010245ul
#define MSR_NB_PERF_CTL3                    0xC0010246ul
#define MSR_NB_PERF_CTR3                    0xC0010247ul
#define CU_SPMCTL                           0xC0010281ul    ///< Streaming Performance Monitor Control
#define MSR_PERF_CONTROL3                   0xC0010003ul    ///< Performance control register number 3
#define MSR_PERF_COUNTER3                   0xC0010007ul    ///< Performance counter register number 3
#define MSR_MCA_INTR_CFG                    0xC0000410ul    ///< MCA Interrupt Configuration
#define MSR_MCA_CFG_BANK0                   0xC0002004ul    ///< MCA Config Register 0xC0002xx4
#define MCA_IPID_LS_ADDRESS                 0xC0002005UL    ///< MCA LS IP Identification Register
#define MSR_MCA_CONFIG_IF                   0xC0002014ul
#define MSR_MCA_CONFIG_L2                   0xC0002024ul
#define MSR_MCA_CONFIG_DE                   0xC0002034ul
#define MSR_MCA_CONFIG_EX                   0xC0002054ul
#define MSR_MCA_CONFIG_FP                   0xC0002064ul
#define MCA_CTL_MASK_LS_ADDRESS             0xC0010400ul
#define MCA_CTL_MASK_IF_ADDRESS             0xC0010401ul
#define MCA_CTL_MASK_L2_ADDRESS             0xC0010402ul
#define MCA_CTL_MASK_FP_ADDRESS             0xC0010406ul
#define MSR_MC0_CTL                         0x00000400ul
#define MSR_MC0_STATUS                      0x00000401ul
#define MSR_MC0_MISC                        0x00000403ul
#define MSR_MC1_MISC                        0x00000407ul
#define MSR_MC2_MISC                        0x0000040Bul
#define MSR_MC4_MISC0                       0x00000413ul
#define MSR_MC5_MISC                        0x00000417ul
#define MSR_MC4_MISC1                       0xC0000408ul
#define MSR_MC5_STATUS                      0x00000415ul
#define MSR_MC6_STATUS                      0x00000419ul
#define MSR_MC0_CTL_MASK                    0xC0010044ul   ///< MC0 Control Mask
#define MSR_MC1_CTL_MASK                    0xC0010045ul   ///< MC1 Control Mask
#define MSR_MC2_CTL_MASK                    0xC0010046ul   ///< MC2 Control Mask
#define MSR_MC4_CTL_MASK                    0xC0010048ul   ///< MC4 Control Mask

/**
 * @brief SYSENTER EIP register
 *
 * @details The SYSENTER EIP register (Core::X86::Msr::SYSENTER_EIP) is located in in MSR address 0x176.
 */
#define MSR_SYSENTER_EIP                (0x00000176UL)

/**
 * @brief Global Machine Check Capabilities register
 *
 * @details The Global Machine Check Capabilities register (Core::X86::Msr::MCG_CAP) is located in MSR address 0x179.
 */
#define MSR_MCG_CAP                     (0x00000179UL)

/**
 * @brief Global Machine Check Status register
 *
 * @details The Global Machine Check Status register (Core::X86::Msr::MCG_STAT) is located in MSR address 0x17A.
 */
#define MSR_MCG_STAT                    (0x0000017AUL)
/**
 * @brief The error instruction pointer valid bit in the MCG_STAT register
 *
 * @details BIT1 in the Core::X86::Msr::MCG_STAT register is the error instruction pointer valid bit.
 */
  #define MSR_MCG_EIPV                  BIT_64(1)

/**
 * @brief Hardware Configuration register
 *
 * @details The Hardware Configuration register (Core::X86::Msr::HWCR) is located in MSR address 0xC001_0015.
 */
#define MSR_HWCR                        (0xC0010015UL)
/**
 * @brief The error instruction pointer valid bit in the MCG_STAT register
 *
 * @details BIT18 in the Core::X86::Msr::HWCR register is the machine check status write enable bit.
 */
  #define MCA_STS_WREN_BIT              BIT_64(18)

/**
 * @brief Machine Check Exception Redirection register
 *
 * @details The Machine Check Exception Redirection register (Core::X86::Msr::McExcepRedir)
 *          is located in MSR address 0xC001_0022.
 */
#define MSR_MCEXCEPREDIR                (0xC0010022UL)

/**
 * @brief SMI Trigger IO Cycle register
 *
 * @details The SMI Trigger IO Cycle register (Core::X86::Msr::SmiTrigIoCycle)
 *          is located in MSR address 0xC001_0056.
 */
#define MSR_SMITRIGIOCYCLE              (0xC0010056UL)

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
 * @brief Local SMI Status register.
 *
 * @details The Local SMI Status register (Core::X86::Msr::LocalSmiStatus)
 *          is located in MSR address 0xC001_011A.
 */
#define MSR_LOCAL_SMI_STATUS            (0xC001011AUL)

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
#define MCA_LEGACY_REG_PER_BANK         (1<<2)

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
#define SMCA_REG_PER_BANK               (1<<4)

/**
 * @brief The MSR address of the MCA_CONFIG_UMC register for UMC[8,6,4,2,10,0] instances
 *
 * @details MCA_CONFIG_UMC MSRs are present at MSRC000_2154 corresponding to even instances of UMC (8,6,4,2,10,0)
*/
#define MSR_MCA_CONFIG_UMC_EVEN_INST_ADDRESS              (0xC0002154UL)

/**
 * @brief The MSR address of the MCA_CONFIG_UMC register for UMC[9,7,5,3,1,11] instances
 *
 * @details MCA_CONFIG_UMC MSRs are present at MSRC000_2164 corresponding to odd instances of UMC (9,7,5,3,1,11)
*/
#define MSR_MCA_CONFIG_UMC_ODD_INST_ADDRESS              (0xC0002164UL)

/**
 * @brief The Legacy MCA MSR offset 0x00
 *
 * @details The legacy MCA MSRs are MSR0000_04[7F:00]. The legacy MCA MSR space contains 32 banks
 *          of 4 registers per bank.
 */
//This is old x86 MCA address
#define LMCA_ADDR_REG                   (0x00)
/**
 * @brief The Legacy MCA MSR offset 0x01
 *
 * @details The legacy MCA MSRs are MSR0000_04[7F:00]. The legacy MCA MSR space contains 32 banks
 *          of 4 registers per bank.
 */
#define LMCA_STATUS_REG                 (0x01)
