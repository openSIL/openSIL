/**
 * @file
 * @brief OpenSIL CCX Apic modes and function declarations.
 *
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once
#include <Utils.h>

/**********************************************************************************************************************
 * Module data structure
 *
 */

/**********************************************************************************************************************
 * variable declaration
 *
 */
typedef enum {
  ApicCompatibilityMode   = 0x00,     // threads below 255 run in xAPIC with xAPIC ACPI structures and threads 255 &
                                      // above run in x2 mode with x2 ACPI structures
  xApicMode               = 0x01,     // force legacy xApic mode (downcores if enabled threads are >= 255)
  x2ApicMode              = 0x02,     // force x2Apic mode independent of thread count
  ApicAutoMode            = 0xFF      // Auto
} APIC_MODE;


/**********************************************************************************************************************
 * Declare macros here
 *
 */
#define XAPIC_ID_MAX 0xFF
