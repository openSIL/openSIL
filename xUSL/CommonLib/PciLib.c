/* SPDX-License-Identifier: MIT */
/**
 * @file  PciOps.c
 * @brief OpenSIL PCI access functions
 *
 */

/**
 * Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */

#include <SilCommon.h>
#include "PciLib.h"

/**
 * xUSLPciIsDevicePresent
 * @brief Read PCI VendorID/DeviceID register and check if the value is valid
 * @param Address 32-bit PCI address that encodes the PCI Bus, Device,
 *                Function and Register
 *
 */
bool xUSLPciIsDevicePresent (uint32_t Address)
{
  return (xUSLPciRead32(Address) == 0xffffffff) ? false : true;
}

/**
 * xUSLPciIsMultiFunctionDevice
 * @brief Read PCI HeaderType register and check if the value is valid
 * @param Address 32-bit PCI address that encodes the PCI Bus, Device,
 *                Function and Register
 *
 */
bool xUSLPciIsMultiFunctionDevice (uint32_t Address)
{
  return ((xUSLPciRead8 (Address | PCI_HEADER_TYPE_REG) & MULTI_FUNC_DEVICE_MASK) != 0) ? true : false;
}

/**
 * xUSLPciLibbGetPcieDeviceType
 * @brief Locates PCI Express caapability pointer on the PCI device and
 *        returns PCI Express device type.
 * @param Address 32-bit PCI address that encodes the PCI Bus, Device,
 *                Function and Register
 *
 */
PCIE_DEVICE_TYPE xUSLPciLibbGetPcieDeviceType (uint32_t Address)
{
  uint8_t PcieCapPtr = xUSLPciLibFindPciCapability (Address, PCIE_CAP_ID);
  if (PcieCapPtr != 0) {
    return xUSLPciRead8 (Address| (PcieCapPtr + 0x2)) >> 4;
  }
  return PcieNotPcieDevice;
}

/**
 * xUSLPciIsSlotImplemented
 * @brief Locates PCI Express caapability pointer on the PCI device and checks
 *        if slot implemented bit is set.
 * @param Address 32-bit PCI address that encodes the PCI Bus, Device,
 *                Function and Register
 *
 */
bool xUSLPciIsSlotImplemented (uint32_t Address)
{
  uint8_t   PcieCapPtr = 0;

  PcieCapPtr = xUSLPciLibFindPciCapability (Address, PCIE_CAP_ID);
  if (PcieCapPtr == 0) {
    return false;
  }
  return !!((xUSLPciRead32 (Address | PcieCapPtr) >> 24) & 1);
}

/**
 * xUSLPciIsSlotImplemented
 * @brief Locates PCI caapability pointer on the PCI device given the capability ID.
 * @param Address      32-bit PCI address that encodes the PCI Bus, Device,
 *                     Function and Register
 * @param CapabilityId The PCI capability ID to look for
 *
 */
uint8_t xUSLPciLibFindPciCapability (uint32_t Address, uint8_t CapabilityId)
{
  uint8_t CapabilityPtr = PCI_CAP_POINTER_REG;
  uint8_t CurrentCapabilityId;

  if (!xUSLPciIsDevicePresent (Address)) {
    return  0;
  }
  while (CapabilityPtr != 0) {
    CapabilityPtr = xUSLPciRead8 (Address | CapabilityPtr);
    if (CapabilityPtr != 0) {
      CurrentCapabilityId = xUSLPciRead8 (Address | CapabilityPtr);
      if (CurrentCapabilityId == CapabilityId) {
        break;
      }
      CapabilityPtr++;
    }
  }
  return  CapabilityPtr;
}
