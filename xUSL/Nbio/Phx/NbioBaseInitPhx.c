/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  NbioBaseInitPhx.c
 * @brief This file contains API definition Nbio Base Initialization
 *
 */

#include <xSIM.h>
#include <Nbio/NbioIp2Ip.h>
#include <CommonLib/SmnAccess.h>
#include <SilSocLogicalId.h>

#include <Nbio/Phx/includePHX/PHX_IOHC.h>
#include <Nbio/Phx/includePHX/PHX_NBIFMM.h>

#include "NbioIohcPhxTbl.h"
#include "NbioIoapicPhxTbl.h"
#include "NbioPhx.h"
#include "NbioSmnTable.h"
#include "NbioNbifPhxTbl.h"
#include "NbioWorkaroundPhxTbl.h"
#include "NbioUsbOverPciePhxTbl.h"
#include "NbioCmn2RevPhx.h"

/*
 *  This table contains configuration data of all sockets
 */
const SMN_TABLE GnbEarlyInitTableCommon [] = {
  NBIO_IOHC_P2P_TBL                           // Enabling Peer-to-Peer
  NBIO_IOHC_CRS_TBL                           // PCI Express Configuration Cycle Hardware Retry on CRS
  NBIO_IOHC_CLOCK_GATING_TBL                  // IOHC Clock Gating
  NBIO_IOHC_POWER_GATING_TBL                  // IOHC Power Gating
  NBIO_IOHC_ARBITRATION_TBL                   // IOHC Arbitration Control
  NBIO_IOHC_POISON_ACTION_TBL                 // IOHC POISON_ACTION_CONTROL
  NBIO_IOAPIC_CLOCK_GATING_TBL                // IOAPIC Clock Gating and Power Management
  NBIO_IOAPIC_INTR_ROUTING_TBL                // IOAPIC Initialization: Recommended Interrupt Routing and Swizzling
  NBIO_WA_ACP_SUB_CLASS_TBL                   // ACP Configuration Sub Class
  NBIO_WA_PCI_CV_TEST_TBL                     // PCIe CV test
  NBIO_WA_CPL_TIMEOUT_TBL                     // Enable Completion Timeout feature
  NBIO_WA_RIOMMU_DIS_TIMEOUT_TBL              // rIOMMU, disable timeout, 25MHz REFCLK, 31 (0x1F) invalidations limit
  NBIO_WA_RIOMMU_TW_COHERENT_TBL              // rIOMMU TW Coherent bit
  NBIO_WA_PME_TURNOFF_MODE_TBL                // PME_TURNOFF_MODE
  NBIO_NBIF_FIRST_VF_OFFSET_TBL               // nBIF IOV First VF offset
  NBIO_WA_RSMU_MASTER_MESSAGE_SEND_ENABLE_TBL // RSMU_MASTER_MESSAGE_SEND_ENABLE
  NBIO_ENABLE_FLR_TBL                         // Enable FLR for IPU
  SMN_ENTRY_TERMINATE
};


/*
 * This table contains the Nbif Specific configuration for all sockets
 */
const SMN_TABLE GnbnBifInitTable [] = {
  NBIO_NBIF_DEVICE_CFG_TBL           // nBIF Configurations
  NBIO_NBIF_ATC_ATS_TBL              // ATC/ATS Support
  NBIO_NBIF_DEVICE_INTERRUPT_TBL     // Endpoint function interrupt enable
  NBIO_NBIF_DMA_ARBITRATION_TBL      // DMA Arbitration
  NBIO_NBIF_ATOMIC_TBL               // Atomic
  NBIO_NBIF_PERF_REG_TBL             // PerfReg Final Setting
  NBIO_NBIF_DS_CTRL_LCLK_TBL
  NBIO_SYSHUB_DS_CTRL_SOCCLK_TBL
  NBIO_SYSHUB_DS_CTRL_SHUBCLK_TBL
  NBIO_SYSHUB_DS_CTRL_HSPCLK_TBL
  NBIO_SYSHUB_NGDC_PG_MISC_CTRL_TBL
  SMN_ENTRY_TERMINATE
};


const SMN_TABLE GnbPCIe23InitTable [] = {
  PCIE23_TX_PORT_CTRL
  PCIE23_TX_REQUESTER_ID_DEVICE
  PCIE23_Enable_L1
  PCIE23_SDP_DISCONNECT_CNTL
  PCIE23_LCLK_DS_AND_LCLK_GATING
  PCIE23_RSMU_MASTER_MESSAGE_SEND_ENABLE
  PCIE23_PERFREG_FINAL_SETTING
  PCIE23_MISC_SETTING
  SMN_ENTRY_TERMINATE
};

/*----------------------------------------------------------------------------------------*/
/**
 * NbioBaseInitPhx
 *
 * @brief  This function initializes the Nbif ports
 *
 * @param  GnbHandle             Pointer to Iohc config data
 * @param  NBIOCLASS_DATA_BLOCK  Pointer to NBIO Data Block
 *
 * @retval SilPass              The Nbio base initialization is successful
 * @retval SilInvalidParameter  Error indicating invalid input arguments are passed
 *
 */

static
SIL_STATUS
NbioBaseInitPhx (
  GNB_HANDLE            *GnbHandle,
  NBIOCLASS_DATA_BLOCK  *NbioIpBlockData
  )
{
  uint8_t index = 0;

  if ((GnbHandle == NULL) || (NbioIpBlockData == NULL)) {
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "ERROR : Invalid input argument\n");
    assert(false);
    return SilInvalidParameter;
  }

  //nBIF ReqID
  xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, NBIFMMx1013101c),
    (uint32_t) ~(BITS_16_TO_23_MASK_AAAABFV |
    BITS_24_TO_28_MASK_AAAABFR |
    BITS_29_TO_31_MASK_AAAABFT),
    (GnbHandle->Address.Address.Bus << BIT16_OFFSET_AAAABFW) |
    (0x8 << BIT24_OFFSET_AAAABFS) |
    (0x1 << BIT29_OFFSET_AAAABFU)
    );

  xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, NBIFMMx1013121c),
    (uint32_t) ~(BITS_16_TO_23_MASK_AAAABGF |
    BITS_24_TO_28_MASK_AAAABGB |
    BITS_29_TO_31_MASK_AAAABGD),
    (GnbHandle->Address.Address.Bus << BIT16_OFFSET_AAAABGG) |
    (0x8 << BIT24_OFFSET_AAAABGC) |
    (0x2 << BIT29_OFFSET_AAAABGE)
    );

  xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, NBIFMMx1013141c),
    (uint32_t) ~(BITS_16_TO_23_MASK_AAAABGX |
    BITS_24_TO_28_MASK_AAAABGT |
    BITS_29_TO_31_MASK_AAAABGV),
    (GnbHandle->Address.Address.Bus << BIT16_OFFSET_AAAABGY) |
    (0x8 << BIT24_OFFSET_AAAABGU) |
    (0x3 << BIT29_OFFSET_AAAABGW)
    );


  if (NbioIpBlockData->NbioConfigData.CfgACSEnable == true) {
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "Enabling ACS capability...\n");
    //For NBIO0 Port0-2
    for (index = 0; index < 3; index++) {
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, NBIFMMSx10131000 + index * 0x200),
        (uint32_t) ~(BIT2_MASK_AAAABET),
        (0x1 << BIT2_OFFSET_AAAABEU)
        );
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, NBIFMMx10131014 + index * 0x200),
        (uint32_t) ~(BIT23_MASK_AAAABFJ | \
        BIT24_MASK_AAAABFL | \
        BIT25_MASK_AAAABFH | \
        BIT26_MASK_AAAABFF | \
        BIT27_MASK_AAAABFN | \
        BIT29_MASK_AAAABFD),
        ((0x1 << BIT23_OFFSET_AAAABFK) | \
        (0x1 << BIT24_OFFSET_AAAABFM) | \
        (0x1 << BIT25_OFFSET_AAAABFI) | \
        (0x1 << BIT26_OFFSET_AAAABFG) | \
        (0x1 << BIT27_OFFSET_AAAABFO) | \
        (0x1 << BIT29_OFFSET_AAAABFE))
        );
    }
    //For NBIF0 Port0
    for (index = 0; index < 8; index++) {
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, NBIFMMx10134008 + index * 0x200),
        (uint32_t) ~(BIT17_MASK_AAAABDH),
        (0x1 << BIT17_OFFSET_AAAABDI)
        );
    }
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, NBIFMMx10135008),
      (uint32_t) ~(BIT17_MASK_AAAABDH),
      (0x1 << BIT17_OFFSET_AAAABDI)
      );
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, NBIFMMx10135208),
      (uint32_t) ~(BIT17_MASK_AAAABDH | \
      BIT16_MASK_AAAABDJ | \
      BITS_24_TO_26_MASK_AAAABDP | \
      BIT28_MASK_AAAABDT),
      ((0x1 << BIT17_OFFSET_AAAABDI) | \
      (0x1 << BIT16_OFFSET_AAAABDK) | \
      (0x4 << BIT24_OFFSET_AAAABDQ) | \
      (0x1 << BIT28_OFFSET_AAAABDU))
      );
    //For NBIF0 Port2
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, NBIFMMx10136008),
      (uint32_t) ~(BIT17_MASK_AAAABDH),
      (0x1 << BIT17_OFFSET_AAAABDI)
      );

    for (index = 0; index < 4; index++) {
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, NBIFMMx10136608 + index * 0x200),
        (uint32_t) ~(BIT17_MASK_AAAABDH),
        (0x1 << BIT17_OFFSET_AAAABDI)
        );
    }
  }

  for (index = 0; index < 4; index++) {
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, NBIFMMx10136608 + index * 0x200),
      (uint32_t) ~(BIT17_MASK_AAAABDH),
      (0x1 << BIT17_OFFSET_AAAABDI)
      );

  }

  return SilPass;
}


/*----------------------------------------------------------------------------------------*/
/**
 * NbioBaseConfigurationRs
 *
 * @brief  This function performs early NBIO initialization.
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param  NBIOCLASS_DATA_BLOCK  Pointer to NBIO Data Block
 *
 * @retval SilPass              The Nbio base configuration is successful
 * @retval SilInvalidParameter  Error indicating invalid input arguments are passed
 *
 */
SIL_STATUS
NbioBaseConfigurationPhx (
  SIL_CONTEXT          *SilContext,
  NBIOCLASS_DATA_BLOCK *NbioIpBlockData
  )
{
  uint32_t                Property;
  GNB_HANDLE              *GnbHandle;

  NBIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (NbioIpBlockData == NULL) {
    return SilInvalidParameter;
  }
  // Adjust Property based on Debug Info and values
  // From Earlier
  GnbHandle = GetGnbHandle(SilContext);
  if (GnbHandle == NULL) {
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "ERROR : Invalid GnbHandle\n");
    assert(false);
    return SilInvalidParameter;
  }

  //Need to declare them over NbioIpBlockData
  if (NbioIpBlockData->NbioConfigData.NbioGlobalCgOverride == 0) {
    NbioIpBlockData->NbioConfigData.CfgSstunlClkGating = false;
    NbioIpBlockData->NbioConfigData.IOHCClkGatingSupport = false;
    NbioIpBlockData->NbioConfigData.IOHCPgEnable = false;
    NbioIpBlockData->NbioConfigData.CfgNbifMgcgClkGating = false;
    NbioIpBlockData->NbioConfigData.CfgSyshubMgcgClkGating = false;
    NbioIpBlockData->NbioConfigData.NbifMgcgHysteresis = 0;
    NbioIpBlockData->NbioConfigData.SyshubMgcgHysteresis = 0;
  }

  GnbHandle = GetGnbHandle(SilContext);
  if (GnbHandle == NULL) {
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "ERROR : Invalid GnbHandle\n");
    assert(false);
    return SilInvalidParameter;
  }

  while (GnbHandle != NULL) {
    ///
    /// Blast table(s) specific to socket 0 (first socket)
    ///
    Property = PROPERTY_DEFAULT_DEVICE_PRESENCE;
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "Default properties for RB %d = 0x%X", GnbHandle->RBIndex, Property);
    Property &= ~(GnbHandle->Header.DescriptorFlags << 16);
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "Properties for RB %d = 0x%X", GnbHandle->RBIndex, Property);

    if (Property & PROPERTY_PRESENT_IOHC) {
      if (NbioIpBlockData->NbioConfigData.IOHCClkGatingSupport) {
        Property |= PROPERTY_IOHC_CLKGATING_ENABLED;
      } else {
        Property |= PROPERTY_IOHC_CLKGATING_DISABLED;
      }
      if (NbioIpBlockData->NbioConfigData.CfgSstunlClkGating) {
        Property |= PROPERTY_SST_CLKGATING_ENABLED;
      } else {
        Property |= PROPERTY_SST_CLKGATING_DISABLED;
      }
      if (NbioIpBlockData->NbioConfigData.IOHCPgEnable) {
        Property |= PROPERTY_IOHC_CLKGATING_ENABLED;
      } else {
        Property |= PROPERTY_IOHC_CLKGATING_DISABLED;
      }
      if (NbioIpBlockData->NbioConfigData.CfgNbifMgcgClkGating) {
        Property |= PROPERTY_NBIF_MGCG_CLKGATING_ENABLED;
      } else {
        Property |= PROPERTY_NBIF_MGCG_CLKGATING_DISABLED;
      }
      if (Property & PROPERTY_PRESENT_SYSHUB) {
        if (NbioIpBlockData->NbioConfigData.CfgSyshubMgcgClkGating) {
          Property |= PROPERTY_SYSHUB_MGCG_CLKGATING_ENABLED;
        } else {
          Property |= PROPERTY_SYSHUB_MGCG_CLKGATING_DISABLED;
        }
      }
      if (0 != NbioIpBlockData->NbioConfigData.TPHCompleterEnable) {
        Property |= PROPERTY_TPH_COMPLETER_ENABLED;
      }
    }

    ///
    /// Blast Gnb Early Init Common tables that should be applied to each Socket
    ///
    ProgramNbioSmnTable(GnbHandle, GnbEarlyInitTableCommon, NBIO_SPACE(GnbHandle, 0), Property);
    ProgramNbioSmnTable(GnbHandle, GnbnBifInitTable, NBIO_SPACE(GnbHandle, 0), Property);
    ProgramNbioSmnTable(GnbHandle, GnbPCIe23InitTable, NBIO_SPACE(GnbHandle, 0), Property);

    NbioBaseInitPhx(GnbHandle, NbioIpBlockData);

    ///
    /// Bar allocation for SMN non pci devices
    /// Note that PSPP bar allocation is slightly different and is
    /// done elsewhere
    ///

    if (NbioIpBlockData->NbioConfigData.NbifMgcgHysteresis != 0) {
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIFMMx1013a21c,
        (uint32_t) ~(BITS_2_TO_9_MASK_AAAABCU),
        ((NbioIpBlockData->NbioConfigData.NbifMgcgHysteresis) <<
          BIT2_OFFSET_AAAABCV)
        );
    }
    if (NbioIpBlockData->NbioConfigData.SyshubMgcgHysteresis != 0) {
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        SYSHUBMMx1410020,
        (uint32_t) ~(BITS_2_TO_9_MASK_AAAAAOV),
        ((NbioIpBlockData->NbioConfigData.SyshubMgcgHysteresis) <<
          BIT2_OFFSET_AAAAAOW)
        );
    }

    if (NbioIpBlockData->NbioConfigData.IohcNonPCIBarInitSmu) {
      NonPciBarInit(SilContext,
        GnbHandle,
        NBIO_SPACE(GnbHandle, IOHCx13b102e8),
        NBIO_SPACE(GnbHandle, IOHCx13b102ec),
        NONPCI_BARSIZE_1MB,
        true,
        true,
        false
        );
    }
    if (NbioIpBlockData->NbioConfigData.IohcNonPCIBarInitDbg) {
      NonPciBarInit(SilContext,
        GnbHandle,
        NBIO_SPACE(GnbHandle, IOHCx13b102ec),
        NBIO_SPACE(GnbHandle, IOHCx13b102fc),
        NONPCI_BARSIZE_512KB,
        true,
        true,
        false
        );
    }
    if (NbioIpBlockData->NbioConfigData.IohcNonPCIBarInitFastReg) {
      NonPciBarInit(SilContext,
        GnbHandle,
        NBIO_SPACE(GnbHandle, IOHCx13b10300),
        NBIO_SPACE(GnbHandle, IOHCx13b10304),
        NONPCI_BARSIZE_1MB,
        true,
        true,
        false
        );
    }
    if (NbioIpBlockData->NbioConfigData.IohcNonPCIBarInitFastRegCtl) {
      NonPciBarInit(SilContext,
        GnbHandle,
        NBIO_SPACE(GnbHandle, IOHCx13b10308),
        NBIO_SPACE(GnbHandle, IOHCx13b1030c),
        NONPCI_BARSIZE_4KB,
        true,
        true,
        false
        );
    }

    GnbHandle = GnbGetNextHandle(GnbHandle);
  }

  NBIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");

  return SilPass;
}


/**
 * NbioEnumerateHarvestWrappersPhx
 *
 * @brief  Function to remove harvested PCIe cores from SOC Complex Descriptor
 *
 * @param  Pcie  Pointer to SOC Complex Descriptor
 *
 */
void
NbioEnumerateHarvestWrappersPhx (
  PCIe_PLATFORM_CONFIG *Pcie
  )
{
  NBIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}
