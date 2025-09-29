/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file FchPMRegs.h
 */

#pragma once

#define FCH_PM_DECODEEN                                                        0x0
#define FCH_PM_ISACONTROL                                                      0x4
#define FCH_PM_PCICONTROL                                                      0x8
#define FCH_PM_PWRRSTCFG                                                       0x10
#define FCH_PM_PGPWRGOODTOALLENTMR                                             0x14
#define FCH_PM_PLLRSTBTMR                                                      0x1C
#define FCH_PM_PCIRSTBTMR                                                      0x20
#define FCH_PM_NBPWRGOODTMR                                                    0x24
#define FCH_PM_PWRRSTCNFG                                                      0x3E
#define FCH_PM_BOOTTIMEREN                                                     0x44
#define FCH_PM_PGPWRENDLY                                                      0x48
#define PMx0000004C                                               0x4C
#define FCH_PM_APUPLLCTRL                                                      0x50
#define FCH_PM_SERIALIRQCONFIG                                                 0x54
#define FCH_PM_RTCSHADOW                                                       0x5B
#define FCH_PM_RTCEXTINDEX                                                     0x5E
#define FCH_PM_RTCEXTDATA                                                      0x5F
#define FCH_PM_ACPIPM1EVTBLK                                                   0x60
#define FCH_PM_ACPIPM1CNTBLK                                                   0x62
#define FCH_PM_ACPIPMTMRBLK                                                    0x64
#define FCH_PM_ACPIGPE0BLK                                                     0x68
#define FCH_PM_ACPISMICMD                                                      0x6A
#define FCH_PM_ACPIPMACNTBLK                                                   0x6E
#define PMx00000070                                   0x70
#define FCH_PM_ACPICONFIG                                                      0x74
#define FCH_PM_BTTNSCICONTROL                                                  0x8C
#define FCH_PM_SPIPADPUPD                                                      0x90
#define FCH_PM_VIRTUALWIRE                                                     0xA8
#define FCH_PM_TPRESET2                                                        0xB8
#define FCH_PM_S_STATECONTROL                                                  0xBA
#define FCH_PM_RESETCONTROL2                                                   0xBF
#define FCH_PM_S5_RESET_STATUS                                                 0xC0
#define FCH_PM_RESETCOMMAND                                                    0xC4
#define FCH_PM_PMIODEBUG                                                       0xD2
#define FCH_PM_ABREGBAR                                                        0xE0
#define FCH_PM_USBGATING                                                       0xED
