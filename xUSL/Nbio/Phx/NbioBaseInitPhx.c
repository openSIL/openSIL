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

#include <Nbio/Phx/includePHX/PHX_ACP.h>
#include <Nbio/Phx/includePHX/PHX_AZALIA.h>
#include <Nbio/Phx/includePHX/PHX_IOHC.h>
#include <Nbio/Phx/includePHX/PHX_IOMMUL1.h>
#include <Nbio/Phx/includePHX/PHX_IOMMUL2.h>
#include <Nbio/Phx/includePHX/PHX_NBIFMM.h>

#include "NbioIohcPhxTbl.h"
#include "NbioIoapicPhxTbl.h"
#include "NbioPhx.h"
#include "NbioSmnTable.h"
#include "NbioNbifPhxTbl.h"
#include "NbioWorkaroundPhxTbl.h"
#include "NbioUsbOverPciePhxTbl.h"
#include "NbioCmn2RevPhx.h"
#include "NbioSyshubPhxTbl.h"

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
  NBIO_NBIF_CLOCK_GATING_TBL
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

const SMN_TABLE GnbnBifPgTable [] = {
  NBIO_NBIF_POWER_GATING_TBL
  SMN_ENTRY_TERMINATE
};

const SMN_TABLE GnbSysHubPgTable [] = {
  NBIO_NBIF_SHUB_POWER_GATING_TBL
  SMN_ENTRY_TERMINATE
};

const SMN_TABLE GnbnBifOBFFCtrlTable [] = {
  NBIO_NBIF_OBFF_CTRL_TBL
  SMN_ENTRY_TERMINATE
};

const SMN_TABLE GnbnBifOBFFDmaTable [] = {
  NBIO_NBIF_OBFF_DMA_TBL
  SMN_ENTRY_TERMINATE
};

const SMN_TABLE GnbSysHubNgdcMgcgCtrCGTable [] = {
  NBIO_SYSHUB_NGDC_MGCG_CTR_CLOCK_GATING_TBL
  SMN_ENTRY_TERMINATE
};

const SMN_TABLE GnbSysHubNgdcMgcgHspCGTable [] = {
  NBIO_SYSHUB_MGCG_CTRL_HSP_CLOCK_GATING_TBL
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

const SMN_TABLE GnbPCIe23DynamicPGTable [] = {
  PCIE23_LC_CLK_GATE
  PCIE23_LC_PM_SUBSTATE
  PCIE23_LCLK_DS_AND_LCLK_GATING2
  SMN_ENTRY_TERMINATE
};

const SMN_TABLE GnbnBifiGpuDisable [] = {
  NBIO_NBIF_DISABLE_IGPU_TBL
  NBIO_NBIF_DISABLE_LTR_TBL
  NBIO_NBIF_DISABLE_AER_TBL
  NBIO_NBIF_DISABLE_FLR_ATOMIC_TBL
  SMN_ENTRY_TERMINATE
};


/**
 * Check AcpPin select to see if in HDA Mode based on the following definitions
 *  4 - Mainstream Desktop (AM5 Package);HDA(3SDI) +  PDM(2CH).
 *  5 -Mainstream Desktop (AM5 Package);HDA(1SDI) +  PDM(6CH) .
 *  6 -Mainstream Desktop (AM5 Package) ;HDA(1SDI) + SW0(1MDATA) + PDM(2CH).
 *  7 - MainstreamDesktop (AM5 Package);SW0(4MDATA) + PDM(6CH) .
 *  8 - MainstreamDesktop (AM5 Package);SW0(4MDATA) +SW1(1MDATA)+ PDM(2CH) .
 *  9 - mainstream notebook;3I2S + 1 REFCLK + 1 INTR.
 *  10 - mainstream notebook;HDA(3SDI) +  PDM(6CH) +I2S .
 *  11 - mainstream notebook;HDA(3SDI) +  PDM(8CH)  .
 *  12 - mainstream notebook;HDA(1SDI) + SW(1MDATA) + PDM(6CH) + I2S .
 *  13 - mainstream notebook;SW0(4MDATA) + SW1(1MDATA) +PDM(6CH) + I2S .
 *  14 - mainstream notebook;SW0(4MDATA) + SW1(1MDATA) + PDM(8CH) .
 *  15 - LPFLL Clock out for lab debug .
 *
 * @param[in] AcpPin   AcpPinConfig select
 * @retval    TRUE     HDA Mode
 * @retval    FALSE    Not HDA Mode
 */
static bool
IsHDAMode(
  uint32_t AcpPin
  )
{
  if (((AcpPin >= 7) && (AcpPin <= 9)) ||
      ((AcpPin >= 13) && (AcpPin <= 14))) {
    return false;
  }
  return true;
}

static void
AudioIOConfigCallback (
  GNB_HANDLE           *GnbHandle,
  NBIOCLASS_DATA_BLOCK *NbioIpBlockData
  )
{
  uint8_t  Value;
  uint32_t AcpPinCfg;
  uint32_t RegisterValue;

  // Get ACP_PIN_CONFIG
  Value = NbioIpBlockData->NbioConfigData.AmdNbioAudioSelect;
  AcpPinCfg = Value;
  if (ISSOCPHXAM5) {
    // For AM5, default value is 4, and valid data is 4~8
    AcpPinCfg = 4;
    if ((Value >= 4) && (Value <= 8)) {
      AcpPinCfg = Value;
    }
  } else  {
    // For FP7/FP7r2 and FP8, default value is 10, and valid data is 9~15
    AcpPinCfg = 10;
    if ((Value >= 9) && (Value <= 15)) {
      AcpPinCfg = Value;
    }
  }

  NBIO_TRACEPOINT(SIL_TRACE_INFO, "ACP_PIN_CONFIG Entry: %x, %x\n", Value, AcpPinCfg);

  xUSLSmnWrite(0,
    GnbHandle->Address.Address.Bus,
    ACPAONx1241440,
    AcpPinCfg
    );
  RegisterValue = xUSLSmnRead(0,
                    GnbHandle->Address.Address.Bus,
                    ACPCLKRSTx1241028);
  NBIO_TRACEPOINT(SIL_TRACE_INFO, "ACP_PGFSM_STATUS %x\n", RegisterValue);
  if (RegisterValue & (BIT_32(1) | BIT_32(3))) {
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "Power on ACP\n");
    xUSLSmnReadModifyWrite(0,
      GnbHandle->Address.Address.Bus,
      ACPCLKRSTx1241024,
      (uint32_t)~(0x1),
      0x1 << 0
      );
    while (true) {
      RegisterValue = xUSLSmnRead(0,
                        GnbHandle->Address.Address.Bus,
                        ACPCLKRSTx1241028);
      if (RegisterValue == 0x0) {
        break;
      }
    }

    /* not use ACP but only HDA, so keep ACP banks and caches in shut down mode while keeping ACP on */
    // Clear ACP soft reset bit (ACP_SOFT_RESET)
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "Clear ACP soft reset bit\n");
    xUSLSmnReadModifyWrite(0,
      GnbHandle->Address.Address.Bus,
      ACPCLKRSTx1241000,
      (uint32_t)~(0x1),
      0x0
      );
    while (true) {
      RegisterValue = xUSLSmnRead(0,
                        GnbHandle->Address.Address.Bus,
                        ACPCLKRSTx1241000
                        );
      RegisterValue &= BIT_32(16);
      if (RegisterValue == 0) {
        break;
      }
    }
  }

  if (!IsHDAMode(AcpPinCfg)) {
    // For not HDA cases, we should do below sequence
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "Program Soundwire Entry\n");

    xUSLSmnReadModifyWrite(0,
      GnbHandle->Address.Address.Bus,
      AZALIAx1200008,
      (uint32_t) ~(0x1),
      (0x0 << 1)
      );

    do {
      RegisterValue = xUSLSmnRead(0, GnbHandle->Address.Address.Bus, AZALIAx1200008);
    } while (RegisterValue != 0);

    RegisterValue = 1;
    xUSLSmnReadModifyWrite (0,
      GnbHandle->Address.Address.Bus,
      AZALIAx1200008,
      (uint32_t) ~(0x1),
      RegisterValue << 0
      );

    do {
      RegisterValue = xUSLSmnRead(0, GnbHandle->Address.Address.Bus, AZALIAx1200008);
    } while (RegisterValue != 0);

    RegisterValue = 0;
    xUSLSmnReadModifyWrite(0, GnbHandle->Address.Address.Bus,
                     AZALIAx1200008,
                     (uint32_t) ~(0x1),
                     RegisterValue << 0
                     );

    do {
      RegisterValue = xUSLSmnRead(0, GnbHandle->Address.Address.Bus, AZALIAx1200008);
    } while (RegisterValue != 0);

    //Disable F6
    xUSLSmnReadModifyWrite(0,
      GnbHandle->Address.Address.Bus,
      NBIFMMx10134c00,
      (uint32_t) ~(0x1 << 28),
      (0 << 28)
      );

  }
  if (AcpPinCfg != 0xf) {
    RegisterValue = xUSLSmnRead(0, GnbHandle->Address.Address.Bus, ACPAONx1241468);
    RegisterValue &= (uint32_t)~(BIT_32(13));
    NBIO_TRACEPOINT(SIL_TRACE_INFO,
      "SmnRegisterWrite SMN_ACP_PAD_DISABLE_OE_CTRL_ADDRESS RegisterValue 0x%x\n",
       RegisterValue);
    xUSLSmnWrite(0, GnbHandle->Address.Address.Bus, ACPAONx1241468, RegisterValue);
  }

  if (!NbioIpBlockData->NbioConfigData.CfgAzaliaEnable) {
    /* Disable DCE.AZ F1 */
    xUSLSmnReadModifyWrite(0,
      GnbHandle->Address.Address.Bus,
      NBIFMMx10134200,
      (uint32_t) ~(0x1 << 28),
      (0 << 28)
      );
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "Disable Azalia \n");
  }
}

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
  uint32_t PgmstCntlCfgPGEn;

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

  if (NbioIpBlockData->NbioConfigData.Pcie23DynPowerGating) {
    PgmstCntlCfgPGEn = 1;
    if (NbioIpBlockData->NbioConfigData.PcieSramWA) {
      PgmstCntlCfgPGEn = 0;
    }

    if (!NbioIpBlockData->NbioConfigData.Usb4Rt0En ||
        !NbioIpBlockData->NbioConfigData.Usb4Rt0PcieTnlEn) {
      PgmstCntlCfgPGEn = 1;
    }
    xUSLSmnReadModifyWrite(0, GnbHandle->Address.Address.Bus,
                    NBIO_SPACE(GnbHandle, PCIEPORTx113804c0),
                    (uint32_t)~(0x100),
                    (uint32_t)(PgmstCntlCfgPGEn << 8)
                    );
    if (!NbioIpBlockData->NbioConfigData.Usb4Rt1En ||
        !NbioIpBlockData->NbioConfigData.Usb4Rt1PcieTnlEn) {
      PgmstCntlCfgPGEn = 1;
    }
    xUSLSmnReadModifyWrite(0, GnbHandle->Address.Address.Bus,
                    NBIO_SPACE(GnbHandle, PCIEPORTx114804c0),
                    (uint32_t)~(0x100),
                    (uint32_t)(PgmstCntlCfgPGEn << 8)
                    );
  }

  if (ISSOCPHX2 || ISSOCHPT2) {
    xUSLSmnReadModifyWrite(0, GnbHandle->Address.Address.Bus,
                  IOHCx13b38404,
                  (uint32_t) ~0x5,
                  0x5
                  );

    // set USB0/1/2 default selection to VC5
    xUSLSmnReadModifyWrite(0, GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SYSHUBMMx1411034),
                  (uint32_t)~0xf,
                  0xf
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
  SOC_LOGICAL_ID          LogicalId;
  uint32_t                Value;


  NBIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (NbioIpBlockData == NULL) {
    return SilInvalidParameter;
  }
  // Adjust Property based on Debug Info and values
  // From Earlier
  GnbHandle = GetGnbHandle(SilContext);
  if (GnbHandle == NULL) {
    NBIO_TRACEPOINT(SIL_TRACE_ERROR, "Invalid GnbHandle\n");
    assert(false);
    return SilInvalidParameter;
  }

  if (ISSOCPHXAM5) {
    NbioIpBlockData->NbioConfigData.UsbSysHubSelect = 0xf;
  }

  // To Check GFX IP is disable or not
  Value = xUSLSmnRead(0, GnbHandle->Address.Address.Bus, 0x5d528);
  NBIO_TRACEPOINT(SIL_TRACE_INFO, "  GFX disable bit 0x%x\n", Value);
  if (Value & BIT_32(1)) {
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "  Disable CfgIgpuControl!!\n");
    NbioIpBlockData->NbioConfigData.CfgIgpuControl = 0;
  }

  GetSocLogicalIdOnCurrentCore(&LogicalId);
  NBIO_TRACEPOINT(SIL_TRACE_INFO, "LogicalId.Revision = 0x%x\n", LogicalId.Revision);
  Value = xUSLSmnRead(0, GnbHandle->Address.Address.Bus, 0x5d600);
  Value = (Value & 0x3c) >> 2;
  if ((LogicalId.Revision & AMD_REV_F19_PHX_A0) ||
      ((LogicalId.Revision & AMD_REV_F19_PHX_A1) && (Value < 0x9)) ||
      ((LogicalId.Revision & AMD_REV_F19_PHX2_A0) && (Value < 0x2))) {
    NbioIpBlockData->NbioConfigData.PcieSramWA = 1;
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "CFG_PG_EN = 0 on A1\n");
  }

  // Copy SB_LOCATION to the L1 and L2 in IOMMU. This only applies to NBIO3, where the FCH is located
  Value = xUSLSmnRead(0, GnbHandle->Address.Address.Bus, NBIO_SPACE(GnbHandle, IOHCx13b1007c));
  NBIO_TRACEPOINT(SIL_TRACE_INFO, "Read SB_LOCATION as 0x%x from 0x%x\n", Value, NBIO_SPACE(GnbHandle, IOHCx13b1007c));
  xUSLSmnWrite(0, GnbHandle->Address.Address.Bus, NBIO_SPACE(GnbHandle, IOMMUL1x14800024), Value);
  NBIO_TRACEPOINT(SIL_TRACE_INFO, "Wrote SB_LOCATION as 0x%x from 0x%x\n", Value, NBIO_SPACE(GnbHandle, IOMMUL1x14800024));
  xUSLSmnWrite(0,GnbHandle->Address.Address.Bus, NBIO_SPACE(GnbHandle, IOMMUL2x13f0112c), Value);
  NBIO_TRACEPOINT(SIL_TRACE_INFO, "Wrote SB_LOCATION as 0x%x from 0x%x\n", Value, NBIO_SPACE(GnbHandle, IOMMUL2x13f0112c));

  //Need to declare them over NbioIpBlockData
  if (NbioIpBlockData->NbioConfigData.NbioGlobalCgOverride == 0) {
    NbioIpBlockData->NbioConfigData.CfgSstunlClkGating = false;
    NbioIpBlockData->NbioConfigData.IOHCClkGatingSupport = false;
    NbioIpBlockData->NbioConfigData.IOHCPgEnable = false;
    NbioIpBlockData->NbioConfigData.CfgNbifMgcgClkGating = false;
    NbioIpBlockData->NbioConfigData.CfgSyshubMgcgClkGating = false;
    NbioIpBlockData->NbioConfigData.NbifMgcgHysteresis = 0;
    NbioIpBlockData->NbioConfigData.SyshubMgcgHysteresis = 0;
    NbioIpBlockData->NbioConfigData.NbifPgHysteresis = 0;
  }

  while (GnbHandle != NULL) {
    ///
    /// Blast table(s) specific to socket 0 (first socket)
    ///
    Property = PROPERTY_DEFAULT_DEVICE_PRESENCE;
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "Default properties for RB %d = 0x%X\n", GnbHandle->RBIndex, Property);
    Property &= ~(GnbHandle->Header.DescriptorFlags << 16);
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "Properties for RB %d = 0x%X\n", GnbHandle->RBIndex, Property);

    if (Property & PROPERTY_PRESENT_IOHC) {
      if (NbioIpBlockData->NbioConfigData.IOHCClkGatingSupport) {
        Property |= PROPERTY_IOHC_CLKGATING_ENABLED;
      } else {
        Property |= PROPERTY_IOHC_CLKGATING_DISABLED;
      }
      if (NbioIpBlockData->NbioConfigData.CfgSstunlClkGating) {
        Property |= PROPERTY_SST_CLKGATING_ENABLED;
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

    if (NbioIpBlockData->NbioConfigData.CfgNbifPgClkGating) {
      ProgramNbioSmnTable(GnbHandle, GnbnBifPgTable, NBIO_SPACE(GnbHandle, 0), Property);
    }
    if (NbioIpBlockData->NbioConfigData.SysHubPg) {
      ProgramNbioSmnTable(GnbHandle, GnbSysHubPgTable, NBIO_SPACE(GnbHandle, 0), Property);
    }
    if (NbioIpBlockData->NbioConfigData.EnableNbifOBFF) {
      ProgramNbioSmnTable(GnbHandle, GnbnBifOBFFCtrlTable, NBIO_SPACE(GnbHandle, 0), Property);
    }
    if (NbioIpBlockData->NbioConfigData.EnableNbifDmaOBFF) {
      ProgramNbioSmnTable(GnbHandle, GnbnBifOBFFDmaTable, NBIO_SPACE(GnbHandle, 0), Property);
    }
    if (NbioIpBlockData->NbioConfigData.CfgSyshubGdcMgcgClkGating) {
      ProgramNbioSmnTable(GnbHandle, GnbSysHubNgdcMgcgCtrCGTable, NBIO_SPACE(GnbHandle, 0), Property);
    }
    if (NbioIpBlockData->NbioConfigData.CfgSyshubMgcgHspClkGating) {
      ProgramNbioSmnTable(GnbHandle, GnbSysHubNgdcMgcgHspCGTable, NBIO_SPACE(GnbHandle, 0), Property);
    }

    ProgramNbioSmnTable(GnbHandle, GnbnBifInitTable, NBIO_SPACE(GnbHandle, 0), Property);

    if (!NbioIpBlockData->NbioConfigData.CfgIgpuControl) {
      ProgramNbioSmnTable(GnbHandle, GnbnBifiGpuDisable, NBIO_SPACE(GnbHandle, 0), Property);
      if (ISSOCPHXAM5) {
        xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          NBIO_SPACE(GnbHandle, NBIFMMx10134000),
          (uint32_t) ~BITS_0_TO_15_MASK_AAAABCY,
          (uint32_t)(0x145A << BIT0_OFFSET_AAAABCZ)
          );
      } else {
        xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          NBIO_SPACE(GnbHandle, NBIFMMx10134000),
          (uint32_t) ~BITS_0_TO_15_MASK_AAAABCY,
          (uint32_t)(0x14EC << BIT0_OFFSET_AAAABCZ)
          );
      }
    }

    ProgramNbioSmnTable(GnbHandle, GnbPCIe23InitTable, NBIO_SPACE(GnbHandle, 0), Property);

    if (NbioIpBlockData->NbioConfigData.Pcie23DynPowerGating) {
      ProgramNbioSmnTable(GnbHandle, GnbPCIe23DynamicPGTable, NBIO_SPACE(GnbHandle, 0), Property);
    }

    NbioBaseInitPhx(GnbHandle, NbioIpBlockData);
    AudioIOConfigCallback (GnbHandle, NbioIpBlockData);

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
        (uint32_t)((NbioIpBlockData->NbioConfigData.NbifMgcgHysteresis) <<
          BIT2_OFFSET_AAAABCV)
        );
    }
    if (NbioIpBlockData->NbioConfigData.SyshubMgcgHysteresis != 0) {
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        SYSHUBMMx1410020,
        (uint32_t) ~(BITS_2_TO_9_MASK_AAAAAOV),
        (uint32_t)((NbioIpBlockData->NbioConfigData.SyshubMgcgHysteresis) <<
          BIT2_OFFSET_AAAAAOW)
        );
    }

    if (NbioIpBlockData->NbioConfigData.NbifPgHysteresis != 0) {
      xUSLSmnReadModifyWrite(0,
        GnbHandle->Address.Address.Bus,
        NBIFMMx1013a0e0,
        (uint32_t) ~(0xff),
        (uint32_t)NbioIpBlockData->NbioConfigData.NbifPgHysteresis
        );
    }
    if (NbioIpBlockData->NbioConfigData.SyshubMgcgHspClkHysteresis != 0) {
      xUSLSmnReadModifyWrite(0,
        GnbHandle->Address.Address.Bus,
        SYSHUBMMx1412020,
        (uint32_t) ~(0x3fc),
        (uint32_t)NbioIpBlockData->NbioConfigData.SyshubMgcgHspClkHysteresis << 2
        );
    }
    if (NbioIpBlockData->NbioConfigData.SyshubGdcMgcgHysteresis != 0) {
      xUSLSmnReadModifyWrite(0,
        GnbHandle->Address.Address.Bus,
        SYSHUBMMx1403ba8,
        (uint32_t) ~(0x3fc),
        (uint32_t)NbioIpBlockData->NbioConfigData.SyshubGdcMgcgHysteresis << 2
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

    if (GnbHandle->RBIndex == 0) {
      NonPciPspBarInit(SilContext,
        GnbHandle,
        NBIO_SPACE(GnbHandle, IOHCx13b102d8),
        NBIO_SPACE(GnbHandle, IOHCx13b102dc),
        NONPCI_BARSIZE_1MB,
        true,
        true
        );
      NonPciPspBarInit(SilContext,
        GnbHandle,
        NBIO_SPACE(GnbHandle, IOHCx13b102e0),
        NBIO_SPACE(GnbHandle, IOHCx13b102e4),
        NONPCI_BARSIZE_1MB,
        true,
        true
        );
    }

    // DmaControlSetting (GnbHandle);

    // SysHub USB select
    if (NbioIpBlockData->NbioConfigData.UsbSysHubSelect != 0xFF) {
      xUSLSmnWrite(0,
        GnbHandle->Address.Address.Bus,
        SYSHUBMMx1411034,
        NbioIpBlockData->NbioConfigData.UsbSysHubSelect
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
