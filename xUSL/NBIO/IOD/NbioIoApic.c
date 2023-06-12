/**
 * @file  NbioIoApic.c
 * @brief OpenSIL NBIO IoApic initialization
 */

/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include "NbioIoApic.h"
#include "NbioCommon.h"
#include "NbioData.h"
#include <RcMgr/RcMgrIp2Ip.h>
#include "NbioPcieTopologyHelper.h"
#include <SilPcie.h>
#include <CommonLib/CpuLib.h>
#include "GnbRegisters.h"
#include <string.h>

#define  SIZE_1KB    0x00000400

extern NBIOCLASS_DATA mNbioIpBlockData;

/**
 * NbioIoApicInit
 *
 * @brief  This function is resposible to allocate the mmio space for IoApic.
 *         Also assign the IoApic base ID for each IOHC instance.
 *
 * @return SIL_STATUS
 * @retval SilPass - everything is OK
 * @retval SilAborted- Failed to allocate MMIO resources.
 */
SIL_STATUS
NbioIoApicInit (void)
{
  FABRIC_TARGET                      MmioTarget;
  FABRIC_MMIO_ATTRIBUTE              MmioAttr;
  uint64_t                           IoapicMmioSize;
  uint64_t                           IoapicMmioBase;
  GNB_HANDLE                         *GnbHandle;
  SIL_STATUS                         Status;
  RCMGR_IP2IP_API                    *RcMgrIp2Ip;

  NBIO_TRACEPOINT (SIL_TRACE_INFO, "Entry\n");

    if (mNbioIpBlockData.NbioConfigData->IoApicMMIOAddressReservedEnable) {
      if (SilGetIp2IpApi (SilId_RcManager, (void **)(&RcMgrIp2Ip)) != SilPass) {
        NBIO_TRACEPOINT (SIL_TRACE_ERROR, " MMIO allocator API is not found.\n");
        return SilNotFound;
      }

      GnbHandle = GetGnbHandle ();
      while (GnbHandle != NULL) {
        memset (&MmioAttr, 0, sizeof(MmioAttr));
        memset (&MmioTarget, 0, sizeof(MmioTarget));
        MmioTarget.TgtType = TARGET_PCI_BUS;
        MmioTarget.SocketNum = GnbHandle->SocketId;
        assert (MmioTarget.SocketNum < MAX_SOCKETS_SUPPORTED);
        MmioTarget.RbNum = GnbHandle->DieNumber;
        MmioTarget.PciBusNum = (uint16_t) GnbHandle->Address.Address.Bus;
        MmioTarget.PciSegNum = (uint16_t) GnbHandle->Address.Address.Segment;
        IoapicMmioSize = SIZE_1KB / 4;
        MmioAttr.ReadEnable = 1;
        MmioAttr.WriteEnable = 1;
        MmioAttr.NonPosted = 0;
        MmioAttr.MmioType = NON_PCI_DEVICE_BELOW_4G;
        IoapicMmioBase = 0;
        Status = RcMgrIp2Ip->FabricReserveMmio (&IoapicMmioBase, &IoapicMmioSize, ALIGN_64K, MmioTarget, &MmioAttr);
        if (Status == SilPass) {
          uint32_t BarLow, BarHigh;
          NBIO_TRACEPOINT (SIL_TRACE_INFO,
                           "IOAPIC MMIO at address 0x%x for Socket:0x%x Bus:0x%x\n",
                           IoapicMmioBase,
                           MmioTarget.SocketNum,
                           MmioTarget.PciBusNum
                           );
          ///
          /// Write the assigned memory address registers to SMN
          ///
          BarLow  = (uint32_t) IoapicMmioBase;
          BarHigh = (uint32_t) (IoapicMmioBase >> 32);
          xUSLSmnWrite (0,
                        GnbHandle->Address.Address.Bus,
                        NBIO_SPACE(GnbHandle, SMN_IOHUB0NBIO0_IOAPIC_BASE_ADDR_HI_ADDRESS),
                        BarHigh
                        );
          xUSLSmnWrite (0,
                        GnbHandle->Address.Address.Bus,
                        NBIO_SPACE(GnbHandle, SMN_IOHUB0NBIO0_IOAPIC_BASE_ADDR_LO_ADDRESS),
                        BarLow
                        );
        } else {
          NBIO_TRACEPOINT (SIL_TRACE_INFO, "Failed to allocate IoApic mmio space\n");
          return SilAborted;
        }
        GnbHandle = GnbGetNextHandle (GnbHandle);
      }
    } else {
      NBIO_TRACEPOINT (SIL_TRACE_INFO, "We don't need reserved IOAPIC MMIO space\n");
    }

    if (mNbioIpBlockData.NbioConfigData->IoApicIdPreDefineEn) {
      uint8_t     ApicIdBase;
      uint32_t    Value32;

      GnbHandle = GetGnbHandle ();
      while (GnbHandle != NULL) {
        ApicIdBase = mNbioIpBlockData.NbioConfigData->IoApicIdBase;
        ApicIdBase += (GnbHandle->SocketId * 4) + GnbHandle->RBIndex;
        Value32 = ((uint32_t) ApicIdBase) << 24;
        xUSLSmnWrite (0,
                      GnbHandle->Address.Address.Bus,
                      NBIO_SPACE(GnbHandle, SMN_IOHUB0NBIO0_IOAPIC_ID_REGISTER_ADDRESS),
                      Value32
                      );
        NBIO_TRACEPOINT (SIL_TRACE_INFO,
                         "Pre-Define IOAPIC ID Addr=0x%x, Value=0x%x\n",
                         NBIO_SPACE(GnbHandle, SMN_IOHUB0NBIO0_IOAPIC_ID_REGISTER_ADDRESS),
                         Value32
                         );

        GnbHandle = GnbGetNextHandle (GnbHandle);
      }

    } else {
      NBIO_TRACEPOINT (SIL_TRACE_INFO, "Pre-Define IOAPIC ID disabled \n");
    }

  NBIO_TRACEPOINT (SIL_TRACE_INFO, "Exit\n");
  return SilPass;
}
