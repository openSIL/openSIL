/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2026 Advanced Micro Devices, Inc. All rights reserved.
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
#define CS_UMC_SOCKET_MAP               ((1u << PROJ_NUM_CS_UMC_BLOCKS) - 1u)
#define CS_UMC_SYSTEM_MAP               (CS_UMC_SOCKET_MAP | (CS_UMC_SOCKET_MAP << NORMALIZED_SOCKET_SHIFT))
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

/// Value of ExtendInfo when a physical domain carries no CXL DPA extension
#define DF_PHYS_DOMAIN_NO_EXT_INFO  0xFFFFFFFFu

/// Physical Dram Info
typedef struct {
  uint32_t   NormalizedCsMap;       ///< Bitmap of CSs comprising this physical domain
  uint32_t   SharingEntityCount;    ///< Number of virtual domains sharing this physical domain
  uint32_t   SharingEntityMap;      ///< Bitmap of reported domains that share this physical domain
  uint32_t   ExtendInfo;            ///< Index of the CXL DPA extension, or DF_PHYS_DOMAIN_NO_EXT_INFO
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
  uint32_t                 NumberOfPhysicalDomains;
  uint32_t                 PhysNps;                ///< actual NPS as reported by ABL, not counting SLink
  uint32_t                 SystemCxlCount;
  bool                     CcxAsNuma;
} DF_DOMAIN_INFO_BLK;
#pragma pack(pop)

/// One SRAT/CRAT memory range, in the 16-bit-shifted form both tables publish
typedef struct {
  uint32_t  Domain;
  uint32_t  BaseLo;
  uint32_t  BaseHi;
  uint32_t  SizeLo;
  uint32_t  SizeHi;
} DF_ACPI_MEM_ENTRY;

/// Per-domain memory total, the unsplit view MSCT and the SRAT service report
typedef struct {
  uint32_t  Domain;
  uint32_t  RegionSizeLo;
  uint32_t  RegionSizeHi;
} DF_ACPI_MEM_INFO;

#define DF_ACPI_MEM_ENTRY_WORDS  (sizeof (DF_ACPI_MEM_ENTRY) / sizeof (uint32_t))
#define DF_ACPI_MEM_INFO_WORDS   (sizeof (DF_ACPI_MEM_INFO) / sizeof (uint32_t))

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

/**
 * DfXAcpiGetDomainCounts
 *
 * @brief Return the scalar domain counts the ACPI generators need.
 *
 * @param[out] ReportedDomainCount  Reported (OS visible) NUMA domain count
 * @param[out] PhysicalDomainCount  Physical domain count
 * @param[out] PhysNodesPerSocket   Actual NPS as determined by ABL
 * @param[out] SystemCxlCount       Number of CXL domains at the end of the list
 * @param[out] MaxDomains           Maximum domain count; MSCT uses this count minus one
 * @param[out] CcxAsNuma            True when each core complex is its own domain
 *
 * @retval SilPass                  Counts are valid
 * @retval SilUnsupported           Domain info has not been built
 */
SIL_STATUS
DfXAcpiGetDomainCounts (
  uint32_t  *ReportedDomainCount,
  uint32_t  *PhysicalDomainCount,
  uint32_t  *PhysNodesPerSocket,
  uint32_t  *SystemCxlCount,
  uint32_t  *MaxDomains,
  bool      *CcxAsNuma
  );

/**
 * DfXAcpiGetReportedDomains
 *
 * @brief Copy out the reported domain table as Type/SocketMap/PhysicalDomain triples.
 *
 * @param[in]  MaxDomains  Capacity of Buffer, in domains
 * @param[out] Count       Domains written
 * @param[out] Buffer      Receives three uint32_t per domain
 *
 * @retval SilPass         Buffer populated
 */
SIL_STATUS
DfXAcpiGetReportedDomains (
  uint32_t  MaxDomains,
  uint32_t  *Count,
  uint32_t  *Buffer
  );

/**
 * DfXAcpiGetPhysicalDomains
 *
 * @brief Copy out the physical domain table.
 *
 * @param[in]  MaxDomains  Capacity of Buffer, in domains
 * @param[out] Count       Domains written
 * @param[out] Buffer      Receives four uint32_t per domain: normalized CS map,
 *                         sharing entity count, sharing entity map, extend info
 *
 * @retval SilPass         Buffer populated
 */
SIL_STATUS
DfXAcpiGetPhysicalDomains (
  uint32_t  MaxDomains,
  uint32_t  *Count,
  uint32_t  *Buffer
  );

/**
 * DfXAcpiCollectMemoryEntries
 *
 * @brief Walk the DRAM address map and produce the SRAT/CRAT memory ranges.
 *
 * @details Reads the fabric DRAM range registers every time it is called, so
 *          the caller decides when the map is final rather than depending on a
 *          time point having run.
 *
 * @param[in]  MaxEntries      Capacity of both output buffers, in entries
 * @param[out] MemEntryCount   Memory ranges written, sorted by ascending domain
 * @param[out] MemEntryBuffer  Receives DF_ACPI_MEM_ENTRY_WORDS uint32_t per range
 * @param[out] MemInfoCount    Per-domain totals written
 * @param[out] MemInfoBuffer   Receives DF_ACPI_MEM_INFO_WORDS uint32_t per total
 *
 * @retval SilPass             Buffers populated
 * @retval SilOutOfBounds      MaxEntries too small for this configuration
 */
SIL_STATUS
DfXAcpiCollectMemoryEntries (
  uint32_t  MaxEntries,
  uint32_t  *MemEntryCount,
  uint32_t  *MemEntryBuffer,
  uint32_t  *MemInfoCount,
  uint32_t  *MemInfoBuffer
  );

/**
 * DfXAcpiGetDistanceInfo
 *
 * @brief Fill the SLIT/CDIT distance matrix for the reported domains.
 *
 * @param[in]  DomainCount  Matrix order
 * @param[in]  BufferSize   Capacity of Distance, in bytes
 * @param[out] Distance     Receives DomainCount squared entries, row major
 *
 * @retval SilPass          Matrix written
 * @retval SilOutOfBounds   BufferSize too small
 */
SIL_STATUS
DfXAcpiGetDistanceInfo (
  uint32_t  DomainCount,
  uint32_t  BufferSize,
  uint8_t   *Distance
  );

/**
 * DfXAcpiGetPxmDomains
 *
 * @brief Return the proximity domains behind one host bridge.
 *
 * @details Stateless: the caller applies any round-robin policy and any
 *          fallback for bridges the fabric does not own, such as CXL.
 *
 * @param[in]  BusBase   Segment adjusted base bus number of the host bridge
 * @param[in]  MaxCount  Capacity of Domains
 * @param[out] Count     Domains written, zero when the bridge is not fabric owned
 * @param[out] Domains   Receives the proximity domain numbers
 *
 * @retval SilPass       Lookup completed, Count may be zero
 */
SIL_STATUS
DfXAcpiGetPxmDomains (
  uint32_t  BusBase,
  uint32_t  MaxCount,
  uint32_t  *Count,
  uint32_t  *Domains
  );
