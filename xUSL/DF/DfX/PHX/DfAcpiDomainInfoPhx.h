/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  DfAcpiDomainInfoPhx.h
 * @brief Fabric ACPI Domain info definitions for Phx
 *
 */


#pragma once

#include <SilCommon.h>
#include <ProjSocConst.h>
#include <stdint.h>
#include <DF/DfX/DfXAcpiDomainInfo.h>
#include <DF/DfX/PHX/DfSilFabricInfoPhx.h>

#pragma pack(push, 1)
#define DF_PHX_DOMAIN_INSTANCE     2

typedef struct {
  uint32_t                 CcdCount[PROJ_MAX_SOCKETS_SUPPORTED];
  uint32_t                 CcxPerCcd[PROJ_MAX_SOCKETS_SUPPORTED];
  uint32_t                 LogToPhysCcd[PROJ_MAX_SOCKETS_SUPPORTED][PROJ_MAX_CCD_DIES_PER_SOCKET];
  uint32_t                 LogToPhysCcm[PROJ_MAX_SOCKETS_SUPPORTED][PHX_NUM_CCM_BLOCKS];
  uint32_t                 ReportedDomainCcxMap[MAX_REPORTED_DOMAINS];
  FABRIC_DOMAIN_INFO2      ReportedDomainInfo[MAX_REPORTED_DOMAINS];
  FABRIC_PHYS_DOMAIN_INFO  PhysicalDomainInfo[MAX_PHYSICAL_DOMAINS];
  uint32_t                 CcmCount[PROJ_MAX_SOCKETS_SUPPORTED];
  uint32_t                 CcxPerCcm[PROJ_MAX_SOCKETS_SUPPORTED];
  bool                     DomainInfoValid;
  uint32_t                 NumberOfReportedDomains;
  bool                     IsMixedDimmConfig;
  bool                     IsLowIntlvAbove;
  uint32_t                 PhysNps;
  uint32_t                 SystemSLinkCount;
} DF_PHX_DOMAIN_INFO_BLK;

#pragma pack(pop)
