/**
 * @file  NbioPcie.c
 * @brief OpenSIL NBIO Pcie initialization prototypes
 *
 */

/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <xSIM.h>
#include <stddef.h>
#include <DF/Common/SilBaseFabricTopologyLib.h>
#include <DF/DfIp2Ip.h>
#include "Nbio.h"
#include "NbioCommon.h"
#include <SilPcie.h>
#include "NbioPcieTopologyHelper.h"
#include "NbioPcie.h"
#include "NbioPcieComplexData.h"
#include <SilSocLogicalId.h>
#include <string.h>

/*----------------------------------------------------------------------------------------*/
/**
 * PcieConfigAttachDescriptors
 *
 * This function is used to build the PCIe topology across the IOHC nodes
 * and helps connect Base IOHC node to New IOHC node.Left here points to
 * last node of Base IOHC for a given descriptor type and Right points to
 * child node on New IOHC node for a given descriptor type, which gets connected
 * as peer to last node(Left) of Base IOHC for a given descriptor type.
 *
 * @param  Type     Descriptor type
 * @param  Base     Base descriptor
 * @param  New      New  descriptor
 */
static void PcieConfigAttachDescriptors ( uint32_t Type, PCIe_DESCRIPTOR_HEADER *Base, PCIe_DESCRIPTOR_HEADER *New)
{
  PCIe_DESCRIPTOR_HEADER  *Left;
  PCIe_DESCRIPTOR_HEADER  *Right;
  PCIe_DESCRIPTOR_HEADER  *Child;

  Left = NULL;
  Child = PcieConfigGetChild (Type, Base);
  if (Child != NULL) {
    Left = PcieConfigGetPeer (DESCRIPTOR_TERMINATE_GNB, Child);
  }

  Right = PcieConfigGetChild (Type, New);

  if (Left != NULL && Right != NULL) {
      Left->Peer = (uint16_t) ((uint8_t *) Right - (uint8_t *) Left);
      PcieConfigResetDescriptorFlags (Left, DESCRIPTOR_TERMINATE_TOPOLOGY);
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * PcieConfigAttachComplexes
 *
 * Attach configurations of two GNB to each other.
 *
 * Function will link all data structure to linked lists
 *
 * @param Base       Base complex descriptor
 * @param New        New complex descriptor
 */
static void PcieConfigAttachComplexes (PCIe_COMPLEX_CONFIG *Base, PCIe_COMPLEX_CONFIG *New)
{
  // Connect Complex
  Base->Header.Peer = (uint16_t) ((uint8_t *) New - (uint8_t *) Base);
  PcieConfigResetDescriptorFlags (Base, DESCRIPTOR_TERMINATE_TOPOLOGY);
  // Connect Silicon
  PcieConfigAttachDescriptors (DESCRIPTOR_SILICON, &Base->Header, &New->Header);
  // Connect Wrappers
  PcieConfigAttachDescriptors (DESCRIPTOR_PCIE_WRAPPER, &Base->Header, &New->Header);
  // Connect Engines
  PcieConfigAttachDescriptors (DESCRIPTOR_PCIE_ENGINE, &Base->Header, &New->Header);
}

/*----------------------------------------------------------------------------------------*/
/*
 * GetPcieComplexSize
 * @brief Function to return PCIe Complex Data size
 *
 * @param Socket                    The target root bridge's socket identifier.
 * @param RootBridgeIndex           The target root bridge identifier
 *
 * @retval                          return IOHC data structure size
 */
static
uint32_t
GetPcieComplexSize (
  uint32_t   SocketIndex,
  uint32_t   RootBridgeIndex
  )
{
  uint32_t          ComplexSize;

  ComplexSize = GetPcieComplexSizeRS(SocketIndex, RootBridgeIndex);

  return ComplexSize;
}


/*----------------------------------------------------------------------------------------*/
/*
 * GetPcieComplexPointer
 * @brief Function to return PCIe Complex Data Pointer
 *
 * @param Socket                    The target root bridge's socket identifier.
 * @param RootBridgeIndex           The target root bridge identifier.
 *
 * @retval                          return IOHC default data initilalizer.
 */
static
void*  GetPcieComplexPointer (
  uint32_t   SocketIndex,
  uint32_t   RootBridgeIndex
  )
{

  void              *ComplexPtr;

  ComplexPtr = GetPcieComplexPointerRS(SocketIndex, RootBridgeIndex);

  return ComplexPtr;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Create internal PCIe configuration topology
 *
 * @retval     SIL_STATUS
 */

SIL_STATUS PcieConfigurationInit (void)
{
  SIL_STATUS                          Status = SilPass;
  PCIe_PLATFORM_CONFIG                *Pcie = NULL;
  NORTH_BRIDGE_PCIE_SIB               *GnbPcieSib;
  uint32_t                            ComplexIndex = 0;
  void                                *Buffer = NULL;
  const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP  *FabricIdMap = NULL;
  PCIe_SILICON_CONFIG                 *Silicon;
  uint32_t                            NumberOfProcessors;
  uint32_t                            TotalNumberOfDie;
  uint32_t                            TotalNumberOfRootBridges;
  ROOT_BRIDGE_LOCATION                SystemFchRootBridgeLocation;
  ROOT_BRIDGE_LOCATION                SystemSmuRootBridgeLocation;
  uint32_t                            SystemIdOffset;
  uint32_t                            SystemFabricID;
  uint32_t                            BusNumberBase;
  uint32_t                            BusNumberLimit;
  uint32_t                            SocketIndex;
  uint32_t                            DieIndex;
  uint32_t                            RootBridgeIndex;
  uint32_t                            PhysicalRootBridgeNumber;
  bool                                HasFchDevice;
  bool                                HasSystemMgmtUnit;
  uint32_t                            ComplexesDataLength;
  DF_IP2IP_API*                       DfIp2IpApi;

  NBIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Status = SilGetIp2IpApi (SilId_DfClass, (void **)&DfIp2IpApi);
  assert (Status == SilPass);

  DfIp2IpApi->DfGetSystemInfo (
    &NumberOfProcessors,
    &TotalNumberOfDie,
    &TotalNumberOfRootBridges,
    &SystemFchRootBridgeLocation,
    &SystemSmuRootBridgeLocation
    );

  ComplexesDataLength = 0;
  for (SocketIndex = 0; SocketIndex < NumberOfProcessors; SocketIndex++) {
    NBIO_TRACEPOINT (SIL_TRACE_INFO, "  Count Socket %d\n", SocketIndex);
    DfIp2IpApi->DfGetProcessorInfo (SocketIndex, &TotalNumberOfDie, &TotalNumberOfRootBridges);
    for (DieIndex = 0; DieIndex < TotalNumberOfDie; DieIndex++) {
      NBIO_TRACEPOINT (SIL_TRACE_INFO, "    Count Die %d\n", DieIndex);
      DfIp2IpApi->DfGetDieInfo (SocketIndex, DieIndex, &TotalNumberOfRootBridges, &SystemIdOffset, &FabricIdMap);
      for (RootBridgeIndex = 0; RootBridgeIndex < TotalNumberOfRootBridges; RootBridgeIndex++) {
        NBIO_TRACEPOINT (SIL_TRACE_INFO, "      Count RootBridge %d\n", RootBridgeIndex);
        DfIp2IpApi->DfGetRootBridgeInfo (
          SocketIndex,
          DieIndex,
          RootBridgeIndex,
          &SystemFabricID,
          &BusNumberBase,
          &BusNumberLimit,
          &PhysicalRootBridgeNumber,
          &HasFchDevice,
          &HasSystemMgmtUnit
          );
        ComplexesDataLength += GetPcieComplexSize (SocketIndex, (uint32_t)PhysicalRootBridgeNumber);
        NBIO_TRACEPOINT (SIL_TRACE_INFO, " New ComplexesDataLength = 0x%x\n", ComplexesDataLength);
      }
    }
  }
  // Allocate PCIe Complex space (maximum possibly needed)
  GnbPcieSib = SilCreateInfoBlock(SilId_NorthBridgePcie,           // ID
            sizeof(NORTH_BRIDGE_PCIE_SIB) + ComplexesDataLength,  // size
            0,                                                      // Instance
            0,                                                      // rev-major
            1);                                                     // rev-minor

    // Build PCIe Complex
  ComplexIndex = 0;
  if (GnbPcieSib != NULL) {
    Pcie = &(GnbPcieSib->PciePlatformConfig);
    memset (GnbPcieSib, 0, sizeof(NORTH_BRIDGE_PCIE_SIB));
    PcieConfigAttachChild (&Pcie->Header, &Pcie->ComplexList[ComplexIndex].Header);
    PcieConfigSetDescriptorFlags (
      Pcie,
      DESCRIPTOR_PLATFORM | DESCRIPTOR_TERMINATE_LIST | DESCRIPTOR_TERMINATE_TOPOLOGY
      );

    Buffer = (void *)&(GnbPcieSib->ComplexConfigs);
    for (SocketIndex = 0; SocketIndex < NumberOfProcessors; SocketIndex++) {
      NBIO_TRACEPOINT (SIL_TRACE_INFO, "  Process Socket %d\n", SocketIndex);
      DfIp2IpApi->DfGetProcessorInfo (SocketIndex, &TotalNumberOfDie, &TotalNumberOfRootBridges);
      for (DieIndex = 0; DieIndex < TotalNumberOfDie; DieIndex++) {
        NBIO_TRACEPOINT (SIL_TRACE_INFO, "    Process Die %d\n", DieIndex);
        DfIp2IpApi->DfGetDieInfo (SocketIndex, DieIndex, &TotalNumberOfRootBridges, &SystemIdOffset, &FabricIdMap);
        for (RootBridgeIndex = 0; RootBridgeIndex < TotalNumberOfRootBridges; RootBridgeIndex++) {
          NBIO_TRACEPOINT (SIL_TRACE_INFO, "      Process  RootBridge %d\n", RootBridgeIndex);
          DfIp2IpApi->DfGetRootBridgeInfo (
            SocketIndex,
            DieIndex,
            RootBridgeIndex,
            &SystemFabricID,
            &BusNumberBase,
            &BusNumberLimit,
            &PhysicalRootBridgeNumber,
            &HasFchDevice,
            &HasSystemMgmtUnit
            );
          NBIO_TRACEPOINT (SIL_TRACE_INFO, "        Bus Number 0x%x to 0x%x\n", BusNumberBase, BusNumberLimit);
          //Attach Complex to Silicon which will be created by PcieFmBuildComplexConfiguration
          PcieConfigAttachChild (&Pcie->ComplexList[ComplexIndex].Header, &((PCIe_SILICON_CONFIG *) Buffer)->Header);

          //Attach Complex to Pcie
          PcieConfigAttachParent (&Pcie->Header, &Pcie->ComplexList[ComplexIndex].Header);
          PcieConfigSetDescriptorFlags (
            &Pcie->ComplexList[ComplexIndex],
            DESCRIPTOR_COMPLEX | DESCRIPTOR_TERMINATE_LIST | DESCRIPTOR_TERMINATE_GNB | DESCRIPTOR_TERMINATE_TOPOLOGY);
          //Add copy from PcieComplex to Buffer using memcopy
          memcpy(
            Buffer,
            GetPcieComplexPointer(
              SocketIndex,
              (uint32_t)PhysicalRootBridgeNumber
              ),
            GetPcieComplexSize (
              SocketIndex,
              (uint32_t)PhysicalRootBridgeNumber
              )
            );

          Silicon = (PCIe_SILICON_CONFIG*) PcieConfigGetChild (DESCRIPTOR_SILICON, &((&Pcie->ComplexList[ComplexIndex])->Header));
          if (Silicon == NULL) {
            NBIO_TRACEPOINT (SIL_TRACE_ERROR, "Silicon structure came back NULL.\n");
            assert(Silicon != NULL);
          } else {
            Silicon->SocketId = (uint8_t) SocketIndex;
            Silicon->DieNumber = (uint8_t) DieIndex;
            if (TotalNumberOfRootBridges > 1) {
              Silicon->RBIndex = (uint8_t) PhysicalRootBridgeNumber;
            } else {
              Silicon->RBIndex = 0;
            }
            Silicon->LogicalRBIndex = (uint8_t) RootBridgeIndex;
            Silicon->InstanceId = (uint8_t) ComplexIndex;
            Silicon->Address.Address.Bus = (uint32_t)BusNumberBase;
            Silicon->BusNumberLimit = (uint8_t) BusNumberLimit;
            Silicon->StartLane += (uint16_t) (0);
            if (TotalNumberOfRootBridges > 1) {
              Silicon->EndLane += (uint16_t) (130);
            }
            Silicon->SbPresent = HasFchDevice;
            Silicon->SmuPresent = HasSystemMgmtUnit;
          }
          while (Silicon != NULL) {
            PcieConfigAttachParent (&Pcie->ComplexList[ComplexIndex].Header, &Silicon->Header);
            Silicon = (PCIe_SILICON_CONFIG *) PcieConfigGetNextTopologyDescriptor (
              Silicon,
              DESCRIPTOR_TERMINATE_TOPOLOGY
              );
          }

          if (ComplexIndex > 0) {
            PcieConfigAttachComplexes (&Pcie->ComplexList[ComplexIndex - 1], &Pcie->ComplexList[ComplexIndex]);
          }
          Buffer = (void *) (((uint8_t *) Buffer) +
                              GetPcieComplexSize (SocketIndex, (uint32_t)PhysicalRootBridgeNumber));
          ComplexIndex++;
        }
      }
    }

    // Initialize pointer to be optionally used for chipset
    Pcie->Header.Parent = (uint16_t) ((uintptr_t) Buffer - (uintptr_t) &Pcie->Header);
    NBIO_TRACEPOINT (SIL_TRACE_INFO, "Pcie->Header.Parent = 0x%x\n", Pcie->Header.Parent);

    // Initialize remaining PcieComplexData defaults for all engines and wrappers
    PcieFixupComplexDefaults (Pcie);
    // Use logical lane numbers to generate physical as well as account for port reversals
    // For use with DXIO
    PcieFixupPlatformConfig (Pcie);

    // Debug output to view complex
    PcieConfigDebugDump (Pcie);

    PcieEnumerateAndHarvestWrappers(Pcie);
  }
  NBIO_TRACEPOINT (SIL_TRACE_EXIT, "Status [0x%x]\n", Status);

  return Status;
}
