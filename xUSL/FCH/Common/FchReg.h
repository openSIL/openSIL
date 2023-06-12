/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved. */
// SPDX-License-Identifier: MIT
/**
 * @file FchReg.h
 * @brief FCH Registers
 *
 */

/**********************************************************************************************************************
 * Declare common variables here
 *
 */



/**
 * Declare function prototypes here
 *
 */

/**********************************************************************************************************************
 * Declare macros here
 *
 */

#define FCH_GPIO_BANK0_PULLUPENABLE BIT_32(20)
#define FCH_GPIO_BANK0_PULLDOWNENABLE BIT_32(21)

//FCH register SMN address
#define FCH_SN_SMN_PMIO_BASE                0x02D01300ul

/** ALTMMIOEN
 * Reg(FCH::PM::ALTMMIOEN)
 * Classification(Public)
 * SOC(Genoa)
 */
#define FCH_PMIO_ALTMMIOEN                 0xD5

/** ALTMMIOBASE
 * Reg(FCH::PM::ALTMMIOBASE)
 * Classification(Public)
 * SOC(Genoa)
 */
#define FCH_PMIO_ALTMMIOBASE               0xD6
