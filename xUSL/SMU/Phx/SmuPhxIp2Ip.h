/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  SmuPhxIp2Ip.h
 * @brief OpenSIL SMU PHX Ip2Ip Api functions functions
 *
 */
#pragma once

#include <xSIM.h>
#include <stdint.h>
#include <SMU/SmuDefs.h>
#include <Pci.h>
#include <Nbio/Common/GnbDxio.h>

SIL_STATUS
SmuReadCacWeightsPhx (
  SIL_CONTEXT   *SilContext,
  uint32_t      MaxNumWeights,
  uint64_t      *ApmWeights
  );

SMC_RESULT
SmuServiceRequestPhx (
  PCI_ADDR PciAddress,
  uint32_t RequestId,
  uint32_t *RequestArgument,
  uint32_t AccessFlags
  );

SIL_STATUS
SmuGetOpnCorePresencePhx (
  SIL_CONTEXT    *SilContext,
  uint32_t       SystemDieNumber,
  uint32_t       *CcdEnableMap,
  uint32_t       *CcdDisableMap,
  uint32_t       *CoreDisMap,
  uint32_t       *CoreDisMapLength,
  bool           *SmtEnable
  );

SIL_STATUS
SmuGetOpnCorePresenceExPhx (
  uint32_t  SystemDieNumber,
  uint32_t  *CcdEnableMap,
  uint32_t  *CcdDisableMap,
  uint32_t  *CoreDisMap,
  uint32_t  *CoreDisMapBufferSize,
  uint32_t  *CoreDisMapLength,
  bool      *SmtEnable,
  uint32_t  *SmtEnableBufferSize
  );

SMC_RESULT
SmuServiceRequestOnlyPhx (
  PCI_ADDR PciAddress,
  uint32_t RequestId,
  uint32_t *RequestArgument,
  uint32_t AccessFlags
  );

void
SmuNotifyS3EntryPhx (
  void
  );

void
SmuServiceInitArgumentsCommon (
  uint32_t *SmuArg
  );

SIL_STATUS
SmuFirmwareTestCommon (
  SIL_CONTEXT *SilContext,
  GNB_HANDLE  *NbioHandle
  );

SIL_STATUS SmuGetGnbHandleCommon (
  SIL_CONTEXT    *SilContext,
  uint32_t       InstanceId,
  GNB_HANDLE     **Handle
  );

SIL_STATUS
SmuReadBrandStringPhx (
  SIL_CONTEXT    *SilContext,
  uint32_t       InstanceId,
  uint32_t       BrandStringLength,
  uint8_t        *BrandString
  );

SIL_STATUS
SmuReadBistInfoPhx (
  SIL_CONTEXT    *SilContext,
  uint8_t        InstanceId,
  uint8_t        PhysicalCcx,
  uint32_t       *BistInfo
  );

SIL_STATUS
SmuReadCacWeightsV13 (
  uint32_t MaxNumWeights,
  uint64_t *ApmWeights
  );

SIL_STATUS
SmuRegisterReadDie (
  SIL_CONTEXT    *SilContext,
  uint32_t       InstanceId,
  uint32_t       RegisterIndex,
  uint32_t       *RegisterValue
  );

SIL_STATUS
SmuRegisterWriteDie (
  SIL_CONTEXT    *SilContext,
  uint32_t       InstanceId,
  uint32_t       RegisterIndex,
  uint32_t       *RegisterValue
  );

SIL_STATUS
SmuRegisterRMWDie (
  SIL_CONTEXT    *SilContext,
  uint32_t       InstanceId,
  uint32_t       RegisterIndex,
  uint32_t       RegisterANDValue,
  uint32_t       RegisterORValue
  );

SIL_STATUS
SmuDisableSmtCommon (
  void
  );
