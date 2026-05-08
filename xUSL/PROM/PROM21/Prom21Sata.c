/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file Prom21Sata.c
 *  @brief Promontory21 SATA related functions
 */

#include <SilCommon.h>
#include <PROM/Common/PromAccess.h>
#include <PROM/Common/PromInit.h>
#include <PROM/PromClass-api.h>
#include "Prom21.h"
#include "Prom21Init.h"

static void
Prom21SetSataClassCode (
  uint32_t  XhciMmio,
  uint32_t  ClassCode,
  uint16_t  DeviceId
)
{
  Prom21XhciWriteByte(XhciMmio, 0x24788, 0x86);
  Prom21XhciWriteDWord(XhciMmio, 0x24714, ClassCode);
  Prom21XhciWriteByte(XhciMmio, 0x24700, 0x2);
  Prom21XhciWriteWord(XhciMmio, 0x24706, DeviceId);
  Prom21XhciWriteByte(XhciMmio, 0x24788, 0x0);
}

static void
Prom21SetSataMode (
  PROM21_DATA_BLK *PromDataBlk,
  uint32_t        XhciMmio
)
{
  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (PromDataBlk->PT21SataMode != 0xf) {
    if (PromDataBlk->PT21SataMode == PTSataAhci) {
      Prom21SetSataClassCode (XhciMmio, PT_SATA_AHCI_CLASS_CODE, PT_SATA_AHCI_DID);
    } else {
      Prom21SetSataClassCode (XhciMmio, PT_SATA_RAID_CLASS_CODE, PT_SATA_RAID_DID);
    }
  }

  PROM_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

static void
Prom21SataControllerEnable (
  uint32_t XhciMmio,
  bool     Enable
  )
{
  uint8_t Data8;

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "XhciMmio 0x%08X, Enable %u\n", XhciMmio, Enable);

  Data8 = Prom21XhciReadByte (XhciMmio, 0x10151);

  if (Enable) {
    if ((Data8 & 0x02) == 0) {
      Data8 |= 0x02;
      Prom21XhciWriteByte (XhciMmio, 0x10151, Data8);
    }
  } else {
    if ((Data8 & 0x02) != 0) {
      Data8 &= (~0x02);
      Prom21XhciWriteByte (XhciMmio, 0x10151, Data8);
    }
  }

  PROM_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return;
}

static void
Prom21SataPortCtrl (
  uint32_t XhciMmio,
  bool     Enable,
  uint8_t  PortNum
  )
{
  uint8_t  Value8;

  if (PortNum >= PROM21_NUM_SATA_PORTS) {
    return;
  }

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Prom21XhciWriteByte (XhciMmio, 0x24788, 0x86);

  Value8 = Prom21XhciReadByte(XhciMmio, 0x2471C);

  if (Enable) {
    Value8 |= BIT_8(PortNum);
  } else {
    Value8 &= ~BIT_8(PortNum);
  }

  Prom21XhciWriteByte (XhciMmio, 0x2471C, Value8);
  Prom21XhciWriteByte (XhciMmio, 0x24788, 0x0);

  Value8 = Prom21XhciReadByte(XhciMmio, 0x2E006);

  if (Enable) {
    Value8 |= BIT_8(PortNum);
  } else {
    Value8 &= ~BIT_8(PortNum);
  }

  Prom21XhciWriteByte (XhciMmio, 0x2E006, Value8);

  PROM_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return;
}

static void
Prom21GppClockOutput (
  uint32_t    XhciMmio,
  uint8_t     ClkReqNum,
  uint8_t     ReqMode
  )
{
  uint32_t Address;
  uint16_t Data16;

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if((ClkReqNum > PROM21_NUM_PCIE_CLKREQ) || (ReqMode > 3)) {
    return;
  }

  Prom21XhciWriteByte (XhciMmio, 0x24788, 0x86);

  Address = 0x24720;
  Data16 = Prom21XhciReadWord (XhciMmio, Address);

  PROM_TRACEPOINT(SIL_TRACE_INFO, "Read Data16 = 0x%x @ Address 0x%x\n", Data16, Address);

  switch(ClkReqNum)
  {
    case 0:
      Data16 &= ~(BIT_16(0) + BIT_16(1));
      Data16 |= ReqMode;
      break;
    case 1:
      Data16 &= ~(BIT_16(2) + BIT_16(3));
      Data16 |= (ReqMode << 2);
      break;
    case 2:
      Data16 &= ~(BIT_16(4) + BIT_16(5));
      Data16 |= (ReqMode << 4);
      break;
    case 3:
      Data16 &= ~(BIT_16(6) + BIT_16(7));
      Data16 |= (ReqMode << 6);
      break;
    case 4:
      Data16 &= ~(BIT_16(8) + BIT_16(9));
      Data16 |= (ReqMode << 8);
      break;
    case 5:
      Data16 &= ~(BIT_16(10) + BIT_16(11));
      Data16 |= (ReqMode << 10);
      break;
    default:
      break;
  }

  Prom21XhciWriteWord(XhciMmio, Address, Data16);
  PROM_TRACEPOINT(SIL_TRACE_INFO, "Write Data16 = 0x%x @ Address 0x%x\n", Data16, Address);

  Data16 = Prom21XhciReadWord (XhciMmio, Address);

  PROM_TRACEPOINT(SIL_TRACE_INFO, "Read back Data16 = 0x%x @ Address 0x%x\n", Data16, Address);

  Prom21XhciWriteByte (XhciMmio, 0x24788, 0x0);

  PROM_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}


static void
Prom21GppClockConfig (
  PROM21_DATA_BLK *PromDataBlk,
  uint32_t        XhciMmio
  )
{
  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "XhciMmio 0x%08x\n", XhciMmio);

  for (uint8_t Clk = 0; Clk < PROM21_NUM_PCIE_CLKREQ; Clk++) {
    if(PromDataBlk->PT21PcieClkreqMode[Clk] != 0xf) {
      Prom21GppClockOutput(XhciMmio, Clk, PromDataBlk->PT21PcieClkreqMode[Clk]);
    }
  }

  PROM_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

static void
Prom21ClkReqPinSelect (
  uint32_t    XhciMmio,
  uint8_t     ClkReqNum,
  uint8_t     PortNumSel
  )
{
  uint32_t       Address;
  uint32_t       Data32;

  Data32 = 0;

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if ((ClkReqNum > PROM21_NUM_PCIE_CLKREQ) || (PortNumSel > 0xf)) {
    return;
  }

  Prom21XhciWriteByte (XhciMmio, 0x24788, 0x86);

  Address = 0x24724;
  Data32 = Prom21XhciReadDWord (XhciMmio, Address);

  PROM_TRACEPOINT(SIL_TRACE_INFO, "Read Data32 = 0x%x @ Address 0x%x\n", Data32, Address);

  switch(ClkReqNum)
  {
    case 0:
      Data32 &= 0xFFFFFFF0;
      Data32 |= PortNumSel;
      break;
    case 1:
      Data32 &= 0xFFFFFF0F;
      Data32 |= (PortNumSel << 4);
      break;
    case 2:
      Data32 &= 0xFFFFF0FF;
      Data32 |= (PortNumSel << 8);
      break;
    case 3:
      Data32 &= 0xFFFF0FFF;
      Data32 |= (PortNumSel << 12);
      break;
    case 4:
      Data32 &= 0xFFF0FFFF;
      Data32 |= (PortNumSel << 16);
      break;
    case 5:
      Data32 &= 0xFF0FFFFF;
      Data32 |= (PortNumSel << 20);
      break;
    default:
      break;
  }


  Prom21XhciWriteDWord (XhciMmio, Address, Data32);

  PROM_TRACEPOINT(SIL_TRACE_INFO, "Write Data32 = 0x%x @ Address 0x%x\n", Data32, Address);

  Data32 = Prom21XhciReadDWord (XhciMmio, Address);

  PROM_TRACEPOINT(SIL_TRACE_INFO, "Readback Data32 = 0x%x @ Address 0x%x\n", Data32, Address);

  Prom21XhciWriteByte (XhciMmio, 0x24788, 0x0);

  PROM_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

static void
Prom21ClkConfig (
  PROM21_DATA_BLK *PromDataBlk,
  uint32_t        XhciMmio
  )
{
  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "XhciMmio 0x%08x\n", XhciMmio);

  for (uint8_t Clk = 0; Clk < PROM21_NUM_PCIE_CLKREQ; Clk++) {
    if(PromDataBlk->PT21PcieClkreqPinSelect[Clk] != 0xf) {
      Prom21ClkReqPinSelect(XhciMmio, Clk, PromDataBlk->PT21PcieClkreqPinSelect[Clk]);
    }
  }

  PROM_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

static void
Prom21SetAhciDevslp (
  uint32_t    XhciMmio,
  bool        Enable,
  uint8_t     PortNum
  )
{
  uint32_t  Address;
  uint8_t   Value8;

  if (PortNum >= PROM21_NUM_SATA_PORTS) {
    return;
  }

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "PortNum- %d -Enable(%d)\n", PortNum, Enable);

  Address = 0x2E00A;
  Value8 = Prom21XhciReadByte(XhciMmio, Address);
  PROM_TRACEPOINT(SIL_TRACE_INFO, "Read Value8 = 0x%x, Address = 0x%x\n", Value8, Address);

  if (Enable) {
    Value8 |= BIT_8(PortNum);
  } else {
    Value8 &= ~BIT_8(PortNum);
  }

  PROM_TRACEPOINT(SIL_TRACE_INFO, "Write Value8 = 0x%x, Address = 0x%x\n", Value8, Address);

  Prom21XhciWriteByte (XhciMmio, Address, Value8);
}

static void
Prom21SetAhciCapReg (
  uint32_t XhciMmio,
  uint32_t Capreg,
  bool     Enable
  )
{
  uint32_t Address;
  uint32_t Value32;

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "Reg- 0x%x -Enable(%d)\n", Capreg, Enable);

  Address = 0x2E000;
  Value32 = Prom21XhciReadDWord (XhciMmio, Address);

  PROM_TRACEPOINT(SIL_TRACE_INFO, "Read Value32 = 0x%x, Address = 0x%x\n", Value32, Address);

  if (Enable) {
    Value32 |= Capreg;
  } else {
    Value32 &= (~Capreg);
  }

  PROM_TRACEPOINT(SIL_TRACE_INFO, "Write Value32 = 0x%x, Address = 0x%x\n", Value32, Address);

  Prom21XhciWriteDWord(XhciMmio, Address, Value32);
}

static void
Prom21AhciPortHotplug (
  uint32_t XhciMmio,
  uint8_t  PortNum,
  bool     Enable
  )
{
  uint32_t Address;
  uint8_t  Value;

  if (PortNum >= PROM21_NUM_SATA_PORTS){
    return;
  }

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "Port %d (%d)\n", PortNum, Enable);

  Address = 0x2E0C8 + PortNum;

  Value = Prom21XhciReadByte(XhciMmio, Address);

  if (Enable) {
    Value |= BIT_8(3);
  } else {
    Value &= ~BIT_8(3);
  }

  PROM_TRACEPOINT(SIL_TRACE_INFO, "Write Value8 = 0x%x, Address = 0x%x\n", Value, Address);

  Prom21XhciWriteByte(XhciMmio, Address, Value);
}

static void
Prom21SataConfigure (
  PROM21_DATA_BLK *PromDataBlk,
  uint32_t        XhciMmio
  )
{
  uint8_t      i;
  uint8_t      SataPort = 0xFF;

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "XhciMmio 0x%08X\n", XhciMmio);

  Prom21SetSataMode (PromDataBlk, XhciMmio);

  for (uint8_t Port = 0; Port < PROM21_NUM_SATA_PORTS; Port++) {
    if (PromDataBlk->PT21SataPortEnable[Port] != 0xf) {
      Prom21SataPortCtrl(XhciMmio, !!PromDataBlk->PT21SataPortEnable[Port], Port);
      if (PromDataBlk->PT21SataPortEnable[Port] == 1) {
        PROM_TRACEPOINT(SIL_TRACE_INFO, "PT21SataPort%uEnable = 1\n", Port);
      } else {
        SataPort &= ~BIT_8(Port);
      }

      if (PromDataBlk->PT21SataAggressiveDevSlp[Port] != 0xf) {
        Prom21SetAhciDevslp(XhciMmio, PromDataBlk->PT21SataAggressiveDevSlp[Port], Port);
      }
    }
  }

  if (PromDataBlk->PT21SataAggrLinkPmCap != 0xf) {
    Prom21SetAhciCapReg(XhciMmio, BIT_32(26), PromDataBlk->PT21SataAggrLinkPmCap);
  }

  if (PromDataBlk->PT21SataPscCap != 0xf) {
    Prom21SetAhciCapReg(XhciMmio, BIT_32(13), PromDataBlk->PT21SataPscCap);
  }

  if (PromDataBlk->PT21SataPTSataCCCSCap != 0xf) {
    Prom21SetAhciCapReg(XhciMmio, BIT_32(7), PromDataBlk->PT21SataPTSataCCCSCap);
  }

  if (PromDataBlk->PT21SataSscCap != 0xf) {
    Prom21SetAhciCapReg(XhciMmio, BIT_32(14), PromDataBlk->PT21SataSscCap);
  }

  if (PromDataBlk->PT21SataHotPlug != 0xf) {
    for (i = 0; i <= PROM21_NUM_SATA_PORTS; i++) {
      Prom21AhciPortHotplug(XhciMmio, i, !!PromDataBlk->PT21SataHotPlug);
    }
  }

  PROM_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

void
Prom21SataSetting (
  PROMCLASS_DATA_BLK *PromDataBlk
  )
{
  uint32_t XhciMmio = PromDataBlk->PromOutputBlk.PT21XhciMmio[0];
  uint32_t SecXhciMmio = PromDataBlk->PromOutputBlk.PT21XhciMmio[1];
  bool ProgramSecondary = (SecXhciMmio == 0x0) || (SecXhciMmio == 0xffffffff);

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "0x%08X, 0x%08X\n", XhciMmio, SecXhciMmio);

  if ((XhciMmio == 0x0) || (XhciMmio == 0xffffffff)) {
    return;
  }

  Prom21SataControllerEnable(XhciMmio, true);
  Prom21SataConfigure(&PromDataBlk->PromInputBlk.Primary, XhciMmio);

  if (ProgramSecondary) {
    Prom21SataControllerEnable(SecXhciMmio, true);
    Prom21SataConfigure(&PromDataBlk->PromInputBlk.Secondary, SecXhciMmio);
  }

  PROM_TRACEPOINT(SIL_TRACE_INFO,  "  Select CLKREQ!\n");
  Prom21ClkConfig(&PromDataBlk->PromInputBlk.Primary, XhciMmio);

  if (ProgramSecondary) {
    Prom21ClkConfig(&PromDataBlk->PromInputBlk.Secondary, SecXhciMmio);
  }

  PROM_TRACEPOINT(SIL_TRACE_INFO,  "  Force GPP Clock Output\n");
  Prom21GppClockConfig(&PromDataBlk->PromInputBlk.Primary, XhciMmio);

  if (ProgramSecondary) {
    Prom21GppClockConfig(&PromDataBlk->PromInputBlk.Secondary, SecXhciMmio);
  }

  if ((PromDataBlk->PromInputBlk.Primary.PT21SataEnable != 0xf) &&
      (PromDataBlk->PromInputBlk.Primary.PT21SataEnable == 0)) {
    Prom21SataControllerEnable(XhciMmio, false);
  }

  if (ProgramSecondary) {
    if ((PromDataBlk->PromInputBlk.Secondary.PT21SataEnable != 0xf) &&
        (PromDataBlk->PromInputBlk.Secondary.PT21SataEnable == 0)) {
      Prom21SataControllerEnable(SecXhciMmio, false);
    }
  }
}
