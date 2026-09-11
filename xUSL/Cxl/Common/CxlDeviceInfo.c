/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2026 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  CxlDeviceInfo.c
 * @brief CXL endpoint discovery and HDM decoder reads for DXE consumers
 *
 * @details Reads device state from configuration space and returns semantic
 *          CXL capability data. Host firmware owns policy applied to the
 *          result, including persistent configuration reconciliation, address
 *          range publication, and reset decisions.
 *
 *          The PEI side of CXL already relies on the capability helpers in
 *          CxlInit.c, so this reuses them rather than growing a second walk.
 */

#include <SilCommon.h>
#include <string.h>
#include <Pci.h>
#include <Cxl/Common/CxlInit.h>
#include <Cxl/Common/CxlDeviceInfo.h>

/// Offset of the PCI revision and class code register
#define PCICFG_SPACE_REV_ID_OFFSET  0x08

/// Offset of the port number field within the PCIe link capabilities register
#define PCIE_LINK_CAP_REGISTER  0x0C

/// DVSEC offsets of the HDM range size registers, high then low, per range
#define DVSEC_RANGE1_SIZE_HI_OFFSET  0x18
#define DVSEC_RANGE1_SIZE_LO_OFFSET  0x1C
#define DVSEC_RANGE2_SIZE_HI_OFFSET  0x28
#define DVSEC_RANGE2_SIZE_LO_OFFSET  0x2C

/// CXL DVSEC capability field: device supports CXL.cache
#define DVSEC_CXL_CAP_CACHE  0x0001

/// CXL DVSEC capability field: device supports CXL.mem
#define DVSEC_CXL_CAP_MEM  0x0004

/// CXL DVSEC capability field: HDM decoder range count
#define DVSEC_CXL_CAP_HDM_COUNT_MASK   0x0030
#define DVSEC_CXL_CAP_HDM_COUNT_SHIFT  4

/// CXL DVSEC capability field: device sets Memory_Active itself after reset
#define DVSEC_CXL_CAP_HW_INIT_MASK   0x0008
#define DVSEC_CXL_CAP_HW_INIT_SHIFT  3

/// The CXL DVSEC this code looks for is the one describing a CXL device
#define DVSEC_ID_FOR_CXL_DEVICE  0

/// Device numbers a PCI bus can carry
#define PCIE_MAX_DEVICES  32

/**
 * CxlDeviceFindDvsec
 *
 * @brief Locate the CXL device DVSEC on an endpoint.
 *
 * @details Both DVSEC vendor IDs are accepted. Early parts shipped the
 *          pre-consortium 0x8086 identifier and the CXL specification later
 *          assigned 0x1E98.
 *
 * @param[in] Address  Encoded PCI address
 *
 * @retval uint16_t    Config offset of the DVSEC, zero when there is none
 */
static
uint16_t
CxlDeviceFindDvsec (
  uint32_t  Address
  )
{
  return CxlGetDvsec (Address, DVSEC_VID, DVSEC_VID2, DVSEC_ID_FOR_CXL_DEVICE);
}

/**
 * CxlRangeSizeLowOffset
 *
 * @brief DVSEC offset of one HDM range's size-low register.
 *
 * @param[in] RangeIndex  Zero based HDM range
 *
 * @retval uint32_t       Offset from the DVSEC base
 */
static
uint32_t
CxlRangeSizeLowOffset (
  uint32_t  RangeIndex
  )
{
  return (RangeIndex == 0) ? DVSEC_RANGE1_SIZE_LO_OFFSET : DVSEC_RANGE2_SIZE_LO_OFFSET;
}

/**
 * CxlRangeSizeHighOffset
 *
 * @brief DVSEC offset of one HDM range's size-high register.
 *
 * @param[in] RangeIndex  Zero based HDM range
 *
 * @retval uint32_t       Offset from the DVSEC base
 */
static
uint32_t
CxlRangeSizeHighOffset (
  uint32_t  RangeIndex
  )
{
  return (RangeIndex == 0) ? DVSEC_RANGE1_SIZE_HI_OFFSET : DVSEC_RANGE2_SIZE_HI_OFFSET;
}

/**
 * CxlDecodeDvsecCapability
 *
 * See CxlDeviceInfo.h for the interface description.
 */
void
CxlDecodeDvsecCapability (
  uint16_t  Capability,
  uint8_t   *CxlType,
  uint8_t   *HdmCount,
  uint8_t   *MemHwInitMode
  )
{
  uint8_t  Ranges;

  if (CxlType != NULL) {
    switch (Capability & (DVSEC_CXL_CAP_CACHE | DVSEC_CXL_CAP_MEM)) {
    case DVSEC_CXL_CAP_CACHE:
      *CxlType = CXL_DEVICE_TYPE_CACHE;
      break;
    case (DVSEC_CXL_CAP_CACHE | DVSEC_CXL_CAP_MEM):
      *CxlType = CXL_DEVICE_TYPE_BOTH;
      break;
    case DVSEC_CXL_CAP_MEM:
      *CxlType = CXL_DEVICE_TYPE_MEM;
      break;
    default:
      *CxlType = CXL_DEVICE_TYPE_NONE;
      break;
    }
  }

  if (HdmCount != NULL) {
    Ranges = (uint8_t) ((Capability & DVSEC_CXL_CAP_HDM_COUNT_MASK) >>
               DVSEC_CXL_CAP_HDM_COUNT_SHIFT);
    //
    // The field is two bits wide but only two ranges exist, so a device
    // reporting three would send the caller past the registers backing them.
    //
    *HdmCount = (Ranges > CXL_MAX_HDM_RANGES) ? CXL_MAX_HDM_RANGES : Ranges;
  }

  if (MemHwInitMode != NULL) {
    *MemHwInitMode = (uint8_t) ((Capability & DVSEC_CXL_CAP_HW_INIT_MASK) >>
                       DVSEC_CXL_CAP_HW_INIT_SHIFT);
  }
}

/**
 * CxlGetRangeSizeLow
 *
 * See CxlDeviceInfo.h for the interface description.
 */
SIL_STATUS
CxlGetRangeSizeLow (
  uint32_t  Address,
  uint16_t  DvsecPtr,
  uint32_t  RangeIndex,
  uint32_t  *Value
  )
{
  if ((Value == NULL) || (RangeIndex >= CXL_MAX_HDM_RANGES) || (DvsecPtr == 0)) {
    return SilInvalidParameter;
  }

  xUSLPciRead (Address | (DvsecPtr + CxlRangeSizeLowOffset (RangeIndex)), AccessWidth32, Value);
  return SilPass;
}

/**
 * CxlGetDeviceInfo
 *
 * See CxlDeviceInfo.h for the interface description.
 */
SIL_STATUS
CxlGetDeviceInfo (
  uint32_t         Address,
  CXL_ENDPOINT_INFO  *Info
  )
{
  uint16_t  CxlCapability;
  uint32_t  ClassCode;
  uint32_t  Range;

  if (Info == NULL) {
    return SilInvalidParameter;
  }

  memset ((void *) Info, 0x00, sizeof (CXL_ENDPOINT_INFO));

  Info->DvsecPtr = CxlDeviceFindDvsec (Address);
  if (Info->DvsecPtr == 0) {
    return SilPass;
  }

  CxlCapability = 0;
  xUSLPciRead (Address | (Info->DvsecPtr + DVSEC_CXL_CAP_OFFSET), AccessWidth16, &CxlCapability);

  CxlDecodeDvsecCapability (CxlCapability,
    &Info->CxlType,
    &Info->HdmCount,
    &Info->MemHwInitMode
    );

  for (Range = 0; Range < Info->HdmCount; Range++) {
    xUSLPciRead (Address | (Info->DvsecPtr + CxlRangeSizeHighOffset (Range)),
      AccessWidth32,
      &Info->RangeSizeHi[Range]
      );
    xUSLPciRead (Address | (Info->DvsecPtr + CxlRangeSizeLowOffset (Range)),
      AccessWidth32,
      &Info->RangeSizeLo[Range]
      );
  }

  //
  // A device that reports memory but does not claim to be a memory controller
  // is a dual mode card, which the caller treats as persistent memory.
  //
  ClassCode = 0;
  xUSLPciRead (Address + PCICFG_SPACE_REV_ID_OFFSET, AccessWidth32, &ClassCode);
  Info->BaseClassCode = (uint8_t) ((ClassCode & 0xFF000000u) >> 24);

  CXL_TRACEPOINT (SIL_TRACE_INFO,
    "CXL endpoint %08x: type %d hdm %d hwinit %d class %02x\n",
    Address,
    Info->CxlType,
    Info->HdmCount,
    Info->MemHwInitMode,
    Info->BaseClassCode
    );

  return SilPass;
}

/**
 * CxlGetLinkPortNumber
 *
 * See CxlDeviceInfo.h for the interface description.
 */
uint8_t
CxlGetLinkPortNumber (
  uint32_t  Address
  )
{
  uint8_t   PcieCapPtr;
  uint32_t  Value;

  PcieCapPtr = SilGnbLibFindPciCapability (Address, PCIE_CAP_ID);
  if (PcieCapPtr == 0) {
    return 0;
  }

  Value = 0;
  xUSLPciRead (Address | (PcieCapPtr + PCIE_LINK_CAP_REGISTER), AccessWidth32, &Value);

  return (uint8_t) ((Value & 0xFF000000u) >> 24);
}

/**
 * CxlGetSwitchEndpoints
 *
 * See CxlDeviceInfo.h for the interface description.
 */
SIL_STATUS
CxlGetSwitchEndpoints (
  uint32_t  SwitchAddress,
  uint32_t  MaxEndpoints,
  uint32_t  *Count,
  uint32_t  *Buffer
  )
{
  uint32_t  ScanBus;
  uint32_t  DsPortBus;
  uint32_t  Segment;
  uint32_t  Device;
  uint32_t  Word;
  PCI_ADDR  DsPort;
  PCI_ADDR  Endpoint;

  if ((Count == NULL) || (Buffer == NULL)) {
    return SilInvalidParameter;
  }

  *Count = 0;
  Word = 0;

  ScanBus = 0;
  xUSLPciRead (SwitchAddress + PCICFG_SPACE_PRIMARY_BUS_OFFSET, AccessWidth32, &ScanBus);
  ScanBus = (ScanBus & 0x0000FF00u) >> 8;
  Segment = (SwitchAddress >> 28) & 0xF;

  for (Device = 0; Device < PCIE_MAX_DEVICES; Device++) {
    DsPort.AddressValue = MAKE_SBDFO (Segment, ScanBus, Device, 0, 0);
    if (!SilGnbLibPciIsDevicePresent (DsPort.AddressValue)) {
      continue;
    }
    if (SilGnbLibGetPcieDeviceType (DsPort, NULL) != PcieDeviceDownstreamPort) {
      continue;
    }

    DsPortBus = 0;
    xUSLPciRead (DsPort.AddressValue + PCICFG_SPACE_PRIMARY_BUS_OFFSET, AccessWidth32, &DsPortBus);
    DsPortBus = (DsPortBus & 0x0000FF00u) >> 8;

    Endpoint.AddressValue = MAKE_SBDFO (Segment, DsPortBus, 0, 0, 0);
    if (!SilGnbLibPciIsDevicePresent (Endpoint.AddressValue)) {
      continue;
    }

    switch (SilGnbLibGetPcieDeviceType (Endpoint, NULL)) {
    case PcieDeviceEndPoint:
    case PCieDeviceRCiEP:
      break;
    default:
      continue;
    }

    if (CxlDeviceFindDvsec (Endpoint.AddressValue) == 0) {
      continue;
    }

    if (*Count >= MaxEndpoints) {
      return SilOutOfBounds;
    }

    Buffer[Word++] = Endpoint.AddressValue;
    Buffer[Word++] = DsPort.AddressValue;
    Buffer[Word++] = CxlGetLinkPortNumber (Endpoint.AddressValue);
    (*Count)++;
  }

  CXL_TRACEPOINT (SIL_TRACE_INFO,
    "CXL switch %08x: %d endpoints behind it\n",
    SwitchAddress,
    *Count
    );

  return SilPass;
}
