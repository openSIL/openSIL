/**
 * @file    RasIp2Ip.h
 * @details This file contains the definition of the RAS Ip 2 Ip API.
 *          This API provides other openSIL IPs with any SMU functionality or data
 *          they require while also abstracting revision specific differences.
 */

/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once
#include <xSIM-api.h>
#include <RAS/Common/RasClass-api.h>

/**
 * IP-to-IP APIs
 *
 * This structure contains an API for inter IP communication.
 *
 * IP specific code should populate this structure of function pointers with
 * the appropriate API function versions.
 *
 */
typedef uint8_t (*GET_UMC_PER_DIE) (void);

typedef uint32_t (*GET_UMC_HARVEST) (
  size_t    BusNumberBase
);

typedef SIL_STATUS (*UPDATE_FRU_TEXT_TO_UMC) (
  uint32_t              IohcBus,
  SIL_FRUTEXT_ENTRY *AmdFruTextSEntry
 );

typedef void (*TRANS_NORM_TO_DRAM_ADDR) (
  uint64_t      ChannelAddr,
  uint8_t       PkgNo,
  uint8_t       MpuNo,
  uint8_t       UmcInstNum,
  uint8_t       UmcChanNum,
  uint8_t       *CsNum,
  uint8_t       *Bank,
  uint32_t      *Row,
  uint16_t      *Col,
  uint8_t       *RankMul,
  uint8_t       *SubChan,
  SIL_ADDR_DATA *AddrData
);

typedef void (*TRANS_NORM_TO_DPA) (
  uint64_t      ChannelAddr,
  uint8_t       PkgNo,
  uint8_t       MpuNo,
  uint8_t       UmcInstNum,
  uint8_t       UmcChanNum,
  uint8_t       CsNum,
  uint8_t       Bank,
  uint32_t      Row,
  uint16_t      Col,
  uint8_t       RankMul,
  uint64_t      *Dpa,
  SIL_ADDR_DATA *AddrData
  );
typedef void (*CALC_NORM_ADDR) (
  uint64_t                SysAddr,
  SIL_NORMALIZED_ADDRESS  *NormalizedAddress
  );

typedef uint64_t (*CALC_SYS_ADDR) (
  uint64_t      NormAddr,
  uint32_t      AddrSocketNum,
  uint32_t      UmcPhysChannelNum
);

typedef void (*SET_SMI_TRIG_IO_CYCLE) (
  uint8_t   IoData,
  uint16_t  IoPortAddress
  );

typedef void (*RAS_SMM_EXIT_TYPE) (
  uint8_t SmiExitType
  );

typedef SIL_STATUS (*SET_CORE_MCA_THRESH) (
  uint32_t      McaBankNumber,
  bool          AllBanks,
  bool          ThresholdControl,
  uint16_t      ThresholdCount,
  uint8_t       ThresholdIntType,
  bool          OvrflwChk,
  uint8_t       ErrCounterEn
  );

typedef void (*SET_IP_MCA_CTL_MASK) (
  uint16_t          HardwareId,
  uint16_t          McaType,
  SIL_IP_RAS_POLICY *IpMcaPolicyCfg
  );

typedef void (*PROG_CORE_MCA_IPID_INST) (
  SIL_CPU_INFO *RasCpuInfo
  );

typedef void (*PROG_CORE_MCA_CFG_UMC) (
  bool  EnableFruText
  );

typedef struct {
  SIL_DATA_BLOCK_ID IpId;       ///< Block Id for this ip
  uint8_t           Version;
} RAS_IP2IP_API_HEADER;

typedef struct {
  RAS_IP2IP_API_HEADER    Header;
  GET_UMC_PER_DIE         GetUmcPerDie;
  GET_UMC_HARVEST         GetUmcHarvestFuse;
  UPDATE_FRU_TEXT_TO_UMC  UpdateFruTextToUmc;
  TRANS_NORM_TO_DRAM_ADDR TranslateNormToDramAddr;
  CALC_SYS_ADDR           CalcSysAddr;
  CALC_NORM_ADDR          CalcNormAddr;
  SET_IP_MCA_CTL_MASK     SetIpMcaCtlMask;
  PROG_CORE_MCA_IPID_INST ProgramCoreMcaIpIdInstanceId;
  PROG_CORE_MCA_CFG_UMC   ProgramCoreMcaConfigUmc;
} RAS_IP2IP_API;
