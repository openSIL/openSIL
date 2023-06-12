/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  RasReg.h
 * @brief RAS common register descriptions and addresses (internal)
 */

#pragma once


///UMC
/**
 * Reg(MCA::UMC::MCA_CTL_UMC)
 *
 * @brief RAS UMC EXT_ERROR_CODE enum
 * @details
 */
typedef enum {
  UMC_DRAMECCERR = 0,                       ///< DRAM ECC error.
  UMC_WRITEDATAPOISONERR,                   ///< Data poison error.
  UMC_SDPPARITYERR,                         ///< SDP parity error.
  UMC_APBERR,                               ///< Advanced peripheral bus error.
  UMC_ADDRCMDPARITYERR,                     ///< Address/command parity error.
  UMC_WRITEDATACRCERR,                      ///< Write data CRC error.
  UMC_DCQSRAMECCERR,                        ///< An ECC error occurred on a DCQ SRAM.
  UMC_AESSRAMECCERR,                        ///< An ECC error occurred on a AES SRAM.
  UMC_ECSROWERR,                            ///< A single device row exceeded four code word errors.
  UMC_ECSERR,                               ///< A device exceeded the ECS Error Threshold Count.
  UMC_THRTTLERR,                            ///< UMC is throttling.
  UMC_RDCRCERR,                             ///< Read CRC error. CRC error occurred on a DRAM read from any subchannel.
  UMC_UNUSED0,
  UMC_UNUSED1,
  UMC_USUSED2,
  UMC_UNUSED3,
  UMC_RFMSRAMECCERR
} UMC_EXT_ERROR_CODE_NUM;

/**
 * @brief The SMN address of UMC00CHx00000000 of UMC Controller
 *
 * @details This is the address of [DRAM CS Base Address] (UMC::BaseAddr)@ UMC0
 */
#define UMC0_CH_REG_BASE                    (0x00050000)

/**
 * @brief The SMN address offset of UMC_x_CH
 *
 * @details The SMN address offset of UMC_x_CH is 0x100000, based on the previous UMC_(x-1)_CH SMN address
 *          where x = 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
 *          e.g The SMN address of UMC00CH = 0x00050000
 *                             =>  UMC01CH = UMC00CH + UMC_SMN_ADDR_OFFSET = 0x100000 = 0x00150000
 *                             =>  UMC02CH = UMC01CH + UMC_SMN_ADDR_OFFSET = 0x100000 = 0x00250000
 *                             =>  UMC03CH = UMC02CH + UMC_SMN_ADDR_OFFSET = 0x100000 = 0x00350000
 *                             =>  UMC04CH = UMC03CH + UMC_SMN_ADDR_OFFSET = 0x100000 = 0x00450000
 *                             =>  UMC05CH = UMC04CH + UMC_SMN_ADDR_OFFSET = 0x100000 = 0x00550000
 *                             =>  UMC06CH = UMC05CH + UMC_SMN_ADDR_OFFSET = 0x100000 = 0x00650000
 *                             =>  UMC07CH = UMC06CH + UMC_SMN_ADDR_OFFSET = 0x100000 = 0x00750000
 *                             =>  UMC08CH = UMC07CH + UMC_SMN_ADDR_OFFSET = 0x100000 = 0x00850000
 *                             =>  UMC09CH = UMC08CH + UMC_SMN_ADDR_OFFSET = 0x100000 = 0x00950000
 *                             =>  UMC10CH = UMC09CH + UMC_SMN_ADDR_OFFSET = 0x100000 = 0x00A50000
 *                             =>  UMC11CH = UMC10CH + UMC_SMN_ADDR_OFFSET = 0x100000 = 0x00B50000
 */
#define UMC_SMN_ADDR_OFFSET                 (0x100000)


/**
 * @brief The SMN address offset of [McaFruText DW0] (UMC::McaFruTextDW0)@Dimm0, based on UMC::BaseAddr@UNC00
 *
 * @details This is the SMN address offset of [McaFruText DW0] (UMC::McaFruTextDW0).
 *          e.g. UMC::EccErrInjCtrl@UMC0 = 0x00050C80
 */
#define UMC_MCAFRUTEXTDW0_DIMM0             (0xC80)
/**
 * @brief The SMN address offset of [McaFruText DW0] (UMC::McaFruTextDW0)@Dimm1, based on UMC::BaseAddr
 *
 * @details This is the SMN address offset of [McaFruText DW0] (UMC::McaFruTextDW0).
 *          e.g. UMC::EccErrInjCtrl@UMC0 = 0x00050C90
 */
#define UMC_MCAFRUTEXTDW0_DIMM1             (0xC90)


/**
 * @brief SMCA_REG_OFFSET_MASK is used to obtain the offset of the MCA register in an MCAX Bank.
 *
 * @details Any MSR located in MSRC000_2[3FF:000] can be 'Bitwise AND' by SMCA_REG_OFFSET_MASK to get its
 *          MCA register offset.
 *          e.g. (MSRC0002001 & SMCA_REG_OFFSET_MASK) = 0x01 = MCA_STATUS_OFFSET
 *          MCA register offset in an MCAX Bank:
 *          0x00: MCA_CTL,   0x01: MCA_STATUS, 0x02: MCA_ADDR,   0x03: MCA_MISC0,  0x04: MCA_CONFIG
 *          0x05: MCA_IPID,  0x06: MCA_SYND,   0x08: MCA_DESTAT, 0x09: MCA_DESTAT, 0x0A: MCA_MISC1
 *          0x0E: MCA_SYND1, 0x0F: MCA_SYND2
 */
#define SMCA_REG_OFFSET_MASK            (0x0000000F)
#define MCA_STATUS_OFFSET               (0x01)
#define MCA_ADDR_OFFSET                 (0x02)
#define MCA_MISC0_OFFSET                (0x03)
#define MCA_CONFIG_OFFSET               (0x04)
#define MCA_IPID_OFFSET                 (0x05)
#define MCA_SYND_OFFSET                 (0x06)
#define MCA_DESTAT_OFFSET               (0x08)
#define MCA_DEADDR_OFFSET               (0x09)
#define MCA_MISC1_OFFSET                (0x0A)
#define MCA_SYND1_OFFSET                (0x0E)
#define MCA_SYND2_OFFSET                (0x0F)

//MCA Hardware ID
/**
 * @brief The MCA Hardware ID and MCA TYPE of CPU CORE Block
 *
 * @details To determine which type of block is mapped to an MCA bank, software can query the MCA_IPID register
 *          within that bank.
 *          MCA_IPID[HardwareID] provides the block type for the block that contains this MCA bank.
 *          For blocks that contain multiple MCA bank types (e.g., CPU cores), MCA_IPID[McaType] provides
 *          an identifier for the type of MCA bank.
 */
#define MCA_CPU_CORE_ID                 (0x0B0)
  #define IF_MCA_TYPE                   (0x0001)  ///< Instruction Fetch Unit
  #define L2_MCA_TYPE                   (0x0002)  ///< L2 Cache Unit
  #define DE_MCA_TYPE                   (0x0003)  ///< Decode Unit
  #define EX_MCA_TYPE                   (0x0005)  ///< Execution Unit
  #define FP_MCA_TYPE                   (0x0006)  ///< Floating-Point Unit

/**
 * @brief The MCA Hardware ID and MCA TYPE of DATA FABRIC Block
 *
 * @details Please refer to MCA_CPU_CORE_ID
 */
  #define CS_MCA_TYPE                   (0x0002)  ///< Coherent Slave

#define CS_MCA_BANK_HARDWARE_ID         0x2E ///< hardware id of CS
                                             ///< (coherent slave) block
                                             ///< which is capable of MCA
                                             ///< banks

/**
 * @brief The MCA Hardware ID of UMC Block
 *
 * @details To determine which type of block is mapped to an MCA bank, software can query the MCA_IPID register
 *          within that bank.
 *          MCA_IPID[HardwareID] provides the block type for the block that contains this MCA bank.
 *          For blocks that contain multiple MCA bank types (e.g., CPU cores), MCA_IPID[McaType] provides
 *          an identifier for the type of MCA bank.
 *          The HardwareID of MCA UMC block is 0x096.
 */
#define MCA_UMC_ID                      (0x096)

/**
 * @brief SMRAM State-Save Area
 *
 * @details When an SMI occurs, the processor saves its state in the 512-byte SMRAM state-save area during the
 *          control transfer into SMM.
 */
//SMM Save State Structure
typedef struct {
  uint64_t   ES[2];                           //FE00
  uint64_t   CS[2];                           //FE10
  uint64_t   SS[2];                           //FE20
  uint64_t   DS[2];                           //FE30
  uint64_t   FS[2];                           //FE40
  uint64_t   GS[2];                           //FE50
  uint64_t   GDTR[2];                         //FE60
  uint64_t   LDTR[2];                         //FE70
  uint64_t   IDTR[2];                         //FE80
  uint64_t   TR[2];                           //FE90
  uint64_t   IO_RESTAERT_RIP;                 //FEA0
  uint64_t   IO_RESTAERT_RCX;                 //FEA8
  uint64_t   IO_RESTAERT_RSI;                 //FEB0
  uint64_t   IO_RESTAERT_RDI;                 //FEB8
  uint32_t   TrapoFFSET;                      //FEC0
  uint32_t   LocalSmiStatus;                  //FEC4
  uint8_t    IoRestart;                       //FEC8
  uint8_t    AutoHalt;                        //FEC9
  uint8_t    NmiMask;                         //FECA
  uint8_t    Reserved1[5];                    //FECB
  uint64_t   EFER;                            //FED0
  uint64_t   SvmState;                        //FED8
  uint64_t   GuestVMCBPyysicalAddress;        //FEE0
  uint64_t   SVMVirtualInterruptControl;      //FEE8
  uint8_t    Reserved2[12];                   //FEF0
  uint32_t   SMMRevId;                        //FEFC
  uint32_t   SMBASE;                          //FF00
  uint8_t    Reserved3[28];                   //FF04
  uint64_t   GuestPAT;                        //FF20
  uint64_t   HostEFER;                        //FF28
  uint64_t   HostCR4;                         //FF30
  uint64_t   NestedCR3;                       //FF38
  uint64_t   HostCR0;                         //FF40
  uint64_t   CR4;                             //FF48
  uint64_t   CR3;                             //FF50
  uint64_t   CR0;                             //FF58
  uint64_t   DR7;                             //FF60
  uint64_t   DR6;                             //FF68
  uint64_t   RFLAGS;                          //FF70
  uint64_t   RIP;                             //FF78
  uint64_t   R15;                             //FF80
  uint64_t   R14;                             //FF88
  uint64_t   R13;                             //FF90
  uint64_t   R12;                             //FF98
  uint64_t   R11;                             //FFA0
  uint64_t   R10;                             //FFA8
  uint64_t   R9;                              //FFB0
  uint64_t   R8;                              //FFB8
  uint64_t   RDI;                             //FFC0
  uint64_t   RSI;                             //FFC8
  uint64_t   RBP;                             //FFD0
  uint64_t   RSP;                             //FFD8
  uint64_t   RBX;                             //FFE0
  uint64_t   RDX;                             //FFE8
  uint64_t   RCX;                             //FFF0
  uint64_t   RAX;                             //FFF8
} SMM_SAVE_STATE;

/**
 * @brief The base offset of SMRAM state-save area
 *
 * @details The top 512 bytes (FE00h to FFFFh) of SMRAM memory space are the default SMM state-save area.
 *          When an SMI occurs, the processor saves its state in the 512-byte SMRAM state-save area during the
 *          control transfer into SMM.
 */
#define SMM_SAVE_STATE_OFFSET           (0xFE00)
