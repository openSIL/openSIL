/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */
/**
 * @file  DfXAcpiDomainInfo.c
 * @brief OpenSIL Fabric ACPI Domain information for SRAT, CRAT, SLIT, CDIT, and MSCT
 *
 */

#include <SilCommon.h>
#include <APOB/Common/ApobCmn.h>
#include <DF/Df.h>
#include <DF/Common/BaseFabricTopologyCmn.h>
#include <DF/Common/DfCmn2Rev.h>
#include <string.h>
#include "DfXAcpiDomainInfo.h"
#include <APOB/Common/ApobCmn2Rev.h>
#include <APOB/ApobIp2Ip.h>

/**
 * BuildCxlInfo
 *
 * @brief This function gathers data about CCMs
 *
 * @param  Index
 * @param  DomainInfo   Input Pointer to the domain info block
 *
 * @retval   uint32_t   Number of Cxls populated with memory
 */
static
uint32_t
BuildCxlInfo (
  uint32_t                           Index,
  DF_DOMAIN_INFO_BLK                 *DomainInfo
  )
{
  uint32_t                           i;
  uint32_t                           ActiveCxlCount;
  APOB_SYSTEM_CXL_INFO_TYPE_STRUCT *CxlMap;
  bool                               CxlDeviceAttached[PROJ_MAX_SOCKETS_SUPPORTED];
  APOB_IP2IP_API                     *ApobIp2IpApi;
  SIL_STATUS                         Status;
  APOB_SOC_DIE_INFO                  SocMaxDieInfo;

  ActiveCxlCount = 0;

  Status = SilGetIp2IpApi(SilId_ApobClass, (void **) &ApobIp2IpApi);
  if ((Status != SilPass) || (ApobIp2IpApi == NULL)) {
    assert(Status == SilPass);
    return 0;
  }
  ApobIp2IpApi->ApobGetMaxDieInfo(&SocMaxDieInfo);

  for (i = 0; i < SocMaxDieInfo.MaxSocSocketsSupportedValue; i++) {
    CxlDeviceAttached[i] = false;
  }

  if ((ApobIp2IpApi->ApobAmdGetApobEntryInstance(APOB_FABRIC,
    APOB_SYS_CXL_INFO_TYPE,
    0,
    0,
    (APOB_TYPE_HEADER **)(void *) &CxlMap
    )) == SilPass) {
    for (i = 0; i < (sizeof (CxlMap->CxlInfo) / sizeof (CxlMap->CxlInfo[0])); i++) {
      if (CxlMap->CxlInfo[i].Size != 0 && CxlMap->CxlInfo[i].Status == CXL_ADDR_SUCCESS) {
        if (CxlMap->CxlInfo[i].Socket < SocMaxDieInfo.MaxSocSocketsSupportedValue) {
          CxlDeviceAttached[CxlMap->CxlInfo[i].Socket] = true;
        }
      }
    }
  }
  for (i = 0; i < SocMaxDieInfo.MaxSocSocketsSupportedValue; i++) {
    if (CxlDeviceAttached[i]) {
      // Build the CS map
      DomainInfo->PhysicalDomainInfo[(Index + ActiveCxlCount)].NormalizedCsMap
        = 0xF000 << (i * NORMALIZED_SOCKET_SHIFT);
      ActiveCxlCount++;
    }
  }

  DF_TRACEPOINT(SIL_TRACE_INFO, "BuildCxlInfo Successful - ActiveCxlCount: %d\n", ActiveCxlCount);

  return ActiveCxlCount;
}

/**
 * GetSocketMapFromNormalizedCsMap
 *
 * @brief This function returns a bitmap of sockets belonging to the CS map
 *
 * @param  NormalizedCsMap     CS map to check
 *
 * @retval     uint32_t            Socket bit map
 */
static
uint32_t
GetSocketMapFromNormalizedCsMap (
  uint32_t           NormalizedCsMap
  )
{
  uint32_t           i;
  uint32_t           SocketMap;
  APOB_IP2IP_API     *ApobIp2IpApi;
  APOB_SOC_DIE_INFO  SocMaxDieInfo;
  SIL_STATUS         Status;

  Status = SilGetIp2IpApi(SilId_ApobClass, (void **) &ApobIp2IpApi);
  if ((Status != SilPass) || (ApobIp2IpApi == NULL)) {
    assert(Status == SilPass);
    return 0;
  }

  ApobIp2IpApi->ApobGetMaxDieInfo(&SocMaxDieInfo);

  SocketMap = 0;
  for (i = 0; i < SocMaxDieInfo.MaxSocSocketsSupportedValue; i++) {
    SocketMap |= ((NormalizedCsMap & (SOCKET_CS_MAP << ((uint32_t) i * NORMALIZED_SOCKET_SHIFT)))
      != 0) ? (1 << (uint32_t) i) : 0;
  }

  DF_TRACEPOINT(SIL_TRACE_INFO, "GetSocketMapFromNormalizedCsMap SocketMap value %d\n", SocketMap);
  return SocketMap;
}

/**
 * DfXBuildDomainInfo
 *
 * @brief    This function gathers data about the NUMA domains
 *
 * @retval   SilPass               Successful
 * @retval   SilInvalidParameter   An error was encountered. Data is not valid
 *           SilUnsupported        An error was encountered (NULL pointer,IP API not found)
 */
SIL_STATUS
DfXBuildDomainInfo (
  void
  )
{
  uint32_t                           Ccd;
  uint32_t                           Ccx;
  uint32_t                           Domain;
  uint32_t                           Half;
  uint32_t                           Quad;
  uint32_t                           Socket;
  uint32_t                           NumberOfCpus;
  uint32_t                           NumberOfDies;
  APOB_SYSTEM_NPS_INFO_TYPE_STRUCT   *NpsInfo = NULL;
  uint32_t                           i;
  uint32_t                           ReportedIndex;
  uint32_t                           CxlBase;
  uint32_t                           Nps0CcxMap;
  uint32_t                           Nps1CcxMap;
  uint32_t                           Nps2CcxMap[2];
  uint32_t                           Nps4CcxMap[4];
  DF_DOMAIN_INFO_BLK                 *DomainInfo;
  DFCLASS_INPUT_BLK                  *DfIpBlock;
  DF_COMMON_2_REV_XFER_BLOCK *DfXfer;
  SIL_STATUS                         Status;
  APOB_IP2IP_API                     *ApobIp2IpApi;
  uint32_t                           NumberOfPhysicalDomains = 0;
  uint32_t                           SystemCxlCount = 0;
  uint32_t                           NumberOfReportedDomains = 0;
  bool                               CcxAsNuma = false;


  Status = SilGetCommon2RevXferTable(SilId_DfClass, (void **) &DfXfer);
  assert(Status == SilPass);

  Status = SilGetIp2IpApi(SilId_ApobClass, (void **) &ApobIp2IpApi);
  if ((Status != SilPass) || (ApobIp2IpApi == NULL)) {
    assert(Status == SilPass);
    return Status;
  }

  DfIpBlock = (DFCLASS_INPUT_BLK *) xUslFindStructure(SilId_DfClass, DFCLASS_INSTANCE);
  if (DfIpBlock == NULL) {
    DF_TRACEPOINT(SIL_TRACE_ERROR, "DfXBuildDomainInfo failed to find Df input data blk\n");
    return SilUnsupported;
  }

  DomainInfo = (DF_DOMAIN_INFO_BLK *) xUslFindStructure(SilId_DfClass, DF_DOMAIN_INSTANCE);
  if (DomainInfo == NULL) {
    DF_TRACEPOINT(SIL_TRACE_ERROR, "DfXBuildDomainInfo failed to find Df domain info data blk\n");
    return SilUnsupported;
  }

  DomainInfo->DomainInfoValid = false;

  if ((ApobIp2IpApi->ApobAmdGetApobEntryInstance(APOB_FABRIC,
    APOB_SYS_NPS_INFO_TYPE,
    0,
    0,
    (APOB_TYPE_HEADER **) (void *) &NpsInfo
    ))
    != SilPass) {
    DF_TRACEPOINT(SIL_TRACE_ERROR, "DfXBuildDomainInfo GetApobEntryInstance failed\n");
    return SilUnsupported;
  } else if (NpsInfo == NULL) {
    DF_TRACEPOINT(SIL_TRACE_ERROR, "NpsInfo is NULL\n");
    return SilUnsupported;
  }

  NumberOfCpus = (uint32_t) DfXfer->DfGetNumberOfProcessorsPresent ();
  NumberOfDies = (uint32_t) DfXfer->DfGetNumberOfSystemDies ();
  CcxAsNuma = DfIpBlock->AmdFabricCcxAsNumaDomain;

  // Fill module global CCD data
  DfXfer->DfBuildCcdInfo((uint32_t) NumberOfCpus, (uint32_t) NumberOfDies, DomainInfo);
  NumberOfPhysicalDomains = 0;

  // Fill the
  DfXfer->DfGetNpsMapData(&Nps0CcxMap, &Nps1CcxMap, Nps2CcxMap, Nps4CcxMap);

  memset((void *) DomainInfo->ReportedDomainCcxMap,
    0x00,
    sizeof (DomainInfo->ReportedDomainCcxMap)
    );

  switch (NpsInfo->ActualNps) {
  case DF_DRAM_NPS0:
    assert(NumberOfCpus == 2);
    DomainInfo->PhysicalDomainInfo[NumberOfPhysicalDomains].NormalizedCsMap
      = ((1u << PROJ_NUM_CS_UMC_BLOCKS) - 1u) | (((1u << PROJ_NUM_CS_UMC_BLOCKS) - 1u) << NORMALIZED_SOCKET_SHIFT);
    if (CcxAsNuma == false) {
      DomainInfo->ReportedDomainCcxMap[NumberOfPhysicalDomains] = Nps0CcxMap;
    }
    NumberOfPhysicalDomains++;
    break;
  case DF_DRAM_NPS1:
    for (Socket = 0; Socket < NumberOfCpus; Socket++) {
      DomainInfo->PhysicalDomainInfo[NumberOfPhysicalDomains].NormalizedCsMap
        = (((1u << PROJ_NUM_CS_UMC_BLOCKS) - 1u) << ((uint32_t) Socket * NORMALIZED_SOCKET_SHIFT));

      if (CcxAsNuma == false) {
        DomainInfo->ReportedDomainCcxMap[NumberOfPhysicalDomains] =
          (Nps1CcxMap) << ((uint32_t) Socket * NORMALIZED_SOCKET_SHIFT);
      }
      NumberOfPhysicalDomains++;
    }
    break;
  case DF_DRAM_NPS2:
    for (Socket = 0; Socket < NumberOfCpus; Socket++) {
      for (Half = 0; Half < 2; Half++) {
        DomainInfo->PhysicalDomainInfo[NumberOfPhysicalDomains].NormalizedCsMap =
          ((((1u << (PROJ_NUM_CS_UMC_BLOCKS / 2)) - 1u) << ((uint32_t) Half * (PROJ_NUM_CS_UMC_BLOCKS / 2)))
          << ((uint32_t) Socket * NORMALIZED_SOCKET_SHIFT));
        if (CcxAsNuma == false) {
          DomainInfo->ReportedDomainCcxMap[NumberOfPhysicalDomains] =
            (Nps2CcxMap[Half]) << ((uint32_t) Socket * NORMALIZED_SOCKET_SHIFT);
        }
        NumberOfPhysicalDomains++;
      }
    }
    break;
  case DF_DRAM_NPS4:
    for (Socket = 0; Socket < NumberOfCpus; Socket++) {
      for (Quad = 0; Quad < 4; Quad++) {
        DomainInfo->PhysicalDomainInfo[NumberOfPhysicalDomains].NormalizedCsMap
          = ((((1u << (PROJ_NUM_CS_UMC_BLOCKS / 4)) - 1u) << ((uint32_t) Quad * (PROJ_NUM_CS_UMC_BLOCKS / 4)))
          << ((uint32_t) Socket * NORMALIZED_SOCKET_SHIFT));
        if (CcxAsNuma == false) {
          DomainInfo->ReportedDomainCcxMap[NumberOfPhysicalDomains] =
            (Nps4CcxMap[Quad]) << ((uint32_t) Socket * NORMALIZED_SOCKET_SHIFT);
        }
        NumberOfPhysicalDomains++;
      }
    }
    break;
  default:
    DF_TRACEPOINT(SIL_TRACE_ERROR, "Error: DfXBuildDomainInfo ActualNps value: %d\n", NpsInfo->ActualNps);
    assert(false);
    return SilUnsupported;
    break;
  }

  SystemCxlCount = BuildCxlInfo(NumberOfPhysicalDomains, DomainInfo);
  NumberOfPhysicalDomains += SystemCxlCount;

  if (NumberOfPhysicalDomains >= MAX_PHYSICAL_DOMAINS) {
    DF_TRACEPOINT(SIL_TRACE_ERROR, "Error: Domain value exit limit\n");
    assert(NumberOfPhysicalDomains < MAX_PHYSICAL_DOMAINS);
    return SilUnsupported;
  }

  if (CcxAsNuma) {
    NumberOfReportedDomains = SystemCxlCount;
    for (Socket = 0; Socket < NumberOfCpus; Socket++) {
      NumberOfReportedDomains += (DomainInfo->CcdCount[Socket]
        * DomainInfo->CcxPerCcd[Socket]);
    }
  } else {
    NumberOfReportedDomains = NumberOfPhysicalDomains;
  }

  assert(NumberOfReportedDomains <= MAX_REPORTED_DOMAINS);
  // Build reported domain info
  if (CcxAsNuma) {
    for (Domain = 0; Domain < MAX_PHYSICAL_DOMAINS; Domain++) {
      DomainInfo->PhysicalDomainInfo[Domain].SharingEntityCount = 0;
      DomainInfo->PhysicalDomainInfo[Domain].SharingEntityMap = 0;
    }

    // Start with non CXL
    ReportedIndex = 0;
    for (Socket = 0; Socket < NumberOfCpus; Socket++) {
      for (Ccd = 0; Ccd < DomainInfo->CcdCount[Socket]; Ccd++) {
        for (Ccx = 0; Ccx < DomainInfo->CcxPerCcd[Socket]; Ccx++) {
          DomainInfo->ReportedDomainInfo[ReportedIndex].Type = NumaDram;
          DomainInfo->ReportedDomainInfo[ReportedIndex].PhysicalDomain =
            DfXfer->DfGetPhysDomainOfCcd((uint32_t) Socket,
            DomainInfo->LogToPhysCcd[Socket][Ccd],
            NumberOfPhysicalDomains,
            DomainInfo
            );
          DomainInfo->ReportedDomainInfo[ReportedIndex].SocketMap
            = GetSocketMapFromNormalizedCsMap(DomainInfo->PhysicalDomainInfo
            [DomainInfo->ReportedDomainInfo[ReportedIndex].PhysicalDomain].NormalizedCsMap
            );
          DomainInfo->PhysicalDomainInfo[DomainInfo->
            ReportedDomainInfo[ReportedIndex].PhysicalDomain].SharingEntityCount++;
          DomainInfo->PhysicalDomainInfo[DomainInfo->
            ReportedDomainInfo[ReportedIndex].PhysicalDomain].SharingEntityMap |= (1 << ReportedIndex);
          ReportedIndex++;
        }
      }
    }

    CxlBase = NumberOfPhysicalDomains - SystemCxlCount;
    for (i = 0; i < SystemCxlCount; i++) {
      DomainInfo->ReportedDomainInfo[ReportedIndex].Type = NumaCxl;
      DomainInfo->ReportedDomainInfo[ReportedIndex].SocketMap
        = GetSocketMapFromNormalizedCsMap(DomainInfo->
        PhysicalDomainInfo[i + CxlBase].NormalizedCsMap
        );
      DomainInfo->ReportedDomainInfo[ReportedIndex].PhysicalDomain = i + CxlBase;
      DomainInfo->PhysicalDomainInfo[DomainInfo->
        ReportedDomainInfo[ReportedIndex].PhysicalDomain].SharingEntityCount = 1;
      DomainInfo->PhysicalDomainInfo[DomainInfo->
        ReportedDomainInfo[ReportedIndex].PhysicalDomain].SharingEntityMap = (1 << ReportedIndex);
      ReportedIndex++;
    }
    assert(ReportedIndex == NumberOfReportedDomains);
  } else if (CS_CMP_SYSTEM_MAP != 0) {
    for (Domain = 0; Domain < NumberOfPhysicalDomains; Domain++) {
      DomainInfo->ReportedDomainInfo[Domain].Type
        = (uint32_t) ((DomainInfo->PhysicalDomainInfo[Domain].NormalizedCsMap & CS_CMP_SYSTEM_MAP) != 0)
          ? NumaCxl : NumaDram;
      DomainInfo->ReportedDomainInfo[Domain].SocketMap
        = GetSocketMapFromNormalizedCsMap(DomainInfo->
        PhysicalDomainInfo[Domain].NormalizedCsMap
        );
      DomainInfo->ReportedDomainInfo[Domain].PhysicalDomain
        = Domain;
      DomainInfo->PhysicalDomainInfo[Domain].SharingEntityCount
        = 1;
      DomainInfo->PhysicalDomainInfo[Domain].SharingEntityMap
        = (1 << (uint32_t) Domain);
    }
  }

  DomainInfo->DomainInfoValid = true;
  DomainInfo->NumberOfReportedDomains = NumberOfReportedDomains;

  DF_TRACEPOINT(SIL_TRACE_INFO, "Updated BuildDomainInfo\n");
  return SilPass;
}


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
  uint32_t *Domain
  )
{
  DF_DOMAIN_INFO_BLK *DomainInfo;
  uint32_t                           i;
  uint32_t                           CcxPerCcd;
  DF_COMMON_2_REV_XFER_BLOCK *DfXfer;
  SIL_STATUS                         Status;

  Status = SilGetCommon2RevXferTable(SilId_DfClass, (void **) &DfXfer);
  CcxPerCcd = (SilGetMemoryBase ())->ActiveSoC.NumCcxPerCcd;
  DomainInfo = (DF_DOMAIN_INFO_BLK *) xUslFindStructure(SilId_DfClass, DF_DOMAIN_INSTANCE);

  if (Status != SilPass) {
    DF_TRACEPOINT(SIL_TRACE_ERROR, "Unable to get C2R Table for DF\n");
    assert(Status == SilPass);
    Status = SilOutOfResources;
  } else if (DomainInfo == NULL) {
    DF_TRACEPOINT(SIL_TRACE_ERROR, "Error :BuildDomainInfo not found RC Memory in DomainXlat\n");
    assert(DomainInfo != NULL);
    Status = SilUnsupported; // Could not find the IP input block
  } else if (Socket >= DfXfer->DfGetNumberOfProcessorsPresent ()) {
    DF_TRACEPOINT(SIL_TRACE_ERROR, "Error: DomainXlat invalid Socket number %d\n", Socket);
    assert(Socket < DfXfer->DfGetNumberOfProcessorsPresent ());
    Status = SilInvalidParameter;
  } else if (Die >= DfGetNumberOfDiesOnSocket(Socket)) {
    DF_TRACEPOINT(SIL_TRACE_ERROR, "Error: DomainXlat invalid Die number %d\n", Socket);
    assert(Die < DfGetNumberOfDiesOnSocket(Socket));
    Status = SilInvalidParameter;
  } else if (Ccd >= DomainInfo->CcdCount[Socket]) {
    DF_TRACEPOINT(SIL_TRACE_ERROR, "Error: DomainXlat invalid Ccd number %d\n", Socket);
    assert(Ccd < DomainInfo->CcdCount[Socket]);
    Status = SilInvalidParameter;
  } else if (Ccx >= DomainInfo->CcxPerCcd[Socket]) {
    DF_TRACEPOINT(SIL_TRACE_ERROR, "Error: DomainXlat invalid Ccx number %d\n", Socket);
    assert(Ccx < DomainInfo->CcxPerCcd[Socket]);
    Status = SilInvalidParameter;
  } else if (Domain == NULL) {
    DF_TRACEPOINT(SIL_TRACE_ERROR, "Error: DomainXlat domain null pointer %d\n", Domain);
    assert(Domain != NULL);
    Status = SilInvalidParameter;
  } else if (DomainInfo->NumberOfReportedDomains >= MAX_REPORTED_DOMAINS) {
    // This assert prevents a tainted scalar coverity warning
    assert(DomainInfo->NumberOfReportedDomains < MAX_REPORTED_DOMAINS);
    Status = SilOutOfBounds;
  } else {
    for (i = 0; i < DomainInfo->NumberOfReportedDomains; i++) {
      if ((DomainInfo->ReportedDomainCcxMap[i] &
        ((1 << (uint32_t) Ccx) << (DomainInfo->LogToPhysCcd[Socket][Ccd]
        * CcxPerCcd) << ((uint32_t) Socket * NORMALIZED_SOCKET_SHIFT))) != 0) {
        break;
      }
    }

    if (i >= DomainInfo->NumberOfReportedDomains) {
      DF_TRACEPOINT(SIL_TRACE_ERROR, "Error: DomainXlat invalid domain value %d\n", i);
      assert(i < DomainInfo->NumberOfReportedDomains);
      Status = SilInvalidParameter;
    } else {
      Status = SilPass;
      *Domain = i;
    }

    DF_TRACEPOINT(SIL_TRACE_INFO, "DomainXlat Domain Value %d\n", i);
  }

  return Status;
}
