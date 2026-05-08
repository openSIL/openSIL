/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file Prom21LoadFw.c
 *  @brief Promontory21 firmware loading functions
 */

#include <SilCommon.h>
#include <CommonLib/Mmio.h>
#include <CommonLib/SmnAccess.h>
#include <PciLib.h>
#include <FCH/Common/FchCommon.h>
#include <Mpio/Common/MpioCmn2Rev.h>
#include <PROM/Common/PromAccess.h>
#include <PROM/Common/PromInit.h>
#include <PROM/PromClass-api.h>
#include "Prom21.h"
#include "Prom21Init.h"

static void
Prom21DspLaneReversal (
  uint32_t   XhciMmio,
  uint8_t    Enable,
  uint8_t    PortNum
  )
{
  uint32_t    Address;
  uint8_t     Value;

  if(PortNum > PROM21_MAX_LANE_REVERSAL_DSP_NUMBER) {
    return;
  }

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Address = PORM21_LANE_REVERSAL_REG(PortNum);

  Value = Prom21XhciReadByte(XhciMmio, Address);

  switch (Enable) {
    case 0:
      Value |= BIT_8(0);
      break;
    case 1:
      Value &= (~BIT_8(0));
      break;
    default:
      Value |= BIT_8(0);
      break;
  }

  Prom21XhciWriteByte(XhciMmio, Address, Value);
}


static void
Prom21DspLaneConfig (
  PROM21_DATA_BLK *PromDataBlk,
  uint32_t        XhciMmio
  )
{
  for (uint8_t PortNum = 0; PortNum < PROM21_NUM_PCIE_LANES; PortNum += 2) {
    if(PromDataBlk->PT21PciePortLaneRev[PortNum / 2] != 0xf) {
      Prom21DspLaneReversal(XhciMmio,
        PromDataBlk->PT21PciePortLaneRev[PortNum / 2],
        PortNum);
    }
  }
}

static uint32_t
FwChecksum (
  uint8_t     *PtFwAddress,
  uint32_t    PtFwSize
  )
{
  uint32_t  Counter;
  uint32_t  Checksum = 0;

  for (Counter = 0; Counter < (PtFwSize - 0x4C); Counter++) {
    Checksum += (uint32_t) *(PtFwAddress + 0x0C + Counter);
  }

  return Checksum;
}

static SIL_STATUS
Prom21LoadFwDoneNew (
  PROMCLASS_DATA_BLK *PromDataBlk,
  uint32_t           XhciMmio
  )
{
  SIL_STATUS      Status;
  uint32_t        Loop;
  uint8_t         Value;
  uint8_t         WriteValue;

  Status  = SilDeviceError;
  Loop    = 0;

  do {
    Value = Prom21XhciReadByteV2(PromDataBlk, XhciMmio, 0x1E51C);
    if (Value & BIT_8(6)) {
      PROM_TRACEPOINT(SIL_TRACE_INFO, "  Prom21LoadFwDoneNew Ready (%x) @loop %d takes %d(ms)\n",
                      Value, Loop, (Loop * 2));
      do {
        Value &= ~BIT_8(6);
        WriteValue = Value;
        Prom21XhciWriteByte(XhciMmio, 0x1E51C, WriteValue);
        Value = Prom21XhciReadByteV2(PromDataBlk, XhciMmio, 0x1E51C);
      } while(Value != WriteValue);
      Status = SilPass;
      break;
    }
    SilFchStall(2 * 1000);
    Loop++;
  } while (Loop < 500);

  return Status;
}

static SIL_STATUS
Prom21CheckLoadFwStatus (
  PROMCLASS_DATA_BLK *PromDataBlk,
  uint32_t           XhciMmio
  )
{
  SIL_STATUS     Status;
  uint8_t        FwVersion;
  uint32_t       Loop;
  uint32_t       Address;
  uint8_t        Value;
  uint8_t        BitMask;

  Status    = SilDeviceError;
  FwVersion = Prom21XhciReadByteV2(PromDataBlk, XhciMmio, 0x1847A);

  Loop     = 0;
  Address  = 0x28025;
  BitMask  = BIT_8(7);

  PROM_TRACEPOINT(SIL_TRACE_INFO, "Fw version: Major Version Year = 0x%x\n", FwVersion);

  if (FwVersion >= 0x24) {
    Status = Prom21LoadFwDoneNew(PromDataBlk, XhciMmio);
  } else {
    do {
      Value = Prom21XhciReadByteV2(PromDataBlk, XhciMmio, Address);
      if (Value & BitMask) {
        PROM_TRACEPOINT(SIL_TRACE_INFO, "  Prom21CheckLoadFwStatus Ready (%x) @loop %d takes %d(ms)\n",
                        Value, Loop, (Loop * 2));
        Status = SilPass;
        break;
      }
      SilFchStall(2 * 1000);
      Loop++;
    } while (Loop < 200);
  }

  return Status;
}

static uint64_t
GetProm21FwVersion (
  PROMCLASS_DATA_BLK *PromDataBlk,
  uint32_t           XhciMmio
  )
{
  uint64_t   PtFwVersionRam;

  PtFwVersionRam = ((uint64_t) Prom21XhciReadByteV2(PromDataBlk, XhciMmio, 0x1847F));
  PtFwVersionRam |= ((uint64_t) Prom21XhciReadByteV2(PromDataBlk, XhciMmio, 0x1847E)) << 8;
  PtFwVersionRam |= ((uint64_t) Prom21XhciReadByteV2(PromDataBlk, XhciMmio, 0x1847D)) << 16;
  PtFwVersionRam |= ((uint64_t) Prom21XhciReadByteV2(PromDataBlk, XhciMmio, 0x1847C)) << 24;
  PtFwVersionRam |= ((uint64_t) Prom21XhciReadByteV2(PromDataBlk, XhciMmio, 0x1847B)) << 32;
  PtFwVersionRam |= ((uint64_t) Prom21XhciReadByteV2(PromDataBlk, XhciMmio, 0x1847A)) << 40;

  PROM_TRACEPOINT(SIL_TRACE_INFO, "  GetProm21FwVersion = 0x%016LX\n", PtFwVersionRam);

  return PtFwVersionRam;
}

static bool
Prom21FwVersionMatch (
  PROMCLASS_DATA_BLK *PromDataBlk,
  uint32_t           XhciMmio
  )
{
  uint64_t  FwVersion;

  FwVersion = GetProm21FwVersion(PromDataBlk, XhciMmio);

  if (FwVersion == PromDataBlk->PromOutputBlk.PT21FwVersion) {
    PROM_TRACEPOINT(SIL_TRACE_INFO, "   Prom21 fw is the same\n");
    return true;
  }

  PROM_TRACEPOINT(SIL_TRACE_INFO, "   Prom21 fw is NOT the same\n");
  return false;
}

static uint8_t
Prom21RunsInRamCode (
  PROMCLASS_DATA_BLK *PromDataBlk,
  uint32_t           XhciMmio
  )
{
  uint8_t   CpuInRam;
  uint8_t   Value8;

  CpuInRam = 0;

  Value8 = Prom21XhciReadByteV2(PromDataBlk, XhciMmio, 0x15041);
  PROM_TRACEPOINT(SIL_TRACE_INFO, "    PT21 CPU runs in %s code\n",
                  (Value8 & BIT_8(0)) ? "RAM" : "ROM");

  if (Value8 & BIT_8(0)) {
    CpuInRam = 1;
  }

  return CpuInRam;
}

static void
Prom21SpecificFeatures (
  PROM21_DATA_BLK *PromDataBlk,
  uint32_t XhciId,
  uint32_t XhciMmio,
  uint8_t  PtCpuMode,
  uint8_t  BootMode,
  bool     IsPrimary
  )
{
  uint8_t       Data8;
  size_t        Loop;
  bool          NeedUpdate;

  Loop       = 0;
  NeedUpdate = false;

  if (PtCpuMode == 1) { // in RAM code
    if ((PromDataBlk->PT21SIProgEnable == 0x1) ||
        (PromDataBlk->PT21SataPortMdPort[0] != 0xf) ||
        (PromDataBlk->PT21SataPortMdPort[1] != 0xf) ||
        (PromDataBlk->PT21SataPortMdPort[2] != 0xf) ||
        (PromDataBlk->PT21SataPortMdPort[3] != 0xf) ||
        (PromDataBlk->PT21AhciMsiCap != 0xf) ||
        (PromDataBlk->PT21XhciPortGen[0] != 0xf) || (PromDataBlk->PT21XhciPortGen[1] != 0xf) ||
        (PromDataBlk->PT21XhciPortGen[2] != 0xf) || (PromDataBlk->PT21XhciPortGen[3] != 0xf) ||
        (PromDataBlk->PT21XhciPortGen[4] != 0xf) || (PromDataBlk->PT21XhciPortGen[5] != 0xf) ||
        (PromDataBlk->PT21Msi != 0xf) ||
        (PromDataBlk->PT21Msix != 0xf) ||
        (PromDataBlk->PT21Usb3GenSelect != 0xf) ||
        (PromDataBlk->PT21UsbPortLateDisable == 0x1) ||
        (PromDataBlk->PT21Usb3Port[0] != 0xf)  || (PromDataBlk->PT21Usb3Port[1] != 0xf) ||
        (PromDataBlk->PT21Usb3Port[2] != 0xf)  || (PromDataBlk->PT21Usb3Port[3] != 0xf) ||
        (PromDataBlk->PT21Usb3Port[4] != 0xf)  || (PromDataBlk->PT21Usb3Port[5] != 0xf) ||
        (PromDataBlk->PT21Usb2Port[0] != 0xf)  || (PromDataBlk->PT21Usb2Port[1] != 0xf) ||
        (PromDataBlk->PT21Usb2Port[2] != 0xf)  || (PromDataBlk->PT21Usb2Port[3] != 0xf) ||
        (PromDataBlk->PT21Usb2Port[4] != 0xf)  || (PromDataBlk->PT21Usb2Port[5] != 0xf) ||
        (PromDataBlk->PT21Usb2Port[6] != 0xf)  || (PromDataBlk->PT21Usb2Port[7] != 0xf) ||
        (PromDataBlk->PT21Usb2Port[8] != 0xf)  || (PromDataBlk->PT21Usb2Port[9] != 0xf) ||
        (PromDataBlk->PT21Usb2Port[10] != 0xf) || (PromDataBlk->PT21Usb2Port[11] != 0xf)) {
      do {
        Data8 = Prom21XhciReadByte(XhciMmio, 0x1E51D);
        if (!(Data8 & BIT_8(3))) {
          NeedUpdate = true;
          break;
        }
        Loop ++;
        SilFchStall(100 * 1000);
      } while (Loop <= 100);
    }
  }

  if (PromDataBlk->PT21SIProgEnable == 0x1) {
    Prom21SIConfig(PromDataBlk, XhciMmio);
  }

  if (IsPrimary) {
    if (PromDataBlk->PT21GpioPerstEnable == 0x1) {
      Data8 = Prom21XhciReadByte(XhciMmio, 0x1C51F);
      Data8 |= BIT_8(4);
      Prom21XhciWriteByte(XhciMmio, 0x1C51F, Data8);
    }

    if (PromDataBlk->PT21LtrSmallEnable == 0x1) {
      Prom21XhciWriteDWord (XhciMmio, 0x1C520, 0x12345678);
      Data8 = Prom21XhciReadByte(XhciMmio, 0x1C51F);
      Data8 |= BIT_8(3);
      Prom21XhciWriteByte(XhciMmio, 0x1C51F, Data8);
    }

    if (PromDataBlk->PT21EqPreset != 0xf) {
      if(PromDataBlk->PT21EqPreset <= 0x3) {
        Data8 = Prom21XhciReadByte(XhciMmio, 0x1E51C);
        Data8 &= (~0x3);
        Data8 |= PromDataBlk->PT21EqPreset;
        Prom21XhciWriteByte(XhciMmio, 0x1E51C, Data8);
      }
    }
  }

  for (uint8_t PortNum = 0; PortNum < PROM21_NUM_SATA_PORTS; PortNum++) {
    if (PromDataBlk->PT21SataPortMdPort[PortNum] != 0xf) {
      Prom21AhciPortSataSpeed (XhciMmio, PortNum, PromDataBlk->PT21SataPortMdPort[PortNum]);
    }
  }

  if (PromDataBlk->PT21AhciMsiCap != 0xf) {
    Prom21AhciMsiCap(XhciMmio, PromDataBlk->PT21AhciMsiCap);
  }

  for (uint8_t PortNum = 0; PortNum < PROM21_XHCI_NUM_USB3_PORTS; PortNum++) {
    if (PromDataBlk->PT21XhciPortGen[PortNum] != 0xf) {
      Prom21XhciGen(XhciMmio, PromDataBlk->PT21XhciPortGen[PortNum], PortNum);
    }
  }

  if (PromDataBlk->PT21Msi != 0xf) {
    if (PromDataBlk->PT21Msi != 0) {
      Prom21XhciWriteByte(XhciMmio, 0x23834, 0x50);
    } else {
      Prom21XhciWriteByte(XhciMmio, 0x23834, 0x68);
    }
  }

  if (PromDataBlk->PT21Msix != 0xf) {
    if (PromDataBlk->PT21Msix != 0) {
      Prom21XhciWriteByte(XhciMmio, 0x23851, 0x68);
    } else {
      Prom21XhciWriteByte(XhciMmio, 0x23851, 0x78);
    }
  }

  Prom21UsbPortSetting(PromDataBlk, XhciId, XhciMmio, BootMode);

  if (NeedUpdate) {
    Data8 = Prom21XhciReadByte(XhciMmio, 0x1E51D);
    Data8 |= BIT_8(2);
    Prom21XhciWriteByte(XhciMmio, 0x1E51D, Data8);
  }
}

static SIL_STATUS
LpPtXhciProceedFwLoad (
  PROMCLASS_DATA_BLK *PromDataBlk,
  uint8_t            *PtFwInRamPtr,
  uint16_t           XhciBus,
  uint8_t            XhciDev,
  uint8_t            XhciFun,
  uint32_t           XhciMmio,
  bool               LoadStatusCheck
  )
{
  uint8_t            DSResult;
  uint8_t            Parameter;
  uint8_t            Data8;
  uint16_t           wPrg_Addr;
  uint32_t           Data32;
  uint32_t           UsbCmd;
  uint32_t           Address;
  uint32_t           Counter;
  uint32_t           FirstCounter;
  uint32_t           LoadCounter;
  SIL_STATUS         Status;

  if (PromDataBlk->PromInputBlk.PT21DbgLoadFw == 1) {
    PROM_TRACEPOINT(SIL_TRACE_INFO, "  Using on-board PT21 fw\n");
    return SilPass;
  }

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "PtFwInRamPtr 0x%08X, [%04X:%02X:%02X], XhciMmio 0x%08X\n",
                  PtFwInRamPtr, XhciBus, XhciDev, XhciFun, XhciMmio);


  if (PromDataBlk->PromInputBlk.PT21TogglePerst == 0) {
    Data8 = Prom21XhciReadByte(XhciMmio, 0x1E51C);
    Data8 |= BIT_8(4);
    Prom21XhciWriteByte(XhciMmio, 0x1E51C, Data8);
  }

  PROM_TRACEPOINT(SIL_TRACE_INFO, "  Program Digital Signature Parameter 1 Offset 0x%08X = 0x%X\n",
                  PtFwInRamPtr + 0x20002, *(PtFwInRamPtr + 0x20002));
  Parameter = Prom21XhciReadByteV2(PromDataBlk, XhciMmio, 0x15180);
  Parameter &= 0x0F;
  Parameter |= ((*(PtFwInRamPtr + 0x20002) & 0x3C) << 2);
  Prom21XhciWriteByte(XhciMmio, 0x15180, Parameter);

  Address = PCI_LIB_ADDRESS(XhciBus, XhciDev, XhciFun, 0xE2);
  LoadCounter = 0;
  do {
    PROM_TRACEPOINT(SIL_TRACE_INFO, "    Loading PT FW Starts LoadCounter = %d !!!\n",
                    LoadCounter);

    FirstCounter = 0;
    do {
      Parameter = Prom21XhciReadByteV2(PromDataBlk, XhciMmio, 0x15040);
      Parameter &= 0xFC;
      Parameter |= 0x02;
      Prom21XhciWriteByte(XhciMmio, 0x15040, Parameter);

      // Reset CPU
      Parameter = Prom21XhciReadByteV2(PromDataBlk, XhciMmio, 0x15042);
      Parameter &= 0xFE;
      Parameter |= 0x01;
      Prom21XhciWriteByte(XhciMmio, 0x15042, Parameter);

      Counter = 0;
      do {
        UsbCmd = xUSLMemRead32((void *)(size_t)(XhciMmio + 0x20));
        PROM_TRACEPOINT(SIL_TRACE_INFO, "    Usb Status (Before) = 0x%X Counter = %d\n",
                        UsbCmd, Counter);
        if (UsbCmd & BIT_32(1)) {
          SilFchStall(2 * 1000);       // Delay 2ms
        }
        Counter++;
      } while ((UsbCmd & BIT_32(1)) && (Counter < 100));

      Parameter = Prom21XhciReadByteV2(PromDataBlk, XhciMmio, 0x15041);
      PROM_TRACEPOINT(SIL_TRACE_INFO, "    PT CPU runs in %s code\n",
                      (Parameter & BIT_8(0)) ? "RAM" : "ROM");

      FirstCounter++;
    } while (((UsbCmd & BIT_32(1)) || (Parameter & BIT_8(0))) && (FirstCounter < 10));


    if (FirstCounter == 10) {
      Counter = 1;
      LoadCounter = 10;
    } else {
      Data32 = 0x2001F;
      Data32 += (uint32_t) ((*(PtFwInRamPtr + 0x20001) & 0x7E) >> 1);
      PROM_TRACEPOINT(SIL_TRACE_INFO, "    Set Digital Signature Parameter 2 Offset 0x%08X = 0x%X -> Address = 0x%08X\n",
                      PtFwInRamPtr + 0x20001, *(PtFwInRamPtr + 0x20001), Data32);
      for (Counter = 0; Counter < 32; Counter++) {
        Prom21XhciWriteByte(XhciMmio, 0x15140 + Counter, *(PtFwInRamPtr + Data32 - Counter));
      }

      PROM_TRACEPOINT(SIL_TRACE_INFO, "    Write the first 64K FW data to RAM\n");
      xUSLPciWrite16(Address, 0);
      while (xUSLPciRead16(Address) != 0) {}

      for (Counter = 0; Counter <= 0x3FFF ; Counter++) {
        wPrg_Addr = (uint16_t) (Counter << 1);

        Data32  = *(PtFwInRamPtr + wPrg_Addr + 0);
        Data32 |= *(PtFwInRamPtr + wPrg_Addr + 1) << 8;
        Data32 |= *(PtFwInRamPtr + wPrg_Addr + 0x8000) << 16;
        Data32 |= *(PtFwInRamPtr + wPrg_Addr + 0x8001) << 24;
        xUSLMemWrite32((void *)(size_t)(XhciMmio + 0x3010), Data32);
        while (xUSLPciRead16(Address) == wPrg_Addr) {}
      }

      PROM_TRACEPOINT(SIL_TRACE_INFO, "    Write the second 64K FW data to RAM\n");
      xUSLPciWrite16(Address, 0x8000);
      while (xUSLPciRead16(Address) != 0x8000) {}

      for (Counter = 0x4000; Counter <= 0x7FFF; Counter++) {
        wPrg_Addr = (uint16_t) (Counter << 1);

        Data32  = *(PtFwInRamPtr + wPrg_Addr + 0x08000);
        Data32 |= *(PtFwInRamPtr + wPrg_Addr + 0x08001) << 8;
        Data32 |= *(PtFwInRamPtr + wPrg_Addr + 0x10000) << 16;
        Data32 |= *(PtFwInRamPtr + wPrg_Addr + 0x10001) << 24;
        xUSLMemWrite32((void *)(size_t)(XhciMmio + 0x3010), Data32);
        while (xUSLPciRead16(Address) == wPrg_Addr) {} 
      }

      Counter = 0;
      do {
        DSResult = Prom21XhciReadByteV2(PromDataBlk, XhciMmio, 0x1A51C);
        PROM_TRACEPOINT(SIL_TRACE_INFO, "    Digital Signature Verification Result = %s\n",
                        (DSResult & BIT_8(5)) ? "Success" : "Fail");

        UsbCmd = xUSLMemRead32((void *)(size_t)(XhciMmio + 0x20));
        PROM_TRACEPOINT(SIL_TRACE_INFO, "    UsbCmd (After) = 0x%X Counter = %d\n",
                        UsbCmd, Counter);
        SilFchStall(2 * 1000);

        Parameter = Prom21XhciReadByteV2(PromDataBlk, XhciMmio, 0x15041);
        PROM_TRACEPOINT(SIL_TRACE_INFO, "    CPU runs in %s code\n",
                        (Parameter & BIT_8(0)) ? "RAM" : "ROM");

        Counter++;
      } while (((UsbCmd & BIT_32(1)) || ((Parameter & BIT_8(0)) == 0) ||
               ((DSResult & BIT_8(5)) == 0)) && (Counter < 500));
      LoadCounter++;
    }
  } while ((Counter == 10) && (LoadCounter < 10));


  if ((Counter == 10) && (LoadCounter == 10)) {
    PROM_TRACEPOINT(SIL_TRACE_INFO, "  Loading PT FW Fail !!!\n");
    Status = SilUnsupported;
  } else {
    if (PromDataBlk->PromInputBlk.PT21SecondPortNumber != 0xE) {
      if (LoadStatusCheck) {
        Status = Prom21CheckLoadFwStatus(PromDataBlk, XhciMmio);
      } else {
        SilFchStall(200 * 1000);
        Status = SilPass;
      }
    } else {
      if (PromDataBlk->PromInputBlk.PT21DelayAfterFWLoading == 0x1) {
        SilFchStall(200 * 1000);
      }
      Status = Prom21CheckLoadFwStatus (PromDataBlk, XhciMmio);
    }
    PROM_TRACEPOINT(SIL_TRACE_INFO, "  Loading PT FW Done. Status: %x !!!\n", Status);
  }

  PROM_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return Status;
}

typedef struct {
  uint32_t OperationCode;
  uint32_t AvailableMemoryAddress;
  uint32_t AvailableMemorySize;
} LOAD_FW_CONFIG_DATA;

typedef struct {
  uint32_t  TotalSize;
  uint32_t  Status;
} MBOX_BUFFER_HEADER;

typedef struct {
  MBOX_BUFFER_HEADER    Header;
  LOAD_FW_CONFIG_DATA   LoadFwConfigData;
} MBOX_LOAD_FW_CONFIG_BUFFER;

static SIL_STATUS
PspMboxBiosCmdLoadFwConfig (
  LOAD_FW_CONFIG_DATA * FwConfigBuf
  )
{
  return SilNotFound;
}

SIL_STATUS
AmdProm21FwLoad (
  SIL_CONTEXT        *SilContext,
  PROMCLASS_DATA_BLK *PromDataBlk
  )
{
  uint16_t                        GppBus;
  uint8_t                         GppDev;
  uint8_t                         GppFun;
  uint16_t                        SecGppBus;
  uint8_t                         SecGppDev;
  uint8_t                         SecGppFun;
  uint16_t                        UspBus;
  uint16_t                        DspBus;
  uint8_t                         CurrentSpeed;
  uint8_t                         SecCurrentSpeed;
  uint8_t                         XhciLock;
  uint8_t                         PtCpuMode;
  uint8_t                         PTTargetSpeed;
  uint8_t                         SecPortNum;
  uint8_t                         CapBase;
  uint8_t                         *PtFwInRamPtr;
  uint32_t                        PTFwSize;
  uint32_t                        PtFwChecksum;
  uint32_t                        Checksum;
  uint32_t                        XhciMmio;
  uint32_t                        SecXhciMmio;
  uint32_t                        GppPcieAddress;
  uint32_t                        SecGppPcieAddress;
  uint32_t                        Counter;
  uint32_t                        UsbCmd;
  uint64_t                        FwVersion;
  SIL_STATUS                      Status;
  LOAD_FW_CONFIG_DATA             FwConfigBuf;
  PROM_PCI_SAVE_RESTORE_TABLE     PtPciTable[2];
  MPIO_COMMON_2_REV_XFER_BLOCK    *MpioXferTable;
  NORTH_BRIDGE_PCIE_SIB           *NbPcieData;

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (SilGetCommon2RevXferTable(SilContext, SilId_MpioClass, (void **)(&MpioXferTable)) != SilPass) {
    return SilNotFound;
  }

  NbPcieData = (NORTH_BRIDGE_PCIE_SIB *)xUslFindStructure(SilContext,
                                          SilId_NbioClass,
                                          NBIOPCIECLASS_INSTANCE);
  if (NbPcieData == NULL) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO Pcie config not found.\n");
    return SilNotFound;
  }

  SecPortNum = 0;
  PtCpuMode  = 0;
  PtFwInRamPtr = (uint8_t *)(uintptr_t)PromDataBlk->PromInputBlk.PT21FwInRamAddress;

  {
    PROM_TRACEPOINT(SIL_TRACE_INFO, "  BootMode != BOOT_ON_S3_RESUME, copy PT FW from Rom to Ram !!!\n");

    if (PromDataBlk->PromInputBlk.PT21FWLoading == 1) {
      if (PromDataBlk->PromInputBlk.PT21FwInRamAddress == 0) {
        PROM_TRACEPOINT(SIL_TRACE_ERROR, "PT21FWLoading = 1 and PT21FwInRamAddress not provided\n");
        return SilUnsupported;
      }
      FwConfigBuf.OperationCode = 0x01;
      FwConfigBuf.AvailableMemoryAddress = PromDataBlk->PromInputBlk.PT21FwInRamAddress;
      FwConfigBuf.AvailableMemorySize = (128 + 4) * 1024;
      Status = PspMboxBiosCmdLoadFwConfig(&FwConfigBuf);
      if (Status != SilPass) {
        PROM_TRACEPOINT(SIL_TRACE_ERROR, "Get PT FW from PSP Directory FAIL !!!\n");
        return Status;
      }
    } else {
      if (PromDataBlk->PromInputBlk.PT21FwInRamAddress == 0) {
        PROM_TRACEPOINT(SIL_TRACE_ERROR, "PT21FWLoading = 0 unsupported and PT21FwInRamAddress not provided\n");
        return SilUnsupported;
      }
    }
  }

  PtFwInRamPtr = (uint8_t *)(uintptr_t)PromDataBlk->PromInputBlk.PT21FwInRamAddress;
  PTFwSize  = ((uint32_t) *(PtFwInRamPtr + 0x07)) << 24;
  PTFwSize |= ((uint32_t) *(PtFwInRamPtr + 0x06)) << 16;
  PTFwSize |= ((uint32_t) *(PtFwInRamPtr + 0x05)) << 8;
  PTFwSize |= ((uint32_t) *(PtFwInRamPtr + 0x04)) << 0;

  PtFwChecksum  = ((uint32_t) *(PtFwInRamPtr + 0x0B)) << 24;
  PtFwChecksum |= ((uint32_t) *(PtFwInRamPtr + 0x0A)) << 16;
  PtFwChecksum |= ((uint32_t) *(PtFwInRamPtr + 0x09)) << 8;
  PtFwChecksum |= ((uint32_t) *(PtFwInRamPtr + 0x08)) << 0;

  Checksum = FwChecksum(PtFwInRamPtr, PTFwSize);

  PROM_TRACEPOINT(SIL_TRACE_INFO, "  PT FW Ram Address    = 0x%08X\n", PtFwInRamPtr);
  PROM_TRACEPOINT(SIL_TRACE_INFO, "  PT FW Size           = 0x%08X\n", PTFwSize);
  PROM_TRACEPOINT(SIL_TRACE_INFO, "  PT FW Checksum       = 0x%08X\n", PtFwChecksum);
  PROM_TRACEPOINT(SIL_TRACE_INFO, "  Checksum Calculation = 0x%08X\n", Checksum);

  if (PtFwChecksum != Checksum) {
    PROM_TRACEPOINT(SIL_TRACE_ERROR, "ERROR: PT FW Checksum ERROR !!!\n");
    return SilUnsupported;
  }

  FwVersion  = ((uint64_t)*(PtFwInRamPtr + 0x8C)) << 40;
  FwVersion |= ((uint64_t)*(PtFwInRamPtr + 0x8D)) << 32;
  FwVersion |= ((uint64_t)*(PtFwInRamPtr + 0x8E)) << 24;
  FwVersion |= ((uint64_t)*(PtFwInRamPtr + 0x8F)) << 16;
  FwVersion |= ((uint64_t)*(PtFwInRamPtr + 0x90)) << 8;
  FwVersion |= ((uint64_t)*(PtFwInRamPtr + 0x91)) << 0;
  PromDataBlk->PromOutputBlk.PT21FwVersion = FwVersion;
  PROM_TRACEPOINT(SIL_TRACE_INFO, "  Promontory FwVersion = 0x%016LX ()\n", FwVersion);

  GppPcieAddress = PromDataBlk->PromOutputBlk.PT21GppPcieAddress[0];
  GppBus = (uint16_t) ((GppPcieAddress >> 20) & 0xFFF);
  GppDev = (uint8_t) ((GppPcieAddress >> 15) & 0x1F);
  GppFun = (uint8_t) ((GppPcieAddress >> 12) & 0x07);

  XhciMmio = PromDataBlk->PromOutputBlk.PT21XhciMmio[0];
  if ((XhciMmio == 0x0) || (XhciMmio == 0xffffffff)) {
    return SilNotFound;
  }

  SaveInitPromBus(PromDataBlk, PtPciTable, 0x00BB);

  PROM_TRACEPOINT(SIL_TRACE_INFO, "  Check USB status before read PT\n");
  Counter = 0;
  do {
    UsbCmd = xUSLMemRead32((void *)(size_t)(XhciMmio + 0x20));
    PROM_TRACEPOINT(SIL_TRACE_INFO, "    UsbCmd = 0x%X Counter = %d\n", UsbCmd, Counter);
    if (UsbCmd & BIT_32(1)) {
      SilFchStall(2 * 1000);
    }
    Counter++;
  } while ((UsbCmd & BIT_32(1)) && (Counter < 100));

  XhciLock = Prom21XhciReadByteV2(PromDataBlk, XhciMmio, 0x15087);
  if ((XhciLock & BIT_8(4)) == 0) {
    PromDataBlk->PromOutputBlk.PT21XhciLock[0] = 1;
    PROM_TRACEPOINT(SIL_TRACE_INFO, "    Set PcdPT21XhciLock = %d\n",
                    PromDataBlk->PromOutputBlk.PT21XhciLock[0]);
  }

  if (XhciLock & BIT_8(4)) {
    PROM_TRACEPOINT(SIL_TRACE_INFO, "  XHCI is unlocked\n");

    CurrentSpeed = xUSLPciRead8(GppPcieAddress + 0x6A);
    PROM_TRACEPOINT(SIL_TRACE_INFO, "  Before set PT speed GEN1: current speed x%d Gen%d\n",
                    CurrentSpeed >> 4, CurrentSpeed & 0xF);
    if ((CurrentSpeed & 0x0F) != PcieGen1) {
      MpioXferTable->MpioPcieSetSpeed(SilContext,
        &NbPcieData->PciePlatformConfig,
        GppDev,
        GppFun,
        PcieGen1);
      do {
          CurrentSpeed = xUSLPciRead8(GppPcieAddress + 0x6B);
        } while ((CurrentSpeed & (BIT_8(5) + BIT_8(3))) != BIT_8(5));

      CurrentSpeed = xUSLPciRead8(GppPcieAddress + 0x6A);
      PROM_TRACEPOINT(SIL_TRACE_INFO, "  Set PT speed to x%d Gen%d\n",
                      CurrentSpeed >> 4, CurrentSpeed & 0xF);
    } else {
      PROM_TRACEPOINT(SIL_TRACE_INFO, "  PT speed is Gen1 already\n");
    }

    PtCpuMode = Prom21RunsInRamCode(PromDataBlk, XhciMmio);
    Prom21DspLaneConfig(&PromDataBlk->PromInputBlk.Primary, XhciMmio);
    Prom21SpecificFeatures(&PromDataBlk->PromInputBlk.Primary,
      PromDataBlk->PromOutputBlk.PT21XhciID[0],
      XhciMmio,
      PtCpuMode,
      PromDataBlk->PromInputBlk.BootMode,
      true
      );


    if ((PtCpuMode == 0) || !Prom21FwVersionMatch(PromDataBlk, XhciMmio)) {
      Status = LpPtXhciProceedFwLoad(PromDataBlk,
                 PtFwInRamPtr + 0x0C,
                 PtPciTable[0].PromBus + 2, 0, 0,
                 XhciMmio,
                 false
                 );
      if (Status == SilPass) {
        PROM_TRACEPOINT(SIL_TRACE_INFO, "  PT load FW success\n");
      }
    } else {
      PROM_TRACEPOINT(SIL_TRACE_INFO, "  PT Runs in RAM code, not load fw\n");
    }
  } else {
    PROM_TRACEPOINT(SIL_TRACE_INFO, "  XHCI is lock: bypass PT code !!!\n");
  }

  CapBase = xUSLPciLibFindPciCapability(GppPcieAddress, PCIE_CAP_ID);
  if ((CapBase == 0) || (CapBase == 0xff)) {
    PROM_TRACEPOINT(SIL_TRACE_ERROR, "  PT PCIe capability not found!\n");
    return SilNotFound;
  }

  CurrentSpeed = xUSLPciRead8(GppPcieAddress + CapBase + 0x12);
  PROM_TRACEPOINT(SIL_TRACE_INFO, "  After 1st fw loading: Current Speed x%d Gen%d\n",
                  CurrentSpeed >> 4, CurrentSpeed & 0xF);

  PTTargetSpeed = PromDataBlk->PromInputBlk.PT21PcieTargetSpeed;
  if ((CurrentSpeed & 0xF) != PTTargetSpeed) {
    PROM_TRACEPOINT(SIL_TRACE_INFO, "  Set PT Target Link Speed to Gen%d\n", PTTargetSpeed);
    MpioXferTable->MpioPcieSetSpeed(SilContext,
      &NbPcieData->PciePlatformConfig,
      GppDev,
      GppFun,
      PTTargetSpeed);
    do {
        CurrentSpeed = xUSLPciRead8(GppPcieAddress + 0x6B);
    } while ((CurrentSpeed & (BIT_8(5) + BIT_8(3))) != BIT_8(5));
    CurrentSpeed = xUSLPciRead8(GppPcieAddress + 0x6A);
    PROM_TRACEPOINT(SIL_TRACE_INFO, "  Current Speed is 0x%x x%d Gen%d\n",
                    CurrentSpeed, CurrentSpeed >> 4, CurrentSpeed & 0xF);

    if ((CurrentSpeed & 0xF) != PTTargetSpeed) {
      PROM_TRACEPOINT(SIL_TRACE_INFO, "  MPIO service failed, current speed is Gen%d\n",
                      (CurrentSpeed & 0xF));
      PROM_TRACEPOINT(SIL_TRACE_INFO, "  Set PT Target Link Speed to Gen%d\n", PTTargetSpeed);
      Prom21RetrainSpeed(PromDataBlk, GppBus, GppDev, GppFun, 0, PTTargetSpeed, false);
    }
  }

  if(PromDataBlk->PromInputBlk.PT21SecondPortNumber == 0xf) {
    SecPortNum = 0x8;
  } else {
    SecPortNum = PromDataBlk->PromInputBlk.PT21SecondPortNumber;
  }

  if (PromDataBlk->PromInputBlk.PT21SecondPortNumber != 0xE) {
    UspBus = xUSLPciRead8(GppPcieAddress + 0x19);
    DspBus = xUSLPciRead8(PCI_LIB_ADDRESS(UspBus, 0, 0, 0x19));

    if (PromDataBlk->PromInputBlk.PT21TogglePerst == 0x1) {
      Status = Prom21RetrainSpeed(PromDataBlk, DspBus, SecPortNum, 0, XhciMmio, 0x1, true);
    } else {
      Status = Prom21RetrainSpeed(PromDataBlk, DspBus, SecPortNum, 0, 0, 0x1, true);
    }

    if (Status == SilPass) {
      PROM_TRACEPOINT(SIL_TRACE_INFO, "  PT UspBus = 0x%x DspBus = 0x%x, Port %x \n",
                      UspBus, DspBus, SecPortNum);
      PROM_TRACEPOINT(SIL_TRACE_INFO, "  Check USB status before scan second PT\n");
      Counter = 0;
      do {
        UsbCmd = xUSLMemRead32((void *)(size_t)(XhciMmio + 0x20));
        PROM_TRACEPOINT(SIL_TRACE_INFO, "    UsbCmd = 0x%X Counter = %d\n", UsbCmd, Counter);
        if (UsbCmd & BIT_32(1)) {
          SilFchStall(2 * 1000);
        }
        Counter++;
      } while ((UsbCmd & BIT_32(1)) && (Counter < 100));

      ScanSecXhciDidVid(PromDataBlk,
        PtPciTable[0].PromBus + 1,
        SecPortNum,
        0,
        PtPciTable[0].PromBus + 4
        );

      if (PromDataBlk->PromOutputBlk.PT21GppPcieAddress[1]) {
        RestorePromBus(PromDataBlk, PtPciTable, 0x00BB);

        SecGppPcieAddress = PromDataBlk->PromOutputBlk.PT21GppPcieAddress[1];

        SecGppBus = (uint16_t)((SecGppPcieAddress >> 20) & 0xFFF);
        SecGppDev = (uint8_t)((SecGppPcieAddress >> 15) & 0x1F);
        SecGppFun = (uint8_t)((SecGppPcieAddress >> 12) & 0x07);

        PROM_TRACEPOINT(SIL_TRACE_INFO, "    Secondary PT is at 0x%x - B%d D% F%d\n",
                        SecGppPcieAddress, SecGppBus, SecGppDev, SecGppFun);

        if (PromDataBlk->PromInputBlk.PT21Revision >= 2) {
          SaveInitPromBus(PromDataBlk, PtPciTable, 0xBBBB);
        }

        SecXhciMmio = PromDataBlk->PromOutputBlk.PT21XhciMmio[1];
        if ((SecXhciMmio == 0x0) || (SecXhciMmio == 0xffffffff)) {
          if (PromDataBlk->PromInputBlk.PT21Revision >= 2) {
            RestorePromBus(PromDataBlk, PtPciTable, 0xBBBB);
          }
          return SilNotFound;
        }

        if (PromDataBlk->PromInputBlk.PT21Revision < 2) {
          SaveInitPromBus(PromDataBlk, PtPciTable, 0xBBBB);
        }

        XhciLock = Prom21XhciReadByteV2(PromDataBlk, SecXhciMmio, 0x15087);
        if ((XhciLock & BIT_8(4)) == 0) {
          PromDataBlk->PromOutputBlk.PT21XhciLock[1] = 1;
          PROM_TRACEPOINT(SIL_TRACE_INFO, "    Set SecPT21XhciLock = %d\n",
                          PromDataBlk->PromOutputBlk.PT21XhciLock[1]);
        }

        if ((Status == SilPass) && (XhciLock & BIT_8(4))) {
          PROM_TRACEPOINT(SIL_TRACE_INFO, "  Secondary PT XHCI is unlock\n");

          PtCpuMode = Prom21RunsInRamCode(PromDataBlk, SecXhciMmio);
          Prom21DspLaneConfig(&PromDataBlk->PromInputBlk.Secondary, SecXhciMmio);
          Prom21SpecificFeatures(&PromDataBlk->PromInputBlk.Secondary,
            PromDataBlk->PromOutputBlk.PT21XhciID[1],
            SecXhciMmio,
            PtCpuMode,
            PromDataBlk->PromInputBlk.BootMode,
            false
            );

          if ((PtCpuMode == 0) || !Prom21FwVersionMatch(PromDataBlk, SecXhciMmio)) {
            Status = LpPtXhciProceedFwLoad (PromDataBlk,
                       PtFwInRamPtr + 0x0C,
                       PtPciTable[0].PromBus + 6, 0, 0,
                       SecXhciMmio,
                       true
                       );

            if (Status == SilPass) {
              PROM_TRACEPOINT(SIL_TRACE_INFO, "  2nd PT load FW success\n");
            }
          } else {
            PROM_TRACEPOINT(SIL_TRACE_INFO, "  2nd PT runs in RAM code, not load fw\n");
          }
          Prom21RetrainSpeed(PromDataBlk,
            SecGppBus,
            SecGppDev,
            SecGppFun,
            0,
            PromDataBlk->PromInputBlk.PT21PcieTargetSpeed,
            false
            );

          SecCurrentSpeed = xUSLPciRead8(SecGppPcieAddress + 0x92);
          PROM_TRACEPOINT(SIL_TRACE_INFO, "  Secondary PT USP PCIe Link Width & Link Speed is x%d Gen%d\n",
                          SecCurrentSpeed >> 4, SecCurrentSpeed & 0xF);
          if ((SecCurrentSpeed & 0x0F) == (PromDataBlk->PromInputBlk.PT21PcieTargetSpeed & 0x0F)) {
            PROM_TRACEPOINT(SIL_TRACE_INFO, "  Restore Secondary PT DSP speed to Gen%d Success !!!\n",
              SecCurrentSpeed & 0x0F);
          } else {
            PROM_TRACEPOINT(SIL_TRACE_INFO, "  Restore Secondary PT DSP speed to Gen%d Fail !!! It's Gen%d now\n",
              PromDataBlk->PromInputBlk.PT21PcieTargetSpeed & 0x0F, SecCurrentSpeed & 0x0F);
          }
        } else {
          PROM_TRACEPOINT(SIL_TRACE_INFO, "  Secondary PT XHCI is lock: bypass PT code !!!\n");
        }

        Prom21RetrainSpeed(PromDataBlk,
          DspBus,
          SecPortNum,
          0,
          0,
          PromDataBlk->PromInputBlk.PT21PcieTargetSpeed,
          false
          );
        RestorePromBus(PromDataBlk, PtPciTable, 0xBBBB);
      } else {
        Prom21RetrainSpeed(PromDataBlk,
          DspBus,
          SecPortNum,
          0,
          0,
          PromDataBlk->PromInputBlk.PT21PcieTargetSpeed,
          false
          );
        RestorePromBus(PromDataBlk, PtPciTable, 0x00BB);
      }
    } else {
      RestorePromBus(PromDataBlk, PtPciTable, 0x00BB);
      Status = SilPass;
    }
  } else {
    RestorePromBus(PromDataBlk, PtPciTable, 0x00BB);
    Status = SilPass;
  }

  PROM_TRACEPOINT(SIL_TRACE_EXIT, "\n");

  return Status;
}

static uint8_t
FindPcieRootPortNumber (
  uint16_t    RootBridge,
  uint8_t     RootPortDevice,
  uint8_t     RootPortFunction
  )
{
  uint32_t            Ioapic0CfgAddress;
  uint32_t            RootPortAddress;
  uint8_t             Counter;

  Ioapic0CfgAddress = 0x14301000ul;
  RootPortAddress = ((uint32_t) (RootPortDevice & 0x1F) << 3) + (uint32_t) (RootPortFunction & 0x7);

  for (Counter = 0; Counter < 24; Counter++) {
    if (RootPortAddress == xUSLSmnRead(0, 0, Ioapic0CfgAddress + (uint32_t) Counter * 4)) {
      break;
    }
  }

  return Counter;
}

static uint32_t
FindPcieRegisterAddress (
  uint16_t RootBridge,
  uint8_t  RootPortDevice,
  uint8_t  RootPortFunction,
  uint8_t  RegisterSelection
  )
{
  uint32_t RootPortNumber;
  uint32_t RcCfgAddress;

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "  (0x%04X, 0x%02X, 0x%02X, 0x%02X)\n",
    RootBridge, RootPortDevice, RootPortFunction, RegisterSelection);

  if (!SocFamilyIdCheck(AMD_FAMILY_PHX)) {
    return 0;
  }

  RootPortNumber = (uint32_t) FindPcieRootPortNumber (RootBridge, RootPortDevice, RootPortFunction);
  PROM_TRACEPOINT(SIL_TRACE_INFO, "    PCIe Root Port Number = %d\n", RootPortNumber);

  RcCfgAddress = 0x11100000ul;

  if (RootPortNumber < 5) {
    RcCfgAddress += (RegisterSelection << 16) + (RootPortNumber << 12);
  } else if (RootPortNumber < 11) {
    RcCfgAddress += (1 << 20) + (RegisterSelection << 16) + ((RootPortNumber - 5) << 12);
  } else if (RootPortNumber == 11) {
    RcCfgAddress += (2 << 20) + (RegisterSelection << 16);
  } else if (RootPortNumber == 12) {
    RcCfgAddress += (3 << 20) + (RegisterSelection << 16);
  }

  if (RegisterSelection == 0x8) {
    RcCfgAddress &= 0xFFFF0FFF;
  }

  PROM_TRACEPOINT(SIL_TRACE_INFO, "    PCIe Register Address = 0x%08X\n", RcCfgAddress);
  return RcCfgAddress;
}

SIL_STATUS
AmdProm21FwLoadA1 (
  SIL_CONTEXT        *SilContext,
  PROMCLASS_DATA_BLK *PromDataBlk
  )
{
  uint16_t                        GppBus;
  uint8_t                         GppDev;
  uint8_t                         GppFun;
  uint16_t                        SecGppBus;
  uint8_t                         SecGppDev;
  uint8_t                         SecGppFun;
  uint16_t                        UspBus;
  uint16_t                        DspBus;
  uint8_t                         CurrentSpeed;
  uint8_t                         SecCurrentSpeed;
  uint8_t                         XhciLock;
  uint8_t                         PTTargetSpeed;
  uint8_t                         SecTargetSpeed;
  uint8_t                         DspTargetSpeed;
  uint8_t                         SecPortNum;
  uint8_t                         *PtFwInRamPtr;
  uint32_t                        PTFwSize;
  uint32_t                        PtFwChecksum;
  uint32_t                        Checksum;
  uint32_t                        XhciMmio;
  uint32_t                        SecXhciMmio;
  uint32_t                        GppPcieAddress;
  uint32_t                        SecGppPcieAddress;
  uint32_t                        Counter;
  uint32_t                        LcResetLinkAddress;
  uint32_t                        UsbCmd;
  uint32_t                        Value;
  SIL_STATUS                      Status;
  LOAD_FW_CONFIG_DATA             FwConfigBuf;
  PROM_PCI_SAVE_RESTORE_TABLE     PtPciTable[2];
  MPIO_COMMON_2_REV_XFER_BLOCK    *MpioXferTable;
  NORTH_BRIDGE_PCIE_SIB           *NbPcieData;
  PROM21_FW_VERSION               FwVersion;

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (SilGetCommon2RevXferTable(SilContext, SilId_MpioClass, (void **)(&MpioXferTable)) != SilPass) {
    return SilNotFound;
  }

  NbPcieData = (NORTH_BRIDGE_PCIE_SIB *)xUslFindStructure(SilContext,
                                          SilId_NbioClass,
                                          NBIOPCIECLASS_INSTANCE);
  if (NbPcieData == NULL) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO Pcie config not found.\n");
    return SilNotFound;
  }

  SecPortNum = 0;
  DspBus     = 0;
  PtFwInRamPtr = (uint8_t *)(uintptr_t)PromDataBlk->PromInputBlk.PT21FwInRamAddress;

  {
    PROM_TRACEPOINT(SIL_TRACE_INFO, "  BootMode != BOOT_ON_S3_RESUME, copy PT FW from Rom to Ram !!!\n");

    if (PromDataBlk->PromInputBlk.PT21FWLoading == 1) {
      FwConfigBuf.OperationCode = 0x01;
      FwConfigBuf.AvailableMemoryAddress = PromDataBlk->PromInputBlk.PT21FwInRamAddress;
      FwConfigBuf.AvailableMemorySize = (128 + 4) * 1024;
      Status = PspMboxBiosCmdLoadFwConfig(&FwConfigBuf);
      if (Status != SilPass) {
        PROM_TRACEPOINT(SIL_TRACE_ERROR, "Get PT FW from PSP Directory FAIL !!!\n");
        return Status;
      }
    } else {
      PROM_TRACEPOINT(SIL_TRACE_ERROR, "PT21FWLoading = 0 unsupported\n");
      return SilUnsupported;
    }
  }

  PtFwInRamPtr = (uint8_t *)(uintptr_t)PromDataBlk->PromInputBlk.PT21FwInRamAddress;
  PTFwSize  = ((uint32_t) *(PtFwInRamPtr + 0x07)) << 24;
  PTFwSize |= ((uint32_t) *(PtFwInRamPtr + 0x06)) << 16;
  PTFwSize |= ((uint32_t) *(PtFwInRamPtr + 0x05)) << 8;
  PTFwSize |= ((uint32_t) *(PtFwInRamPtr + 0x04)) << 0;

  PtFwChecksum  = ((uint32_t) *(PtFwInRamPtr + 0x0B)) << 24;
  PtFwChecksum |= ((uint32_t) *(PtFwInRamPtr + 0x0A)) << 16;
  PtFwChecksum |= ((uint32_t) *(PtFwInRamPtr + 0x09)) << 8;
  PtFwChecksum |= ((uint32_t) *(PtFwInRamPtr + 0x08)) << 0;

  Checksum = FwChecksum(PtFwInRamPtr, PTFwSize);

  PROM_TRACEPOINT(SIL_TRACE_INFO, "  PT FW Ram Address    = 0x%08X\n", PtFwInRamPtr);
  PROM_TRACEPOINT(SIL_TRACE_INFO, "  PT FW Size           = 0x%08X\n", PTFwSize);
  PROM_TRACEPOINT(SIL_TRACE_INFO, "  PT FW Checksum       = 0x%08X\n", PtFwChecksum);
  PROM_TRACEPOINT(SIL_TRACE_INFO, "  Checksum Calculation = 0x%08X\n", Checksum);

  if (PtFwChecksum != Checksum) {
    PROM_TRACEPOINT(SIL_TRACE_ERROR, "ERROR: PT FW Checksum ERROR !!!\n");
    return SilUnsupported;
  }

  FwVersion.Raw  = ((uint64_t)*(PtFwInRamPtr + 0x8C)) << 40;
  FwVersion.Raw |= ((uint64_t)*(PtFwInRamPtr + 0x8D)) << 32;
  FwVersion.Raw |= ((uint64_t)*(PtFwInRamPtr + 0x8E)) << 24;
  FwVersion.Raw |= ((uint64_t)*(PtFwInRamPtr + 0x8F)) << 16;
  FwVersion.Raw |= ((uint64_t)*(PtFwInRamPtr + 0x90)) << 8;
  FwVersion.Raw |= ((uint64_t)*(PtFwInRamPtr + 0x91)) << 0;
  PromDataBlk->PromOutputBlk.PT21FwVersion = FwVersion.Raw;
  PROM_TRACEPOINT(SIL_TRACE_INFO, "  Promontory FwVersion = 0x%016LX (%u%u%u%u%u%u_%02x.%02x.%02x)\n",
    FwVersion.Raw,
    FwVersion.Fields.Year >> 4 & 0xf, FwVersion.Fields.Year & 0xf,
    FwVersion.Fields.Month >> 4 & 0xf, FwVersion.Fields.Month & 0xf,
    FwVersion.Fields.Day >> 4 & 0xf, FwVersion.Fields.Day & 0xf,
    FwVersion.Fields.SubVersion[2], FwVersion.Fields.SubVersion[1],
    FwVersion.Fields.SubVersion[0]
    );

  GppPcieAddress = PromDataBlk->PromOutputBlk.PT21GppPcieAddress[0];
  SecGppPcieAddress = PromDataBlk->PromOutputBlk.PT21GppPcieAddress[1];

  GppBus = (uint16_t) ((GppPcieAddress >> 20) & 0xFFF);
  GppDev = (uint8_t) ((GppPcieAddress >> 15) & 0x1F);
  GppFun = (uint8_t) ((GppPcieAddress >> 12) & 0x07);

  XhciMmio = PromDataBlk->PromOutputBlk.PT21XhciMmio[0];
  if ((XhciMmio == 0x0) || (XhciMmio == 0xffffffff)) {
    return SilNotFound;
  }

  SaveInitPromBus(PromDataBlk, PtPciTable, 0x00BB);

  XhciLock = Prom21XhciReadByteV2(PromDataBlk, XhciMmio, 0x15087);
  if ((XhciLock & BIT_8(4)) == 0) {
    PromDataBlk->PromOutputBlk.PT21XhciLock[0] = 1;
    PROM_TRACEPOINT(SIL_TRACE_INFO, "    Set PcdPT21XhciLock = %d\n",
                    PromDataBlk->PromOutputBlk.PT21XhciLock[0]);
  }

  if (XhciLock & BIT_8(4)) {
    PROM_TRACEPOINT(SIL_TRACE_INFO, "  XHCI is unlocked\n");

    CurrentSpeed = xUSLPciRead8(GppPcieAddress + 0x6A);
    PROM_TRACEPOINT(SIL_TRACE_INFO, "  Before set PT speed GEN1: current speed x%d Gen%d\n",
                    CurrentSpeed >> 4, CurrentSpeed & 0xF);
    if ((CurrentSpeed & 0x0F) != PcieGen1) {
      MpioXferTable->MpioPcieSetSpeed(SilContext,
        &NbPcieData->PciePlatformConfig,
        GppDev,
        GppFun,
        PcieGen1);
      do {
          CurrentSpeed = xUSLPciRead8(GppPcieAddress + 0x6B);
        } while ((CurrentSpeed & (BIT_8(5) + BIT_8(3))) != BIT_8(5));

      CurrentSpeed = xUSLPciRead8(GppPcieAddress + 0x6A);
      PROM_TRACEPOINT(SIL_TRACE_INFO, "  Set PT speed to x%d Gen%d\n",
                      CurrentSpeed >> 4, CurrentSpeed & 0xF);
    } else {
      PROM_TRACEPOINT(SIL_TRACE_INFO, "  PT speed is Gen1 already\n");
    }

    Status = LpPtXhciProceedFwLoad(PromDataBlk,
                PtFwInRamPtr + 0x0C,
                PtPciTable[0].PromBus + 2, 0, 0,
                XhciMmio,
                false
                );
    if (Status == SilPass) {
      PROM_TRACEPOINT(SIL_TRACE_INFO, "  PT load FW success\n");
    }
  } else {
    PROM_TRACEPOINT(SIL_TRACE_INFO, "  XHCI is lock: bypass PT code !!!\n");
  }

  RestorePromBus(PromDataBlk, PtPciTable, 0x00BB);

  CurrentSpeed = xUSLPciRead8(GppPcieAddress + 0x6A);
  PROM_TRACEPOINT(SIL_TRACE_INFO, "  Before set DSP port to GEN1 for 2nd PT: Current Speed x%d Gen%d\n",
    CurrentSpeed >> 4, CurrentSpeed & 0xF);

  if(PromDataBlk->PromInputBlk.PT21SecondPortNumber == 0xf) {
    SecPortNum = 0x8;
  } else {
    SecPortNum = PromDataBlk->PromInputBlk.PT21SecondPortNumber;
  }

  // force DSP port 0 and port 8 to GEN1 for loading Secondary PROM21
  if ((SecGppPcieAddress == 0) && (PromDataBlk->PromInputBlk.PT21SecondPortNumber != 0xE)) {
    SaveInitPromBus (PromDataBlk, PtPciTable, 0x00BB);
    if (PromDataBlk->PromInputBlk.PT21RuninRam == 0x0) {
      UspBus = (uint16_t)xUSLPciRead8(GppPcieAddress + 0x19) | (GppBus & 0xFF00);
      DspBus = (uint16_t)xUSLPciRead8(PCI_LIB_ADDRESS(UspBus, 0, 0, 0x19)) | (UspBus & 0xFF00);

      DspTargetSpeed = xUSLPciRead8(PCI_LIB_ADDRESS(DspBus, SecPortNum, 0, 0xB0));
      DspTargetSpeed &= 0xF0;
      DspTargetSpeed |= 0x1;
      xUSLPciWrite8(PCI_LIB_ADDRESS(DspBus, SecPortNum, 0, 0xB0), DspTargetSpeed);
      PROM_TRACEPOINT(SIL_TRACE_INFO, "  PT UspBus = 0x%x DspBus = 0x%x, Port %x DspTargetSpeed = 0x%x\n",
        UspBus, DspBus, SecPortNum, DspTargetSpeed);
    }
  }

  // Link down WA
  LcResetLinkAddress = FindPcieRegisterAddress(GppBus, GppDev, GppFun, 4) + 0x280;
  if (LcResetLinkAddress == 0) {
    if ((SecGppPcieAddress == 0) && (PromDataBlk->PromInputBlk.PT21SecondPortNumber != 0xE)) {
      RestorePromBus (PromDataBlk, PtPciTable, 0x00BB);
    }
    PROM_TRACEPOINT(SIL_TRACE_ERROR, "  PT USP PCIe LC_RESET_LINK SMN Address not found!\n");
    return SilNotFound;
  }

  PROM_TRACEPOINT(SIL_TRACE_INFO, "  PT USP PCIe LC_RESET_LINK SMN Address is 0x%08X\n",
    LcResetLinkAddress);

  Value = xUSLSmnRead(0, 0, LcResetLinkAddress);
  xUSLSmnWrite(0, 0, LcResetLinkAddress, Value | BIT_32(3));
  SilFchStall(20 * 1000);
  xUSLSmnWrite(0, 0, LcResetLinkAddress, Value);

  do {
    CurrentSpeed = xUSLPciRead8(GppPcieAddress + 0x6B);
  } while ((CurrentSpeed & (BIT_8(5) + BIT_8(3))) != BIT_8(5));

  CurrentSpeed = xUSLPciRead8(GppPcieAddress + 0x6A);
  PROM_TRACEPOINT(SIL_TRACE_INFO, "  Before set Target speed: Current Speed x%d Gen%d\n",
    CurrentSpeed >> 4, CurrentSpeed & 0xF);

  PTTargetSpeed = PromDataBlk->PromInputBlk.PT21PcieTargetSpeed;
  if ((CurrentSpeed & 0xF) != PTTargetSpeed) {
    PROM_TRACEPOINT(SIL_TRACE_INFO, "  Set PT Target Link Speed to Gen%d\n", PTTargetSpeed);
    MpioXferTable->MpioPcieSetSpeed(SilContext,
      &NbPcieData->PciePlatformConfig,
      GppDev,
      GppFun,
      PTTargetSpeed);
    do {
        CurrentSpeed = xUSLPciRead8(GppPcieAddress + 0x6B);
    } while ((CurrentSpeed & (BIT_8(5) + BIT_8(3))) != BIT_8(5));
    CurrentSpeed = xUSLPciRead8(GppPcieAddress + 0x6A);
    PROM_TRACEPOINT(SIL_TRACE_INFO, "  Current Speed is 0x%x x%d Gen%d\n",
                    CurrentSpeed, CurrentSpeed >> 4, CurrentSpeed & 0xF);

    if ((CurrentSpeed & 0xF) != PTTargetSpeed) {
      PROM_TRACEPOINT(SIL_TRACE_INFO, "  MPIO service failed, current speed is Gen%d\n",
                      (CurrentSpeed & 0xF));
      PROM_TRACEPOINT(SIL_TRACE_INFO, "  Set PT Target Link Speed to Gen%d\n", PTTargetSpeed);
      Prom21RetrainSpeed(PromDataBlk, GppBus, GppDev, GppFun, 0, PTTargetSpeed, false);
    }
  }

  // Load Secondary PT21
  if ((SecGppPcieAddress == 0) && (PromDataBlk->PromInputBlk.PT21SecondPortNumber != 0xE)) {
    if (PromDataBlk->PromInputBlk.PT21RuninRam == 0x0) {
      PROM_TRACEPOINT(SIL_TRACE_INFO, "  Check USB status before scan second PT\n");
      Counter = 0;
      do {
        UsbCmd = xUSLMemRead32((void *)(size_t)(XhciMmio + 0x20));
        PROM_TRACEPOINT(SIL_TRACE_INFO, "    UsbCmd = 0x%X Counter = %d\n", UsbCmd, Counter);
        if (UsbCmd & BIT_32(1)) {
          SilFchStall(2 * 1000);
        }
        Counter++;
      } while ((UsbCmd & BIT_32(1)) && (Counter < 100));

      ScanSecXhciDidVid(PromDataBlk,
        PtPciTable[0].PromBus + 1,
        SecPortNum,
        0,
        PtPciTable[0].PromBus + 4
        );

      SecGppPcieAddress = PromDataBlk->PromOutputBlk.PT21GppPcieAddress[1];

      if (SecGppPcieAddress) {
        RestorePromBus(PromDataBlk, PtPciTable, 0x00BB);

        SecGppBus = (uint16_t)((SecGppPcieAddress >> 20) & 0xFFF);
        SecGppDev = (uint8_t)((SecGppPcieAddress >> 15) & 0x1F);
        SecGppFun = (uint8_t)((SecGppPcieAddress >> 12) & 0x07);

        PROM_TRACEPOINT(SIL_TRACE_INFO, "    Secondary PT is at 0x%x - B%d D% F%d\n",
                        SecGppPcieAddress, SecGppBus, SecGppDev, SecGppFun);

        SecXhciMmio = PromDataBlk->PromOutputBlk.PT21XhciMmio[1];
        if ((SecXhciMmio == 0x0) || (SecXhciMmio == 0xffffffff)) {
          if (PromDataBlk->PromInputBlk.PT21Revision >= 2) {
            RestorePromBus(PromDataBlk, PtPciTable, 0xBBBB);
          }
          return SilNotFound;
        }

        SaveInitPromBus(PromDataBlk, PtPciTable, 0xBBBB);

        XhciLock = Prom21XhciReadByteV2(PromDataBlk, SecXhciMmio, 0x15087);
        if ((XhciLock & BIT_8(4)) == 0) {
          PromDataBlk->PromOutputBlk.PT21XhciLock[1] = 1;
          PROM_TRACEPOINT(SIL_TRACE_INFO, "    Set SecPT21XhciLock = %d\n",
                          PromDataBlk->PromOutputBlk.PT21XhciLock[1]);
        }

        if ((Status == SilPass) && (XhciLock & BIT_8(4))) {
          PROM_TRACEPOINT(SIL_TRACE_INFO, "  Secondary PT XHCI is unlock\n");

          // Force PT DSP Gen1 before load Secondary FW
          SecCurrentSpeed = xUSLPciRead8(SecGppPcieAddress + 0x92);
          PROM_TRACEPOINT(SIL_TRACE_INFO, "  Secondary PT USP PCIe Link Width & Link Speed is x%d Gen%d\n",
            SecCurrentSpeed >> 4, SecCurrentSpeed & 0xF);
          if ((SecCurrentSpeed & 0x0F) == PcieGen1) {
            PROM_TRACEPOINT(SIL_TRACE_INFO, "  Secondary PT DSP set to Gen1 Success !!!\n");
          } else {
            PROM_TRACEPOINT(SIL_TRACE_INFO, "  Secondary PT DSP set to Gen1 Fail !!!\n");
          }

          Status = LpPtXhciProceedFwLoad (PromDataBlk,
                      PtFwInRamPtr + 0x0C,
                      PtPciTable[0].PromBus + 6, 0, 0,
                      SecXhciMmio,
                      true
                      );
          if (Status == SilPass) {
            PROM_TRACEPOINT(SIL_TRACE_INFO, "  2nd PT load FW success\n");
          }

          SecCurrentSpeed = xUSLPciRead8(SecGppPcieAddress + 0x92);
          PROM_TRACEPOINT(SIL_TRACE_INFO, "  Secondary PT USP PCIe Link Width & Link Speed is x%d Gen%d\n",
                          SecCurrentSpeed >> 4, SecCurrentSpeed & 0xF);

          SecCurrentSpeed = xUSLPciRead8(SecGppPcieAddress + 0x90);
          xUSLPciWrite8(SecGppPcieAddress + 0x90, SecCurrentSpeed | BIT_8(4));
          SilFchStall(10 * 1000);
          SecTargetSpeed = xUSLPciRead8(SecGppPcieAddress + 0x3E);
          xUSLPciWrite8(SecGppPcieAddress + 0x3E, SecTargetSpeed | BIT_8(6));
          SilFchStall(10 * 1000);
          xUSLPciWrite8(SecGppPcieAddress + 0x3E, SecTargetSpeed);
          SilFchStall(10 * 1000);
          xUSLPciWrite8(SecGppPcieAddress + 0x90, SecCurrentSpeed);

          do {
            SecTargetSpeed = xUSLPciRead8(SecGppPcieAddress + 0x93);
          } while ((SecTargetSpeed & (BIT_8(5) + BIT_8(3))) != BIT_8(5));

          SecCurrentSpeed = xUSLPciRead8(SecGppPcieAddress + 0x92);
          PROM_TRACEPOINT(SIL_TRACE_INFO, "  Secondary PT USP PCIe Link Width & Link Speed is x%d Gen%d\n",
            SecCurrentSpeed >> 4, SecCurrentSpeed & 0xF);

          if ((SecCurrentSpeed & 0x0F) != (PromDataBlk->PromInputBlk.PT21PcieTargetSpeed & 0x0F)) {
            SecTargetSpeed = xUSLPciRead8(SecGppPcieAddress + 0xB0);
            SecTargetSpeed &= 0xF0;
            SecTargetSpeed |= PromDataBlk->PromInputBlk.PT21PcieTargetSpeed  & 0x0F;
            xUSLPciWrite8(SecGppPcieAddress + 0xB0, SecTargetSpeed);

            SecTargetSpeed = xUSLPciRead8(SecGppPcieAddress + 0x90);
            xUSLPciWrite8(SecGppPcieAddress + 0x90, SecTargetSpeed | BIT_8(5));

            for (Counter = 0; Counter < 50; Counter++) {
              SecTargetSpeed = xUSLPciRead8(SecGppPcieAddress + 0x92);
              if ((SecTargetSpeed & 0x0F) == (PromDataBlk->PromInputBlk.PT21PcieTargetSpeed & 0x0F)) {
                break;
              }
              SilFchStall(20 * 1000);
            }

            SecCurrentSpeed = xUSLPciRead8(SecGppPcieAddress + 0x92);
            PROM_TRACEPOINT(SIL_TRACE_INFO, "  Secondary PT USP PCIe Link Width & Link Speed is x%d Gen%d\n",
              SecCurrentSpeed >> 4, SecCurrentSpeed & 0xF);

            if ((SecCurrentSpeed & 0x0F) == (PromDataBlk->PromInputBlk.PT21PcieTargetSpeed & 0x0F)) {
              PROM_TRACEPOINT(SIL_TRACE_INFO, "  Restore Secondary PT DSP speed to Gen%d Success !!!\n",
                SecCurrentSpeed & 0x0F);
            } else {
              PROM_TRACEPOINT(SIL_TRACE_INFO, "  Restore Secondary PT DSP speed to Gen%d Fail !!! It's Gen%d now\n",
                PromDataBlk->PromInputBlk.PT21PcieTargetSpeed & 0x0F, SecCurrentSpeed & 0x0F);
            }
          }
        } else {
          PROM_TRACEPOINT(SIL_TRACE_INFO, "  Secondary PT XHCI is lock: bypass PT code !!!\n");
        }
        RestorePromBus(PromDataBlk, PtPciTable, 0xBBBB);
      } else {
        PROM_TRACEPOINT(SIL_TRACE_INFO, " Bus %x is NOT Second PT, retrain to target speed\n", DspBus);
        Prom21RetrainSpeed(PromDataBlk,
          DspBus,
          SecPortNum,
          0,
          0,
          PromDataBlk->PromInputBlk.PT21PcieTargetSpeed,
          false
          );
        RestorePromBus(PromDataBlk, PtPciTable, 0x00BB);
      }
    } else {
      ScanSecXhciDidVid(PromDataBlk,
        PtPciTable[0].PromBus + 1,
        SecPortNum,
        0,
        PtPciTable[0].PromBus + 4
        );
      RestorePromBus(PromDataBlk, PtPciTable, 0x00BB);
      Status = SilPass;
    }
  }

  PROM_TRACEPOINT(SIL_TRACE_EXIT, "\n");

  return Status;
}
