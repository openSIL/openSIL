/**
 * @file  Mmio.h
 * @brief OpenSIL MMIO access functions prototype
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once


static inline uint8_t xUSLMemRead8(const volatile void *Addr)
{
	return *((volatile uint8_t *)(Addr));
}

static inline uint16_t xUSLMemRead16(const volatile void *Addr)
{
	return *((volatile uint16_t *)(Addr));
}

static inline uint32_t xUSLMemRead32(const volatile void *Addr)
{
	return *((volatile uint32_t *)(Addr));
}

static inline uint64_t xUSLMemRead64(const volatile void *Addr)
{
	return *((volatile uint64_t *)(Addr));
}

static inline void xUSLMemWrite8(volatile void *Addr, uint8_t Value)
{
	*((volatile uint8_t *)(Addr)) = Value;
}

static inline void xUSLMemWrite16(volatile void *Addr, uint16_t Value)
{
	*((volatile uint16_t *)(Addr)) = Value;
}

static inline void xUSLMemWrite32(volatile void *Addr, uint32_t Value)
{
	*((volatile uint32_t *)(Addr)) = Value;
}

static inline void xUSLMemWrite64(volatile void *Addr, uint64_t Value)
{
	*((volatile uint64_t *)(Addr)) = Value;
}

/* Function declarations */

void xUSLMemReadModifyWrite8(void *Addr, uint8_t AndMask, uint8_t OrMask);

void xUSLMemReadModifyWrite16(void *Addr, uint16_t AndMask, uint16_t OrMask);

void xUSLMemReadModifyWrite32(void *Addr, uint32_t AndMask, uint32_t OrMask);

