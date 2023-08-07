/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved. */
// SPDX-License-Identifier: MIT
/**
 * @file  SmuV13.c
 * @brief OpenSIL SMU V13 specific initialization
 *
 */

#pragma once

#include <xSIM.h>
#include <SMU/Common/SmuCommon.h>
#include <Apob.h>

SMC_RESULT
SmuServiceRequestV13 (
  PCI_ADDR PciAddress,
  SMC_MSG  RequestId,
  uint32_t *RequestArgument,
  uint32_t AccessFlags
  );

SMC_RESULT
DxioServiceRequestV13 (
  GNB_HANDLE  *NbioHandle,
  uint32_t    DxioMBoxRequest,
  uint32_t    *DxioMBoxArg,
  uint32_t    AccessFlags
  );

SIL_STATUS
SmuGetOpnCorePresenceExV13 (
  uint32_t  SystemDieNumber,
  uint32_t  *CcdEnableMap,
  uint32_t  *CcdDisableMap,
  uint32_t  *CoreDisMap,
  uint32_t  *CoreDisMapBufferSize,
  bool      *SmtEnable,
  uint32_t  *SmtEnableBufferSize
  );

SIL_STATUS
SmuGetOpnCorePresenceV13 (
  uint32_t SystemDieNumber,
  uint32_t *CcdEnableMap,
  uint32_t *CcdDisableMap,
  uint32_t *CoreDisMap,
  bool     *SmtEnable
  );

SIL_STATUS
SmuReadBrandStringV13 (
  uint32_t  InstanceId,
  uint32_t  BrandStringLength,
  uint8_t   *BrandString
  );

SIL_STATUS
SmuLaunchThreadV13 (
  uint32_t Socket,
  uint32_t Die,
  uint32_t LogicalCcd,
  uint32_t LogicalComplex,
  uint32_t LogicalCore,
  uint32_t LogicalThread
  );

SIL_STATUS
SmuReadBistInfoV13 (
  uint8_t   InstanceId,
  uint8_t   PhysicalCcx,
  uint32_t  *BistInfo
  );

SIL_STATUS
SmuReadCacWeightsV13 (
  uint32_t MaxNumWeights,
  uint64_t *ApmWeights
  );

SIL_STATUS
InitializeSmuApiV13 (void);

/**********************************************************************************************************************
 * Declare macros here
 *
 */

#define SIL_RESERVED_815                0x5D000
#define SIL_RESERVED_807                (SIL_RESERVED_815 + 0x3B8)
#define DFX_FUSE_CCD_PRESENT_1_OFFSET   22
#define DFX_FUSE_CCD_PRESENT_1_MASK     0xFFC00000
#define DFX_CCD_PRESENT_2_ADDR          (SIL_RESERVED_815 + 0x3BC)
#define DFX_CCD_PRESENT_2_OFFSET        0
#define DFX_CCD_PRESENT_2_MASK          0x3
#define SIL_RESERVED_810                10

#define SIL_RESERVED_811                (SIL_RESERVED_815 + 0x3BC)
#define DFX_FUSE_CCD_DOWN_ADDR_OFFSET   2
#define DFX_FUSE_CCD_DOWN_ADDR_MASK     0x3FFC

#define SIL_RESERVED_817                0x30081800
#define SIL_RESERVED_816                0x4A081800
#define SIL_RESERVED_814(CCD) ((CCD < 8) ? \
                               (SIL_RESERVED_817 + (CCD << 25)) : \
                               (SIL_RESERVED_816 + ((CCD - 8) << 25)))

#define SIL_RESERVED_818(CCD)            (SIL_RESERVED_814(CCD) + 0x4D0)
#define MP5_SMT_ENABLE_BIT               BIT_32(8)
#define CORE_FUSE_ADDR(FAMILY, CCD)      (SIL_RESERVED_814(CCD) + 0x4D0)
#define SIL_RESERVED_813(Ccd)            (SIL_RESERVED_814(Ccd) + 0x4D0)
#define MP5_CORE_DISABLE_OFFSET          0
#define SIL_RESERVED_808                 0xFF
#define MP5_CORE_DISABLE_CCX1_OFFSET     17
#define MP5_CORE_DISABLE_CCX1_MASK       0x01FE0000

#define CORE_DIS_MAP(FAMILY, CCD, FUSE)  ((FUSE & SIL_RESERVED_808) >> MP5_CORE_DISABLE_OFFSET)
