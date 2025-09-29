/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file NbioIommu.c
 * @brief OpenSIL NBIO Iommu initialization module.
 *
 */

#include <Nbio/NbioIp2Ip.h>
#include <CCX/Common/CcxApic.h>
#include <RcMgr/RcMgrIp2Ip.h>
#include <CommonLib/SmnAccess.h>
#include <Nbio/Phx/includePHX/PHX_IOMMUL2.h>
#include <Nbio/Phx/includePHX/PHX_IOHC.h>
#include <Nbio/Phx/includePHX/PHX_IOMMUL1.h>
#include <Nbio/Phx/includePHX/PHX_GnbRegistersPhx.h>

#include "NbioSmnTable.h"
#include "NbioIommuPhxTbl.h"
#include "NbioSmnTable.h"
#include <SilSocLogicalId.h>
#include <xSIM-api.h>

/*
 *  This table contains the Specific configuration for IOMMU
 */
const SMN_TABLE GnbIommuEnvInitTable [] = {
  NBIO_IOMMU_L1_INIT_TBL       // IOMMU L1 Initialization
  NBIO_IOMMU_L2_INIT_TBL       // IOMMU L2 Initialization
  NBIO_IOMMU_CLOCK_GATING_TBL  // IOMMU Clock Gating
  //
  // Configure IOMMU Power Island, hide IOMMU function if disabled
  //
  SMN_ENTRY_PROPERTY_RMW(PROPERTY_IOMMU_DISABLED,
    IOHCx13b10034,
    BIT0_MASK_AAAAATB,
    (0x0 << BIT0_OFFSET_AAAAATC)
    ),

  SMN_ENTRY_RMW(IOMMUL2x13f01150,
    L2_TW_CONTROL_TWForceCoherent_MASK,
    (1 << L2_TW_CONTROL_TWForceCoherent_OFFSET)
    ),
  // Disable IO Read/Writes Reordering
  SMN_ENTRY_RMW(IOMMUL1x148001f4,
    BITS_17_TO_18_MASK_AAAAALR,
    (0 << BIT17_OFFSET_AAAAALS)
    ),
  SMN_ENTRY_RMW(IOMMUL1x147001f4,
    BITS_17_TO_18_MASK_AAAAALR,
    (0 << BIT17_OFFSET_AAAAALS)
    ),
  SMN_ENTRY_TERMINATE
};

/*----------------------------------------------------------------------------------------*/
/**
 * NbioIommuInit
 *
 * Initialize IOMMU. Program NBIO IOMMU tables for features such as
 * L1/L2 clock gating, TW Filter Disable, Disable IOMMU if IOMMU enable
 * config is set to false. Allocate MMIO space for IOMMU controller.
 * Read modify Write IOMMU register based on a NBIO input parameter.
 *
 * @param  SilContext            A context structure through which host firmware defined data
 *                               can be passed to openSIL. The host firmware is responsible
 *                               for initializing the SIL_CONTEXT structure.
 * @param  NBIOCLASS_DATA_BLOCK  Pointer to NBIO Data Block
 *
 * @return SIL_STATUS
 * @retval SilPass - everything is OK
 * @retval SilAborted- Failed to allocate MMIO resources or program IOMMU NBIO tables.
 *
 */
SIL_STATUS
NbioIommuInit (
  SIL_CONTEXT           *SilContext,
  NBIOCLASS_DATA_BLOCK  *NbioIpBlockData
  )
{

  FABRIC_TARGET                      MmioTarget = {0};
  FABRIC_MMIO_ATTRIBUTE              MmioAttr;
  uint64_t                           IommMmioSize;
  uint64_t                           IommMmioBase;
  GNB_HANDLE                         *GnbHandle;
  SIL_STATUS                         Status;
  PCI_ADDR                           IommuPciAddress;
  uint32_t                           Value;
  bool                               ReserveIommuBar;
  uint32_t                           Property;
  RCMGR_IP2IP_API                    *RcMgrIp2Ip;
  uint8_t                            Index;

  Property = NBIO_TABLE_PROPERTY_DEFAULT;
  Status = SilPass;

  if (NbioIpBlockData == NULL) {
    assert(false);
    return SilInvalidParameter;
  }

  if (NbioIpBlockData->NbioConfigData.IommuL1ClockGatingEnable) {
    Property |= PROPERTY_IOMMU_L1CLKGATING_ENABLED;
  } else {
    Property |= PROPERTY_IOMMU_L1CLKGATING_DISABLED;
  }
  if (NbioIpBlockData->NbioConfigData.IommuL2ClockGatingEnable) {
    Property |= PROPERTY_IOMMU_L2CLKGATING_ENABLED;
  } else {
    Property |= PROPERTY_IOMMU_L2CLKGATING_DISABLED;
  }
  if (xApicMode == NbioIpBlockData->NbioConfigData.AmdApicMode) {
    Property |= PROPERTY_XAPIC_MODE;
  }
  if (false == NbioIpBlockData->NbioConfigData.IommuSupport) {
    Property |= PROPERTY_IOMMU_DISABLED;
  }

  NBIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (NbioIpBlockData->NbioConfigData.IommuMMIOAddressReservedEnable == 0x00) {
    NBIO_TRACEPOINT(SIL_TRACE_INFO, " We don't need reserved IOMMU MMIO space from GNB module \n");
    ReserveIommuBar = false;
  } else {
    NBIO_TRACEPOINT(SIL_TRACE_INFO, " Will reserve IOMMU Bars \n");
    ReserveIommuBar = true;
  }

  GnbHandle = GetGnbHandle(SilContext);

  if (SilGetIp2IpApi(SilContext, SilId_RcManager, (void **)(&RcMgrIp2Ip)) != SilPass) {
    NBIO_TRACEPOINT(SIL_TRACE_ERROR, " MMIO allocator API is not found.\n");
    return SilNotFound;
  }

  // Loop to program each GNB Handle appropriately
  while (GnbHandle != NULL) {

    NBIO_TRACEPOINT(SIL_TRACE_INFO, " GnbHandle = 0x%x\n", GnbHandle);

    // Allocate BAR for IOMMU
    if (ReserveIommuBar) {
      NBIO_TRACEPOINT(SIL_TRACE_INFO, "%a : Will reserve IOMMU Bars \n", __FUNCTION__);

      MmioTarget.TgtType = TARGET_RB;
      MmioTarget.SocketNum = GnbHandle->SocketId;
      MmioTarget.RbNum = GnbHandle->RBIndex;
      MmioTarget.PciBusNum = (uint16_t) GnbHandle->Address.Address.Bus;
      MmioTarget.PciSegNum = 0;
      NBIO_TRACEPOINT(SIL_TRACE_INFO,
        "iommu rb_index: %d/%08x\n",
        MmioTarget.RbNum,
        MmioTarget.PciBusNum
        );

      IommMmioSize = SIZE_512KB;
      MmioAttr.MmioType = NON_PCI_DEVICE_BELOW_4G;
      Status = RcMgrIp2Ip->FabricReserveMmio(SilContext,
        &IommMmioBase,
        &IommMmioSize,
        ALIGN_512K,
        MmioTarget,
        &MmioAttr
        );
      NBIO_TRACEPOINT(SIL_TRACE_INFO,
        " IOMMU MMIO at address 0x%x for Socket 0x%x Silicon 0x%x\n",
        IommMmioBase,
        GnbHandle->SocketId,
        GnbHandle->DieNumber
        );

      if (Status != SilPass) {
        NBIO_TRACEPOINT(SIL_TRACE_INFO, "Failed to allocate IoApic mmio space\n");
        return SilAborted;
      }
      Value = (uint32_t)IommMmioBase;
      IommuPciAddress = NbioGetHostPciAddress(GnbHandle);
      IommuPciAddress.Address.Function = 0x2;

      // If IOMMU is enabled set MMIO enable bit
      if (NbioIpBlockData->NbioConfigData.IommuSupport == true) {
        Value = Value | 1;
        NBIO_TRACEPOINT(SIL_TRACE_INFO, "%a : Hit enable bit for IOMMU BAR 0x%x\n", __FUNCTION__, Value);
      } else {
        NBIO_TRACEPOINT(SIL_TRACE_INFO, "%a : Did NOT enable bit for IOMMU BAR 0x%x\n", __FUNCTION__, Value);
      }

      xUSLPciWrite32(IommuPciAddress.AddressValue | BIT44_OFFSET_AAAAASR, Value);
    } else {
      NBIO_TRACEPOINT(SIL_TRACE_INFO, "%a : System is configured to not enable MMIO \n", __FUNCTION__);
    }

    // Program up IOMMU NBIO Tables
    ProgramNbioSmnTable(GnbHandle, GnbIommuEnvInitTable, NBIO_SPACE(GnbHandle, 0), Property);


    if (NbioIpBlockData->NbioConfigData.AmdApicMode != xApicMode) {
      for (Index = 0; Index < 4; Index++) {
        xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          IOMMUL1x147000dc + (0x100000 * Index),
          (uint32_t) ~(L1_FEATURE_SUP_CNTRL_L1_XT_SUP_W_MASK),
          (1 << L1_FEATURE_SUP_CNTRL_L1_XT_SUP_W_OFFSET)
          );
      }
      if (NbioIpBlockData->NbioConfigData.IommuSupport == true) {

        IommuPciAddress = NbioGetHostPciAddress(GnbHandle);
        IommuPciAddress.Address.Function = 0x2;
        Value = 0;
        Value = xUSLSmnRead(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          NBIO_SPACE(GnbHandle, SMN_IOMMU_MMIO_CONTROL0_W_ADDRESS)
          );

        Value |= BIT_32(2);
        xUSLSmnWrite(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          NBIO_SPACE(GnbHandle, SMN_IOMMU_MMIO_CONTROL0_W_ADDRESS),
          Value
          );
      }
    }

    GnbHandle = GnbGetNextHandle(GnbHandle);
  }

  NBIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}
