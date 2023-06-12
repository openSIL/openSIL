/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  NbioIoApic.c
 * @brief OpenSIL NBIO IoApic initialization
 */

#include <Nbio/NbioIp2Ip.h>
#include "IohcReg.h"
#include "IoapicReg.h"
#include "GnbRegistersBrh.h"
#include "NbioCmn2RevBrh.h"
/**
 * NbioIoApicMmioAddressBrh
 *
 * @brief  This function is responsible for Writing the assigned
 *         memory address registers to SMN
 *
 * @param[in]     GnbHandle         GnbHandle Pointer
 * @param[in]     IoapicMmioBase    Ioapic MMIO base address
 *
 * @return        void
 */
void
NbioIoApicMmioAddressBrh (
  GNB_HANDLE    *GnbHandle,
  uint64_t      IoapicMmioBase
  )
{
  uint32_t          BarLow;
  uint32_t          BarHigh;

  ///
  /// Write the assigned memory address registers to SMN
  ///
  BarLow = (uint32_t) IoapicMmioBase;
  BarHigh = (uint32_t) (IoapicMmioBase >> 32);
  if (GnbHandle->RBIndex < 4) {
    xUSLSmnWrite(0,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, SMN_IOHUB0NBIO0_IOAPIC_BASE_ADDR_HI_ADDRESS),
      BarHigh
      );
    xUSLSmnWrite(0,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, SMN_IOHUB0NBIO0_IOAPIC_BASE_ADDR_LO_ADDRESS),
      BarLow
      );

    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, SMN_IOHUB0NBIO0_IOAPIC_BASE_ADDR_LO_ADDRESS),
      (uint32_t) ~(SIL_RESERVED_0604 & SIL_RESERVED_0603),
      0x1
      );
  } else {
    xUSLSmnWrite(0,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_1D4102F4),
      BarHigh
      );
    xUSLSmnWrite(0,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_1D4102F0),
      BarLow
      );
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_1D4102F0),
      (uint32_t) ~(SIL_RESERVED_0604 & SIL_RESERVED_0603),
      0x1
      );
  }
  return;
}

/**
 * NbioIoApicPreDefIdBrh
 *
 * @brief  This function is responsible for assigning the IoApic
 *         base ID for each IOHC instance.
 *
 * @param[in]     GnbHandle         GnbHandle Pointer
 * @param[in]     IoapicMmioBase    Ioapic MMIO base address
 *
 * @return void
 */
void
NbioIoApicPreDefIdBrh (
  GNB_HANDLE     *GnbHandle,
  uint8_t        IoApicIdBase
  )
{
  uint8_t                 ApicIdBase;
  uint32_t                Value32;


  ApicIdBase = IoApicIdBase;
  ApicIdBase += (GnbHandle->SocketId * 8) + GnbHandle->RBIndex;
  Value32 = ((uint32_t) ApicIdBase) << 24;
  if (GnbHandle->RBIndex < 4) {
    xUSLSmnWrite(0,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, SMN_IOHUB0NBIO0_IOAPIC_ID_REGISTER_ADDRESS),
      Value32
      );
    NBIO_TRACEPOINT(SIL_TRACE_INFO,
      "Pre-Define IOAPIC ID Addr=0x%x, Value=0x%x\n",
      NBIO_SPACE(GnbHandle, SMN_IOHUB0NBIO0_IOAPIC_ID_REGISTER_ADDRESS),
      Value32
      );
  } else {
    xUSLSmnWrite(0,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, SIL_RESERVED_0599),
      Value32
      );
    NBIO_TRACEPOINT(SIL_TRACE_INFO,
      "Pre-Define IOAPIC ID Addr=0x%x, Value=0x%x\n",
      NBIO_SPACE(GnbHandle, SIL_RESERVED_0599),
      Value32
      );
  }
  return;
}
