/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

/**
 *  @file CxlInit.c
 *  @brief Cxl related functions for MPIO
 */

#include <xSIM.h>
#include <string.h>
#include <Mpio/MpioIp2Ip.h>
#include <RcMgr/RcMgrIp2Ip.h>
#include <FCH/Common/FchCommonCfg.h>
#include "CxlInit.h"
#include "CxlCmn2Rev.h"
#include "CxlClass-api.h"


#define CXLCLASS_MAJOR_REV   0
#define CXLCLASS_MINOR_REV   1
#define CXLCLASS_INSTANCE    0

/**--------------------------------------------------------------------
 * GnbLibPciIsDevicePresent
 *
 * @brief Check if device present
 *
 * @details tbd
 *
 * @param[in] Address         PCI address (as described in PCI_ADDR)
 * @param[in] StdHeader       Standard configuration header
 *
 * @returns Device is or is not present
 * @retval boolean value
 */
static
bool
GnbLibPciIsDevicePresent (
  uint32_t              Address,
  void   *StdHeader
  )
{
  uint32_t  DeviceId;
  xUSLPciRead (Address, AccessWidth32, &DeviceId);
  if (DeviceId == 0xffffffff) {
    return false;
  } else {
    return true;
  }
}

/**--------------------------------------------------------------------
 * GnbLibFindPciCapability
 *
 * @brief Find PCI capability pointer
 *
 * @details tbd
 *
 * @param[in] Address         PCI address (as described in PCI_ADDR)
 * @param[in] CapabilityId    PCI capability ID
 * @param[in] StdHeader       Standard configuration header
 *
 * @returns Register address of capability pointer
 * @retval 8-bit value
 */
static
uint8_t
GnbLibFindPciCapability (
  uint32_t              Address,
  uint8_t               CapabilityId,
  void   *StdHeader
  )
{
  uint8_t     CapabilityPtr;
  uint8_t     CurrentCapabilityId;
  CapabilityPtr = 0x34;
  if (!GnbLibPciIsDevicePresent (Address, StdHeader)) {
    return  0;
  }
  while (CapabilityPtr != 0) {
    xUSLPciRead (Address | CapabilityPtr, AccessWidth8 , &CapabilityPtr);
    if (CapabilityPtr != 0) {
      xUSLPciRead (Address | CapabilityPtr , AccessWidth8 , &CurrentCapabilityId);
      if (CurrentCapabilityId == CapabilityId) {
        break;
      }
      CapabilityPtr++;
    }
  }
  return  CapabilityPtr;
}

/**--------------------------------------------------------------------
 * GnbLibPciIsPcieDevice
 *
 * @brief Check if device is PCIe device
 *
 * @details tbd
 *
 * @param[in] Address         PCI address (as described in PCI_ADDR)
 * @param[in] StdHeader       Standard configuration header
 *
 * @returns Device is or is not a PCIe device
 * @retval boolean value
 */
static
bool
GnbLibPciIsPcieDevice (
  uint32_t              Address,
  void   *StdHeader
  )
{
  if (GnbLibFindPciCapability (Address, PCIE_CAP_ID, StdHeader) != 0 ) {
    return true;
  } else {
    return false;
  }
}

/**--------------------------------------------------------------------
 * GnbLibFindPcieExtendedCapability
 *
 * @brief Find PCIe extended capability pointer
 *
 * @details tbd
 *
 * @param[in] Address               PCI address (as described in PCI_ADDR)
 * @param[in] ExtendedCapabilityId  Extended PCIe capability ID
 * @param[in] StdHeader             Standard configuration header
 *
 * @returns Register address of extended capability pointer
 * @retval 16-bit value
 */
static
uint16_t
GnbLibFindPcieExtendedCapability (
  uint32_t              Address,
  uint16_t              ExtendedCapabilityId,
  void   *StdHeader
  )
{
  uint16_t  CapabilityPtr;
  uint32_t  ExtendedCapabilityIdBlock;
  if (GnbLibPciIsPcieDevice (Address, StdHeader)) {
    CapabilityPtr = 0x100;
    xUSLPciRead (Address | CapabilityPtr , AccessWidth32 , &ExtendedCapabilityIdBlock);
    if ((ExtendedCapabilityIdBlock != 0) && ((uint16_t)ExtendedCapabilityIdBlock != 0xffff)) {
      do {
        xUSLPciRead (Address | CapabilityPtr , AccessWidth32 , &ExtendedCapabilityIdBlock);
    CXL_TRACEPOINT (
      SIL_TRACE_INFO,
      "   - Capability at 0x%x with type 0x%x\n",
      CapabilityPtr,
      (uint16_t)ExtendedCapabilityIdBlock
      );
        if ((uint16_t)ExtendedCapabilityIdBlock == ExtendedCapabilityId) {
          return  CapabilityPtr;
        }
        CapabilityPtr = (uint16_t) ((ExtendedCapabilityIdBlock >> 20) & 0xfff);
      } while (CapabilityPtr !=  0);
    }
  }
  return  0;
}

/**--------------------------------------------------------------------
 * GnbLibFindNextPcieExtendedCapability
 *
 * @brief Find the next PCIe extended capability pointer
 *
 * @details tbd
 *
 * @param[in] Address               PCI address (as described in PCI_ADDR)
 * @param[in] StartCapabilityPtr    Starting register address of extended capability pointer
 * @param[in] ExtendedCapabilityId  Extended PCIe capability ID
 * @param[in] StdHeader             Standard configuration header
 *
 * @returns Register address of extended capability pointer
 * @retval 16-bit value
 */
static
uint16_t
GnbLibFindNextPcieExtendedCapability (
  uint32_t              Address,
  uint16_t              StartCapabilityPtr,
  uint16_t              ExtendedCapabilityId,
  void   *StdHeader
  )
{
  uint16_t  NextCapabilityPtr = 0;
  uint32_t  ExtendedCapabilityIdBlock = 0;
  if (GnbLibPciIsPcieDevice (Address, StdHeader)) {
    xUSLPciRead (Address | StartCapabilityPtr , AccessWidth32 , &ExtendedCapabilityIdBlock);
    if ((ExtendedCapabilityIdBlock != 0) && ((uint16_t)ExtendedCapabilityIdBlock != 0xffff)) {
      NextCapabilityPtr = (uint16_t) ((ExtendedCapabilityIdBlock >> 20) & 0xfff);
      if(NextCapabilityPtr != 0) {
        xUSLPciRead (Address | NextCapabilityPtr , AccessWidth32 , &ExtendedCapabilityIdBlock);
        CXL_TRACEPOINT (
          SIL_TRACE_INFO,
          "   - Next Capability at 0x%x with type 0x%x\n",
          NextCapabilityPtr,
          (uint16_t)ExtendedCapabilityIdBlock
          );
        if ((uint16_t)ExtendedCapabilityIdBlock == ExtendedCapabilityId) {
          return NextCapabilityPtr;
        }
      }
    }
  }
  return 0;
}

/**--------------------------------------------------------------------
 * CxlRegisterRead
 *
 * @brief CXL register read function
 *
 * @details tbd
 *
 * @param [in] DsRcrb DsRcrb Pointer to downstream RCRB
 * @param [in] RegOffset Register offset to read
 *
 * @returns Value read from the register
 * @retval 32-bit value
 */
uint32_t
CxlRegisterRead (
  uint32_t *DsRcrb,
  uint32_t RegOffset
  )
{
  volatile  uint32_t *RegisterAddress;

  RegisterAddress = (uint32_t *) ((uintptr_t) DsRcrb + RegOffset);
  CXL_TRACEPOINT (SIL_TRACE_INFO, "MAIN_FLOW - CxlRegisterRead: %08x\n", RegisterAddress);
  return *RegisterAddress;
}

/**--------------------------------------------------------------------
 * CxlGetDvsec
 *
 * @brief Get CXL DVSEC
 *
 * @details tbd
 *
 * @param [in] Address        PCI address
 * @param [in] CapabilityPtr  Starting register address to the PCI extended capability
 * @param [in] DvsecVendorId  The DVSEC Vendor ID to find
 * @param [in] DvsecVendorId2 Alternative DVSEC Vendor ID to find
 * @param [in] DvsecId        The DVSEC ID to find
 *
 * @returns  Register address of desired DVSEC (extended capability pointer)
 * @retval 16-bit value
 *
 */
uint16_t
CxlGetDvsec (
  uint32_t  Address,
  uint16_t  DvsecVendorId,
  uint16_t  DvsecVendorId2,
  uint16_t  DvsecId
  )
{
  uint32_t DvsecValue;
  uint16_t DvsecPtr = GnbLibFindPcieExtendedCapability (Address, DVSEC_CAP, NULL);

  while (DvsecPtr != 0) {
    xUSLPciRead (
        Address | (DvsecPtr + SIL_RESERVED_21),
        AccessWidth32,
        &DvsecValue
        );

    if (((uint16_t) DvsecValue == DvsecVendorId) || ((uint16_t) DvsecValue == DvsecVendorId2)) {
      xUSLPciRead (
        Address | (DvsecPtr + DVSEC_ID_OFFSET),
        AccessWidth16,
        &DvsecValue
        );

      if ((uint16_t) DvsecValue == DvsecId) {
        return DvsecPtr;
      }
    }
    DvsecPtr = GnbLibFindNextPcieExtendedCapability(Address, DvsecPtr, DVSEC_CAP, NULL);
  }
  return 0;
}

/**--------------------------------------------------------------------
 * CxlRegisterWrite
 *
 * @brief CXL register write function
 *
 * @details tbd
 *
 * @param[in]  DsRcrb Pointer to downstream RCRB
 * @param[in]  RegOffset Register offset to write
 * @param[in]  WriteValue  Data to write into register
 *
 * @returns Nothing
 * @retval Nothing
 */
void
CxlRegisterWrite (
  uint32_t *DsRcrb,
  uint32_t RegOffset,
  uint32_t WriteValue
  )
{
  volatile  uint32_t *RegisterAddress;

  RegisterAddress = (uint32_t *) ((uintptr_t) DsRcrb + RegOffset);
  CXL_TRACEPOINT (SIL_TRACE_INFO, "MAIN_FLOW - CxlRegisterWrite: %08x\n", RegisterAddress);
  *RegisterAddress = WriteValue;
  return;
}

/**--------------------------------------------------------------------
 * CxlMmioAlloc
 *
 * @brief This function assigns MMIO for a non-pci device.
 *
 * @details tbd
 *
 * @param[in]  GnbHandle       Silicon handle to assign
 * @param[in]  MmioBarLow      Address of low byte
 * @param[in]  MmioBarHigh     Address of high byte
 * @param[in]  MemorySize      Size of the allocated bar required
 * @param[in]  Above4G         If MMIO above the 4G boundary should be allocated
 *
 * @returns Opensil status
 * @retval SilPass is valid
 */
SIL_STATUS
CxlMmioAlloc (
  GNB_HANDLE    *GnbHandle,
  uint32_t      *MmioBarLow,
  uint32_t      *MmioBarHigh,
  uint64_t      MemorySize,
  bool          Above4G
  )
{
  SIL_STATUS             Status;
  FABRIC_TARGET          MmioTarget;
  FABRIC_MMIO_ATTRIBUTE  Attributes;
  uint64_t               MmioBase, Length;
  RCMGR_IP2IP_API        *RcMgrIp2Ip;

  CXL_TRACEPOINT (SIL_TRACE_INFO, "MAIN_FLOW: Begin to allocate bars for SMN low %x high %x, size %x\n",
      MmioBarLow,
      MmioBarHigh,
      MemorySize
    );

  CXL_TRACEPOINT (SIL_TRACE_INFO, "MAIN_FLOW: Bars have not been assigned, attempting to allocate MMIO \n");

  ///
  /// Assign bars:
  /// Allocate a chunk of MMIO first
  ///
  memset (&Attributes, 0, sizeof(Attributes));
  memset (&MmioTarget, 0, sizeof(MmioTarget));
  Length = MemorySize;
  MmioTarget.TgtType = TARGET_PCI_BUS;
  MmioTarget.SocketNum = GnbHandle->SocketId;
  MmioTarget.RbNum = GnbHandle->RBIndex;
  MmioTarget.PciBusNum = (uint16_t) GnbHandle->Address.Address.Bus;
  MmioTarget.PciSegNum = (uint16_t) GnbHandle->Address.Address.Segment;
  Attributes.ReadEnable = 1;
  Attributes.WriteEnable = 1;
  Attributes.NonPosted = 0;
  MmioBase = 0;
  if (Above4G) {
    Attributes.MmioType = NON_PCI_DEVICE_ABOVE_4G;
  }
  else {
    Attributes.MmioType = NON_PCI_DEVICE_BELOW_4G;
  }

  CXL_TRACEPOINT (SIL_TRACE_INFO, "MAIN_FLOW: FabricAllocateMmio : Socket %d , RB # %d\n",
      MmioTarget.SocketNum,
      MmioTarget.RbNum
    );

  if (SilGetIp2IpApi (SilId_RcManager, (void **)(&RcMgrIp2Ip)) != SilPass) {
    CXL_TRACEPOINT (SIL_TRACE_ERROR, " MMIO allocator API is not found.\n");
    return SilNotFound;
  }

  Status = RcMgrIp2Ip->FabricReserveMmio (&MmioBase, &Length, ALIGN_1M, MmioTarget, &Attributes);

  if (Status != SilPass) {
    CXL_TRACEPOINT (SIL_TRACE_ERROR, " Allocate MMIO Fail\n");

    assert(Status != SilPass);
  } else {
    CXL_TRACEPOINT (SIL_TRACE_INFO, " Allocate MMIO @0x%llx\n", MmioBase);
  }
  ///
  /// Write the assigned memory address to the return values
  ///
  *MmioBarLow  = (uint32_t) MmioBase;
  *MmioBarHigh = (uint32_t) (MmioBase >> 32);

  CXL_TRACEPOINT (SIL_TRACE_EXIT, "\n");

  return Status;
}

/**--------------------------------------------------------------------
 * GetMem32BarSize
 *
 * @brief Calculate size of 32-bit BAR
 *
 * @details tbd
 *
 * @param[in]       EndpointBar       Pointer to engine config descriptor
 *
 * @returns Size
 * @retval 32-bit value
 */
static
uint32_t
GetMem32BarSize (
  uint32_t     EndpointBar
  )
{
  uint32_t        OriginalValue;
  uint32_t        Value;

  xUSLPciRead (EndpointBar, AccessWidth32, &OriginalValue);
  Value = 0xFFFFFFFF;
  xUSLPciWrite (EndpointBar, AccessS3SaveWidth32, &Value);
  xUSLPciRead (EndpointBar, AccessWidth32, &Value);
  xUSLPciWrite (EndpointBar, AccessS3SaveWidth32, &OriginalValue);
  Value &= 0xFFFFFFF0;
  Value = (~Value) + 1;
  return (Value);
}

/**--------------------------------------------------------------------
 * GetMem64BarSize
 *
 * @brief Calculate size of 32-bit BAR
 *
 * @details tbd
 *
 * @param[in]       EndpointBar       Pointer to engine config descriptord
 *
 * @returns Size
 * @retval 64-bit value
 */
static
uint64_t
GetMem64BarSize (
  uint32_t     EndpointBar
  )
{
  uint32_t        NextBar;
  uint32_t        OrgValue;
  uint32_t        OrgValue2;
  uint32_t        Value;
  uint64_t        Value64;

  NextBar = EndpointBar + 4;
  xUSLPciRead (EndpointBar, AccessWidth32, &OrgValue);
  xUSLPciRead (NextBar, AccessWidth32, &OrgValue2);

  Value = 0xFFFFFFFF;
  xUSLPciWrite (EndpointBar, AccessWidth32, &Value);
  xUSLPciWrite (NextBar, AccessWidth32, &Value);
  xUSLPciRead (EndpointBar, AccessWidth32, &Value);
  if ((Value & 0xFFFFFFF0) == 0) {
    /*
	 * BAR size is greater than 32-bits.. check high bar
	 */
    Value = 0xFFFFFFFF;
    xUSLPciWrite (NextBar, AccessWidth32, &Value);
    xUSLPciRead (NextBar, AccessWidth32, &Value);
    Value = (~Value) + 1;
    Value64 = ((uint64_t) Value) << 32;
  } else {
    xUSLPciRead (EndpointBar, AccessWidth32, &Value);
    xUSLPciWrite (EndpointBar, AccessWidth32, &OrgValue);
    Value64 = 0xFFFFFFFF00000000 + (Value & 0xFFFFFFF0);
    Value64 = (~Value64) + 1;
  }
  xUSLPciWrite (NextBar, AccessWidth32, &OrgValue2);
  xUSLPciWrite (EndpointBar, AccessWidth32, &OrgValue);
  return (Value64);
}

/**--------------------------------------------------------------------
 * AllocateForCxlIo
 *
 * @brief Allocate MMIO space for Cxl.io
 *
 * @details tbd
 *
 * @param[in]       EndpointBar       Pointer to engine config descriptor
 *
 * @returns Nothing
 * @retval Nothing
 */
void
AllocateForCxlIo (
  PCIe_ENGINE_CONFIG   *Engine,
  uint32_t             DeviceAddr
  )
{
  PCI_ADDR            EndPointAddress;
  uint32_t            Index;
  PCIE_BAR_STRUCT     Bar32;
  uint32_t            Value;
  uint64_t            Value64;
  uint32_t            Mem32Size = 0;
  uint32_t            Mem32Granularity = GRANULARITY_32BIT;
  uint64_t            MemP64Size = 0;
  uint64_t            MemP64Granularity = GRANULARITY_64BIT;

  CXL_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  /*
   * Need to support multi-function devices - i.e. allocate for functions other than 0
   */
  EndPointAddress.AddressValue = DeviceAddr;
  /*
   * Read RCiEP BARs and aggregate requests
   */
  for (Index = 0x10; Index <= 0x24; Index += 4) {
    EndPointAddress.Address.Register = Index;
    xUSLPciRead (EndPointAddress.AddressValue, AccessWidth32, &Bar32.Value);
    if (Bar32.Field.MemIo == 0) {
      if (Bar32.Field.MemSize == MEM_32_BAR) {
        Value = GetMem32BarSize (EndPointAddress.AddressValue);
        if (Value > Mem32Granularity) {
          Mem32Granularity = Value;
        }
        CXL_TRACEPOINT (SIL_TRACE_INFO, " 32 bit bar at 0x%x with size 0x%llX\n", Index, Value);
        Mem32Size += Value;
      } else if (Bar32.Field.MemSize == MEM_64_BAR) {
        Value64 = GetMem64BarSize (EndPointAddress.AddressValue);
        if (Value64 > MemP64Granularity) {
          MemP64Granularity = Value64;
        }
        CXL_TRACEPOINT (SIL_TRACE_INFO, " 64 bit bar at 0x%x with size 0x%llX\n", Index, Value64);
        MemP64Size += Value64;
        Index += 4;
      }
    }
  }

  /*
   * Round sizes up to allow for alignment and ACPI space
   */
  Mem32Size += (Mem32Granularity - 1);
  Mem32Size &= ~(Mem32Granularity - 1);
  if (Mem32Size == 0) {
    Mem32Size = GRANULARITY_32BIT;
  }
  CXL_TRACEPOINT (SIL_TRACE_INFO, " Mem32 Size = 0x%x\n", Mem32Size);
  Engine->Type.Cxl.Mmio32Size = Mem32Size;
  Engine->Type.Cxl.Mmio32Gran = Mem32Granularity;

  MemP64Size += (MemP64Granularity - 1);
  MemP64Size &= ~(MemP64Granularity - 1);
  CXL_TRACEPOINT (SIL_TRACE_INFO, " Mem64PSize = 0x%lX\n", MemP64Size);
  Engine->Type.Cxl.Mmio64Size = MemP64Size;

  CXL_TRACEPOINT (SIL_TRACE_EXIT, "\n");
}

/**--------------------------------------------------------------------
 * CxlPortInit
 *
 * @brief Determine CXL capabilities of all ports
 *
 * @details tbd
 *
 * @param[in]  GnbHandle           Pointer to the Silicon Descriptor for this node
 * @param[in]  PortDevMap          Pointer to PortDevMap
 *
 * @returns Nothing
 * @retval Nothing
 */
void
CxlPortInit (
    PCIe_PLATFORM_CONFIG  *Pcie
    )
{

  CXL_BUS_LIMITS                Resources;
  GNB_HANDLE                    *GnbHandle;
  CXL_INFO_LIST                 CxlInfo;
  NBIO_IP2IP_API                *NbioIp2Ip;
  CXL_COMMON_2_REV_XFER_BLOCK   *CxlXferTable;

  CXL_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  if (SilGetIp2IpApi (SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    CXL_TRACEPOINT (SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  /*
   * Get CXL Cmn2Rev transfer table
   */
  if (SilGetCommon2RevXferTable (SilId_CxlClass, (void **)(&CxlXferTable)) != SilPass) {
    return;
  }

  NbioIp2Ip->PcieConfigRunProcForAllEngines(
               DESCRIPTOR_ALLOCATED | DESCRIPTOR_PCIE_ENGINE,
               CxlXferTable->CxlFindPorts,
               NULL,
               Pcie
               );

  memset (&Resources, 0, sizeof(CXL_BUS_LIMITS));

  GnbHandle = NbioIp2Ip->NbioGetHandle (Pcie);
  while (GnbHandle != NULL) {
    Resources.BusLimits[GnbHandle->InstanceId] = (uint8_t) GnbHandle->BusNumberLimit;

    CXL_TRACEPOINT (SIL_TRACE_INFO, "  inst/bus/limit/rbidx/lrbidx: %d/%02x/%02x/%02x/%02x\n",
          GnbHandle->InstanceId,
          GnbHandle->Address.Address.Bus,
          GnbHandle->BusNumberLimit,
          GnbHandle->RBIndex,
          GnbHandle->LogicalRBIndex
          );

    GnbHandle = GnbGetNextHandle (GnbHandle);
  }

  /*
   * Are resources available? (Details TBD)
   */
  CXL_TRACEPOINT (SIL_TRACE_INFO, "Assign Resources\n");

  NbioIp2Ip->PcieConfigRunProcForAllEngines(
    DESCRIPTOR_CXL_ENGINE,
    CxlXferTable->CxlAssignResources,
    &Resources,
    Pcie
    );

  CxlXferTable->CxlDevListGenerate(Pcie, &CxlInfo);

  CXL_TRACEPOINT (SIL_TRACE_EXIT, "\n");
}

/**--------------------------------------------------------------------
 * InitializeCxlTp1
 *
 * @brief This function initializes the CXL IP during timepoint 1 (pre-Pcie phase) .
 *
 * @details  This is an IP private function, not visible to the Host.
 *           The CXL Silicon block is initialized by calling this function
 *           in the IP block list.
 * @return SIL_STATUS
 * @retval  SilPass - everything is OK
 * @retval  SilNotFound - Something went wrong
 */
SIL_STATUS InitializeCxlTp1 (void)
{
  CXLCLASS_DATA_BLK             *SilData;
  NORTH_BRIDGE_PCIE_SIB         *NbPcieData;
  SIL_STATUS                    Status;

  Status = SilPass;
  CXL_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  /*
   * Get IP block data
   */
  SilData = (CXLCLASS_DATA_BLK *)SilFindStructure (SilId_CxlClass,  0);
  if (SilData == NULL) {
    // Could not find the IP input block
    Status = SilNotFound;
    CXL_TRACEPOINT (SIL_TRACE_INFO, "CXL IP block not found \n");
    return Status;
  }

  CXL_TRACEPOINT (SIL_TRACE_INFO, "CXL IP block is located at %x\n", SilData);
  /*
   * Get PCIe topology from platform Host Firmware
   */
  NbPcieData = (NORTH_BRIDGE_PCIE_SIB*)SilFindStructure (SilId_NorthBridgePcie, 0);
  assert (NbPcieData);

  CxlPortInit(&NbPcieData->PciePlatformConfig);

  CXL_TRACEPOINT (SIL_TRACE_EXIT, "\n");

  return Status;
}

/**--------------------------------------------------------------------
 * CxlClassSetInputBlock
 *
 * @brief Establish CXL input defaults
 *
 * @details This function is called by the xSIM core to populate the IP's input
 * block with its default values. Each block established by the IP must
 * have a unique header.ID and its header.size set to the single block
 * only - so as to allow the FindStruct() to traverse the block chain.
 *
 * This is an IP private function, not visible to the Host.
 *
 * @return SIL_STATUS.
 * @retval  SilPass - everything is OK
 * @retval  SilAborted - Something went wrong
 */
SIL_STATUS CxlClassSetInputBlock (void)
{
  CXLCLASS_DATA_BLK *CxlConfigData;

  CxlConfigData = (CXLCLASS_DATA_BLK *)SilCreateInfoBlock (SilId_CxlClass,
                                 sizeof(CXLCLASS_DATA_BLK),
                                 CXLCLASS_INSTANCE,
                                 CXLCLASS_MAJOR_REV,
                                 CXLCLASS_MINOR_REV);
  CXL_TRACEPOINT(SIL_TRACE_INFO, "CxlSetInputBlk at: 0x%x \n", CxlConfigData);
  if (CxlConfigData == NULL) {
    return SilAborted;
  }
  // fill CXL IP data structure with defaults
  memcpy ((void *)CxlConfigData, &CxlClassDflts, sizeof(CXLCLASS_DATA_BLK));

  return SilPass;
}
