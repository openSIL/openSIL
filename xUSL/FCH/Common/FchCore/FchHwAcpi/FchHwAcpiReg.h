/**
 * @file  FchHwAcpiReg.h
 * @brief FCH HW ACPI registers definition
 */
/* Copyright 2021-2024 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT


#pragma once

/**********************************************************************************************************************
 * @brief Function prototypes
 *
 */
/**********************************************************************************************************************
 * @brief Module data structure
 *
 */


///
/// _AcpiRegWrite - ACPI MMIO register R/W structure
///
typedef struct _ACPI_REG_WRITE {
  uint8_t        MmioBase;               /// MmioBase: Index of Fch block (For instance GPIO_BASE:0x01 SMI_BASE:0x02)
  uint8_t        MmioReg;                /// MmioReg      : Register index
  uint8_t        DataAndMask;            /// DataANDMask  : AND Register Data
  uint8_t        DataOrMask;             /// DataOrMask   : Or Register Data
} ACPI_REG_WRITE;

/**********************************************************************************************************************
 * @brief Declare macros here
 *
 */

#define ABCFG 6 // ABCFG
#define ACPI_MMIO_BASE 0xFED80000ul
#define AOAC_BASE 0x1E00
#define SHADOW_COUNTER_BASE 0x1100
#define ASF_BASE 0x900 // DWORD
#define CMOS_RAM_BASE 0x600 // BYTE
#define FCH_AOAC_ESPI             0x1B
#define FCH_ABCFG_REG10050 0x10050ul // MISCCTL_10050
#define FCH_AOAC_I2C0 0x05
#define FCH_AOAC_I2C1 0x06
#define FCH_AOAC_I2C2 0x07
#define FCH_AOAC_I2C3 0x08
#define FCH_AOAC_I2C4 0x09
#define FCH_AOAC_I2C5 0x0A
#define FCH_AOAC_REG94 0x94 //
#define FCH_AOAC_UART0 0x0B
#define FCH_AOAC_UART1 0x0C
#define FCH_AOAC_UART2 0x10
#define FCH_AOAC_UART3 0x1A
#define FCH_CFG_REG04 0x004 // Command- RW
#define FCH_CFG_REG11 0x011 //;
#define FCH_CFG_REG2C 0x02C // Subsystem Vendor ID- W
#define FCH_EMMC_CFG_BASE               0xFEDD5800ul
#define FCH_EMMC_CFG_REGBB (FCH_EMMC_CFG_BASE + 0xBB)
#define FCH_GPIO_OUTPUT_ENABLE (1 << 7)
#define FCH_HS_IRQ_EMMC 0x43 // EMMC
#define FCH_HS_IRQ_GPIO 0x62 // GPIO
#define FCH_HS_IRQ_I2C0 0x70 // I2C0
#define FCH_HS_IRQ_I2C1 0x71 // I2C1
#define FCH_HS_IRQ_I2C2 0x72 // I2C2
#define FCH_HS_IRQ_I2C3 0x73 // I2C3
#define FCH_HS_IRQ_I2C4 0x76 // I2C4
#define FCH_HS_IRQ_I2C5 0x77 // I2C5
#define FCH_HS_IRQ_INTA 0x00
#define FCH_HS_IRQ_INTB 0x01
#define FCH_HS_IRQ_INTC 0x02
#define FCH_HS_IRQ_INTD 0x03
#define FCH_HS_IRQ_SD 0x17 // SD
#define FCH_HS_IRQ_SDIO 0x1A // SDIO
#define FCH_HS_IRQ_UART0 0x74 // UART0
#define FCH_HS_IRQ_UART1 0x75 // UART1
#define FCH_HS_IRQ_UART2 0x78 // UART2
#define FCH_HS_IRQ_UART3 0x79 // UART3
#define FCH_IOAPIC_ID_REG 0x00
#define FCH_IOAPIC_INDEX 0xFEC00000
#define FCH_IOAPIC_DATA  0xFEC00010
#define FCH_IOMAP_REG70 0x070 // Nmi_Enable
#define FCH_IOMAP_REG71 0x071 // RtcDataPort
#define FCH_IOMAP_REGC00 0x0C00 // Pci_Intr_Index
#define FCH_IOMAP_REGC01 0x0C01 // Pci_Intr_Data
#define FCH_IOMAP_REGCD7 0x0CD7 // PM_Data
#define FCH_IOMUX_REG15 0x15
#define FCH_IOMUX_REG16 0x16
#define FCH_IOMUX_REG1F 0x1F
#define FCH_IOMUX_REG20 0x20
#define FCH_IOMUX_REG44 0x44
#define FCH_IOMUX_REG46 0x46
#define FCH_IOMUX_REG4A 0x4A
#define FCH_IOMUX_REG4B 0x4B
#define FCH_IOMUX_REG57 0x57
#define FCH_IOMUX_REG58 0x58
#define FCH_IOMUX_REG68 0x68
#define FCH_IOMUX_REG69 0x69
#define FCH_IOMUX_REG6A 0x6A
#define FCH_IOMUX_REG6B 0x6B
#define FCH_IOMUX_REG6D 0x6D
#define FCH_IRQ_IOAPIC 0x80 // Select IRQ routing to IoApic mode
#define FCH_ISA_BUS 0
#define FCH_ISA_DEV 20
#define FCH_ISA_FUNC 0
#define FCH_KBC_RESET_COMMAND 0xFE
#define FCH_KBDRST_BASE_IO 0x64
#define FCH_MISC_REG50 0x50 //
#define FCH_MISC_REG6C 0x6C // EcoBit2
#define FCH_MISC_REG80 0x80 // FCH_MISC_REG80
#define FCH_PCIRST_BASE_IO 0xCF9
#define FCH_PCI_RESET_COMMAND06 0x06
#define FCH_PCI_RESET_COMMAND0E 0x0E
#define FCH_PMIOA_REG00 0x00 // ISA Decode
#define FCH_PMIOA_REG04 0x04 // ISA Control
#define FCH_PMIOA_REG08 0x08 // PCI Control
#define FCH_PMIOA_REG10 0x10 // RetryDetect
#define FCH_PMIOA_REG20 0x20 // BiosRamEn
#define FCH_PMIOA_REG3C 0x3C // BiosRamEn
#define FCH_PMIOA_REG44 0x44 // BootTimerEn
#define FCH_PMIOA_REG54 0x54 // SerialIrqConfig
#define FCH_PMIOA_REG5B 0x5B // RtcShadow
#define FCH_PMIOA_REG5E 0x5E // RtcExtIndex
#define FCH_PMIOA_REG5F 0x5F // RtcExtData
#define FCH_PMIOA_REG60 0x60 // AcpiPm1EvtBlk
#define FCH_PMIOA_REG62 0x62 // AcpiPm1CntBlk
#define FCH_PMIOA_REG64 0x64 // AcpiPmTmrBlk
#define FCH_PMIOA_REG66 0x66 // P_CNTBlk
#define FCH_PMIOA_REG68 0x68 // AcpiGpe0Blk
#define FCH_PMIOA_REG6A 0x6A // AcpiSmiCmd
#define FCH_PMIOA_REG6C 0x6C // AcpiPm2CntBlk
#define FCH_PMIOA_REG6E 0x6E // AcpiPmaCntBlk
#define FCH_PMIOA_REG74 0x74 // AcpiConfig
#define FCH_PMIOA_REG90 0x90 //
#define FCH_PMIOA_REGA0 0xA0 // MessageCState
#define FCH_PMIOA_REGBA 0xBA // S_StateControl
#define FCH_PMIOA_REGBC 0xBC // ThrottlingControl
#define FCH_PMIOA_REGBE 0xBE // ResetControl
#define FCH_PMIOA_REGC0 0xC0 // S5Status
#define FCH_PMIOA_REGC4 0xC4 // ResetCommand
#define FCH_PMIOA_REGC8 0xC8 // Misc
#define FCH_PMIOA_REGD2 0xD2 // PmioDebug
#define FCH_PMIOA_REGDC 0xDC // PMIO_XDC_SATACONFIG
#define FCH_PMIOA_REGED 0xED
#define FCH_REGE0 0xE0
#define FCH_SDB_REG00 0x00
#define FCH_SMI_Gevent1 0x41 // SciMap1
#define FCH_SMI_Gevent23 0x57 // SciMap23
#define FCH_SMI_Gevent3 0x43 // SciMap3
#define FCH_SMI_Gevent4 0x44 // SciMap4
#define FCH_SMI_Gevent5 0x45 // SciMap5
#define FCH_SMI_Gevent6 0x46 // SciMap6
#define FCH_SMI_Gevent8 0x48 // SciMap8
#define FCH_SMI_HS_SCIMAP0      0x40
#define FCH_SMI_HS_FANTHGEVENT (FCH_SMI_HS_SCIMAP0 + 44) // SciMap44
#define FCH_SMI_HS_NBGPPHP (FCH_SMI_HS_SCIMAP0 + 30) // SciMap30
#define FCH_SMI_HS_NBGPPPME (FCH_SMI_HS_SCIMAP0 + 29) // SciMap29
#define FCH_SMI_HS_TWRN (FCH_SMI_HS_SCIMAP0 + 48) // SciMap48
#define FCH_SMI_HS_XHC0WAKE (FCH_SMI_HS_SCIMAP0 + 56) // SciMap56
#define FCH_SMI_HS_XHC1WAKE (FCH_SMI_HS_SCIMAP0 + 57) // SciMap57
#define FCH_SMI_REG00 0x00 // EventStatus
#define FCH_SMI_REG04 0x04 // EventEnable
#define FCH_SMI_REG08 0x08 // SciTrig
#define FCH_SMI_REG0C 0x0C // SciLevl
#define FCH_SMI_REG18 0x18 // ForceSciEn
#define FCH_SMI_REGB0 0xB0
#define FCH_SMI_REGB4 0xB4
#define GPIO_BANK0_BASE 0x1500 // DWORD
#define GPIO_BANK1_BASE 0x1600 // DWORD
#define IOMUX_BASE 0xD00 // BYTE
#define MISC_BASE 0xE00
#define PMIO_BASE 0x300 // DWORD
#define S0I3CTRL_ARBITERDIS 1 << 14
#define S0I3CTRL_INTERRUPTDIS 1 << 15
#define SERIAL_DEBUG_BASE 0x1000
#define SMBUS_BUS_DEV_FUN ((0x14 << 3) + 0)
#define SMI_BASE 0x200 // DWORD
#define FCH_BP_AGPIO3_SPD_HOST_CTRL_L 0xC
