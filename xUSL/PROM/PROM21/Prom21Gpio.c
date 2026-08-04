/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file Prom21Gpio.c
 *  @brief Promontory21 GPIO related functions
 */

#include <SilCommon.h>
#include <CommonLib/Mmio.h>
#include <Pci.h>
#include <PROM/Common/PromAccess.h>
#include <PROM/Common/PromInit.h>
#include <PROM/PromClass-api.h>
#include "Prom21.h"
#include "Prom21Gpio.h"
#include "Prom21Init.h"

SIL_STATUS
Prom21GpioWrite (
  PROMCLASS_DATA_BLK *PromDataBlk,
  uint8_t            Pin,
  uint8_t            Value
  )
{
  SIL_STATUS                    Status;
  PROM_PCI_SAVE_RESTORE_TABLE   PtPciTable[2];
  uint32_t                      SecGppPcieAddress;
  uint32_t                      GpioMmioAddr;
  uint32_t                      SecGpioMmioAddr;
  uint32_t                      MmioAddr;
  PT_GPIO_REG                   PtGpioReg;
  bool                          FoundSecondary;

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, " Pin %d = %x\n", Pin, Value);

  Status = SilNotFound;
  MmioAddr = 0;
  FoundSecondary = PromDataBlk->PromOutputBlk.SecondaryPTPresent;
  SecGppPcieAddress = PromDataBlk->PromOutputBlk.PT21GppPcieAddress[1];

  if (SecGppPcieAddress) {
    SaveInitPromBus(PromDataBlk, PtPciTable, 0x1111);
  } else {
    SaveInitPromBus(PromDataBlk, PtPciTable, 0x11);
  }

  if (xUSLPciRead32(PCI_LIB_ADDRESS(PtPciTable[1].PromBus, 0, 0, 0)) == PT21_USP_ID) {
    SecGpioMmioAddr = xUSLPciRead32(PCI_LIB_ADDRESS(PtPciTable[1].PromBus, 0, 0, 0x40));
    SecGpioMmioAddr &= 0xFFFFFFF0;
  } else {
    RestorePromBus(PromDataBlk, PtPciTable, 0x1100);
    FoundSecondary = false;
  }

  GpioMmioAddr = xUSLPciRead32(PCI_LIB_ADDRESS(PtPciTable[0].PromBus, 0, 0, 0x40));
  GpioMmioAddr &= 0xFFFFFFF0;

  if (Pin < PROM21_MAX_GPIO_PIN_NUMBER) {
    MmioAddr = GpioMmioAddr;
  } else {
    if (FoundSecondary) {
      if (Pin < (PROM21_MAX_GPIO_PIN_NUMBER << 1)) {
        Pin -= PROM21_MAX_GPIO_PIN_NUMBER;
        MmioAddr = SecGpioMmioAddr;
      }
    } else {
      return Status;
    }
  }

  if (MmioAddr) {
    PtGpioReg.GpioPinDir = xUSLMemRead32((void *)(size_t)(MmioAddr));
    if (PtGpioReg.GpioPinDir & BIT_32(Pin)) {
      PtGpioReg.GpioOutputData = xUSLMemRead32((void *)(size_t)(MmioAddr + 0x8));
      if (Value) {
        PtGpioReg.GpioOutputData |= BIT_32(Pin);
      } else {
        PtGpioReg.GpioOutputData &= ~BIT_32(Pin);
      }
      xUSLMemWrite32((void *)(size_t)(MmioAddr + 0x8), PtGpioReg.GpioOutputData);
    }
    Status = SilPass;
  }

  if (FoundSecondary) {
    RestorePromBus(PromDataBlk, PtPciTable, 0x1111);
  } else {
    RestorePromBus(PromDataBlk, PtPciTable, 0x11);
  }

  return Status;
}

SIL_STATUS
Prom21GpioRead (
  PROMCLASS_DATA_BLK *PromDataBlk,
  uint8_t            Pin,
  uint8_t            *Value
  )
{
  SIL_STATUS                    Status;
  PROM_PCI_SAVE_RESTORE_TABLE   PtPciTable[2];
  uint32_t                      SecGppPcieAddress;
  uint32_t                      GpioMmioAddr;
  uint32_t                      SecGpioMmioAddr;
  uint32_t                      MmioAddr;
  PT_GPIO_REG                   PtGpioReg;
  bool                          FoundSecondary;

  Status = SilNotFound;
  FoundSecondary =  PromDataBlk->PromOutputBlk.SecondaryPTPresent;
  SecGppPcieAddress = PromDataBlk->PromOutputBlk.PT21GppPcieAddress[1];
  MmioAddr = 0;

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, " Pin %d\n", Pin, Value);

  if (SecGppPcieAddress) {
    SaveInitPromBus(PromDataBlk, PtPciTable, 0x1111);
  } else {
    SaveInitPromBus(PromDataBlk, PtPciTable, 0x11);
  }

  if (xUSLPciRead32(PCI_LIB_ADDRESS(PtPciTable[1].PromBus, 0, 0, 0)) == PT21_USP_ID) {
    SecGpioMmioAddr = xUSLPciRead32(PCI_LIB_ADDRESS(PtPciTable[1].PromBus, 0, 0, 0x40));
    SecGpioMmioAddr &= 0xFFFFFFF0;
  } else {
    RestorePromBus(PromDataBlk, PtPciTable, 0x1100);
    FoundSecondary = false;
  }

  GpioMmioAddr = xUSLPciRead32(PCI_LIB_ADDRESS(PtPciTable[0].PromBus, 0, 0, 0x40));
  GpioMmioAddr &= 0xFFFFFFF0;

  if (Pin < PROM21_MAX_GPIO_PIN_NUMBER) {
    MmioAddr = GpioMmioAddr;
  } else {
    if (FoundSecondary) {
      if (Pin < (PROM21_MAX_GPIO_PIN_NUMBER << 1)) {
        Pin -= PROM21_MAX_GPIO_PIN_NUMBER;
        MmioAddr = SecGpioMmioAddr;
      }
    } else {
      return Status;
    }
  }

  if (MmioAddr) {
    PtGpioReg.GpioPinDir = xUSLMemRead32((void *)(size_t)(MmioAddr));
    if (PtGpioReg.GpioPinDir & BIT_32(Pin)) {
      PtGpioReg.GpioOutputData = xUSLMemRead32((void *)(size_t)(MmioAddr + 0x8));
      PROM_TRACEPOINT(SIL_TRACE_INFO, "GpioOutputData 0x%x , %x\n",
                      PtGpioReg.GpioOutputData, PtGpioReg.GpioOutputData & BIT_32(Pin));
      if (PtGpioReg.GpioOutputData & BIT_32(Pin)) {
        *Value = 0x1;
        PROM_TRACEPOINT(SIL_TRACE_INFO, "GpioOutputData High (%d)\n", *Value);
      } else {
        *Value = 0x0;
        PROM_TRACEPOINT(SIL_TRACE_INFO, "GpioOutputData Low (%d)\n", *Value);
      }
    } else {
      PtGpioReg.GpioInputData = xUSLMemRead32((void *)(size_t)(MmioAddr + 0x4));
      PROM_TRACEPOINT(SIL_TRACE_INFO, "GpioInputData 0x%x , %x\n", PtGpioReg.GpioInputData, PtGpioReg.GpioInputData & BIT_32(Pin));
      if (PtGpioReg.GpioInputData & BIT_32(Pin)) {
        *Value = 0x1;
        PROM_TRACEPOINT(SIL_TRACE_INFO, "GpioInputData High (%d)\n", *Value);
      } else {
        *Value = 0x0;
        PROM_TRACEPOINT(SIL_TRACE_INFO, "GpioInputData Low (%d)\n", *Value);
      }
    }
    Status = SilPass;
  }

  if (FoundSecondary) {
    RestorePromBus(PromDataBlk, PtPciTable, 0x1111);
  } else {
    RestorePromBus(PromDataBlk, PtPciTable, 0x11);
  }

  return Status;
}

static void
Prom21GpioInitial (
  PROMCLASS_DATA_BLK *PromDataBlk
  )
{
  uint32_t            GpioPin;
  uint32_t            GpioMmio;
  uint32_t            SecGpioMmio;
  uint32_t            Data32;
  uint16_t            MaxGpioPin;
  uint16_t            PinNum;
  PT_GPIO_REG         GpioReg;
  PT_GPIO_REG         GpioRegData;
  PT_GPIO_REG         SecGpioReg;
  PT_GPIO_REG         SecGpioRegData;
  PROM21_GPIO_INIT_TABLE      *PTGpioDataPtr;
  PROM21_GPIO_ITEM            *TablePtr;

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  GpioMmio = PromDataBlk->PromOutputBlk.PT21GpioMmio[0];
  SecGpioMmio = PromDataBlk->PromOutputBlk.PT21GpioMmio[1];
  Data32   = 0;

  GpioReg.GpioPinDir = 0;
  GpioReg.GpioInputData = 0;
  GpioReg.GpioOutputData = 0;

  SecGpioReg.GpioPinDir = 0;
  SecGpioReg.GpioInputData = 0;
  SecGpioReg.GpioOutputData = 0;

  PTGpioDataPtr = (PROM21_GPIO_INIT_TABLE *)&PromDataBlk->PromInputBlk.PT21GpioInitTable;

  if (PromDataBlk->PromOutputBlk.SecondaryPTPresent) {
    MaxGpioPin = PROM21_MAX_GPIO_PIN_NUMBER << 1;
  } else {
    MaxGpioPin = PROM21_MAX_GPIO_PIN_NUMBER;
  }

  PROM_TRACEPOINT(SIL_TRACE_INFO, "PT-MaxGpioPin = %d\n", MaxGpioPin);

  if (PTGpioDataPtr) {
    TablePtr = &PTGpioDataPtr->GpioList[0];

    while ((TablePtr->Pin != 0xFF) && (TablePtr->Pin < MaxGpioPin)) {
      PinNum = TablePtr->Pin;
      if (PinNum < PROM21_MAX_GPIO_PIN_NUMBER) {
        GpioPin = BIT_32(PinNum);
        PROM_TRACEPOINT(SIL_TRACE_INFO, "Primary PT GPIO Pin-%d Setting = %d (0x%x)\n",
                        PinNum, TablePtr->Pin, GpioPin);
        if (TablePtr->Setting.Gpio.OutEnB) {
          GpioReg.GpioPinDir |= GpioPin;
          if (TablePtr->Setting.Gpio.Out) {
            GpioReg.GpioOutputData |= GpioPin;
          } else {
            GpioReg.GpioOutputData &= ~GpioPin;
        }
        } else {
          GpioReg.GpioPinDir &= ~GpioPin;
        }
      } else {
        PinNum -= PROM21_MAX_GPIO_PIN_NUMBER;
        GpioPin = BIT_32(PinNum);
        PROM_TRACEPOINT(SIL_TRACE_INFO, "Secondary PT GPIO Pin-%d Setting = %d (0x%x)\n",
                        PinNum, TablePtr->Pin, GpioPin);
        if (TablePtr->Setting.Gpio.OutEnB) {
          SecGpioReg.GpioPinDir |= GpioPin;
          if (TablePtr->Setting.Gpio.Out) {
            SecGpioReg.GpioOutputData |= GpioPin;
          } else {
            SecGpioReg.GpioOutputData &= ~GpioPin;
          }
        } else {
          SecGpioReg.GpioPinDir &= ~GpioPin;
        }
      }
      TablePtr++;
    }

    GpioRegData.GpioOutputData = xUSLMemRead32((void *)(size_t)(GpioMmio + 0x8));
    xUSLMemWrite32((void *)(size_t)(GpioMmio + 0x8), (Data32 | GpioReg.GpioOutputData));

    GpioRegData.GpioPinDir  = xUSLMemRead32((void *)(size_t)(GpioMmio));
    xUSLMemWrite32((void *)(size_t)(GpioMmio), (Data32 | GpioReg.GpioPinDir));

    GpioRegData.GpioInputData = xUSLMemRead32((void *)(size_t)(GpioMmio + 0x4));

    PROM_TRACEPOINT(SIL_TRACE_INFO, "PT GPIO Pin Setting = 0x%x, 0x%x\n",
                    GpioRegData.GpioPinDir, GpioRegData.GpioOutputData);
    PROM_TRACEPOINT(SIL_TRACE_INFO, "PT GPIO Pin New Setting = 0x%x, 0x%x\n",
                    GpioReg.GpioPinDir, GpioReg.GpioOutputData);

    if (PromDataBlk->PromOutputBlk.SecondaryPTPresent) {
      SecGpioRegData.GpioOutputData = xUSLMemRead32((void *)(size_t)(SecGpioMmio + 0x8));
      xUSLMemWrite32((void *)(size_t)(SecGpioMmio + 0x8), SecGpioReg.GpioOutputData);

      SecGpioRegData.GpioPinDir = xUSLMemRead32((void *)(size_t)(SecGpioMmio));
      xUSLMemWrite32((void *)(size_t)(SecGpioMmio), SecGpioReg.GpioPinDir);

      SecGpioRegData.GpioInputData = xUSLMemRead32((void *)(size_t)(SecGpioMmio + 0x4));
      PROM_TRACEPOINT(SIL_TRACE_INFO, "Sec PT GPIO Pin Setting = 0x%x, 0x%x\n",
                      SecGpioRegData.GpioPinDir, SecGpioRegData.GpioOutputData);
      PROM_TRACEPOINT(SIL_TRACE_INFO, "Sec PT GPIO Pin New Setting = 0x%x, 0x%x\n",
                      SecGpioReg.GpioPinDir, SecGpioReg.GpioOutputData);
    }
  }

  PROM_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

SIL_STATUS
Prom21ProgramGpiosInitial (
  PROMCLASS_DATA_BLK *PromDataBlk
  )
{
  uint16_t                      GppBus;
  uint8_t                       GppDev;
  uint8_t                       GppFun;
  uint16_t                      SecGppBus;
  uint8_t                       SecGppDev;
  uint8_t                       SecGppFun;
  uint16_t                      SecUspBus;
  uint32_t                      Value32;
  uint16_t                      IoHcBus;
  uint32_t                      IohcBridgeCntlSmnReg;
  uint32_t                      GppPcieAddress;
  uint32_t                      SecGppPcieAddress;
  PROM_PCI_SAVE_RESTORE_TABLE   PtPciTable[2];
  bool                          SecondaryFound;

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (PromDataBlk->PromOutputBlk.PT21XhciID[0] == 0) {
    PROM_TRACEPOINT(SIL_TRACE_ERROR," Promontory is NOT FOUND !!\n");
    return SilNotFound;
  }

  if (PromDataBlk->PromInputBlk.BootMode == 0x20 /* BOOT_IN_RECOVERY_MODE */) {
    PROM_TRACEPOINT(SIL_TRACE_INFO, "  In Recovery mode!\n");
    return SilPass;
  }

  if (PromDataBlk->PromOutputBlk.PT21GpioID == 0) {
    PROM_TRACEPOINT(SIL_TRACE_ERROR, "PT GPIO is NOT FOUND!\n");
    return SilNotFound;
  }

  SecondaryFound = PromDataBlk->PromOutputBlk.SecondaryPTPresent;
  GppPcieAddress = PromDataBlk->PromOutputBlk.PT21GppPcieAddress[0];
  SecGppPcieAddress = PromDataBlk->PromOutputBlk.PT21GppPcieAddress[1];
  IoHcBus = (uint16_t) PromDataBlk->PromOutputBlk.PT21IoHcBusNum |
            (uint16_t) ((GppPcieAddress >> 20) & 0xF00);

  if (SecGppPcieAddress) {
    SaveInitPromBus(PromDataBlk, PtPciTable, 0x1111);
  } else {
    SaveInitPromBus(PromDataBlk, PtPciTable, 0x11);
  }

  if (PtPciTable[0].PromBus != 0) {
    if (xUSLPciRead32(PCI_LIB_ADDRESS(PtPciTable[0].PromBus, 0, 0, 0)) != PT21_USP_ID) {
      RestorePromBus(PromDataBlk, PtPciTable, 0x11);
      PROM_TRACEPOINT(SIL_TRACE_ERROR, "PT GPIO device NOT FOUND!\n");
      return SilNotFound;
    }
  }

  if (PtPciTable[1].PromBus != 0) {
    if (xUSLPciRead32(PCI_LIB_ADDRESS(PtPciTable[1].PromBus, 0, 0, 0)) != PT21_USP_ID) {
      RestorePromBus(PromDataBlk, PtPciTable, 0x1100);
      PROM_TRACEPOINT(SIL_TRACE_ERROR, "Secondary PT GPIO device NOT FOUND!\n");
      SecondaryFound = false;
    }
  }

  IohcBridgeCntlSmnReg = PromDataBlk->PromOutputBlk.PT21IohcBridgeCntl;
  xUSLPciWrite32(PCI_LIB_ADDRESS(IoHcBus, 0, 0, 0x60), IohcBridgeCntlSmnReg);
  Value32 = xUSLPciRead32(PCI_LIB_ADDRESS(IoHcBus, 0, 0, 0x64));
  Value32 &= 0x007FFFFF;
  Value32 |= ((((PromDataBlk->PromOutputBlk.PT21GpioMmio[0] >> 12) & 0xFF) << 24) + (1 << 23));
  xUSLPciWrite32(PCI_LIB_ADDRESS(IoHcBus, 0, 0, 0x64), Value32);

  Value32 = PromDataBlk->PromOutputBlk.PT21GpioMmio[0];

  xUSLPciWrite32(PCI_LIB_ADDRESS(PtPciTable[0].PromBus, 0, 0, 0x40), Value32);
  Value32 = 0x0;
  xUSLPciWrite32(PCI_LIB_ADDRESS(PtPciTable[0].PromBus, 0, 0, 0x44), Value32);

  Value32 = xUSLPciRead32(PCI_LIB_ADDRESS(PtPciTable[0].PromBus, 0, 0, 0x48));
  Value32 |= 0x01;
  xUSLPciWrite32(PCI_LIB_ADDRESS(PtPciTable[0].PromBus, 0, 0, 0x48), Value32);

  if (PromDataBlk->PromOutputBlk.SecondaryPTPresent) {
    GppBus = (uint16_t) ((GppPcieAddress >> 20) & 0xFFF);
    GppDev = (uint8_t) ((GppPcieAddress >> 15) & 0x1F);
    GppFun = (uint8_t) ((GppPcieAddress >> 12) & 0x07);

    SecGppBus = (uint16_t) ((SecGppPcieAddress >> 20) & 0xFFF);
    SecGppDev = (uint8_t) ((SecGppPcieAddress >> 15) & 0x1F);
    SecGppFun = (uint8_t) ((SecGppPcieAddress >> 12) & 0x07);
    SecUspBus = (uint16_t) xUSLPciRead8(
                             PCI_LIB_ADDRESS(SecGppBus, SecGppDev, SecGppFun, 0x19)) |
                             (SecGppBus & 0xFF00);

    PROM_TRACEPOINT(SIL_TRACE_INFO, "PT @ Bus %x, Dev %x, Func %x\n",
                    GppBus, GppDev, GppFun);
    PROM_TRACEPOINT(SIL_TRACE_INFO, "Sec PT @ Bus %x, Dev %x, Func %x, Usp %x\n",
                    SecGppBus, SecGppDev, SecGppFun, SecUspBus);

    Value32 = PromDataBlk->PromOutputBlk.PT21GpioMmio[1];

    xUSLPciWrite16(PCI_LIB_ADDRESS(GppBus, GppDev, GppFun, 0x20),
      (uint16_t)(Value32 >> 16));
    xUSLPciWrite16(PCI_LIB_ADDRESS(GppBus, GppDev, GppFun, 0x22),
      (uint16_t)(Value32 >> 16));

    xUSLPciWrite16(PCI_LIB_ADDRESS(SecGppBus, SecGppDev, SecGppFun, 0x20),
      (uint16_t)(Value32 >> 16));
    xUSLPciWrite16(PCI_LIB_ADDRESS(SecGppBus, SecGppDev, SecGppFun, 0x22),
      (uint16_t)(Value32 >> 16));

    xUSLPciWrite32(PCI_LIB_ADDRESS(SecUspBus, 0, 0, 0x40), Value32);
    Value32 = 0x0;
    xUSLPciWrite32(PCI_LIB_ADDRESS(SecUspBus, 0, 0, 0x44), Value32);

    Value32 = xUSLPciRead32(PCI_LIB_ADDRESS(SecUspBus, 0, 0, 0x48));
    Value32 |= 0x01;
    xUSLPciWrite32(PCI_LIB_ADDRESS(SecUspBus, 0, 0, 0x48), Value32);
  }

  Prom21GpioInitial(PromDataBlk);

  if (SecGppPcieAddress && SecondaryFound) {
    RestorePromBus(PromDataBlk, PtPciTable, 0x1111);
  } else {
    RestorePromBus(PromDataBlk, PtPciTable, 0x11);
  }

  PROM_TRACEPOINT(SIL_TRACE_INFO, "\n");

  return SilPass;
}
