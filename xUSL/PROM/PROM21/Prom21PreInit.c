/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file PromPreInit.c
 *  @brief Promontory21 early initialization
 */

#include <SilCommon.h>
#include <FCH/Common/FchCommon.h>
#include <PROM/Common/PromAccess.h>
#include <PROM/Common/PromInit.h>
#include <PROM/PromClass-api.h>
#include "Prom21.h"
#include "Prom21Init.h"

#define SET_BITS(value, mask, shift) (((value) & (mask)) << (shift))

static void
Prom21XhciGenSelect (
  uint32_t    XhciMmio,
  uint8_t     GenSelect
  )
{
  uint8_t    Value;

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "Gen%d\n", GenSelect);

  Value = Prom21XhciReadByte(XhciMmio, 0x1C51C);

  Value &= (~BIT_8(0));

  switch(GenSelect) {
    case 0:
      Value |= 0x0;
      break;
    case 1:
      Value |= 0x1;
      break;
    default:
      Value |= 0x1;
      break;
  }

  PROM_TRACEPOINT(SIL_TRACE_INFO, "  Write Value = 0x%x\n", Value);

  Prom21XhciWriteDWord(XhciMmio, 0x1C520, 0x12345678);
  Prom21XhciWriteByte(XhciMmio, 0x1C51C, Value);
}

static void
Prom21XhciCtrl (
  uint32_t   XhciMmio,
  bool       Enable,
  uint32_t   PortNum,
  uint8_t    GenSel
  )
{
  uint32_t   Address;
  uint8_t    Value8;
  uint8_t    BitMask;

  Value8 = 0;
  Address = 0;

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "P%dGenx%x\n",
                  PortNum, GenSel);

  if (GenSel == 0x1) {
    BitMask = USBGen2by1_port_mapping[PortNum];

    switch(PortNum)
    {
      case Usb3Port0:
      case Usb3Port1:
      case Usb3Port2:
      case Usb3Port3:
      case Usb3Port4:
      case Usb3Port5:
        Address = 0x1C51C;
        break;
      case Usb2Port0:
      case Usb2Port1:
      case Usb2Port2:
      case Usb2Port3:
      case Usb2Port4:
      case Usb2Port5:
      case Usb2Port6:
      case Usb2Port7:
        Address = 0x1C51D;
        break;
      case Usb2Port8:
      case Usb2Port9:
      case Usb2Port10:
      case Usb2Port11:
        Address = 0x1C51E;
        break;
      default:
        break;
    }
  }else if (GenSel == 0xE) {
    BitMask = USBL4_port_mapping[PortNum];

    switch (PortNum) {
      case Usb3Port0:
      case Usb3Port1:
      case Usb3Port2:
      case Usb3Port3:
      Address = 0x1C51C;
        break;
      case Usb2Port0:
      case Usb2Port1:
      case Usb2Port2:
      case Usb2Port3:
      case Usb2Port4:
      case Usb2Port5:
      Address = 0x1C51D;
        break;
      case Usb2Port6:
      case Usb2Port7:
      case Usb2Port8:
      case Usb2Port9:
        Address = 0x1C51E;
        break;
      default:
        break;
    }
  }else {
    BitMask = USBGen2by2_port_mapping[PortNum];

    switch (PortNum) {
      case Usb3Port1:
        return;
      case Usb3Port0:
      case Usb3Port2:
      case Usb3Port3:
      case Usb3Port4:
      case Usb3Port5:
      Address = 0x1C51C;
        break;
      case Usb2Port0:
      case Usb2Port1:
      case Usb2Port2:
      case Usb2Port3:
      case Usb2Port4:
      case Usb2Port5:
      case Usb2Port6:
      case Usb2Port10:
      Address = 0x1C51D;
        break;
      case Usb2Port7:
      case Usb2Port8:
      case Usb2Port9:
      case Usb2Port11:
        Address = 0x1C51E;
        break;
      default:
        break;
    }
  }

  Value8 = Prom21XhciReadByte(XhciMmio, Address);

  PROM_TRACEPOINT(SIL_TRACE_INFO, "  Read Value = 0x%x, Address = 0x%x with BitMask %x\n",
                  Value8, Address, BitMask);

  if (Enable) {
    Value8 &= ~(uint8_t)(BitMask);
  } else {
    Value8 |= (uint8_t)(BitMask);
  }

  PROM_TRACEPOINT(SIL_TRACE_INFO, "  Write Value = 0x%x, Address = 0x%x\n",
                  Value8, Address);

  Prom21XhciWriteDWord(XhciMmio, 0x1C520, 0x12345678);
  Prom21XhciWriteByte(XhciMmio, Address, Value8);
}

void
Prom21UsbPortSetting (
  PROM21_DATA_BLK *PromDataBlk,
  uint32_t XhciId,
  uint32_t XhciMmio,
  uint8_t  BootMode
  )
{
  uint8_t    GenSelect;
  uint8_t    MaxUsb2Ports;
  uint8_t    MaxUsb3Ports;
  uint8_t    PromVariant;

  GenSelect = 0;
  PromVariant = 0;

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  switch (XhciId) {
  case PT21_XHCI_ID_L1: PromVariant = 1; break;
  case PT21_XHCI_ID_L2: PromVariant = 2; break;
  case PT21_XHCI_ID_L3: PromVariant = 3; break;
  case PT21_XHCI_ID_L4: PromVariant = 4; break;
  case PT21_XHCI_ID_L5: PromVariant = 5; break;
  case PT21_XHCI_ID_L6: PromVariant = 6; break;
  case PT21_XHCI_ID_L7: PromVariant = 7; break;
  case PT21_XHCI_ID_L8: PromVariant = 8; break;
  default: break;
  }

  if (PromVariant != 0) {
    PROM_TRACEPOINT(SIL_TRACE_INFO, "  XHCI XhciId == 0x%04X, PT21 L.%u\n", XhciId, PromVariant);
  } else {
    PROM_TRACEPOINT(SIL_TRACE_INFO, "  XHCI XhciId == 0x%04X, PT21 Unknown\n", XhciId);
    return;
  }

  if (XhciId != PT21_XHCI_ID_L4) {
    MaxUsb2Ports = PROM21_XHCI_NUM_USB2_PORTS;
    MaxUsb3Ports = PROM21_XHCI_NUM_USB3_PORTS;
    if (PromDataBlk->PT21Usb3GenSelect != 0xf) {
      GenSelect = PromDataBlk->PT21Usb3GenSelect;
      Prom21XhciGenSelect (XhciMmio, GenSelect);
    }
  } else {
    MaxUsb2Ports = PROM21L4_XHCI_NUM_USB2_PORTS;
    MaxUsb3Ports = PROM21L4_XHCI_NUM_USB3_PORTS;
    GenSelect = 0xE;
    Prom21XhciGenSelect (XhciMmio, GenSelect);
  }

  if ((PromDataBlk->PT21UsbPortLateDisable != 0x1) || (SilFchReadSleepType () == 0x3)) {
    for (uint8_t PortNum = 0; PortNum < MaxUsb3Ports; PortNum++) {
      if (PromDataBlk->PT21Usb3Port[PortNum] != 0xf) {
        Prom21XhciCtrl(XhciMmio, PromDataBlk->PT21Usb3Port[PortNum], Usb3Port0 + PortNum, GenSelect);
      }
    }
    for (uint8_t PortNum = 0; PortNum < MaxUsb2Ports; PortNum++) {
      if (PromDataBlk->PT21Usb2Port[PortNum] != 0xf) {
        Prom21XhciCtrl(XhciMmio, PromDataBlk->PT21Usb3Port[PortNum], Usb2Port0 + PortNum, GenSelect);
      }
    }
  } else {
    for (uint8_t PortNum = 0; PortNum < MaxUsb3Ports; PortNum++) {
      if (PromDataBlk->PT21Usb3Port[PortNum] != 0xf) {
        Prom21XhciCtrl(XhciMmio, 1, Usb3Port0 + PortNum, GenSelect);
      }
    }
    for (uint8_t PortNum = 0; PortNum < MaxUsb2Ports; PortNum++) {
      if (PromDataBlk->PT21Usb2Port[PortNum] != 0xf) {
        Prom21XhciCtrl(XhciMmio, 1, Usb2Port0 + PortNum, GenSelect);
      }
    }
  }

  PROM_TRACEPOINT(SIL_TRACE_INFO, "\n");
}

void
Prom21XhciGen (
  uint32_t   XhciMmio,
  uint8_t    XhciGen,
  uint8_t    PortNum
  )
{
  uint32_t Address;
  uint8_t  Data;
  uint8_t  Value;
  size_t   Count;

  Count = 5;
  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "P%d %s\n",
                  PortNum, (XhciGen == 0) ? "Gen1x1" : "Gen2x2");

  Address = r_force_superspeed[PortNum];

  Data = Prom21XhciReadByte(XhciMmio, Address);
  Data &= (~0x7);

  switch(XhciGen)
  {
    case 1:
      // Gen2x2
      Data |= 0x7;
      break;
    case 0:
      // Gen1x1
      Data |= 0x4;
      break;
    default:
      // Gen2x2
      Data |= 0x7;
      break;
  }

  do {
    Prom21XhciWriteByte(XhciMmio, Address, Data);
    Value = Prom21XhciReadByte(XhciMmio, Address);
    Count--;
  } while((Value != Data) && (Count > 0));

  Address = r_warm_rst_assert[PortNum];

  Value = Prom21XhciReadByte(XhciMmio, Address);
  Value |= BIT_8(2);

  Prom21XhciWriteByte(XhciMmio, Address, Value);

  PROM_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

void
Prom21AhciMsiCap (
  uint32_t  XhciMmio,
  bool      Enable
  )
{
  PROM_TRACEPOINT(SIL_TRACE_INFO, "%s\n", (Enable == 0) ? "Disable" : "Enable");
  Prom21XhciWriteByte(XhciMmio, 0x23C34, Enable ? 0x50 : 0x70);
}

void
Prom21AhciPortSataSpeed (
  uint32_t XhciMmio,
  uint8_t  PortNum,
  uint8_t  Speed
  )
{
  uint32_t Address;
  uint8_t  Value;

  if((PortNum >= PROM21_NUM_SATA_PORTS) ||
     (Speed < PROM21_MIN_SATA_SPEED) ||
     (Speed > PROM21_MAX_SATA_SPEED)) {
    return;
  }

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "P%dGen%d\n", PortNum, Speed);

  Address = SATA_gen_reg[PortNum];

  switch(Speed)
  {
    case 1:
      Value = 0x90;
      break;
    case 2:
      Value = 0xA0;
      break;
    case 3:
      Value = 0xC0;
      break;
  }

  PROM_TRACEPOINT(SIL_TRACE_INFO, "  Write Value = 0x%x, Address = 0x%x\n", Value, Address);

  Prom21XhciWriteByte(XhciMmio, Address, Value);
}

static void
Prom21Usb3Gen2PortTx (
  uint32_t  XhciMmio,
  uint8_t   PortNum,
  uint8_t   Gen2Tx,
  uint8_t   CpValue,
  uint8_t   Value,
  uint8_t   Enable
)
{
  uint16_t      Data;
  uint32_t     Address;

  Data = 0;
  Address = 0;
  Enable = (Enable == 0 ? 0 : 1);

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "XhciMmio 0x%08X, P%dGen2Cp%d %s: Value %x, Enable %x\n",
                  XhciMmio, PortNum, CpValue,
                  (Gen2Tx == 0) ? "TxEmphasis" :
                  (Gen2Tx == 1) ? "TxPreshoot" : "Unknown",
                  Value, Enable);

  switch(CpValue)
  {
    case Usb3_Cp0_Cp9:
      Address = USB3_gen2_cp0_cp9_emphasis_preshoot[PortNum];
      break;
    case Usb3_Cp13:
      Address = USB3_gen2_cp13_emphasis_preshoot[PortNum];
      break;
    case Usb3_Cp14:
      Address = USB3_gen2_cp14_emphasis_preshoot[PortNum];
      break;
    case Usb3_Cp15:
      Address = USB3_gen2_cp15_emphasis_preshoot[PortNum];
      break;
    case Usb3_Cp16:
      Address = USB3_gen2_cp16_emphasis_preshoot[PortNum];
      break;
    default:
      break;
  }

  Data = Prom21XhciReadWord (XhciMmio, Address);

  switch(Gen2Tx)
  {
    case TxEmphasis:
      Data &= ~(0x1F0);
      Data |= SET_BITS (Value, 0xf, 4);
      Data |= (Enable << 8);
      break;
    case TxPreshoot:
      Data &= ~(0xF);
      Data |= SET_BITS (Value, 0x7, 0);
      Data |= (Enable << 3);
      break;
    default:
      break;
  }

  PROM_TRACEPOINT(SIL_TRACE_INFO, "  Write Data %x to Address %x\n", Data, Address);

  Prom21XhciWriteWord (XhciMmio, Address, Data);
}

static void
Prom21Usb3Gen1PortTx (
  uint32_t  XhciMmio,
  uint8_t   PortNum,
  uint8_t   Gen1Tx,
  uint8_t   Value,
  uint8_t   Enable
  )
{
  uint16_t     Data;
  uint32_t     Address;

  Data = 0;
  Address = 0;
  Enable = (Enable == 0 ? 0 : 1);

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "XhciMmio 0x%08X, P%dGen1 %s: Value 0x%x, Enable %x\n",
                  XhciMmio, PortNum,
                  (Gen1Tx == 0) ? L"TxEmphasis" :
                  (Gen1Tx == 1) ? L"TxPreshoot" :
                  (Gen1Tx == 2) ? L"TxSquelch" : L"Unknown",
                  Value, Enable);

  Address = USB3_gen1_emphasis_preshoot [PortNum];
  Data = Prom21XhciReadWord(XhciMmio, Address);

  switch(Gen1Tx)
  {
    case TxEmphasis:
      Data &= ~(0x1F0);
      Data |= SET_BITS (Value, 0xf, 4);
      Data |= (Enable << 8);
      break;
    case TxPreshoot:
      Data &= ~(0xF);
      Data |= SET_BITS (Value, 0x7, 0);
      Data |= (Enable << 3);
      break;
    case TxSquelch:
      Address = USB3_gen1_squelch [PortNum];
      Data = Prom21XhciReadWord(XhciMmio, Address);
      Data &= ~(0xC0);
      Data |= SET_BITS(Value, 0x3, 6);
      break;
    default:
      break;
  }

  PROM_TRACEPOINT(SIL_TRACE_INFO, "  Write Data %x to Address %x\n", Data, Address);

  Prom21XhciWriteWord(XhciMmio, Address, Data);
}

static void
Prom21Usb3PortTxSwing (
  uint32_t  XhciMmio,
  uint8_t   PortNum,
  uint8_t   GenNum,
  uint8_t   Value
  )
{
  uint8_t      Data;
  uint32_t     Address;

  Data = 0;
  Address = 0;

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "XhciMmio 0x%08X, P%dGen%d TxSwing: Value %x\n",
                  XhciMmio, PortNum, GenNum, Value);

  switch(GenNum)
  {
    case 0x1:
      Address = USB3_gen1_swing[PortNum];
      break;
    case 0x2:
      Address = USB3_gen2_swing[PortNum];
      break;
    default:
      break;
  }

  Data = Prom21XhciReadByte(XhciMmio, Address);
  Data &= ~(0xF0);
  Data |= SET_BITS(Value, 0xf, 4);

  PROM_TRACEPOINT(SIL_TRACE_INFO, "  Write Data %x to Address %x\n", Data, Address);

  Prom21XhciWriteByte(XhciMmio, Address, Data);
}

static void
Prom21USB3SIConfig (
  PROM21_DATA_BLK *PromDataBlk,
  uint32_t        XhciMmio
)
{
  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "XhciMmio 0x%08X\n", XhciMmio);

  for (uint8_t Port = 0; Port < PROM21_XHCI_NUM_USB3_PORTS; Port++) {
    if (PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen1Swing < 0x10) {
      Prom21Usb3PortTxSwing(XhciMmio,
        0, 0x1,
        PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen1Swing
        );
    }
    if (PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen1EmpLevel < 0x10) {
      Prom21Usb3Gen1PortTx(XhciMmio,
        0, TxEmphasis,
        PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen1EmpLevel,
        PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen1EmpLevelEn
        );
    }
    if (PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen1Preshoot < 0x8) {
      Prom21Usb3Gen1PortTx(XhciMmio,
        0, TxPreshoot,
        PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen1Preshoot,
        PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen1PreshootEn
        );
    }
    if (PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen2Swing < 0x10) {
      Prom21Usb3PortTxSwing(XhciMmio,
        0,
        0x2,
        PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen2Swing
        );
    }
    if (PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen2Cp0EmpLevel < 0x10) {
      Prom21Usb3Gen2PortTx(XhciMmio,
        0, TxEmphasis, Usb3_Cp0_Cp9,
        PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen2Cp0EmpLevel,
        PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen2Cp0EmpLevelEn
        );
    }
    if (PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen2Cp0Preshoot < 0x8) {
      Prom21Usb3Gen2PortTx(XhciMmio,
        0, TxPreshoot, Usb3_Cp0_Cp9,
        PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen2Cp0Preshoot,
        PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen2Cp0PreshootEn
        );
    }
    if (PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen2Cp13EmpLevel < 0x10) {
      Prom21Usb3Gen2PortTx(XhciMmio,
        0, TxEmphasis, Usb3_Cp13,
        PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen2Cp13EmpLevel,
        PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen2Cp13EmpLevelEn
        );
    }
    if (PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen2Cp13Preshoot < 0x8) {
      Prom21Usb3Gen2PortTx(XhciMmio,
        0, TxPreshoot, Usb3_Cp13,
        PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen2Cp13Preshoot,
        PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen2Cp13PreshootEn
        );
    }
    if (PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen2Cp14EmpLevel < 0x10) {
      Prom21Usb3Gen2PortTx(XhciMmio,
        0, TxEmphasis, Usb3_Cp14,
        PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen2Cp14EmpLevel,
        PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen2Cp14EmpLevelEn
        );
    }
    if (PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen2Cp14Preshoot < 0x8) {
      Prom21Usb3Gen2PortTx(XhciMmio,
        0, TxPreshoot, Usb3_Cp14,
        PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen2Cp14Preshoot,
        PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen2Cp14PreshootEn
        );
    }
    if (PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen2Cp15EmpLevel < 0x10) {
      Prom21Usb3Gen2PortTx(XhciMmio,
        0, TxEmphasis, Usb3_Cp15,
        PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen2Cp15EmpLevel,
        PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen2Cp15EmpLevelEn
        );
    }
    if (PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen2Cp15Preshoot < 0x8) {
      Prom21Usb3Gen2PortTx(XhciMmio,
        0, TxPreshoot, Usb3_Cp15,
        PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen2Cp15Preshoot,
        PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen2Cp15PreshootEn
        );
    }
    if (PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen2Cp16EmpLevel < 0x10) {
      Prom21Usb3Gen2PortTx(XhciMmio,
        0, TxEmphasis, Usb3_Cp16,
        PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen2Cp16EmpLevel,
        PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen2Cp16EmpLevelEn
        );
    }
    if (PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen2Cp16Preshoot < 0x8) {
      Prom21Usb3Gen2PortTx(XhciMmio,
        0, TxPreshoot, Usb3_Cp16,
        PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen2Cp16Preshoot,
        PromDataBlk->PT21USB3Phy[Port].PT21USB3PortGen2Cp16PreshootEn
        );
    }
  }
}

static void
Prom21SIUSB2Tx (
  uint32_t    XhciMmio,
  uint8_t     PortNum,
  uint8_t     TxSetting,
  uint16_t    Value
)
{
  uint8_t      Data;
  uint16_t     Data16;
  uint32_t     Address;

  Data = 0;
  Data16 = 0;
  Address = 0;

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "XhciMmio 0x%08X, Port%x-TxSet%d, Value %x\n",
                  XhciMmio, PortNum, TxSetting, Value);

  Address = USB2_tx_reg[PortNum];

  Data = Prom21XhciReadByte(XhciMmio, Address);
  Data16 = Prom21XhciReadWord(XhciMmio, Address);

  if ((PortNum % 2) == 0) { // Port 0/2/4/6/8/10
    switch (TxSetting)
    {
      case 0x0:
        Data &= 0xFC;
        break;
      case 0x1:
        Data &= 0xE3;
        Value = (Value << 2);
        break;
      case 0x2:
        Data &= 0x1F;
        Value = (Value << 5);
        break;
      default:
        break;
    }
    PROM_TRACEPOINT(SIL_TRACE_INFO, "Write Data %x, Value %x\n", Data, Value);
    Data |= (uint8_t)Value;
    Prom21XhciWriteByte(XhciMmio, Address, Data);
  }else {
    switch (TxSetting)
    {
      case 0x0:
        Data &= 0xCF;
        Data |= (Value << 4);
        PROM_TRACEPOINT(SIL_TRACE_INFO, "Write Data %x, Value %x\n", Data, Value);
        Prom21XhciWriteByte(XhciMmio, Address, Data);
        break;
      case 0x1:
        Data16 &= 0xFE3F;
        Value = (Value << 6);
        Data16 |= Value;
        PROM_TRACEPOINT(SIL_TRACE_INFO, "Write Data16 %x, Value %x\n", Data16, Value);
        Prom21XhciWriteWord(XhciMmio, Address, Data16);
        break;
      case 0x2:
        Data16 &= 0xF1FF;
        Value = (Value << 9);
        Data16 |= Value;
        PROM_TRACEPOINT(SIL_TRACE_INFO, "Write Data16 %x, Value %x\n", Data16, Value);
        Prom21XhciWriteWord(XhciMmio, Address, Data16);
        break;
      default:
        break;
    }
  }
}

static void
Prom21SIUSB2Config (
  PROM21_DATA_BLK *PromDataBlk,
  uint32_t   XhciMmio
)
{
  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "XhciMmio 0x%08X\n", XhciMmio);

  /* 0/2, 1/3, 4/6, 5/7, 8/10, 9/11 */
  uint8_t Usb2Ports[] = { 0, 1, 4, 5, 8, 9 };

  for (uint8_t Port = 0; Port < PROM21_XHCI_NUM_USB2_PORTS / 2; Port++) {
    if (PromDataBlk->PT21USB2Phy[Port].PT21USB2SlewRate != BIT_8(2)) {
      Prom21SIUSB2Tx(XhciMmio,
        Usb2Ports[Port],
        0x0,
        PromDataBlk->PT21USB2Phy[Port].PT21USB2SlewRate
        );
    }
    if (PromDataBlk->PT21USB2Phy[Port].PT21USB2DrivingCurrent != BIT_8(3)) {
      Prom21SIUSB2Tx(XhciMmio,
        Usb2Ports[Port],
        0x1,
        PromDataBlk->PT21USB2Phy[Port].PT21USB2DrivingCurrent
        );
    }
    if (PromDataBlk->PT21USB2Phy[Port].PT21USB2Termination != BIT_8(3)) {
      Prom21SIUSB2Tx(XhciMmio,
        Usb2Ports[Port],
        0x2,
        PromDataBlk->PT21USB2Phy[Port].PT21USB2Termination
        );
    }
  }
}

static void
Prom21SataPortTxEmp (
  uint32_t  XhciMmio,
  uint8_t   PortNum,
  uint8_t   GenNum,
  uint8_t   Value
  )
{
  uint8_t      Data;
  uint32_t     Address;

  Data = 0;
  Address = 0;

  PROM_TRACEPOINT(SIL_TRACE_INFO, "XhciMmio 0x%08X, P%dGen%d: Value %x\n",
                  XhciMmio, PortNum, GenNum, Value);

  switch (GenNum)
  {
    case 0x1:
      Address = SATA_gen1_emphasis[PortNum];
      break;
    case 0x2:
      Address = SATA_gen2_emphasis[PortNum];
      break;
    case 0x3:
      Address = SATA_gen3_emphasis[PortNum];
      break;
    default:
      break;
  }

  Data = Prom21XhciReadByte(XhciMmio, Address);
  Data &= ~(0x1F);
  Data |= SET_BITS(Value, 0x1f, 0);
  Data |= BIT_8(7);

  PROM_TRACEPOINT(SIL_TRACE_INFO, "Write Data %x to Address %x\n", Data, Address);

  Prom21XhciWriteByte(XhciMmio, Address, Data);
}

static void
Prom21SataPortTxSwing (
  uint32_t   XhciMmio,
  uint8_t    PortNum,
  uint8_t    GenNum,
  uint8_t    Value
  )
{
  uint8_t      Data;
  uint32_t     Address;

  Data = 0;
  Address = 0;

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "XhciMmio 0x%08X, P%dGen%d: Value %x\n",
                  XhciMmio, PortNum, GenNum, Value);

  switch (GenNum)
  {
    case 0x1:
      Address = SATA_gen1gen2_swing[PortNum];
      Data = Prom21XhciReadByte(XhciMmio, Address);
      Data &= ~(0xF);
      Data |= SET_BITS(Value, 0xf, 0);
      break;
    case 0x2:
      Address = SATA_gen1gen2_swing[PortNum];
      Data = Prom21XhciReadByte(XhciMmio, Address);
      Data &= ~(0xF0);
      Data |= SET_BITS(Value, 0xf, 4);
      break;
    case 0x3:
      Address = SATA_gen3_swing[PortNum];
      Data = Prom21XhciReadByte(XhciMmio, Address);
      Data &= ~(0xF);
      Data |= SET_BITS(Value, 0xf, 0);
      break;
    default:
      break;
  }

  PROM_TRACEPOINT(SIL_TRACE_INFO, "Write Data %x to Address %x\n", Data, Address);

  Prom21XhciWriteByte(XhciMmio, Address, Data);
}

static void
Prom21SISATAConfig (
  PROM21_DATA_BLK *PromDataBlk,
  uint32_t        XhciMmio
  )
{
  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "XhciMmio 0x%08X\n", XhciMmio);

  for (uint8_t Port = 0; Port < PROM21_NUM_SATA_PORTS; Port++) {
    if (PromDataBlk->PT21SataPhy[Port].PT21SataPortGen1Swing != BIT_8(4)) {
      Prom21SataPortTxSwing(XhciMmio, Port, 0x1, PromDataBlk->PT21SataPhy[Port].PT21SataPortGen1Swing);
    }
    if (PromDataBlk->PT21SataPhy[Port].PT21SataPortGen1EmpLevel != BIT_8(5)) {
      Prom21SataPortTxEmp(XhciMmio, Port, 0x1, PromDataBlk->PT21SataPhy[Port].PT21SataPortGen1EmpLevel);
    }
    if (PromDataBlk->PT21SataPhy[Port].PT21SataPortGen2Swing != BIT_8(4)) {
      Prom21SataPortTxSwing(XhciMmio, Port, 0x2, PromDataBlk->PT21SataPhy[Port].PT21SataPortGen2Swing);
    }
    if (PromDataBlk->PT21SataPhy[Port].PT21SataPortGen2EmpLevel != BIT_8(5)) {
      Prom21SataPortTxEmp(XhciMmio, Port, 0x2, PromDataBlk->PT21SataPhy[Port].PT21SataPortGen2EmpLevel);
    }
    if (PromDataBlk->PT21SataPhy[Port].PT21SataPortGen3Swing != BIT_8(4)) {
      Prom21SataPortTxSwing(XhciMmio, Port, 0x3, PromDataBlk->PT21SataPhy[Port].PT21SataPortGen3Swing);
    }
    if (PromDataBlk->PT21SataPhy[Port].PT21SataPortGen3EmpLevel != BIT_8(5)) {
      Prom21SataPortTxEmp(XhciMmio, Port, 0x3, PromDataBlk->PT21SataPhy[Port].PT21SataPortGen3EmpLevel);
    }
  }
}


void
Prom21SIConfig (
  PROM21_DATA_BLK *PromDataBlk,
  uint32_t        XhciMmio
  )
{
  uint8_t Data;

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "XhciMmio 0x%08X\n",  XhciMmio);

  Prom21SISATAConfig(PromDataBlk, XhciMmio);
  Prom21USB3SIConfig(PromDataBlk, XhciMmio);
  Prom21SIUSB2Config(PromDataBlk, XhciMmio);

  Data = Prom21XhciReadByte(XhciMmio, 0x1C51F);
  Data |= BIT_8(7);
  Prom21XhciWriteByte(XhciMmio, 0x1C51F, Data);

  PROM_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}


