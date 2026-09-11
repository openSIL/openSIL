/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2026 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  xPrfCxl.c
 * @brief Platform Reference Firmware - CXL endpoint discovery services.
 *
 * @details Answered on demand rather than at a time point, because host
 *          firmware asks for them while it walks PCI in DXE, long after the
 *          openSIL time points for this instance have run.
 */

#include <SilCommon.h>
#include <xSIM.h>
#include <xPRF-api.h>
#include <Cxl/Common/CxlInit.h>
#include <Cxl/Common/CxlDeviceInfo.h>
#include <xPrfCxl.h>

/**
 * xPrfCxlGetDeviceInfo
 *
 * @brief   Read a CXL endpoint's DVSEC and HDM decoder state.
 *
 * @param   Address      Encoded PCI address of the endpoint
 * @param   BufferWords  Capacity of Buffer, in words
 * @param   Buffer       Receives XPRF_CXL_DEVICE_INFO_WORDS words: DVSEC
 *                       pointer, CXL type, HDM count, hardware init mode, base
 *                       class, then size high and size low for each HDM range
 *
 * @return  SIL_STATUS
 */
SIL_STATUS
xPrfCxlGetDeviceInfo (
  uint32_t  Address,
  uint32_t  BufferWords,
  uint32_t  *Buffer
  )
{
  CXL_ENDPOINT_INFO  Info;
  SIL_STATUS       Status;
  uint32_t         Word;
  uint32_t         Range;

  if ((Buffer == NULL) || (BufferWords < XPRF_CXL_DEVICE_INFO_WORDS)) {
    return SilInvalidParameter;
  }

  Status = CxlGetDeviceInfo (Address, &Info);
  if (Status != SilPass) {
    return Status;
  }

  Word = 0;
  Buffer[Word++] = Info.DvsecPtr;
  Buffer[Word++] = Info.CxlType;
  Buffer[Word++] = Info.HdmCount;
  Buffer[Word++] = Info.MemHwInitMode;
  Buffer[Word++] = Info.BaseClassCode;
  for (Range = 0; Range < CXL_MAX_HDM_RANGES; Range++) {
    Buffer[Word++] = Info.RangeSizeHi[Range];
    Buffer[Word++] = Info.RangeSizeLo[Range];
  }

  return SilPass;
}

/**
 * xPrfCxlGetRangeSizeLow
 *
 * @brief   Re-read one HDM range's size-low register.
 *
 * @details Separate from the full read so that a caller waiting for a device to
 *          set Memory_Active can poll without re-walking the capability chain.
 *
 * @param   Address     Encoded PCI address of the endpoint
 * @param   DvsecPtr    Config offset of the CXL DVSEC
 * @param   RangeIndex  Zero based HDM range
 * @param   Value       Populated with the register contents
 *
 * @return  SIL_STATUS
 */
SIL_STATUS
xPrfCxlGetRangeSizeLow (
  uint32_t  Address,
  uint32_t  DvsecPtr,
  uint32_t  RangeIndex,
  uint32_t  *Value
  )
{
  return CxlGetRangeSizeLow (Address, (uint16_t) DvsecPtr, RangeIndex, Value);
}

/**
 * xPrfCxlGetLinkPortNumber
 *
 * @brief   Read the port number a device reports in its PCIe link capabilities.
 *
 * @param   Address    Encoded PCI address
 *
 * @return  uint32_t   Port number, zero when there is no PCIe capability
 */
uint32_t
xPrfCxlGetLinkPortNumber (
  uint32_t  Address
  )
{
  return (uint32_t) CxlGetLinkPortNumber (Address);
}

/**
 * xPrfCxlFindPciCapability
 *
 * @brief   Walk a device's PCI capability list for one capability.
 *
 * @param   Address       Encoded PCI address
 * @param   CapabilityId  Capability to find
 *
 * @return  uint32_t      Config offset of the capability, zero when absent
 */
uint32_t
xPrfCxlFindPciCapability (
  uint32_t  Address,
  uint32_t  CapabilityId
  )
{
  return (uint32_t) SilGnbLibFindPciCapability (Address, (uint8_t) CapabilityId);
}

/**
 * xPrfCxlFindPcieExtendedCapability
 *
 * @brief   Walk a device's PCIe extended capability list for one capability.
 *
 * @details A StartCapabilityPtr of zero starts the walk from the beginning;
 *          any other value resumes from that entry, which is how a caller
 *          iterates several capabilities with the same identifier.
 *
 * @param   Address               Encoded PCI address
 * @param   StartCapabilityPtr    Entry to resume from, or zero to start
 * @param   ExtendedCapabilityId  Capability to find
 *
 * @return  uint32_t              Config offset of the capability, zero when absent
 */
uint32_t
xPrfCxlFindPcieExtendedCapability (
  uint32_t  Address,
  uint32_t  StartCapabilityPtr,
  uint32_t  ExtendedCapabilityId
  )
{
  if (StartCapabilityPtr == 0) {
    return (uint32_t) SilGnbLibFindPcieExtendedCapability (Address, (uint16_t) ExtendedCapabilityId);
  }

  return (uint32_t) SilGnbLibFindNextPcieExtendedCapability (
                      Address,
                      (uint16_t) StartCapabilityPtr,
                      (uint16_t) ExtendedCapabilityId
                      );
}

/**
 * xPrfCxlGetPcieDeviceType
 *
 * @brief   Read the device or port type a device reports in its PCIe capability.
 *
 * @param   Address    Encoded PCI address
 *
 * @return  uint32_t   PCIE_DEVICE_TYPE value
 */
uint32_t
xPrfCxlGetPcieDeviceType (
  uint32_t  Address
  )
{
  PCI_ADDR  Device;

  Device.AddressValue = Address;
  return (uint32_t) SilGnbLibGetPcieDeviceType (Device, NULL);
}

/**
 * xPrfCxlGetSwitchEndpoints
 *
 * @brief   List the CXL endpoints behind a switch.
 *
 * @param   SwitchAddress  Encoded PCI address of the switch upstream port
 * @param   MaxEndpoints   Capacity of Buffer, in endpoints
 * @param   Count          Populated with the endpoints written
 * @param   Buffer         Receives endpoint address, downstream port address
 *                         and port number per endpoint
 *
 * @return  SIL_STATUS
 */
SIL_STATUS
xPrfCxlGetSwitchEndpoints (
  uint32_t  SwitchAddress,
  uint32_t  MaxEndpoints,
  uint32_t  *Count,
  uint32_t  *Buffer
  )
{
  return CxlGetSwitchEndpoints (SwitchAddress, MaxEndpoints, Count, Buffer);
}
