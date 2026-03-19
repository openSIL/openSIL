/* SPDX-License-Identifier: MIT */
/**
 * @file  Pci.h
 * @brief OpenSIL PCI access functions prototype
 *
 */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */

#pragma once

#include <stdbool.h>
#include "Pci.h"

#define PCI_CAP_POINTER_REG       (0x34)

bool xUSLPciIsDevicePresent (uint32_t Address);
bool xUSLPciIsMultiFunctionDevice (uint32_t Address);
PCIE_DEVICE_TYPE xUSLPciLibbGetPcieDeviceType (uint32_t Address);
bool xUSLPciIsSlotImplemented (uint32_t Address);
uint8_t xUSLPciLibFindPciCapability (uint32_t Address, uint8_t CapabilityId);
