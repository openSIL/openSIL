
/**
 * @file  FchSpi.h
 * @brief FCH SPI related function prototypes and other definitions
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#include <FCH/Common/FchCommonCfg.h>

/**********************************************************************************************************************
 * @brief Function prototypes
 *
 */

void FchSpiPrePcieInit (FCH_SPI *FchSpi);
SIL_STATUS FchSpiSetInputBlk (void);
SIL_STATUS InitializeFchSpiTp1 (void);
SIL_STATUS InitializeFchSpiTp2 (void);
SIL_STATUS InitializeFchSpiTp3 (void);
/**********************************************************************************************************************
 * @brief Module data structure
 *
 */


/**********************************************************************************************************************
 * @brief Declare macros here
 *
 */
#define FCHSPI_MAJOR_REV 0
#define FCHSPI_MINOR_REV 1
#define FCHSPI_INSTANCE  0

