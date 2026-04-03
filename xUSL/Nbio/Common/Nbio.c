/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  Nbio.c
 * @brief OpenSIL NBIO initialization
 *
 */

#include <xSIM.h>
#include <DF/DfIp2Ip.h>
#include <RcMgr/RcMgrIp2Ip.h>
#include <Nbio/NbioIp2Ip.h>
#include <string.h>
#include <CommonLib/SmnAccess.h>
#include <APOB/Common/ApobCmn.h>
#include <APOB/ApobIp2Ip.h>

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
void
PcieConfigAttachDescriptors (
  uint32_t Type,
  PCIe_DESCRIPTOR_HEADER *Base,
  PCIe_DESCRIPTOR_HEADER *New
  )
{
  PCIe_DESCRIPTOR_HEADER  *Left;
  PCIe_DESCRIPTOR_HEADER  *Right;
  PCIe_DESCRIPTOR_HEADER  *Child;

  Left = NULL;
  Child = PcieConfigGetChild(Type, Base);
  if (Child != NULL) {
    Left = PcieConfigGetPeer(DESCRIPTOR_TERMINATE_GNB, Child);
  }

  Right = PcieConfigGetChild(Type, New);

  if (Left != NULL && Right != NULL) {
    Left->Peer = (uint16_t) ((uint8_t *) Right - (uint8_t *) Left);
    PcieConfigResetDescriptorFlags(Left, DESCRIPTOR_TERMINATE_TOPOLOGY);
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * NbioPcieConfigAttachComplexes
 *
 * Attach configurations of two GNB to each other.
 *
 * Function will link all data structure to linked lists
 *
 * @param Base       Base complex descriptor
 * @param New        New complex descriptor
 */
static
void
NbioPcieConfigAttachComplexes (
  PCIe_COMPLEX_CONFIG *Base,
  PCIe_COMPLEX_CONFIG *New
  )
{
  // Connect Complex
  Base->Header.Peer = (uint16_t) ((uint8_t *) New - (uint8_t *) Base);
  PcieConfigResetDescriptorFlags(Base, DESCRIPTOR_TERMINATE_TOPOLOGY);
  // Connect Silicon
  PcieConfigAttachDescriptors(DESCRIPTOR_SILICON, &Base->Header, &New->Header);
  // Connect Wrappers
  PcieConfigAttachDescriptors(DESCRIPTOR_PCIE_WRAPPER, &Base->Header, &New->Header);
  // Connect Engines
  PcieConfigAttachDescriptors(DESCRIPTOR_PCIE_ENGINE, &Base->Header, &New->Header);
}

/*----------------------------------------------------------------------------------------*/
/**
 * PcieConfigurationInit
 *
 * @brief    Create internal PCIe configuration topology
 *
 * @details  Create internal PCIe configuration topology based on DF system info
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @return  SIL_STATUS  initialization status
 *
 * @retval  success      for successful initialization
 * @retval  SilNotFound  if IP transfer table was not found
 */
SIL_STATUS
PcieConfigurationInit (
  SIL_CONTEXT  *SilContext
  )
{
  SIL_STATUS                          Status = SilPass;
  void                                *Buffer = NULL;
  PCIe_PLATFORM_CONFIG                *Pcie = NULL;
  PCIe_SILICON_CONFIG                 *Silicon;
  NORTH_BRIDGE_PCIE_SIB               *GnbPcieSib;
  const AMD_FABRIC_TOPOLOGY_DIE_DEVICE_MAP  *FabricIdMap = NULL;
  uint32_t                            ComplexesDataLength;
  uint32_t                            ComplexIndex = 0;
  uint32_t                            NumberOfProcessors;
  uint32_t                            TotalNumberOfDie;
  uint32_t                            TotalNumberOfRootBridges;
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
  ROOT_BRIDGE_LOCATION                SystemFchRootBridgeLocation;
  ROOT_BRIDGE_LOCATION                SystemSmuRootBridgeLocation;
  DF_IP2IP_API                        *DfIp2IpApi;
  NBIO_COMMON_2_REV_XFER_BLOCK        *NbioXfer;

  NBIO_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (SilGetCommon2RevXferTable(SilContext, SilId_NbioClass, (void **)(&NbioXfer)) != SilPass) {
    return SilNotFound;
  }
  if (SilGetIp2IpApi(SilContext, SilId_DfClass, (void **)(&DfIp2IpApi)) != SilPass) {
    return SilNotFound;
  }

  DfIp2IpApi->DfGetSystemInfo(SilContext,
    &NumberOfProcessors,
    &TotalNumberOfDie,
    &TotalNumberOfRootBridges,
    &SystemFchRootBridgeLocation,
    &SystemSmuRootBridgeLocation
    );

  ComplexesDataLength = 0;
  for (SocketIndex = 0; SocketIndex < NumberOfProcessors; SocketIndex++) {
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "  Count Socket %d\n", SocketIndex);
    DfIp2IpApi->DfGetProcessorInfo(SilContext, SocketIndex, &TotalNumberOfDie, &TotalNumberOfRootBridges);
    for (DieIndex = 0; DieIndex < TotalNumberOfDie; DieIndex++) {
      NBIO_TRACEPOINT(SIL_TRACE_INFO, "    Count Die %d\n", DieIndex);
      DfIp2IpApi->DfGetDieInfo(SilContext,
        SocketIndex,
        DieIndex,
        &TotalNumberOfRootBridges,
        &SystemIdOffset,
        &FabricIdMap
        );
      for (RootBridgeIndex = 0; RootBridgeIndex < TotalNumberOfRootBridges; RootBridgeIndex++) {
        NBIO_TRACEPOINT(SIL_TRACE_INFO, "      Count RootBridge %d\n", RootBridgeIndex);

        ComplexesDataLength += NbioXfer->NbioGetPcieComplexSize(SocketIndex, DieIndex, RootBridgeIndex);
        NBIO_TRACEPOINT(SIL_TRACE_INFO, " New ComplexesDataLength = 0x%x\n", ComplexesDataLength);
      }
    }
  }
  // Allocate PCIe Complex space (maximum possibly needed) in the Nbio IP Block but a different instance
  GnbPcieSib = SilCreateInfoBlock(SilContext,
    SilId_NbioClass,                                      // ID
    sizeof (NORTH_BRIDGE_PCIE_SIB) + ComplexesDataLength, // size
    NBIOPCIECLASS_INSTANCE,                               // Instance
    NBIOCLASS_MAJOR_REV,                                  // rev-major
    NBIOCLASS_MINOR_REV
    );                                 // rev-minor

  // Build PCIe Complex
  ComplexIndex = 0;
  if (GnbPcieSib != NULL) {
    Pcie = &(GnbPcieSib->PciePlatformConfig);
    memset(GnbPcieSib, 0, sizeof (NORTH_BRIDGE_PCIE_SIB));
    PcieConfigAttachChild(&Pcie->Header, &Pcie->ComplexList[ComplexIndex].Header);
    PcieConfigSetDescriptorFlags(Pcie,
      DESCRIPTOR_PLATFORM | DESCRIPTOR_TERMINATE_LIST | DESCRIPTOR_TERMINATE_TOPOLOGY
      );

    Buffer = (void *)&(GnbPcieSib->ComplexConfigs);
    Pcie->RootBridgesPerSocket = (uint8_t) TotalNumberOfRootBridges;
    for (SocketIndex = 0; SocketIndex < NumberOfProcessors; SocketIndex++) {
      NBIO_TRACEPOINT(SIL_TRACE_INFO, "  Process Socket %d\n", SocketIndex);
      for (DieIndex = 0; DieIndex < TotalNumberOfDie; DieIndex++) {
        NBIO_TRACEPOINT(SIL_TRACE_INFO, "    Process Die %d\n", DieIndex);
        for (RootBridgeIndex = 0; RootBridgeIndex < TotalNumberOfRootBridges; RootBridgeIndex++) {
          NBIO_TRACEPOINT(SIL_TRACE_INFO, "      Process  RootBridge %d\n", RootBridgeIndex);
          DfIp2IpApi->DfGetRootBridgeInfo(SilContext,
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
          NBIO_TRACEPOINT(SIL_TRACE_INFO, "        Bus Number 0x%x to 0x%x\n", BusNumberBase, BusNumberLimit);
          //Attach Complex to Silicon which will be created by PcieFmBuildComplexConfiguration
          PcieConfigAttachChild(&Pcie->ComplexList[ComplexIndex].Header, &((PCIe_SILICON_CONFIG *) Buffer)->Header);

          //Attach Complex to Pcie
          PcieConfigAttachParent(&Pcie->Header, &Pcie->ComplexList[ComplexIndex].Header);
          PcieConfigSetDescriptorFlags(&Pcie->ComplexList[ComplexIndex],
            DESCRIPTOR_COMPLEX | DESCRIPTOR_TERMINATE_LIST | DESCRIPTOR_TERMINATE_GNB | DESCRIPTOR_TERMINATE_TOPOLOGY
            );
          //Add copy from PcieComplex to Buffer using memcopy
          memcpy(Buffer,
            NbioXfer->NbioGetPcieComplexPointer(SocketIndex, DieIndex, RootBridgeIndex),
            NbioXfer->NbioGetPcieComplexSize(SocketIndex, DieIndex, RootBridgeIndex)
            );

          Silicon = (PCIe_SILICON_CONFIG *) PcieConfigGetChild(DESCRIPTOR_SILICON,
            &((&Pcie->ComplexList[ComplexIndex])->Header)
            );
          if (Silicon == NULL) {
            NBIO_TRACEPOINT(SIL_TRACE_ERROR, "Silicon structure came back NULL.\n");
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
            Silicon->Address.Address.Bus = (uint32_t) (BusNumberBase % MAX_PCI_BUS_NUMBER_PER_SEGMENT);
            Silicon->Address.Address.Segment = BusNumberBase / MAX_PCI_BUS_NUMBER_PER_SEGMENT;
            Silicon->BusNumberLimit = (uint8_t) (BusNumberLimit % MAX_PCI_BUS_NUMBER_PER_SEGMENT);
            Silicon->StartLane += (uint16_t) (0);
            if (TotalNumberOfRootBridges > 1) {
              Silicon->EndLane += (uint16_t) (130);
            }
            Silicon->SbPresent = HasFchDevice;
            Silicon->SmuPresent = HasSystemMgmtUnit;
          }
          while (Silicon != NULL) {
            PcieConfigAttachParent(&Pcie->ComplexList[ComplexIndex].Header, &Silicon->Header);
            Silicon = (PCIe_SILICON_CONFIG *) PcieConfigGetNextTopologyDescriptor(Silicon,
              DESCRIPTOR_TERMINATE_TOPOLOGY
              );
          }

          if (ComplexIndex > 0) {
            NbioPcieConfigAttachComplexes(&Pcie->ComplexList[ComplexIndex - 1], &Pcie->ComplexList[ComplexIndex]);
          }
          Buffer = (void *) (((uint8_t *) Buffer) +
            NbioXfer->NbioGetPcieComplexSize(SocketIndex, DieIndex, RootBridgeIndex));
          ComplexIndex++;
        }
      }
    }

    // Initialize pointer to be optionally used for chipset
    Pcie->Header.Parent = (uint16_t) ((uintptr_t) Buffer - (uintptr_t) &Pcie->Header);
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "Pcie->Header.Parent = 0x%x\n", Pcie->Header.Parent);

    // Initialize remaining PcieComplexData defaults for all engines and wrappers
    NbioXfer->NbioPcieFixupComplexDefaults(SilContext, Pcie);
    // Use logical lane numbers to generate physical as well as account for port reversals
    // For use with DXIO
    NbioXfer->NbioPcieFixupPlatformConfig(SilContext, Pcie);

    // Debug output to view complex
    PcieConfigDebugDump(Pcie);

    NbioXfer->NbioEnumerateHarvestWrappers(Pcie);
  } else {
    assert(GnbPcieSib != NULL);
    Status = SilAborted;
  }

  NBIO_TRACEPOINT(SIL_TRACE_EXIT, "Status [0x%x]\n", Status);
  return Status;
}

/**
 * NbioIoApicInit
 *
 * @brief  This function is responsible to allocate the mmio space for IoApic.
 *         Also assign the IoApic base ID for each IOHC instance.
 *
 * @param  SilContext            A context structure through which host firmware defined data
 *                               can be passed to openSIL. The host firmware is responsible
 *                               for initializing the SIL_CONTEXT structure.
 * @param  NBIOCLASS_DATA_BLOCK  Pointer to NBIO Data Block
 *
 * @return SIL_STATUS
 * @retval SilPass - everything is OK
 * @retval SilAborted- Failed to allocate MMIO resources.
 */
SIL_STATUS
NbioIoApicInit (
  SIL_CONTEXT           *SilContext,
  NBIOCLASS_DATA_BLOCK  *NbioIpBlockData
  )
{
  FABRIC_TARGET                      MmioTarget;
  FABRIC_MMIO_ATTRIBUTE              MmioAttr;
  uint64_t                           IoapicMmioSize;
  uint64_t                           IoapicMmioBase;
  GNB_HANDLE                         *GnbHandle;
  SIL_STATUS                         Status;
  RCMGR_IP2IP_API                    *RcMgrIp2Ip;
  NBIO_COMMON_2_REV_XFER_BLOCK       *NbioXfer;
  APOB_IP2IP_API                     *ApobIp2IpApi;
  APOB_SOC_DIE_INFO                  SocMaxDieInfo;

  NBIO_TRACEPOINT(SIL_TRACE_INFO, "Entry\n");

  if (SilGetCommon2RevXferTable(SilContext, SilId_NbioClass, (void **)(&NbioXfer)) != SilPass) {
    return SilNotFound;
  }
  if (NbioIpBlockData == NULL) {
    return SilInvalidParameter;
  }

  Status = SilGetIp2IpApi(SilContext, SilId_ApobClass, (void **) &ApobIp2IpApi);
  if ((Status != SilPass) || (ApobIp2IpApi == NULL)) {
    assert(Status == SilPass);
    return Status;
  }
  ApobIp2IpApi->ApobGetMaxDieInfo(SilContext, &SocMaxDieInfo);

  if (NbioIpBlockData->NbioConfigData.IoApicMMIOAddressReservedEnable) {
    if (SilGetIp2IpApi(SilContext, SilId_RcManager, (void **)(&RcMgrIp2Ip)) != SilPass) {
      NBIO_TRACEPOINT(SIL_TRACE_ERROR, " MMIO allocator API is not found.\n");
      return SilNotFound;
    }

    GnbHandle = GetGnbHandle(SilContext);
    while (GnbHandle != NULL) {
      memset(&MmioAttr, 0, sizeof (MmioAttr));
      memset(&MmioTarget, 0, sizeof (MmioTarget));
      MmioTarget.TgtType = TARGET_PCI_BUS;
      MmioTarget.SocketNum = GnbHandle->SocketId;
      assert(MmioTarget.SocketNum < SocMaxDieInfo.MaxSocSocketsSupportedValue);
      MmioTarget.RbNum = GnbHandle->DieNumber;
      MmioTarget.PciBusNum = (uint16_t) GnbHandle->Address.Address.Bus;
      MmioTarget.PciSegNum = (uint16_t) GnbHandle->Address.Address.Segment;
      IoapicMmioSize = SIZE_1KB / 4;
      MmioAttr.ReadEnable = 1;
      MmioAttr.WriteEnable = 1;
      MmioAttr.NonPosted = 0;
      MmioAttr.MmioType = NON_PCI_DEVICE_BELOW_4G;
      IoapicMmioBase = 0;
      Status = RcMgrIp2Ip->FabricReserveMmio(SilContext,
        &IoapicMmioBase,
        &IoapicMmioSize,
        ALIGN_64K,
        MmioTarget,
        &MmioAttr
        );
      if (Status == SilPass) {
        NBIO_TRACEPOINT(SIL_TRACE_INFO,
          "IOAPIC MMIO at address 0x%x for Socket:0x%x Bus:0x%x\n",
          IoapicMmioBase,
          MmioTarget.SocketNum,
          MmioTarget.PciBusNum
          );
        ///
        /// Write the assigned memory address registers to SMN
        ///
        NbioXfer->NbioIoApicMmioAddress(GnbHandle, IoapicMmioBase);
      } else {
        NBIO_TRACEPOINT(SIL_TRACE_INFO, "Failed to allocate IoApic mmio space\n");
        return SilAborted;
      }
      GnbHandle = GnbGetNextHandle(GnbHandle);
    }
  } else {
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "We don't need reserved IOAPIC MMIO space\n");
  }

  if (NbioIpBlockData->NbioConfigData.IoApicIdPreDefineEn) {
    GnbHandle = GetGnbHandle(SilContext);
    while (GnbHandle != NULL) {
      NbioXfer->NbioIoApicPreDefId(GnbHandle, NbioIpBlockData->NbioConfigData.IoApicIdBase);
      GnbHandle = GnbGetNextHandle(GnbHandle);
    }
  } else {
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "Pre-Define IOAPIC ID disabled \n");
  }

  NBIO_TRACEPOINT(SIL_TRACE_INFO, "Exit\n");
  return SilPass;
}

/*----------------------------------------------------------------------------------------*/
/**
 * NbioGetPcieTopology
 *
 * @brief    Get instance of NBIO PCIe topology
 *
 * @details  Get instance of NBIO PCIe topology based on ID class
 *
 * @param    SilContext         A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @return   pointer to NBIO PCIe topology.
 */
void *
NbioGetPcieTopology (
  SIL_CONTEXT  *SilContext
  )
{
  return xUslFindStructure(SilContext, SilId_NbioClass, NBIOPCIECLASS_INSTANCE);
}

/**
 * NonPciBarInit
 *
 * @brief  This function assigns MMIO for a non-pci device.
 *
 * @param  SilContext      A context structure through which host firmware defined data
 *                         can be passed to openSIL. The host firmware is responsible
 *                         for initializing the SIL_CONTEXT structure.
 * @param  GnbHandle       Silicon handle to assign
 * @param  MmioBarLow      Address of low byte
 * @param  MmioBarHigh     Address of high byte
 * @param  MemorySize      Size of the allocated bar required
 * @param  Enable          Set enable bit in BAR
 * @param  LockSettings    If the allocated memory range should be locked or not
 * @param  Above4G         If MMIO above the 4G boundary should be allocated
 *
 * @return  nothing
 */
void
NonPciBarInit (
  SIL_CONTEXT  *SilContext,
  GNB_HANDLE   *GnbHandle,
  uint32_t     MmioBarLow,
  uint32_t     MmioBarHigh,
  uint32_t     MemorySize,
  bool         Enable,
  bool         LockSettings,
  bool         Above4G
  )
{
  SIL_STATUS             Status;
  FABRIC_TARGET          MmioTarget;
  FABRIC_MMIO_ATTRIBUTE  Attributes;
  uint64_t               MmioBase, Length;
  uint32_t               BarLow, BarHigh;
  RCMGR_IP2IP_API        *RcMgrIp2Ip;

  BarLow = 0;
  BarHigh = 0;
  NBIO_TRACEPOINT(SIL_TRACE_INFO,
    "Begin to allocate bars for SMN low %x high %x, size %x\n",
    MmioBarLow,
    MmioBarHigh,
    MemorySize
    );

  ///
  /// See if the given BAR have already been assigned
  ///
  BarLow = xUSLSmnRead(GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus, MmioBarLow);
  BarHigh = xUSLSmnRead(GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus, MmioBarHigh);

  if (BarLow == 0 && BarHigh == 0) {
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "Bars have not been assigned, attempting to allocate MMIO \n");
    ///
    /// Assign bars:
    /// Allocate a chunk of MMIO first
    ///
    memset(&Attributes, 0, sizeof (Attributes));
    memset(&MmioTarget, 0, sizeof (MmioTarget));
    Length = MemorySize;
    MmioTarget.TgtType = TARGET_PCI_BUS;
    MmioTarget.SocketNum = GnbHandle->SocketId;
    MmioTarget.PciBusNum = (uint16_t) GnbHandle->Address.Address.Bus;
    MmioTarget.PciSegNum = (uint16_t) GnbHandle->Address.Address.Segment;
    MmioTarget.RbNum = GnbHandle->RBIndex;
    Attributes.ReadEnable = 1;
    Attributes.WriteEnable = 1;
    Attributes.NonPosted = 0;
    MmioBase = 0;
    if (Above4G) {
      Attributes.MmioType = NON_PCI_DEVICE_ABOVE_4G;
    } else {
      Attributes.MmioType = NON_PCI_DEVICE_BELOW_4G;
    }

    NBIO_TRACEPOINT(SIL_TRACE_INFO,
      "FabricAllocateMmio : Socket %d , RB # %d\n",
      MmioTarget.SocketNum,
      MmioTarget.RbNum
      );

    if (SilGetIp2IpApi(SilContext, SilId_RcManager, (void **)(&RcMgrIp2Ip)) != SilPass) {
      NBIO_TRACEPOINT(SIL_TRACE_ERROR, " MMIO allocator API is not found.\n");
      return;
    }

    Status = RcMgrIp2Ip->FabricReserveMmio(SilContext, &MmioBase, &Length, ALIGN_1M, MmioTarget, &Attributes);

    if (Status != SilPass) {
      NBIO_TRACEPOINT(SIL_TRACE_INFO, "Allocate MMIO Fail\n");
      return;
    }
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "Allocate MMIO @0x%llx\n", MmioBase);
  } else {
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "Bars have already been assigned!\n");
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "End\n");
    return;
  }

  ///
  /// Write the assigned memory address registers to SMN
  ///
  BarLow = (uint32_t) MmioBase;
  BarHigh = (uint32_t) (MmioBase >> 32);

  xUSLSmnWrite(GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus, MmioBarLow, BarLow);
  xUSLSmnWrite(GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus, MmioBarHigh, BarHigh);

  NBIO_TRACEPOINT(SIL_TRACE_INFO, "BarLow = %x , BarHigh %x\n", BarLow, BarHigh);

  // Set enable bit separate from other bits
  if (Enable) {
    BarLow = BarLow | BIT_32(0); /// Set enable bit
    xUSLSmnWrite(0, GnbHandle->Address.Address.Bus, MmioBarLow, BarLow);
    xUSLSmnWrite(0, GnbHandle->Address.Address.Bus, MmioBarHigh, BarHigh);
  }

  // Set lock bit separate from other bits
  if (LockSettings) {
    BarLow = BarLow | BIT_32(1); /// Set lock bit
    xUSLSmnWrite(GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus, MmioBarLow, BarLow);
    xUSLSmnWrite(GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus, MmioBarHigh, BarHigh);
  }
  NBIO_TRACEPOINT(SIL_TRACE_INFO, "End\n");
}


/**
 * NonPciPspBarInit
 *
 * @brief  This function assigns non-pci MMIO for a PSP device.
 *
 * @param  GnbHandle       Silicon handle to assign
 * @param  MmioBarLow      Address of low byte
 * @param  MmioBarHigh     Address of high byte
 * @param  MemorySize      Size of the allocated bar required
 * @param  Enable          Set enable bit in BAR
 * @param  LockSettings    If the allocated memory range should be locked or not
 *
 * @return  nothing
 */
void
NonPciPspBarInit (
  SIL_CONTEXT  *SilContext,
  GNB_HANDLE   *GnbHandle,
  uint32_t     MmioBarLow,
  uint32_t     MmioBarHigh,
  uint32_t     MemorySize,
  bool         Enable,
  bool         LockSettings
  )
{
  SIL_STATUS             Status;
  FABRIC_TARGET          MmioTarget;
  FABRIC_MMIO_ATTRIBUTE  Attributes;
  uint64_t               MmioBase, Length;
  uint32_t               BarLow, BarHigh;
  RCMGR_IP2IP_API        *RcMgrIp2Ip;

  BarLow = 0;
  BarHigh = 0;
  NBIO_TRACEPOINT(SIL_TRACE_INFO,
    "Begin to allocate bars for SMN low %x high %x, size %x\n",
    MmioBarLow,
    MmioBarHigh,
    MemorySize
    );

  ///
  /// See if the given BAR have already been assigned
  ///
  BarLow = xUSLSmnRead(GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus, MmioBarLow);
  BarHigh = xUSLSmnRead(GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus, MmioBarHigh);

  if (BarLow == 0 && BarHigh == 0) {
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "Bars have not been assigned, attempting to allocate MMIO \n");
    ///
    /// Assign bars:
    /// Allocate a chunk of MMIO first
    ///
    memset(&Attributes, 0, sizeof (Attributes));
    memset(&MmioTarget, 0, sizeof (MmioTarget));
    Length = MemorySize;
    MmioTarget.TgtType = TARGET_RB;
    MmioTarget.SocketNum = GnbHandle->SocketId;
    MmioTarget.RbNum = GnbHandle->RBIndex;
    Attributes.ReadEnable = 1;
    Attributes.WriteEnable = 1;
    Attributes.NonPosted = 0;
    MmioBase = 0;
    Attributes.MmioType = NON_PCI_DEVICE_BELOW_4G;

    NBIO_TRACEPOINT(SIL_TRACE_INFO,
      "FabricAllocateMmio : Socket %d , RB # %d\n",
      MmioTarget.SocketNum,
      MmioTarget.RbNum
      );

    if (SilGetIp2IpApi(SilContext, SilId_RcManager, (void **)(&RcMgrIp2Ip)) != SilPass) {
      NBIO_TRACEPOINT(SIL_TRACE_ERROR, " MMIO allocator API is not found.\n");
      return;
    }

    Status = RcMgrIp2Ip->FabricReserveMmio(SilContext, &MmioBase, &Length, ALIGN_1M, MmioTarget, &Attributes);

    if (Status != SilPass) {
      NBIO_TRACEPOINT(SIL_TRACE_INFO, "Allocate MMIO Fail\n");
      return;
    }
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "Allocate MMIO @0x%llx\n", MmioBase);
  } else {
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "Bars have already been assigned!\n");
    NBIO_TRACEPOINT(SIL_TRACE_INFO, "End\n");
    return;
  }

  ///
  /// Write the assigned memory address registers to SMN
  ///
  BarLow = (uint32_t) MmioBase;
  BarHigh = (uint32_t) (MmioBase >> 32);

  xUSLSmnWrite(GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus, MmioBarLow, BarLow);
  xUSLSmnWrite(GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus, MmioBarHigh, BarHigh);

  NBIO_TRACEPOINT(SIL_TRACE_INFO, "BarLow = %x , BarHigh %x\n", BarLow, BarHigh);

  // Set enable bit separate from other bits
  if (Enable) {
    BarLow = BarLow | BIT_32(0); /// Set enable bit
    xUSLSmnWrite(0, GnbHandle->Address.Address.Bus, MmioBarLow, BarLow);
    xUSLSmnWrite(0, GnbHandle->Address.Address.Bus, MmioBarHigh, BarHigh);
  }

  // Set lock bit separate from other bits
  if (LockSettings) {
    BarLow = BarLow | BIT_32(8); /// Set lock bit
    xUSLSmnWrite(GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus, MmioBarLow, BarLow);
    xUSLSmnWrite(GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus, MmioBarHigh, BarHigh);
  }
  NBIO_TRACEPOINT(SIL_TRACE_INFO, "End\n");
}
