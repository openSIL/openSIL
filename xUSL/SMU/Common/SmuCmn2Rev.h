/**
 * @file  SmuCmn2Rev.h
 * This file defines the types of all functions contained within SMU's Cmn2Rev
 * xfer table, as well as the table itself as a struct containing pointers to
 * these functions
 */
/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once
#include <SMU/Common/SmuCommon.h>
#include <SMU/SmuDefs.h>

// Common function type definitions of Cmn2Rev functions

typedef SMC_RESULT (*SMU_CMN2REV_SERVICE_REQUEST) (
  PCI_ADDR  NbioPciAddress,
  SMC_MSG   RequestId,
  uint32_t  *RequestArgument,
  uint32_t  AccessFlags
  );

// Define the Cmn2Rev xfer table containing pointers to these functions

typedef struct {
  SMU_CMN2REV_SERVICE_REQUEST SmuServiceRequest;
} SMU_COMMON_2_REV_XFER_BLOCK;

