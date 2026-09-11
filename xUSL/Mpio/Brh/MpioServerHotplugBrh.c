/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 *  @file MpioServerHotplugBrh.c
 *  @brief Pcie server hotplug configuration.
 *
 */

#include <string.h>
#include <Cxl/Common/CxlCmn2Rev.h>
#include <Mpio/Common/MpioPcie.h>
#include <Mpio/Common/MpioLib.h>
#include <Nbio/Brh/include/PcieCoreReg.h>
#include <Nbio/Brh/include/GlobalRegB0.h>
#include <Nbio/Brh/include/IohcReg.h>
#include <Nbio/Brh/include/Iommul1Reg.h>
#include <Nbio/Brh/include/NbioBaseReg.h>
#include <Nbio/Brh/include/PcieRcCfgReg.h>
#include <Nbio/Brh/include/PcieportReg.h>
#include <Nbio/Brh/GnbRegistersBrh.h>
#include "MpioCmn2Brh.h"

#define HotplugGetNextDescriptor(Descriptor) ((Descriptor == NULL) ? NULL : ((Descriptor->Flags & DESCRIPTOR_TERMINATE_LIST) != 0) ? NULL : (Descriptor + 1))

typedef struct {
  uint32_t       Address;
  uint32_t       SlotNum;
  uint8_t        FunctionMask;
  uint8_t        Format;
} HP_SLOT_INFO;

typedef struct {
  uint8_t numEntries;
  HP_SLOT_INFO entries[65];
} HP_SLOT_INFO_LIST;

typedef struct {
  uint8_t        HotplugFormat;       ///< Hotplug format selected by user
  uint8_t        MaskBit;             ///< Mask bit set by user
  uint32_t       CapMask;             ///< Capability bit to be cleared
} MASK_TO_CAP_TABLE;

MASK_TO_CAP_TABLE MaskToCapTable[] = {
    { HotplugExpressModule,  PCIE_HOTPLUG_EXPRESSMODULE_ATNSW_,
                             SIL_RESERVED_1576 },
    { HotplugExpressModule,  PCIE_HOTPLUG_EXPRESSMODULE_PWREN_,
                             SIL_RESERVED_1590 },
    { HotplugExpressModule,  PCIE_HOTPLUG_EXPRESSMODULE_ATNLED,
                             SIL_RESERVED_1578},
    { HotplugExpressModule,  PCIE_HOTPLUG_EXPRESSMODULE_PWRLED,
                             SIL_RESERVED_1592 },
    { HotplugExpressModuleB, PCIE_HOTPLUG_EXPRESSMODULE_B_ATNSW_,
                             SIL_RESERVED_1576 },
    { HotplugExpressModuleB, PCIE_HOTPLUG_EXPRESSMODULE_B_PWREN_,
                             SIL_RESERVED_1590 },
    { HotplugExpressModuleB, PCIE_HOTPLUG_EXPRESSMODULE_B_ATNLED,
                             SIL_RESERVED_1578},
    { HotplugExpressModuleB, PCIE_HOTPLUG_EXPRESSMODULE_B_PWRLED,
                             SIL_RESERVED_1592 },
};

static SIL_STATUS ResetStatus = SilPass;

/**
 * ServerHotplugFinalInitCallback
 *
 * @brief Callback to init hotplug features on all hotplug ports
 *
 * @param      Engine          Pointer to engine config descriptor
 * @param      Buffer          Not used
 * @param      Pcie            Pointer to global PCIe configuration
 *
 */
static void
ServerHotplugFinalInitCallback (
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  PCIe_WRAPPER_CONFIG   *Wrapper;
  GNB_HANDLE            *GnbHandle;
  uint32_t              SlotNum;
  uint32_t              Address;
  uint32_t              Value;
  uint32_t              Index;
  uint8_t               FunctionMask;
  uint8_t               HotplugFormat;
  HP_SLOT_INFO_LIST     *HplugSlotInfo;
  NBIOCLASS_DATA_BLOCK  *NbioData;
  MPIOCLASS_INPUT_BLK   *SilData;
  NBIO_IP2IP_API        *NbioIp2Ip;
  CXL_COMMON_2_REV_XFER_BLOCK  *CxlXferTable;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  /*
   * Get IP block data
   */
  SilData = (MPIOCLASS_INPUT_BLK *)xUslFindStructure(SilId_MpioClass, 0);
  if (SilData == NULL) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, "Unable to get MPIO Block Data.\n");
    assert(SilData != NULL);
    return;
  }

  NbioData = (NBIOCLASS_DATA_BLOCK *)xUslFindStructure(SilId_NbioClass, 0);
  if (SilData == NULL) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, "Unable to get NBIO Block Data.\n");
    assert(SilData != NULL);
    return;
  }

  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    assert(SilData == SilPass);
    return;
  }

  if ((Engine->Type.Port.PortData.LinkHotplug == PcieHotplugServerExpress ) ||
      (Engine->Type.Port.PortData.LinkHotplug == PcieHotplugServerEntSSD ) ||
      (Engine->Type.Port.PortData.LinkHotplug == PcieHotplugUBM) ||
      (Engine->Type.Port.PortData.LinkHotplug == PcieHotplugOCP )) {
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "Found Server Hotplug Engine at:\n");
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "  Port.PortId = %d\n", Engine->Type.Port.PortId);
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "  Port.PcieBridgeId = %d\n", Engine->Type.Port.PcieBridgeId);
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "  Port.Address = %x\n", Engine->Type.Port.Address);
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "  Type = ");

    Wrapper = (PCIe_WRAPPER_CONFIG *)NbioIp2Ip->PcieConfigGetParent(DESCRIPTOR_ALL_WRAPPERS, &(Engine->Header));
    GnbHandle = (GNB_HANDLE *)NbioIp2Ip->PcieConfigGetParent(DESCRIPTOR_SILICON, &(Engine->Header));

    MpioSmnPrivateRegRMW(GnbHandle,
      WRAP_SPACE(GnbHandle, Wrapper, SIL_RESERVED_1501),
      (uint32_t) ~(0x1),
      0 << 0,
      0
      );

    MPIO_TRACEPOINT(SIL_TRACE_INFO, "  PROG DevNum = %d\n", Engine->Type.Port.PortData.DeviceNumber);
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "  PROG FunNum = %d\n", Engine->Type.Port.PortData.FunctionNumber);
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "  PROG Addr = %x\n", Engine->Type.Port.Address.AddressValue);

    SlotNum = 0;
    FunctionMask = 0;
    HotplugFormat = 0;
    HplugSlotInfo = (HP_SLOT_INFO_LIST *)Buffer;
    for (Index = 0; Index < HplugSlotInfo->numEntries; Index++) {
      if (Engine->Type.Port.Address.AddressValue == HplugSlotInfo->entries[Index].Address) {
        SlotNum = HplugSlotInfo->entries[Index].SlotNum;
        FunctionMask = HplugSlotInfo->entries[Index].FunctionMask;
        HotplugFormat = HplugSlotInfo->entries[Index].Format;
      }
    }
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "  Found SlotNum     = %d\n", SlotNum);
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "        Mask        = 0x%02x\n", FunctionMask);

    // Some of the "hotplug function mask" bits require a Slot Cap bit to be cleared,
    // so read the Slot Cap register ...

    Address = MAKE_SBDFO (0, 0,
                Engine->Type.Port.PortData.DeviceNumber,
                Engine->Type.Port.PortData.FunctionNumber,
                PCICFG_OFFSET (SIL_RESERVED_1630)
                );
    Value = xUSLPciRead32(GnbHandle->Address.AddressValue | Address);

    MPIO_TRACEPOINT(SIL_TRACE_INFO, "  SlotCap (in)      = %x\n", Value);

    // ... and clear any needed bits depending on the hotplug format ...
    for (Index = 0; Index < sizeof (MaskToCapTable) / sizeof (MASK_TO_CAP_TABLE); Index++) {
      if (HotplugFormat == MaskToCapTable[Index].HotplugFormat) {
        if (FunctionMask & MaskToCapTable[Index].MaskBit) {
          Value &= ~(MaskToCapTable[Index].CapMask);
        }
      }
    }

    MPIO_TRACEPOINT(SIL_TRACE_INFO, "  SlotCap (out)     = %x\n", Value);

    // ... and then write tne new value back to the Slot Cap register
    xUSLPciWrite32( GnbHandle->Address.AddressValue | Address, Value);

    if ((Engine->Type.Port.PortData.LinkHotplug == PcieHotplugUBM) ||
        (Engine->Type.Port.PortData.LinkHotplug == PcieHotplugOCP)) {
      SlotNum = Engine->Type.Port.PortData.SlotNum;
    }

    // update cap reg with slot number
    xUSLPciReadModifyWrite32(GnbHandle->Address.AddressValue | Address,
      (uint32_t) ~(SLOT_CAP_PHYSICAL_SLOT_NUM_MASK),
      (uint32_t) (SlotNum << SLOT_CAP_PHYSICAL_SLOT_NUM_OFFSET)
      );

    Address = MAKE_SBDFO (0, 0,
                Engine->Type.Port.PortData.DeviceNumber,
                Engine->Type.Port.PortData.FunctionNumber,
                PCICFG_OFFSET(SIL_RESERVED_1631)
                );

    // if PRESENCE_DETECT is clear, then turn off slot power among things
    Value = xUSLPciRead32(GnbHandle->Address.AddressValue | Address);

    MPIO_TRACEPOINT(SIL_TRACE_INFO, "  SlotCtl           = %x\n", Value);

    if (Value & BIT_32(22)) {
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "Found endpoint -> power on slot\n");
      xUSLPciReadModifyWrite32(GnbHandle->Address.AddressValue | Address,
        (uint32_t) ~(SIL_RESERVED_1576 |
            SIL_RESERVED_1590 |
            SIL_RESERVED_1578 |
            SLOT_CNTL_PWR_INDICATOR_CNTL_MASK |
            SIL_RESERVED_1584),
        (uint32_t) ((1 << SIL_RESERVED_1577) |
            (1 << SIL_RESERVED_1591) |
            (1 << SIL_RESERVED_1579) |
            (1 << SLOT_CNTL_PWR_INDICATOR_CNTL_OFFSET) |
            (1 << SIL_RESERVED_1585) )
        );

      /*
       * Get CXL Cmn2Rev transfer table
       */
      if (SilGetCommon2RevXferTable(SilId_CxlClass, (void **)(&CxlXferTable)) == SilPass) {
        // Trigger warm reset if CXL was not discovered in CxlPortInit
        ResetStatus = CxlXferTable->CxlHotPlugSlotReset(GnbHandle, Engine);
        if (ResetStatus >= SilResetRequestColdImm) {
          return;
        }
      } else {
        MPIO_TRACEPOINT(SIL_TRACE_ERROR, " CXL XFER table is not found.\n");
        assert(false);
      }

    } else {
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "No endpoint -> power off slot\n", Value);
      xUSLPciReadModifyWrite32(GnbHandle->Address.AddressValue | Address,
        (uint32_t) ~(SIL_RESERVED_1576 |
            SLOT_CNTL_PWR_CONTROLLER_CNTL_MASK |
            SIL_RESERVED_1578 |
            SIL_RESERVED_1584),
        (uint32_t) ((1 << SIL_RESERVED_1577) |
            (1 << SLOT_CNTL_PWR_CONTROLLER_CNTL_OFFSET) |
            (1 << SIL_RESERVED_1579) |
            (1 << SIL_RESERVED_1585) )
        );
    }

    // Clear SLOT_CNTL
    Value = xUSLPciRead32(GnbHandle->Address.AddressValue | Address);
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "  SlotCtl (out)     = %x\n", Value);
    xUSLPciWrite32(GnbHandle->Address.AddressValue | Address, Value);

    if (SilData->AmdHotPlugHandlingMode == 5) {
      Address = MAKE_SBDFO (0, 0,
                  Engine->Type.Port.PortData.DeviceNumber,
                  Engine->Type.Port.PortData.FunctionNumber,
                  PCICFG_OFFSET (SIL_RESERVED_1628)
                  );
      // Clear SFI_STATUS
      Value = xUSLPciRead32(GnbHandle->Address.AddressValue | Address);
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "  SFI_STATUS (out)     = %x\n", Value);
      xUSLPciWrite32(GnbHandle->Address.AddressValue | Address, Value);
    }

    MpioSmnPrivateRegRMW(GnbHandle,
      WRAP_SPACE(GnbHandle, Wrapper, SIL_RESERVED_1501),
      (uint32_t) ~(0x1),
      1 << 0,
      0
      );
  }
}

/**
 * ServerHotplugPreInitCallback
 *
 * @brief Callback to init hotplug features on all hotplug ports
 *
 * @param Engine          Pointer to engine config descriptor
 * @param Buffer          Not used
 * @param Pcie            Pointer to global PCIe configuration
 */
static void
ServerHotplugPreInitCallback (
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  PCIe_WRAPPER_CONFIG   *Wrapper;
  GNB_HANDLE            *GnbHandle;
  uint32_t              SlotNum;
  uint32_t              Index;
  uint32_t              Address;
  uint32_t              Value;
  NBIO_IP2IP_API        *NbioIp2Ip;
  HP_SLOT_INFO_LIST     *HplugSlotInfo;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    assert(false);
    return;
  }

  if ((Engine->Type.Port.PortData.LinkHotplug == PcieHotplugServerExpress) ||
      (Engine->Type.Port.PortData.LinkHotplug == PcieHotplugServerEntSSD)) {

    MPIO_TRACEPOINT(SIL_TRACE_INFO, "Found Server Hotplug Engine at:\n");
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "  Port.PortId = %d\n", Engine->Type.Port.PortId);
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "  Port.PcieBridgeId = %d\n", Engine->Type.Port.PcieBridgeId);
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "  Port.Address = %x\n", Engine->Type.Port.Address);

    Wrapper = (PCIe_WRAPPER_CONFIG *)NbioIp2Ip->PcieConfigGetParent(DESCRIPTOR_ALL_WRAPPERS, &(Engine->Header));
    GnbHandle = (GNB_HANDLE *)NbioIp2Ip->PcieConfigGetParent(DESCRIPTOR_SILICON, &(Engine->Header));

    MpioSmnPrivateRegRMW(GnbHandle,
      WRAP_SPACE(GnbHandle, Wrapper, SIL_RESERVED_1501),
      (uint32_t) ~(0x1),
      0 << 0,
      0
      );
  
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "  PROG DevNum = %d\n", Engine->Type.Port.PortData.DeviceNumber);
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "  PROG FunNum = %d\n", Engine->Type.Port.PortData.FunctionNumber);
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "  PROG Addr = %x\n", Engine->Type.Port.Address.AddressValue);

    SlotNum = 0;
    HplugSlotInfo = (HP_SLOT_INFO_LIST *)Buffer;
    for (Index = 0; Index < HplugSlotInfo->numEntries; Index++) {
      if (Engine->Type.Port.Address.AddressValue == HplugSlotInfo->entries[Index].Address) {
        SlotNum = HplugSlotInfo->entries[Index].SlotNum;
      }
    }
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "  Found SlotNum     = %d\n", SlotNum);

    Address = MAKE_SBDFO(0, 0,
      Engine->Type.Port.PortData.DeviceNumber,
      Engine->Type.Port.PortData.FunctionNumber,
      PCICFG_OFFSET (SIL_RESERVED_1630)
      );
    Value = xUSLPciRead32(GnbHandle->Address.AddressValue | Address);
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "  Addr/Val= %x/%x\n", GnbHandle->Address.AddressValue | Address, Value);

    xUSLPciReadModifyWrite32(GnbHandle->Address.AddressValue | Address,
      (uint32_t) ~(SLOT_CAP_PHYSICAL_SLOT_NUM_MASK),
      (uint32_t) (SlotNum << SLOT_CAP_PHYSICAL_SLOT_NUM_OFFSET)
      );

    MpioSmnPrivateRegRMW(GnbHandle,
      PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1486),
      (uint32_t) ~(0xFFFFFFFF),
      (uint32_t) (0x80000000 | SlotNum),
      0
      );
  }
}

/**
 *  MpioServerHotplugInitBrh
 *
 *  @brief Server Hotplug MPIO Initialization
 *
 *  @param  Pcie    PCIe_PLATFORM_CONFIG pointer
 **/
SIL_STATUS
MpioServerHotplugInitBrh (
  PCIe_PLATFORM_CONFIG                 *Pcie
  )
{
  GNB_HANDLE                    *GnbHandle;
  GNB_HANDLE                    *MasterHandle;
  GNB_HANDLE                    *LoopHandle;
  uint32_t                      MpioArg[6];
  uint32_t                      HotplugI2cAddress;
  PcieHotPlugConfigTable_t      HotPlugConfig;
  MPIO_HOTPLUG_DESCRIPTOR       *HotplugUserConfig;
  MPIO_HOTPLUG_DESCRIPTOR       *LastHotplugUserConfig=NULL;
  PCIe_ENGINE_CONFIG            *EngineList;
  PCIe_WRAPPER_CONFIG           *Wrapper;
  uint32_t                      DescriptorIndex;
  bool                          SlotFound;
  uint32_t                      SlotNum;
  bool                          WrapperFound;
  uint32_t                      HotplugSlotIndex;
  uint32_t                      HotplugMode;
  HP_SLOT_INFO_LIST             HplugSlotInfo;
  MPIOCLASS_INPUT_BLK           *SilData;
  NBIOCLASS_DATA_BLOCK          *NbioData;
  NBIO_IP2IP_API                *NbioIp2Ip;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  /*
   * Get IP block data
   */
  SilData = (MPIOCLASS_INPUT_BLK *)xUslFindStructure(SilId_MpioClass, 0);
  if (SilData == NULL) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, "Unable to get MPIO Block Data.\n");
    assert(SilData != NULL);
    return SilNotFound;
  }

  NbioData = (NBIOCLASS_DATA_BLOCK *)xUslFindStructure(SilId_NbioClass, 0);
  if (SilData == NULL) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, "Unable to get NBIO Block Data.\n");
    assert(SilData != NULL);
    return SilNotFound;
  }

  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    assert(SilData == SilPass);
    return SilNotFound;
  }

  memset(&HotPlugConfig, 0, sizeof(PcieHotPlugConfigTable_t));

  MasterHandle = NbioIp2Ip->NbioGetHandle (Pcie);
  Wrapper = NULL;
  EngineList = NULL;

  HplugSlotInfo.numEntries = 0;
  HotplugUserConfig = SilData->PcieTopologyData.HotplugPortList;
  SlotNum = 0;

  while (HotplugUserConfig != NULL) {
    SlotFound = false;
    WrapperFound = false;
    GnbHandle = NbioIp2Ip->NbioGetHandle (Pcie);
    MasterHandle = GnbHandle;
    while (GnbHandle != NULL) {
      if (GnbHandle->RBIndex < 8) {
        if (HotplugUserConfig->Engine.SocketNumber == GnbHandle->SocketId) {
          Wrapper = PcieConfigGetChildWrapper (GnbHandle);
          while (Wrapper != NULL) {
            if ((HotplugUserConfig->Engine.StartLane >= Wrapper->StartPhyLane + GnbHandle->StartLane) &&
                (HotplugUserConfig->Engine.EndLane <= Wrapper->EndPhyLane + GnbHandle->StartLane)) {
              WrapperFound = true;
              break;
            }
            Wrapper = PcieLibGetNextDescriptor (Wrapper);
          }
        }
        if (WrapperFound) {
          break;
        }
      }
      GnbHandle = GnbGetNextHandle (GnbHandle);
    }
    if (WrapperFound == true) {
      EngineList = PcieConfigGetChildEngine (Wrapper);
      while (EngineList != NULL) {
        if ((((HotplugUserConfig->Engine.StartLane == EngineList->EngineData.StartLane) &&
              (HotplugUserConfig->Engine.EndLane == EngineList->EngineData.EndLane)) ||
             ((HotplugUserConfig->Engine.StartLane == EngineList->EngineData.EndLane) &&
              (HotplugUserConfig->Engine.EndLane == EngineList->EngineData.StartLane))) &&
            (EngineList->Type.Port.PortData.LinkHotplug != PcieHotplugDisabled)) {
          SlotFound = true;
          SlotNum = HotplugUserConfig->Engine.SlotNumber;
          HplugSlotInfo.entries[HplugSlotInfo.numEntries].FunctionMask = (uint8_t) HotplugUserConfig->Function.FunctionMask;
          HplugSlotInfo.entries[HplugSlotInfo.numEntries].Format = (uint8_t) HotplugUserConfig->Mapping.HotPlugFormat;
          HplugSlotInfo.entries[HplugSlotInfo.numEntries].SlotNum = SlotNum;
          HplugSlotInfo.entries[HplugSlotInfo.numEntries].Address = EngineList->Type.Port.Address.AddressValue;
          HplugSlotInfo.numEntries++;
          MPIO_TRACEPOINT(SIL_TRACE_INFO, "  SlotNum: %x\n", SlotNum);
          MPIO_TRACEPOINT(SIL_TRACE_INFO, "  Addr: %x\n", EngineList->Type.Port.Address.AddressValue);
          MPIO_TRACEPOINT(SIL_TRACE_INFO, "  Entries: %x\n", HplugSlotInfo.numEntries);
          break;
        }
        EngineList = PcieLibGetNextDescriptor (EngineList);
      }
    }
    if (SlotFound) {
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "Found this slot at StartLane %d and EndLane %d\n", EngineList->EngineData.StartLane, EngineList->EngineData.EndLane);
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "  SlotNum: %x\n", SlotNum);

      GnbHandle = (GNB_HANDLE *) PcieConfigGetParentSilicon (EngineList);
      Wrapper = PcieConfigGetParentWrapper (EngineList);
      DescriptorIndex = SlotNum - SilData->HotplugSlotIndex;
      memcpy((void *)&HotPlugConfig.MappingDescriptor[DescriptorIndex],
             (void *)&HotplugUserConfig->Mapping,
             sizeof (PcieHotPlugMappingDescriptorV3_t));
      memcpy((void *)&HotPlugConfig.FunctionDescriptor[DescriptorIndex],
             (void *)&HotplugUserConfig->Function,
             sizeof (PcieHotPlugFunctionDescriptor_t));
      memcpy((void *)&HotPlugConfig.ResetDescriptor[DescriptorIndex],
             (void *)&HotplugUserConfig->Reset,
             sizeof (PcieHotPlugResetDescriptor_t));
      HotPlugConfig.MappingDescriptor[DescriptorIndex].PcieTileInstance = (Wrapper->WrapId) ?
                                                                            (Wrapper->WrapId << 3) :
                                                                            (GnbHandle->RBIndex);
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "  PcieTileInstance: %x\n", (Wrapper->WrapId) ?
                                                                    (Wrapper->WrapId << 3) :
                                                                    (GnbHandle->RBIndex));
      HotPlugConfig.MappingDescriptor[DescriptorIndex].PciePortInstance = EngineList->Type.Port.PortId;
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "  PciePortInstance: %x\n", EngineList->Type.Port.PortId);
      HotPlugConfig.MappingDescriptor[DescriptorIndex].DieID = GnbHandle->SocketId;
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "  DieID: %x\n", GnbHandle->SocketId);
      HotPlugConfig.MappingDescriptor[DescriptorIndex].BridgeOffset = EngineList->Type.Port.LogicalBridgeId;
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "  LogicalBridgeId: %x\n", EngineList->Type.Port.LogicalBridgeId);
      HotPlugConfig.FunctionDescriptor[DescriptorIndex].i2c_bus_segment2 = EngineList->Type.Port.I2CMuxInfo;
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "  I2CMuxInfo: %x\n", EngineList->Type.Port.I2CMuxInfo);
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "  LinkHotplug: %x\n", EngineList->Type.Port.PortData.LinkHotplug);
    }

    LastHotplugUserConfig = HotplugUserConfig;
    HotplugUserConfig = HotplugGetNextDescriptor (HotplugUserConfig);
  }

  if (!HplugSlotInfo.numEntries) {
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "LinkHotplug Disabled\n");
    return SilPass;
  }

  if (LastHotplugUserConfig->Flags != DESCRIPTOR_TERMINATE_LIST) {
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "Fail: Hotplug Descriptor Table ended on a non-terminated entry!\n");
    assert(false);
  }

  xUslDumpBuffer((void *)&HotPlugConfig, sizeof (PcieHotPlugConfigTable_t), 3);

  memset(MpioArg, 0x00, sizeof (MpioArg));
  HotplugI2cAddress = SilData->HotplugI2cAddress;
  MpioArg[0] = (uint32_t)HotplugI2cAddress;
  MPIO_TRACEPOINT(SIL_TRACE_INFO, "  HotplugI2cAddress: %x\n", HotplugI2cAddress);
  MpioServiceRequestCommon(MasterHandle->Address, BIOS_MPIO_MSG_HOTPLUG_I2C_SWITCH_ADDR_SET, MpioArg, 0);
  if (MpioArg[0] != 1) {
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "BIOS_MPIO_MSG_HOTPLUG_I2C_SWITCH_ADDR_SET Return Status = %x\n", MpioArg[0]);
    return SilInvalidParameter;
  }

  memset(MpioArg, 0x00, sizeof (MpioArg));
  HotplugI2cAddress += 1;
  HotplugI2cAddress |= (uint32_t) 0x100;
  MpioArg[0] = (uint32_t)HotplugI2cAddress;
  MPIO_TRACEPOINT(SIL_TRACE_INFO, "  HotplugI2cAddress: %x\n", HotplugI2cAddress);
  MpioServiceRequestCommon(MasterHandle->Address, BIOS_MPIO_MSG_HOTPLUG_I2C_SWITCH_ADDR_SET, MpioArg, 0);
  if (MpioArg[0] != 1) {
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "BIOS_MPIO_MSG_HOTPLUG_I2C_SWITCH_ADDR_SET Return Status = %x\n", MpioArg[0]);
    return SilInvalidParameter;
  }

  // Pass hotplug descriptor table address to MPIO
  memset(MpioArg, 0x00, sizeof (MpioArg));
  HotplugI2cAddress += 1;
  HotplugI2cAddress &= 0xFF;
  HotplugI2cAddress |= (uint32_t) 0x200;
  MpioArg[0] = (uint32_t)HotplugI2cAddress;
  MPIO_TRACEPOINT(SIL_TRACE_INFO, "  HotplugI2cAddress: %x\n", HotplugI2cAddress);
  MpioServiceRequestCommon(MasterHandle->Address, BIOS_MPIO_MSG_HOTPLUG_I2C_SWITCH_ADDR_SET, MpioArg, 0);
  if (MpioArg[0] != 1) {
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "BIOS_MPIO_MSG_HOTPLUG_I2C_SWITCH_ADDR_SET Return Status = %x\n", MpioArg[0]);
    return SilInvalidParameter;
  }

  memset(MpioArg, 0x00, sizeof (MpioArg));
  HotplugI2cAddress += 1;
  HotplugI2cAddress |= (uint32_t) 0x300;
  MpioArg[0] = (uint32_t)HotplugI2cAddress;
  MPIO_TRACEPOINT(SIL_TRACE_INFO, "  HotplugI2cAddress: %x\n", HotplugI2cAddress);
  MpioServiceRequestCommon(MasterHandle->Address, BIOS_MPIO_MSG_HOTPLUG_I2C_SWITCH_ADDR_SET, MpioArg, 0);
  if (MpioArg[0] != 1) {
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "BIOS_MPIO_MSG_HOTPLUG_I2C_SWITCH_ADDR_SET Return Status = %x\n", MpioArg[0]);
    return SilInvalidParameter;
  }

  // Transfer hotplug descriptor table to MPIO
  LoopHandle = MasterHandle;
  while (LoopHandle != NULL) {
    memset(MpioArg, 0x00, sizeof (MpioArg));
    MpioArg[0] = (uint32_t) (((uint64_t)&HotPlugConfig) & 0xFFFFFFFF);
    MpioArg[1] = (uint32_t) (((uint64_t)&HotPlugConfig) >> 32);
    MpioArg[2] = (uint32_t) sizeof(PcieHotPlugConfigTable_t);
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "Transfer hotplug descriptor table to MPIO\n");
    MpioServiceRequestCommon(LoopHandle->Address, BIOS_MPIO_MSG_HOTPLUG_CONFIG_TABLE_SET, MpioArg, 0);
    if (MpioArg[0] != 1) {
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "BIOS_MPIO_MSG_HOTPLUG_CONFIG_TABLE_SET Return Status = %x\n", MpioArg[0]);
      return SilInvalidParameter;
    }
    LoopHandle = GnbGetNextHandle (LoopHandle);
  }

  NbioIp2Ip->PcieConfigRunProcForAllEngines (
    DESCRIPTOR_ALLOCATED | DESCRIPTOR_ALL_ENGINES,
    ServerHotplugPreInitCallback,
    &HplugSlotInfo,
    Pcie
    );

  {
    uint32_t t1;
    uint32_t t2;
    uint32_t timeout = 1000;
    GnbHandle = NbioIp2Ip->NbioGetHandle(Pcie);
    WrapperFound = false;
    while (GnbHandle != NULL) {
      Wrapper = (PCIe_WRAPPER_CONFIG *) PcieConfigGetChild (DESCRIPTOR_ALL_WRAPPERS, &GnbHandle->Header);
      if (NULL != Wrapper) {
        WrapperFound = true;
        break;  // Terminate the loop when we find a Wrapper;
      }
      GnbHandle = GnbGetNextHandle (GnbHandle);
    }

    if ((GnbHandle != NULL) && !WrapperFound) {
      MpioSmnPrivateRegRead(GnbHandle, WRAP_SPACE(GnbHandle, Wrapper, SIL_RSVD_ADDR_1a380488), &t1);
      t2 = t1;
      while ((t2 - t1) < 100000L) {
        MpioSmnPrivateRegRead(GnbHandle, WRAP_SPACE(GnbHandle, Wrapper, SIL_RSVD_ADDR_1a380488), &t2);
        timeout--;
        if(!timeout)
          break;
      }
    }
  }

  //Set any relevant debug flags
  memset(MpioArg, 0x00, sizeof (MpioArg));
  // Force A0 hot plug mode if AmdHotPlugHandlingMode == 0
  if (SilData->AmdHotPlugHandlingMode == 0) {
    MpioArg[0] = BIOS_MPIO_MSG_Param_HotplugForceA0Mode;
  }
  // To skip sideband presence updates if set
  if (SilData->AmdHotPlugNVMESkipHPStatUpdate) {
    MpioArg[0] |= BIOS_MPIO_MSG_Param_HotplugSkipHpStatUpdate;
  }
  if (MpioArg[0]) {
    MpioServiceRequestCommon(MasterHandle->Address, BIOS_MPIO_MSG_HOTPLUG_FLAGS_SET, MpioArg, 0);
    if (MpioArg[0] != 1) {
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "BIOS_MPIO_MSG_HOTPLUG_FLAGS_SET Return Status = %x\n", MpioArg[0]);
      return SilInvalidParameter;
    }
  }

  // Enable Hotplug Config
  HotplugMode = 0;
  memset(MpioArg, 0x00, sizeof (MpioArg));
  if (SilData->CfgHotplugMode) {
    HotplugMode = BIOS_MPIO_MSG_Param_HotplugPolling;
  }
  if (SilData->AmdHotPlugHandlingMode == 3 || SilData->AmdHotPlugHandlingMode == 6) {
    HotplugMode |= BIOS_MPIO_MSG_Param_HotplugPlatformFirst;
  }
  if (SilData->AmdHotPlugHandlingMode == 5) {
    HotplugMode |= BIOS_MPIO_MSG_Param_HotplugSFI;
  }
  if (SilData->AmdHotplugPortReset == 1) {
    HotplugMode |= BIOS_MPIO_MSG_Param_HotplugPortReset;
  }
  HotplugSlotIndex = SilData->HotplugSlotIndex & 0xF;
  MpioArg[0] = HotplugSlotIndex | HotplugMode |
               (SilData->AmdHotPlugSettleTime << 8) |
               (SilData->AmdHotPlugPDSettle << 16) |
               (SilData->AmdHotPlugDisBridgeDis << 23) |
               ((SilData->AmdHotPlugSettleTimeMultiplier & 0x3) << 17) |
               ((SilData->AmdHotPlugDLPDSyncCount & 0xF) << 19);

  MPIO_TRACEPOINT(SIL_TRACE_INFO, "Enabling hot plug\n");
  MpioServiceRequestCommon(MasterHandle->Address, BIOS_MPIO_MSG_HOTPLUG_ENABLE, MpioArg, 0);
  MPIO_TRACEPOINT(SIL_TRACE_INFO, "Finished enabling hot plug\n");
  if (MpioArg[0] != 1) {
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "BIOS_MPIO_MSG_HOTPLUG_ENABLE Return Status = %d\n", MpioArg[0]);
    return SilInvalidParameter;
  }

  NbioIp2Ip->PcieConfigRunProcForAllEngines (
    DESCRIPTOR_ALLOCATED | DESCRIPTOR_ALL_ENGINES,
    ServerHotplugFinalInitCallback,
    &HplugSlotInfo,
    Pcie
    );

  if (ResetStatus >= SilResetRequestColdImm) {
    return ResetStatus;
  }

  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}
