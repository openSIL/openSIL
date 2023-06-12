/**
 * @file  NbioCommon.h
 * @brief OpenSIL NBIO Common APIs prototypes
 *
 */
/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <stdint.h>
#include <NBIO/Nbio.h>
#include <NBIO/NbioPcieTopologyHelper.h>
#include <SilPcie.h>

void NonPciBarInit (GNB_HANDLE *GnbHandle, uint32_t MmioBarLow, uint32_t MmioBarHigh,
        uint32_t MemorySize, bool Enable, bool LockSettings,bool Above4G);

void NbioEnableNbifDevice (GNB_HANDLE *GnbHandle,uint8_t DeviceType);
void NbioDisableNbifDevice (GNB_HANDLE *GnbHandle,uint8_t DeviceType);

SIL_STATUS NbioBaseConfigurationGenoa (void);
SIL_STATUS NbioIOMMUInit(void);
SIL_STATUS NbioSetTopOfMemory (void);


#define PcieConfigAttachChild(P, C) \
  (P)->Child = (uint16_t) ((uint8_t *) C - (uint8_t *) P);
#define PcieConfigAttachParent(P, C) \
  (C)->Parent = (uint16_t) ((uint8_t *) C - (uint8_t *) P);
#define PcieConfigSetDescriptorFlags(Descriptor, SetDescriptorFlags) \
  if (Descriptor != NULL) (Descriptor)->Header.DescriptorFlags |= SetDescriptorFlags
#define PcieConfigGetNextDescriptor(Descriptor) \
  ((((Descriptor->Header.DescriptorFlags & DESCRIPTOR_TERMINATE_LIST) != 0) ? NULL : ((Descriptor + 1))))
#define PcieConfigIsPcieEngine(Descriptor) \
  ((Descriptor != NULL) ? ((Descriptor->Header.DescriptorFlags & DESCRIPTOR_PCIE_ENGINE) != 0) : false)
#define PcieConfigIsSbPcieEngine(Engine) \
  ((Engine != NULL) ? ((bool) (Engine->Type.Port.PortData.MiscControls.SbLink)) : false)
#define PcieConfigIsEngineAllocated(Descriptor) \
  ((Descriptor != NULL) ? ((Descriptor->Header.DescriptorFlags & DESCRIPTOR_ALLOCATED) != 0) : false)
#define PcieConfigResetDescriptorFlags(Descriptor, ResetDescriptorFlags) \
  if (Descriptor != NULL) ((PCIe_DESCRIPTOR_HEADER *) Descriptor)->DescriptorFlags &= (~(ResetDescriptorFlags))
#define PcieInputParsetGetNextDescriptor(Descriptor) \
  ((Descriptor == NULL) ? NULL : ((Descriptor->Flags & DESCRIPTOR_TERMINATE_LIST) != 0) ? NULL : (Descriptor + 1))
#define PcieConfigGetNextTopologyDescriptor(Descriptor, Termination) \
  ((Descriptor == NULL) ? NULL : ((((PCIe_DESCRIPTOR_HEADER *) Descriptor)->DescriptorFlags & Termination) != 0) ? \
    NULL : ((uint8_t *) Descriptor + ((PCIe_DESCRIPTOR_HEADER *) Descriptor)->Peer))
#define GnbGetNextHandle(Descriptor) \
  (GNB_HANDLE *) PcieConfigGetNextTopologyDescriptor (Descriptor, DESCRIPTOR_TERMINATE_TOPOLOGY)
#define PcieConfigGetNextDataDescriptor(Descriptor) \
  ((Descriptor->Flags & DESCRIPTOR_TERMINATE_LIST) != 0 ? NULL : (Descriptor + 1))


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


#define NONPCI_BARSIZE_512MB                0x20000000ul            ///< Size of 512MB BAR
#define NONPCI_BARSIZE_256MB                0x10000000ul            ///< Size of 256MB BAR                                                             ///
#define NONPCI_BARSIZE_1MB                  0x100000ul              ///< Size of 1MB BAR
#define NONPCI_BARSIZE_512KB                0x7D000ul               ///< Size of 512KB BAR
#define NONPCI_BARSIZE_4KB                  0x1000                  ///< Size of 4KB BAR

#define NBIO_TABLE_PROPERTY_DEFAULT                     0x00000000ul
#define NBIO_TABLE_PROPERTY_IGFX_DISABLED               0x00000001ul
#define NBIO_TABLE_PROPERTY_IOMMU_DISABLED              0x00000002ul
#define NBIO_TABLE_PROPERTY_SST_CLOCK_GATING_DISABLED   0x00000004ul
#define NBIO_TABLE_PROPERTY_ORB_CLK_GATING              0x00000008ul
#define NBIO_TABLE_PROPERTY_IOC_LCLK_CLOCK_GATING       0x00000010ul
#define NBIO_TABLE_PROPERTY_IOC_SCLK_CLOCK_GATING       0x00000020ul
#define NBIO_TABLE_PROPERTY_IOMMU_L1_CLOCK_GATING       0x00000040ul
#define NBIO_TABLE_PROPERTY_IOMMU_L2_CLOCK_GATING       0x00000080ul
#define NBIO_TABLE_PROPERTY_BAPM                        0x00000100ul
#define NBIO_TABLE_PROPERTY_SECONDARY_GNB               0x00000200ul
#define NBIO_TABLE_PROPERTY_NMI_SYNCFLOOD               0x00000400ul
#define NBIO_TABLE_PROPERTY_LOADLINE_ENABLE             0x00001000ul
#define NBIO_TABLE_PROPERTY_SMU_SCLK_CLOCK_GATING       0x00002000ul
#define NBIO_TABLE_PROPERTY_CHUB_CLOCK_GATING           0x00004000ul
#define NBIO_TABLE_PROPERTY_HYBRID_BOOST                0x00008000ul
#define NBIO_TABLE_PROPERTY_LHTC                        0x00010000ul
#define NBIO_TABLE_PROPERTY_SVI2                        0x00020000ul
#define NBIO_TABLE_PROPERTY_TDC                         0x00040000ul
#define NBIO_TABLE_PROPERTY_PKG_POWER_TRACKING          0x00080000ul
#define NBIO_TABLE_PROPERTY_DGFX_DISABLED               0x00100000ul
#define NBIO_TABLE_PROPERTY_ACP_CLOCK_GATING_DISABLED   0x00200000ul
#define NBIO_TABLE_PROPERTY_ACP_POWER_GATING_ENABLED    0x00400000ul
#define NBIO_TABLE_PROPERTY_ACP_POWER_GATING_DISABLED   0x00800000ul
#define NBIO_TABLE_PROPERTY_IOMMU_L2_DEBUG_3_31         0x01000000ul
#define NBIO_TABLE_PROPERTY_AXG_DISABLE                 0x02000000ul
#define NBIO_TABLE_PROPERTY_SPG_CLOCK_GATING            0x04000000ul
#define NBIO_TABLE_PROPERTY_ACG_AZ_CLOCK_GATING         0x08000000ul
#define NBIO_TABLE_PROPERTY_IOMMU_L1_SST_CLOCK_GATING   0x10000000ul
#define NBIO_TABLE_PROPERTY_IOHC_Clock_GATING           0x20000000ul
#define NBIO_TABLE_PROPERTY_POISON_ACTION_CONTROL       0x40000000ul
