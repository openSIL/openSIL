/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  NbioBaseInitBrh.c
 * @brief This file contains API definition Nbio Base Initialization
 *
 */

#include <xSIM.h>
#include <Nbio/NbioIp2Ip.h>
#include <CommonLib/SmnAccess.h>
#include <Nbio/Brh/include/SdpmuxReg.h>
#include <Nbio/Brh/include/SyshubmmReg.h>
#include "NbioBrh.h"
#include "NbioSmnTable.h"
#include "NbioDefaults.h"
#include "GnbRegistersBrh.h"
#include "NbioCmn2RevBrh.h"
#include "NbioNbifBrhTbl.h"

/*----------------------------------------------------------------------------------------
 *             D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *            T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *         P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *         T A B L E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *         P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

// --------------------------------------------------
//   PPR: Non-VC1 write chain disable
// --------------------------------------------------
SMN_TABLE NbioNonVc1WriteTbl [] = {
  SMN_ENTRY_RMW(SIL_RESERVED_1704,
    A2S_CNTL_SW0_SDP_WR_CHAIN_DIS_MASK,
    (0x0 << A2S_CNTL_SW0_SDP_WR_CHAIN_DIS_OFFSET)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1713,
    A2S_CNTL_SW0_SDP_WR_CHAIN_DIS_MASK,
    (0x0 << A2S_CNTL_SW0_SDP_WR_CHAIN_DIS_OFFSET)
    ),
  SMN_ENTRY_RMW(SIL_RESERVED_1706,
    SIL_RESERVED_1696,
    (0x0 << SIL_RESERVED_1697)
    ),
  SMN_ENTRY_TERMINATE
};
// --------------------------------------------------
// 15.1.4.8 IOHC Clock Gating, dynamic
// --------------------------------------------------
//
SMN_TABLE NbioSstClockGatingTbl [] = {
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_SST_CLKGATING_ENABLED,
    SIL_RSVD_ADDR_17400004,
    SIL_RESERVED_1688 |
    SIL_RESERVED_1686,
    (0x1 << SIL_RESERVED_1689) |
    (0x1 << SIL_RESERVED_1687)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_SST_CLKGATING_DISABLED,
    SIL_RSVD_ADDR_17400004,
    SIL_RESERVED_1688 |
    SIL_RESERVED_1686,
    (0x0 << SIL_RESERVED_1689) |
    (0x0 << SIL_RESERVED_1687)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_SST_CLKGATING_ENABLED,
    SIL_RSVD_ADDR_17400004,
    SIL_RESERVED_1684,
    (0xF0 << SIL_RESERVED_1685)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_SST_CLKGATING_ENABLED,
    SIL_RSVD_ADDR_17400404,
    SIL_RESERVED_1680,
    (0x1 << SIL_RESERVED_1681)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_SST_CLKGATING_DISABLED,
    SIL_RSVD_ADDR_17400404,
    SIL_RESERVED_1680,
    (0x0 << SIL_RESERVED_1681)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_SST_CLKGATING_ENABLED,
    SIL_RESERVED_1683,
    SIL_RESERVED_1688 |
    SIL_RESERVED_1686,
    (0x1 << SIL_RESERVED_1689) |
    (0x1 << SIL_RESERVED_1687)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_SST_CLKGATING_DISABLED,
    SIL_RESERVED_1683,
    SIL_RESERVED_1688 |
    SIL_RESERVED_1686,
    (0x0 << SIL_RESERVED_1689) |
    (0x0 << SIL_RESERVED_1687)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_SST_CLKGATING_ENABLED,
    SIL_RESERVED_1683,
    SIL_RESERVED_1684,
    (0xF0 << SIL_RESERVED_1685)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_SST_CLKGATING_ENABLED,
    SIL_RESERVED_1682,
    SIL_RESERVED_1680,
    (0x1 << SIL_RESERVED_1681)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_SST_CLKGATING_DISABLED,
    SIL_RESERVED_1682,
    SIL_RESERVED_1680,
    (0x0 << SIL_RESERVED_1681)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_SST_CLKGATING_ENABLED,
    SIL_RSVD_ADDR_17700004,
    SIL_RESERVED_1688 |
    SIL_RESERVED_1686,
    (0x1 << SIL_RESERVED_1689) |
    (0x1 << SIL_RESERVED_1687)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_SST_CLKGATING_DISABLED,
    SIL_RSVD_ADDR_17700004,
    SIL_RESERVED_1688 |
    SIL_RESERVED_1686,
    (0x0 << SIL_RESERVED_1689) |
    (0x0 << SIL_RESERVED_1687)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_SST_CLKGATING_ENABLED,
    SIL_RSVD_ADDR_17700004,
    SIL_RESERVED_1684,
    (0xF0 << SIL_RESERVED_1685)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_SST_CLKGATING_ENABLED,
    SIL_RSVD_ADDR_17700404,
    SIL_RESERVED_1680,
    (0x1 << SIL_RESERVED_1681)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_SST_CLKGATING_DISABLED,
    SIL_RSVD_ADDR_17700404,
    SIL_RESERVED_1680,
    (0x0 << SIL_RESERVED_1681)
    ),
  SMN_ENTRY_TERMINATE
};
///
/// Each entry in this table is dispatched once per socket.
///
SMN_TABLE_ENTRY GnbOncePerSocketInitMP [] = {
  SMN_ENTRY_TABLE_64(NbioNonVc1WriteTbl),            // 15.6.7.5.9  : Non-VC1 write chain disable
  SMN_ENTRY_TABLE_64(NbioSstClockGatingTbl),
  SMN_ENTRY_TERMINATE_TABLE
};

// --------------------------------------------------
// 15.1.4.8 IOHC SDPMUX Clock Gating, dynamic
// --------------------------------------------------
//
SMN_TABLE NbioIohcSpdmuxClockGatingTbl [] = {
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_IOHC_CLKGATING_ENABLED | PROPERTY_PRESENT_SDPMUX,
    SIL_RESERVED_1678,
    SIL_RESERVED_1648 |
    SIL_RESERVED_1646 |
    SIL_RESERVED_1644 |
    SIL_RESERVED_1642 |
    SIL_RESERVED_1640 |
    SIL_RESERVED_1638 |
    SDPMUX_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK3_MASK |
    SIL_RESERVED_1636 |
    SIL_RESERVED_1634 |
    SIL_RESERVED_1632,
    (0x0 << SIL_RESERVED_1649) |
    (0x0 << SIL_RESERVED_1647) |
    (0x0 << SIL_RESERVED_1645) |
    (0x0 << SIL_RESERVED_1643) |
    (0x0 << SIL_RESERVED_1641) |
    (0x0 << SIL_RESERVED_1639) |
    (0x0 << SIL_RESERVED_1637) |
    (0x0 << SDPMUX_GLUE_CG_LCLK_CTRL_0_SOFT_OVERRIDE_CLK2_OFFSET) |
    (0x0 << SIL_RESERVED_1635) |
    (0x0 << SIL_RESERVED_1633)
    ),
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_IOHC_CLKGATING_ENABLED | PROPERTY_PRESENT_SDPMUX,
    SIL_RESERVED_1679,
    SIL_RESERVED_1665 |
    SIL_RESERVED_1664 |
    SIL_RESERVED_1662 |
    SIL_RESERVED_1660 |
    SIL_RESERVED_1658 |
    SIL_RESERVED_1656 |
    SIL_RESERVED_1655 |
    SIL_RESERVED_1653 |
    SDPMUX_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK1_MASK |
    SIL_RESERVED_1650,
    (0x0 << SIL_RESERVED_1666) |
    (0x0 << SDPMUX_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK8_OFFSET) |
    (0x0 << SIL_RESERVED_1663) |
    (0x0 << SIL_RESERVED_1661) |
    (0x0 << SIL_RESERVED_1659) |
    (0x0 << SIL_RESERVED_1657) |
    (0x0 << SDPMUX_GLUE_CG_LCLK_CTRL_1_SOFT_OVERRIDE_CLK3_OFFSET) |
    (0x0 << SIL_RESERVED_1654) |
    (0x0 << SIL_RESERVED_1652) |
    (0x0 << SIL_RESERVED_1651)
    ),
  SMN_ENTRY_TERMINATE
};

//
// --------------------------------------------------
// IOHC SDPMUX Performance
// --------------------------------------------------
//
SMN_TABLE  NbioIohcSpdmuxPerformanceTbl [] = {
  SMN_ENTRY_PROPERTY_WR(PROPERTY_PRESENT_SDPMUX,
    SIL_RSVD_ADDR_4400018,
    0x00010002
    ),
  SMN_ENTRY_PROPERTY_WR(PROPERTY_PRESENT_SDPMUX,
    SIL_RSVD_ADDR_4400014,
    0x00020001
    ),
  SMN_ENTRY_PROPERTY_WR(PROPERTY_PRESENT_SDPMUX,
    SIL_RSVD_ADDR_440001C,
    0x00020004
    ),
  SMN_ENTRY_PROPERTY_WR(PROPERTY_PRESENT_SDPMUX,
    SIL_RSVD_ADDR_4400024,
    0x00020001
    ),
  SMN_ENTRY_PROPERTY_WR(PROPERTY_PRESENT_SDPMUX,
    SIL_RSVD_ADDR_4400020,
    0x00010002
    ),
  SMN_ENTRY_TERMINATE
};

// --------------------------------------------------
// 15.1.4.11 IOHC Arbitration Control
// --------------------------------------------------
SMN_TABLE NbioIohcSpdmuxArbitrationTbl [] = {
  SMN_ENTRY_PROPERTY_WR(PROPERTY_PRESENT_SDPMUX,
    SIL_RESERVED_1667,
    0x04040404
    ),
  SMN_ENTRY_PROPERTY_WR(PROPERTY_PRESENT_SDPMUX,
    SIL_RESERVED_1670,
    0x04040404
    ),
  SMN_ENTRY_PROPERTY_WR(PROPERTY_PRESENT_SDPMUX,
    SIL_RESERVED_1674,
    0x04040404
    ),
  SMN_ENTRY_PROPERTY_WR(PROPERTY_PRESENT_SDPMUX,
    SIL_RESERVED_1668,
    0x04040404
    ),
  SMN_ENTRY_PROPERTY_WR(PROPERTY_PRESENT_SDPMUX,
    SIL_RESERVED_1671,
    0x04040404
    ),
  SMN_ENTRY_PROPERTY_WR(PROPERTY_PRESENT_SDPMUX,
    SIL_RESERVED_1675,
    0x04040404
    ),
  SMN_ENTRY_PROPERTY_WR(PROPERTY_PRESENT_SDPMUX,
    SIL_RESERVED_1669,
    0x04040404
    ),
  SMN_ENTRY_PROPERTY_WR(PROPERTY_PRESENT_SDPMUX,
    SIL_RESERVED_1672,
    0x04040404
    ),
  SMN_ENTRY_PROPERTY_WR(PROPERTY_PRESENT_SDPMUX,
    SIL_RESERVED_1676,
    0x04040404
    ),
  SMN_ENTRY_PROPERTY_WR(PROPERTY_PRESENT_SDPMUX,
    SMN_IOHUB0_N0NBIO0_SDPMUX_SION_S1_Client_Req_BurstTarget_Upper_ADDRESS,
    0x04040404
    ),
  SMN_ENTRY_PROPERTY_WR(PROPERTY_PRESENT_SDPMUX,
    SIL_RESERVED_1673,
    0x04040404
    ),
  SMN_ENTRY_PROPERTY_WR(PROPERTY_PRESENT_SDPMUX,
    SIL_RESERVED_1677,
    0x04040404
    ),
  SMN_ENTRY_TERMINATE
};

SMN_TABLE_ENTRY GnbSdpMuxInitTableCommon [] = {
  SMN_ENTRY_TABLE_64(NbioIohcSpdmuxClockGatingTbl),         // 15.1.4.8  IOHC SDPMUX Clock Gating
  SMN_ENTRY_TABLE_64(NbioIohcSpdmuxArbitrationTbl),         // 15.1.4.11 IOHC Arbitration Control
  SMN_ENTRY_TABLE_64(NbioIohcSpdmuxPerformanceTbl),         // IOHC Performance
  SMN_ENTRY_TERMINATE_TABLE
};

/*----------------------------------------------------------------------------------------*/
/**
 * Checks for requested CPU stepping
 *
 *  @param  CpuStep       CPU stepping to check for
 *
 * @retval     true if CPU stepping, FALSE Otherwise
 */

bool
IsCpuStepping (
  uint16_t        CpuStep
  )
{
  SOC_LOGICAL_ID    LogicalId;
  SIL_STATUS        Status;

  // Get logical family and revision for the executing core
  Status = GetSocLogicalIdOnCurrentCore(&LogicalId);
  if (Status == SilPass) {
    if (LogicalId.Revision & CpuStep) {
      return true;
    }
  }
  return false;
}

/**
 * Checks for requested CPU Family
 *
 *  @param  CpuFamily       CPU Family to check for
 *
 * @retval     true if CPU Family, FALSE Otherwise
 */

bool
IsCpuFamily (
  uint32_t        CpuFamily
  )
{
  SOC_LOGICAL_ID    LogicalId;
  SIL_STATUS        Status;

  // Get logical family and revision for the executing core
  Status = GetSocLogicalIdOnCurrentCore(&LogicalId);
  if (Status == SilPass) {
    if (LogicalId.Family & CpuFamily) {
      return true;
    }
  }
  return false;
}

/**
 * NbioBaseInitBrh
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
NbioBaseInitBrh (
  GNB_HANDLE            *GnbHandle,
  NBIOCLASS_DATA_BLOCK  *NbioIpBlockData
  )
{

  if ((GnbHandle == NULL) || (NbioIpBlockData == NULL)) {
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "ERROR : Invalid input argument\n");
    assert(false);
    return SilInvalidParameter;
  }

  if (NbioIpBlockData->NbioConfigData.SyshubMgcgHysteresis != 0) {
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, SIL_RESERVED_1712),
      (uint32_t) ~(SIL_RESERVED_1726),
      (NbioIpBlockData->NbioConfigData.SyshubMgcgHysteresis <<
        SIL_RESERVED_1727)
      );
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, SIL_RESERVED_1715),
      (uint32_t) ~(SIL_RESERVED_1726),
      (NbioIpBlockData->NbioConfigData.SyshubMgcgHysteresis <<
        SIL_RESERVED_1727)
      );
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, SIL_RESERVED_1707),
      (uint32_t) ~(SIL_RESERVED_1702),
      (NbioIpBlockData->NbioConfigData.SyshubMgcgHysteresis <<
        SIL_RESERVED_1703)
      );
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, SIL_RESERVED_1714),
      (uint32_t) ~(SIL_RESERVED_1702),
      (NbioIpBlockData->NbioConfigData.SyshubMgcgHysteresis <<
        SIL_RESERVED_1703)
      );
  }

  return SilPass;
}

/*----------------------------------------------------------------------------------------*/
/**
 * NbioBaseConfigurationBrh
 *
 * @brief  This function performs early NBIO initialization.
 *
 * @param  NBIOCLASS_DATA_BLOCK  Pointer to NBIO Data Block
 *
 * @retval SilPass              The Nbio base configuration is successful
 * @retval SilInvalidParameter  Error indicating invalid input arguments are passed
 *
 */
SIL_STATUS
NbioBaseConfigurationBrh (
  NBIOCLASS_DATA_BLOCK *NbioIpBlockData
  )
{
  uint32_t                Property;
  GNB_HANDLE              *GnbHandle;
  uint32_t                Value;
  NB_BUS_NUM_CNTL_STRUCT  BusNumCntl;
  uint32_t                CxlCfg2Addr;

  NBIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (NbioIpBlockData == NULL) {
    return SilInvalidParameter;
  }
  // Adjust Property based on Debug Info and PCD's
  // From Earlier
  GnbHandle = GetGnbHandle ();
  if (GnbHandle == NULL) {
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "ERROR : Invalid GnbHandle\n");
    assert(false);
    return SilInvalidParameter;
  }

  while (GnbHandle != NULL) {
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "Set SB_Location for RBIndex %d\n", GnbHandle->RBIndex);
    // IOHC needs to know the segment it resides on for MCTP message routing.
    // Setting the segment and bus information and LAT_Mode is set to 1
    BusNumCntl.Field.NB_SEGMENT = GnbHandle->Address.Address.Segment;
    BusNumCntl.Field.NB_BUS_NUM = GnbHandle->Address.Address.Bus;
    BusNumCntl.Field.NB_BUS_LAT_Mode = 1;

    if (GnbHandle->RBIndex < 4) {
      xUSLSmnWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_13B10044),
        BusNumCntl.Value
        );
      // Copy SB_LOCATION to the L1 and L2 in IOMMU. This only applies to NBIO3, where the FCH is located
      if (GnbHandle->SbPresent == 1) {
        Value = 0x00040001;    // We should be reading SMN_IOHUB0NBIO0_SB_LOCATION_ADDRESS

        xUSLSmnWrite(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_15300024),
          Value
          );
        xUSLSmnWrite(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_13F0112C),
          Value
          );
      } else {
        Value = 0;
        xUSLSmnWrite(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_13B1007C),
          Value
          );
        xUSLSmnWrite(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_15300024),
          Value
          );
        xUSLSmnWrite(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_13F0112C),
          Value
          );
      }
    } else {
      Value = 0;
      xUSLSmnWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_1D410044),
        BusNumCntl.Value
        );
      xUSLSmnWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_1D41007C),
        Value
        );
    }
    GnbHandle = GnbGetNextHandle(GnbHandle);
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

  GnbHandle = GetGnbHandle ();
  if (GnbHandle == NULL) {
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "ERROR : Invalid GnbHandle\n");
    assert(false);
    return SilInvalidParameter;
  }

  while (GnbHandle != NULL) {
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "Set device presence for RBIndex %d\n", GnbHandle->RBIndex);
    if (GnbHandle->RBIndex < 4) {
      if ((GnbHandle->RBIndex & 1) != 0) {
        GnbHandle->Header.DescriptorFlags |= SILICON_NOT_PRESENT_NBIF0;
        GnbHandle->Header.DescriptorFlags |= SILICON_NOT_PRESENT_NBIF2;
        GnbHandle->Header.DescriptorFlags |= SILICON_NOT_PRESENT_SDPMUX;
      } else {
        GnbHandle->Header.DescriptorFlags |= SILICON_NOT_PRESENT_NBIF1;
      }
      if ((GnbHandle->RBIndex & 0x2) != 0) {
        GnbHandle->Header.DescriptorFlags |= SILICON_NOT_PRESENT_SST;
      }
      NBIO_TRACEPOINT(SIL_TRACE_INFO,
        "Descriptor flags for RB %d = 0x%X \n",
        GnbHandle->RBIndex,
        GnbHandle->Header.DescriptorFlags
        );
    }
    GnbHandle = GnbGetNextHandle(GnbHandle);
  }

  GnbHandle = GetGnbHandle ();
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
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "Default properties for RB %d = 0x%X\n", GnbHandle->RBIndex, Property);
    Property &= ~(GnbHandle->Header.DescriptorFlags << 16);
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "Properties for RB %d = 0x%X\n", GnbHandle->RBIndex, Property);

    if (Property & PROPERTY_PRESENT_IOHC) {
      if (NbioIpBlockData->NbioConfigData.IOHCClkGatingSupport) {
        Property |= PROPERTY_IOHC_CLKGATING_ENABLED;
      } else if (NbioIpBlockData->NbioConfigData.IOHCClkGatingSupport == 1) {
        NBIO_TRACEPOINT(SIL_TRACE_INFO, "PROPERTY_IOHC_CLKGATING skipped");
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

    NBIO_TRACEPOINT(SIL_TRACE_INFO, "Actual Properties for RB %d = 0x%X\n", GnbHandle->RBIndex, Property);
    if ((GnbHandle->RBIndex & 0x3) == 0) {
      ///
      /// Blast table(s) specific to socket 1 (second socket)
      ///
      NBIO_TRACEPOINT(SIL_TRACE_INFO, "Blasting table GnbOncePerSocketInitMP for RB %d\n", GnbHandle->RBIndex);
      ProgramNbioSmnTable(GnbHandle, (SMN_TABLE *)GnbOncePerSocketInitMP, NBIO_SPACE(GnbHandle, 0), Property);
    }
    if ((GnbHandle->RBIndex & 1) == 0) {
      NbioBaseInitBrh(GnbHandle, NbioIpBlockData);
    }
    //For disabling SATA which is on NBIF1 Dev1 iohc2
    if (GnbHandle->RBIndex == 0) {
      NBIO_TRACEPOINT(SIL_TRACE_INFO, "Blasting table NbifSATAHideBridgeTbl for RB %d\n", GnbHandle->RBIndex);
      ProgramNbioSmnTable(GnbHandle, NbifSATAHideBridgeTbl, NBIO_SPACE(GnbHandle, 0), Property);
    }

    ///
    /// Bar allocation for SMN non pci devices
    /// Note that PSPP bar allocation is slightly different and is
    /// done elsewhere
    ///
    if (NbioIpBlockData->NbioConfigData.IohcNonPCIBarInitSmu) {
      NBIO_TRACEPOINT(SIL_TRACE_INFO, "(IohcNonPCIBarInitSmu) is true\n");
      NonPciBarInit(GnbHandle,
        NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_13B102E8),
        NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_13B102EC),
        NONPCI_BARSIZE_1MB,
        true,
        true,
        false
        );
    }

    ///
    /// For LCLK Deep Sleep enabling, PCIE6 is not utilized on
    /// Turin. Mask IOHC LCLK Deep Sleep for IOHC 2.
    ///
    if (IOHC_INDEX(GnbHandle) == 2) {
      Value = 2;
      xUSLSmnWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_13B10024),
        Value
        );

    }
    if ((IOHC_INDEX(GnbHandle) == 2) || (IOHC_INDEX(GnbHandle) == 1)) {
      // hide G link RCEC
      CxlCfg2Addr = ((IOHC_INDEX(GnbHandle) & BIT_32(0)) == 0) ?
        (SIL_RSVD_ADDR_13B158E8) : (SIL_RSVD_ADDR_1D414CDC);
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, CxlCfg2Addr),
        (uint32_t) ~(SIL_RESERVED_0704),
        (uint32_t) ~(0x1 << SIL_RESERVED_0602)
        );
    }
    if (GnbHandle->RBIndex < 4) {
      ///
      /// Blast tables that should be applied to each GnbHandle
      ///
      NBIO_TRACEPOINT(SIL_TRACE_INFO, "Blasting table GnbSdpMuxInitTableCommon for RB %d\n", GnbHandle->RBIndex);
      ProgramNbioSmnTable(GnbHandle, (SMN_TABLE *)GnbSdpMuxInitTableCommon, SDPMUX_SPACE(GnbHandle, 0), Property);
      NBIO_TRACEPOINT(SIL_TRACE_INFO, "Blasting table NbioPprInitValues for RB %d\n", GnbHandle->RBIndex);
      ProgramNbioSmnTable(GnbHandle, NbioPprInitValues, NBIO_SPACE(GnbHandle, 0), Property);
      NBIO_TRACEPOINT(SIL_TRACE_INFO, "Blasting table SyshubClockGatingTbl for RB %d\n", GnbHandle->RBIndex);
      ProgramNbioSmnTable(GnbHandle, SyshubClockGatingTbl, NBIO_SPACE(GnbHandle, 0), Property);
      NBIO_TRACEPOINT(SIL_TRACE_INFO, "Blasting table GnbEarlyInitTableCommon for RB %d\n", GnbHandle->RBIndex);
      ///
      /// Blast SdpMux tables that should be applied to each Socket
      ///
      ProgramNbioSmnTable(GnbHandle, (SMN_TABLE *)GnbEarlyInitTableCommon, NBIO_SPACE(GnbHandle, 0), Property);
      if (GnbHandle->RBIndex != 1) {
        NBIO_TRACEPOINT(SIL_TRACE_INFO, "Blasting table PcieHideBridgeTbl for RB %d\n", GnbHandle->RBIndex);
        ProgramNbioSmnTable(GnbHandle, PcieHideBridgeTbl, NBIO_SPACE(GnbHandle, 0), Property);
      }
      NbioNbifInit(GnbHandle, Property, NbioIpBlockData);
      if ((GnbHandle->RBIndex & 1) == 0) {
        NbioBaseInitBrh(GnbHandle, NbioIpBlockData);
      }
      //For disabling SATA which is on NBIF1 Dev1 iohc2
      if (GnbHandle->RBIndex == 0) {
        NBIO_TRACEPOINT(SIL_TRACE_INFO, "Blasting table NbifSATAHideBridgeTbl for RB %d\n", GnbHandle->RBIndex);
        ProgramNbioSmnTable(GnbHandle, NbifSATAHideBridgeTbl, NBIO_SPACE(GnbHandle, 0), Property);
      }

      if (((IsCpuStepping (AMD_REV_F1A_BRH_BX)) && IsCpuFamily (AMD_FAMILY_BRH))) {
        xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          NBIO_SPACE(GnbHandle, SIL_RESERVED_1704),
          (uint32_t) ~(SIL_RESERVED_1692),
          (0x1 << SIL_RESERVED_1693)
          );
        xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          NBIO_SPACE(GnbHandle, SIL_RESERVED_1705),
          (uint32_t) ~(SIL_RESERVED_1694),
          (0x1 << SIL_RESERVED_1695)
          );
      }
      ///
      /// Bar allocation for SMN non pci devices
      /// Note that PSPP bar allocation is slightly different and is
      /// done elsewhere
      ///
      if (NbioIpBlockData->NbioConfigData.IohcNonPCIBarInitSmu) {
        NonPciBarInit(GnbHandle,
          NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_13B102E8),
          NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_13B102EC),
          NONPCI_BARSIZE_1MB,
          true,
          true,
          false
          );
      }
      if (NbioIpBlockData->NbioConfigData.IohcNonPCIBarInitDbg) {
        NonPciBarInit(GnbHandle,
          NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_13B102F8),
          NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_13B102FC),
          NONPCI_BARSIZE_512KB,
          true,
          true,
          false
          );
      }
      if (NbioIpBlockData->NbioConfigData.IohcNonPCIBarInitFastReg) {
        NonPciBarInit(GnbHandle,
          NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_13B10300),
          NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_13B10304),
          NONPCI_BARSIZE_1MB,
          true,
          true,
          false
          );
      }
      if (NbioIpBlockData->NbioConfigData.IohcNonPCIBarInitFastRegCtl) {
        NonPciBarInit(GnbHandle,
          NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_13B10308),
          NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_13B1030C),
          NONPCI_BARSIZE_4KB,
          true,
          true,
          false
          );
      }
    } else {
      NBIO_TRACEPOINT(SIL_TRACE_INFO,
        "Blasting table GnbEarlyInitTableCommonSmallIohc for RB %d\n",
        GnbHandle->RBIndex
        );
      ProgramNbioSmnTable(GnbHandle,
        (SMN_TABLE *)GnbEarlyInitTableCommonSmallIohc,
        NBIO_SPACE(GnbHandle, 0),
        Property
        );
    }
    GnbHandle = GnbGetNextHandle(GnbHandle);
  }

  NBIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");

  return SilPass;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Find the "wrapper" to be removed, along with the pointer to the previous wrapper.
 *
 *
 * @param[in]     Wrapper   Wrapper configuration info
 * @param[in,out] Buffer    Buffer pointer points to the hidden Wrapper's Info
 * @param[in]     GnbHandle GnbHandle Pointer
 */
static
void
FindWrapperCallback (
  PCIe_WRAPPER_CONFIG               *Wrapper,
  void                              *Buffer,
  GNB_HANDLE                        *GnbHandle
  )
{
  HIDE_WRAPPER_INFO   *HideInfo;
  HideInfo = (HIDE_WRAPPER_INFO *) Buffer;

  if (Wrapper->WrapId == HideInfo->PcieCoreNum) {
    // Found the matching Wrapper. Save it to ThisWrapper.
    HideInfo->ThisPcieCore = Wrapper;
  } else {
    // If ThisWrapper has not been found, always set PrevWrapper with the current Wrapper
    if (HideInfo->ThisPcieCore == NULL) {
      HideInfo->PrevPcieCore = Wrapper;
    } else {
      // Only set NextWrapper once.
      if (HideInfo->NextPcieCore == NULL) {
        HideInfo->NextPcieCore = Wrapper;
      }
    }
  }
  return;
}

/**
 * Given the 1st wrapper on a non-zero NBIO GenHandle, find its previous wrapper
 * The previous wrapper is Last wrapper on GnbHandle - 1.
 * Save the previous wrapper to PrevWrapper.
 *
 *
 * @param[in]     Buffer    Save the hidden Wrapper's Info
 * @param[in,out] Wrapper   Wrapper to be hidden
 */
static
void
FindPrevWrapper (
  void                              *Buffer,
  PCIe_WRAPPER_CONFIG               *Wrapper
  )
{
  PCIe_SILICON_CONFIG  *Silicon = NULL;
  PCIe_SILICON_CONFIG  *PrevSilicon = NULL;
  PCIe_WRAPPER_CONFIG  *PrevWrapper = NULL;
  uint32_t              InstanceID = 0;
  HIDE_WRAPPER_INFO     *HideInfo = (HIDE_WRAPPER_INFO *)Buffer;
  PCIe_PLATFORM_CONFIG  *Pcie = NULL;

  Pcie = PcieConfigGetPlatform(Wrapper);

  // Get parent Silicon of Wrapper
  Silicon = (PCIe_SILICON_CONFIG *) PcieConfigGetParentSilicon(Wrapper);
  InstanceID = Silicon->InstanceId;

  // Invalid. InstanceID should be greater than 0 for this function.
  if (InstanceID == 0)
    return;

  // Get the Child Silicon from Pcie
  PrevSilicon = PcieConfigGetChildSilicon(Pcie);

  while (PrevSilicon != NULL) {
    // Look for the previous Silicon
    if (PrevSilicon->InstanceId == (InstanceID - 1))
      break;
    PrevSilicon = (PCIe_SILICON_CONFIG *)PcieConfigGetNextTopologyDescriptor(PrevSilicon,
      DESCRIPTOR_TERMINATE_TOPOLOGY
      );
  }
  // Added for solving coverity error, technically PrevSilicon will never be NULL.
  if (PrevSilicon == NULL)
    return;
  PrevWrapper = PcieConfigGetChildWrapper(PrevSilicon);

  while (PrevWrapper) {
    // Eventually, the last Wrapper will be saved
    HideInfo->PrevPcieCore = PrevWrapper;
    PrevWrapper = PcieConfigGetNextDescriptor(PrevWrapper);
  }

  return;
}

/**
 * Check if the PCIe controller is present. Otherwise, it will be removed from the complex.
 *
 * @param  Segment - PCI segment number
 * @param  Bus - PCI bus number
 * @param  RbIndex - root bridge index
 * @param  WrapperID - PCIe controller wrapper ID
 *
 * @retval     TRUE - PCIe bridge is present
 * @retval     FALSE - PCIe bridge is not present
 */
static
bool
IsPCIeControllerPresent (
  uint32_t   Segment,
  uint32_t   Bus,
  uint32_t   RbIndex,
  uint32_t   WrapperID
  )
{
  SIL_RESERVED_UNION_0029 ScratchReg;

  if (RbIndex < 4) {
    ScratchReg.Value = xUSLSmnRead(Segment,
      Bus,
      SIL_RSVD_ADDR_13B0006C + (RbIndex << 20)
      );

    if ((ScratchReg.Field.field_bit_0 == 1) && (WrapperID == 0)) {
      // If harvesting is supported, the PCIe bridge is NOT present
      return false;
    }
  }
  return true;
}

/**
 * Find the last engine of a given wrapper (PrevWrapper) by saving the pointer for each engine.
 * The last time this is called will save the pointer to the last one. *
 *
 *
 * @param[in]     Engine    Engineer on this Wrapper
 * @param[in,out] Buffer    Save the hidden Wrapper's Info
 * @param[in]     Wrapper   Wrapper to be hidden
 */
static
void
FindLastEngineOfPrevCallback (
  PCIe_ENGINE_CONFIG                *Engine,
  void                              *Buffer,
  PCIe_WRAPPER_CONFIG               *Wrapper
  )
{
  HIDE_WRAPPER_INFO     *HideInfo = (HIDE_WRAPPER_INFO *)Buffer;
  HideInfo->LastPciePortOnPrev = Engine;

  return;
}

/**
 * Find the last engine of a given wrapper (ThisWrapper) by saving the pointer for each engine.
 * The last time this is called will save the pointer to the last one. *
 *
 *
 * @param[in]     Engine    Engineer on this Wrapper
 * @param[in,out] Buffer    Save the hidden Wrapper's Info
 * @param[in]     Wrapper   Wrapper to be hidden
 */
static
void
FindLastEngineOfThisCallback (
  PCIe_ENGINE_CONFIG                *Engine,
  void                              *Buffer,
  PCIe_WRAPPER_CONFIG               *Wrapper
  )
{
  HIDE_WRAPPER_INFO     *HideInfo = (HIDE_WRAPPER_INFO *)Buffer;
  HideInfo->LastPciePortOnThis = Engine;

  return;
}

/**
 *  @brief Function to remove wrapper from a GnbHandle
 *
 *  @param [in] GnbHandle Pointer to the Silicon Descriptor
 *  @param [in] WrapperNum WrapId to be hidden
 *  @return SIL_STATUS
 *
 *  @details In the event of PCIe core power gating or harvesting, we may need to remove a wrapper from
 *  the PCIe complex.
 */
static
SIL_STATUS
HideWrapper (
  GNB_HANDLE     *GnbHandle,
  uint32_t        WrapperNum
  )
{
  HIDE_WRAPPER_INFO     HideInfo;
  PCIe_SILICON_CONFIG   *Silicon = (PCIe_SILICON_CONFIG *)GnbHandle;
  uint8_t               DeleteFirstWrapper = 0;
  uint8_t               DeleteLastWrapper = 0;

  HideInfo.PcieCoreNum = WrapperNum;
  HideInfo.PrevPcieCore = NULL;
  HideInfo.ThisPcieCore = NULL;
  HideInfo.NextPcieCore = NULL;
  HideInfo.FirstPciePortOnNext = NULL;
  HideInfo.LastPciePortOnThis = NULL;
  HideInfo.LastPciePortOnPrev = NULL;

  NBIO_TRACEPOINT(SIL_TRACE_INFO, "%a RBIndex %d PCIECORE %d\n", __FUNCTION__, GnbHandle->RBIndex, WrapperNum);

  // Update HideInfo with ThisWrapper, PrevWrapper, and NextWrapper
  PcieConfigRunProcForAllWrappersInNbio(DESCRIPTOR_ALL_WRAPPERS,
    FindWrapperCallback,
    &HideInfo,
    GnbHandle
    );

  // For the case of hiding "Wrapper 0 from NBIO 1, 2, 3, etc. Find the PrevWrapper from the last GnbHandle - 1
  if (HideInfo.PrevPcieCore == NULL) {
    DeleteFirstWrapper = 1;
    if (Silicon->InstanceId != 0)
      FindPrevWrapper(&HideInfo, HideInfo.ThisPcieCore);
  }

  // For the case of hiding the Last Wrapper, need to set NextWrapper to the first Wrapper from GnbHandle + 1
  if (HideInfo.NextPcieCore == NULL && HideInfo.ThisPcieCore != NULL) {
    HideInfo.NextPcieCore = (PCIe_WRAPPER_CONFIG *) PcieConfigGetNextTopologyDescriptor(HideInfo.ThisPcieCore,
      DESCRIPTOR_TERMINATE_TOPOLOGY
      );
    DeleteLastWrapper = 1;

  }

  // Find the 1st Engine on NextWrapper
  if (HideInfo.NextPcieCore != NULL) {
    HideInfo.FirstPciePortOnNext = PcieConfigGetChildEngine(HideInfo.NextPcieCore);
  } else {
    HideInfo.FirstPciePortOnNext = NULL;
  }
  // Find the Last Engine on PrevWrapper
  PcieConfigRunProcForAllEnginesInWrapper(DESCRIPTOR_PCIE_ENGINE,
    FindLastEngineOfPrevCallback,
    &HideInfo,
    HideInfo.PrevPcieCore
    );

  // Find the Last Engine on ThisWrapper
  PcieConfigRunProcForAllEnginesInWrapper(DESCRIPTOR_PCIE_ENGINE,
    FindLastEngineOfThisCallback,
    &HideInfo,
    HideInfo.ThisPcieCore
    );

  // First wrapper (Wrapper0) in GnbHandle
  if (DeleteFirstWrapper) {
    if (Silicon->InstanceId != 0) {
      PcieConfigAttachDescriptors(DESCRIPTOR_PCIE_WRAPPER,
        &HideInfo.PrevPcieCore->Header,
        &HideInfo.NextPcieCore->Header
        );
      PcieConfigAttachDescriptors(DESCRIPTOR_PCIE_ENGINE,
        &HideInfo.LastPciePortOnPrev->Header,
        &HideInfo.FirstPciePortOnNext->Header
        );
    }
    if (DeleteLastWrapper) {
      (&Silicon->Header)->Child = 0;
    } else {
      // Set Silicon's Child to Wrapper1
      PcieConfigAttachChild(&Silicon->Header, &HideInfo.NextPcieCore->Header);
    }
  }
  // Last wrapper in GnbHandle
  else if (DeleteLastWrapper) {
    PcieConfigSetDescriptorFlags(HideInfo.PrevPcieCore, HideInfo.ThisPcieCore->Header.DescriptorFlags);
    PcieConfigSetDescriptorFlags(HideInfo.LastPciePortOnPrev, HideInfo.LastPciePortOnThis->Header.DescriptorFlags);

    // Note: without this condition, the DescriptorFlags set above will be overwritten when NextWrapper = NULL.
    if (HideInfo.NextPcieCore) {
      PcieConfigAttachDescriptors(DESCRIPTOR_PCIE_WRAPPER,
        &HideInfo.PrevPcieCore->Header,
        &HideInfo.NextPcieCore->Header
        );
      PcieConfigAttachDescriptors(DESCRIPTOR_PCIE_ENGINE,
        &HideInfo.LastPciePortOnPrev->Header,
        &HideInfo.FirstPciePortOnNext->Header
        );
    }
  }
  // wrapper that is neither the first nor the last on the GnbHandle
  else {
    // The existing PcieConfigAttachDescriptors() will not work. so, trying this way.
    HideInfo.PrevPcieCore->Header.Peer = (uint16_t) ((uint8_t *) &HideInfo.NextPcieCore->Header -
      (uint8_t *) &HideInfo.PrevPcieCore->Header);
    HideInfo.LastPciePortOnPrev->Header.Peer = (uint16_t) ((uint8_t *) &HideInfo.FirstPciePortOnNext->Header -
      (uint8_t *) &HideInfo.LastPciePortOnPrev->Header);
  }

  NBIO_TRACEPOINT(SIL_TRACE_INFO, "%a Exit\n", __FUNCTION__);

  return SilPass;
}

/**
 * NbioEnumerateHarvestWrappersBrh
 *
 * @brief  Function to remove harvested PCIe cores from SOC Complex Descriptor
 *
 * @param  Pcie  Pointer to SOC Complex Descriptor
 *
 */
void
NbioEnumerateHarvestWrappersBrh (
  PCIe_PLATFORM_CONFIG *Pcie
  )
{
  PCIe_SILICON_CONFIG   *SiliconList;
  PCIe_WRAPPER_CONFIG   *WrapperList;
  PCIe_COMPLEX_CONFIG   *ComplexList;
  bool                   PCIeControllerPresent;

  PCIeControllerPresent = false;

  ComplexList = (PCIe_COMPLEX_CONFIG *) PcieConfigGetChild(DESCRIPTOR_COMPLEX, &Pcie->Header);
  NBIO_TRACEPOINT(SIL_TRACE_INFO, "NbioEnumerateHarvestWrappersBrh start.\n");

  while (ComplexList != NULL) {
    SiliconList = PcieConfigGetChildSilicon(ComplexList);
    while (SiliconList != NULL) {
      WrapperList = PcieConfigGetChildWrapper(SiliconList);
      while (WrapperList != NULL) {
        PCIeControllerPresent = IsPCIeControllerPresent(SiliconList->Address.Address.Segment,
          SiliconList->Address.Address.Bus,
          SiliconList->RBIndex,
          WrapperList->WrapId
          );
        if (!PCIeControllerPresent) {
          HideWrapper(SiliconList, WrapperList->WrapId);
        }
        /* coverity[ptr_arith] */
        WrapperList = PcieLibGetNextDescriptor(WrapperList);
      }
      /* coverity[ptr_arith] */
      SiliconList = PcieLibGetNextDescriptor(SiliconList);
    }
    if ((ComplexList->Header.DescriptorFlags & DESCRIPTOR_TERMINATE_TOPOLOGY) == 0) {
      /* coverity[ptr_arith] */
      ComplexList++;
    } else {
      ComplexList = NULL;
    }
  }
  NBIO_TRACEPOINT(SIL_TRACE_INFO, "EnumerateAndHarvestWrappers end.\n");
  return;
}
