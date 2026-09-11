/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 *  @file MpioPcieBrh.c
 *  @brief Pcie training and hotplug configuration.
 *
 */

#include <string.h>
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
#include "MpioPcieStrapsBrh.h"
#include "MpioCmn2Brh.h"
#include "MpioIp2IpBrh.h"

#define MAX_REMAP_COUNT    24

typedef struct {
  GNB_HANDLE        *GnbHandle;
  uint8_t           LogicalBridgeId;
} TRAINED;

static const uint8_t DefaultPortDevMap [] = {
  DEVFUNC(1, 1),
  DEVFUNC(1, 2),
  DEVFUNC(1, 3),
  DEVFUNC(1, 4),
  DEVFUNC(1, 5),
  DEVFUNC(1, 6),
  DEVFUNC(1, 7),
  DEVFUNC(2, 1),
  DEVFUNC(2, 2),
  DEVFUNC(3, 1),
  DEVFUNC(3, 2),
  DEVFUNC(3, 3),
  DEVFUNC(3, 4),
  DEVFUNC(3, 5),
  DEVFUNC(3, 6),
  DEVFUNC(3, 7),
  DEVFUNC(4, 1)
};

/**-----------------------------------------------------------------------------------
 * PcieGetPortStrapIndexBrh
 *
 * @brief Routine to get the PCIe port strap index value
 *
 * @param  Strap        PCIe strap value
 * @param  Port         Port ID on wrapper
 *
 * @return uint16_t     Port strap index
 *
 */
uint16_t
PcieGetPortStrapIndexBrh (
  uint16_t Strap,
  uint16_t Port
  )
{
  uint16_t PortStrapIndex;

  PortStrapIndex = (Strap + (Port * STRAP_BIF_PORT_DIFF));
  return PortStrapIndex;
}

/**-----------------------------------------------------------------------------------
 * PcieGetPortDpcCapabilityStrapBrh
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
 *
 */
uint16_t
PcieGetPortDpcCapabilityStrapBrh (void)
{
  return ((uint16_t) SIL_RESERVED_0423);
}

/**-----------------------------------------------------------------------------------
 * PcieGetTphSupportStrapBrh
 *
 * @brief   Routine to get the PCIe strap for overriding the TPH support
 *
 * @details The PCIe read/write straps are unique to each program and are
 *          defined in MPIO. The TPH override strap is also used by SDXI
 *          to enable TPH support for the downstream ports. This function is
 *          called by SDXI using MPIO Ip2Ip API (instead of directly accessing
 *          the program specific strap value) to enable TPH support for each
 *          program.
 *
 * @return  uint16_t  PCIe strap for TPH support
 *
 */
uint16_t
PcieGetTphSupportStrapBrh (void)
{
  return ((uint16_t) SIL_RESERVED_0476);
}

/**-----------------------------------------------------------------------------------
 * PcieGetMembar0SizeStrapBrh
 *
 * @brief   Routine to get the PCIe strap for the size of MEMBAR0 for CXL
 *
 * @details The PCIe read/write straps are unique to each program and are
 *          defined in MPIO. The MEMBAR0 size strap is also used by CXL
 *          to control the MEMBAR0 size. This function is called by CXL
 *          using MPIO Ip2Ip API (instead of directly accessing
 *          the program specific strap value) to get the value of MEMBAR0
 *          when CXL is enabled.
 *
 * @return  uint16_t  PCIe strap for the size of MEMBAR0
 *
 */
uint16_t
PcieGetMembar0SizeStrapBrh (void)
{
  return ((uint16_t) SIL_RESERVED_0455);
}

/**-----------------------------------------------------------------------------------
 * PcieGetCxlModeStrapBrh
 *
 * @brief   Routine to get the PCIe strap for CXL mode
 *
 * @details The PCIe read/write straps are unique to each program and are
 *          defined in MPIO. The CXL mode strap is also used by CXL.
 *          This function is called by CXL using MPIO Ip2Ip API (instead
 *          of directly accessing the program specific strap value)
 *          to get the value of CXL mode when CXL is enabled.
 *
 * @return  uint16_t  PCIe strap for CXL mode
 *
 */
uint16_t
PcieGetCxlModeStrapBrh (void)
{
  return ((uint16_t) SIL_RESERVED_0417);
}

/**-----------------------------------------------------------------------------------
 * PcieGetDeviceMappingSizeBrh
 *
 * @brief Routine to get the size of the device mapping
 *
 * @return uint32_t      Size of the default port map
 *
 */
uint32_t
PcieGetDeviceMappingSizeBrh (void)
{
  return ((uint32_t) (sizeof (DefaultPortDevMap) / sizeof (DefaultPortDevMap[0])));
}

/**-----------------------------------------------------------------------------------
 * PcieGetDeviceMappingBrh
 *
 * @brief Routine to get the the device mapping
 *
 * @return uint8_t*      Pointer to the default port map
 *
 */
uint8_t
*PcieGetDeviceMappingBrh (void)
{
  return (uint8_t *) DefaultPortDevMap;
}

/**-----------------------------------------------------------------------------------
 * PcieGetDevFnBrh
 *
 * @brief Routine to get the device Function
 *
 * @param[in]  GnbHandle   Pointer to the Silicon Descriptor for this node
 * @param[in]  Index       Index in the Port Device Map
 *
 * @return uint8_t         The device Function
 *
 */
uint8_t
PcieGetDevFnBrh (
  GNB_HANDLE            *GnbHandle,
  uint8_t               Index
  )
{
  NB_PROG_DEVICE_REMAP_STRUCT   DeviceRemap;

  DeviceRemap.Value = xUSLSmnRead(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    IOHC_REMAP_SPACE(GnbHandle, SMN_IOHUB0_N0NBIO0_NB_PROG_DEVICE_REMAP_ADDRESS, Index)
    );

  return (uint8_t) DeviceRemap.Field.DevFnMap;
}

/**-----------------------------------------------------------------------------------
 * Clean up programming from early BMC initialization
 *
 *
 *
 * @param[in]     GnbHandle      Pointer to the Silicon Descriptor for this node
 * @param[in]     DieNumber      InstanceId of the Die that contains the BMC link
 *
 * @returns Nothing
 *
 */
void
MpioCleanUpEarlyInitSP5Brh (
  GNB_HANDLE            *GnbHandle,
  uint8_t               DieNumber
  )
{
  GNB_HANDLE                    *NbioHandle;
  PCIe_ENGINE_CONFIG            *Engine;
  PCIe_WRAPPER_CONFIG           *Wrapper;
  uint32_t                      Value;
  PCI_ADDR                      Endpoint;
  SIL_RESERVED_UNION_0033         LcState0;
  TRAINED                       Trained[MAX_REMAP_COUNT];
  uint8_t                       Index;
  NB_PROG_DEVICE_REMAP_STRUCT   DeviceRemap;
  NBIO_IP2IP_API                *NbioIp2Ip;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  NbioHandle = GnbHandle;
  MPIO_TRACEPOINT(SIL_TRACE_INFO, "Looking for Die Number %d\n", DieNumber);

  // Temporary hack for WAFL BMC
  while (NbioHandle != NULL) {
    Index = 0;
    memset(Trained, 0, sizeof (Trained));

    // Collect each PCIE port which already trained
    Wrapper = (PCIe_WRAPPER_CONFIG *)(NbioIp2Ip->PcieConfigGetChild(DESCRIPTOR_ALL_WRAPPERS, &(NbioHandle->Header)));
    while (Wrapper != NULL) {
      Engine = (PCIe_ENGINE_CONFIG *)(NbioIp2Ip->PcieConfigGetChild(DESCRIPTOR_ALL_ENGINES, &(Wrapper->Header)));

      while (Engine != NULL) {
        MpioSmnPrivateRegRead(NbioHandle,
          PORT_SPACE(NbioHandle,
          Wrapper,
          Engine->Type.Port.PortId,
          SIL_RESERVED_1484
          ),
          &LcState0.Value
          );

        if ((LcState0.Field.field_bits_0_to_5 > 0xF) && (LcState0.Field.field_bits_0_to_5 < 0x1C)) {
          //Mark the core as early configured since the links have been early trained
          if (!Wrapper->IsEarlyConfigured) {
            Wrapper->IsEarlyConfigured = 1;
          }
          //This logic applies only for Wrapid 1 where BMC will be present
          MPIO_TRACEPOINT(SIL_TRACE_INFO, "Wrapper->WrapId %d\n", Wrapper->WrapId);
          if (Wrapper->WrapId) {
            Trained[Index].GnbHandle = NbioHandle;
            Trained[Index].LogicalBridgeId = Engine->Type.Port.LogicalBridgeId;
            Index++;
          }
        }

        Engine = PcieLibGetNextDescriptor(Engine);
      }
      Wrapper = PcieLibGetNextDescriptor(Wrapper);
    }

    MPIO_TRACEPOINT(SIL_TRACE_INFO, "Index %d\n", Index);
    // Base on LogicalBridgeId to found the Remapped DevFun number
    while (Index) {
      Index--;
      DeviceRemap.Value = 0;
      DeviceRemap.Value = xUSLSmnRead(Trained[Index].GnbHandle->Address.Address.Segment,
        Trained[Index].GnbHandle->Address.Address.Bus,
        IOHC_REMAP_SPACE(Trained[Index].GnbHandle,
        SMN_IOHUB0_N0NBIO0_NB_PROG_DEVICE_REMAP_ADDRESS,
        Trained[Index].LogicalBridgeId
        )
        );

      MPIO_TRACEPOINT(SIL_TRACE_INFO, "DeviceRemap.Value %d\n", DeviceRemap.Value);
      if (DeviceRemap.Value != 0) {
        Wrapper = (PCIe_WRAPPER_CONFIG *)(NbioIp2Ip->PcieConfigGetChild(DESCRIPTOR_ALL_WRAPPERS,
          &(NbioHandle->Header)
          ));
        while (Wrapper != NULL) {
          //This logic should run only for WrapId 1 where BMC will be present
          if (Wrapper->WrapId == 0) {
            Wrapper = PcieLibGetNextDescriptor(Wrapper);
            continue;
          }
          Engine = (PCIe_ENGINE_CONFIG *)(NbioIp2Ip->PcieConfigGetChild(DESCRIPTOR_ALL_ENGINES, &(Wrapper->Header)));

          MPIO_TRACEPOINT(SIL_TRACE_INFO, "Engine->Type.Port.NativeDevNumber %d\n", Engine->Type.Port.NativeDevNumber);
          MPIO_TRACEPOINT(SIL_TRACE_INFO, "DeviceRemap.Field.DevFnMap %d\n", DeviceRemap.Field.DevFnMap);
          while (Engine != NULL) {
            if (((Engine->Type.Port.NativeDevNumber << 3) |
              Engine->Type.Port.NativeFunNumber) == (uint8_t)DeviceRemap.Field.DevFnMap) {
              MPIO_TRACEPOINT(SIL_TRACE_INFO,
                "    Engine Dev=%d Dev=%d LogicalBridgeId=%d\n",
                Engine->Type.Port.NativeDevNumber,
                Engine->Type.Port.NativeFunNumber,
                Trained[Index].LogicalBridgeId
                );
              Engine->InitStatus = INIT_STATUS_PCIE_TRAINING_SUCCESS;
            }
            Engine = PcieLibGetNextDescriptor(Engine);
          }
          Wrapper = PcieLibGetNextDescriptor(Wrapper);
        }
      }
    }
    NbioHandle = GnbGetNextHandle(NbioHandle);
  }
  // End of hack

  NbioHandle = GnbHandle;

  while (NbioHandle != NULL) {
    Wrapper = (PCIe_WRAPPER_CONFIG *)(NbioIp2Ip->PcieConfigGetChild(DESCRIPTOR_ALL_WRAPPERS,
      &(NbioHandle->Header)
      ));
    while (Wrapper != NULL) {
      Engine = (PCIe_ENGINE_CONFIG *) (NbioIp2Ip->PcieConfigGetChild(DESCRIPTOR_ALL_ENGINES, &(Wrapper->Header)));

      while (Engine != NULL) {
        if (Engine->InitStatus == INIT_STATUS_PCIE_TRAINING_SUCCESS) {
          MPIO_TRACEPOINT(SIL_TRACE_INFO,
            "Trained engine found at Socket %d, Die %d, RB %d\n",
            NbioHandle->SocketId,
            NbioHandle->DieNumber,
            NbioHandle->RBIndex
            );
          xUSLPciRead(MAKE_SBDFO(NbioHandle->Address.Address.Segment,
            NbioHandle->Address.Address.Bus,
            Engine->Type.Port.NativeDevNumber,
            Engine->Type.Port.NativeFunNumber,
            0x18
            ),
            AccessWidth32,
            &Value
            );

          if ((Value & 0x00FFFF00) != 0) {
            Endpoint.AddressValue = MAKE_SBDFO(NbioHandle->Address.Address.Segment,
              (Value & 0x0000FF00) >> 8,
              0,
              0,
              0
              );
            MpioCleanUpEarlyBridge(Endpoint);
            Value = 0;

            xUSLPciWrite(MAKE_SBDFO(NbioHandle->Address.Address.Segment,
              NbioHandle->Address.Address.Bus,
              Engine->Type.Port.NativeDevNumber,
              Engine->Type.Port.NativeFunNumber,
              0x1C
              ),
              AccessWidth16,
              &Value
              );

            xUSLPciWrite(MAKE_SBDFO(NbioHandle->Address.Address.Segment,
              NbioHandle->Address.Address.Bus,
              Engine->Type.Port.NativeDevNumber,
              Engine->Type.Port.NativeFunNumber,
              0x20
              ),
              AccessWidth16,
              &Value
              );

            xUSLPciWrite(MAKE_SBDFO(NbioHandle->Address.Address.Segment,
              NbioHandle->Address.Address.Bus,
              Engine->Type.Port.NativeDevNumber,
              Engine->Type.Port.NativeFunNumber,
              0x24
              ),
              AccessWidth16,
              &Value
              );

            xUSLPciWrite(MAKE_SBDFO(NbioHandle->Address.Address.Segment,
              NbioHandle->Address.Address.Bus,
              Engine->Type.Port.NativeDevNumber,
              Engine->Type.Port.NativeFunNumber,
              0x28
              ),
              AccessWidth16,
              &Value
              );

            xUSLPciWrite(MAKE_SBDFO(NbioHandle->Address.Address.Segment,
              NbioHandle->Address.Address.Bus,
              Engine->Type.Port.NativeDevNumber,
              Engine->Type.Port.NativeFunNumber,
              0x2C
              ),
              AccessWidth16,
              &Value
              );

            xUSLPciWrite(MAKE_SBDFO(NbioHandle->Address.Address.Segment,
              NbioHandle->Address.Address.Bus,
              Engine->Type.Port.NativeDevNumber,
              Engine->Type.Port.NativeFunNumber,
              0x30
              ),
              AccessWidth16,
              &Value
              );

            xUSLPciWrite(MAKE_SBDFO(NbioHandle->Address.Address.Segment,
              NbioHandle->Address.Address.Bus,
              Engine->Type.Port.NativeDevNumber,
              Engine->Type.Port.NativeFunNumber,
              0x04
              ),
              AccessWidth16,
              &Value
              );

            xUSLPciRead(MAKE_SBDFO(NbioHandle->Address.Address.Segment,
              NbioHandle->Address.Address.Bus,
              Engine->Type.Port.NativeDevNumber,
              Engine->Type.Port.NativeFunNumber,
              0x18
              ),
              AccessWidth16,
              &Value
              );

            Value &= 0xFF0000FF;

            xUSLPciWrite(MAKE_SBDFO(NbioHandle->Address.Address.Segment,
              NbioHandle->Address.Address.Bus,
              Engine->Type.Port.NativeDevNumber,
              Engine->Type.Port.NativeFunNumber,
              0x18
              ),
              AccessWidth16,
              &Value
              );
          }
        }
        Engine = PcieLibGetNextDescriptor(Engine);
      }
      Wrapper = PcieLibGetNextDescriptor(Wrapper);
    }
    NbioHandle = GnbGetNextHandle(NbioHandle);
  }
  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");

  return;
}

/**----------------------------------------------------------------------------------------
 * Map engine to specific PCI device address
 *
 * CHCECK
 * @param[in]  GnbHandle           Pointer to the Silicon Descriptor for this node
 * @param[in]  PortDevMap          Pointer to PortDevMap
 *
 * @returns Nothing
 *
 */
void
PcieSetPortPciAddressMapBrh (
  GNB_HANDLE    *GnbHandle,
  uint8_t       *PortDevMap
  )
{
  uint8_t   Index;
  uint8_t   DevFuncIndex;
  uint8_t   PortDevMapLocal[sizeof (DefaultPortDevMap)];
  MPIO_PROG_DEVICE_REMAP_STRUCT   DeviceRemap;

  memcpy((void *) PortDevMapLocal, (void *) DefaultPortDevMap, sizeof (DefaultPortDevMap));
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

  if (GnbHandle->RBIndex < 4) {
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
      DeviceRemap.Value = xUSLSmnRead(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        IOHC_REMAP_SPACE(GnbHandle, SMN_IOHUB0_N0NBIO0_NB_PROG_DEVICE_REMAP_ADDRESS, Index)
        );
      DeviceRemap.Field.DevFnMap = PortDevMap[Index];
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "sizeof (DefaultPortDevMap) %d\n", sizeof (DefaultPortDevMap));
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "Set NB_PROG_DEVICE_REMAP for RBIndex %d\n", GnbHandle->RBIndex);
      xUSLSmnWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        IOHC_REMAP_SPACE(GnbHandle, SMN_IOHUB0_N0NBIO0_NB_PROG_DEVICE_REMAP_ADDRESS, Index),
        DeviceRemap.Value
        );
    }
  } else {
    for (Index = 0; Index < 9; ++Index) {
      if (PortDevMapLocal[Index] == 0) {
        for (DevFuncIndex = 0; DevFuncIndex < 9; ++DevFuncIndex) {
          if (PortDevMapLocal[DevFuncIndex] != 0) {
            PortDevMap[Index] = PortDevMapLocal[DevFuncIndex];
            PortDevMapLocal[DevFuncIndex] = 0;
            break;
          }
        }
      }
      DeviceRemap.Value = xUSLSmnRead(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        IOHC_REMAP_SPACE(GnbHandle, SIL_RSVD_ADDR_1D4100B8, Index)
        );
      DeviceRemap.Field.DevFnMap = PortDevMap[Index];
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "Set NB_PROG_DEVICE_REMAP for RBIndex %d\n", GnbHandle->RBIndex);
      xUSLSmnWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        IOHC_REMAP_SPACE(GnbHandle, SIL_RSVD_ADDR_1D4100B8, Index),
        DeviceRemap.Value
        );
    }
  }
}

/**----------------------------------------------------------------------------------------
 * WritePcieStrapBrh
 *
 * @brief Routine to write pcie soft straps
 *
 * @param  GnbHandle         The associated Gnb Handle
 * @param  StrapIndex        Strap index
 * @param  Value             Contains value of strap register to write with
 * @param  Wrapper           Pcie wrapper number
 *
 * @returns Nothing
 *
 */
void
WritePcieStrapBrh (
  GNB_HANDLE     *GnbHandle,
  uint16_t       StrapIndex,
  uint32_t       Value,
  uint8_t        Wrapper
  )
{
  uint32_t               Response;
  uint32_t               MpioArg[6];
  uint8_t                InstanceNumber;

  InstanceNumber = GnbHandle->RBIndex + Wrapper * 8; //RB index + (pcie core * MaxRootBridge)

  //Bonus lanes WA
  if (InstanceNumber > 8) {
    InstanceNumber = 8;
  }

  memset(MpioArg, 0x00, sizeof (MpioArg));
  MpioArg[0] = (uint32_t) StrapIndex + ((uint32_t) InstanceNumber << 16);
  MpioArg[1] = Value;

  Response = MpioServiceRequestCommon(NbioGetHostPciAddress(GnbHandle), MPIO_MSG_PCIE_WRITE_STRAP, MpioArg, 0);
  MPIO_TRACEPOINT(SIL_TRACE_INFO, "  MPIO Response = 0x%x\n", Response);

  return;
}

/**----------------------------------------------------------------------------------------
 * Callback before hot plug initialization on all ports
 *
 * @brief TBD
 *
 * @param[in]       Engine          Pointer to engine config descriptor
 * @param[in, out]  Buffer          Not used
 * @param[in]       Pcie            Pointer to global PCIe configuration
 *
 * @returns Nothing
 *
 */
void
PcieHotplugPreInitBrh (
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  PCIe_WRAPPER_CONFIG   *Wrapper;
  GNB_HANDLE            *GnbHandle;
  NBIO_IP2IP_API        *NbioIp2Ip;

  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    assert(false);
  } else {
    Wrapper = (PCIe_WRAPPER_CONFIG *) NbioIp2Ip->PcieConfigGetParent(DESCRIPTOR_ALL_WRAPPERS, &(Engine->Header));
    GnbHandle = (GNB_HANDLE *) (NbioIp2Ip->PcieConfigGetParent(DESCRIPTOR_SILICON, &(Engine->Header)));
    if (GnbHandle == NULL) {
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "ERROR : Invalid input argument: GnbHandle\n");
      assert(false);
    } else {
      MpioSmnPrivateRegRMW(GnbHandle,
        PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1488),
        (uint32_t) ~(SIL_RESERVED_1410 | SIL_RESERVED_1408),
        ((0 << SIL_RESERVED_1411) | (0 << SIL_RESERVED_1409)),
        0
        );
    }
  }

}

/**----------------------------------------------------------------------------------------
 * Initialize hotplug features on all hotplug ports
 *
 * @brief TBD
 *
 * @param[in]       Engine          Pointer to engine config descriptor
 * @param[in, out]  Buffer          Not used
 * @param[in]       Pcie            Pointer to global PCIe configuration
 *
 * @returns Nothing
 *
 */
void
PcieHotplugInitBrh (
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  PCIe_WRAPPER_CONFIG   *Wrapper;
  GNB_HANDLE            *GnbHandle;
  uint32_t              Value;

  SOC_LOGICAL_ID              LogicalId;
  MPIO_COMPLEX_DESCRIPTOR     *MpioTopologyData;
  MPIO_PORT_DESCRIPTOR        *TopologyEntry;
  uint8_t                     PortParamIndex;
  MPIOCLASS_INPUT_BLK         *SilData;
  NBIOCLASS_DATA_BLOCK        *NbioData;
  NBIO_IP2IP_API              *NbioIp2Ip;

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

  /* Sync MPIO block with NBIO block */
  SilData->AmdHotPlugSettleTime = NbioData->NbioConfigData.AmdHotPlugSettleTime;
  SilData->AmdHotPlugSettleTimeMultiplier = NbioData->NbioConfigData.AmdHotPlugSettleTimeMultiplier;
  SilData->AmdHotPlugDLPDSyncCount = NbioData->NbioConfigData.AmdHotPlugDLPDSyncCount;
  SilData->AmdHotPlugPDSettle = NbioData->NbioConfigData.AmdHotPlugPDSettle;

  /*
   * Get PCIe topology from platform BIOS
   */
  MpioTopologyData = (MPIO_COMPLEX_DESCRIPTOR *)&SilData->PcieTopologyData;

  /*
   * Get Logical CPU ID info
   */
  GetSocLogicalIdOnCurrentCore(&LogicalId);


  if (Engine->Type.Port.PortData.LinkHotplug != PcieHotplugDisabled) {
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "Found Hotplug Engine at:\n");
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "  Port.PortId = %d\n", Engine->Type.Port.PortId);
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "  Port.PcieBridgeId = %d\n", Engine->Type.Port.PcieBridgeId);
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "  Port.Address = %x\n", Engine->Type.Port.Address);
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "  Type = ");

    Wrapper = (PCIe_WRAPPER_CONFIG *) NbioIp2Ip->PcieConfigGetParent(DESCRIPTOR_ALL_WRAPPERS, &(Engine->Header));
    GnbHandle = (GNB_HANDLE *) (NbioIp2Ip->PcieConfigGetParent(DESCRIPTOR_SILICON, &(Engine->Header)));

    if (GnbHandle == NULL) {
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "ERROR : Invalid input argument: GnbHandle\n");
      assert(false);
      return;
    }

    xUSLPciRMW(GnbHandle->Address.AddressValue |
      MAKE_SBDFO(0,
      0,
      Engine->Type.Port.PortData.DeviceNumber,
      Engine->Type.Port.PortData.FunctionNumber,
      PCICFG_OFFSET(SIL_RESERVED_1630)
      ),
      AccessWidth32,
      (uint32_t) ~(SIL_RESERVED_1582),
      1 << SIL_RESERVED_1583
      );

    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1629),
      (uint32_t) ~(SIL_RESERVED_1574),
      1 << SIL_RESERVED_1575
      );

    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1631),
      (uint32_t) ~(SIL_RESERVED_1599),
      (SilData->AmdDisableInbandPDSupport << SIL_RESERVED_1600)
      );

    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, SIL_RESERVED_1495),
      (uint32_t) ~(SIL_RESERVED_1246),
      1 << SIL_RESERVED_1247
      );
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, SIL_RESERVED_0775),
      (uint32_t) ~(SIL_RESERVED_1246),
      1 << SIL_RESERVED_1247
      );
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, SIL_RESERVED_1496),
      (uint32_t) ~(SIL_RESERVED_1246),
      1 << SIL_RESERVED_1247
      );

    if ((Value = SilData->AmdPresenceDetectSelectMode) != 0xFF) {
      MpioSmnPrivateRegRMW(GnbHandle,
        WRAP_SPACE(GnbHandle, Wrapper, SIL_RSVD_ADDR_1A3804E0),
        (uint32_t) ~(SIL_RESERVED_1428),
        (Value & (SIL_RESERVED_1428 >>
        SIL_RESERVED_1429)) <<
          SIL_RESERVED_1429,
          0
        );
    }

    if (SilData->AmdHotPlugHandlingMode != 5) {
      if ((SilData->AmdHotPlugPDSettle) && (Engine->Type.Port.PortData.LinkHotplug != PcieHotplugServerExpress)) {
        MPIO_TRACEPOINT(SIL_TRACE_INFO, "Setting Blocking Bits.");
        MpioSmnPrivateRegRMW(GnbHandle,
          PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1487),
          (uint32_t) ~((SIL_RESERVED_1412) |
          (SIL_RESERVED_1414) |
          (SIL_RESERVED_1406) |
          (SIL_RESERVED_1416) |
          (SIL_RESERVED_1418)),
          ((0 << SIL_RESERVED_1413) |
           (0 << SIL_RESERVED_1415) |
           (1 << SIL_RESERVED_1407) |
           (1 << SIL_RESERVED_1417) |
           (1 << SIL_RESERVED_1419)),
          0
          );
      }
    } else {
      MpioSmnPrivateRegRMW(GnbHandle,
        PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1487),
        (uint32_t) ~((SIL_RESERVED_1412) |
        (SIL_RESERVED_1414) |
        (SIL_RESERVED_1406) |
        (SIL_RESERVED_1416) |
        (SIL_RESERVED_1418)),
        ((0 << SIL_RESERVED_1413) |
         (0 << SIL_RESERVED_1415) |
         (1 << SIL_RESERVED_1407) |
         (0 << SIL_RESERVED_1417) |
         (0 << SIL_RESERVED_1419)),
        0
        );
    }

    MpioSmnPrivateRegRMW(GnbHandle,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1520),
      (uint32_t) ~(SIL_RESERVED_1517),
      0 << SIL_RESERVED_1518,
      0
      );

    MpioSmnPrivateRegRMW(GnbHandle,
      WRAP_SPACE(GnbHandle, Wrapper, SIL_RSVD_ADDR_1A3801B0),
      (uint32_t) ~(SIL_RESERVED_1434 |
      SIL_RESERVED_1432),
      (uint32_t) (1 << SIL_RESERVED_1435) |
      (1 << SIL_RESERVED_1433),
      0
      );

    /*
     * Firmware first mode / Firmware first but allow OS First mode
     */
    if ((SilData->AmdHotPlugHandlingMode == 3) || (SilData->AmdHotPlugHandlingMode == 6)) {
      /*
       * Enable SW SMI
       */
      if (GnbHandle->RBIndex < 4) {
        xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_13B20244),
          (uint32_t) ~((SIL_RESERVED_0700) |
          (SIL_RESERVED_0702)),
          ((1 << SIL_RESERVED_0701) |
          (1 << SIL_RESERVED_0703))
          );
      } else {
        xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_1D4200EC),
          (uint32_t) ~((SIL_RESERVED_0700) |
          (SIL_RESERVED_0702)),
          ((1 << SIL_RESERVED_0701) |
          (1 << SIL_RESERVED_0703))
          );
      }

      /*
       * Configure eDPC
       */
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE(GnbHandle,
        Wrapper,
        (Engine->Type.Port.PortId),
        SIL_RESERVED_1623
        ),
        (uint32_t) ~((SIL_RESERVED_1538 << 16) |
        (SIL_RESERVED_1542 << 16) |
        (SIL_RESERVED_1540 << 16)),
        ((0 << (SIL_RESERVED_1539 + 16)) |
        (1 << (SIL_RESERVED_1543 + 16)) |
        (0 << (SIL_RESERVED_1541 + 16)))
        );

      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE(GnbHandle,
        Wrapper,
        (Engine->Type.Port.PortId),
        SIL_RESERVED_1625
        ),
        (uint32_t) ~((SIL_RESERVED_1546) |
        (SIL_RESERVED_1544) |
        (SIL_RESERVED_1548) |
        (SIL_RESERVED_1552) |
        (SIL_RESERVED_1550)),
        ((1 << SIL_RESERVED_1547) |
        (1 << SIL_RESERVED_1545) |
        (1 << SIL_RESERVED_1549) |
        (1 << SIL_RESERVED_1553) |
        (1 << SIL_RESERVED_1551))
        );

      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE(GnbHandle,
        Wrapper,
        (Engine->Type.Port.PortId),
        SIL_RESERVED_1626
        ),
        (uint32_t) ~(-1),
        (uint32_t) (-1)
        );
    } else if (SilData->AmdHotPlugHandlingMode == 5) {
      // Enable SW SMI
      if (GnbHandle->RBIndex < 4) {
        xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_13B20244),
          (uint32_t) ~(SIL_RESERVED_0702),
          (1 << SIL_RESERVED_0703)
          );
      } else {
        xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_1D4200EC),
          (uint32_t) ~(SIL_RESERVED_0702),
          (1 << SIL_RESERVED_0703)
          );
      }
      /*
       * SFI mode - configure any SFI settings required for all hot plug modes
       * (SLOT_CNTL:INBAND_PD_DISABLE = 1)
       * (SLOT_CNTL: PRESENCE_DETECT_CHANGED_EN = 1)
       */
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE(GnbHandle,
        Wrapper,
        (Engine->Type.Port.PortId),
        SIL_RESERVED_1631
        ),
        (uint32_t) ~(SIL_RESERVED_1599 |
        SIL_RESERVED_1601),
        (0 << SIL_RESERVED_1600) | (1 << SIL_RESERVED_1602)
        );

      /* (PCIERCCFG::SFI_CNTL: SFI_PD_STATE_MASK = 1)
         (PCIERCCFG::SFI_CNTL: SFI_DLL_STATE_MASK = 1)
         (PCIERCCFG::SFI_CNTL: SFI_OOB_PD_CHANGED_EN = 1)
         (PCIERCCFG::SFI_CNTL: SFI_DLL_STATE_CHANGED_EN = 0)
         (PCIERCCFG::SFI_CNTL: SFI_DRS_MASK = 1)
         (PCIERCCFG::SFI_CNTL: SFI_DRS_SIGNALING_EN = 1) */
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1627),
        (uint32_t) ~((SIL_RESERVED_1566 << 16) |
        (SIL_RESERVED_1554 << 16) |
        (SIL_RESERVED_1562 << 16) |
        (SIL_RESERVED_1564 << 16)),
        (0 << (SIL_RESERVED_1567 + 16)) |
        (1 << (SIL_RESERVED_1555 + 16)) |
        (1 << (SIL_RESERVED_1563 + 16)) |
        (1 << (SIL_RESERVED_1565 + 16))
        );
    }

    if (GnbHandle->RBIndex < 4) {
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_13B20244),
        (uint32_t) ~((SIL_RESERVED_0700)),
        ((1 << SIL_RESERVED_0701))
        );
    } else {
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        NBIO_SPACE(GnbHandle, SIL_RSVD_ADDR_13D20244),
        (uint32_t) ~((SIL_RESERVED_0700)),
        ((1 << SIL_RESERVED_0701))
        );
    }

    // Sets the bit so that port does not go into loopback mode
    MpioSmnPrivateRegRMW(GnbHandle,
      PORT_SPACE(GnbHandle,
      Wrapper,
      (Engine->Type.Port.PortId),
      SIL_RESERVED_1485
      ),
      (uint32_t) ~(SIL_RESERVED_0499),
      1 << SIL_RESERVED_0500,
        0
      );

    // Sets the bit so that port does not go into loopback mode
    MpioSmnPrivateRegRMW(GnbHandle,
      PORT_SPACE(GnbHandle,
      Wrapper,
      (Engine->Type.Port.PortId),
      SIL_RESERVED_1491
      ),
      (uint32_t) ~(SIL_RESERVED_1430),
      1 << SIL_RESERVED_1431,
        0
      );

    /*
     * Type specific hotplug configuration
     */
    switch (Engine->Type.Port.PortData.LinkHotplug) {
    /*
     * Basic Hotplug Configuration
     */
    case PcieHotplugBasic:
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "PcieHotplugBasic\n");
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE(GnbHandle,
        Wrapper,
        (Engine->Type.Port.PortId),
        SIL_RESERVED_1631
        ),
        (uint32_t) ~(SIL_RESERVED_1597),
        (1 << SIL_RESERVED_1598)
        );

      MpioSmnPrivateRegRMW(GnbHandle,
        PORT_SPACE(GnbHandle,
        Wrapper,
        (Engine->Type.Port.PortId),
        SIL_RESERVED_1490
        ),
        (uint32_t) ~(SIL_RESERVED_1424 | SIL_RESERVED_1422),
        (0 << SIL_RESERVED_1425) | (1 << SIL_RESERVED_1423),
        0
        );

      Value = xUSLSmnRead(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1619)
        );

      if ((Value & (uint32_t) (1 << (SIL_RESERVED_1251 + 16))) == 0) {
        if (GnbHandle->RBIndex < 4) {
          xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
            GnbHandle->Address.Address.Bus,
            IOHC_BRIDGE_SPACE(GnbHandle, Engine, SIL_RESERVED_0748),
            (uint32_t) ~(SIL_RESERVED_0605),
            (uint32_t)((!SilData->AmdHotPlugDisBridgeDis) << SIL_RESERVED_0606)
            );
          MPIO_TRACEPOINT(SIL_TRACE_INFO,
            "No ep - BridgeDis: %08x = %u\n",
            IOHC_BRIDGE_SPACE(GnbHandle, Engine, SIL_RESERVED_0748),
            !SilData->AmdHotPlugDisBridgeDis
            );
        } else {
          xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
            GnbHandle->Address.Address.Bus,
            IOHC_BRIDGE_SPACE(GnbHandle, Engine, SIL_RSVD_ADDR_1D431004),
            (uint32_t) ~(SIL_RESERVED_0605),
            (uint32_t)((!SilData->AmdHotPlugDisBridgeDis) << SIL_RESERVED_0606)
            );
          MPIO_TRACEPOINT(SIL_TRACE_INFO,
            "No ep - BridgeDis: %08x = %u\n",
            IOHC_BRIDGE_SPACE(GnbHandle, Engine, SIL_RSVD_ADDR_1D431004),
            !SilData->AmdHotPlugDisBridgeDis
            );
        }

        Value = (uint8_t) SilData->AmdHotPlugNvmeDefaultMaxPayload;
        if (Value != 0xFF) {
          xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
            GnbHandle->Address.Address.Bus,
            PORT_SPACE(GnbHandle,
            Wrapper,
            (Engine->Type.Port.PortId),
            SIL_RESERVED_1614
            ),
            (uint32_t) ~(SIL_RESERVED_1524),
            (Value & 0x7) << SIL_RESERVED_1525
            );
        }
      }
      break;

    /*
     * Enhanced Hotplug Configuration
     */
    case PcieHotplugEnhanced:
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "PcieHotplugEnhanced\n");
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE(GnbHandle,
        Wrapper,
        (Engine->Type.Port.PortId),
        SIL_RESERVED_1631
        ),
        (uint32_t) ~(SIL_RESERVED_1597 |
        SIL_RESERVED_1601 |
        SIL_RESERVED_1595),
        ((1 << SIL_RESERVED_1598) |
        (1 << SIL_RESERVED_1602) |
        (1 << SIL_RESERVED_1596) |
        (1 << (SIL_RESERVED_1603 + 16)) |
        (1 << (SIL_RESERVED_1608 + 16)) |
        (1 << (SIL_RESERVED_1606 + 16)) |
        (1 << (SIL_RESERVED_1607 + 16)) |
        (1 << (SIL_RESERVED_1604 + 16)) |
        (1 << (SIL_RESERVED_1605 + 16)))
        );

      MpioSmnPrivateRegRMW(GnbHandle,
        PORT_SPACE(GnbHandle,
        Wrapper,
        (Engine->Type.Port.PortId),
        SIL_RESERVED_1490
        ),
        (uint32_t) ~(SIL_RESERVED_1424 | SIL_RESERVED_1422),
        (1 << SIL_RESERVED_1425) | (1 << SIL_RESERVED_1423),
        0
        );

      MpioSmnPrivateRegRMW(GnbHandle,
        PORT_SPACE(GnbHandle,
        Wrapper,
        (Engine->Type.Port.PortId),
        SIL_RESERVED_1469
        ),
        (uint32_t) ~(SIL_RESERVED_1318 |
        SIL_RESERVED_1324),
        ((1 << SIL_RESERVED_1319) |
        (0 << SIL_RESERVED_1325)),
        0
        );

      xUSLPciRead(GnbHandle->Address.AddressValue |
        MAKE_SBDFO(0,
        0,
        Engine->Type.Port.PortData.DeviceNumber,
        Engine->Type.Port.PortData.FunctionNumber,
        PCICFG_OFFSET(SIL_RESERVED_1631)
        ),
        AccessWidth32,
        &Value
        );

      MPIO_TRACEPOINT(SIL_TRACE_INFO, "Value = 0x%x\n", Value);

      MpioSmnPrivateRegRead(GnbHandle,
        PORT_SPACE(GnbHandle,
        Wrapper,
        (Engine->Type.Port.PortId),
        SIL_RESERVED_1469
        ),
        &Value
        );

      MPIO_TRACEPOINT(SIL_TRACE_INFO, "Value = 0x%x\n", Value);

      MpioSmnPrivateRegRMW(GnbHandle,
        WRAP_SPACE(GnbHandle, Wrapper, SIL_RESERVED_1501),
        (uint32_t) ~(SIL_RESERVED_1296),
        0x5 << SIL_RESERVED_1297,
          0
        );

      MpioSmnPrivateRegRMW(GnbHandle,
        WRAP_SPACE(GnbHandle, Wrapper, SIL_RSVD_ADDR_1A380460),
        (uint32_t) ~(SIL_RESERVED_1160),
        0x1 << SIL_RESERVED_1161,
          0
        );

      Value = xUSLSmnRead(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1619)
        );

      if ((Value & (uint32_t) (1 << (SIL_RESERVED_1251 + 16))) == 0) {
        if (GnbHandle->RBIndex < 4) {
          xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
            GnbHandle->Address.Address.Bus,
            IOHC_BRIDGE_SPACE(GnbHandle, Engine, SIL_RESERVED_0748),
            (uint32_t) ~(SIL_RESERVED_0605),
            (uint32_t)((!SilData->AmdHotPlugDisBridgeDis) << SIL_RESERVED_0606)
            );

          MPIO_TRACEPOINT(SIL_TRACE_INFO,
            "No ep - BridgeDis: %08x = %u\n",
            IOHC_BRIDGE_SPACE(GnbHandle, Engine, SIL_RESERVED_0748),
            !SilData->AmdHotPlugDisBridgeDis
            );
        } else {
          xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
            GnbHandle->Address.Address.Bus,
            IOHC_BRIDGE_SPACE(GnbHandle, Engine, SIL_RSVD_ADDR_1D431004),
            (uint32_t) ~(SIL_RESERVED_0605),
            (uint32_t)((!SilData->AmdHotPlugDisBridgeDis) << SIL_RESERVED_0606)
            );
          MPIO_TRACEPOINT(SIL_TRACE_INFO,
            "No ep - BridgeDis: %08x = %u\n",
            IOHC_BRIDGE_SPACE(GnbHandle, Engine, SIL_RSVD_ADDR_1D431004),
            !SilData->AmdHotPlugDisBridgeDis
            );
        }
      } else {
        if (GnbHandle->RBIndex < 4) {
          xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
            GnbHandle->Address.Address.Bus,
            IOHC_BRIDGE_SPACE(GnbHandle, Engine, SIL_RESERVED_0748),
            (uint32_t) ~(SIL_RESERVED_0605),
            0
            );

          MPIO_TRACEPOINT(SIL_TRACE_INFO,
            "Found ep - BridgeDis: %08x = %u\n",
            IOHC_BRIDGE_SPACE(GnbHandle, Engine, SIL_RESERVED_0748),
            0
            );
        } else {
          xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
            GnbHandle->Address.Address.Bus,
            IOHC_BRIDGE_SPACE(GnbHandle, Engine, SIL_RSVD_ADDR_1D431004),
            (uint32_t) ~(SIL_RESERVED_0605),
            0
            );
          MPIO_TRACEPOINT(SIL_TRACE_INFO,
            "No ep - BridgeDis: %08x = %u\n",
            IOHC_BRIDGE_SPACE(GnbHandle, Engine, SIL_RSVD_ADDR_1D431004),
            0
            );
        }
      }

      break;
    /*
     * Inboard Hotplug Configuration
     * Inboard Hotplug is similar to Enhanced Hotplug, except that the device must be present at boot
     */
    case PcieHotplugInboard:
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "PcieHotplugInboard\n");
      if (Engine->InitStatus == INIT_STATUS_PCIE_TRAINING_SUCCESS) {
        MpioSmnPrivateRegRMW(GnbHandle,
          PORT_SPACE(GnbHandle,
          Wrapper,
          (Engine->Type.Port.PortId),
          SIL_RESERVED_1490
          ),
          (uint32_t) ~(SIL_RESERVED_1424),
          1 << SIL_RESERVED_1425,
            0
          );

        MpioSmnPrivateRegRMW(GnbHandle,
          PORT_SPACE(GnbHandle,
          Wrapper,
          (Engine->Type.Port.PortId),
          SIL_RESERVED_1469
          ),
          (uint32_t) ~(SIL_RESERVED_1318 |
          SIL_RESERVED_1324),
          ((1 << SIL_RESERVED_1319) |
          (0 << SIL_RESERVED_1325)),
          0
          );

        xUSLPciRead(GnbHandle->Address.AddressValue |
          MAKE_SBDFO(0,
          0,
          Engine->Type.Port.PortData.DeviceNumber,
          Engine->Type.Port.PortData.FunctionNumber,
          PCICFG_OFFSET(SIL_RESERVED_1630)
          ),
          AccessWidth32,
          &Value
          );

        MPIO_TRACEPOINT(SIL_TRACE_INFO, "Value = 0x%x\n", Value);

        MpioSmnPrivateRegRead(GnbHandle,
          PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1469),
          &Value
          );

        MPIO_TRACEPOINT(SIL_TRACE_INFO, "Value = 0x%x\n", Value);

        MpioSmnPrivateRegRMW(GnbHandle,
          WRAP_SPACE(GnbHandle, Wrapper, SIL_RESERVED_1501),
          (uint32_t) ~(SIL_RESERVED_1296),
          0x5 << SIL_RESERVED_1297,
            0
          );
      }
      break;
    case PcieHotplugServerExpress:
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "PcieHotplugServerExpress\n");
      //Express Module
      if (SilData->AmdHotplugPortReset == 1) {
        xUSLPciRMW(GnbHandle->Address.AddressValue |
          MAKE_SBDFO(0,
          0,
          Engine->Type.Port.PortData.DeviceNumber,
          Engine->Type.Port.PortData.FunctionNumber,
          PCICFG_OFFSET(SIL_RESERVED_1630)
          ),
          AccessWidth32,
          (uint32_t) ~(SIL_RESERVED_1576 |
          SIL_RESERVED_1590 |
          SIL_RESERVED_1586 |
          SIL_RESERVED_1578 |
          SIL_RESERVED_1592 |
          SIL_RESERVED_1584 |
          SIL_RESERVED_1580 |
          SIL_RESERVED_1588),
          ((0 << SIL_RESERVED_1577) |
          (1 << SIL_RESERVED_1591) |
          (0 << SIL_RESERVED_1587) |
          (1 << SIL_RESERVED_1579) |
          (1 << SIL_RESERVED_1593) |
          (1 << SIL_RESERVED_1585) |
          (1 << SIL_RESERVED_1581) |
          (0 << SIL_RESERVED_1589))
          );
      } else {
        xUSLPciRMW(GnbHandle->Address.AddressValue |
          MAKE_SBDFO(0,
          0,
          Engine->Type.Port.PortData.DeviceNumber,
          Engine->Type.Port.PortData.FunctionNumber,
          PCICFG_OFFSET(SIL_RESERVED_1630)
          ),
          AccessWidth32,
          (uint32_t) ~(SIL_RESERVED_1576 |
          SIL_RESERVED_1590 |
          SIL_RESERVED_1586 |
          SIL_RESERVED_1578 |
          SIL_RESERVED_1592 |
          SIL_RESERVED_1584 |
          SIL_RESERVED_1580 |
          SIL_RESERVED_1588),
          ((1 << SIL_RESERVED_1577) |
          (1 << SIL_RESERVED_1591) |
          (0 << SIL_RESERVED_1587) |
          (1 << SIL_RESERVED_1579) |
          (1 << SIL_RESERVED_1593) |
          (0 << SIL_RESERVED_1585) |
          (1 << SIL_RESERVED_1581) |
          (0 << SIL_RESERVED_1589))
          );
      }

      MpioSmnPrivateRegRMW(GnbHandle,
        PORT_SPACE(GnbHandle,
        Wrapper,
        (Engine->Type.Port.PortId),
        SIL_RESERVED_1490
        ),
        (uint32_t) ~(SIL_RESERVED_1426),
        1 << SIL_RESERVED_1427,
          0
        );

      MpioSmnPrivateRegRMW(GnbHandle,
        WRAP_SPACE(GnbHandle, Wrapper, SIL_RSVD_ADDR_1A380428),
        (uint32_t) ~(1 << (Engine->Type.Port.PortId)),
        0,
        0
        );

      Value = xUSLSmnRead(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1619)
        );

      if ((Value & (uint32_t) (1 << (SIL_RESERVED_1251 + 16))) == 0) {
        if (GnbHandle->RBIndex < 4) {
          xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
            GnbHandle->Address.Address.Bus,
            IOHC_BRIDGE_SPACE(GnbHandle, Engine, SIL_RESERVED_0748),
            (uint32_t) ~(SIL_RESERVED_0605),
            (uint32_t)((!SilData->AmdHotPlugDisBridgeDis) << SIL_RESERVED_0606)
            );

          MPIO_TRACEPOINT(SIL_TRACE_INFO,
            "No ep - BridgeDis: %08x = %u\n",
            IOHC_BRIDGE_SPACE(GnbHandle, Engine, SIL_RESERVED_0748),
            !SilData->AmdHotPlugDisBridgeDis
            );
        } else {
          xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
            GnbHandle->Address.Address.Bus,
            IOHC_BRIDGE_SPACE(GnbHandle, Engine, SIL_RSVD_ADDR_1D431004),
            (uint32_t) ~(SIL_RESERVED_0605),
            (uint32_t)((!SilData->AmdHotPlugDisBridgeDis) << SIL_RESERVED_0606)
            );
          MPIO_TRACEPOINT(SIL_TRACE_INFO,
            "No ep - BridgeDis: %08x = %u\n",
            IOHC_BRIDGE_SPACE(GnbHandle, Engine, SIL_RSVD_ADDR_1D431004),
            !SilData->AmdHotPlugDisBridgeDis
            );
        }
      }

      if (SilData->AmdAllowComplianceForHpPort == true) {
        // Clear the bit so that port does go into loopback mode
        MpioSmnPrivateRegRMW(GnbHandle,
          PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1491),
          (uint32_t) ~(SIL_RESERVED_1430),
          0 << SIL_RESERVED_1431,
            0
          );
      }
      break;

    case PcieHotplugUBM:
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "PcieHotplugUBM\n");

      while (MpioTopologyData != NULL) {
        TopologyEntry = MpioTopologyData->PciePortList;
        while (TopologyEntry != NULL) {
          if ((Engine->EngineData.EngineType == MpioPcieEngine) &&
            (Engine->EngineData.StartLane >= TopologyEntry->EngineData.StartLane) &&
            (Engine->EngineData.EndLane <= TopologyEntry->EngineData.EndLane)) {
            PortParamIndex = 0;
            while (PortParamIndex < PCIE_PORT_PARAMETER_COUNT) {
              /*
               * Configure NPEM Capability value
               */
              if (TopologyEntry->PortParams.PhyParam[PortParamIndex].ParamType == MPIO_PP_NPEM_CAPABILITES) {
                xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  PORT_SPACE(GnbHandle,
                  Wrapper,
                  (Engine->Type.Port.PortId),
                  SIL_RESERVED_1620
                  ),
                  (uint32_t) ~(0xFFFFFFFF),
                  TopologyEntry->PortParams.PhyParam[PortParamIndex].ParamValue & 0x0FFF
                  );
              }

              /*
               * Configure overall NPEM Enable bit
               */
              if (TopologyEntry->PortParams.PhyParam[PortParamIndex].ParamType == MPIO_PP_NPEM_ENABLE) {
                xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  PORT_SPACE(GnbHandle,
                  Wrapper,
                  (Engine->Type.Port.PortId),
                  SIL_RESERVED_1621
                  ),
                  (uint32_t) ~(0xFFFFFFFF),
                  TopologyEntry->PortParams.PhyParam[PortParamIndex].ParamValue & 0x0FFF
                  );
              }
              PortParamIndex++;
            }
          }
          TopologyEntry = PcieConfigGetNextDataDescriptor(TopologyEntry);
        }
        MpioTopologyData = PcieConfigGetNextDataDescriptor(MpioTopologyData);
      }
    // fallthrough

    case PcieHotplugOCP:
      Value = xUSLSmnRead(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE(GnbHandle,
        Wrapper,
        (Engine->Type.Port.PortId),
        SIL_RESERVED_1630
        )
        );

      Value = (Value & SLOT_CAP_PHYSICAL_SLOT_NUM_MASK) >> SLOT_CAP_PHYSICAL_SLOT_NUM_OFFSET;
      MpioSmnPrivateRegRMW(GnbHandle,
        PORT_SPACE(GnbHandle,
        Wrapper,
        (Engine->Type.Port.PortId),
        SIL_RESERVED_1486
        ),
        (uint32_t) ~(0xFFFFFFFF),
        (uint32_t) (0x80000000 | Value),
        0
        );
    // fallthrough
    case PcieHotplugServerEntSSD:
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "PcieHotplugServerEntSSD\n");
      /*
       * Enterprise SSD
       */
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE(GnbHandle,
        Wrapper,
        Engine->Type.Port.PortId,
        SIL_RESERVED_1630
        ),
        (uint32_t) ~(SIL_RESERVED_1576 |
        SIL_RESERVED_1590 |
        SIL_RESERVED_1586 |
        SIL_RESERVED_1578 |
        SIL_RESERVED_1592 |
        SIL_RESERVED_1584 |
        SIL_RESERVED_1580 |
        SIL_RESERVED_1588),
        ((0 << SIL_RESERVED_1577) |
        (0 << SIL_RESERVED_1591) |
        (0 << SIL_RESERVED_1587) |
        (0 << SIL_RESERVED_1579) |
        (0 << SIL_RESERVED_1593) |
        (1 << SIL_RESERVED_1585) |
        (0 << SIL_RESERVED_1581) |
        (1 << SIL_RESERVED_1589))
        );

      if (SilData->AmdHotPlugHandlingMode == 5) {
        xUSLPciRMW(GnbHandle->Address.AddressValue |
          MAKE_SBDFO(0,
          0,
          Engine->Type.Port.PortData.DeviceNumber,
          Engine->Type.Port.PortData.FunctionNumber,
          PCICFG_OFFSET(SIL_RESERVED_1630)
          ),
          AccessWidth32,
          (uint32_t) ~(SIL_RESERVED_1584),
          (SilData->HotPlugSurpriseMechanism << SIL_RESERVED_1585)
          );
      }

      MpioSmnPrivateRegRMW(GnbHandle,
        WRAP_SPACE(GnbHandle, Wrapper, SIL_RSVD_ADDR_1A380428),
        (uint32_t) ~(1 << (Engine->Type.Port.PortId)),
        0,
        0
        );

      MpioSmnPrivateRegRMW(GnbHandle,
        PORT_SPACE(GnbHandle,
        Wrapper,
        (Engine->Type.Port.PortId),
        SIL_RESERVED_1490
        ),
        (uint32_t) ~(SIL_RESERVED_1426),
        1 << SIL_RESERVED_1427,
          0
        );

      MpioSmnPrivateRegRMW(GnbHandle,
        PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1519),
        (uint32_t) ~(SIL_RESERVED_1515),
        0x20 << SIL_RESERVED_1516,
          0
        );

      Value = xUSLSmnRead(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE(GnbHandle,
        Wrapper,
        (Engine->Type.Port.PortId),
        SIL_RESERVED_1619
        )
        );

      if ((Value & ((uint32_t) 1 << (SIL_RESERVED_1251 + 16))) == 0) {
        if (SilData->AmdHotPlugHandlingMode != 5) {
          if (GnbHandle->RBIndex < 4) {
            xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
              GnbHandle->Address.Address.Bus,
              IOHC_BRIDGE_SPACE(GnbHandle, Engine, SIL_RESERVED_0748),
              (uint32_t) ~((SIL_RESERVED_0605) |
              (SIL_RESERVED_0608) |
              (SIL_RESERVED_0607)),
              (uint32_t)((!SilData->AmdHotPlugDisBridgeDis) << SIL_RESERVED_0606)
              );

            MPIO_TRACEPOINT(SIL_TRACE_INFO,
              "No ep - BridgeDis: %08x = %u\n",
              IOHC_BRIDGE_SPACE(GnbHandle, Engine, SIL_RESERVED_0748),
              !SilData->AmdHotPlugDisBridgeDis
              );
          } else {
            xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
              GnbHandle->Address.Address.Bus,
              IOHC_BRIDGE_SPACE(GnbHandle, Engine, SIL_RSVD_ADDR_1D431004),
              (uint32_t) ~((SIL_RESERVED_0605) |
              (SIL_RESERVED_0608) |
              (SIL_RESERVED_0607)),
              (uint32_t)((!SilData->AmdHotPlugDisBridgeDis) << SIL_RESERVED_0606)
              );
            MPIO_TRACEPOINT(SIL_TRACE_INFO,
              "No ep - BridgeDis: %08x = %u\n",
              IOHC_BRIDGE_SPACE(GnbHandle, Engine, SIL_RSVD_ADDR_1D431004),
              !SilData->AmdHotPlugDisBridgeDis
              );
          }
        }

        if (SilData->AmdHotPlugHandlingMode == 5) {
          MPIO_TRACEPOINT(SIL_TRACE_INFO,
            "No ep SFI Mode - Set SFI_PD_STATE_MASK, SFI_DLL_STATE_MASK, SFI_DRS_MASK: %08x\n",
            PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId),
            SIL_RESERVED_1631
            )
            );

          xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
            GnbHandle->Address.Address.Bus,
            PORT_SPACE(GnbHandle,
            Wrapper,
            (Engine->Type.Port.PortId),
            SIL_RESERVED_1627
            ),
            (uint32_t) ~((SIL_RESERVED_1568 << 16) |
            (SIL_RESERVED_1556 << 16) |
            (SIL_RESERVED_1560 << 16)),
            (1 << (SIL_RESERVED_1569 + 16)) |
            (1 << (SIL_RESERVED_1557 + 16)) |
            (1 << (SIL_RESERVED_1561 + 16))
            );
        }

        Value = (uint8_t) SilData->AmdHotPlugNvmeDefaultMaxPayload;
        if (Value != 0xFF) {
          xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
            GnbHandle->Address.Address.Bus,
            PORT_SPACE(GnbHandle,
            Wrapper,
            (Engine->Type.Port.PortId),
            SIL_RESERVED_1614
            ),
            (uint32_t) ~(SIL_RESERVED_1524),
            (Value & 0x7) << SIL_RESERVED_1525
            );
        }
        // b) If slot is empty, block empty traffic (PCIERCCFG::SFI_CNTL: SFI_DPF_CONTROL = 1)
        xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          PORT_SPACE(GnbHandle,
          Wrapper,
          (Engine->Type.Port.PortId),
          SIL_RESERVED_1627
          ),
          (uint32_t) ~(SIL_RESERVED_1558 << 16),
          (1 << (SIL_RESERVED_1559 + 16))
          );
      } else {
        if (SilData->AmdHotPlugHandlingMode == 5) {
          xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
            GnbHandle->Address.Address.Bus,
            PORT_SPACE(GnbHandle,
            Wrapper,
            (Engine->Type.Port.PortId),
            SIL_RESERVED_1627
            ),
            (uint32_t) ~(SIL_RESERVED_1558 << 16),
            (0 << (SIL_RESERVED_1559 + 16))
            );

          /*
           * Clear out SFI_STATUS on BOOT
           */
          xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
            GnbHandle->Address.Address.Bus,
            PORT_SPACE(GnbHandle,
            Wrapper,
            (Engine->Type.Port.PortId),
            SIL_RESERVED_1628
            ),
            (uint32_t) ~((SIL_RESERVED_1570) |
            (SIL_RESERVED_1572)),
            (1 << (SIL_RESERVED_1571)) |
            (1 << (SIL_RESERVED_1573))
            );
        } else {
          if (GnbHandle->RBIndex < 4) {
            xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
              GnbHandle->Address.Address.Bus,
              IOHC_BRIDGE_SPACE(GnbHandle, Engine, SIL_RESERVED_0748),
              (uint32_t) ~((SIL_RESERVED_0605) |
              (SIL_RESERVED_0608) |
              (SIL_RESERVED_0607)),
              0
              );
            MPIO_TRACEPOINT(SIL_TRACE_INFO,
              "Ep found - BridgeDis: %08x = %u\n",
              IOHC_BRIDGE_SPACE(GnbHandle, Engine, SIL_RESERVED_0748),
              0
              );
          } else {
            xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
              GnbHandle->Address.Address.Bus,
              IOHC_BRIDGE_SPACE(GnbHandle, Engine, SIL_RSVD_ADDR_1D431004),
              (uint32_t) ~((SIL_RESERVED_0605) |
              (SIL_RESERVED_0608) |
              (SIL_RESERVED_0607)),
              0
              );
            MPIO_TRACEPOINT(SIL_TRACE_INFO,
              "No ep - BridgeDis: %08x = %u\n",
              IOHC_BRIDGE_SPACE(GnbHandle, Engine, SIL_RSVD_ADDR_1D431004),
              0
              );
          }
        }
      }
      break;

    default:
      MPIO_TRACEPOINT(SIL_TRACE_INFO, "Invalid Hotplug Type\n");
      assert(false);
      break;
    }
    MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  }

  return;
}

//----------------------------------------------------------------//
/**
 * Function to verify if the wrapper contains an early trained BMC
 *
 * @param[in]     Wrapper   Wrapper configuration info
 * @param[in]     GnbHandle GnbHandle Pointer
 * @return        TRUE  - The BMC is located in this wrapper
 *                FALSE - The BMC is not located in this wrapper
 *                        or BMC is not trained in early training
 **/
bool
IsEarlyTrainedBmcInWrapperBrh (
  PCIe_WRAPPER_CONFIG               *Wrapper,
  GNB_HANDLE                        *GnbHandle
  )
{
  MPIOCLASS_INPUT_BLK    *SilData;

  /*
   * Get IP block data
   */
  SilData = (MPIOCLASS_INPUT_BLK *)xUslFindStructure(SilId_MpioClass, 0);
  assert(SilData != NULL);

  if (SilData->EarlyBmcLinkTraining) {
    if ((GnbHandle->SocketId == SilData->EarlyBmcLinkSocket) &&
      (SilData->EarlyBmcLinkLaneNum >= Wrapper->StartPhyLane) &&
      (SilData->EarlyBmcLinkLaneNum <= Wrapper->EndPhyLane)) {
      return true;
    }
  }
  return false;
}
