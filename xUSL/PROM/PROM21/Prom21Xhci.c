/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file Prom21Xhci.c
 *  @brief Promontory21 XHCI related functions
 */

#include <SilCommon.h>
#include <CommonLib/Mmio.h>
#include <FCH/Common/FchCommon.h>
#include <PROM/Common/PromAccess.h>
#include <PROM/Common/PromInit.h>
#include <PROM/PromClass-api.h>
#include "Prom21.h"
#include "Prom21Init.h"

void
Prom21UsbSetting (
  PROM21_DATA_BLK *PromDataBlk,
  uint32_t        XhciMmio,
  uint8_t         BootMode
  )
{
  uint8_t  i;
  uint8_t  Data8;
  uint32_t UsbCmd;
  uint32_t UsbOpBase;
  uint32_t Counter;

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "XhciMmio 0x%08X\n",  XhciMmio);

  UsbOpBase = XhciMmio + 0x20;

  Counter = 0;
  do {
    UsbCmd = xUSLMemRead32((void *)(size_t)(UsbOpBase));
    PROM_TRACEPOINT(SIL_TRACE_INFO, "-->  Usb Status = 0x%x\n", UsbCmd);
    if (UsbCmd & BIT_8(1)) {
      SilFchStall(2 * 1000);
    }
  } while ((UsbCmd & BIT_8(1)) && (Counter < 100));

  if (PromDataBlk->PT21HW_LPM != 0xf) {
    if (PromDataBlk->PT21HW_LPM == 1) {
      for (i = 0; i < 3; i++) {
        Data8 = Prom21XhciReadByte (XhciMmio, hw_lpm_en[i]);
        Prom21XhciWriteByte (XhciMmio, hw_lpm_en[i], (Data8 | 0x1E));
      }
    } else {
      for (i = 0; i < 3; i++) {
        Data8 = Prom21XhciReadByte (XhciMmio, hw_lpm_en[i]);
        Prom21XhciWriteByte (XhciMmio, hw_lpm_en[i], (Data8 & (~0x1E)));
      }
    }
  }

  if (PromDataBlk->PT21XHC_PME != 0xf) {
    if (PromDataBlk->PT21XHC_PME == 1) {
      Data8 = Prom21XhciReadByte (XhciMmio, 0x18515);
      Prom21XhciWriteByte (XhciMmio, 0x18515, (Data8 & ~BIT_8(7)));
    } else {
      Data8 = Prom21XhciReadByte (XhciMmio, 0x18515);
      Prom21XhciWriteByte (XhciMmio, 0x18515, (Data8 | BIT_8(7)));
    }
  }

  if (PromDataBlk->PT21DbC != 0xf) {
    if (PromDataBlk->PT21DbC != 0) {
      Prom21XhciWriteByte (XhciMmio, 0x18A61, 8);
    } else {
      Prom21XhciWriteByte (XhciMmio, 0x18A61, 16);
    }
  }

  PROM_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}
