/**
 * @file NbioIommu.c
 * @brief OpenSIL NBIO Iommu initialization module.
 *
 */

/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <NbioData.h>
#include <NbioPcieTopologyHelper.h>
#include <NbioCommon.h>
#include <CCX/Common/CcxApic.h>
#include <RcMgr/RcMgrIp2Ip.h>
#include <CommonLib/SmnAccess.h>
#include "NbioSmnTable.h"
#include "GnbRegisters.h"
#include <SilSocLogicalId.h>

#define  SIZE_512KB  0x00080000
#define  SMN_IOAGR_L1_INTRPT_ORDER_CTRL_ADDRESS_A0                0x153001fcUL

/* Nbio Configuration data */
extern NBIOCLASS_DATA mNbioIpBlockData;
extern SMN_TABLE GnbIommuEnvInitTable [];

/*----------------------------------------------------------------------------------------*/
/**
 * NbioIOMMUInit
 *
 * Initialize IOMMU. Program NBIO IOMMU tables for features such as
 * L1/L2 clock gating, TW Filter Disable, Disable IOMMU if IOMMU enable
 * config is set to false. Allocate MMIO space for IOMMU controller.
 * Read modify Write IOMMU register based on a NBIO input parameter.
 *
 * @return SIL_STATUS
 * @retval SilPass - everything is OK
 * @retval SilAborted- Failed to allocate MMIO resources or program IOMMU NBIO tables.
 *
 */
SIL_STATUS
NbioIOMMUInit (void)
{

  FABRIC_TARGET                      MmioTarget;
  FABRIC_MMIO_ATTRIBUTE              MmioAttr;
  uint64_t                           IommMmioSize;
  uint64_t                           IommMmioBase;
  GNB_HANDLE                         *GnbHandle;
  SIL_STATUS                         Status;
  PCI_ADDR                           IommuPciAddress;
  uint32_t                           Value;
  bool                               ReserveIommuBar;
  uint32_t                           Property;
  SIL_RESERVED_3                     MmioControl0;
  SOC_LOGICAL_ID                     LogicalId;
  RCMGR_IP2IP_API                    *RcMgrIp2Ip;

  Property = NBIO_TABLE_PROPERTY_DEFAULT;
  Status = SilPass;
  if (mNbioIpBlockData.NbioConfigData->IommuL1ClockGatingEnable) {
    Property |= PROPERTY_IOMMU_L1CLKGATING_ENABLED;
  } else {
    Property |= PROPERTY_IOMMU_L1CLKGATING_DISABLED;
  }
  if (mNbioIpBlockData.NbioConfigData->IommuL2ClockGatingEnable) {
    Property |= PROPERTY_IOMMU_L2CLKGATING_ENABLED;
  } else {
    Property |= PROPERTY_IOMMU_L2CLKGATING_DISABLED;
  }
  if (xApicMode == mNbioIpBlockData.NbioConfigData->AmdApicMode) {
    Property |= PROPERTY_XAPIC_MODE;
  }
  if (false == mNbioIpBlockData.NbioConfigData->IommuSupport) {
    Property |= PROPERTY_IOMMU_DISABLED;
  }

  NBIO_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  if (mNbioIpBlockData.NbioConfigData->IommuMMIOAddressReservedEnable == 0x00) {
    NBIO_TRACEPOINT (SIL_TRACE_INFO, " We don't need reserved IOMMU MMIO space from GNB module \n");
    ReserveIommuBar = false;
  } else {
    NBIO_TRACEPOINT (SIL_TRACE_INFO, " Will reserve IOMMU Bars \n");
    ReserveIommuBar = true;
  }

  GnbHandle = GetGnbHandle ();

  if (SilGetIp2IpApi (SilId_RcManager, (void **)(&RcMgrIp2Ip)) != SilPass) {
    NBIO_TRACEPOINT (SIL_TRACE_ERROR, " MMIO allocator API is not found.\n");
    return SilNotFound;
  }

  // Loop to program each GNB Handle appropriately
  while (GnbHandle != NULL) {

    NBIO_TRACEPOINT (SIL_TRACE_INFO, " GnbHandle = 0x%x\n" , GnbHandle);

    // Allocate BAR for IOMMU
    if (ReserveIommuBar) {
      MmioTarget.TgtType = TARGET_RB;
      MmioTarget.SocketNum = GnbHandle->SocketId;
      MmioTarget.RbNum = GnbHandle->RBIndex;
      MmioTarget.PciBusNum = (uint16_t) GnbHandle->Address.Address.Bus;
      NBIO_TRACEPOINT (SIL_TRACE_INFO, "iommu rb_index: %d/%08x\n",
              MmioTarget.RbNum, MmioTarget.PciBusNum);

      IommMmioSize = SIZE_512KB;
      MmioAttr.MmioType = NON_PCI_DEVICE_BELOW_4G;
      Status = RcMgrIp2Ip->FabricReserveMmio(&IommMmioBase, &IommMmioSize, ALIGN_512K, MmioTarget, &MmioAttr);
      NBIO_TRACEPOINT (SIL_TRACE_INFO, " IOMMU MMIO at address 0x%x for Socket 0x%x Silicon 0x%x\n" ,
              IommMmioBase, GnbHandle->SocketId, GnbHandle->DieNumber);

      if (Status != SilPass) {
          NBIO_TRACEPOINT (SIL_TRACE_INFO, "Failed to allocate IoApic mmio space\n");
          return SilAborted;
      }
      Value = (uint32_t)IommMmioBase;
      IommuPciAddress = NbioGetHostPciAddress (GnbHandle);
      IommuPciAddress.Address.Function = 0x2;
      xUSLPciWrite32 (IommuPciAddress.AddressValue | PCICFG_NBIO0_IOHUB0_IOMMU_CAP_BASE_LO_OFFSET, Value);
    }

    // Program up IOMMU NBIO Tables
    ProgramNbioSmnTable (GnbHandle, GnbIommuEnvInitTable, NBIO_SPACE (GnbHandle, 0), Property);


    if (mNbioIpBlockData.NbioConfigData->AmdApicMode != xApicMode) {
      IommuPciAddress = NbioGetHostPciAddress (GnbHandle);
      IommuPciAddress.Address.Function = 0x2;
      MmioControl0.Value = xUSLPciRead32(IommuPciAddress.AddressValue | PCICFG_NBIO0_IOHUB0_IOMMU_MMIO_CONTROL0_W_OFFSET);
      MmioControl0.Field.XT_SUP_W = 1;
      MmioControl0.Field.GA_SUP_W = 1;
      xUSLPciWrite32(IommuPciAddress.AddressValue | PCICFG_NBIO0_IOHUB0_IOMMU_MMIO_CONTROL0_W_OFFSET, MmioControl0.Value);
    }

    // IOMMU configuration for SEV-SNP enabled
    if (mNbioIpBlockData.NbioInputBlk->SevSnpSupport == true) {
      xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus,
        NBIO_SPACE (GnbHandle, SMN_IOHUB0NBIO0_IOMMU_MMIO_CONTROL0_W_ADDRESS),
        (uint32_t)~(IOMMU_MMIO_CONTROL0_W_GAM_SUP_W_MASK),
                  (0 << IOMMU_MMIO_CONTROL0_W_GAM_SUP_W_OFFSET));
      xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus,
        NBIO_SPACE (GnbHandle, SIL_RESERVED3_1501),
        (uint32_t)~(SIL_RESERVED3_1500),
                  (0 << SIL_RESERVED3_1499));

      // IOMMU General AVIC modes support
      xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus,
        NBIO_SPACE (GnbHandle, SMN_IOHUB0NBIO0_IOMMU_MMIO_CONTROL0_W_ADDRESS),
        (uint32_t)~(IOMMU_MMIO_CONTROL0_W_GAM_SUP_W_MASK),
              ((mNbioIpBlockData.NbioConfigData->IommuAvicSupport ? 1 : 0) << IOMMU_MMIO_CONTROL0_W_GAM_SUP_W_OFFSET));
      // IOMMU SNPAVIC support
      NBIO_TRACEPOINT (SIL_TRACE_INFO, "Enable SNPAVICSup to %d\n",
               (mNbioIpBlockData.NbioConfigData->IommuAvicSupport? 1 : 0));
      xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus,
        NBIO_SPACE (GnbHandle, SMN_IOHUB0NBIO0_IOMMU_MMIO_CONTROL0_W_ADDRESS),
        (uint32_t)~(IOMMU_MMIO_CONTROL0_W_SNPAVICSup_W_MASK),
        ((mNbioIpBlockData.NbioConfigData->IommuAvicSupport ? 1 : 0) << IOMMU_MMIO_CONTROL0_W_SNPAVICSup_W_OFFSET));
      Value = xUSLSmnRead (GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus,
        NBIO_SPACE (GnbHandle, SMN_IOHUB0NBIO0_IOMMU_MMIO_CONTROL0_W_ADDRESS));
      NBIO_TRACEPOINT (SIL_TRACE_INFO, "Read back from IOMM_MMIO_CONTRO0_W 0x%x\n", Value);

    } else {
      // IOMMU configuration for SEV-SNP disabled
      xUSLSmnReadModifyWrite (0,  GnbHandle->Address.Address.Bus,
        NBIO_SPACE (GnbHandle, SMN_IOHUB0NBIO0_IOMMU_MMIO_CONTROL1_W_ADDRESS),
        (uint32_t)~(IOMMU_MMIO_CONTROL1_W_SNP_SUP_W_MASK),
            (0 << IOMMU_MMIO_CONTROL1_W_SNP_SUP_W_OFFSET));

      // IOMMU General AVIC modes support
      xUSLSmnReadModifyWrite (0,  GnbHandle->Address.Address.Bus,
        NBIO_SPACE (GnbHandle, SMN_IOHUB0NBIO0_IOMMU_MMIO_CONTROL0_W_ADDRESS),
        (uint32_t)~(IOMMU_MMIO_CONTROL0_W_GAM_SUP_W_MASK),
              (mNbioIpBlockData.NbioConfigData->IommuAvicSupport ? 1 : 0) << IOMMU_MMIO_CONTROL0_W_GAM_SUP_W_OFFSET);
      xUSLSmnReadModifyWrite (0,  GnbHandle->Address.Address.Bus,
        NBIO_SPACE (GnbHandle, SIL_RESERVED3_1501),
        (uint32_t)~(SIL_RESERVED3_1500),
              (mNbioIpBlockData.NbioConfigData->IommuAvicSupport ? 1 : 0) << SIL_RESERVED3_1499);
    }

    //Enable relaxed interrupt ordering for A0. Its enabled by default in B0
    Status = GetSocLogicalIdOnCurrentCore(&LogicalId);
    if (Status == SilPass)
    {
        if((LogicalId.Revision & (AMD_REV_F19_GENOA_AX)) && (LogicalId.Family & AMD_FAMILY_GENOA))
        {
            Value = 0xFFFF;
            xUSLSmnWrite (GnbHandle->Address.Address.Segment,GnbHandle->Address.Address.Bus,
                                 NBIO_SPACE(GnbHandle, SMN_IOAGR_L1_INTRPT_ORDER_CTRL_ADDRESS_A0), Value);
        }
    }

    // Sets the GAM_SUP value in IOMMU Extended feature register.
    xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus,
        NBIO_SPACE (GnbHandle, SMN_IOMMU_MMIO_CONTROL0_W_ADDRESS),
        (uint32_t)~(IOMMU_MMIO_CONTROL0_W_GAM_SUP_W_MASK),
            (1 << IOMMU_MMIO_CONTROL0_W_GAM_SUP_W_OFFSET));

    GnbHandle = GnbGetNextHandle (GnbHandle);
  }

  NBIO_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SilPass;
}
