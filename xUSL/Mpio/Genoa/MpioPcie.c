/**
 *  @file MpioPcie.c
 *  @brief Pcie training and hotplug configuration.
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <string.h>
#include <Mpio/Common/MpioPcie.h>
#include <Mpio/Common/MpioLib.h>
#include <NBIO/NbioIp2Ip.h>
#include <NBIO/IOD/include/PciecoreReg.h>
#include <NBIO/IOD/include/IohcReg.h>
#include <NBIO/IOD/include/NbioBaseReg.h>
#include <NBIO/IOD/GnbRegisters.h>
#include "MpioPcieStraps.h"
#include "MpioCmn2.h"
#include "Mpio.h"

#define MAX_REMAP_COUNT    24

typedef struct {
  GNB_HANDLE        *GnbHandle;
  uint8_t           LogicalBridgeId;
} TRAINED;

static const uint8_t DefaultPortDevMap [] = {
  DEVFUNC (1, 1),
  DEVFUNC (1, 2),
  DEVFUNC (1, 3),
  DEVFUNC (1, 4),
  DEVFUNC (1, 5),
  DEVFUNC (1, 6),
  DEVFUNC (1, 7),
  DEVFUNC (2, 1),
  DEVFUNC (2, 2),
  DEVFUNC (3, 1),
  DEVFUNC (3, 2),
  DEVFUNC (3, 3),
  DEVFUNC (3, 4),
  DEVFUNC (3, 5),
  DEVFUNC (3, 6),
  DEVFUNC (3, 7),
  DEVFUNC (4, 1),
  DEVFUNC (4, 2),
  DEVFUNC (5, 1),
  DEVFUNC (5, 2),
  DEVFUNC (5, 3),
  DEVFUNC (5, 4),
  DEVFUNC (7, 1),
  DEVFUNC (7, 2)
};

/**
 * PcieGetPortStrapIndex
 *
 * @brief Routine to get the PCIe port strap index value
 *
 * @param  Strap        PCIe strap value
 * @param  Port         Port ID on wrapper
 *
 * @return uint16_t     Port strap index
 */
uint16_t
PcieGetPortStrapIndex (
  uint16_t Strap,
  uint16_t Port
  )
{
  uint16_t PortStrapIndex;

  PortStrapIndex = (Strap + (Port * SIL_RESERVED_881));
  return PortStrapIndex;
}

/**
 * PcieGetPortDpcCapabilityStrap
 *
 * @brief   Routine to get the PCIe strap for per port DPC Capability
 *
 * @details The PCIe read/write straps are unique to each program and are
 *          defined in MPIO. The DPC Capability strap is also used by CXL
 *          to disable DPC capability, if CXL is enabled. This function is
 *          called by CXL using MPIO Ip2Ip API (instead of directly accessing
 *          the program specific strap value) to disable DPC when CXL is
 *          enabled.
 *
 * @return  uint16_t  PCIe strap for per port DPC Capability
 */

uint16_t
PcieGetPortDpcCapabilityStrap (void)
{
  return ((uint16_t)SIL_RESERVED_846);
}

/**
 * PcieGetTphSupportStrap
 *
 * @brief   Routine to get the PCIe strap for overriding the TPH support
 *
 * @details The PCIe read/write straps are unique to each program and are
 *          defined in MPIO. The TPH override strap is also used by SDCI
 *          to enable TPH support for the downstream ports. This function is
 *          called by SDCI using MPIO Ip2Ip API (instead of directly accessing
 *          the program specific strap value) to enable TPH support for each
 *          program.
 *
 * @return  uint16_t  PCIe strap for TPH support
 */

uint16_t
PcieGetTphSupportStrap (void)
{
  return ((uint16_t)SIL_RESERVED_893);
}

/**
 * PcieGetDeviceMappingSize
 *
 * @brief Routine to get the size of the device mapping
 *
 * @return uint32_t      Size of the default port map
 */
uint32_t
PcieGetDeviceMappingSize (void)
{
    return ((uint32_t) (sizeof (DefaultPortDevMap) / sizeof (DefaultPortDevMap[0])));
}

/**
 * PcieGetDeviceMapping
 *
 * @brief Routine to get the the device mapping
 *
 * @return uint8_t*      Pointer to the default port map
 */
uint8_t
*PcieGetDeviceMapping (void)
{
    return (uint8_t *)DefaultPortDevMap;
}

/**
 * PcieGetDevFn
 *
 * @brief Routine to get the device Function
 *
 * @param[in]  GnbHandle   Pointer to the Silicon Descriptor for this node
 * @param[in]  Index       Index in the Port Device Map
 *
 * @return uint8_t         The device Function
 */
uint8_t
PcieGetDevFn (
  GNB_HANDLE            *GnbHandle,
  uint8_t               Index
  )
{
  NB_PROG_DEVICE_REMAP_STRUCT   DeviceRemap;

  DeviceRemap.Value = xUSLSmnRead (GnbHandle->Address.Address.Segment,
                        GnbHandle->Address.Address.Bus,
                        IOHC_REMAP_SPACE(GnbHandle, SMN_IOHUB0_N0NBIO0_NB_PROG_DEVICE_REMAP_ADDRESS, Index)
                        );

  return (uint8_t) DeviceRemap.Field.DevFnMap;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Clean up programming from early BMC initialization
 *
 *
 *
 * @param[in]     GnbHandle      Pointer to the Silicon Descriptor for this node
 * @param[in]     DieNumber      InstanceId of the Die that contains the BMC link
 */
void
MpioCleanUpEarlyInitSP5 (
  GNB_HANDLE            *GnbHandle,
  uint8_t               DieNumber
  )
{
  GNB_HANDLE                    *NbioHandle;
  PCIe_ENGINE_CONFIG            *Engine;
  PCIe_WRAPPER_CONFIG           *Wrapper;
  uint32_t                      Value;
  PCI_ADDR                      Endpoint;
  PCIE_LC_STATE10_STRUCT        LcState0;
  TRAINED                       Trained[MAX_REMAP_COUNT];
  uint8_t                       Index;
  NB_PROG_DEVICE_REMAP_STRUCT   DeviceRemap;
  NBIO_IP2IP_API                *NbioIp2Ip;

  MPIO_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  if (SilGetIp2IpApi (SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT (SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  NbioHandle = GnbHandle;
  MPIO_TRACEPOINT (SIL_TRACE_INFO, "Looking for Die Number %d\n", DieNumber);
  // Temporary hack for WAFL BMC
  while (NbioHandle != NULL) {

    Index = 0;
    memset (Trained, 0, sizeof (Trained));

    // Collect each PCIE port which already trained
    Wrapper = (PCIe_WRAPPER_CONFIG *)(NbioIp2Ip->PcieConfigGetChild (DESCRIPTOR_ALL_WRAPPERS, &(NbioHandle->Header)));
    while (Wrapper != NULL) {
      Engine = (PCIe_ENGINE_CONFIG *)(NbioIp2Ip->PcieConfigGetChild(DESCRIPTOR_ALL_ENGINES, &(Wrapper->Header)));
      while (Engine != NULL) {
        LcState0.Value = xUSLSmnRead (NbioHandle->Address.Address.Segment,
                          NbioHandle->Address.Address.Bus,
                          PORT_SPACE (NbioHandle, Wrapper, Engine->Type.Port.PortId,
                          SIL_RESERVED_529)
                          );
        if ((LcState0.Field.LC_PREV_STATE40 > 0xF) && (LcState0.Field.LC_PREV_STATE40 < 0x1C)) {
          Trained[Index].GnbHandle = NbioHandle;
          Trained[Index].LogicalBridgeId = Engine->Type.Port.LogicalBridgeId;
          Index++;
        }
        Engine = PcieLibGetNextDescriptor (Engine);
      }
      Wrapper = PcieLibGetNextDescriptor (Wrapper);
    }

    // Base on LogicalBridgeId to found the Remapped DevFun number
    while(Index) {
      Index--;
      DeviceRemap.Value = 0;
      DeviceRemap.Value = xUSLSmnRead (Trained[Index].GnbHandle->Address.Address.Segment,
                        Trained[Index].GnbHandle->Address.Address.Bus,
                        IOHC_REMAP_SPACE(Trained[Index].GnbHandle, SMN_IOHUB0_N0NBIO0_NB_PROG_DEVICE_REMAP_ADDRESS,
                          Trained[Index].LogicalBridgeId)
                        );
      if (DeviceRemap.Value != 0) {
        Wrapper = (PCIe_WRAPPER_CONFIG *)(NbioIp2Ip->PcieConfigGetChild (DESCRIPTOR_ALL_WRAPPERS, &(NbioHandle->Header)));
        while (Wrapper != NULL) {
          Engine = (PCIe_ENGINE_CONFIG *)(NbioIp2Ip->PcieConfigGetChild (DESCRIPTOR_ALL_ENGINES, &(Wrapper->Header)));
          while (Engine != NULL) {
            if (((Engine->Type.Port.NativeDevNumber << 3) | Engine->Type.Port.NativeFunNumber) ==
                  (uint8_t)DeviceRemap.Field.DevFnMap) {
              MPIO_TRACEPOINT (SIL_TRACE_INFO, "    Engine Dev=%d Dev=%d LogicalBridgeId=%d\n",
                Engine->Type.Port.NativeDevNumber, Engine->Type.Port.NativeFunNumber, Trained[Index].LogicalBridgeId);
              Engine->InitStatus = INIT_STATUS_PCIE_TRAINING_SUCCESS;
            }
            Engine = PcieLibGetNextDescriptor (Engine);
          }
          Wrapper = PcieLibGetNextDescriptor (Wrapper);
        }
      }
    }
    NbioHandle = GnbGetNextHandle (NbioHandle);
  }

  NbioHandle = GnbHandle;
  // End of hack
  MPIO_TRACEPOINT (SIL_TRACE_INFO, "Looking for NbioInstanceId %d\n", DieNumber);
  while (NbioHandle != NULL) {
    if (NbioHandle->RBIndex == DieNumber) {
      Engine = (PCIe_ENGINE_CONFIG *) (NbioIp2Ip->PcieConfigGetChild (DESCRIPTOR_ALL_ENGINES, &(NbioHandle->Header)));
      while (Engine != NULL) {
        xUSLPciRead (MAKE_SBDFO (NbioHandle->Address.Address.Segment, NbioHandle->Address.Address.Bus,
          Engine->Type.Port.NativeDevNumber, Engine->Type.Port.NativeFunNumber, 0x18), AccessWidth32, &Value);
        if ((Value & 0x00FFFF00) != 0) {
          Endpoint.AddressValue = MAKE_SBDFO (NbioHandle->Address.Address.Segment, (Value & 0x0000FF00) >> 8, 0, 0, 0);
          MpioCleanUpEarlyBridge (Endpoint);
          Value = 0;
          xUSLPciWrite (MAKE_SBDFO (NbioHandle->Address.Address.Segment,
                                      NbioHandle->Address.Address.Bus,
                                      Engine->Type.Port.NativeDevNumber,
                                      Engine->Type.Port.NativeFunNumber,
                                      0x1C),
                          AccessWidth16,
                          &Value
                          );
          xUSLPciWrite (MAKE_SBDFO (NbioHandle->Address.Address.Segment,
                                      NbioHandle->Address.Address.Bus,
                                      Engine->Type.Port.NativeDevNumber,
                                      Engine->Type.Port.NativeFunNumber,
                                      0x20),
                          AccessWidth16,
                          &Value
                          );
          xUSLPciWrite (MAKE_SBDFO (NbioHandle->Address.Address.Segment,
                                      NbioHandle->Address.Address.Bus,
                                      Engine->Type.Port.NativeDevNumber,
                                      Engine->Type.Port.NativeFunNumber,
                                      0x24),
                          AccessWidth16,
                          &Value
                          );
          xUSLPciWrite (MAKE_SBDFO (NbioHandle->Address.Address.Segment,
                                      NbioHandle->Address.Address.Bus,
                                      Engine->Type.Port.NativeDevNumber,
                                      Engine->Type.Port.NativeFunNumber,
                                      0x28),
                          AccessWidth16,
                          &Value
                          );
          xUSLPciWrite (MAKE_SBDFO (NbioHandle->Address.Address.Segment,
                                      NbioHandle->Address.Address.Bus,
                                      Engine->Type.Port.NativeDevNumber,
                                      Engine->Type.Port.NativeFunNumber,
                                      0x2C),
                          AccessWidth16,
                          &Value
                          );
          xUSLPciWrite (MAKE_SBDFO (NbioHandle->Address.Address.Segment,
                                      NbioHandle->Address.Address.Bus,
                                      Engine->Type.Port.NativeDevNumber,
                                      Engine->Type.Port.NativeFunNumber,
                                      0x30),
                          AccessWidth16,
                          &Value
                          );
          xUSLPciWrite (MAKE_SBDFO (NbioHandle->Address.Address.Segment,
                                      NbioHandle->Address.Address.Bus,
                                      Engine->Type.Port.NativeDevNumber,
                                      Engine->Type.Port.NativeFunNumber,
                                      0x04),
                          AccessWidth16,
                          &Value
                          );
          xUSLPciRead (MAKE_SBDFO (NbioHandle->Address.Address.Segment,
                                     NbioHandle->Address.Address.Bus,
                                     Engine->Type.Port.NativeDevNumber,
                                     Engine->Type.Port.NativeFunNumber,
                                     0x18),
                          AccessWidth16,
                          &Value
                          );
          Value &= 0xFF0000FF;
          xUSLPciWrite (MAKE_SBDFO (NbioHandle->Address.Address.Segment,
                                      NbioHandle->Address.Address.Bus,
                                      Engine->Type.Port.NativeDevNumber,
                                      Engine->Type.Port.NativeFunNumber,
                                      0x18),
                          AccessWidth16,
                          &Value
                          );
        }
        Engine = (PCIe_ENGINE_CONFIG *) PcieConfigGetNextTopologyDescriptor (Engine, DESCRIPTOR_TERMINATE_GNB);
      }
    }
    NbioHandle = GnbGetNextHandle(NbioHandle);
  }
  MPIO_TRACEPOINT (SIL_TRACE_EXIT, "\n");
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
PcieSetPortPciAddressMap (
  GNB_HANDLE    *GnbHandle,
  uint8_t       *PortDevMap
  )
{
  uint8_t   Index;
  uint8_t   DevFuncIndex;
  uint8_t   PortDevMapLocal [sizeof (DefaultPortDevMap)];
  MPIO_PROG_DEVICE_REMAP_STRUCT   DeviceRemap;

  memcpy ((void*) PortDevMapLocal, (void*) DefaultPortDevMap, sizeof (DefaultPortDevMap));
  for (Index = 0; Index < sizeof (DefaultPortDevMap); ++Index) {
    if (PortDevMap[Index] != 0) {
      for (DevFuncIndex = 0; DevFuncIndex < sizeof (DefaultPortDevMap); ++DevFuncIndex) {
        if (PortDevMapLocal[DevFuncIndex] == PortDevMap[Index]) {
          PortDevMapLocal[DevFuncIndex] = 0;
          break;
        }
      }
    }
  }
  for (Index = 0; Index < sizeof (DefaultPortDevMap); ++Index) {
    if (PortDevMap[Index] == 0) {
      for (DevFuncIndex = 0; DevFuncIndex < sizeof (DefaultPortDevMap); ++DevFuncIndex) {
        if (PortDevMapLocal[DevFuncIndex] != 0) {
          PortDevMap[Index] = PortDevMapLocal[DevFuncIndex];
          PortDevMapLocal[DevFuncIndex] = 0;
          break;
        }
      }
    }
    DeviceRemap.Value = xUSLSmnRead (GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus,
      IOHC_REMAP_SPACE(GnbHandle, SMN_IOHUB0_N0NBIO0_NB_PROG_DEVICE_REMAP_ADDRESS, Index));
    DeviceRemap.Field.DevFnMap = PortDevMap[Index];
    xUSLSmnWrite (GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus,
      IOHC_REMAP_SPACE(GnbHandle, SMN_IOHUB0_N0NBIO0_NB_PROG_DEVICE_REMAP_ADDRESS, Index), DeviceRemap.Value);
  }
}

/**
 * WritePcieStrap
 *
 * @brief Routine to write pcie soft straps
 *
 * @param  GnbHandle         The associated Gnb Handle
 * @param  StrapIndex        Strap index
 * @param  Value             Contains value of strap register to write with
 * @param  Wrapper           Pcie wrapper number
 */
void
WritePcieStrap (
     GNB_HANDLE     *GnbHandle,
     uint16_t       StrapIndex,
     uint32_t       Value,
     uint8_t        Wrapper
  )
{
  uint32_t  Response;
  uint32_t  MpioArg[6];
  uint8_t   InstanceNumber;
  SIL_STATUS      Status;
  CORE_LOGICAL_ID LogicalId;
  uint16_t          StrapIndexAdjusted;

  // Adjust strap index to account for 2 additional offsets in Genoa B0 and later
  StrapIndexAdjusted = StrapIndex;
  Status = GetCoreLogicalIdOnCurrentCore ((CORE_LOGICAL_ID*)&LogicalId);
  if (Status == SilPass) {
    if (((LogicalId.CoreFamily & AMD_FAMILY_GENOA) != 0) && (LogicalId.CoreRevision & (AMD_REV_F19_GENOA_AX))) {
      if (StrapIndex >= 0xA8) {
        StrapIndexAdjusted -= 2;
      }
    }
  }

  InstanceNumber = GnbHandle->RBIndex + Wrapper * 4; /// RBIndex is the NBIO #
  if (InstanceNumber > 9) {
    InstanceNumber = 9;
  }

  memset (MpioArg, 0x00, sizeof(MpioArg));
  MpioArg[0] = (uint32_t) StrapIndexAdjusted + ((uint32_t) InstanceNumber << 16);
  MpioArg[1] = Value;

  Response = MpioServiceRequestCommon (NbioGetHostPciAddress (GnbHandle), MPIO_MSG_PCIE_WRITE_STRAP, MpioArg, 0);
  MPIO_TRACEPOINT (SIL_TRACE_INFO, "  MPIO Response = 0x%x\n", Response);
}
