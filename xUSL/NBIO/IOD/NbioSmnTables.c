/**
 * @file  NbioSmnTable.c
 *
 * This file contains Global configuration structures for Nbio Initialization
 *
 */

/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include  <xSIM.h>
#include  <SilPcie.h>
#include  "NbioSmnTable.h"
#include  "GnbRegisters.h"
#include  "NbioIohcTbl.h"
#include  "NbioIoapicTbl.h"
#include  "NbioIommuTbl.h"
#include  "NbioNbifTbl.h"
#include  "NbioWorkaroundTbl.h"
#include  "NbioDefaults.h"

#define PROPERTY_IOHC_CLKGATING_ENABLED                 0x00000001ull
#define PROPERTY_IOHC_CLKGATING_DISABLED                0x00000002ull
#define PROPERTY_TPH_COMPLETER_ENABLED                  0x00000004ull
#define PROPERTY_TPH_COMPLETER_DISABLED                 0x00000008ull
#define PROPERTY_NBIF_MGCG_CLKGATING_ENABLED            0x00000010ull
#define PROPERTY_NBIF_MGCG_CLKGATING_DISABLED           0x00000020ull
#define PROPERTY_SYSHUB_MGCG_CLKGATING_ENABLED          0x00000040ull
#define PROPERTY_SYSHUB_MGCG_CLKGATING_DISABLED         0x00000080ull
#define PROPERTY_SST_CLKGATING_ENABLED                  0x00000100ull
#define PROPERTY_SST_CLKGATING_DISABLED                 0x00000200ull
#define PROPERTY_IOMMU_DISABLED                         0x00000400ull
#define PROPERTY_IOMMU_L2CLKGATING_ENABLED              0x00000800ull
#define PROPERTY_IOMMU_L2CLKGATING_DISABLED             0x00001000ull
#define PROPERTY_IOMMU_L1CLKGATING_ENABLED              0x00002000ull
#define PROPERTY_IOMMU_L1CLKGATING_DISABLED             0x00004000ull
#define PROPERTY_XAPIC_MODE                             0x00008000ull

#define PROPERTY_PRESENT_IOHC                           0x00010000ull
#define PROPERTY_PRESENT_SYSHUB                         0x00020000ull
#define PROPERTY_PRESENT_NBIF0                          0x00040000ull
#define PROPERTY_PRESENT_NBIF1                          0x00080000ull
#define PROPERTY_PRESENT_NBIF2                          0x00100000ull
#define PROPERTY_PRESENT_IOMMU                          0x00200000ull
#define PROPERTY_PRESENT_L1IOAGR                        0x00400000ull
#define PROPERTY_PRESENT_L1PCIE0                        0x00800000ull
#define PROPERTY_PRESENT_L1PCIE1                        0x01000000ull
#define PROPERTY_PRESENT_L1NBIF0                        0x02000000ull
#define PROPERTY_PRESENT_NB                             0x04000000ull
#define PROPERTY_PRESENT_PCIE0                          0x08000000ull
#define PROPERTY_PRESENT_PCIE1                          0x10000000ull
#define PROPERTY_PRESENT_PCIE2                          0x20000000ull
#define PROPERTY_PRESENT_SDPMUX                         0x40000000ull
#define PROPERTY_PRESENT_SST                            0x80000000ull

#define PROPERTY_DEFAULT_DEVICE_PRESENCE  PROPERTY_PRESENT_IOHC   | \
                                          PROPERTY_PRESENT_SYSHUB | \
                                          PROPERTY_PRESENT_NBIF0  | \
                                          PROPERTY_PRESENT_NBIF1  | \
                                          PROPERTY_PRESENT_NBIF2  | \
                                          PROPERTY_PRESENT_IOMMU  | \
                                          PROPERTY_PRESENT_L1IOAGR| \
                                          PROPERTY_PRESENT_L1PCIE0| \
                                          PROPERTY_PRESENT_L1PCIE1| \
                                          PROPERTY_PRESENT_L1NBIF0| \
                                          PROPERTY_PRESENT_NB     | \
                                          PROPERTY_PRESENT_PCIE0  | \
                                          PROPERTY_PRESENT_PCIE1  | \
                                          PROPERTY_PRESENT_PCIE2  | \
                                          PROPERTY_PRESENT_SDPMUX | \
                                          PROPERTY_PRESENT_SST

/*
 *  This table contains default configuration data of all sockets
 */
const SMN_TABLE NbioPprInitValues [] = {
    NBIO_PPR_INIT_VALUES_TBL
    SMN_ENTRY_TERMINATE
};

/*
 *  This table contains configuration data of all sockets
 */
const SMN_TABLE GnbEarlyInitTableCommon [] = {
  NBIO_IOHC_P2P_TBL                  // 15.1.4.3 Enabling Peer-to-Peer
  NBIO_IOHC_CRS_TBL                  // 15.1.4.5 PCI Express Configuration Cycle Hardware Retry on CRS
  NBIO_IOHC_CLOCK_GATING_TBL         // 15.1.4.8 IOHC Clock Gating
  NBIO_IOHC_ARBITRATION_TBL          // 15.1.4.11 IOHC Arbitration Control
  NBIO_IOAPIC_CLOCK_GATING_TBL       // 15.2.3.1 IOAPIC Clock Gating and Power Management
  NBIO_IOAPIC_INTR_ROUTING_TBL       // 15.2.3.2 IOAPIC Initialization:  Table 224: Recommended Interrupt Routing and Swizzling

  SMN_ENTRY_RMW (
    SIL_RESERVED_737,
    SIL_RESERVED_206 |
    SIL_RESERVED_207,
    (0x1 << SIL_RESERVED_208) |
    (0x0 << SIL_RESERVED_209)
    ),
  SMN_ENTRY_TERMINATE
};

/*
 * This table contains the Nbif Specific configuration for all sockets
 */
const SMN_TABLE GnbnBifInitTable [] = {
  NBIO_NBIF_DEVICE_CFG_TBL           // 15.6.7.1 nBIF Configurations
  NBIO_NBIF_DEVICE_TBL               // Device Enable/Disable
  NBIO_NBIF_DEVICE_INTERRUPT_TBL     // 15.6.7.5.1 Endpoint function interrupt enable
  NBIO_NBIF_DMA_ARBITRATION_TBL      // 15.6.7.5.4 DMA Arbitration
  NBIO_NBIF_SYSHUB_WRT_PERF_ENH_TBL  // SystemHub Write Performance Enhancement (Need confirm again)
  NBIO_TPH_TBL
  NBIO_NBIF_CLOCK_GATING_TBL         // 15.6.7.6.1 : nBIF Clock Gating Feature
  NBIO_NBIF_FLR_TBL
  NBIO_LCLK_DS_IDLE_TBL
  NBIO_ENABLE_SOCCLK_SHUBCLK_DS_TBL
  NBIO_ENABLE_COMPLETION_TIMEOUT_TBL
  SMN_ENTRY_TERMINATE
};

const SMN_TABLE GnbSdpMuxInitTableCommon [] = {
  SIL_RESERVED_784
  SIL_RESERVED_783
  SMN_ENTRY_TERMINATE
};

/*
 *  This table contains the Specific configuration that need to be initialized to RB0
 */
const SMN_TABLE  GnbOncePerSocketInitMP [] = {
  NBIO_Non_VC1_WRITE_TBL             // 15.6.7.5.9  : Non-VC1 write chain disable
  NBIF_DEVICE_ID_DEV_TBL
  NBIO_SST_CLOCK_GATING_TBL
  SMN_ENTRY_TERMINATE
};


/*
 *  This table contains the Specific configuration for IOMMU
 */
const SMN_TABLE GnbIommuEnvInitTable [] = {
  NBIO_IOMMU_INIT_TBL          // 15.3.1.2.1  This table initializes IOMMU
  NBIO_IOMMU_L1_INIT_TBL       // 15.3.1.2.2  IOMMU L1 Initialization
  NBIO_IOMMU_L2_INIT_TBL       // 15.3.1.2.3  IOMMU L2 Initialization
  NBIO_IOMMU_CLOCK_GATING_TBL  // 15.3.1.2.4  IOMMU Clock Gating
  NBIO_IOMMU_SMI_FILTERING_TBL // 15.3.1.2.5  IOMMU SMI Filtering
  //
  // If X2 APIC is enabled, enable XT_SUP in IOMMU
  //
  SMN_ENTRY_PROPERTY_RMW ( \
    PROPERTY_XAPIC_MODE, \
    SMN_IOAGR_IOHUB0NBIO0_L1_FEATURE_SUP_CNTRL_ADDRESS,
    L1_FEATURE_SUP_CNTRL_L1_XT_SUP_W_MASK,
    (0x1 << L1_FEATURE_SUP_CNTRL_L1_XT_SUP_W_OFFSET)
    ),
  SMN_ENTRY_TERMINATE
};

