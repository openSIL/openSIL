/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file PromAccess.c
 *  @brief Promontory21 helper functions
 */

#include <SilCommon.h>
#include <CommonLib/Mmio.h>
#include <PciLib.h>
#include <FCH/Common/FchCommon.h>
#include <PROM/PROM21/Prom21.h>
#include <PROM/PROM21/Prom21Init.h>
#include "PromAccess.h"
#include "PromInit.h"

static void CheckPcieWriteCycleV2 (
  PROMCLASS_DATA_BLK *PromDataBlk,
  uint32_t           XhciMmio,
  uint8_t            Address,
  uint8_t            Offset
  )
{
  uint8_t                           Data;
  uint8_t                           Counter;
  PROM_PCI_SAVE_RESTORE_TABLE       PtPciTable[2];

  Counter = 0;
  do {
    Data = xUSLMemRead8((void *)(size_t)(XhciMmio + 0x3009));
    if (Data == 0xFF) {
      PROM_TRACEPOINT(SIL_TRACE_INFO, "  PT reset itself --%u-- !!!\n", Offset);
      if (PromDataBlk->PromInputBlk.PT21Revision < 2) {
        if (XhciMmio == PromDataBlk->PromOutputBlk.PT21XhciMmio[0]) {
          SaveInitPromBus(PromDataBlk, PtPciTable, 0x00BB);
        } else {
          SaveInitPromBus(PromDataBlk, PtPciTable, 0xBBBB);
        }
      }
      xUSLMemWrite8((void *)(size_t)(XhciMmio + 0x3002 - Offset), Address);
    } else if (Data & BIT_8(7)) {
      Counter++;
      if (Counter == 100) {
        SilFchStall(200 * 1000);
        Counter = 0;
      }
    }
  } while (Data & BIT_8(7));
}

static void
Prom21XhciSetAddressV2 (
  PROMCLASS_DATA_BLK *PromDataBlk,
  uint32_t           XhciMmio,
  uint32_t           Address
  )
{
  xUSLMemWrite8((void *)(size_t)(XhciMmio + 0x3002), (Address >> 16) & 0xFF);
  CheckPcieWriteCycleV2(PromDataBlk, XhciMmio, (Address >> 16) & 0xFF, 0);

  xUSLMemWrite8((void *)(size_t)(XhciMmio + 0x3001), (Address >> 8) & 0xFF);
  CheckPcieWriteCycleV2(PromDataBlk, XhciMmio, (Address >> 8) & 0xFF, 1);

  xUSLMemWrite8((void *)(size_t)(XhciMmio + 0x3000), Address & 0xFF);
  CheckPcieWriteCycleV2(PromDataBlk, XhciMmio, Address & 0xFF, 2);
}

uint8_t
Prom21XhciReadByteV2 (
  PROMCLASS_DATA_BLK *PromDataBlk,
  uint32_t           XhciMmio,
  uint32_t           Address
  )
{
  Prom21XhciSetAddressV2(PromDataBlk, XhciMmio, Address);
  return xUSLMemRead8((void *)(size_t)(XhciMmio + 0x3008));
}

static void CheckPcieWriteCycle (
  uint32_t XhciMmio
  )
{
  while (xUSLMemRead8((void *)(size_t)(XhciMmio + 0x3009)) & BIT_8(7));
}

static void
Prom21XhciSetAddress (
  uint32_t    XhciMmio,
  uint32_t    Address
  )
{
  xUSLMemWrite8((void *)(size_t)(XhciMmio + 0x3002), ((Address >> 16) & 0xFF));
  CheckPcieWriteCycle(XhciMmio);

  xUSLMemWrite8((void *)(size_t)(XhciMmio + 0x3001), ((Address >> 8) & 0xFF));
  CheckPcieWriteCycle(XhciMmio);

  xUSLMemWrite8((void *)(size_t)(XhciMmio + 0x3000), (Address & 0xFF));
  CheckPcieWriteCycle(XhciMmio);
}

uint8_t
Prom21XhciReadByte (
  uint32_t    XhciMmio,
  uint32_t    Address
  )
{
  xUSLMemRead8((void *)(size_t)(XhciMmio + 0x3009)); // check
  Prom21XhciSetAddress(XhciMmio, Address);
  return xUSLMemRead8((void *)(size_t)(XhciMmio + 0x3008));
}

uint16_t
Prom21XhciReadWord (
  uint32_t    XhciMmio,
  uint32_t    Address
  )
{
  uint8_t   DataByte;
  uint16_t  DataWord = 0;
 
  xUSLMemRead8((void *)(size_t)(XhciMmio + 0x3009)); // check

  for (uint32_t i = 0; i < sizeof(DataWord); i++) {
    Prom21XhciSetAddress(XhciMmio, Address + i);
    DataByte = xUSLMemRead8((void *)(size_t)(XhciMmio + 0x3008));
    DataWord |= ((uint16_t)DataByte) << (8 * i);
  }

  return  DataWord;
}

uint32_t
Prom21XhciReadDWord (
  uint32_t    XhciMmio,
  uint32_t    Address
  )
{
  uint8_t   DataByte;
  uint32_t  DataDWord = 0;
 
  xUSLMemRead8((void *)(size_t)(XhciMmio + 0x3009)); // check

  for (uint32_t i = 0; i < sizeof(DataDWord); i++) {
    Prom21XhciSetAddress(XhciMmio, Address + i);
    DataByte = xUSLMemRead8((void *)(size_t)(XhciMmio + 0x3008));
    DataDWord |= ((uint32_t)DataByte) << (8 * i);
  }

  return DataDWord;
}

void
Prom21XhciWriteByte (
  uint32_t    XhciMmio,
  uint32_t    Address,
  uint8_t     Data
  )
{ 
  xUSLMemRead8((void *)(size_t)(XhciMmio + 0x3009)); // check
  Prom21XhciSetAddress(XhciMmio, Address);
  xUSLMemWrite8((void *)(size_t)(XhciMmio + 0x3004), Data);
  CheckPcieWriteCycle(XhciMmio);
}

void
Prom21XhciWriteWord (
  uint32_t    XhciMmio,
  uint32_t    Address,
  uint16_t    Data
  )
{ 
  xUSLMemRead8((void *)(size_t)(XhciMmio + 0x3009)); // check

  for (uint32_t i = 0; i < sizeof(Data); i++) {
    Prom21XhciSetAddress(XhciMmio, Address + i);
    xUSLMemWrite8((void *)(size_t)(XhciMmio + 0x3004), ((Data >> (8 * i)) & 0xFF));
    CheckPcieWriteCycle(XhciMmio);
  }
}

void
Prom21XhciWriteDWord (
  uint32_t    XhciMmio,
  uint32_t    Address,
  uint32_t    Data
  )
{ 
  xUSLMemRead8((void *)(size_t)(XhciMmio + 0x3009)); // check
  for (uint32_t i = 0; i < sizeof(Data); i++) {
    Prom21XhciSetAddress(XhciMmio, Address + i);
    xUSLMemWrite8((void *)(size_t)(XhciMmio + 0x3004), ((Data >> (8 * i)) & 0xFF));
    CheckPcieWriteCycle(XhciMmio);
  }
}

static void
SaveInitSingleProm (
  PROMCLASS_DATA_BLK            *PromDataBlk,
  PROM_PCI_SAVE_RESTORE_TABLE   *PtPciTable,
  uint8_t                       CheckBit,
  bool                          IsPrimary
  )
{
  uint16_t  GppBus;
  uint8_t   GppDev;
  uint8_t   GppFun;
  uint32_t  GppPcieAddress;
  uint32_t  XhciMmio;
  uint32_t  SecXhciMmio;
  uint32_t  PciAddr;
  uint32_t  PromAddr;
  uint32_t  XhciDspAddr;
  uint32_t  SataDspAddr;
  uint32_t  XhciAddr;
  uint32_t  SataAddr;

  if (IsPrimary) {
    GppPcieAddress = PromDataBlk->PromOutputBlk.PT21GppPcieAddress[0];
  } else {
    GppPcieAddress = PromDataBlk->PromOutputBlk.PT21GppPcieAddress[1];
  }

  XhciMmio = PromDataBlk->PromOutputBlk.PT21XhciMmio[0];
  SecXhciMmio = PromDataBlk->PromOutputBlk.PT21XhciMmio[1];

  GppBus = (uint16_t) ((GppPcieAddress >> 20) & 0xFFF);
  GppDev = (uint8_t) ((GppPcieAddress >> 15) & 0x1F);
  GppFun = (uint8_t) ((GppPcieAddress >> 12) & 0x07);

  if (GppPcieAddress == 0) {
    return;
  }

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "  Save Init %s PROM21"
                                  "-%s"
                                  "-%s"
                                  "-%s"
                                  "-%s"
                                  "-%s"
                                  "-%s"
                                  "-%s"
                                  "-%s\n",
                                  IsPrimary ? "Primary" : "Secondary",
                                  (CheckBit & BIT_8(0)) ? "UspBus" : " ",
                                  (CheckBit & BIT_8(1)) ? "XhciBus" : " ",
                                  (CheckBit & BIT_8(2)) ? "SataBus" : " ",
                                  (CheckBit & BIT_8(3)) ? "XhciMmio" : " ",
                                  (CheckBit & BIT_8(4)) ? "UspCmd" : " ",
                                  (CheckBit & BIT_8(5)) ? "XhciCmd" : " ",
                                  (CheckBit & BIT_8(6)) ? "SataCmd" : " ",
                                  (CheckBit & BIT_8(7)) ? "ASPM" : " "
                                  );

  PciAddr = PCI_LIB_ADDRESS(GppBus, GppDev, GppFun, 0);
  PromAddr = PCI_LIB_ADDRESS(PtPciTable->PromBus, 0, 0, 0);
  XhciDspAddr = PCI_LIB_ADDRESS(PtPciTable->PromBus + 1, PROM21_XHCI_DSP_DEV, 0, 0);
  SataDspAddr = PCI_LIB_ADDRESS(PtPciTable->PromBus + 1, PROM21_SATA_DSP_DEV, 0, 0);
  XhciAddr = PCI_LIB_ADDRESS(PtPciTable->PromBus + 2, 0, 0, 0);
  SataAddr = PCI_LIB_ADDRESS(PtPciTable->PromBus + 3, 0, 0, 0);


  if (CheckBit & (BIT_8(7) + BIT_8(2) + BIT_8(1) + BIT_8(0))) {
    PtPciTable->GppBus = xUSLPciRead32(PciAddr + 0x18);
    xUSLPciWrite8(PciAddr + 0x18, (uint8_t)GppBus);
    xUSLPciWrite8(PciAddr + 0x19, (uint8_t)PtPciTable->PromBus);
    xUSLPciWrite8(PciAddr + 0x1A, (uint8_t)(PtPciTable->PromBus + 7));

    PtPciTable->UspBus = xUSLPciRead32(PromAddr + 0x18);
    xUSLPciWrite8(PromAddr + 0x18, (uint8_t)PtPciTable->PromBus);
    xUSLPciWrite8(PromAddr + 0x19, (uint8_t)(PtPciTable->PromBus + 1));
    xUSLPciWrite8(PromAddr + 0x1A, (uint8_t)(PtPciTable->PromBus + 7));
  }

  if (CheckBit & (BIT_8(7) + BIT_8(1))) {
    PtPciTable->XhciDspBus = xUSLPciRead32(XhciDspAddr + 0x18);
    xUSLPciWrite8(XhciDspAddr + 0x18, (uint8_t)PtPciTable->PromBus + 1);
    xUSLPciWrite8(XhciDspAddr + 0x19, (uint8_t)PtPciTable->PromBus + 2);
    xUSLPciWrite8(XhciDspAddr + 0x1A, (uint8_t)PtPciTable->PromBus + 2);
  }

  if (CheckBit & (BIT_8(7) + BIT_8(2))) {
    PtPciTable->SataDspBus = xUSLPciRead32(SataDspAddr + 0x18);
    xUSLPciWrite8(SataDspAddr + 0x18, (uint8_t)(PtPciTable->PromBus + 1));
    xUSLPciWrite8(SataDspAddr + 0x19, (uint8_t)(PtPciTable->PromBus + 3));
    xUSLPciWrite8(SataDspAddr + 0x1A, (uint8_t)(PtPciTable->PromBus + 3));
  }

  if ((CheckBit & BIT_8(3))) {
    PtPciTable->GppMmio = xUSLPciRead32(PciAddr + 0x20);
    xUSLPciWrite16(PciAddr + 0x20, (uint16_t)(XhciMmio >> 16));
    xUSLPciWrite16(PciAddr + 0x22, (uint16_t)(SecXhciMmio >> 16));

    PtPciTable->UspMmio = xUSLPciRead32(PromAddr + 0x20);
    xUSLPciWrite16(PromAddr + 0x20, (uint16_t)(XhciMmio >> 16));
    xUSLPciWrite16(PromAddr + 0x22, (uint16_t)(SecXhciMmio >> 16));

    PtPciTable->XhciDspMmio = xUSLPciRead32(XhciDspAddr + 0x20);
    xUSLPciWrite16(XhciDspAddr + 0x20, (uint16_t)(XhciMmio >> 16));
    xUSLPciWrite16(XhciDspAddr + 0x22, (uint16_t)(XhciMmio >> 16));

    PtPciTable->XhciMmio = xUSLPciRead32(XhciAddr + 0x10) & 0xFFFFFF00;
    xUSLPciWrite32(XhciAddr + 0x10, XhciMmio);
  }

  if ((CheckBit & (BIT_8(6) + BIT_8(5) + BIT_8(4)))) {
    PtPciTable->GppCmd = xUSLPciRead8(PciAddr + 0x04);
    xUSLPciWrite8(PciAddr + 0x04, 6);

    PtPciTable->UspCmd = xUSLPciRead8(PromAddr + 0x04);
    xUSLPciWrite8(PromAddr + 0x04, 6);
  }

  if ((CheckBit & BIT_8(5))) {
    PtPciTable->XhciDspCmd = xUSLPciRead8(XhciDspAddr + 0x04);
    xUSLPciWrite8(XhciDspAddr + 0x04, 6);

    PtPciTable->XhciCmd = xUSLPciRead8(XhciAddr + 0x04);
    xUSLPciWrite8(XhciAddr + 0x04, 6);
  }

  if ((CheckBit & BIT_8(6))) {
    PtPciTable->SataDspCmd = xUSLPciRead8(SataDspAddr + 0x04);
    xUSLPciWrite8(SataDspAddr + 0x04, 6);

    PtPciTable->SataCmd = xUSLPciRead8(SataAddr + 0x04);
    xUSLPciWrite8(SataAddr + 0x04, 6);
  }

  if ((CheckBit & BIT_8(7)) == BIT_8(7)) {
    PtPciTable->GppAspm = xUSLPciRead8(PciAddr + 0x68);
    xUSLPciWrite8(PciAddr + 0x68, PtPciTable->GppAspm & 0xFC);

    PtPciTable->UspAspm = xUSLPciRead8(PromAddr + 0x90);
    xUSLPciWrite8(PromAddr + 0x90, PtPciTable->UspAspm & 0xFC);

    PtPciTable->XhciDspAspm = xUSLPciRead8(XhciDspAddr + 0x90);
    xUSLPciWrite8(XhciDspAddr + 0x90, PtPciTable->XhciDspAspm & 0xFC);

    PtPciTable->SataDspAspm = xUSLPciRead8(SataDspAddr + 0x90);
    xUSLPciWrite8(SataDspAddr + 0x90, PtPciTable->SataDspAspm & 0xFC);

    PtPciTable->XhciAspm = xUSLPciRead8(XhciAddr + 0x90);
    xUSLPciWrite8(XhciAddr + 0x90, PtPciTable->XhciAspm & 0xFC);

    PtPciTable->SataAspm = xUSLPciRead8(SataAddr + 0x90);
    xUSLPciWrite8(SataAddr + 0x90, PtPciTable->SataAspm & 0xFC);
  }

  PROM_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

static void
SaveInitPromBusNumbers (
  PROMCLASS_DATA_BLK            *PromDataBlk,
  PROM_PCI_SAVE_RESTORE_TABLE   *PtPciTable,
  uint16_t                       CheckBit
  )
{
  uint16_t GppBus = (uint16_t) ((PromDataBlk->PromOutputBlk.PT21GppPcieAddress[0] >> 20) & 0xFFF);

  PtPciTable[0].PromBus    = 0;
  PtPciTable[1].PromBus = 0;

  if (CheckBit & 0xff) {
    PtPciTable[0].PromBus = PromDataBlk->PromInputBlk.PT21TempBusNum + GppBus;
  }

  if (CheckBit & 0xff00) {
    PtPciTable[1].PromBus = PtPciTable->PromBus + 4;
  }
}

/**
 * SaveInitPromBus
 *
 * @brief Save and Init Prom Bus/Command/Mmio/Aspm
 *
 * @param  PtPciTable   Save Restore Table location
 * @param  CheckBit     Bit0  = 1 - Save and Init UspBus
 *                      Bit1  = 1 - Save and Init XhciBus
 *                      Bit2  = 1 - Save and Init SataBus
 *                      Bit3  = 1 - Save and Init XhciMmio
 *                      Bit4  = 1 - Save and Init UspCmd
 *                      Bit5  = 1 - Save and Init XhciCmd
 *                      Bit6  = 1 - Save and Init SataCmd
 *                      Bit7  = 1 - Save and Disable ASPM
 *                      Bit8  = 1 - Save and Init SecUspBus
 *                      Bit9  = 1 - Save and Init SecXhciBus
 *                      Bit10 = 1 - Save and Init SecSataBus
 *                      Bit11 = 1 - Save and Init SecXhciMmio
 *                      Bit12 = 1 - Save and Init SecUspCmd
 *                      Bit13 = 1 - Save and Init SecXhciCmd
 *                      Bit14 = 1 - Save and Init SecSataCmd
 *                      Bit15 = 1 - Save and Disable Secondary ASPM
 */
void
SaveInitPromBus (
  PROMCLASS_DATA_BLK            *PromDataBlk,
  PROM_PCI_SAVE_RESTORE_TABLE   *PtPciTable,
  uint16_t                      CheckBit
  )
{
  SaveInitPromBusNumbers(PromDataBlk, PtPciTable, CheckBit);
  SaveInitSingleProm(PromDataBlk, &PtPciTable[0], CheckBit & 0xff, true);
  SaveInitSingleProm(PromDataBlk, &PtPciTable[1], CheckBit >> 8, false);
}

static void
RestoreSingleProm (
  PROMCLASS_DATA_BLK          *PromDataBlk,
  PROM_PCI_SAVE_RESTORE_TABLE *PtPciTable,
  uint8_t                     CheckBit,
  bool                        IsPrimary
  )
{
  uint16_t  GppBus;
  uint8_t   GppDev;
  uint8_t   GppFun;
  uint32_t  GppPcieAddress;
  uint32_t  PciAddr;
  uint32_t  PromAddr;
  uint32_t  XhciDspAddr;
  uint32_t  SataDspAddr;
  uint32_t  XhciAddr;
  uint32_t  SataAddr;

  if (IsPrimary) {
    GppPcieAddress = PromDataBlk->PromOutputBlk.PT21GppPcieAddress[0];
  } else {
    GppPcieAddress = PromDataBlk->PromOutputBlk.PT21GppPcieAddress[1];
  }

  GppBus = (uint16_t) ((GppPcieAddress >> 20) & 0xFFF);
  GppDev = (uint8_t) ((GppPcieAddress >> 15) & 0x1F);
  GppFun = (uint8_t) ((GppPcieAddress >> 12) & 0x07);

  if (GppPcieAddress == 0) {
    return;
  }

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "  Restore %s PROM21"
                                  "-%s"
                                  "-%s"
                                  "-%s"
                                  "-%s"
                                  "-%s"
                                  "-%s"
                                  "-%s"
                                  "-%s\n",
                                  IsPrimary ? "Primary" : "Secondary",
                                  (CheckBit & BIT_8(0)) ? "UspBus" : " ",
                                  (CheckBit & BIT_8(1)) ? "XhciBus" : " ",
                                  (CheckBit & BIT_8(2)) ? "SataBus" : " ",
                                  (CheckBit & BIT_8(3)) ? "XhciMmio" : " ",
                                  (CheckBit & BIT_8(4)) ? "UspCmd" : " ",
                                  (CheckBit & BIT_8(5)) ? "XhciCmd" : " ",
                                  (CheckBit & BIT_8(6)) ? "SataCmd" : " ",
                                  (CheckBit & BIT_8(7)) ? "ASPM" : " "
                                  );

  PciAddr = PCI_LIB_ADDRESS(GppBus, GppDev, GppFun, 0);
  PromAddr = PCI_LIB_ADDRESS(PtPciTable->PromBus, 0, 0, 0);
  XhciDspAddr = PCI_LIB_ADDRESS(PtPciTable->PromBus + 1, PROM21_XHCI_DSP_DEV, 0, 0);
  SataDspAddr = PCI_LIB_ADDRESS(PtPciTable->PromBus + 1, PROM21_SATA_DSP_DEV, 0, 0);
  XhciAddr = PCI_LIB_ADDRESS(PtPciTable->PromBus + 2, 0, 0, 0);
  SataAddr = PCI_LIB_ADDRESS(PtPciTable->PromBus + 3, 0, 0, 0);

  if (CheckBit & BIT_8(7)) {
    xUSLPciWrite8(XhciAddr + 0x90, PtPciTable->XhciAspm);
    xUSLPciWrite8(SataAddr + 0x90, PtPciTable->SataAspm);
    xUSLPciWrite8(XhciDspAddr + 0x90, PtPciTable->XhciDspAspm);
    xUSLPciWrite8(SataDspAddr + 0x90, PtPciTable->SataDspAspm);
    xUSLPciWrite8(PromAddr + 0x90, PtPciTable->UspAspm);
    xUSLPciWrite8(PciAddr + 0x68, PtPciTable->GppAspm);
  }

  if (CheckBit & BIT_8(5)) {
    xUSLPciWrite8(XhciAddr + 0x04, PtPciTable->XhciCmd);
    xUSLPciWrite8(XhciDspAddr + 0x04, PtPciTable->XhciDspCmd);
  }

  if (CheckBit & BIT_8(6)) {
    xUSLPciWrite8(SataAddr + 0x04, PtPciTable->SataCmd);
    xUSLPciWrite8(SataDspAddr + 0x04, PtPciTable->SataDspCmd);
  }

  if (CheckBit & (BIT_8(6) + BIT_8(5) + BIT_8(4))) {
    xUSLPciWrite8(PromAddr + 0x04, PtPciTable->UspCmd);
    xUSLPciWrite8(PciAddr + 0x04, PtPciTable->GppCmd);
  }

  if (CheckBit & BIT_8(3)) {
    xUSLPciWrite32(XhciAddr + 0x10, PtPciTable->XhciMmio);
    xUSLPciWrite32(XhciDspAddr + 0x20, PtPciTable->XhciDspMmio);
    xUSLPciWrite32(PromAddr + 0x20, PtPciTable->UspMmio);
    xUSLPciWrite32(PciAddr + 0x20, PtPciTable->GppMmio);
  }

  if (CheckBit & (BIT_8(7) + BIT_8(1))) {
    xUSLPciWrite32(XhciDspAddr + 0x18, PtPciTable->XhciDspBus);
  }

  if (CheckBit & (BIT_8(7) + BIT_8(2))) {
    xUSLPciWrite32(SataDspAddr + 0x18, PtPciTable->SataDspBus);
  }

  if (CheckBit & (BIT_8(7) + BIT_8(2) + BIT_8(1) + BIT_8(0))) {
    xUSLPciWrite32(PromAddr + 0x18, PtPciTable->UspBus);
    xUSLPciWrite32(PciAddr + 0x18, PtPciTable->GppBus);
  }

  PROM_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}


/**
 * RestorePromBus
 * 
 * @brief Restore Prom Bus/Command/Mmio/Aspm
 *
 * @param PtPciTable   Save Restore Table location
 * @param CheckBit     Bit0  = 1 - Restore Original UspBus
 *                     Bit1  = 1 - Restore Original XhciBus
 *                     Bit2  = 1 - Restore Original SataBus
 *                     Bit3  = 1 - Restore Original XhciMmio
 *                     Bit4  = 1 - Restore Original UspCmd
 *                     Bit5  = 1 - Restore Original XhciCmd
 *                     Bit6  = 1 - Restore Original SataCmd
 *                     Bit7  = 1 - Restore Original ASPM
 *                     Bit8  = 1 - Restore Original SecUspBus
 *                     Bit9  = 1 - Restore Original SecXhciBus
 *                     Bit10 = 1 - Restore Original SecSataBus
 *                     Bit11 = 1 - Restore Original SecXhciMmio
 *                     Bit12 = 1 - Restore Original SecUspCmd
 *                     Bit13 = 1 - Restore Original SecXhciCmd
 *                     Bit14 = 1 - Restore Original SecSataCmd
 *                     Bit15 = 1 - Restore Original Secondary ASPM
 */
void
RestorePromBus (
  PROMCLASS_DATA_BLK              *PromDataBlk,
  PROM_PCI_SAVE_RESTORE_TABLE     *PtPciTable,
  uint16_t                        CheckBit
  )
{
  RestoreSingleProm(PromDataBlk, &PtPciTable[1], CheckBit >> 8, false);
  RestoreSingleProm(PromDataBlk, &PtPciTable[0], CheckBit & 0xFF, true);
}

SIL_STATUS
Prom21RetrainSpeed (
  PROMCLASS_DATA_BLK *PromDataBlk,
  uint16_t           Bus,
  uint8_t            Dev,
  uint8_t            Func,
  uint32_t           XhciMmio,
  uint8_t            Speed,
  bool               LinkDisable
  )
{
  SIL_STATUS      Status;
  uint16_t        LinkStatus;
  uint8_t         TargetSpeed;
  uint8_t         CurrentSpeed;
  uint8_t         CapBaseAddr;
  size_t          Counter;
  uint8_t         AssertPin;
  uint32_t        Address;

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "[%04X:%02X:%02X] at Speed %d\n", Bus, Dev, Func, Speed);

  Status = SilDeviceError;
  Address = PCI_LIB_ADDRESS(Bus, Dev, Func, 0);

  CapBaseAddr = xUSLPciLibFindPciCapability(Address, PCIE_CAP_ID);
  if ((CapBaseAddr == 0) || (CapBaseAddr == 0xFF)) {
    return Status;
  }

  Status = SilPass;
  CurrentSpeed = xUSLPciRead8(Address + CapBaseAddr + PCIE_LINK_STATUS_REG);

  PROM_TRACEPOINT(SIL_TRACE_INFO, "PCIe Link Width & Link Speed is x%d Gen%d LinkDisable (%d)\n",
    CurrentSpeed >> 4, CurrentSpeed & 0xF, LinkDisable);

  if (LinkDisable) {
    LinkStatus = xUSLPciRead16(Address + CapBaseAddr + PCIE_LINK_CONTROL_REG);
    LinkStatus |= BIT_16(4);
    xUSLPciWrite16(Address + CapBaseAddr + PCIE_LINK_CONTROL_REG, LinkStatus);

     if (XhciMmio != 0) {
      SilFchStall(5 * 1000);
      PROM_TRACEPOINT(SIL_TRACE_INFO, " Assert GPP PERST# before link disabled\n");
      AssertPin = Prom21XhciReadByteV2(PromDataBlk, XhciMmio, 0x2E20F);
      AssertPin |= BIT_8(4);
      Prom21XhciWriteByte(XhciMmio, 0x2E20F, AssertPin);
      SilFchStall(20 * 1000);
    }
    do {
      LinkStatus = xUSLPciRead16(Address + CapBaseAddr + PCIE_LINK_STATUS_REG);
    } while (LinkStatus & BIT_16(13));

    TargetSpeed = xUSLPciRead8(Address + CapBaseAddr + PCIE_LINK_CONTROL2_REG);
    TargetSpeed &= 0xF0;
    TargetSpeed |= Speed & 0x0F;
    xUSLPciWrite8(Address + CapBaseAddr + PCIE_LINK_CONTROL2_REG, TargetSpeed);

    LinkStatus = xUSLPciRead16(Address + CapBaseAddr + PCIE_LINK_CONTROL_REG);
    LinkStatus &= ~BIT_16(4);
    xUSLPciWrite16(Address + CapBaseAddr + PCIE_LINK_CONTROL_REG, LinkStatus);

    SilFchStall(50 * 1000);
    if (XhciMmio != 0) {
      PROM_TRACEPOINT(SIL_TRACE_INFO, " De-assert GPP PERST# before DL ACTIVE\n");
      AssertPin = Prom21XhciReadByteV2(PromDataBlk, XhciMmio, 0x2E20F);
      AssertPin &= ~BIT_8(4);
      Prom21XhciWriteByte(XhciMmio, 0x2E20F, AssertPin);
    }

    SilFchStall(50 * 1000);
    for (Counter = 0; Counter < 100; Counter++) {
      LinkStatus = xUSLPciRead16(Address + CapBaseAddr + PCIE_LINK_STATUS_REG);
      if (LinkStatus & BIT_16(13)) {
        break;
      }
      SilFchStall(2 * 1000);
    }
  } else {
    if (Speed == 0x4) {
      if ((CurrentSpeed & 0xF) < 0x3) {
        TargetSpeed = xUSLPciRead8(Address + CapBaseAddr + PCIE_LINK_CONTROL2_REG);
        TargetSpeed &= 0xF0;
        TargetSpeed |= 0x3 & 0x0F;
        xUSLPciWrite8(Address + CapBaseAddr + PCIE_LINK_CONTROL2_REG, TargetSpeed);

        LinkStatus = xUSLPciRead16(Address + CapBaseAddr + PCIE_LINK_CONTROL_REG);
        LinkStatus |= BIT_16(5);
        xUSLPciWrite16(Address + CapBaseAddr + PCIE_LINK_CONTROL_REG, LinkStatus);

        SilFchStall(20 * 1000);
        for (Counter = 0; Counter < 100; Counter++) {
          TargetSpeed = xUSLPciRead8(Address + CapBaseAddr + PCIE_LINK_STATUS_REG);
          if ((TargetSpeed & 0x0F) == (0x3 & 0x0F)) {
            break;
          }
          SilFchStall(2 * 1000);
        }
      }
    }

    TargetSpeed = xUSLPciRead8(Address + CapBaseAddr + PCIE_LINK_CONTROL2_REG);
    TargetSpeed &= 0xF0;
    TargetSpeed |= Speed & 0x0F;
    xUSLPciWrite8(Address + CapBaseAddr + PCIE_LINK_CONTROL2_REG, TargetSpeed);

    LinkStatus = xUSLPciRead16(Address + CapBaseAddr + PCIE_LINK_CONTROL_REG);
    LinkStatus |= BIT_16(5);
    xUSLPciWrite16(Address + CapBaseAddr + PCIE_LINK_CONTROL_REG, LinkStatus);
  }

  SilFchStall(20 * 1000);
  for (Counter = 0; Counter < 100; Counter++) {
    TargetSpeed = xUSLPciRead8(Address + CapBaseAddr + PCIE_LINK_STATUS_REG);
    if ((TargetSpeed & 0x0F) == (Speed & 0x0F)) {
      break;
    }
    SilFchStall(2 * 1000);
  }

  CurrentSpeed = xUSLPciRead8(Address + CapBaseAddr + PCIE_LINK_STATUS_REG);
  PROM_TRACEPOINT(SIL_TRACE_INFO, "PCIe Link Width & Link Speed is x%d Gen%d Counter = %d\n",
    CurrentSpeed >> 4, CurrentSpeed & 0xF, Counter);

  if ((CurrentSpeed & 0x0F) == (Speed & 0x0F)) {
    PROM_TRACEPOINT(SIL_TRACE_INFO, "  Restore PCIe link speed to Gen%d Success !!!\n",
      CurrentSpeed & 0x0F);
  } else {
    PROM_TRACEPOINT(SIL_TRACE_INFO, "  Restore PCIe link speed to Gen%d Fail !!! It's Gen%d now\n",
      Speed & 0x0F, CurrentSpeed & 0x0F);
  }

  return Status;
}