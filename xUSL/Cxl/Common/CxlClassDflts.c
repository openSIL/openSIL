/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

/**
 * @file CxlClassDflts.c
 * @brief This file holds the platform default values for the CXL Input Block
 */

#include "CxlClass-api.h"

/// Default values for CXL configuration data
const CXLCLASS_DATA_BLK CxlClassDflts = {
    .CxlInputBlock = {
        .ReportErrorsToRcec = true,
        .CxlIoArbWeights = 0,
        .CxlCaMemArbWeights = 0,
        .CnliTokenAdvertisement = 0,
        .AmdCxlProtocolErrorReporting = 0,
        .AmdPcieAerReportMechanism = 1,
        .CxlCamemRxOptimization = false,
        .CxlTxOptimizeDirectOutEn = false
    },
    .CxlOutputBlock = {
        .AmdPcieAerReportMechanism = 1
    }
};