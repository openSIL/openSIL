/**
 * @file  FabricAcpiDomainInfo.h
 * @brief Fabric ACPI DOMAIN Info prototypes
 *
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#define  DF_DRAM_NPS0                   0      ///< No NUMA nodes
#define  DF_DRAM_NPS1                   1      ///< 1 NUMA node per socket
#define  DF_DRAM_NPS2                   2      ///< 2 NUMA node per socket
#define  DF_DRAM_NPS4                   3      ///< 4 NUMA node per socket

#define NPS0_CCD_BITMASK                0x0FFF0FFF
#define NPS1_CCD_BITMASK                0x0FFF
#define NPS2_CCD_BITMASK                0x0555
#define NPS4_CCD_BITMASK_DOMAIN0        0x111
#define NPS4_CCD_BITMASK_DOMAIN1        0x444
#define NPS4_CCD_BITMASK_DOMAIN2        0x888
#define NPS4_CCD_BITMASK_DOMAIN3        0x222
#define MAX_CCX_PER_CCD                 2
#define MAX_CCX_PER_CCD_GENOA           1
#define MAX_CXL_PER_SOCKET              DFX_NUM_CS_CMP_BLOCKS
#define MAX_NPS                         4
#define NORMALIZED_SOCKET_SHIFT        (1 << 4)
#define SOCKET_CS_MAP                  ((1 << NORMALIZED_SOCKET_SHIFT) - 1)
#define CS_CMP_SOCKET_MAP              (((1 << DFX_NUM_CS_CMP_BLOCKS) - 1) << DFX_NUM_CS_UMC_BLOCKS)
#define CS_CMP_SYSTEM_MAP              (CS_CMP_SOCKET_MAP | (CS_CMP_SOCKET_MAP << NORMALIZED_SOCKET_SHIFT))
#define MAX_CCX_PER_CCD_PER_SOCKET     (MAX_CCX_PER_CCD_GENOA * DFX_MAX_CCD_PER_SOCKET * MAX_SOCKETS_SUPPORTED)
#define MAX_REPORTED_DOMAINS ( MAX_CCX_PER_CCD_PER_SOCKET + (MAX_CXL_PER_SOCKET * MAX_SOCKETS_SUPPORTED))
#define MAX_PHYSICAL_DOMAINS ((MAX_SOCKETS_SUPPORTED * MAX_NPS) + (MAX_CXL_PER_SOCKET * MAX_SOCKETS_SUPPORTED))

/**
 * BuildDomainInfo
 *
 * @brief    This function gathers data about the NUMA domains
 *           to be used by the protocol procedures
 *
 * @retval   SilPass               Successful
 * @retval   SilInvalidParameter   An error was encountered. Data is not valid
 */
SIL_STATUS
DfXBuildDomainInfo (
  void
  );

/**
 * DfDomainXlat
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
DfDomainXlat (
  uint32_t   Socket,
  uint32_t   Die,
  uint32_t   Ccd,
  uint32_t   Ccx,
  uint32_t   *Domain
  );
