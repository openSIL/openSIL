/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file MpioLibBrh.c
 * @brief Breithorn specific Libraries used in MPIO
 *
 */

#include <Pci.h>
#include <Mpio/Common/MpioLib.h>
#include "MpioCmn2Brh.h"

/* Address C2PMSG 21 */
#define MPIO_C2PMSG_DOORBELL_ADDRESS_BRH                    0xC910954

/**---------------------------------------------------------
 * MpioGetC2pmsgDoorbellAddressBrh
 *
 * @brief Routine to get the PCIe port strap index value
 *
 * @param  Strap        PCIe strap value
 * @param  Port         Port ID on wrapper
 *
 * @return uint16_t     Port strap index
 */
uint32_t
MpioGetC2pmsgDoorbellAddressBrh (void)
{
  uint32_t MpioC2pmsgDoorbellAddress;

  MpioC2pmsgDoorbellAddress = MPIO_C2PMSG_DOORBELL_ADDRESS_BRH;
  return MpioC2pmsgDoorbellAddress;
}
