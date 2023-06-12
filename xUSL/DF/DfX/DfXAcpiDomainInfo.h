/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */
/**
 * @file  DfXAcpiDomainInfo.h
 * @brief Fabric ACPI Domain info definitions for DfX
 *
 */

#pragma once

#include <SilCommon.h>
#include <ProjSocConst.h>
#include <stdint.h>

#define  DF_DRAM_NPS0                   0      ///< No NUMA nodes
#define  DF_DRAM_NPS1                   1      ///< 1 NUMA node per socket
#define  DF_DRAM_NPS2                   2      ///< 2 NUMA node per socket
#define  DF_DRAM_NPS4                   3      ///< 4 NUMA node per socket

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

/**
 *
 * @details This instance value represents the DF_DOMAIN_INFO_BLK in the SIL IP block data region.  These definitions
 *          are not defined in *-api.h because they are internal openSIL data.  They are created by openSIL and used
 *          by xPRF services to create SRAT APIC structures.
 *
 *          The DF_DOMAIN_INFO_BLK is not common between SOCs as the element and substructure sizes are dependent on
 *          SOC specific characteristics.
 *
 *          How can we use ProjSocConst.h for this?  The final values of the constants defined there are decided at
 *          build time (The largest value specific in the included *Charz.h headers).  But can the code handle larger
 *          constants than what is supported by the SOC?
 */
#define DF_DOMAIN_INSTANCE     1 // Instance number of DF_DOMAIN_INFO_BLK

#pragma pack(push, 1)
/// Domain type
typedef enum {
  NumaDram,
  NumaSLink,
  NumaCxl,
  MaxNumaDomainType2,
} FABRIC_DOMAIN_TYPE2;

/// Reported Domain Info
typedef struct {
  FABRIC_DOMAIN_TYPE2  Type;            ///< Type
  uint32_t             SocketMap;       ///< Bitmap indicating physical socket location
  uint32_t             PhysicalDomain;  ///< Physical domain number
} FABRIC_DOMAIN_INFO2;

/// Physical Dram Info
typedef struct {
  uint32_t   NormalizedCsMap;       ///< Bitmap of CSs comprising this physical domain
  uint32_t   SharingEntityCount;    ///< Number of virtual domains sharing this physical domain
  uint32_t   SharingEntityMap;      ///< Bitmap of reported domains that share this physical domain
  uint32_t   Reserved;              ///< Reserved
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
 * @retval   SilPass               Successful
 * @retval   SilInvalidParameter   An error was encountered. Data is not valid
 */
SIL_STATUS
DfXBuildDomainInfo (
  void
  );

/**
 * DfXDomainXlat
 *
 * @brief This function translates a core's physical location to the appropriate NUMA domain.
 *
 * @param[in]   Socket                Zero based socket that the core is attached to
 * @param[in]   Die                   DF die on socket that the core is attached to
 * @param[in]   Ccd                   Logical CCD the core is on
 * @param[in]   Ccx                   Logical core complex
 * @param[out]  Domain                Domain the core belongs to
 *
 * @retval      SilPass               Domain is valid
 * @retval      SilInvalidParameter   No core at location
 */
SIL_STATUS
DfXDomainXlat (
  uint32_t   Socket,
  uint32_t   Die,
  uint32_t   Ccd,
  uint32_t   Ccx,
  uint32_t   *Domain
  );
