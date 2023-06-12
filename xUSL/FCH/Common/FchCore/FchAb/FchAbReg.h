/**
 * @file  FchAbReg.h
 * @brief FCH AB register definitions
 *
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

/**********************************************************************************************************************
 * Function prototypes
 *
 */

/**********************************************************************************************************************
 * Module data structure
 *
 */

/**********************************************************************************************************************
 * Declare macros here
 *
 */
#define ABCFG                       6                      ///< ABCFG
#define ACPI_MMIO_BASE              0xFED80000ul           ///< ACPI Memory mapped IO base address
#define ALINK_ACCESS_DATA           ALINK_ACCESS_INDEX + 4 ///< ALink indirect data address
#define ALINK_ACCESS_INDEX          0x0CD8      ///< ALink indirect index  address
#define AXINDC                      0
#define AXINDP                      2
#define FCH_ABCFG_REG10050          0x10050ul   ///< Misc Control 3 (FCH::AB::MiscCtl2)
#define FCH_ABCFG_REG10054          0x10054ul   ///< A-Link Arbitration Control and Clock Control (FCH::AB::AL_Arb_Ctl)
#define FCH_ABCFG_REG10060          0x10060ul   ///< A-Link DMA Prefetch Enable (FCH::AB::ALinkDmaPrefetchEn)
#define FCH_ABCFG_REG10090          0x10090ul   ///< Misc Control 3 (FCH::AB::MiscCtl3)
#define SIL_RESERVED_23             0x180
#define SIL_RESERVED_24             0x184
#define FCH_ABCFG_REG208            0x208       ///< SDP Misc Control (FCH::SDP::SDP_MISC_CTL)
#define FCH_ABCFG_REG228            0x228       ///< SDP Disconnect Control (FCH::SDP::SDP_DISCONNECT_CTL)
#define SIL_RESERVED_25             0x22C
#define SIL_RESERVED_26             0x238
#define FCH_ABCFG_REG54             0x54        ///< Misc Control 1 (FCH::AB::MiscCtl1)
#define FCH_ABCFG_REG58             0x58        ///< B-Link RAB Control (FCH::AB::BLinkRABCtl)
#define FCH_ABCFG_REG90             0x90        ///< BIF Control 0 (FCH::AB::BIFCtl0)
#define FCH_ABCFG_REG94             0x94        ///< MSI Control (FCH::AB::MsiCtl)
#define SIL_RESERVED_27             0xBC
#define FCH_AX_INDXP_REG38          0x38
#define FCH_SMN_RSMU_BASE           0x02D00000ul ///< FCH RSMU base address
#define FCH_MISC_REG2C              0x2C         ///< FCH::MISC::CLKGATEDCNTL
#define FCH_MISC_REG40              0x40         ///< MiscCntrl for clock only
#define FCH_PMIOA_REG04             0x04         ///< ISA Control
#define FCH_PMIOA_REGE0             0xE0         ///< ABRegBar
#define MISC_BASE                   0xE00
#define PMIO_BASE                   0x300        ///< Power management base register
