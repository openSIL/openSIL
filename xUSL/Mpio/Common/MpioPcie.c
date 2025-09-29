/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file MpioPcie.c
 *  @brief Pcie training and hotplug configuration.
 */

#include <string.h>
#include "MpioInitLib.h"
#include "MpioCmn2Rev.h"
#include "MpioPcie.h"
#include "MpioLibLocal.h"
#include <Nbio/NbioIp2Ip.h>

/*----------------------------------------------------------------------------------------*/
/**
 * Map engine to specific PCI device address
 *
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   GnbHandle           Pointer to the Silicon Descriptor for this node
 * @param   PortDevMap          Pointer to PortDevMap
 */
void
PcieConfigureHotplugPorts (
  SIL_CONTEXT           *SilContext,
  PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  MPIO_COMMON_2_REV_XFER_BLOCK  *MpioXferTable;
  NBIO_IP2IP_API                *NbioIp2Ip;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");
  /*
   * Get MPIO Cmn2Rev transfer table
   */
  if (SilGetCommon2RevXferTable(SilContext, SilId_MpioClass, (void **)(&MpioXferTable)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }
  /*
   * Get NBIO Ip2Ip API
   */
  if (SilGetIp2IpApi(SilContext, SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  NbioIp2Ip->PcieConfigRunProcForAllEngines(SilContext,
    DESCRIPTOR_ALLOCATED | DESCRIPTOR_PCIE_ENGINE,
    MpioXferTable->PcieHotplugPreInit,
    NULL,
    Pcie
    );

  NbioIp2Ip->PcieConfigRunProcForAllEngines(SilContext,
    DESCRIPTOR_ALLOCATED | DESCRIPTOR_PCIE_ENGINE,
    MpioXferTable->PcieHotplugInit,
    NULL,
    Pcie
    );
  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Check if engine can be remapped to Device/function number requested by user
 * defined engine descriptor
 *
 *   Function only called if requested device/function does not much native device/function
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   DevFunc             PCI Device(7:3) and Func(2:0)
 * @retval     true                Descriptor can be mapped to engine
 * @retval     false               Descriptor can NOT be mapped to engine
 */
static
bool
PcieCheckPortPciDeviceMapping (
  SIL_CONTEXT     *SilContext,
  uint8_t         DevFunc
  )
{
  uint8_t   Index;
  uint32_t DefaultPortDevMapSize;
  uint8_t *DefaultPortDevMap;
  MPIO_COMMON_2_REV_XFER_BLOCK  *MpioXferTable;
  SIL_STATUS Status;

  Status = SilGetCommon2RevXferTable(SilContext, SilId_MpioClass, (void **)(&MpioXferTable));
  if (Status != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "Unable to get MPIO C2R Table.\n");
    assert(Status == SilPass);
  } else {
    DefaultPortDevMapSize = MpioXferTable->PcieGetDeviceMappingSize ();
    DefaultPortDevMap = MpioXferTable->PcieGetDeviceMapping ();

    for (Index = 0; Index < DefaultPortDevMapSize; Index++) {
      if (DefaultPortDevMap[Index] == DevFunc) {
        return true;
      }
    }
  }
  return false;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Map engine to specific PCI device address
 *
 *
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   Engine              Pointer to engine configuration
 * @param   PortDevMap          Pointer to PortDevMap
 * @retval     SilNotFound         Fail to map PCI device address
 * @retval     SilPass             Successfully allocate PCI address
 */
SIL_STATUS
PcieMapPortPciAddress (
  SIL_CONTEXT            *SilContext,
  PCIe_ENGINE_CONFIG     *Engine,
  uint8_t *PortDevMap
  )
{
  SIL_STATUS                    Status;
  uint8_t                       DevFunc;
  uint8_t                       Index;
  uint8_t                       PortId;
  uint32_t                      DefaultPortDevMapSize;
  uint8_t                       *DefaultPortDevMap;
  MPIO_COMMON_2_REV_XFER_BLOCK  *MpioXferTable;

  if (SilGetCommon2RevXferTable(SilContext, SilId_MpioClass, (void **)(&MpioXferTable)) != SilPass) {
    return SilNotFound;
  }
  DefaultPortDevMapSize = MpioXferTable->PcieGetDeviceMappingSize ();
  DefaultPortDevMap = MpioXferTable->PcieGetDeviceMapping ();

  Status = SilPass;
  PortId = MpioXferTable->MpioGetPortId(Engine);
  if (PortId < DefaultPortDevMapSize) {
    if (Engine->Type.Port.PortData.DeviceNumber == 0 && Engine->Type.Port.PortData.FunctionNumber == 0) {
      Engine->Type.Port.PortData.DeviceNumber = Engine->Type.Port.NativeDevNumber;
      Engine->Type.Port.PortData.FunctionNumber = Engine->Type.Port.NativeFunNumber;
    }

    DevFunc = (Engine->Type.Port.PortData.DeviceNumber << 3) | Engine->Type.Port.PortData.FunctionNumber;
    if (PcieCheckPortPciDeviceMapping(SilContext, DevFunc) != true) {
      Status = SilNotFound;
    } else {
      MPIO_TRACEPOINT(SIL_TRACE_INFO,
        "    Device %x, Function %x\n",
        Engine->Type.Port.PortData.DeviceNumber,
        Engine->Type.Port.PortData.FunctionNumber
        );
      for (Index = 0; Index < DefaultPortDevMapSize; ++Index) {
        if (PortDevMap[Index] == DevFunc) {
          Status = SilNotFound;
          break;
        }
      }
    }
    if (Status == SilPass) {
      PortDevMap[PortId] = DevFunc;
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "    PortDevMap DevFunc 0x%x, for PortId %d\n", DevFunc, PortId);
    }
    for (Index = 0; Index < DefaultPortDevMapSize; ++Index) {
      if (DevFunc == DefaultPortDevMap[Index]) {
        MpioXferTable->SwapLogicalBridgeId(Engine, Index);
        Engine->Type.Port.LogicalBridgeId = Index;
        break;
      }
    }
  }
  return Status;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Sync Early Training Remap configuration
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   GnbHandle           Pointer to the Silicon Descriptor for this node
 * @param   PortDevMap          Pointer to PortDevMap
 * @param   Engine              Pointer to engine configuration
 *
 */
void
SyncEarlyTrainRemap (
  SIL_CONTEXT               *SilContext,
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

  if (SilGetCommon2RevXferTable(SilContext, SilId_MpioClass, (void **)(&MpioXferTable)) != SilPass) {
    return;
  }
  DefaultPortDevMapSize = MpioXferTable->PcieGetDeviceMappingSize ();
  DefaultPortDevMap = MpioXferTable->PcieGetDeviceMapping ();

  // Is Early Training EP device
  if (Engine->InitStatus == INIT_STATUS_PCIE_TRAINING_SUCCESS) {
    for (Index = 0; Index < DefaultPortDevMapSize; Index++) {

      DevFunc = DEVFUNC(Engine->Type.Port.NativeDevNumber, Engine->Type.Port.NativeFunNumber);
      if (DefaultPortDevMap[Index] != DevFunc) {
        continue;
      }
      if (PortDevMap[Index] == 0) {
        if (Engine->Type.Port.PortData.DeviceNumber == 0 && Engine->Type.Port.PortData.FunctionNumber == 0) {
          Engine->Type.Port.PortData.DeviceNumber = Engine->Type.Port.NativeDevNumber;
          Engine->Type.Port.PortData.FunctionNumber = Engine->Type.Port.NativeFunNumber;
        }
        MpioXferTable->SwapLogicalBridgeId(Engine, Index);
        MPIO_TRACEPOINT(SIL_TRACE_INFO,
          " Sync EarlyTrain Dev 0x%x Fun 0x%x LogicalId %d\n",
          Engine->Type.Port.PortData.DeviceNumber,
          Engine->Type.Port.PortData.FunctionNumber,
          Engine->Type.Port.LogicalBridgeId
          );

        PcieConfigSetDescriptorFlags(Engine, DESCRIPTOR_ALLOCATED);

        // Sync Remap Device Function
        DevFnMap = MpioXferTable->PcieGetDevFn(GnbHandle, Index);

        for (SwapIndex = 0; SwapIndex < DefaultPortDevMapSize; SwapIndex++) {
          if (DevFnMap != DefaultPortDevMap[SwapIndex]) {
            continue;
          }
          if (Index == SwapIndex) {
            // Remap unnecessary
            break;
          }
          PortDevMap[Index] = DevFnMap;
          MPIO_TRACEPOINT(SIL_TRACE_INFO, " Sync EarlyTrain Remap PortDevMap[%d] = 0x%X\n", Index, PortDevMap[Index]);
          PortDevMap[SwapIndex] = DevFunc;
          MPIO_TRACEPOINT(SIL_TRACE_INFO, "         Remap PortDevMap[%d] = 0x%X\n", SwapIndex, PortDevMap[SwapIndex]);
          break;
        }
        break;
      } else {
        MPIO_TRACEPOINT(SIL_TRACE_ERROR, " Sync EarlyTrain Remap Conflict\n");
      }
    }
  }
}
