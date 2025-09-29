/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchCio.c
 * @brief FCH USB4 controller functions
 *
 */

#include <SilCommon.h>
#include <FCH/Common/Fch.h>
#include "FchUsbCmn2Rev.h"
#include "FchXhci.h"
#include "FchCio.h"

/**
 * FchCioTunnelDisable
 *
 * @brief Disable protocol tunneling on USB4 routers
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param DieBusNum             Bus Number on Current Die.
 * @param Usb4Router            USB4 router number.
 * @param RouterSelect          Number passed to the Host/RT select bit.
 * @param FchUsbData            Fch Usb configuration structure pointer.
 *
 */
void
FchCioTunnelDisable (
  SIL_CONTEXT      *SilContext,
  uint32_t         DieBusNum,
  uint32_t         Usb4Router,
  uint32_t         RouterSelect,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  uint32_t RegValue;
  FCH_USB4_HOST      *Usb4Host;
  FCH_USB_XFER_TABLE *FchUsbXfer;

  Usb4Host = &(FchUsbData->Usb4Host[Usb4Router]);

  if (SilGetCommon2RevXferTable(SilContext, SilId_FchUsb, (void **)(&FchUsbXfer)) != SilPass) {
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "FCH USB C2R transfer table not found!\n");
    return;
  }

  RegValue = 0;

  if (Usb4Host->InitEnable && Usb4Host->HostEnable) {
    if (Usb4Host->Usb3TunnelingDisable & BIT_32(0)) {
      RegValue |= BIT_32(0);
    }
    if (Usb4Host->PcieTunnelingDisable) {
      RegValue |= BIT_32(8);
      FchUsbXfer->FchCioPcieDisable(Usb4Router, FchUsbData);
    }
    if (Usb4Host->DPTunnelingDisable) {
      RegValue |= BIT_32(16);
    }

    FchXhciSmuUsbConfigUpdate(SilContext,
      DieBusNum,
      FCHUSBxE7B0C,
      0x00010101,
      RegValue,
      GET_USB_OP_GROUP(RouterSelect, FchUsbConfigRegGroup6)
      );

    FCH_TRACEPOINT(SIL_TRACE_INFO,
      "Set tunnel disable to 0x%x for USB4 Router %x, RouterSelect = %x\n",
      RegValue,
      Usb4Router,
      RouterSelect
      );
  }
}

/**
 * FchCioAdapterHide
 *
 * @brief Hide protocol adapters on USB4 routers
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param DieBusNum             Bus Number on Current Die.
 * @param Usb4Router            USB4 router number.
 * @param RouterSelect          Number passed to the Host/RT select bit.
 * @param FchUsbData            Fch Usb configuration structure pointer.
 *
 */
void
FchCioAdapterHide (
  SIL_CONTEXT      *SilContext,
  uint32_t         DieBusNum,
  uint32_t         Usb4Router,
  uint32_t         RouterSelect,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  uint32_t RegValue;
  FCH_USB4_HOST      *Usb4Host;
  FCH_USB_XFER_TABLE *FchUsbXfer;

  Usb4Host = &(FchUsbData->Usb4Host[Usb4Router]);

  if (SilGetCommon2RevXferTable(SilContext, SilId_FchUsb, (void **)(&FchUsbXfer)) != SilPass) {
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "FCH USB C2R transfer table not found!\n");
    return;
  }

  RegValue = 0;

  if (Usb4Host->InitEnable && Usb4Host->HostEnable) {
    if (Usb4Host->Usb3AdpHidden & BIT_32(0)) {
      RegValue |= BIT_32(0);
    }
    if (Usb4Host->PcieAdpHidden & BIT_32(0)) {
      RegValue |= BIT_32(8);
      FchUsbXfer->FchCioPcieDisable(Usb4Router, FchUsbData);
    }
    RegValue |= (Usb4Host->DPAdpHidden & (BIT_8(0) + BIT_8(1))) << 16;

    FchXhciSmuUsbConfigUpdate(SilContext,
      DieBusNum,
      FCHUSBxE7B08,
      0x00030101,
      RegValue,
      GET_USB_OP_GROUP(RouterSelect, FchUsbConfigRegGroup6)
      );

    FCH_TRACEPOINT(SIL_TRACE_INFO,
      "Set adapter hide to 0x%x for USB4 Router %x, RouterSelect = %x\n",
      RegValue,
      Usb4Router,
      RouterSelect
      );
  }
}

/**
 * FchCioTbt3Disable
 *
 * @brief Disable USB4 Thunderbolt 3 compatibility
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param DieBusNum             Bus Number on Current Die.
 * @param Usb4Router            USB4 router number.
 * @param RouterSelect          Number passed to the Host/RT select bit.
 * @param FchUsbData            Fch Usb configuration structure pointer.
 *
 */
void
FchCioTbt3Disable (
  SIL_CONTEXT      *SilContext,
  uint32_t         DieBusNum,
  uint32_t         Usb4Router,
  uint32_t         RouterSelect,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  FCH_USB4_HOST *Usb4Host;

  Usb4Host = &(FchUsbData->Usb4Host[Usb4Router]);

  if (Usb4Host->InitEnable && Usb4Host->HostEnable && Usb4Host->Tbt3NotSupport) {
    FchXhciSmuUsbConfigUpdate(SilContext,
      DieBusNum,
      FCHUSBxE7B04,
      0x00000001,
      0x00000001,
      GET_USB_OP_GROUP(RouterSelect, FchUsbConfigRegGroup6)
      );
    FCH_TRACEPOINT(SIL_TRACE_INFO,
      "Disabled TBT3 for USB4 Router %x, RouterSelect = %x\n",
      Usb4Router,
      RouterSelect
      );
  }
}

/**
 * FchCioTxFFEConfigure
 *
 * @brief Configure TxFFE
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param DieBusNum             Bus Number on Current Die.
 * @param Usb4Router            USB4 router number.
 * @param RouterSelect          Number passed to the Host/RT select bit.
 * @param FchUsbData            Fch Usb configuration structure pointer.
 *
 */
void
FchCioTxFFEConfigure (
  SIL_CONTEXT      *SilContext,
  uint32_t         DieBusNum,
  uint32_t         Usb4Router,
  uint32_t         RouterSelect,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  FCH_USB4_HOST *Usb4Host;
  uint32_t DW0;
  uint32_t DW1;

  DW0 = 0;
  DW1 = 0;

  Usb4Host = &(FchUsbData->Usb4Host[Usb4Router]);

  if (Usb4Host->TxFFEMode == 1) {
    // FW mode without preset value override
    DW1 = 0x01;
  } else if (Usb4Host->TxFFEMode == 2) {
    DW1 = 0x2;
    DW0 = Usb4Host->TxFFEPreSetValue;
  }

  FchXhciSmuUsbConfigUpdate(SilContext,
    DieBusNum,
    DW0,
    DW1,
    0x00000000,
    0xD0000000 | (RouterSelect << 20)
    );

  FCH_TRACEPOINT(SIL_TRACE_INFO,
    "USB4 Router %x, RouterSelect = %x\n",
    Usb4Router,
    RouterSelect
    );
}

/**
 * FchCioGen3Support
 *
 * @brief Configure USB4 Gen 3 support
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param DieBusNum             Bus Number on Current Die.
 * @param Usb4Router            USB4 router number.
 * @param RouterSelect          Number passed to the Host/RT select bit.
 * @param FchUsbData            Fch Usb configuration structure pointer.
 *
 */
void
FchCioGen3Support (
  SIL_CONTEXT      *SilContext,
  uint32_t         DieBusNum,
  uint32_t         Usb4Router,
  uint32_t         RouterSelect,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  FCH_USB4_HOST *Usb4Host;

  Usb4Host = &(FchUsbData->Usb4Host[Usb4Router]);

  FchXhciSmuUsbConfigUpdate(SilContext,
    DieBusNum,
    FCHUSBxD8264,
    BIT_32(13),
    (Usb4Host->Gen3Support & BIT_8(0)) << 13,
      GET_USB_OP_GROUP(RouterSelect, FchUsbConfigRegGroup3)
    );

  FchXhciSmuUsbConfigUpdate(SilContext,
    DieBusNum,
    FCHUSBxD8270,
    BIT_32(18),
    (Usb4Host->Gen3Support & BIT_8(0)) << 18,
      GET_USB_OP_GROUP(RouterSelect, FchUsbConfigRegGroup3)
    );

  FCH_TRACEPOINT(SIL_TRACE_INFO,
    "Gen3Support = 0x%x, USB4 Router %x, RouterSelect = %x\n",
    Usb4Host->Gen3Support,
    Usb4Router,
    RouterSelect
    );
}

/**
 * FchCioPhyLoad
 *
 * @brief USB4 PHY loading
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param Usb4Router            USB4 router number.
 * @param PhySelect             Number passed to the Host/RT select bit.
 * @param FchUsbData            Fch Usb configuration structure pointer.
 *
 */
void
FchCioPhyLoad (
  SIL_CONTEXT      *SilContext,
  uint32_t         Usb4Router,
  uint32_t         PhySelect,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  FCH_USB4_HOST *Usb4Host;

  Usb4Host = &(FchUsbData->Usb4Host[Usb4Router]);

  if (Usb4Host->InitEnable) {
    FchXhciSmuUsbConfigUpdate(SilContext,
      FchUsbData->DieBusNum,
      0x00000000,
      0x00000000,
      0x00000000,
      0xC0000003 | (PhySelect << 20)
      );

    FCH_TRACEPOINT(SIL_TRACE_INFO,
      "PHY loading enabled for USB4 Router %x, PhySelect = %x\n",
      Usb4Router,
      PhySelect
      );
  }
}

/**
 * FchCioClxEnable
 *
 * @brief USB4 Clx enable
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param Usb4Router            USB4 router number.
 * @param RouterSelect          Number passed to the Host/RT select bit.
 * @param FchUsbData            Fch Usb configuration structure pointer.
 *
 */
void
FchCioClxEnable (
  SIL_CONTEXT      *SilContext,
  uint32_t         Usb4Router,
  uint32_t         RouterSelect,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  FCH_USB4_HOST *Usb4Host;

  Usb4Host = &(FchUsbData->Usb4Host[Usb4Router]);

  if (Usb4Host->InitEnable && Usb4Host->ClxEnable) {
    FchXhciSmuUsbConfigUpdate(SilContext,
      FchUsbData->DieBusNum,
      0x00000000,
      0x00000000,
      0x00000000,
      0xF1000001 | (RouterSelect << 20)
      );
  }
}

/**
 * FchCioP4PgEnable
 *
 * @brief USB4 P4.PG enable
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param Usb4Router            USB4 router number.
 * @param RouterSelect          Number passed to the Host/RT select bit.
 * @param FchUsbData            Fch Usb configuration structure pointer.
 *
 */
void
FchCioP4PgEnable (
  SIL_CONTEXT      *SilContext,
  uint32_t         Usb4Router,
  uint32_t         RouterSelect,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  FCH_USB4_HOST *Usb4Host;

  Usb4Host = &(FchUsbData->Usb4Host[Usb4Router]);

  if (Usb4Host->InitEnable && Usb4Host->P4PgEnable) {
    FchXhciSmuUsbConfigUpdate(SilContext,
      FchUsbData->DieBusNum,
      0x00000000,
      0x00000000,
      0x00000000,
      0xF3000001 | (RouterSelect << 20)
      );
  }
}

/**
 * FchCioLinkSpeed
 *
 * @brief Enable USB4 Gen3 link speed
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param Usb4Router            USB4 router number.
 * @param RouterSelect          Number passed to the Host/RT select bit.
 * @param FchUsbData            Fch Usb configuration structure pointer.
 *
 */
void
FchCioLinkSpeed (
  SIL_CONTEXT      *SilContext,
  uint32_t         Usb4Router,
  uint32_t         RouterSelect,
  FCHUSB_INPUT_BLK *FchUsbData
  )
{
  FCH_USB4_HOST *Usb4Host;

  Usb4Host = &(FchUsbData->Usb4Host[Usb4Router]);

  if (Usb4Host->InitEnable && Usb4Host->LinkSpeed == 0x1) {
    // Supports Gen2 and Gen3 speeds
    FchXhciSmuUsbConfigUpdate(SilContext,
      FchUsbData->DieBusNum,
      0x00000000,
      0x00000000,
      0x00000000,
      0xF4000001 | (RouterSelect << 20)
      );
  }
}

/**
 * FchCioDebugSCDisable
 *
 * @brief Disable USB4 debug streaming control
 *
 * @param SilContext            A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param RouterSelect          Number passed to the Host/RT select bit.
 * @param Disable               Whether to disable Debug SC or enable it
 *
 */
void
FchCioDebugSCDisable (
  SIL_CONTEXT      *SilContext,
  uint32_t         DieBusNum,
  uint32_t         RouterSelect,
  bool             Disable
  )
{
  if (Disable) {
    FchXhciSmuUsbConfigUpdate(SilContext,
      DieBusNum,
      0x00000000,
      0x00000000,
      0x00000000,
      0xF5000001 | (RouterSelect << 20)
      );
    FCH_TRACEPOINT(SIL_TRACE_INFO, "Debug SC disabled.\n");
  } else {
    FchXhciSmuUsbConfigUpdate(SilContext,
      DieBusNum,
      0x00000000,
      0x00000000,
      0x00000000,
      0xF5000000 | (RouterSelect << 20)
      );
    FCH_TRACEPOINT(SIL_TRACE_INFO, "Debug SC enabled.\n");
  }
}
