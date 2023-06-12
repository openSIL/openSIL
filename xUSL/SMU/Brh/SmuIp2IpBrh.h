/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  SmuBrhIp2Ip.h
 * @brief OpenSIL SMU BRH Ip2Ip Api functions functions
 *
 */
#pragma once
#include <xSIM.h>
#include <stdint.h>
#include <SMU/SmuDefs.h>

SMC_RESULT
SmuServiceRequestBrh (
  PCI_ADDR PciAddress,
  uint32_t RequestId,
  uint32_t *RequestArgument,
  uint32_t AccessFlags
  );

SIL_STATUS
SmuGetOpnCorePresenceBrh (
  uint32_t SystemDieNumber,
  uint32_t *CcdEnableMap,
  uint32_t *CcdDisableMap,
  uint32_t *CoreDisMap,
  uint32_t *CoreDisMapLength,
  bool     *SmtEnable
  );

SIL_STATUS
SmuGetOpnCorePresenceExBrh (
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
SmuServiceRequestOnlyBrh (
  PCI_ADDR PciAddress,
  uint32_t RequestId,
  uint32_t *RequestArgument,
  uint32_t AccessFlags
  );

void
SmuNotifyS3EntryBrh (void);

SIL_STATUS
SmuLaunchThreadBrh (
  uint32_t Socket,
  uint32_t Die,
  uint32_t Ccd,
  uint32_t Complex,
  uint32_t Core,
  uint32_t Thread
  );

SIL_STATUS
SmuReadBrandStringBrh (
  uint32_t  InstanceId,
  uint32_t  BrandStringLength,
  uint8_t   *BrandString
  );

SIL_STATUS
SmuReadBistInfoBrh (
  uint8_t   InstanceId,
  uint8_t   PhysicalCcx,
  uint32_t  *BistInfo
  );

SIL_STATUS
SmuReadCacWeightsBrh (
  uint32_t MaxNumWeights,
  uint64_t *ApmWeights
  );
