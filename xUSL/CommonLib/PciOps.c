/**
 * @file  PciOps.c
 * @brief OpenSIL PCI access functions
 *
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include "PciExpress.h"
#include "Pci.h"

/**
 * xUSLPciRead8
 * @brief Read 8-bit PCI register
 * @param Address 32-bit PCI address that encodes the PCI Bus, Device, Function and Register
 *
 */
uint8_t xUSLPciRead8 (uint32_t Address)
{
  return xUSLPciExpressRead8 ((void*)(size_t)Address);
}

/**
 * @brief Write 8-bit PCI register
 * @param Address 32-bit PCI address that encodes the PCI Bus, Device, Function and Register
 *
 */

void xUSLPciWrite8 (uint32_t Address, uint8_t Value)
{
  xUSLPciExpressWrite8 ((void*)(size_t)Address, Value);
}


/**
 * @brief Read 16-bit PCI register
 * @param Address 32-bit PCI address that encodes the PCI Bus, Device, Function and Register
 *
 */

uint16_t xUSLPciRead16 (uint32_t Address)
{
  return xUSLPciExpressRead16 ((void*)(size_t)Address);
}

/**
 * @brief Write 16-bit PCI register
 * @param Address 32-bit PCI address that encodes the PCI Bus, Device, Function and Register
 * @param Value   The value to be written in Address.
 *
 */
void xUSLPciWrite16 (uint32_t Address, uint16_t Value)
{
  xUSLPciExpressWrite16 ((void*)(size_t)Address, Value);
}

/**
 * @brief Read 32-bit PCI register
 * @param Address 32-bit PCI address that encodes the PCI Bus, Device, Function and Register
 *
 */

uint32_t xUSLPciRead32 (uint32_t Address)
{
  return xUSLPciExpressRead32 ((void*)(size_t)Address);
}


/**
 * @brief Write 32-bit PCI register
 * @param Address 32-bit PCI address that encodes the PCI Bus, Device, Function and Register
 * @param Value   The value to be written in Address.
 *
 */
void xUSLPciWrite32 (uint32_t Address, uint32_t Value)
{
  xUSLPciExpressWrite32 ((void*)(size_t)Address, Value);
}


/**
 * @brief Read 64-bit PCI register
 * @param Address 32-bit PCI address that encodes the PCI Bus, Device, Function and Register
 *
 */
uint64_t xUSLPciRead64 (uint32_t Address)
{
  return xUSLPciExpressRead64 ((void*)(size_t)Address);
}

/**
 * @brief Write 64-bit PCI register
 * @param Address 32-bit PCI address that encodes the PCI Bus, Device, Function and Register
 * @param Value   The value to be written in Address.
 *
 */
void xUSLPciWrite64 (uint32_t Address, uint64_t Value)
{
  xUSLPciExpressWrite64 ((void*)(size_t)Address, Value);
}


/**
 * @brief Modify 8-bit PCI register using AND mask and OR mask
 * @param Address 32-bit PCI address that encodes the PCI Bus, Device, Function and Register
 * @param AndMask 8-bit value to AND with the value at Address
 * @param OrMask  8-bit value to OR with the value at Address
 */
void xUSLPciReadModifyWrite8(uint32_t Address, uint8_t AndMask, uint8_t OrMask)
{
    uint8_t Value;
    Value = xUSLPciRead8(Address);
    Value &= AndMask;
    Value |= OrMask;
    xUSLPciWrite8(Address, Value);
}

/**
 * @brief Modify 16-bit PCI register using AND mask and OR mask
 * @param Address 32-bit PCI address that encodes the PCI Bus, Device, Function and Register
 * @param AndMask 16-bit value to AND with the value at Address
 * @param OrMask  16-bit value to OR with the value at Address
 *
 */
void xUSLPciReadModifyWrite16(uint32_t Address, uint16_t AndMask, uint16_t OrMask)
{
    uint16_t Value;
    Value = xUSLPciRead16(Address);
    Value &= AndMask;
    Value |= OrMask;
    xUSLPciWrite16(Address, Value);
}

/**
 * @brief Modify 32-bit PCI register using AND mask and OR mask
 * @param Address 32-bit PCI address that encodes the PCI Bus, Device, Function and Register
 * @param AndMask 32-bit value to AND with the value at Address
 * @param OrMask  32-bit value to OR with the value at Address
 *
 */
void xUSLPciReadModifyWrite32(uint32_t Address, uint32_t AndMask, uint32_t OrMask)
{
    uint32_t Value;
    Value = xUSLPciRead32(Address);
    Value &= AndMask;
    Value |= OrMask;
    xUSLPciWrite32(Address, Value);
}

/**
 * @brief Indirect Pci Read Access
 * @param PciBase       32-bit PCI base address of indirect register
 * @param IndexAddress  Offset of indirect register
 * @param Value         pointer to hold read value.
 *
 */
void xUSLIndirectPciRead32 (volatile uint32_t PciBase, uint32_t IndexAddress, uint32_t *Value)
{
  PciBase |= 0xB8;
  xUSLPciWrite32 (PciBase, IndexAddress);
  *Value = xUSLPciRead32 (PciBase + sizeof(uint32_t));
}

/**
 * @brief Indirect Pci Write Access
 * @param PciBase       32-bit PCI base address of indirect register
 * @param IndexAddress  Offset of indirect register
 * @param Value         The value to be written.
 *
 */
void xUSLIndirectPciWrite32 (volatile uint32_t PciBase, uint32_t IndexAddress, uint32_t Value)
{
  PciBase |= 0xB8;
  xUSLPciWrite32 (PciBase, IndexAddress);
  xUSLPciWrite32 (PciBase + sizeof(uint32_t), Value);
}
/*----------------------------------------------------------------------------------------*/
/**
 * Read PCI registers
 *
 *
 *
 * @param[in]   Address         PCI address (as presented in PCI_ADDR.AddressValue)
 * @param[in]   Width           Access width
 * @param[out]  Value           Pointer to value
 */

void
xUSLPciRead (uint32_t Address, ACCESS_WIDTH  Width, void *Value)
{
  switch (Width) {
  case AccessWidth8:
  case AccessS3SaveWidth8:
    *((uint8_t *) Value) = xUSLPciRead8 (Address);
    break;
  case AccessWidth16:
  case AccessS3SaveWidth16:
    *((uint16_t *) Value) = xUSLPciRead16 (Address);
    break;
  case AccessWidth32:
  case AccessS3SaveWidth32:
    *((uint32_t *) Value) = xUSLPciRead32 (Address);
    break;
  default:
    XUSL_TRACEPOINT (SIL_TRACE_ERROR, "Invalid Pci Read access width.\n");
  }
}
/*----------------------------------------------------------------------------------------*/
/**
 * Read/Modify/Write PCI registers
 *
 *
 *
 * @param[in] Address         PCI address
 * @param[in] Width           Access width
 * @param[in] Mask            AND Mask
 * @param[in] Value           OR Value
 */
void xUSLPciRMW(uint32_t Address, ACCESS_WIDTH Width, uint32_t Mask, uint32_t Value)
{
  uint32_t Data;
  xUSLPciRead (Address, Width, &Data);
  Data = (Data & Mask) | Value;
  xUSLPciWrite (Address, Width, &Data);
}
/*----------------------------------------------------------------------------------------*/
/**
 * Write PCI registers
 *
 *
 *
 * @param[in] Address         PCI address (as presented in PCI_ADDR.AddressValue)
 * @param[in] Width           Access width
 * @param[in] Value           Pointer to value
 * @param[in] StdHeader       Pointer to standard header
 */
void xUSLPciWrite (uint32_t Address, ACCESS_WIDTH Width, void *Value)
{
  switch (Width) {
  case AccessWidth8:
  case AccessS3SaveWidth8:
    xUSLPciWrite8 (Address, *((uint8_t *) Value));
    break;
  case AccessWidth16:
  case AccessS3SaveWidth16:
    xUSLPciWrite16 (Address, *((uint16_t *) Value));
    break;
  case AccessWidth32:
  case AccessS3SaveWidth32:
    xUSLPciWrite32 (Address, *((uint32_t *) Value));
    break;
  default:
    XUSL_TRACEPOINT (SIL_TRACE_ERROR, "Invalid Pci access width.\n");
  }
}
