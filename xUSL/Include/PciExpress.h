/* SPDX-License-Identifier: MIT */
/**
 * Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 * @file  PciExpress.h
 * @brief OpenSIL PCIe access functions prototype
 *
 */

#pragma once

static inline uint8_t xUSLPciExpressRead8 (void *Addr)
{
  return *((volatile uint8_t *)Addr);
}

static inline uint16_t xUSLPciExpressRead16 (void *Addr)
{
  return *((volatile uint16_t *)((uint8_t *)Addr));
}

static inline uint32_t xUSLPciExpressRead32 (void *Addr)
{
  return *((volatile uint32_t *)((uint8_t *)Addr));
}

static inline uint64_t xUSLPciExpressRead64 (void *Addr)
{
  return *((volatile uint64_t *)((uint8_t *)Addr));
}

static inline void xUSLPciExpressWrite8 (void *Addr, uint8_t Value)
{
  *((volatile uint8_t *)Addr) = Value;
}

static inline void xUSLPciExpressWrite16 (void *Addr, uint16_t Value)
{
  *((volatile uint16_t *)((uint8_t *)Addr)) = Value;
}

static inline void xUSLPciExpressWrite32 (void *Addr, uint32_t Value)
{
  *((volatile uint32_t *)((uint8_t *)Addr)) = Value;
}

static inline void xUSLPciExpressWrite64 (void *Addr, uint64_t Value)
{
  *((volatile uint64_t *)((uint8_t *)Addr)) = Value;
}
