/**
 * @file  SmnAccess.c
 * @brief OpenSIL SMN access functions
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include "SmnAccess.h"
#include "Pci.h"

/**
 * xUSLSmnRead - Read SMN register
 *
 * @param[in] SegmentNumber     - IOHC (Node) Segment number
 * @param[in] IohcBus           - IOHC (Node) bus number
 * @param[in] SmnAddress        - Register SMN address
 * @retval    Value             - Register value
 *
 */
uint32_t
xUSLSmnRead (
  uint32_t SegmentNumber,
  uint32_t IohcBus,
  uint32_t SmnAddress
  )
{
  uint32_t RegIndex;
  PCI_ADDR PciAddress;

  RegIndex = SmnAddress;

  PciAddress.AddressValue = 0;
  PciAddress.Address.Bus = IohcBus;
  PciAddress.Address.Segment= SegmentNumber;
  PciAddress.Address.Register = SIL_RESERVED2_896;

  xUSLPciWrite32 (PciAddress.AddressValue, RegIndex);
  PciAddress.Address.Register = SIL_RESERVED2_897;
  return xUSLPciRead32 (PciAddress.AddressValue);
}

/**
 * xUSLSmnWrite - write SMN register in Byte
 *
 * @param[in] SegmentNumber     - IOHC (Node) Segment number
 * @param[in] IohcBus           - IOHC (Node) bus number
 * @param[in] SmnAddress        - Register SMN address
 * @param[in] Value             - Register value
 *
 */
void
xUSLSmnWrite (
  uint32_t SegmentNumber,
  uint32_t IohcBus,
  uint32_t SmnAddress,
  uint32_t Value
  )
{
  uint32_t RegIndex;
  PCI_ADDR PciAddress;

  RegIndex = SmnAddress;

  PciAddress.AddressValue = 0;
  PciAddress.Address.Bus = IohcBus;
  PciAddress.Address.Segment= SegmentNumber;
  PciAddress.Address.Register = SIL_RESERVED2_896;

  xUSLPciWrite32 (PciAddress.AddressValue, RegIndex);
  PciAddress.Address.Register = SIL_RESERVED2_897;
  xUSLPciWrite32 (PciAddress.AddressValue, Value);
}

/**
 * xUSLSmnReadModifyWrite - Read,write SMN register in Byte
 *
 * @param[in] SegmentNumber     - IOHC (Node) Segment number
 * @param[in] IohcBus           - IOHC (Node) bus number
 * @param[in] SmnAddress        - Register SMN address
 * @param[in] AndMask           - AND mask value
 * @param[in] OrMask            - OR mask value
 *
 */
void
xUSLSmnReadModifyWrite (
  uint32_t SegmentNumber,
  uint32_t IohcBus,
  uint32_t SmnAddress,
  uint32_t AndMask,
  uint32_t OrMask
  )
{
  uint32_t RegValue;

  RegValue = xUSLSmnRead (SegmentNumber, IohcBus, SmnAddress);
  RegValue &= AndMask;
  RegValue |= OrMask;
  xUSLSmnWrite (SegmentNumber, IohcBus, SmnAddress, RegValue);
}

/**
 * xUSLSmnRead8 - Read SMN register in Byte
 *
 * @param[in] SegmentNumber     - IOHC (Node) Segment number
 * @param[in] IohcBus           - IOHC (Node) bus number
 * @param[in] SmnAddress        - Register SMN address
 * @retval    Value             - register value
 *
 */
uint8_t
xUSLSmnRead8 (
  uint32_t SegmentNumber,
  uint32_t IohcBus,
  uint32_t SmnAddress
  )
{
  uint32_t    RegIndex;
  PCI_ADDR    PciAddress;

  RegIndex = SmnAddress & 0xFFFFFFFC;

  PciAddress.AddressValue = 0;
  PciAddress.Address.Bus = IohcBus;
  PciAddress.Address.Segment= SegmentNumber;
  PciAddress.Address.Register = SIL_RESERVED2_896;

  xUSLPciWrite32 (PciAddress.AddressValue, RegIndex);
  PciAddress.Address.Register = SIL_RESERVED2_897;
  return xUSLPciRead8 (PciAddress.AddressValue + (SmnAddress & 0x3));
}

/**
 * xUSLSmnWrite8 - Write SMN register in Byte
 *
 * @param[in] SegmentNumber     - IOHC (Node) Segment number
 * @param[in] IohcBus           - IOHC (Node) bus number
 * @param[in] SmnAddress        - Register SMN address
 * @param[in] Value8            - Register value to be written
 *
 */
void
xUSLSmnWrite8 (
  uint32_t SegmentNumber,
  uint32_t IohcBus,
  uint32_t SmnAddress,
  uint8_t  Value8
  )
{
  uint32_t    RegIndex;
  PCI_ADDR    PciAddress;

  RegIndex = SmnAddress & 0xFFFFFFFC;

  PciAddress.AddressValue = 0;
  PciAddress.Address.Bus = IohcBus;
  PciAddress.Address.Segment= SegmentNumber;
  PciAddress.Address.Register = SIL_RESERVED2_896;

  xUSLPciWrite32 (PciAddress.AddressValue, RegIndex);
  PciAddress.Address.Register = SIL_RESERVED2_897;
  xUSLPciWrite8 (PciAddress.AddressValue + (SmnAddress & 0x3), Value8);
}

/**
 * xUSLSmnReadModifyWrite8 - Read,Write SMN register in Byte
 *
 * @param[in] SegmentNumber     - IOHC (Node) Segment number
 * @param[in] IohcBus           - IOHC (Node) bus number
 * @param[in] SmnAddress        - Register SMN address
 * @param[in] AndMask           - AND mask value
 * @param[in] OrMask            - OR mask value
 *
 */
void
xUSLSmnReadModifyWrite8 (
  uint32_t SegmentNumber,
  uint32_t IohcBus,
  uint32_t SmnAddress,
  uint8_t AndMask,
  uint8_t OrMask
  )
{
  uint8_t  RegValue;

  RegValue = xUSLSmnRead8 (SegmentNumber, IohcBus, SmnAddress);
  RegValue &= AndMask;
  RegValue |= OrMask;
  xUSLSmnWrite8 (SegmentNumber, IohcBus, SmnAddress, RegValue);
}
