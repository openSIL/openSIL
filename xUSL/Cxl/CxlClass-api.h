/* SPDX-License-Identifier: MIT */
/*********************************************************************
 *  Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 ********************************************************************/

/**
 * @file CxlClass-api.h
 * @brief openSIL-Host CXL IP interface
 *
 * @details The Compute Express Link (CXL) API adds the capability
 * to connect/enumerate CXL capable devices on the PCIe bus. A PCIe
 * port can auto-negotiate either a PCIe transaction, or enumerate
 * as a CXL device. MPIO firmware is responsible for link training
 * the CXL devices.
 *
 *  This file provides the structures details for the Host to configure the
 *  CXL operations.
 */

/** @cond API_Doc
 *  @ingroup MODULES_IP
 *  @page CXL  CXL IP api
 *
 *   The Compute Express Link (CXL) OpenSIL module determines CXL capabilities of all ports, which is
 *   the minimum CXL functionality required for enabling MPIO.
 *
 *
 *   See the 'Files - @ref CxlClass-api.h' section of this document for
 *   details.
 *
 *
 * @endcond
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>

#define CXLCLASS_MAJOR_REV   0
#define CXLCLASS_MINOR_REV   1
#define CXLCLASS_INSTANCE    0

///  CXL openSIL Input Block
typedef struct {
  bool ReportErrorsToRcec;              //Pcd
  uint8_t CxlIoArbWeights;              //Pcd
  uint8_t CxlCaMemArbWeights;           //Pcd
  uint8_t CnliTokenAdvertisement;       //Pcd
  uint8_t CxlHotPlugSlotTimeOut;        //Pcd
  uint8_t AmdCxlProtocolErrorReporting; //Pcd
  uint8_t AmdPcieAerReportMechanism;    //Pcd
  bool CxlCamemRxOptimization;          //Pcd
  bool CxlTxOptimizeDirectOutEn;        //Pcd
  bool SyncHeaderByPass;
} CXLCLASS_INPUT_BLK;

///  CXL openSIL Output Block
typedef struct {
  uint8_t AmdPcieAerReportMechanism; //Pcd
} CXLCLASS_OUTPUT_BLK;

typedef struct {
  CXLCLASS_INPUT_BLK CxlInputBlock;
  CXLCLASS_OUTPUT_BLK CxlOutputBlock;
} CXLCLASS_DATA_BLK;
