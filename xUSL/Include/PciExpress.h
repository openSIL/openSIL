/**
 * @file  PciExpress.h
 * @brief OpenSIL PCIe access functions prototype
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#define PCI_EXPRESS_BASE_ADDRESS 0xE0000000

static inline uint8_t xUSLPciExpressRead8(void *Addr)
{
	return *((volatile uint8_t *)Addr + PCI_EXPRESS_BASE_ADDRESS);
}

static inline uint16_t xUSLPciExpressRead16(void *Addr)
{
	return *((volatile uint16_t *)((uint8_t*)Addr + PCI_EXPRESS_BASE_ADDRESS));
}

static inline uint32_t xUSLPciExpressRead32(void *Addr)
{
	return *((volatile uint32_t *)((uint8_t*)Addr + PCI_EXPRESS_BASE_ADDRESS));
}

static inline uint64_t xUSLPciExpressRead64(void *Addr)
{
	return *((volatile uint64_t *)((uint8_t*)Addr + PCI_EXPRESS_BASE_ADDRESS));
}

static inline void xUSLPciExpressWrite8(void *Addr, uint8_t Value)
{
	*((volatile uint8_t *)Addr + PCI_EXPRESS_BASE_ADDRESS) = Value;
}

static inline void xUSLPciExpressWrite16(void *Addr, uint16_t Value)
{
	*((volatile uint16_t *)((uint8_t*)Addr + PCI_EXPRESS_BASE_ADDRESS)) = Value;
}

static inline void xUSLPciExpressWrite32(void *Addr, uint32_t Value)
{
	*((volatile uint32_t *)((uint8_t*)Addr + PCI_EXPRESS_BASE_ADDRESS)) = Value;
}

static inline void xUSLPciExpressWrite64(void *Addr, uint64_t Value)
{
	*((volatile uint64_t *)((uint8_t*)Addr + PCI_EXPRESS_BASE_ADDRESS)) = Value;
}


