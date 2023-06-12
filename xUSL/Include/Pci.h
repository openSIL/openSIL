/**
 * @file  Pci.h
 * @brief OpenSIL PCI access functions prototype
 *
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include "stdint.h"

#define MAKE_SBDFO(Seg, Bus, Dev, Fun, Off) ((((uint32_t) (Seg)) << 28) | (((uint32_t) (Bus)) << 20) | \
                   (((uint32_t)(Dev)) << 15) | (((uint32_t)(Fun)) << 12) | ((uint32_t)(Off)))
#define ILLEGAL_SBDFO 0xFFFFFFFFul

/// These width descriptors are used by xUSLPciRead to specify the data size
typedef enum ACCESS_WIDTH {
  AccessWidthNone = 0,                                      ///< dummy access width
  AccessWidth8 = 1,                                         ///< Access width is 8 bits.
  AccessWidth16,                                            ///< Access width is 16 bits.
  AccessWidth32,                                            ///< Access width is 32 bits.
  AccessWidth64,                                            ///< Access width is 64 bits.

  AccessS3SaveWidth8 = 0x81,                                ///< Save 8 bits data.
  AccessS3SaveWidth16,                                      ///< Save 16 bits data.
  AccessS3SaveWidth32,                                      ///< Save 32 bits data.
  AccessS3SaveWidth64,                                      ///< Save 64 bits data.
} ACCESS_WIDTH;

/// Extended PCI address format
typedef struct {
  uint32_t      Register:12;             ///< Register offset
  uint32_t      Function:3;              ///< Function number
  uint32_t      Device:5;                ///< Device number
  uint32_t      Bus:8;                   ///< Bus number
  uint32_t      Segment:4;               ///< Segment
} EXT_PCI_ADDR;

/// Union type for PCI address
typedef union _PCI_ADDR {
  uint32_t          AddressValue;        ///< Formal address
  EXT_PCI_ADDR      Address;             ///< Extended address
} PCI_ADDR;

#define PCI_LIB_ADDRESS(Bus,Device,Function,Register)   \
  (((Register) & 0xfff) | (((Function) & 0x07) << 12) | (((Device) & 0x1f) << 15) | (((Bus) & 0xff) << 20))

uint8_t xUSLPciRead8 (uint32_t Address);
void xUSLPciWrite8 (uint32_t Address, uint8_t Value);
uint16_t xUSLPciRead16 (uint32_t Address);
void xUSLPciWrite16 (uint32_t Address, uint16_t Value);
uint32_t xUSLPciRead32 (uint32_t Address);
void xUSLPciWrite32 (uint32_t Address, uint32_t Value);
uint64_t xUSLPciRead64 (uint32_t Address);
void xUSLPciWrite64 (uint32_t Address, uint64_t Value);
void xUSLPciReadModifyWrite8(uint32_t Address, uint8_t AndMask, uint8_t OrMask);
void xUSLPciReadModifyWrite16(uint32_t Address, uint16_t AndMask, uint16_t OrMask);
void xUSLPciReadModifyWrite32(uint32_t Address, uint32_t AndMask, uint32_t OrMask);
void xUSLIndirectPciRead32 (volatile uint32_t PciBase, uint32_t IndexAddress, uint32_t *Value);
void xUSLIndirectPciWrite32 (volatile uint32_t PciBase, uint32_t IndexAddress, uint32_t Value);
void xUSLPciRead(uint32_t Address, ACCESS_WIDTH Width, void *Value);
void xUSLPciRMW(uint32_t Address, ACCESS_WIDTH Width, uint32_t Mask, uint32_t Value);
void xUSLPciWrite (uint32_t Address, ACCESS_WIDTH Width, void *Value);

/**********************************************************************************************************************
 * Declare macros here
 *
 */

#define PCI_LIB_ADDRESS(Bus,Device,Function,Register)   \
  (((Register) & 0xfff) | (((Function) & 0x07) << 12) | (((Device) & 0x1f) << 15) | (((Bus) & 0xff) << 20))

#define PCI_HEADER_TYPE_REG       (0x0E)
#define MULTI_FUNC_DEVICE_MASK    (BIT_32(7))
#define PCI_REVISION_ID_REG       (0x08)
