/**
 * @file  NbioCommon.c
 * @brief OpenSIL NBIO Common APIs
 *
 */
/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <xSIM.h>
#include <DF/DfIp2Ip.h>
#include "Nbio.h"
#include <RcMgr/RcMgrIp2Ip.h>
#include "NbioCommon.h"
#include <CommonLib/SmnAccess.h>
#include <string.h>

/**
 * NonPciBarInit
 *
 * @brief  This function assigns MMIO for a non-pci device.
 *
 * @param  GnbHandle       Silicon handle to assign
 * @param  MmioBarLow      Address of low byte
 * @param  MmioBarHigh     Address of high byte
 * @param  MemorySize      Size of the allocated bar required
 * @param  Enable          Set enable bit in BAR
 * @param  LockSettings    If the allocated memory range should be locked or not
 * @param  Above4G         If MMIO above the 4G boundary should be allocated
 */
void
NonPciBarInit (
         GNB_HANDLE   *GnbHandle,
         uint32_t     MmioBarLow,
         uint32_t     MmioBarHigh,
         uint32_t     MemorySize,
         bool         Enable,
         bool         LockSettings,
         bool         Above4G
  )
{
  SIL_STATUS             Status;
  FABRIC_TARGET          MmioTarget;
  FABRIC_MMIO_ATTRIBUTE  Attributes;
  uint64_t               MmioBase, Length;
  uint32_t               BarLow, BarHigh;
  RCMGR_IP2IP_API        *RcMgrIp2Ip;

  BarLow = 0;
  BarHigh = 0;
  NBIO_TRACEPOINT (SIL_TRACE_INFO,
                   "Begin to allocate bars for SMN low %x high %x, size %x\n",
                   MmioBarLow,
                   MmioBarHigh,
                   MemorySize
                   );

  ///
  /// See if the given BAR have already been assigned
  ///
  BarLow = xUSLSmnRead (0, GnbHandle->Address.Address.Bus, MmioBarLow);
  BarHigh = xUSLSmnRead (0, GnbHandle->Address.Address.Bus, MmioBarHigh);

  if (BarLow == 0 && BarHigh == 0) {
    NBIO_TRACEPOINT (SIL_TRACE_INFO, "Bars have not been assigned, attempting to allocate MMIO \n");
    ///
    /// Assign bars:
    /// Allocate a chunk of MMIO first
    ///
    memset (&Attributes, 0, sizeof(Attributes));
    memset (&MmioTarget, 0, sizeof(MmioTarget));
    Length = MemorySize;
    MmioTarget.TgtType = TARGET_PCI_BUS;
    MmioTarget.SocketNum = GnbHandle->SocketId;
    MmioTarget.PciBusNum = (uint16_t) GnbHandle->Address.Address.Bus;
    Attributes.ReadEnable = 1;
    Attributes.WriteEnable = 1;
    Attributes.NonPosted = 0;
    MmioBase = 0;
    if (Above4G) {
      Attributes.MmioType = NON_PCI_DEVICE_ABOVE_4G;
    }
    else {
      Attributes.MmioType = NON_PCI_DEVICE_BELOW_4G;
    }

    NBIO_TRACEPOINT (SIL_TRACE_INFO,
                     "FabricAllocateMmio : Socket %d , RB # %d\n",
                     MmioTarget.SocketNum,
                     MmioTarget.RbNum
                     );

    if (SilGetIp2IpApi (SilId_RcManager, (void **)(&RcMgrIp2Ip)) != SilPass) {
      NBIO_TRACEPOINT (SIL_TRACE_ERROR, " MMIO allocator API is not found.\n");
      return;
    }

    Status = RcMgrIp2Ip->FabricReserveMmio (&MmioBase, &Length, ALIGN_1M, MmioTarget, &Attributes);

    if (Status != SilPass) {
      NBIO_TRACEPOINT (SIL_TRACE_INFO, "Allocate MMIO Fail\n");
      return;
    }
    NBIO_TRACEPOINT (SIL_TRACE_INFO, "Allocate MMIO @0x%llx\n", MmioBase);
  } else {
    NBIO_TRACEPOINT (SIL_TRACE_INFO, "Bars have already been assigned!\n");
    NBIO_TRACEPOINT (SIL_TRACE_INFO, "End\n");
    return;
  }

  ///
  /// Write the assigned memory address registers to SMN
  ///
  BarLow  = (uint32_t) MmioBase;
  BarHigh = (uint32_t) (MmioBase >> 32);

  xUSLSmnWrite (0, GnbHandle->Address.Address.Bus, MmioBarLow,  BarLow);
  xUSLSmnWrite (0, GnbHandle->Address.Address.Bus, MmioBarHigh, BarHigh);

  NBIO_TRACEPOINT (SIL_TRACE_INFO, "BarLow = %x , BarHigh %x\n", BarLow, BarHigh);

  // Set enable bit separate from other bits
  if (Enable) {
    BarLow = BarLow | BIT_32(0); /// Set enable bit
    xUSLSmnWrite (0, GnbHandle->Address.Address.Bus, MmioBarLow, BarLow);
    xUSLSmnWrite (0, GnbHandle->Address.Address.Bus, MmioBarHigh, BarHigh);
  }

  // Set lock bit separate from other bits
  if (LockSettings) {
    BarLow = BarLow | BIT_32(1); /// Set lock bit
    xUSLSmnWrite (0, GnbHandle->Address.Address.Bus, MmioBarLow, BarLow);
    xUSLSmnWrite (0, GnbHandle->Address.Address.Bus, MmioBarHigh, BarHigh);
  }
  NBIO_TRACEPOINT (SIL_TRACE_INFO, "End\n");
}
