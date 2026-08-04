/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file MpioCfgPoints.c
 *  @brief Callouts from MPIO initialization
 */

#include <string.h>
#include <xSIM.h>
#include <SilSocLogicalId.h>
#include <Nbio/NbioIp2Ip.h>
#include <PciLib.h>
#include <SMU/SmuIp2Ip.h>
#include "MpioInitLib.h"
#include "MpioLib.h"
#include "MpioCmn2Rev.h"
#include "MpioLibLocal.h"
#include "MpioStructs.h"

/**-------------------------------------------------------------------
 * MpioCfgBeforeReconfig
 *
 * @brief Interface to configure DXIO/PCIe ports
 *
 * @details This function is called once for each socket after ports
 * are mapped and before reconfig
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param   GnbHandle           Pointer to the Silicon Descriptor for this node
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
MpioCfgBeforeReconfig (
  SIL_CONTEXT      *SilContext,
  GNB_HANDLE       *GnbHandle
  )
{
  GNB_HANDLE                    *LocalHandle;
  uint32_t                      SocketId;
  MPIO_COMMON_2_REV_XFER_BLOCK  *MpioXferTable;
  NBIO_IP2IP_API                *NbioIp2Ip;

  MPIO_TRACEPOINT(SIL_TRACE_INFO, " Socket %d\n", GnbHandle->SocketId);

  if (SilGetCommon2RevXferTable(SilContext, SilId_MpioClass, (void **)(&MpioXferTable)) != SilPass) {
    return;
  }

  LocalHandle = GnbHandle;
  SocketId = GnbHandle->SocketId;
  if (SilGetIp2IpApi(SilContext, SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }
  while (LocalHandle != NULL) {
    if (LocalHandle->SocketId == SocketId) {
      NbioIp2Ip->PcieConfigRunProcForAllWrappersInNbio(SilContext,
        DESCRIPTOR_ALL_WRAPPERS,
        MpioXferTable->MpioCfgBeforeReconfigWrapper,
        NULL,
        LocalHandle
        );

    }
    LocalHandle = GnbGetNextHandle(LocalHandle);
  }
  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**-------------------------------------------------------------------
 * MpioCfgAfterReconfig
 *
 * @brief Interface to configure DXIO/PCIe ports after reconfig and before training
 *
 * @details This function is called once for each socket
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param[in]  GnbHandle        Pointer to the Silicon Descriptor for this node
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
MpioCfgAfterReconfig (
  SIL_CONTEXT      *SilContext,
  GNB_HANDLE       *GnbHandle
  )
{
  GNB_HANDLE                      *LocalHandle;
  uint32_t                        SocketId;
  AFTER_RECONFIG_BUFFER           AfterReconfigBuffer;
  MPIOCLASS_COMMON_INPUT_BLK      *SilDataCommon;
  MPIO_COMMON_2_REV_XFER_BLOCK    *MpioXferTable;
  NBIO_IP2IP_API                  *NbioIp2Ip;

  /*
   * Get IP block data
   */
  SilDataCommon = (MPIOCLASS_COMMON_INPUT_BLK *)xUslFindStructure(SilContext,
    SilId_MpioClass,
    MPIOCLASS_COMMON_INSTANCE
    );
  assert(SilDataCommon != NULL);

  if (SilGetCommon2RevXferTable(SilContext, SilId_MpioClass, (void **)(&MpioXferTable)) != SilPass) {
    return;
  }

  AfterReconfigBuffer.DpcStatusData = NULL;

  MPIO_TRACEPOINT(SIL_TRACE_INFO, " Socket %d\n", GnbHandle->SocketId);
  AfterReconfigBuffer.DpcStatusData = &SilDataCommon->DpcStatusData;

  LocalHandle = GnbHandle;
  SocketId = GnbHandle->SocketId;
  if (SilGetIp2IpApi(SilContext, SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    MPIO_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }
  while (LocalHandle != NULL) {
    if (LocalHandle->SocketId == SocketId) {
      NbioIp2Ip->PcieConfigRunProcForAllWrappersInNbio(SilContext,
        DESCRIPTOR_ALL_WRAPPERS,
        MpioXferTable->MpioCfgAfterReconfigWrapper,
        &AfterReconfigBuffer,
        LocalHandle
        );
    }
    LocalHandle = GnbGetNextHandle(LocalHandle);
  }
  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

#define SCAN_SKIP_FUNCTIONS         0x1
#define SCAN_SKIP_DEVICES           0x2
#define SCAN_SKIP_BUSES             0x4
#define SCAN_SUCCESS                0x0

static uint32_t
PciScanEarlyLink (
  uint8_t *StartBusNumber,
  uint8_t *UsbController
  )
{
  uint8_t           DeviceNumber;
  uint8_t           Function;
  uint8_t           PrimaryBus;
  PCI_ADDR          Device;
  PCIE_DEVICE_TYPE  DeviceType;
  uint32_t          Value32;
  uint8_t           LastFunction;
  uint32_t          SkipScanStatus;

  MPIO_TRACEPOINT (SIL_TRACE_ENTRY, " (Bus=0x%x)\n", *StartBusNumber);
  SkipScanStatus = 0;
  Device.AddressValue = 0;
  PrimaryBus = *StartBusNumber;
  for (DeviceNumber = 0; DeviceNumber <= 0x1F; DeviceNumber++) {
    Device.AddressValue = MAKE_SBDFO (0, PrimaryBus, DeviceNumber, 0, 0);
    if (!xUSLPciIsDevicePresent (Device.AddressValue)) {
      continue;
    }
    LastFunction = 0x0;
    if (xUSLPciIsMultiFunctionDevice (Device.AddressValue)) {
      LastFunction = 0x7;
    }
    for (Function = 0; Function <= LastFunction; Function++) {
      Device.AddressValue = MAKE_SBDFO (0, PrimaryBus, DeviceNumber, Function, 0);
      if (xUSLPciIsDevicePresent (Device.AddressValue)) {
        DeviceType = xUSLPciLibbGetPcieDeviceType (Device.AddressValue);
        switch (DeviceType) {
        case  PcieDeviceRootComplex:
        case  PcieDeviceUpstreamPort:
          (*StartBusNumber)++;
          xUSLPciReadModifyWrite32 (
            Device.AddressValue | 0x18,
            0xFF000000,
            (0xFF << 16) | ((uint8_t) *StartBusNumber) << 8 | PrimaryBus
            );
          SkipScanStatus = PciScanEarlyLink (StartBusNumber, UsbController);
          xUSLPciReadModifyWrite32 (
            Device.AddressValue | 0x18,
            0xFF00FFFF,
            *StartBusNumber << 16
            );
          break;
        case  PcieDeviceDownstreamPort:
          if (!xUSLPciIsSlotImplemented (Device.AddressValue)) {
            (*StartBusNumber)++;
            xUSLPciReadModifyWrite32 (
              Device.AddressValue | 0x18,
              0xFF000000,
              ((*StartBusNumber) << 16) | ((uint8_t) *StartBusNumber) << 8 | PrimaryBus
              );
            if (xUSLPciIsDevicePresent (MAKE_SBDFO (0, *StartBusNumber, 0, 0, 0))) {
              SkipScanStatus =  PciScanEarlyLink (StartBusNumber, UsbController);
            } else {
              xUSLPciReadModifyWrite32 (
                Device.AddressValue | 0x18,
                0xFF000000,
                0
                );
              (*StartBusNumber)--;
            }
          }
          break;
        case  PcieDeviceEndPoint:
        case  PcieDeviceLegacyEndPoint:
          Value32 = xUSLPciRead32 (Device.AddressValue);
          MPIO_TRACEPOINT (SIL_TRACE_INFO, "  Device DID/VID = 0x%08x\n", Value32);
          Value32 = xUSLPciRead32 (Device.AddressValue | 0x8);
          MPIO_TRACEPOINT (SIL_TRACE_INFO, "  B%xD0F0x08 = 0x%x\n", Device.Address.Bus, Value32);
          if (((Value32 >> 24) & 0xFF) == 0x0C) {
            MPIO_TRACEPOINT (SIL_TRACE_INFO, " Found the Promontory USB controller \n");
            *UsbController = 1;
            SkipScanStatus = SCAN_SKIP_FUNCTIONS | SCAN_SKIP_DEVICES;
            return SkipScanStatus;
          }
          break;
        default:
          SkipScanStatus = SCAN_SKIP_FUNCTIONS;
          break;
        }
      }
      if ((SkipScanStatus & SCAN_SKIP_FUNCTIONS) != 0) {
        Function = LastFunction + 1;
        SkipScanStatus &= ~ ((uint32_t) SCAN_SKIP_FUNCTIONS);
      }
    }
    if ((SkipScanStatus & SCAN_SKIP_DEVICES) != 0) {
      DeviceNumber = 0x20;
      SkipScanStatus &= ~ ((uint32_t) SCAN_SKIP_DEVICES);
    }
  }

  MPIO_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SkipScanStatus;
}

static void
PciFreeEarlyLinkResource (
  uint8_t *StartBusNumber
  )
{
  uint8_t             DeviceNumber;
  uint8_t             Function;
  uint8_t             PrimaryBus;
  PCI_ADDR            Device;
  PCIE_DEVICE_TYPE    DeviceType;
  uint32_t            LastFunction;

  MPIO_TRACEPOINT (SIL_TRACE_ENTRY, "(Bus=0x%x)\n", *StartBusNumber);
  Device.AddressValue = 0;
  PrimaryBus = *StartBusNumber;
  for (DeviceNumber = 0; DeviceNumber <= 0x1F; DeviceNumber++) {
    Device.AddressValue = MAKE_SBDFO (0, PrimaryBus, DeviceNumber, 0, 0);
    if (!xUSLPciIsDevicePresent (Device.AddressValue)) {
      continue;
    }
    LastFunction = 0x0;
    if (xUSLPciIsMultiFunctionDevice (Device.AddressValue)) {
      LastFunction = 0x7;
    }
    for (Function = 0; Function <= LastFunction; Function++) {
      Device.AddressValue = MAKE_SBDFO (0, PrimaryBus, DeviceNumber, Function, 0);
      if (xUSLPciIsDevicePresent (Device.AddressValue)) {
        DeviceType = xUSLPciLibbGetPcieDeviceType (Device.AddressValue);
        switch (DeviceType) {
        case  PcieDeviceRootComplex:
        case  PcieDeviceUpstreamPort:
          (*StartBusNumber)++;
          PciFreeEarlyLinkResource (StartBusNumber);
          xUSLPciReadModifyWrite32 (
            Device.AddressValue | 0x18,
            0xFF000000,
            0
            );
          break;
        case  PcieDeviceDownstreamPort:
          if (!xUSLPciIsSlotImplemented (Device.AddressValue)) {
            (*StartBusNumber)++;
            PciFreeEarlyLinkResource (StartBusNumber);
            xUSLPciReadModifyWrite32 (
              Device.AddressValue | 0x18,
              0xFF000000,
              0
              );
            (*StartBusNumber)--;
          }
          break;
        case  PcieDeviceEndPoint:
        case  PcieDeviceLegacyEndPoint:
          break;
        default:
          break;
        }
      }
    }
  }

  MPIO_TRACEPOINT (SIL_TRACE_EXIT, "\n");
}

/**----------------------------------------------------------------------------------------
 * MpioCfgEarlyLinkAuthentication
 *
 * @brief Interface to perform early link authentication
 *
 * @details This function is called once for each socket
 *
 * @param  SilContext        Input buffer for the Sil Context structure.
 *                           The Host is responsible for initializing
 *                           the data in the SIL_CONTEXT structure.
 * @param  GnbHandle         Pointer to the Silicon Descriptor for this node
 * @param  EarlyLinkAddress  Address of the of early trained PCIe engine port
 *
 * @returns   Nothing
 * @retval    Nothing
 **/
void
MpioCfgEarlyLinkAuthentication (
  SIL_CONTEXT      *SilContext,
  GNB_HANDLE       *GnbHandle,
  PCI_ADDR         EarlyLinkAddress
  )
{
  uint8_t                    SubBusNumber;
  uint8_t                    UsbController;
  uint32_t                   MpioArg[6];
  PCI_ADDR                   NbioAddress;
  MPIOCLASS_COMMON_INPUT_BLK *SilDataCommon;

  MPIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  SilDataCommon = (MPIOCLASS_COMMON_INPUT_BLK *)xUslFindStructure(SilContext,
    SilId_MpioClass,
    MPIOCLASS_COMMON_INSTANCE
    );

  assert(SilDataCommon != NULL);

  UsbController = 0;
  SubBusNumber = (uint8_t) (EarlyLinkAddress.Address.Bus + 2);
  xUSLPciReadModifyWrite32 (
    EarlyLinkAddress.AddressValue | 0x18,
    0xFF000000,
    (0xFF << 16) | (SubBusNumber) << 8 | EarlyLinkAddress.Address.Bus
    );

  PciScanEarlyLink (&SubBusNumber, &UsbController);

  xUSLPciReadModifyWrite32 (
    EarlyLinkAddress.AddressValue | 0x18,
    0xFF00FFFF,
    (SubBusNumber << 16)
    );

  if (UsbController == 1) {
    memset(MpioArg, 0x00, sizeof (MpioArg));
    NbioAddress.AddressValue = 0x0;
    NbioAddress.Address.Bus = (uint8_t)(EarlyLinkAddress.Address.Bus + 2);
    MpioArg[0] = NbioAddress.AddressValue;
    MpioArg[1] = SilDataCommon->CfgSkipPspMessage;
    MPIO_TRACEPOINT(SIL_TRACE_INFO, "BIOS_MPIO_MSG_AUTHENTICATE_CHIPSET\n");
    MPIO_TRACEPOINT(SIL_TRACE_INFO,
                    "Args = 0x%x | 0x%x | 0x%x | 0x%x | 0x%x | 0x%x\n",
                    MpioArg[0],
                    MpioArg[1],
                    MpioArg[2],
                    MpioArg[3],
                    MpioArg[4],
                    MpioArg[5]
                    );
    MpioServiceRequestCommon (
      SilContext,
      GnbHandle->Address,
      BIOS_MPIO_MSG_AUTHENTICATE_CHIPSET,
      MpioArg,
      0
      );
  }

  SubBusNumber = (uint8_t) (EarlyLinkAddress.Address.Bus + 2);
  PciFreeEarlyLinkResource (&SubBusNumber);

  xUSLPciReadModifyWrite32 (
    EarlyLinkAddress.AddressValue | 0x18,
    0xFF000000,
    0
    );

  MPIO_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/*
 *=========================================================================================
 * Timepoint before DXIO firmware initialization begins
 *=========================================================================================
 */

/**-------------------------------------------------------------------
 * MpioCfgBeforeDxioInit
 *
 * @brief Configuration Timepoint before DXIO firmware initialization starts
 *
 * @param   SilContext           A context structure through which host firmware defined data
 *                               can be passed to openSIL. The host firmware is responsible
 *                               for initializing the SIL_CONTEXT structure.
 * @param   Pcie                 Pointer silicon complex descriptor
 * @param   ComplexDescriptor    Pointer to platform complex descriptor
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
MpioCfgBeforeDxioInit (
  SIL_CONTEXT               *SilContext,
  PCIe_PLATFORM_CONFIG      *Pcie,
  MPIO_COMPLEX_DESCRIPTOR   *ComplexDescriptor
  )
{
  MPIO_COMPLEX_DESCRIPTOR       *LocalDescriptor;
  MPIO_COMMON_2_REV_XFER_BLOCK  *MpioXferTable;

  if (SilGetCommon2RevXferTable(SilContext, SilId_MpioClass, (void **)(&MpioXferTable)) != SilPass) {
    return;
  }

  MpioXferTable->MpioConfigurePspp(SilContext, Pcie);

  LocalDescriptor = ComplexDescriptor;
  while (LocalDescriptor != NULL) {
    MPIO_TRACEPOINT(SIL_TRACE_INFO, " Socket %d\n", LocalDescriptor->SocketId);
    MpioManageTopology(LocalDescriptor);
    MpioXferTable->MpioTopologyWorkarounds(SilContext, LocalDescriptor);

    LocalDescriptor = PcieConfigGetNextDataDescriptor(LocalDescriptor);
  }
}
