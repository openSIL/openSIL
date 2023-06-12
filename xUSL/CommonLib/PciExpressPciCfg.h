/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  PciExpressPciCfg.h
 * @brief PCIe functions prototype
 *
 */

#pragma once

#include "VirtualMemory.h"

#define PCI_READ   0
#define PCI_WRITE  1

/**
 * xUslPciReadWrite
 *
 * @details  This function is a helper function to access 64bit PCIe address in 32-bit mode.
 *           Any PCI access should be services through the functions exposed in Pci.h (e.g. xUSLPciRead8)
 *
 * @param Address  64-bit PCI address that encodes the PCI Bus, Device, Function and Register
 * @param Width    Access width
 * @param Value    The value to be written
 * @param RW       Indicate access type of the function. (PCI Read: 0 / PCI Write: 1)
 * @param Gdt      The pointer to a GDTR descriptor.
 *
 **/
NASM_ABI uint32_t xUslPciReadWrite (uint64_t Address, uint8_t Width, uint32_t Value, uint32_t RW, const DescriptorReg *Gdt);
