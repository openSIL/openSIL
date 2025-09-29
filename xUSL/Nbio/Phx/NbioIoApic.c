/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  NbioIoApic.c
 * @brief OpenSIL NBIO IoApic initialization
 */

#include <Nbio/NbioIp2Ip.h>
#include <includePHX/PHX_IOHC.h>
#include <includePHX/PHX_IOAPIC.h>
#include <includePHX/PHX_GnbRegistersPhx.h>
#include "NbioCmn2RevPhx.h"
/**
 * NbioIoApicMmioAddressPhx
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
NbioIoApicMmioAddressPhx (
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
  xUSLSmnWrite(0,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, IOHCx13b102f4),
    BarHigh
    );
  xUSLSmnWrite(0,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, IOHCx13b102f0),
    BarLow
    );
  return;
}

/**
 * NbioIoApicPreDefIdPhx
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
NbioIoApicPreDefIdPhx (
  GNB_HANDLE     *GnbHandle,
  uint8_t        IoApicIdBase
  )
{
  uint8_t                 ApicIdBase;
  uint32_t                Value32;


  ApicIdBase = IoApicIdBase;
  ApicIdBase += (GnbHandle->SocketId * 4) + GnbHandle->RBIndex;
  Value32 = ((uint32_t) ApicIdBase) << 24;
  xUSLSmnWrite(0,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, IOAPICx2801000),
    Value32
    );
  NBIO_TRACEPOINT(SIL_TRACE_INFO,
    "Pre-Define IOAPIC ID Addr=0x%x, Value=0x%x\n",
    NBIO_SPACE(GnbHandle, IOAPICx2801000),
    Value32
    );
  return;
}
