/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file Prom21Init.c
 *  @brief Promontory21 related functions
 */

#include <SilCommon.h>
#include <xSIM.h>
#include <string.h>
#include <SilSocLogicalId.h>
#include <DF/DfIp2Ip.h>
#include <RcMgr/RcMgrIp2Ip.h>
#include <Mpio/Common/MpioCmn2Rev.h>
#include <Nbio/Phx/includePHX/PHX_IOHC.h>
#include <PROM/Common/PromAccess.h>
#include <PROM/Common/PromInit.h>
#include <PROM/PromClass-api.h>
#include "Prom21.h"
#include "Prom21Gpio.h"
#include "Prom21Init.h"

void
ScanSecXhciDidVid (
  PROMCLASS_DATA_BLK *PromDataBlk,
  uint16_t           GppBus,
  uint8_t            GppDev,
  uint8_t            GppFun,
  uint16_t           TempBusNum
  )
{
  uint32_t DidVid = 0;
  uint32_t PtDspBusData = 0;
  uint32_t SecPtUspBusData = 0;
  uint32_t SecPtDspBusData = 0;
  uint32_t GppPcieAddress = 0;

  PROM_TRACEPOINT(SIL_TRACE_ENTRY,"    PT DSP [%04X:%02X:%02X]\n", GppBus, GppDev, GppFun);

  DidVid = xUSLPciRead32(PCI_LIB_ADDRESS(GppBus, GppDev, GppFun, 0x00));
  if (DidVid == 0xFFFFFFFF) {
    PROM_TRACEPOINT(SIL_TRACE_INFO, "    PT DSP [%04X:%02X:%02X] is not present\n",
                    GppBus, GppDev, GppFun);
    return;
  }

  DidVid = xUSLPciRead32(PCI_LIB_ADDRESS(GppBus, GppDev, GppFun, 0x08));
  if ((DidVid >> 16) != 0x0604) {
    PROM_TRACEPOINT(SIL_TRACE_INFO, "    PT DSP [%04X:%02X:%02X] is not a PCIE bridge\n",
                    GppBus, GppDev, GppFun);
    return;
  }

  DidVid = xUSLPciRead32(PCI_LIB_ADDRESS(GppBus, GppDev, GppFun, 0x98));
  if ((DidVid & BIT_32(22)) == 0) {
    PROM_TRACEPOINT(SIL_TRACE_INFO, "    PT DSP [%04X:%02X:%02X] EP device is not present\n",
                    GppBus, GppDev, GppFun);
    return;
  }

  PtDspBusData = xUSLPciRead32(PCI_LIB_ADDRESS(GppBus, GppDev, GppFun, 0x18));

  xUSLPciWrite8(PCI_LIB_ADDRESS(GppBus, GppDev, GppFun, 0x18), (uint8_t)GppBus);
  xUSLPciWrite8(PCI_LIB_ADDRESS(GppBus, GppDev, GppFun, 0x19), (uint8_t)TempBusNum);
  xUSLPciWrite8(PCI_LIB_ADDRESS(GppBus, GppDev, GppFun, 0x1A), (uint8_t)TempBusNum + 3);

  DidVid = xUSLPciRead32(PCI_LIB_ADDRESS(TempBusNum, 0, 0, 0x08));
  if ((DidVid >> 16) != 0x0604) {
    xUSLPciWrite32(PCI_LIB_ADDRESS(GppBus, GppDev, GppFun, 0x18), PtDspBusData);
    PROM_TRACEPOINT(SIL_TRACE_INFO, "    SEC PT USP [%04X:%02X:%02X] is not a PCIE bridge\n",
                    TempBusNum, 0, 0);
    return;
  }

  SecPtUspBusData = xUSLPciRead32(PCI_LIB_ADDRESS(TempBusNum, 0, 0, 0x18));

  xUSLPciWrite8(PCI_LIB_ADDRESS(TempBusNum, 0, 0, 0x18), (uint8_t)TempBusNum);
  xUSLPciWrite8(PCI_LIB_ADDRESS(TempBusNum, 0, 0, 0x19), (uint8_t)TempBusNum + 1);
  xUSLPciWrite8(PCI_LIB_ADDRESS(TempBusNum, 0, 0, 0x1A), (uint8_t)TempBusNum + 3);

  DidVid = xUSLPciRead32(PCI_LIB_ADDRESS(TempBusNum + 1, PROM21_XHCI_DSP_DEV, 0, 0x08));
  if ((DidVid >> 16) != 0x0604) {
    xUSLPciWrite32(PCI_LIB_ADDRESS(TempBusNum, 0, 0, 0x18), SecPtUspBusData);
    xUSLPciWrite32(PCI_LIB_ADDRESS(GppBus, GppDev, GppFun, 0x18), PtDspBusData);
    PROM_TRACEPOINT(SIL_TRACE_INFO, "    SEC XHCI DSP [%04X:%02X:%02X] is not a PCIE bridge\n",
                    TempBusNum + 1, PROM21_XHCI_DSP_DEV, 0);
    return;
  }

  SecPtDspBusData = xUSLPciRead32(PCI_LIB_ADDRESS(TempBusNum + 1, PROM21_XHCI_DSP_DEV, 0, 0x18));

  xUSLPciWrite8(PCI_LIB_ADDRESS(TempBusNum + 1, PROM21_XHCI_DSP_DEV, 0, 0x18),
    (uint8_t)TempBusNum + 1);
  xUSLPciWrite8(PCI_LIB_ADDRESS(TempBusNum + 1, PROM21_XHCI_DSP_DEV, 0, 0x19),
    (uint8_t)TempBusNum + 2);
  xUSLPciWrite8(PCI_LIB_ADDRESS(TempBusNum + 1, PROM21_XHCI_DSP_DEV, 0, 0x1A),
    (uint8_t)TempBusNum + 2);

  DidVid = xUSLPciRead32(PCI_LIB_ADDRESS(TempBusNum + 2, 0, 0, 0));

  PROM_TRACEPOINT(SIL_TRACE_INFO, "    EndPoint DidVid = 0x%08X\n", DidVid);

  if ((DidVid == PT21_XHCI_ID_L1) || (DidVid == PT21_XHCI_ID_L2) ||
      (DidVid == PT21_XHCI_ID_L3) || (DidVid == PT21_XHCI_ID_L4) ||
      (DidVid == PT21_XHCI_ID_L5) || (DidVid == PT21_XHCI_ID_L6) ||
      (DidVid == PT21_XHCI_ID_L7) || (DidVid == PT21_XHCI_ID_L8)) {
    GppPcieAddress = PCI_LIB_ADDRESS(GppBus, GppDev, GppFun, 0);
    PromDataBlk->PromOutputBlk.PT21XhciID[1] = DidVid;
    PromDataBlk->PromOutputBlk.PT21GppPcieAddress[1] = GppPcieAddress;
  }

  xUSLPciWrite32(PCI_LIB_ADDRESS(TempBusNum + 1, PROM21_XHCI_DSP_DEV, 0, 0x18),
    SecPtDspBusData);
  xUSLPciWrite32(PCI_LIB_ADDRESS(TempBusNum, 0, 0, 0x18),
    SecPtUspBusData);
  xUSLPciWrite32(PCI_LIB_ADDRESS(GppBus, GppDev, GppFun, 0x18),
    PtDspBusData);

  if (GppPcieAddress != 0) {
    PROM_TRACEPOINT(SIL_TRACE_INFO, "    Found Second Promontory!\n");
    PROM_TRACEPOINT(SIL_TRACE_INFO, "    Set Secondary PT21XhciID         = 0x%08X\n",
                    PromDataBlk->PromOutputBlk.PT21XhciID[1]);
    PROM_TRACEPOINT(SIL_TRACE_INFO, "    Set Secondary PT21GppPcieAddress = 0x%08X\n",
                    PromDataBlk->PromOutputBlk.PT21GppPcieAddress[1]);
    PromDataBlk->PromOutputBlk.SecondaryPTPresent = true;
  } else {
    PROM_TRACEPOINT(SIL_TRACE_INFO, "    It's not Second Promontory!\n");
  }

  return;
}

static void
ScanXhciDidVid (
  PROMCLASS_DATA_BLK *PromDataBlk,
  uint16_t           GppBus,
  uint8_t            GppDev,
  uint8_t            GppFun,
  uint16_t           TempBusNum
  )
{
  uint32_t DidVid = 0;
  uint32_t GpioDidVid = 0;
  uint32_t GppBusData = 0;
  uint32_t UspBusData = 0;
  uint32_t DspBusData = 0;
  uint32_t GppPcieAddress = 0;

  PROM_TRACEPOINT(SIL_TRACE_ENTRY,"    GPP [%04X:%02X:%02X]\n", GppBus, GppDev, GppFun);
  DidVid = xUSLPciRead32(PCI_LIB_ADDRESS(GppBus, GppDev, GppFun, 0x00));
  if (DidVid == 0xFFFFFFFF) {
    PROM_TRACEPOINT(SIL_TRACE_INFO, "    GPP [%04X:%02X:%02X] is not present\n", GppBus, GppDev, GppFun);
    return;
  }

  DidVid = xUSLPciRead32(PCI_LIB_ADDRESS(GppBus, GppDev, GppFun, 0x08));
  if ((DidVid >> 16) != 0x0604) {
    PROM_TRACEPOINT(SIL_TRACE_INFO, "    GPP [%04X:%02X:%02X] is not a PCIE bridge\n",
                    GppBus, GppDev, GppFun);
    return;
  }

  DidVid = xUSLPciRead32(PCI_LIB_ADDRESS(GppBus, GppDev, GppFun, 0x70));
  if ((DidVid & BIT_32(22)) == 0) {
    PROM_TRACEPOINT(SIL_TRACE_INFO, "    GPP [%04X:%02X:%02X] endpoint device is not present\n",
                    GppBus, GppDev, GppFun);
    return;
  }

  GppBusData = xUSLPciRead32(PCI_LIB_ADDRESS(GppBus, GppDev, GppFun, 0x18));
  xUSLPciWrite8(PCI_LIB_ADDRESS(GppBus, GppDev, GppFun, 0x18), (uint8_t)GppBus);
  xUSLPciWrite8(PCI_LIB_ADDRESS(GppBus, GppDev, GppFun, 0x19), (uint8_t)TempBusNum);
  xUSLPciWrite8(PCI_LIB_ADDRESS(GppBus, GppDev, GppFun, 0x1A), (uint8_t)TempBusNum + 7);

  DidVid = xUSLPciRead32(PCI_LIB_ADDRESS(TempBusNum, 0, 0, 0x08));
  if ((DidVid >> 16) != 0x0604) {
    xUSLPciWrite32(PCI_LIB_ADDRESS(GppBus, GppDev, GppFun, 0x18), GppBusData);
    PROM_TRACEPOINT(SIL_TRACE_INFO, "    USP [%04X:%02X:%02X] is not a PCIE bridge\n",
                    TempBusNum, 0, 0);
    return;
  }

  UspBusData = xUSLPciRead32(PCI_LIB_ADDRESS(TempBusNum, 0, 0, 0x18));
  xUSLPciWrite8(PCI_LIB_ADDRESS(TempBusNum, 0, 0, 0x18), (uint8_t)TempBusNum);
  xUSLPciWrite8(PCI_LIB_ADDRESS(TempBusNum, 0, 0, 0x19), (uint8_t)TempBusNum + 1);
  xUSLPciWrite8(PCI_LIB_ADDRESS(TempBusNum, 0, 0, 0x1A), (uint8_t)TempBusNum + 7);

  DidVid = xUSLPciRead32(PCI_LIB_ADDRESS(TempBusNum + 1, PROM21_XHCI_DSP_DEV, 0, 0x08));
  if ((DidVid >> 16) != 0x0604) {
    xUSLPciWrite32(PCI_LIB_ADDRESS(TempBusNum, 0, 0, 0x18), UspBusData);
    xUSLPciWrite32(PCI_LIB_ADDRESS(GppBus, GppDev, GppFun, 0x18), GppBusData);
    PROM_TRACEPOINT(SIL_TRACE_INFO, "    XHCI DSP [%02X:%02X:%02X] is not a PCIE bridge\n",
                    TempBusNum + 1, PROM21_XHCI_DSP_DEV, 0);
    return;
  }

  DspBusData = xUSLPciRead32(PCI_LIB_ADDRESS(TempBusNum + 1, PROM21_XHCI_DSP_DEV, 0, 0x18));
  xUSLPciWrite8(PCI_LIB_ADDRESS(TempBusNum + 1, PROM21_XHCI_DSP_DEV, 0, 0x18),
    (uint8_t)TempBusNum + 1);
  xUSLPciWrite8(PCI_LIB_ADDRESS(TempBusNum + 1, PROM21_XHCI_DSP_DEV, 0, 0x19),
    (uint8_t)TempBusNum + 2);
  xUSLPciWrite8(PCI_LIB_ADDRESS(TempBusNum + 1, PROM21_XHCI_DSP_DEV, 0, 0x1A),
    (uint8_t)TempBusNum + 2);


  DidVid     = xUSLPciRead32(PCI_LIB_ADDRESS(TempBusNum + 2, 0, 0, 0));
  GpioDidVid = xUSLPciRead32(PCI_LIB_ADDRESS(TempBusNum, 0, 0, 0));
  PROM_TRACEPOINT(SIL_TRACE_INFO, "    EndPoint DidVid = 0x%08X\n", DidVid);

  if ((DidVid == PT21_XHCI_ID_L1) || (DidVid == PT21_XHCI_ID_L2) ||
      (DidVid == PT21_XHCI_ID_L3) || (DidVid == PT21_XHCI_ID_L4) ||
      (DidVid == PT21_XHCI_ID_L5) || (DidVid == PT21_XHCI_ID_L6) ||
      (DidVid == PT21_XHCI_ID_L7) || (DidVid == PT21_XHCI_ID_L8) ) {
    GppPcieAddress = PCI_LIB_ADDRESS (GppBus, GppDev, GppFun, 0);
    GppPcieAddress |= ((uint32_t) (GppBus & 0xFF00) << 20);
    PromDataBlk->PromOutputBlk.PT21GpioID = GpioDidVid;
    PromDataBlk->PromOutputBlk.PT21XhciID[0] = DidVid;
    PromDataBlk->PromOutputBlk.PT21GppPcieAddress[0] = GppPcieAddress;
    if (DidVid == PT21_XHCI_ID_L4) {
      if (PromDataBlk->PromInputBlk.PT21PcieTargetSpeed >= 0x4) {
        PromDataBlk->PromInputBlk.PT21PcieTargetSpeed = 0x3;
      }
    }
  }

  xUSLPciWrite32(PCI_LIB_ADDRESS(TempBusNum + 1, PROM21_XHCI_DSP_DEV, 0, 0x18), DspBusData);
  xUSLPciWrite32(PCI_LIB_ADDRESS(TempBusNum, 0, 0, 0x18), UspBusData);
  xUSLPciWrite32(PCI_LIB_ADDRESS(GppBus, GppDev, GppFun, 0x18), GppBusData);

  if (GppPcieAddress != 0) {
    PROM_TRACEPOINT(SIL_TRACE_INFO, "    Found Promontory!\n");
    PROM_TRACEPOINT(SIL_TRACE_INFO, "    Set PT21XhciID        = 0x%08X\n",
                    PromDataBlk->PromOutputBlk.PT21XhciID[0]);
    PROM_TRACEPOINT(SIL_TRACE_INFO, "    Set PT21GpioID        = 0x%08X\n",
                    PromDataBlk->PromOutputBlk.PT21GpioID);
    PROM_TRACEPOINT(SIL_TRACE_INFO, "    Set PT21GppPcieAddress  = 0x%08X\n",
                    PromDataBlk->PromOutputBlk.PT21GppPcieAddress[0]);
  } else {
    PROM_TRACEPOINT(SIL_TRACE_INFO, "    It's not Promontory!\n");
  }

  return;
}

static void
GetIohcBridgeControl (
  PROMCLASS_DATA_BLK *PromDataBlk,
  uint16_t           GppBus,
  uint8_t            GppDev,
  uint8_t            GppFun,
  uint8_t            NbioNum
  )
{
  uint32_t            IohcBridgeCntl = 0;
  uint32_t            Index;

  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "  GPP [%04X:%02X:%02X] NBIO[%d]\n",
                  GppBus, GppDev, GppFun, NbioNum);

  if (SocFamilyIdCheck (AMD_FAMILY_PHX)) {
    static const uint8_t PhxIohcBrightNumbering[17] = {
      0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
      0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
      0x19, 0x1A, 0x1B, 0x1C, 0x21
    };
  
    for (Index = 0; Index < 17; Index++) {
      if (PhxIohcBrightNumbering[Index] == ((GppDev << 3) + GppFun)) {
        PROM_TRACEPOINT(SIL_TRACE_INFO, "    F19_PHX IohcBrightNumbering Index = %d\n", Index);
        break;
      }
    }
    if (Index < 17) {
      if (Index >= 6) {
        Index += 2;
        if (Index >= 14) {
          Index += 2;
          if (Index >= 20) {
            Index += 4;
          }
        }
      }
      Index <<= 10;
      IohcBridgeCntl = Index + IOHCx13b31004;
    }
  }

  if (IohcBridgeCntl > 0) {
    PromDataBlk->PromOutputBlk.PT21IohcBridgeCntl = IohcBridgeCntl;
    PROM_TRACEPOINT(SIL_TRACE_INFO, "    Set PT21IohcBridgeCntl = 0x%08X\n", IohcBridgeCntl);
  }
}

static SIL_STATUS
SearchProm (
  SIL_CONTEXT        *SilContext,
  PROMCLASS_DATA_BLK *PromDataBlk
  )
{
  SIL_STATUS                             Status;
  EARLY_LINK_STATUS                      NbioPtLinkData;
  FABRIC_TARGET                          MmioTarget;
  FABRIC_MMIO_ATTRIBUTE                  MmioAttr;
  PROM_PCI_SAVE_RESTORE_TABLE            PtPciTable[2];
  uint32_t                                 NumberOfSocket;
  uint32_t                                 NumberOfRootBridgeOnDie;
  uint32_t                                 NumberOfDie;
  uint32_t                                 SocketLoop;
  uint32_t                                 DieLoop;
  uint32_t                                 RootBridgeLoop;
  uint32_t                                 BusNumberBase;
  uint32_t                                 PhysicalRootBridgeNumber;
  uint64_t                               MmioSize;
  uint64_t                               MmioBase;
  uint16_t                               GppBus = 0;
  uint8_t                                GppDev = 0;
  uint8_t                                GppFun = 0;
  uint8_t                                NbioNum = 0;
  uint16_t                               TempBusNum;
  uint8_t                                RootBridgeNum = 0;
  uint8_t                                eFuseValue8 = 0;
  uint32_t                               ForceGppPcieAddress;
  uint32_t                               XhciMmio;
  uint32_t                               SecXhciMmio;
  MPIO_COMMON_2_REV_XFER_BLOCK           *MpioXferTable;
  DF_IP2IP_API                           *DfIp2IpApi;
  RCMGR_IP2IP_API                        *RcMgrIp2Ip;

  PROM_TRACEPOINT(SIL_TRACE_ENTRY,"\n");

  if (SilGetCommon2RevXferTable(SilContext, SilId_MpioClass, (void **)(&MpioXferTable)) != SilPass) {
    return SilNotFound;
  }

  if (SilGetIp2IpApi(SilContext, SilId_DfClass, (void **)(&DfIp2IpApi)) != SilPass) {
    return SilNotFound;
  }

  if (SilGetIp2IpApi(SilContext, SilId_RcManager, (void **)(&RcMgrIp2Ip)) != SilPass) {
    return SilNotFound;
  }

  MmioTarget.PciBusNum = 0;
  MmioTarget.PciSegNum = 0;
  MmioTarget.RbNum = 0;
  MmioTarget.SocketNum = 0;
  MmioTarget.TgtType = 0;

  TempBusNum = (uint16_t) PromDataBlk->PromInputBlk.PT21TempBusNum;
  ForceGppPcieAddress = PromDataBlk->PromInputBlk.PT21ForceGppPcieAddress;

  if (ForceGppPcieAddress == 0) {
    Status = MpioXferTable->MpioGetEarlyLinkConfig(SilContext, &NbioPtLinkData);
    if (Status == SilPass) {
      if (NbioPtLinkData.EarlyLinkStatus) {
        NbioNum       = NbioPtLinkData.PhysicalRootBridge;
        RootBridgeNum = NbioPtLinkData.LogicalRootBridge;
        DfIp2IpApi->DfGetSystemInfo(SilContext, &NumberOfSocket, NULL, NULL, NULL, NULL);
        for (SocketLoop = 0; SocketLoop < NumberOfSocket; SocketLoop++) {
          DfIp2IpApi->DfGetProcessorInfo(SilContext, SocketLoop, &NumberOfDie, NULL);
          for (DieLoop = 0; DieLoop < NumberOfDie; DieLoop++) {
            DfIp2IpApi->DfGetDieInfo(SilContext,
              SocketLoop,
              DieLoop,
              &NumberOfRootBridgeOnDie,
              NULL,
              NULL
              );
            for (RootBridgeLoop = 0;
                 RootBridgeLoop < NumberOfRootBridgeOnDie;
                 RootBridgeLoop++) {
              DfIp2IpApi->DfGetRootBridgeInfo(SilContext,
                SocketLoop,
                DieLoop,
                RootBridgeLoop,
                NULL,
                &BusNumberBase,
                NULL,
                &PhysicalRootBridgeNumber,
                NULL,
                NULL
                );
              if (PhysicalRootBridgeNumber == NbioNum) {
                GppBus = (uint16_t)BusNumberBase;
              }
            }
          }
        }
        GppDev        = NbioPtLinkData.RootPortDevice;
        GppFun        = NbioPtLinkData.RootPortFunction;
        TempBusNum    += GppBus;
        PromDataBlk->PromOutputBlk.PT21RootBridgeNum = RootBridgeNum;
        PromDataBlk->PromOutputBlk.PT21IoHcBusNum = (uint8_t)GppBus;
        PROM_TRACEPOINT(SIL_TRACE_INFO, "  [%04X:%02X:%02X]\n", GppBus, GppDev, GppFun);
        PROM_TRACEPOINT(SIL_TRACE_INFO, "  NbioNum = %x, RootBridgeNum = %x, TempBusNum = %x\n",
                        NbioNum, RootBridgeNum, TempBusNum);

        ScanXhciDidVid(PromDataBlk, GppBus, GppDev, GppFun, TempBusNum);
        GetIohcBridgeControl(PromDataBlk, GppBus, GppDev, GppFun, NbioNum);
      }
    }
  } else {
    NbioNum       = (uint8_t)((ForceGppPcieAddress >> 20) & 0xFF);
    RootBridgeNum = (uint8_t)((ForceGppPcieAddress >> 20) & 0xFF);
    GppBus        = (uint16_t)((ForceGppPcieAddress >> 20) & 0xFFF);
    GppDev        = (uint8_t)((ForceGppPcieAddress >> 15) & 0x1F);
    GppFun        = (uint8_t)((ForceGppPcieAddress >> 12) & 0x07);
    TempBusNum    += GppBus;
    PromDataBlk->PromOutputBlk.PT21RootBridgeNum = RootBridgeNum;
    PROM_TRACEPOINT(SIL_TRACE_INFO, "  Force PT21 GPP to [%04X:%02X:%02X]\n",
                    GppBus, GppDev, GppFun);
    PROM_TRACEPOINT(SIL_TRACE_INFO, "  Force PT21 GPP to NbioNum = %x, RootBridgeNum = %x, TempBusNum = %x\n",
                    NbioNum, RootBridgeNum, TempBusNum);

    ScanXhciDidVid(PromDataBlk, GppBus, GppDev, GppFun, TempBusNum);
    GetIohcBridgeControl(PromDataBlk, GppBus, GppDev, GppFun, NbioNum);
  }

  if (PromDataBlk->PromOutputBlk.PT21XhciID[0] == 0) {
    Status = SilDeviceError;
  } else {
    Status = SilPass;
  }

  if (Status == SilPass) {
    MmioTarget.TgtType = TARGET_RB;
    MmioTarget.SocketNum = 0;
    MmioTarget.RbNum = RootBridgeNum;

    MmioSize = 0x200000; // 2MB
    MmioAttr.MmioType = NON_PCI_DEVICE_BELOW_4G;
    Status = RcMgrIp2Ip->FabricReserveMmio(SilContext,
               &MmioBase,
               &MmioSize,
               ALIGN_1M,
               MmioTarget,
               &MmioAttr);
    if (Status != SilPass) {
      PROM_TRACEPOINT(SIL_TRACE_ERROR, "  Failed to allocate DF MMIO for PT XHCI.\n");
      return Status;
    }

    XhciMmio = (uint32_t)MmioBase;
    SecXhciMmio = (uint32_t)(MmioBase + 0x100000);

    PromDataBlk->PromOutputBlk.PT21XhciMmio[0] = XhciMmio;
    PromDataBlk->PromOutputBlk.PT21XhciMmio[1] = SecXhciMmio;

    PROM_TRACEPOINT(SIL_TRACE_INFO, "  Allocate MMIO resources from Fabric 0x%08X Size 0x%08X\n",
                    XhciMmio, MmioSize);

    PromDataBlk->PromOutputBlk.PT21GpioMmio[0] = 0xFEC40000;
    PromDataBlk->PromOutputBlk.PT21GpioMmio[0] = 0xFEC50000;

    PROM_TRACEPOINT(SIL_TRACE_INFO, "  Enable DF MMIO specific decode.\n");
    PROM_TRACEPOINT(SIL_TRACE_INFO, "  XHCI MMIO set, start to read efuse value\n");

    SaveInitPromBus(PromDataBlk, PtPciTable, 0xBB);

    if (PromDataBlk->PromInputBlk.PT21Revision == 0xf) {
      eFuseValue8 = Prom21XhciReadByte (XhciMmio, 0x2E37B);
      if((eFuseValue8 & BIT_8(3)) == BIT_8(3)) {
        PROM_TRACEPOINT(SIL_TRACE_INFO, "  Using A2 now\n");
        PromDataBlk->PromInputBlk.PT21Revision = 0x2;
      } else {
        PROM_TRACEPOINT(SIL_TRACE_INFO, "  Using A0/A1 now\n");
        PromDataBlk->PromInputBlk.PT21Revision = 0x1;
      }
    }

    RestorePromBus(PromDataBlk, PtPciTable, 0xBB);
  }

  PROM_TRACEPOINT(SIL_TRACE_EXIT, "Status = %x\n", Status);
  return Status;
}

/**--------------------------------------------------------------------
 * InitializePromontoryChipset
 *
 * @brief This function initializes the PROM21 IP during timepoint 1 (pre-Pcie phase).
 *
 * @details  This is an IP private function, not visible to the Host.
 *           The PROM Silicon block is initialized by calling this function
 *           in the IP block list.
 *
 * @param   SilContext     A context structure through which host firmware defined data
 *                         can be passed to openSIL. The host firmware is responsible
 *                         for initializing the SIL_CONTEXT structure.
 * @param   PromDataBlk   A pointer to the PROM configuration data.
 * @return SIL_STATUS
 * @retval  SilPass - everything is OK
 * @retval  SilNotFound - Something went wrong
 */
SIL_STATUS
InitializePromontoryChipset (
  SIL_CONTEXT        *SilContext,
  PROMCLASS_DATA_BLK *PromDataBlk
  )
{
  SIL_STATUS                    Status;

  Status = SilPass;
  PROM_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Status = SearchProm(SilContext, PromDataBlk);
  if (Status != SilPass) {
    PROM_TRACEPOINT(SIL_TRACE_ERROR, "  Search for Prom21 Failed!\n");
    return Status;
  }

  if (PromDataBlk->PromInputBlk.PT21Revision < 2) {
    Status = AmdProm21FwLoadA1(SilContext, PromDataBlk);
  } else {
    Status = AmdProm21FwLoad(SilContext, PromDataBlk);
  }

  if (Status != SilPass) {
    PROM_TRACEPOINT(SIL_TRACE_ERROR, "  Prom21 FW Loading Failed!\n");
    return Status;
  }

  if (PromDataBlk->PromOutputBlk.PT21XhciLock[0] == 1) {
    PROM_TRACEPOINT(SIL_TRACE_INFO, "  XHCI is locked == Skip Prom21Setting!\n");
  } else {
    Status = Prom21Setting(SilContext, PromDataBlk);
    if (Status != SilPass) {
      PROM_TRACEPOINT(SIL_TRACE_ERROR, "  Prom21Setting fail!\n");
    }

    Status = Prom21ProgramGpiosInitial(PromDataBlk);
    if (Status != SilPass) {
      PROM_TRACEPOINT(SIL_TRACE_ERROR, "  Prom21ProgramGpiosInitial fail!\n");
    }
  
    Status = Prom21LateSetting (SilContext, PromDataBlk);
    if (Status != SilPass) {
      PROM_TRACEPOINT(SIL_TRACE_ERROR, "  Prom21LateSetting fail!\n");
    }
  }

  PROM_TRACEPOINT(SIL_TRACE_EXIT, "\n");

  return Status;
}
