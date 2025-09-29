/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  DfXAcpiDomainInfo.h
 * @brief Fabric ACPI Domain info definitions for DfX
 *
 */

#pragma once

#include <SilCommon.h>
#include <ProjSocConst.h>
#include <stdint.h>

#define  DF_DRAM_NPS0                   0
#define  DF_DRAM_NPS1                   1
#define  DF_DRAM_NPS2                   2
#define  DF_DRAM_NPS4                   3

#define MAX_CXL_PER_SOCKET              PROJ_NUM_CS_CMP_BLOCKS
#define MAX_NPS                         4
#define NORMALIZED_SOCKET_SHIFT         (1u << 4)
#define SOCKET_CS_MAP                   ((1u << NORMALIZED_SOCKET_SHIFT) - 1u)
#define CS_CMP_SOCKET_MAP               (((1u << PROJ_NUM_CS_CMP_BLOCKS) - 1u) << PROJ_NUM_CS_UMC_BLOCKS)
#define CS_CMP_SYSTEM_MAP               (CS_CMP_SOCKET_MAP | (CS_CMP_SOCKET_MAP << NORMALIZED_SOCKET_SHIFT))
#define MAX_CCX_PER_CCD_PER_SOCKET      (PROJ_MAX_COMPLEXES_PER_CCD * \
        PROJ_MAX_CCD_DIES_PER_SOCKET * \
        PROJ_MAX_SOCKETS_SUPPORTED)
#define MAX_REPORTED_DOMAINS            (MAX_CCX_PER_CCD_PER_SOCKET + \
        (MAX_CXL_PER_SOCKET * PROJ_MAX_SOCKETS_SUPPORTED))
#define MAX_PHYSICAL_DOMAINS            ((PROJ_MAX_SOCKETS_SUPPORTED * MAX_NPS) + \
        (MAX_CXL_PER_SOCKET * PROJ_MAX_SOCKETS_SUPPORTED))

#define DF_DOMAIN_INSTANCE     1

#pragma pack(push, 1)

typedef enum {
  NumaDram,
  NumaSLink,
  NumaCxl,
  MaxNumaDomainType2,
} FABRIC_DOMAIN_TYPE2;

typedef struct {
  FABRIC_DOMAIN_TYPE2  Type;
  uint32_t             SocketMap;
  uint32_t             PhysicalDomain;
} FABRIC_DOMAIN_INFO2;

typedef struct {
  uint32_t   NormalizedCsMap;
  uint32_t   SharingEntityCount;
  uint32_t   SharingEntityMap;
  uint32_t   Reserved;
} FABRIC_PHYS_DOMAIN_INFO;

typedef struct {
  uint32_t                 CcdCount[PROJ_MAX_SOCKETS_SUPPORTED];
  uint32_t                 CcxPerCcd[PROJ_MAX_SOCKETS_SUPPORTED];
  uint32_t                 LogToPhysCcd[PROJ_MAX_SOCKETS_SUPPORTED][PROJ_MAX_CCD_DIES_PER_SOCKET];
  uint32_t                 ReportedDomainCcxMap[MAX_REPORTED_DOMAINS];
  FABRIC_DOMAIN_INFO2      ReportedDomainInfo[MAX_REPORTED_DOMAINS];
  FABRIC_PHYS_DOMAIN_INFO  PhysicalDomainInfo[MAX_PHYSICAL_DOMAINS];
  bool                     DomainInfoValid;
  uint32_t                 NumberOfReportedDomains;
} DF_DOMAIN_INFO_BLK;
#pragma pack(pop)

/**
 * DfXBuildDomainInfo
 *
 * @brief    This function gathers data about the NUMA domains
 *
 * @param    SilContext            A context structure through which host firmware defined data
 *                                 can be passed to openSIL. The host firmware is responsible
 *                                 for initializing the SIL_CONTEXT structure.
 * @retval   SilPass               Successful
 * @retval   SilInvalidParameter   An error was encountered. Data is not valid
 */
SIL_STATUS
DfXBuildDomainInfo (
  SIL_CONTEXT  *SilContext
  );

/**
 * DfXDomainXlat
 *
 * @brief This function translates a core's physical location to the appropriate NUMA domain.
 *
 * @param  SilContext            A context structure through which host firmware defined data
 *                               can be passed to openSIL. The host firmware is responsible
 *                               for initializing the SIL_CONTEXT structure.
 * @param  Socket                Zero based socket that the core is attached to
 * @param  Die                   DF die on socket that the core is attached to
 * @param  Ccd                   Logical CCD the core is on
 * @param  Ccx                   Logical core complex
 * @param  Domain                Domain the core belongs to
 *
 * @retval      SilPass               Domain is valid
 * @retval      SilInvalidParameter   No core at location
 */
SIL_STATUS
DfXDomainXlat (
  SIL_CONTEXT  *SilContext,
  uint32_t     Socket,
  uint32_t     Die,
  uint32_t     Ccd,
  uint32_t     Ccx,
  uint32_t     *Domain
  );
