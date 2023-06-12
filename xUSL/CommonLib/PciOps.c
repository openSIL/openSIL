/* SPDX-License-Identifier: MIT */
/**
 * Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 * @file  PciOps.c
 * @brief OpenSIL PCI access functions
 *
 */

#include <SilCommon.h>
#include <DF/Df.h>
#include "CommonLib/CpuLib.h"
#include "PciExpress.h"
#include "PciExpressPciCfg.h"
#include "Pci.h"
#include "VirtualMemory.h"
#include "SmnAccess.h"
#include <MsrReg.h>

//
// Global Descriptor Table (GDT)
//
const SegmentDescriptor gGdtEntries[] = {
/* selector { Global Segment Descriptor                              } */
/* 0x00 */  {{0,      0,  0,  {0,    0,  0,  0},  {0,    0,  0, 0,  0},  0}}, //null descriptor
/* 0x08 */  {{0xffff, 0,  0,  {0x2,  1,  0,  1},  {0xf,  0,  0, 1,  1},  0}}, //linear data segment descriptor
/* 0x10 */  {{0xffff, 0,  0,  {0xf,  1,  0,  1},  {0xf,  0,  0, 1,  1},  0}}, //linear code segment descriptor
/* 0x18 */  {{0xffff, 0,  0,  {0x3,  1,  0,  1},  {0xf,  0,  0, 1,  1},  0}}, //system data segment descriptor
/* 0x20 */  {{0xffff, 0,  0,  {0xa,  1,  0,  1},  {0xf,  0,  0, 1,  1},  0}}, //system code segment descriptor
/* 0x28 */  {{0,      0,  0,  {0,    0,  0,  0},  {0,    0,  0, 0,  0},  0}}, //spare segment descriptor
/* 0x30 */  {{0xffff, 0,  0,  {0x2,  1,  0,  1},  {0xf,  0,  0, 1,  1},  0}}, //system data segment descriptor
/* 0x38 */  {{0xffff, 0,  0,  {0xa,  1,  0,  1},  {0xf,  0,  1, 0,  1},  0}}, //system code segment descriptor
/* 0x40 */  {{0,      0,  0,  {0,    0,  0,  0},  {0,    0,  0, 0,  0},  0}}, //spare segment descriptor
};

//
// Gdt register
//
const DescriptorReg gGdt = {
  sizeof (gGdtEntries) - 1,
  (size_t) gGdtEntries
};

static
uint64_t
PciReadWrite (
  uint64_t                    Address,
  uint8_t                     Width,
  uint64_t                    Value,
  uint32_t                    RW
  )
{
  uint64_t          ReturnValue;
  uintptr_t         OrgCr3;
  uint64_t          MsrData;
  uint64_t          PciAddr;
  DFCLASS_INPUT_BLK *DfIpBlock;

  DfIpBlock = (DFCLASS_INPUT_BLK *) xUslFindStructure(SilId_DfClass, DFCLASS_INSTANCE);
  if (DfIpBlock == NULL) {
    return 0;
  }

  if (Width != AccessWidth8 && Width != AccessWidth16 && Width != AccessWidth32 && Width != AccessWidth64) {
    assert(false);
    return 0;
  }

  MsrData = xUslRdMsr(MSR_EXTENDED_FEATURE_EN);
  PciAddr = Address + DfIpBlock->AmdPciExpressBaseAddress;

  // Check if "long mode was disabled in MSR_EXTENDED_FEATURE_EN" and "PCI Address over than 4GB (1 << 32)"
  if (((MsrData & BIT_64(10)) == 0) && (PciAddr >= BIT_64(32))) {
    if (DfIpBlock->AmdLongModePageTables == 0) {
      assert(false);
    }

    // Go to 64-bit mode and process the PCI read/write
    xUslWriteCr0(xUslReadCr0 () & (~BIT_32(31)));
    OrgCr3 = xUslReadCr3 ();
    xUslWriteCr3((uintptr_t) DfIpBlock->AmdLongModePageTables);

    ReturnValue = xUslPciReadWrite(PciAddr, Width, (uint32_t)Value, RW, &gGdt);

    xUslWriteCr3(OrgCr3);
  } else {
    if (RW == PCI_READ) {
      // Read
      switch (Width) {
      case AccessWidth8:
        ReturnValue = xUSLPciExpressRead8((void *)(size_t)PciAddr);
        break;
      case AccessWidth16:
        ReturnValue = xUSLPciExpressRead16((void *)(size_t)PciAddr);
        break;
      case AccessWidth32:
        ReturnValue = xUSLPciExpressRead32((void *)(size_t)PciAddr);
        break;
      case AccessWidth64:
        ReturnValue = xUSLPciExpressRead64((void *)(size_t)PciAddr);
        break;
      }
    } else {
      // Write
      switch (Width) {
      case AccessWidth8:
        xUSLPciExpressWrite8((void *)(size_t)PciAddr, (uint8_t)Value);
        break;
      case AccessWidth16:
        xUSLPciExpressWrite16((void *)(size_t)PciAddr, (uint16_t)Value);
        break;
      case AccessWidth32:
        xUSLPciExpressWrite32((void *)(size_t)PciAddr, (uint32_t)Value);
        break;
      case AccessWidth64:
        xUSLPciExpressWrite64((void *)(size_t)PciAddr, (size_t)Value);
        break;
      }
      ReturnValue = 0;
    }
  }

  return ReturnValue;
}

/**
 * PciRead
 *
 * @brief Read PCI register
 *
 * @param Address  64-bit PCI address that encodes the PCI Bus, Device, Function and Register
 * @param Width    Access width
 *
 * @return uint64_t  PCIe register value which point to the PCI address.
 *
 */
static
uint64_t
PciRead (
  uint64_t Address,
  uint8_t  Width
  )
{
  uint64_t Value;

  Value = PciReadWrite(Address, Width, 0, PCI_READ);

  return Value;
}

/**
 * PciWrite
 *
 * @brief Write PCI register
 *
 * @param Address  64-bit PCI address that encodes the PCI Bus, Device, Function and Register
 * @param Value    The value to be written
 * @param Width    Access width
 *
 * @return Nothing.
 *
 */
static
void
PciWrite (
  uint64_t Address,
  uint64_t Value,
  uint8_t  Width
  )
{
  PciReadWrite(Address, Width, Value, PCI_WRITE);
}

/**
 * xUSLPciRead8
 * @brief Read 8-bit PCI register
 * @param Address 32-bit PCI address that encodes the PCI Bus, Device, Function and Register
 *
 */
uint8_t xUSLPciRead8 (uint32_t Address)
{
  return (uint8_t) PciRead(Address, AccessWidth8);
}

/**
 * @brief Write 8-bit PCI register
 * @param Address 32-bit PCI address that encodes the PCI Bus, Device, Function and Register
 *
 */
void xUSLPciWrite8 (uint32_t Address, uint8_t Value)
{
  PciWrite(Address, Value, AccessWidth8);
}


/**
 * @brief Read 16-bit PCI register
 * @param Address 32-bit PCI address that encodes the PCI Bus, Device, Function and Register
 *
 */
uint16_t xUSLPciRead16 (uint32_t Address)
{
  return (uint16_t) PciRead(Address, AccessWidth16);
}

/**
 * @brief Write 16-bit PCI register
 * @param Address 32-bit PCI address that encodes the PCI Bus, Device, Function and Register
 * @param Value   The value to be written in Address.
 *
 */
void xUSLPciWrite16 (uint32_t Address, uint16_t Value)
{
  PciWrite(Address, Value, AccessWidth16);
}

/**
 * @brief Read 32-bit PCI register
 * @param Address 32-bit PCI address that encodes the PCI Bus, Device, Function and Register
 *
 */
uint32_t xUSLPciRead32 (uint32_t Address)
{
  return (uint32_t) PciRead(Address, AccessWidth32);
}

/**
 * @brief Write 32-bit PCI register
 * @param Address 32-bit PCI address that encodes the PCI Bus, Device, Function and Register
 * @param Value   The value to be written in Address.
 *
 */
void xUSLPciWrite32 (uint32_t Address, uint32_t Value)
{
  PciWrite(Address, Value, AccessWidth32);
}


/**
 * @brief Read 64-bit PCI register
 * @param Address 32-bit PCI address that encodes the PCI Bus, Device, Function and Register
 *
 */
uint64_t xUSLPciRead64 (uint32_t Address)
{
  return (uint64_t) PciRead(Address, AccessWidth64);
}

/**
 * @brief Write 64-bit PCI register
 * @param Address 32-bit PCI address that encodes the PCI Bus, Device, Function and Register
 * @param Value   The value to be written in Address.
 *
 */
void xUSLPciWrite64 (uint32_t Address, uint64_t Value)
{
  PciWrite(Address, Value, AccessWidth64);
}


/**
 * @brief Modify 8-bit PCI register using AND mask and OR mask
 * @param Address 32-bit PCI address that encodes the PCI Bus, Device, Function and Register
 * @param AndMask 8-bit value to AND with the value at Address
 * @param OrMask  8-bit value to OR with the value at Address
 */
void xUSLPciReadModifyWrite8 (uint32_t Address, uint8_t AndMask, uint8_t OrMask)
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
void xUSLPciReadModifyWrite16 (uint32_t Address, uint16_t AndMask, uint16_t OrMask)
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
void xUSLPciReadModifyWrite32 (uint32_t Address, uint32_t AndMask, uint32_t OrMask)
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
  PciBase |= SIL_RESERVED_0009;
  xUSLPciWrite32(PciBase, IndexAddress);
  *Value = xUSLPciRead32(PciBase + sizeof (uint32_t));
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
  PciBase |= SIL_RESERVED_0009;
  xUSLPciWrite32(PciBase, IndexAddress);
  xUSLPciWrite32(PciBase + sizeof (uint32_t), Value);
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
    *((uint8_t *) Value) = xUSLPciRead8(Address);
    break;
  case AccessWidth16:
  case AccessS3SaveWidth16:
    *((uint16_t *) Value) = xUSLPciRead16(Address);
    break;
  case AccessWidth32:
  case AccessS3SaveWidth32:
    *((uint32_t *) Value) = xUSLPciRead32(Address);
    break;
  default:
    XUSL_TRACEPOINT(SIL_TRACE_ERROR, "Invalid Pci Read access width.\n");
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
void xUSLPciRMW (uint32_t Address, ACCESS_WIDTH Width, uint32_t Mask, uint32_t Value)
{
  uint32_t Data;
  xUSLPciRead(Address, Width, &Data);
  Data = (Data & Mask) | Value;
  xUSLPciWrite(Address, Width, &Data);
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
    xUSLPciWrite8(Address, *((uint8_t *) Value));
    break;
  case AccessWidth16:
  case AccessS3SaveWidth16:
    xUSLPciWrite16(Address, *((uint16_t *) Value));
    break;
  case AccessWidth32:
  case AccessS3SaveWidth32:
    xUSLPciWrite32(Address, *((uint32_t *) Value));
    break;
  default:
    XUSL_TRACEPOINT(SIL_TRACE_ERROR, "Invalid Pci access width.\n");
  }
}
