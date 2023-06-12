/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file FchIORegs.h
 *
 */

#pragma once



/** NMI Enable/RTC Address Port Register
 * Reg(FCH::IO::NMI_ENABLE)
 */
#define FCH_IO_NMI_ENABLE                                                      0x70

/** RTC Data Port Register
 * Reg(FCH::IO::RTCDATAPORT)
 */
#define FCH_IO_RTCDATAPORT                                                     0x71

/** PCI Interrupt Index Register
 * Reg(FCH::IO::PCI_INTR_INDEX)
 */
#define FCH_IO_PCI_INTR_INDEX                                                  0xC00

/** PCI Interrupt Data Register
 * Reg(FCH::IO::PCI_INTR_DATA)
 */
#define FCH_IO_PCI_INTR_DATA                                                   0xC01
