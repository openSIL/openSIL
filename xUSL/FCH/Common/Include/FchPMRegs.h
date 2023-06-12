/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file FchPMRegs.h
 *
 */

#pragma once



/** IO Decode Enable Register
 * Reg(FCH::PM::DECODEEN)
 */
#define FCH_PM_DECODEEN                                                        0x0

/** ISA Control Register
 * Reg(FCH::PM::ISACONTROL)
 */
#define FCH_PM_ISACONTROL                                                      0x4

/** PCI Control Register
 * Reg(FCH::PM::PCICONTROL)
 */
#define FCH_PM_PCICONTROL                                                      0x8

/** CF9 Reset Control Register
 * Reg(FCH::PM::PWRRSTCFG)
 */
#define FCH_PM_PWRRSTCFG                                                       0x10

/** S0 Reset B Timer Value Register
 * Reg(FCH::PM::S0RESETTMR)
 */
#define FCH_PM_S0RESETTMR                                                      0x3C

/** Boot Timer Enable Register
 * Reg(FCH::PM::BOOTTIMEREN)
 */
#define FCH_PM_BOOTTIMEREN                                                     0x44

/** IRQ Configuration Register
 * Reg(FCH::PM::SERIALIRQCONFIG)
 */
#define FCH_PM_SERIALIRQCONFIG                                                 0x54

/** RTC Shadow Register
 * Reg(FCH::PM::RTCSHADOW)
 */
#define FCH_PM_RTCSHADOW                                                       0x5B

/** External RTC Address Index Register
 * Reg(FCH::PM::RTCEXTINDEX)
 */
#define FCH_PM_RTCEXTINDEX                                                     0x5E

/** External RTC Data Register
 * Reg(FCH::PM::RTCEXTDATA)
 */
#define FCH_PM_RTCEXTDATA                                                      0x5F

/** PM1 Event Block Register
 * Reg(FCH::PM::ACPIPM1EVTBLK)
 */
#define FCH_PM_ACPIPM1EVTBLK                                                   0x60

/** PM1 Control Block Register
 * Reg(FCH::PM::ACPIPM1CNTBLK)
 */
#define FCH_PM_ACPIPM1CNTBLK                                                   0x62

/** PM Timer Block Register
 * Reg(FCH::PM::ACPIPMTMRBLK)
 */
#define FCH_PM_ACPIPMTMRBLK                                                    0x64

/**
 * Reg(FCH::PM::ACPIGPE0BLK)
 */
#define FCH_PM_ACPIGPE0BLK                                                     0x68

/**
 * Reg(FCH::PM::ACPISMICMD)
 */
#define FCH_PM_ACPISMICMD                                                      0x6A

/**
 * Reg(FCH::PM::ACPIPMACNTBLK)
 */
#define FCH_PM_ACPIPMACNTBLK                                                   0x6E

/** ACPI Configuration Register
 * Reg(FCH::PM::ACPICONFIG)
 */
#define FCH_PM_ACPICONFIG                                                      0x74

/** SPI Pad Control Register
 * Reg(FCH::PM::SPIPADPUPD)
 */
#define FCH_PM_SPIPADPUPD                                                      0x90

/** Sleep State Control Register
 * Reg(FCH::PM::S_STATECONTROL)
 */
#define FCH_PM_S_STATECONTROL                                                  0xBA

/** PM Throttling Control Register
 * Reg(FCH::PM::THROTTLINGCONTROL)
 */
#define FCH_PM_THROTTLINGCONTROL                                               0xBC

/** PM S5 Reset Status Register
 * Reg(FCH::PM::S5_RESET_STATUS)
 */
#define FCH_PM_S5_RESET_STATUS                                                 0xC0

/** PM Reset Command Register
 * Reg(FCH::PM::RESETCOMMAND)
 */
#define FCH_PM_RESETCOMMAND                                                    0xC4

/** PM IO Miscellaneous Register
 * Reg(FCH::PM::MISC_PMIO)
 */
#define FCH_PM_MISC_PMIO                                                       0xC8

/** PM IO Debug Register
 * Reg(FCH::PM::PMIODEBUG)
 */
#define FCH_PM_PMIODEBUG                                                       0xD2

/** Power Signals Debug Bus Select Register
 * Reg(FCH::PM::PWRRSTDEBSEL)
 */
#define FCH_PM_PWRRSTDEBSEL                                                    0xD4

/** PM XDC Sata Configuration Register
 * Reg(FCH::PM::PMIO_XDC_SATACONFIG)
 */
#define FCH_PM_PMIO_XDC_SATACONFIG                                             0xDC

/** AB IO Base Address Register (bits 3 to 15)
 * Reg(FCH::PM::ABREGBAR)
 */
#define FCH_PM_ABREGBAR                                                        0xE0

#define SIL_RESERVED_0331                                                    0xE8

/** PM USB Gating Register
 * Reg(FCH::PM::USBGATING)
 */
#define FCH_PM_USBGATING                                                       0xED
