/**
 * @file  RasReg.h
 * @brief RAS common register descriptions and addresses (internal)
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once


///UMC
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

typedef union {
  struct {
     uint32_t WrEccEn:1;                ///< Enables ECC generation for DRAM data.
     uint32_t :3;                       ///< Reserved
     uint32_t EccBadDramSymEn:1;        ///< Enables the software managed ECC history mechanism for x8 symbol size.
     uint32_t EccHardwareHistoryEn:1;   ///< Enables the hardware managed ECC history mechanism for x8 symbol size.
     uint32_t EccBitInterleaving:1;     ///<
     uint32_t EccSymbolSize:1;          ///< 0=x4 symbol. 1=x8 symbol.
     uint32_t UCFatalEn:1;              ///< Promote uncorrectable errors from deferred to fatal.
     uint32_t EccSymbolSize16:1;        ///< 1=x16 symbol. 0=use EccSymbolSize
     uint32_t RdEccEn:1;                ///< Enable DRAM data ECC checking and correction.
     uint32_t :21;                      ///< Reserved
  } Field;
  uint32_t  Value;
} UMC_ECCCTRL_REG;

 /**
 * @brief The SMN address offset of [DRAM ECC Control] (UMC::EccCtrl), based on UMC::BaseAddr
 *
 * @details This is the SMN address offset of [DRAM ECC Control] (UMC::EccCtrl). e.g. UMC::EccCtrl@UMC0 = 0x0005014C
 */
#define UMC_ECC_CTRL                        (0x0000014C)

 /**
 * @brief The SMN address offset of [McaFruText DW0] (UMC::McaFruTextDW0)@Dimm0, based on UMC::BaseAddr@UNC00
 *
 * @details This is the SMN address offset of [McaFruText DW0] (UMC::McaFruTextDW0).
 *          e.g. UMC::EccErrInjCtrl@UMC0 = 0x00050C80
 */
#define UMC_MCAFRUTEXTDW0_DIMM0             (0xC80)
#define UMC_MCAFRUTEXTDW0_DIMM1             (0xC90)

#define SIL_RESERVED_795           0x3FUL
#define SIL_RESERVED_794           0x80000000UL

typedef union {
  struct {
    uint32_t                                                   Index: 6;
    uint32_t                                           Reserved_30_6:25;
    uint32_t                                                 AutoInc: 1;
  } Field;
  uint32_t Value;
} SIL_RESERVED_6;

#define SIL_RESERVED_805     0x50d80UL
#define SIL_RESERVED_806     0x2
#define SIL_RESERVED_785     0x4
#define SIL_RESERVED_802     0x8
#define SIL_RESERVED_788     0xffffUL
#define SIL_RESERVED_789     0xffff0000UL

typedef union {
  struct {
    uint32_t   Reserved1:16;
    uint32_t   Reserved2:16;
  } Field;
  uint32_t Value;
} SIL_RESERVED_7;

#define SIL_RESERVED_803     0x50d84UL
#define SIL_RESERVED_790     0x01UL
#define SIL_RESERVED_792     0x02UL
#define SIL_RESERVED_791     0x30UL
#define SIL_RESERVED_793     0x1F00UL

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
#define MCA_CTL_OFFSET                  (0x00)
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

// Syndrome Error Priority
/**
 * @brief ErrorPriority field of MCA_SYND register
 *
 * @details The ErrorPriority (Bit 26:24) encodes the priority of the error logged in MCA_SYND register
 *          3'b000 = 0x00 = No error
 *          3'b001 = 0x01 = Reserved
 *          3'b010 = 0x02 = Corrected Error
 *          3'b011 = 0x03 = Deferred Error
 *          3'b100 = 0x04 = Uncorrected Error
 *          3'b101 = 0x05 = Fatal Error
 *          all others reserved.
 */
#define MCA_SYND_ERROR_PRIORITY_NO_ERROR    0x00
#define MCA_SYND_ERROR_PRIORITY_RESERVED    0x01
#define MCA_SYND_ERROR_PRIORITY_CORRECTED   0x02
#define MCA_SYND_ERROR_PRIORITY_DEFERRED    0x03
#define MCA_SYND_ERROR_PRIORITY_UNCORRECTED 0x04
#define MCA_SYND_ERROR_PRIORITY_FATAL       0x05

// Syndrome Types
/**
 * @brief Type field of MCA_SYND register
 *
 * @details The Type (Bit 31:28) specifies the type of error contained in MCA_SYND register
 *          4'b0000         = 0x00 = DRAM error
 *          4'b0001         = 0x01 = Cache error
 *          4'b0010-4'b1111 = 0x02 = Other error
 */
#define MCA_SYND_TYPE_DRAM_ERR     0x00
#define MCA_SYND_TYPE_CACHE_ERR    0x01
#define MCA_SYND_TYPE_INTERNAL_ERR 0x02

/**
 * @brief The bank number of an unused MCAX bank
 *
 * @details MCAX Bank 4 is always Read-as-zero/Writes Ignored (RAZ/WRIG). Access to unused MCAX MSRs is RAZ/WRIG.
 */
#define MCA_EMPTY0_BANK                 (4)

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
  #define LS_MCA_TYPE                   (0x0010)  ///< Load-Store Unit
  #define IF_MCA_TYPE                   (0x0001)  ///< Instruction Fetch Unit
  #define L2_MCA_TYPE                   (0x0002)  ///< L2 Cache Unit
  #define DE_MCA_TYPE                   (0x0003)  ///< Decode Unit
  #define EX_MCA_TYPE                   (0x0005)  ///< Execution Unit
  #define FP_MCA_TYPE                   (0x0006)  ///< Floating-Point Unit
  #define L3_MCA_TYPE                   (0x0007)  ///< L3 Cache Unit

/**
 * @brief The MCA Hardware ID and MCA TYPE of DATA FABRIC Block
 *
 * @details Please refer to MCA_CPU_CORE_ID
 */
#define MCA_DATA_FABRIC_ID              (0x02E)
  #define CS_MCA_TYPE                   (0x0002)  ///< coherent station
  #define PIE_MCA_TYPE                  (0x0001)  ///< Power Management, Interrupts, Etc.

#define CS_MCA_BANK_HARDWARE_ID         0x2E ///< hardware id of CS
                                             ///< (coherent station) block
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
 * @brief The MCA TYPE of UMC Block
 *
 * @details To determine which type of block is mapped to an MCA bank, software can query the MCA_IPID register
 *          within that bank.
 *          MCA_IPID[HardwareID] provides the block type for the block that contains this MCA bank.
 *          For blocks that contain multiple MCA bank types (e.g., CPU cores), MCA_IPID[McaType] provides
 *          an identifier for the type of MCA bank.
 *          The McaType of MCA UMC block is 0x0000.
 */
  #define UMC_MCA_TYPE                  (0x0000)  ///< Unified Memory Controller
/**
 * @brief The MCA InstanceId of UMC Controller 0
 *
 * @details This is a unique ID per instance of the MCA UMC Bank.
 *          The InstanceId of UMC Controller 0 is 0x050F00.
 */
  #define UMC0_MCA_INS_ID               (0x050F00)
/**
 * @brief The MCA InstanceId of UMC Controller 1
 *
 * @details This is a unique ID per instance of the MCA UMC Bank.
 *          The InstanceId of UMC Controller 1 is 0x150F00.
 */
  #define UMC1_MCA_INS_ID               (0x150F00)
/**
 * @brief The MCA InstanceId of UMC Controller 2
 *
 * @details This is a unique ID per instance of the MCA UMC Bank.
 *          The InstanceId of UMC Controller 2 is 0x250F00.
 */
  #define UMC2_MCA_INS_ID               (0x250F00)
/**
 * @brief The MCA InstanceId of UMC Controller 3
 *
 * @details This is a unique ID per instance of the MCA UMC Bank.
 *          The InstanceId of UMC Controller 3 is 0x350F00.
 */
  #define UMC3_MCA_INS_ID               (0x350F00)
/**
 * @brief The MCA InstanceId of UMC Controller 4
 *
 * @details This is a unique ID per instance of the MCA UMC Bank.
 *          The InstanceId of UMC Controller 4 is 0x450F00.
 */
  #define UMC4_MCA_INS_ID               (0x450F00)
/**
 * @brief The MCA InstanceId of UMC Controller 5
 *
 * @details This is a unique ID per instance of the MCA UMC Bank.
 *          The InstanceId of UMC Controller 5 is 0x550F00.
 */
  #define UMC5_MCA_INS_ID               (0x550F00)
/**
 * @brief The MCA InstanceId of UMC Controller 6
 *
 * @details This is a unique ID per instance of the MCA UMC Bank.
 *          The InstanceId of UMC Controller 6 is 0x650F00.
 */
  #define UMC6_MCA_INS_ID               (0x650F00)
/**
 * @brief The MCA InstanceId of UMC Controller 7
 *
 * @details This is a unique ID per instance of the MCA UMC Bank.
 *          The InstanceId of UMC Controller 7 is 0x750F00.
 */
  #define UMC7_MCA_INS_ID               (0x750F00)
/**
 * @brief The MCA InstanceId of UMC Controller 8
 *
 * @details This is a unique ID per instance of the MCA UMC Bank.
 *          The InstanceId of UMC Controller 8 is 0x850F00.
 */
  #define UMC8_MCA_INS_ID               (0x850F00)
/**
 * @brief The MCA InstanceId of UMC Controller 9
 *
 * @details This is a unique ID per instance of the MCA UMC Bank.
 *          The InstanceId of UMC Controller 9 is 0x950F00.
 */
  #define UMC9_MCA_INS_ID               (0x950F00)
/**
 * @brief The MCA InstanceId of UMC Controller 10
 *
 * @details This is a unique ID per instance of the MCA UMC Bank.
 *          The InstanceId of UMC Controller 10 is 0xA50F00.
 */
  #define UMCA_MCA_INS_ID               (0xA50F00)
/**
 * @brief The MCA InstanceId of UMC Controller 11
 *
 * @details This is a unique ID per instance of the MCA UMC Bank.
 *          The InstanceId of UMC Controller 11 is 0xB50F00.
 */
  #define UMCB_MCA_INS_ID               (0xB50F00)

/**
 * @brief The MCA Hardware ID and MCA TYPE of Parameter Block
 *
 * @details Please refer to MCA_CPU_CORE_ID
 */
#define MCA_PARAMETER_BLOCK_ID          (0x005)
  #define PB_MCA_TYPE                   (0x0000)  ///< Parameter Block
  #define PB0_MCA_INS_ID                (0x32082900)
  #define PB1_MCA_INS_ID                (0x34082900)
  #define PB2_MCA_INS_ID                (0x36082900)
  #define PB3_MCA_INS_ID                (0x38082900)
  #define PB4_MCA_INS_ID                (0x3A082900)
  #define PB5_MCA_INS_ID                (0x3C082900)
  #define PB6_MCA_INS_ID                (0x3E082900)
  #define PB7_MCA_INS_ID                (0x4A082900)
  #define PB8_MCA_INS_ID                (0x4C082900)
  #define PB9_MCA_INS_ID                (0x4E082900)
  #define PB10_MCA_INS_ID               (0x50082900)

/**
 * @brief The MCA Hardware ID and MCA TYPE of Platform Security Processor Block
 *
 * @details Please refer to MCA_CPU_CORE_ID
 */
#define MCA_PSP_ID                      (0x0FF)
  #define PSP_MCA_TYPE                  (0x0001)  ///< Platform Security Processor

/**
 * @brief The MCA Hardware ID and MCA TYPE of System Management Controller Unit Block
 *
 * @details Please refer to MCA_CPU_CORE_ID
 */
#define MCA_SMU_ID                      (0x001)
  #define SMU_MCA_TYPE                  (0x0001)  ///< System Management Controller Unit
  #define MP5_MCA_TYPE                  (0x0002)  ///< Microprocessor5 Management Controller
  #define MPDMA_MCA_TYPE                (0x0003)  ///< DMA Engine Controller
  #define MP50_MCA_INS_ID               (0x32430400)
  #define MP51_MCA_INS_ID               (0x34430400)
  #define MP52_MCA_INS_ID               (0x36430400)
  #define MP53_MCA_INS_ID               (0x38430400)
  #define MP54_MCA_INS_ID               (0x3A430400)
  #define MP55_MCA_INS_ID               (0x3C430400)
  #define MP56_MCA_INS_ID               (0x3E430400)
  #define MP57_MCA_INS_ID               (0x4A430400)
  #define MP58_MCA_INS_ID               (0x4C430400)
  #define MP59_MCA_INS_ID               (0x4E430400)
  #define MP510_MCA_INS_ID              (0x50430400)

/**
 * @brief The MCA Hardware ID and MCA TYPE of Northbridge IO Unit Block
 *
 * @details Please refer to MCA_CPU_CORE_ID
 */
#define MCA_NBIO_ID                     (0x0018)
  #define NBIO_MCA_TYPE                 (0x0000)  ///< Northbridge IO Unit
  #define NBIO0_IOHC0_MCA_INS_ID        (0x13B17000)
  #define NBIO0_IOHC1_MCA_INS_ID        (0x13C17000)
  #define NBIO1_IOHC0_MCA_INS_ID        (0x13D17000)
  #define NBIO1_IOHC1_MCA_INS_ID        (0x13E17000)

/**
 * @brief The MCA Hardware ID and MCA TYPE of PCIe Root Port Block
 *
 * @details Please refer to MCA_CPU_CORE_ID
 */
#define MCA_PCIE_ID                     (0x0046)
  #define PCIE_MCA_TYPE                 (0x0001)  ///< PCIe Root Port
  #define PCIE0_MCA_INS_ID              (0x1A4C0000)
  #define PCIE1_MCA_INS_ID              (0x1A6C0000)
  #define PCIE2_MCA_INS_ID              (0x1A7C0000)
  #define PCIE3_MCA_INS_ID              (0x1A9C0000)
  #define PCIE4_MCA_INS_ID              (0x1A8C0000)
  #define PCIE5_MCA_INS_ID              (0x1AAC0000)
  #define PCIE6_MCA_INS_ID              (0x1ABC0000)
  #define PCIE7_MCA_INS_ID              (0x1ADC0000)

/**
 * @brief The MCA Hardware ID and MCA TYPE of High Speed Interface Unit (XGMI) Block
 *
 * @details Please refer to MCA_CPU_CORE_ID
 */
#define MCA_KPX_SERDES_ID               (0x0259)
  #define KPX_SERDES_MCA_TYPE           (0x0000)

/**
 * @brief The MCA Hardware ID and MCA TYPE of On-chip PCIe Bus Interface Block
 *
 * @details Please refer to MCA_CPU_CORE_ID
 */
#define MCA_NBIF_ID                     (0x006C)
  #define NBIF_MCA_TYPE                 (0x0000)

/**
 * @brief The MCA Hardware ID and MCA TYPE of GMI Controller Block
 *
 * @details Please refer to MCA_CPU_CORE_ID
 */
#define MCA_PCS_GMI_ID                  (0x0241)
  #define PCS_GMI_MCA_TYPE              (0x0000)

/**
 * @brief The MCA Hardware ID and MCA TYPE of XGMI Controller Block
 *
 * @details Please refer to MCA_CPU_CORE_ID
 */
#define MCA_PCS_XGMI_ID                 (0x0050)
  #define PCS_XGMI_MCA_TYPE             (0x0000)

/**
 * @brief The MCA Hardware ID and MCA TYPE of System Hub Block
 *
 * @details Please refer to MCA_CPU_CORE_ID
 */
#define MCA_SHUB_ID                     (0x0080)
  #define SHUB_MCA_TYPE                 (0x0000)

/**
 * @brief The MCA Hardware ID and MCA TYPE of High Speed Interface Unit (GMI) Block
 *
 * @details Please refer to MCA_CPU_CORE_ID
 */
#define MCA_KPX_GMI_ID                  (0x0269)
  #define KPX_GMI_MCA_TYPE              (0x0000)

/**
 * @brief The MCA Hardware ID and MCA TYPE of SATA Controller Block
 *
 * @details Please refer to MCA_CPU_CORE_ID
 */
#define MCA_SATA_ID                     (0x00A8)
  #define SATA_MCA_TYPE                 (0x0000)

/**
 * @brief The MCA Hardware ID and MCA TYPE of USB Controller Block
 *
 * @details Please refer to MCA_CPU_CORE_ID
 */
#define MCA_USB_ID                      (0x00AA)
  #define USB_MCA_TYPE                  (0x0000)

/**
 * @brief The MCA Hardware ID and MCA TYPE of High Speed Interface Unit (WAFL) Block
 *
 * @details Please refer to MCA_CPU_CORE_ID
 */
#define MCA_KPX_WAFL_ID                 (0x0267)
  #define KPX_WAFL_MCA_TYPE             (0x0000)

/**
 * @brief The MCA Hardware ID and MCA TYPE of IO Microprocessor Block
 *
 * @details Please refer to MCA_CPU_CORE_ID
 */
#define MCA_MPIO_ID                     (0x0135)
  #define MPIO_MCA_TYPE                 (0x0000)

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
