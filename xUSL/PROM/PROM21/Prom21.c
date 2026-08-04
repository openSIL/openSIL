/* SPDX-License-Identifier: MIT */
/* Copyright(C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file Prom21.c
 *  @brief Promontory21 related functions
 */

#include <SilCommon.h>
#include <PciLib.h>
#include <PROM/Common/PromAccess.h>
#include <PROM/Common/PromInit.h>
#include <PROM/PromClass-api.h>
#include "Prom21.h"

static void
Prom21SvidSsid (
  PROM21_DATA_BLK   *PromDataBlk,
  uint32_t           XhciMmio
  )
{
  uint16_t          Value;

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "XhciMmio 0x%08X\n", XhciMmio);

  Prom21XhciWriteByte(XhciMmio, 0x24788, 0x86);

  if (PromDataBlk->PT21XhciSsid != 0x11421B21) {
    Value = (uint16_t)PromDataBlk->PT21XhciSsid;
    PROM_TRACEPOINT(SIL_TRACE_INFO, "XhciSvid = 0x%08X\n", Value);
    Prom21XhciWriteWord(XhciMmio, 0x2475A, Value);

    Value = (uint16_t)(PromDataBlk->PT21XhciSsid >> 16);
    PROM_TRACEPOINT(SIL_TRACE_INFO, "XhciSsid = 0x%08X\n", Value);
    Prom21XhciWriteWord(XhciMmio, 0x2474A, Value);
  }

  if (PromDataBlk->PT21AhciSsid != 0x10621B21) {
    Value = (uint16_t)PromDataBlk->PT21AhciSsid;
    PROM_TRACEPOINT(SIL_TRACE_INFO, "AhciSvid = 0x%08X\n", Value);
    Prom21XhciWriteWord(XhciMmio, 0x2475C, Value);

    Value = (uint16_t)(PromDataBlk->PT21AhciSsid >> 16);
    PROM_TRACEPOINT(SIL_TRACE_INFO, "AhciSsid = 0x%08X\n", Value);
    Prom21XhciWriteWord(XhciMmio, 0x2474C, Value);
  }

  if (PromDataBlk->PT21PcieUspSsid != 0x33281B21) {
    Value = (uint16_t)PromDataBlk->PT21PcieUspSsid;
    PROM_TRACEPOINT(SIL_TRACE_INFO, "PcieUspSvid = 0x%08X\n", Value);
    Prom21XhciWriteWord(XhciMmio, 0x24750, Value);

    Value = (uint16_t)(PromDataBlk->PT21PcieUspSsid >> 16);
    PROM_TRACEPOINT(SIL_TRACE_INFO, "PcieUspSsid = 0x%08X\n", Value);
    Prom21XhciWriteWord(XhciMmio, 0x24740, Value);
  }

  if (PromDataBlk->PT21PcieDspSsid != 0x33281B21) {
    Value = (uint16_t)PromDataBlk->PT21PcieDspSsid;
    PROM_TRACEPOINT(SIL_TRACE_INFO, "PcieDspSvid = 0x%08X\n", Value);
    Prom21XhciWriteWord(XhciMmio, 0x24752, Value);

    Value = (uint16_t)(PromDataBlk->PT21PcieDspSsid >> 16);
    PROM_TRACEPOINT(SIL_TRACE_INFO, "PcieDspSsid = 0x%08X\n", Value);
    Prom21XhciWriteWord(XhciMmio, 0x24742, Value);
  }

  if (PromDataBlk->PT21PcieDspXhciSsid != 0x33281B21) {
    Value = (uint16_t)PromDataBlk->PT21PcieDspXhciSsid;
    PROM_TRACEPOINT(SIL_TRACE_INFO, "PcieDspXhciSvid = 0x%08X\n", Value);
    Prom21XhciWriteWord(XhciMmio, 0x24756, Value);

    Value = (uint16_t)(PromDataBlk->PT21PcieDspXhciSsid >> 16);
    PROM_TRACEPOINT(SIL_TRACE_INFO, "PcieDspXhciSsid = 0x%08X\n", Value);
    Prom21XhciWriteWord(XhciMmio, 0x24746, Value);
  }

  if (PromDataBlk->PT21PcieDspAhciSsid != 0x33281B21) {
    Value = (uint16_t)PromDataBlk->PT21PcieDspAhciSsid;
    PROM_TRACEPOINT(SIL_TRACE_INFO, "PcieDspAhciSvid = 0x%08X\n", Value);
    Prom21XhciWriteWord(XhciMmio, 0x24758, Value);

    Value = (uint16_t)(PromDataBlk->PT21PcieDspAhciSsid >> 16);
    PROM_TRACEPOINT(SIL_TRACE_INFO, "PcieDspAhciSsid = 0x%08X\n", Value);
    Prom21XhciWriteWord(XhciMmio, 0x24748, Value);
  }

  Prom21XhciWriteByte(XhciMmio, 0x24788, 0x0);

  PROM_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

static void
Prom21ThermalSetting (
  PROM21_DATA_BLK *PromDataBlk,
  uint32_t        XhciMmio
  )
{
  uint8_t RegMisc;

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "XhciMmio 0x%08X\n", XhciMmio);

  if (PromDataBlk->PT21ThermalThrottle != 0xf) {
    if (PromDataBlk->PT21ThermalThrottle == 1) {
      Prom21XhciWriteDWord(XhciMmio, 0x1C520, 0x12345678);
      Prom21XhciWriteByte(XhciMmio, 0x1E521, PromDataBlk->PT21ThermalThreshold);
      RegMisc = Prom21XhciReadByte(XhciMmio, 0x1C51F);
      Prom21XhciWriteByte(XhciMmio, 0x1C51F, RegMisc | BIT_8(1));
    } else {
      Prom21XhciWriteDWord(XhciMmio, 0x1C520, 0x12345678);
      RegMisc = Prom21XhciReadByte(XhciMmio, 0x1C51F);
      Prom21XhciWriteByte(XhciMmio, 0x1C51F, RegMisc & ~BIT_8(1));
    }
  }

  PROM_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

static void
Prom21Gen1Swing (
  uint32_t XhciMmio,
  uint8_t  PortNum,
  uint8_t  Value
  )
{
  uint32_t  Address;
  uint8_t   Data8;

  Address = 0x2C001 + (PortNum <<  8);
  Data8 = Prom21XhciReadByte(XhciMmio, Address);

  switch(PortNum)
  {
    case 0:
    case 1:
    case 2:
    case 3:
    case 8:
    case 9:
    case 10:
    case 11:
      Data8 &= 0xE0;
      break;
    case 4:
    case 5:
    case 6:
    case 7:
      Data8 &= 0xF0;
      break;
    default:
      break;
  }
  Data8 |= Value;

  Prom21XhciWriteByte(XhciMmio, Address, Value);
}

static void
Prom21PcieSIConfig (
  PROM21_DATA_BLK  *PromDataBlk,
  uint32_t         XhciMmio
)
{
  uint8_t  *PcieGen1Ptr;
  uint8_t  Data8;
  uint8_t  i;

  PcieGen1Ptr = PromDataBlk->PT21PcieGen1Swing;
  if (PcieGen1Ptr == NULL) {
    return;
  }

  for(i = 0; i < PROM21_NUM_PCIE_LANES; i++) {
    PROM_TRACEPOINT(SIL_TRACE_INFO, "[Prom21] Port %d: Gen1Swing Value = %x\n", i, *PcieGen1Ptr);
    Prom21Gen1Swing(XhciMmio, i, *PcieGen1Ptr);
    PcieGen1Ptr++;
  }

  Data8 = Prom21XhciReadByte(XhciMmio, 0x1C51F);
  Data8 |= BIT_8(2);
  Prom21XhciWriteByte(XhciMmio, 0x1C51F, Data8);

}

static void
Prom21DSPortSetting (
  uint32_t   XhciMmio,
  bool       Enable,
  uint32_t   PortNum
)
{
  uint32_t  Address;
  uint16_t  Data16;

  if (PortNum > PROM21_MAX_DSP_NUMBER) {
    return;
  }

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "XhciMmio 0x%08X\n", XhciMmio);

  Prom21XhciWriteByte(XhciMmio, 0x24788, 0x86);

  Address = 0x24734;
  Data16 = Prom21XhciReadWord(XhciMmio, Address);

  if (Enable) {
    Data16 |= BIT_16(PortNum);
  } else {
    Data16 &= ~BIT_16(PortNum);
  }

  Prom21XhciWriteWord(XhciMmio, Address, Data16);
  Prom21XhciWriteByte(XhciMmio, 0x24788, 0x0);

  PROM_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

static void
Prom21PcieTargetSpeedConfig (
  PROMCLASS_DATA_BLK *PromDataBlk,
  uint16_t    UspBus,
  uint8_t     PortNum,
  uint8_t     TargetSpeed
  )
{
  uint16_t   DspBus;

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (xUSLPciRead32(PCI_LIB_ADDRESS(UspBus, 0, 0, 0)) == PT21_USP_ID) {
    DspBus = (uint16_t)xUSLPciRead8(PCI_LIB_ADDRESS(UspBus, 0, 0, 0x19)) | (UspBus & 0xFF00);
    if (DspBus != 0){
      Prom21RetrainSpeed(PromDataBlk, DspBus, PortNum, 0, 0, TargetSpeed, false);
    }
  }

  PROM_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

static void
Prom21DspStatusCheck (
  uint32_t   XhciMmio,
  uint32_t   PortNum
  )
{
  uint32_t  Address;
  uint16_t  Data16;

  if (PortNum > PROM21_MAX_DSP_NUMBER) {
    return;
  }

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "XhciMmio 0x%08X\n", XhciMmio);

  Prom21XhciWriteByte(XhciMmio, 0x24788, 0x86);

  Address = 0x24734;
  Data16 = Prom21XhciReadWord(XhciMmio, Address);

  Prom21XhciWriteByte(XhciMmio, 0x24788, 0x0);

  if (Data16 & ((uint16_t)(1 << PortNum))) {
    PROM_TRACEPOINT(SIL_TRACE_INFO, "Port %d is default value\n", PortNum);
  } else {
    Prom21DSPortSetting(XhciMmio, 1, PortNum);
  }

  PROM_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

static void
Prom21PcieConfig (
  PROMCLASS_DATA_BLK *PromClassBlk,
  PROM21_DATA_BLK    *PromDataBlk,
  uint16_t           UspBus,
  uint32_t           XhciMmio
  )
{
  uint8_t PortNum;

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "UspBus=%x XhciMmio=0x%08x\n", UspBus , XhciMmio);

  for (PortNum = 0; PortNum < PROM21_NUM_PCIE_LANES; PortNum++) {
    if (PromDataBlk->PT21PciePortEnable[PortNum] != 0xf) {
      if (PromDataBlk->PT21PciePortEnable[PortNum] == 1) {
        Prom21DSPortSetting(XhciMmio, 1, PortNum);
        if (PromDataBlk->PT21PciePortTargetSpeed[PortNum] != 0xf) {
          Prom21PcieTargetSpeedConfig(PromClassBlk,
            UspBus,
            PortNum,
            PromDataBlk->PT21PciePortTargetSpeed[PortNum]
            );
        }
      } else {
        Prom21DSPortSetting(XhciMmio, 0, PortNum);
      }
    } else {
      Prom21DspStatusCheck(XhciMmio, PortNum);
    }
  }

  PROM_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

static void
Prom21DisableUnusedPciePorts (
  uint32_t   GppPcieAddress,
  uint32_t   XhciMmio
  )
{
  uint32_t                VidDid;
  uint8_t                 DevNum;
  uint8_t                 SlotStatus;
  uint8_t                 HotPlugCapable;
  uint16_t                GppBus;
  uint8_t                 GppDev;
  uint8_t                 GppFun;
  uint16_t                PtUspBus;
  uint16_t                PtDspBus;
  uint8_t                 CapBase;
  uint32_t                UspDev;
  uint32_t                DspDev;

  GppBus = (uint16_t)((GppPcieAddress >> 20) & 0xFFF);
  GppDev = (uint8_t)((GppPcieAddress >> 15) & 0x1F);
  GppFun = (uint8_t)((GppPcieAddress >> 12) & 0x07);

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "[%04X:%02X:%02X]\n", GppBus, GppDev, GppFun);

  PtUspBus = (uint16_t) xUSLPciRead8(GppPcieAddress + 0x19) | (GppBus & 0xFF00);
  UspDev = PCI_LIB_ADDRESS(PtUspBus, 0, 0, 0x00);
  VidDid = xUSLPciRead32(UspDev);
  PROM_TRACEPOINT(SIL_TRACE_INFO, "PtUspBus %x, VidDid %x\n", PtUspBus, VidDid);

  if (VidDid == PT21_USP_ID) {
    PtDspBus = (uint16_t) xUSLPciRead8(UspDev + 0x19) | (PtUspBus & 0xFF00);
    for (DevNum = 0; DevNum <= PROM21_MAX_DSP_NUMBER; DevNum ++) {
      DspDev = PCI_LIB_ADDRESS(PtDspBus, DevNum, 0, 0x00);
      VidDid = xUSLPciRead32(DspDev);
      PROM_TRACEPOINT(SIL_TRACE_INFO, "PtDspBus %x, VidDid %x\n", PtDspBus, VidDid);
      if (VidDid == PT21_DSP_ID) {
        CapBase = xUSLPciLibFindPciCapability(DspDev, PCIE_CAP_ID);
        if ((CapBase == 0) || (CapBase == 0xff)) {
          continue;
        }

        SlotStatus = xUSLPciRead8(DspDev + CapBase + PCIE_SLOT_STATUS_REG);
        HotPlugCapable = xUSLPciRead8(DspDev +CapBase + PCIE_LINK_STATUS_REG + 2);
        PROM_TRACEPOINT(SIL_TRACE_INFO, "SlotStatus %x, HotPlugCapable %x at Dev 0x%x\n",
                        SlotStatus, HotPlugCapable, DevNum);

        if (((SlotStatus & BIT_8(6)) != BIT_8(6)) &&
            ((HotPlugCapable & BIT_8(6)) != BIT_8(6))) {
          Prom21DSPortSetting(XhciMmio, 0, DevNum);
        }
      } else {
        Prom21DSPortSetting(XhciMmio, 0, DevNum);
      }
    }
  }

  PROM_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

SIL_STATUS
Prom21Setting (
  SIL_CONTEXT        *SilContext,
  PROMCLASS_DATA_BLK *PromDataBlk
  )
{
  uint32_t                        XhciMmio;
  uint32_t                        SecXhciMmio;
  uint8_t                         Data8;
  PROM_PCI_SAVE_RESTORE_TABLE     PtPciTable[2];
  uint32_t                        SecGppPcieAddress;

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (PromDataBlk->PromOutputBlk.PT21XhciID[0] == 0) {
    PROM_TRACEPOINT(SIL_TRACE_ERROR, "Promontory is NOT FOUND!!\n");
    return SilNotFound;
  }

  if (PromDataBlk->PromInputBlk.BootMode == 0x20) {
    PROM_TRACEPOINT(SIL_TRACE_INFO, "Boot in Recovery mode!!\n");
    return SilPass;;
  }

  XhciMmio = PromDataBlk->PromOutputBlk.PT21XhciMmio[0];
  SecGppPcieAddress = PromDataBlk->PromOutputBlk.PT21GppPcieAddress[1];
  SecXhciMmio = 0;

  if (SecGppPcieAddress) {
    SaveInitPromBus(PromDataBlk, PtPciTable, 0x3B3B);
    SecXhciMmio = PromDataBlk->PromOutputBlk.PT21XhciMmio[1];
  } else {
    SaveInitPromBus(PromDataBlk, PtPciTable, 0x3B);
  }

  //if (PromDataBlk->PromInputBlk.BootMode == 0x11 /* BOOT_ON_S3_RESUME */)
  {
    PROM_TRACEPOINT(SIL_TRACE_INFO, "  BootMode == BOOT_ON_S3_RESUME\n");
    if (PromDataBlk->PromOutputBlk.PT21XhciLock[0]) {
      PROM_TRACEPOINT(SIL_TRACE_INFO, "  Lock PT for S3 Resume !!!\n");
      Data8 = Prom21XhciReadByte(XhciMmio, 0x15087);
      Prom21XhciWriteByte(XhciMmio, 0x15087, Data8 & ~BIT_8(4));
    }
    if (PromDataBlk->PromOutputBlk.PT21XhciLock[1]) {
      PROM_TRACEPOINT(SIL_TRACE_INFO, "  Lock Secondary PT for S3 Resume !!!\n");
      Data8 = Prom21XhciReadByte(SecXhciMmio, 0x15087);
      Prom21XhciWriteByte(SecXhciMmio, 0x15087, Data8 & ~BIT_8(4));
    }
  }

  Prom21UsbSetting(&PromDataBlk->PromInputBlk.Primary,
    XhciMmio,
    PromDataBlk->PromInputBlk.BootMode);
  Prom21PcieConfig(PromDataBlk,
    &PromDataBlk->PromInputBlk.Primary,
    PtPciTable[0].PromBus,
    XhciMmio
    );

  if (PromDataBlk->PromInputBlk.Primary.PT21PcieGen1SwingEnable == 1) {
    Prom21PcieSIConfig(&PromDataBlk->PromInputBlk.Primary, XhciMmio);
  }

  if (PromDataBlk->PromInputBlk.Primary.PT21SsidOverride == 1) {
    Prom21SvidSsid(&PromDataBlk->PromInputBlk.Primary, XhciMmio);
  }

  Prom21SataSetting(PromDataBlk);
  Prom21ThermalSetting(&PromDataBlk->PromInputBlk.Primary, XhciMmio);

  if (SecXhciMmio != 0) {
    Prom21UsbSetting(&PromDataBlk->PromInputBlk.Secondary,
      SecXhciMmio,
      PromDataBlk->PromInputBlk.BootMode);
    Prom21PcieConfig(PromDataBlk,
      &PromDataBlk->PromInputBlk.Secondary,
      PtPciTable[1].PromBus,
      SecXhciMmio
      );

    if (PromDataBlk->PromInputBlk.Secondary.PT21PcieGen1SwingEnable == 1) {
      Prom21PcieSIConfig(&PromDataBlk->PromInputBlk.Secondary, SecXhciMmio);
    }

    if (PromDataBlk->PromInputBlk.Secondary.PT21SsidOverride == 1) {
      Prom21SvidSsid(&PromDataBlk->PromInputBlk.Secondary, SecXhciMmio);
    }

    Prom21ThermalSetting(&PromDataBlk->PromInputBlk.Secondary, SecXhciMmio);
  }

  if (SecGppPcieAddress) {
    RestorePromBus(PromDataBlk, PtPciTable, 0x3B3B);
  } else {
    RestorePromBus(PromDataBlk, PtPciTable, 0x3B);
  }

  PROM_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

SIL_STATUS
Prom21LateSetting (
  SIL_CONTEXT        *SilContext,
  PROMCLASS_DATA_BLK *PromDataBlk
  )
{
  PROM_PCI_SAVE_RESTORE_TABLE PtPciTable[2];
  uint32_t                    XhciMmio;
  uint32_t                    SecXhciMmio;
  uint32_t                    GppPcieAddress;
  uint32_t                    SecGppPcieAddress;

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  XhciMmio          = PromDataBlk->PromOutputBlk.PT21XhciMmio[0];
  GppPcieAddress    = PromDataBlk->PromOutputBlk.PT21GppPcieAddress[0];
  SecGppPcieAddress = PromDataBlk->PromOutputBlk.PT21GppPcieAddress[1];
  SecXhciMmio       = 0;

  PROM_TRACEPOINT(SIL_TRACE_INFO, "XhciMmio = %x, GppPcieAddress = %x, SecGppPcieAddress = %x\n",
                  XhciMmio, GppPcieAddress, SecGppPcieAddress);

  if (PromDataBlk->PromInputBlk.PT21DisableUnusedPciePort != 0xf) {
    if (PromDataBlk->PromInputBlk.PT21DisableUnusedPciePort == 1) {
      if (SecGppPcieAddress) {
        SaveInitPromBus(PromDataBlk, PtPciTable, 0x3B3B);
        SecXhciMmio = PromDataBlk->PromOutputBlk.PT21XhciMmio[1];
      } else {
        SaveInitPromBus(PromDataBlk, PtPciTable, 0x3B);
      }

      if (GppPcieAddress) {
        Prom21DisableUnusedPciePorts(GppPcieAddress, XhciMmio);
      }
      if (SecGppPcieAddress) {
        Prom21DisableUnusedPciePorts(SecGppPcieAddress, SecXhciMmio);
      }

      if (SecGppPcieAddress) {
        RestorePromBus(PromDataBlk, PtPciTable, 0x3B3B);
      } else {
        RestorePromBus(PromDataBlk, PtPciTable, 0x3B);
      }
    }
  }

  return SilPass;
}
