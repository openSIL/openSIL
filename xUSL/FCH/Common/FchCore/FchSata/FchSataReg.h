/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FchSataReg.h
 * @brief FCH SATA Controller Registers Definition
 *
 */

#pragma once
#include <FCH/FchClass-api.h>
#include <FCH/Common/FchReg.h>

#define FCH_SMN_SATA_STEP           0x100000ul
#define SIL_RESERVED_0292      (FCH_SMN_SATA_CONTROL_RSMU + SIL_RESERVED_0344)
#define SIL_RESERVED_0291      (FCH_SMN_SATA_CONTROL_RSMU + SIL_RESERVED_0345)
#define SIL_RESERVED_0286       (0x0100 + SYSHUB_SATA_PORT_SATA_AHCI_P_IS)
#define SIL_RESERVED_0287       (0x0100 + SYSHUB_SATA_PORT_SATA_AHCI_P_CMD)
#define SIL_RESERVED_0288       (0x0100 + SYSHUB_SATA_PORT_SATA_AHCI_P_SSTS)
#define SIL_RESERVED_0289       (0x0100 + SYSHUB_SATA_PORT_SATA_AHCI_P_SCTL)
#define SIL_RESERVED_0290       (0x0100 + SYSHUB_SATA_PORT_SATA_AHCI_P_SERR)
