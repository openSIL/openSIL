/**
 *  @file MpioPcie.c
 *  @brief Pcie training and hotplug configuration.
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <string.h>
#include "MpioInitLib.h"
#include "MpioCmn2Rev.h"
#include "MpioPcie.h"
#include "MpioLibLocal.h"
#include <NBIO/NbioIp2Ip.h>

#define BRIDGE_DEVICE_TYPE        6
#define DISPLAY_CONTROLLER_TYPE   3

/**
 * Clean up bridge
 *
 *
 *
 * @param[in]     Endpoint       PCI Address of the bridge
 */
void
MpioCleanUpEarlyBridge (
  PCI_ADDR    Endpoint
  )
{
  uint32_t    Index;
  uint32_t    Value;
  PCI_ADDR    Downstream;

  xUSLPciRead (Endpoint.AddressValue | 0x08, AccessWidth32, &Value);
  if ((Value >> 24) == DISPLAY_CONTROLLER_TYPE) {
    Value = 0;
    xUSLPciWrite (Endpoint.AddressValue | 0x04, AccessWidth32, &Value);
    for (Index = 0x10; Index < 0x28; Index += 4) {
      xUSLPciWrite (Endpoint.AddressValue | Index, AccessWidth32, &Value);
    }
  } else if ((Value >> 24) == BRIDGE_DEVICE_TYPE) {
    xUSLPciRead (Endpoint.AddressValue | 0x18, AccessWidth32, &Value);
    if ((Value & 0x00FFFF00) != 0) {
      Downstream.AddressValue = MAKE_SBDFO (Endpoint.Address.Segment, (Value & 0x0000FF00) >> 8, 0, 0, 0);
      MpioCleanUpEarlyBridge (Downstream);
      Value = 0;
      xUSLPciWrite (Endpoint.AddressValue | 0x1C, AccessWidth16, &Value);
      xUSLPciWrite (Endpoint.AddressValue | 0x20, AccessWidth32, &Value);
      xUSLPciWrite (Endpoint.AddressValue | 0x24, AccessWidth32, &Value);
      xUSLPciWrite (Endpoint.AddressValue | 0x28, AccessWidth32, &Value);
      xUSLPciWrite (Endpoint.AddressValue | 0x2C, AccessWidth32, &Value);
      xUSLPciWrite (Endpoint.AddressValue | 0x30, AccessWidth32, &Value);
      xUSLPciWrite (Endpoint.AddressValue | 0x04, AccessWidth32, &Value);
      xUSLPciRead  (Endpoint.AddressValue | 0x18, AccessWidth32, &Value);
      Value &= 0xFF0000FF;
      xUSLPciWrite (Endpoint.AddressValue | 0x18, AccessWidth32, &Value);
    }
  }
}

//----------------------------------------------------------------//
/**
  Package specific fixups for early trained link

  @param Pcie Pointer to PCIe complex data
**/
void
PcieEarlyTrainFixups (
  MPIOCLASS_INPUT_BLK       *SilData,
  GNB_HANDLE                *GnbHandle,
  MPIO_COMPLEX_DESCRIPTOR   *PcieTopologyData
  )
{
  MPIO_COMMON_2_REV_XFER_BLOCK  *MpioXferTable;

  if (SilGetCommon2RevXferTable (SilId_MpioClass, (void **)(&MpioXferTable)) != SilPass) {
    return;
  }

  if (true == SilData->EarlyBmcLinkTraining) {
    PcieTopologyData->BmcLinkLocation = SilData->EarlyBmcLinkSocket << 2;
    PcieTopologyData->BmcLinkLaneNum = SilData->EarlyBmcLinkLaneNum;

    MpioXferTable->MpioCleanUpEarlyInitSP5 (GnbHandle, SilData->EarlyBmcLinkDie);
  } else {
      PcieTopologyData->BmcLinkLocation = 0xFF;
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Map engine to specific PCI device address
 *
 *
 * @param[in]  GnbHandle           Pointer to the Silicon Descriptor for this node
 * @param[in]  PortDevMap          Pointer to PortDevMap
 */
void
PcieConfigureHotplugPorts (
  PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  MPIO_TRACEPOINT (SIL_TRACE_ENTRY, "\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Check if engine can be remapped to Device/function number requested by user
 * defined engine descriptor
 *
 *   Function only called if requested device/function does not much native device/function
 *
 * @param[in]  DevFunc             PCI Device(7:3) and Func(2:0)
 * @retval     true                Descriptor can be mapped to engine
 * @retval     false               Descriptor can NOT be mapped to engine
 */
static
bool
PcieCheckPortPciDeviceMapping (
  uint8_t         DevFunc
  )
{
  uint8_t   Index;
  uint32_t DefaultPortDevMapSize;
  uint8_t *DefaultPortDevMap;
  MPIO_COMMON_2_REV_XFER_BLOCK  *MpioXferTable;

  SilGetCommon2RevXferTable (SilId_MpioClass, (void **)(&MpioXferTable));
  DefaultPortDevMapSize = MpioXferTable->PcieGetDeviceMappingSize ();
  DefaultPortDevMap = MpioXferTable->PcieGetDeviceMapping ();

  for (Index = 0; Index < DefaultPortDevMapSize; Index++) {
    if (DefaultPortDevMap[Index] == DevFunc) {
      return true;
    }
  }
  return false;
}

static
void
SwapLogicalBridgeId (
  PCIe_ENGINE_CONFIG     *Engine,
  uint8_t                NewBdgIdx
  )
{
  PCIe_WRAPPER_CONFIG     *Wrapper;
  PCIe_WRAPPER_CONFIG     *TempWrapper;
  PCIe_ENGINE_CONFIG      *EngineList;
  NBIO_IP2IP_API          *NbioIp2Ip;

  if (Engine->Type.Port.LogicalBridgeId == NewBdgIdx) {
    return;
  }

  if (SilGetIp2IpApi (SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT (SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  Wrapper = (PCIe_WRAPPER_CONFIG *) NbioIp2Ip->PcieConfigGetParent (DESCRIPTOR_ALL_WRAPPERS, &(Engine->Header));
  EngineList = (PCIe_ENGINE_CONFIG *) NbioIp2Ip->PcieConfigGetChild (DESCRIPTOR_ALL_ENGINES, &(Engine->Header));
  TempWrapper = (PCIe_WRAPPER_CONFIG *) NbioIp2Ip->PcieConfigGetParent (DESCRIPTOR_ALL_WRAPPERS, &(EngineList->Header));
  while ((EngineList != NULL) && (Wrapper == TempWrapper)) {
    if (EngineList->Type.Port.LogicalBridgeId == NewBdgIdx) {
      EngineList->Type.Port.LogicalBridgeId = Engine->Type.Port.LogicalBridgeId;
    }
    EngineList = (PCIe_ENGINE_CONFIG *) PcieConfigGetNextTopologyDescriptor (EngineList, DESCRIPTOR_TERMINATE_TOPOLOGY);
    TempWrapper = (PCIe_WRAPPER_CONFIG *) NbioIp2Ip->PcieConfigGetParent (DESCRIPTOR_ALL_WRAPPERS, &(EngineList->Header));
  }
  Engine->Type.Port.LogicalBridgeId = NewBdgIdx;

  return;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Map engine to specific PCI device address
 *
 *
 *
 * @param[in]  Engine              Pointer to engine configuration
 * @param[in]  PortDevMap          Pointer to PortDevMap
 * @retval     SilNotFound         Fail to map PCI device address
 * @retval     SilPass             Successfully allocate PCI address
 */
SIL_STATUS
PcieMapPortPciAddress (
  PCIe_ENGINE_CONFIG     *Engine,
  uint8_t*               PortDevMap
  )
{
  SIL_STATUS                    Status;
  uint8_t                       DevFunc;
  uint8_t                       Index;
  uint8_t                       PortId;
  uint32_t                      DefaultPortDevMapSize;
  uint8_t                       *DefaultPortDevMap;
  MPIO_COMMON_2_REV_XFER_BLOCK  *MpioXferTable;

  if (SilGetCommon2RevXferTable (SilId_MpioClass, (void **)(&MpioXferTable)) != SilPass) {
    return SilNotFound;
  }
  DefaultPortDevMapSize = MpioXferTable->PcieGetDeviceMappingSize ();
  DefaultPortDevMap = MpioXferTable->PcieGetDeviceMapping ();

  Status = SilPass;
  PortId = Engine->Type.Port.PcieBridgeId;
  if (PortId < DefaultPortDevMapSize) {
    if (Engine->Type.Port.PortData.DeviceNumber == 0 && Engine->Type.Port.PortData.FunctionNumber == 0) {
      Engine->Type.Port.PortData.DeviceNumber = Engine->Type.Port.NativeDevNumber;
      Engine->Type.Port.PortData.FunctionNumber = Engine->Type.Port.NativeFunNumber;
    }

    DevFunc = (Engine->Type.Port.PortData.DeviceNumber << 3) | Engine->Type.Port.PortData.FunctionNumber;
    if (PcieCheckPortPciDeviceMapping (DevFunc) != true) {
      Status = SilNotFound;
    } else {
      MPIO_TRACEPOINT (SIL_TRACE_INFO,"    Device %x, Function %x\n",
        Engine->Type.Port.PortData.DeviceNumber, Engine->Type.Port.PortData.FunctionNumber);
      for (Index = 0; Index < DefaultPortDevMapSize; ++Index) {
        if (PortDevMap[Index] == DevFunc) {
          Status = SilNotFound;
          break;
        }
      }
    }
    if (Status == SilPass) {
      PortDevMap[PortId] = DevFunc;
      MPIO_TRACEPOINT (SIL_TRACE_INFO, "    PortDevMap DevFunc 0x%x, for PortId %d\n", DevFunc, PortId);
    }
    for (Index = 0; Index < DefaultPortDevMapSize; ++Index) {
      if (DevFunc == DefaultPortDevMap[Index]) {
        SwapLogicalBridgeId (Engine, Index);
        break;
      }
    }
  }
  return  Status;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Sync Early Training Remap configuration
 *
 * @param[in]  GnbHandle           Pointer to the Silicon Descriptor for this node
 * @param[in]  PortDevMap          Pointer to PortDevMap
 * @param[in]  Engine              Pointer to engine configuration
 *
 */
void
SyncEarlyTrainRemap (
  GNB_HANDLE                *GnbHandle,
  uint8_t                   *PortDevMap,
  PCIe_ENGINE_CONFIG        *Engine
  )
{
  uint8_t                       Index;
  uint8_t                       SwapIndex;
  uint8_t                       DevFunc;
  uint8_t                       DevFnMap;
  uint32_t                      DefaultPortDevMapSize;
  uint8_t                       *DefaultPortDevMap;
  MPIO_COMMON_2_REV_XFER_BLOCK  *MpioXferTable;

  if (SilGetCommon2RevXferTable (SilId_MpioClass, (void **)(&MpioXferTable)) != SilPass) {
    return;
  }
  DefaultPortDevMapSize = MpioXferTable->PcieGetDeviceMappingSize ();
  DefaultPortDevMap = MpioXferTable->PcieGetDeviceMapping ();


  // Is Early Training EP device
  if (Engine->InitStatus == INIT_STATUS_PCIE_TRAINING_SUCCESS) {
    for (Index = 0; Index < DefaultPortDevMapSize; Index++) {

      DevFunc = DEVFUNC (Engine->Type.Port.NativeDevNumber, Engine->Type.Port.NativeFunNumber);
      if (DefaultPortDevMap[Index] != DevFunc) {
        continue;
      }
      if (PortDevMap[Index] == 0) {
        if (Engine->Type.Port.PortData.DeviceNumber == 0 && Engine->Type.Port.PortData.FunctionNumber == 0) {
          Engine->Type.Port.PortData.DeviceNumber = Engine->Type.Port.NativeDevNumber;
          Engine->Type.Port.PortData.FunctionNumber = Engine->Type.Port.NativeFunNumber;
        }
        SwapLogicalBridgeId (Engine, Index);
        MPIO_TRACEPOINT (SIL_TRACE_INFO, " Sync EarlyTrain Dev 0x%x Fun 0x%x LogicalId %d\n",
        Engine->Type.Port.PortData.DeviceNumber,
        Engine->Type.Port.PortData.FunctionNumber,
        Engine->Type.Port.LogicalBridgeId);

        PcieConfigSetDescriptorFlags (Engine, DESCRIPTOR_ALLOCATED);

        // Sync Remap Device Function
        DevFnMap = MpioXferTable->PcieGetDevFn (GnbHandle, Index);
        for (SwapIndex = 0; SwapIndex < DefaultPortDevMapSize; SwapIndex++) {
          if (DevFnMap != DefaultPortDevMap[SwapIndex]) {
            continue;
          }
          if (Index == SwapIndex) {
            // Remap unnecessary
            break;
          }
          PortDevMap[Index] = DevFnMap;
          MPIO_TRACEPOINT (SIL_TRACE_INFO, " Sync EarlyTrain Remap PortDevMap[%d] = 0x%X\n", Index, PortDevMap[Index]);
          PortDevMap[SwapIndex] = DevFunc;
          MPIO_TRACEPOINT (SIL_TRACE_INFO, "         Remap PortDevMap[%d] = 0x%X\n", SwapIndex, PortDevMap[SwapIndex]);
          break;
        }
        break;
      } else {
        MPIO_TRACEPOINT (SIL_TRACE_ERROR, " Sync EarlyTrain Remap Conflict\n");
      }
    }
  }
}
