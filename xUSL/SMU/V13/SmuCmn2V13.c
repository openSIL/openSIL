/**
 * @file  SmuCmn2V13.c
 * @brief OpenSIL SMU V13 transfer block functions
 *
 */

/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SMU/SmuDefs.h>
#include <SMU/Common/SmuCmn2Rev.h>
#include "SmuV13.h"

SMU_COMMON_2_REV_XFER_BLOCK mSmuXferV13 = {
  .SmuServiceRequest  = SmuServiceRequestV13
};

/**
 * SmuServiceRequestV13
 *
 * @brief   SMU service request for SMU V13
 *
 * @param   PciAddress      PCI_ADDR of this NBIO
 * @param   RequestId       Host Firmware to SMU Message ID
 * @param   RequestArgument Request Argument
 * @param   AccessFlags     See GNB_ACCESS_FLAGS_* definitions
 *
 * @retval  SmuMessageResponse SMC_RESULT
 */
SMC_RESULT
SmuServiceRequestV13 (
  PCI_ADDR PciAddress,
  SMC_MSG  RequestId,
  uint32_t *RequestArgument,
  uint32_t AccessFlags
  )
{
  return SmuServiceRequestCommon (
    PciAddress,
    RequestId,
    RequestArgument,
    AccessFlags
    );
}
