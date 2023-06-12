/**
 * @file  FabricAcpiDomainInfo.c
 * @brief OpenSIL Fabric ACPI Domain information for SRAT, CRAT, SLIT, CDIT, and MSCT.
 *
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include <ApobCmn.h>
#include <DF/Df.h>
#include <DF/Common/SilBaseFabricTopologyLib.h>
#include <DF/Common/SilFabricInfo.h>
#include <DF/Common/BaseFabricTopologyCmn.h>
#include <DF/DfX/DfXBaseFabricTopology.h>
#include <DF/DfX/SilFabricRegistersDfX.h>
#include <DF/DfX/DfXFabricRegisterAcc.h>
#include <DF/DfX/FabricAcpiDomain/FabricAcpiDomainInfo.h>
#include <RcMgr/DfX/RcManager4-api.h>
#include <string.h>

uint32_t   NumberOfPhysicalDomains  = 0;
uint32_t   PhysNps                  = 0;
uint32_t   SystemCxlCount           = 0;
uint32_t   NumberOfReportedDomains  = 0;
bool       CcxAsNuma                = false;
uint32_t   MaxCcxPerCcd             = 0;

DFX_RCMGR_DATA_BLK *FabricRcMgrData;

/**
 * GetPhysDomainOfCcd
 *
 * @brief This function returns the physical domain that the given CCD belongs to
 *
 * @param[in]  Socket      Zero based socket number
 * @param[in]  PhysCcd     Physical CCD number
 *
 * @retval     uint32_t    Physical domain number
 */

static uint32_t
GetPhysDomainOfCcd (
  uint32_t  Socket,
  uint32_t  PhysCcd
  )
{
  uint32_t  PhysicalDomainOfCcd;
  uint32_t  CcdToCsMap[] = {0, 9, 3, 6, 1, 10, 4, 7, 2, 11, 5, 8};

  if (PhysCcd >= sizeof (CcdToCsMap) / sizeof (CcdToCsMap[0])) {
    assert (false);
    return 0xFF;
  }

  for (PhysicalDomainOfCcd = 0; PhysicalDomainOfCcd < NumberOfPhysicalDomains;
       PhysicalDomainOfCcd++) {
    if ((FabricRcMgrData->DFXRcmgrOutputBlock.PhysicalDomainInfo[PhysicalDomainOfCcd].
        NormalizedCsMap & ((1 << PhysCcd) << (Socket * NORMALIZED_SOCKET_SHIFT))) != 0) {
      break;
    }
  }

  DF_TRACEPOINT(SIL_TRACE_INFO, "GetPhysDomainOfCcd Value %d,%d\n",PhysicalDomainOfCcd,NumberOfPhysicalDomains);

  assert (PhysicalDomainOfCcd < NumberOfPhysicalDomains);
  return PhysicalDomainOfCcd;
}

/**
 * GetSocketMapFromNormalizedCsMap
 *
 * @brief This function returns a bitmap of sockets belonging to the CS map
 *
 * @param[in]  NormalizedCsMap     CS map to check
 *
 * @retval     uint32_t            Socket bit map
 */
static uint32_t
GetSocketMapFromNormalizedCsMap (
  uint32_t  NormalizedCsMap
  )
{
  uint32_t   i;
  uint32_t  SocketMap;

  SocketMap = 0;
  for (i = 0; i < MAX_SOCKETS_SUPPORTED; i++) {
    SocketMap |= ((NormalizedCsMap & (SOCKET_CS_MAP << ((uint32_t) i * NORMALIZED_SOCKET_SHIFT)))
                  != 0) ? (1 << (uint32_t) i) : 0;
  }

  DF_TRACEPOINT(SIL_TRACE_INFO, "GetSocketMapFromNormalizedCsMap SocketMap value %d\n",SocketMap);
  return SocketMap;
}

/**
 * BuildCcdInfo
 *
 * @brief This function gathers data about CCDs to be used by the protocol procedures
 *
 * @param[in]  SocketCount   Number of processors installed
 */
static void
BuildCcdInfo (
  uint32_t  SocketCount
  )
{
  uint32_t             i;
  uint32_t             j;
  uint32_t             CcdMap;
  CCD_ENABLE_REGISTER  CcdEnable;
  CCM_CONFIG4_REGISTER CcmConfig4;

  for (i = 0; i < SocketCount; i++) {
    CcdMap = 0;
    FabricRcMgrData->DFXRcmgrOutputBlock.CcxPerCcd[i] = 1;
    for (j = 0; j < DFX_NUM_CCM_BLOCKS; j++) {
      CcmConfig4.Value = DfXFabricRegisterAccRead (i,CCMCONFIG4_FUNC, CCMCONFIG4_REG, DFX_CCM0_INSTANCE_ID + j);
      CcdEnable.Value = DfXFabricRegisterAccRead (i,CCDENABLE_FUNC, CCDENABLE_REG, DFX_CCM0_INSTANCE_ID + j);
      CcdMap |= (CcdEnable.Field.CCDEn & 1) << j;
      if (CcmConfig4.Field.SdpWideEn == 0) {
        // CCM is not in the wide mode, the second SDP port is for the second CCD
        CcdMap |= ((CcdEnable.Field.CCDEn >> 1) & 1) << DFX_NUM_CCM_BLOCKS << j;
      }
      if (CcdEnable.Field.CCXEn == 3) {
        FabricRcMgrData->DFXRcmgrOutputBlock.CcxPerCcd[i] = 2;
      }
    }
    FabricRcMgrData->DFXRcmgrOutputBlock.CcdCount[i] = 0;
    for (j = 0; CcdMap != 0; CcdMap &= ~(1 << j), j++) {
      if ((CcdMap & (1 << j)) != 0) {
        assert (j < DFX_MAX_CCD_PER_SOCKET);
        FabricRcMgrData->DFXRcmgrOutputBlock.LogToPhysCcd[i][FabricRcMgrData->DFXRcmgrOutputBlock.CcdCount[i]++] = j;
      }
    }
    assert (FabricRcMgrData->DFXRcmgrOutputBlock.CcdCount[i] <= DFX_MAX_CCD_PER_SOCKET);
  }

  DF_TRACEPOINT(SIL_TRACE_INFO, "BuildCcdInfo Successfull \n");
}

/**
 * BuildCxlInfo
 *
 * @brief This function gathers data about CCMs to be used by the protocol procedures
 *
 * @retval   uint32_t   Number of Cxls populated with memory
 */
static uint32_t
BuildCxlInfo (
  uint32_t Index
  )
{
  uint32_t                               i;
  uint32_t                               ActiveCxlCount;
  APOB_SYSTEM_CXL_INFO_TYPE_STRUCT      *CxlMap;
  bool                                  CxlDeviceAttached[MAX_SOCKETS_SUPPORTED];

  ActiveCxlCount = 0;
    for (i = 0; i < MAX_SOCKETS_SUPPORTED; i++) {
    CxlDeviceAttached[i] = false;
  }

  if ((AmdGetApobEntryInstance (APOB_FABRIC, APOB_SYS_CXL_INFO_TYPE, 0, 0,
      (APOB_TYPE_HEADER **)(void*) &CxlMap)) == SilPass) {
    for (i = 0; i < (sizeof (CxlMap->CxlInfo) / sizeof(CxlMap->CxlInfo[0])); i++) {
      if(CxlMap->CxlInfo[i].Size != 0 && CxlMap->CxlInfo[i].Status == CXL_ADDR_SUCCESS) {
        if (CxlMap->CxlInfo[i].Socket < MAX_SOCKETS_SUPPORTED) {
          CxlDeviceAttached[CxlMap->CxlInfo[i].Socket] = true;
        }
      }
    }
  }
  for (i = 0; i < MAX_SOCKETS_SUPPORTED; i++) {
    if (CxlDeviceAttached[i]) {
      // Build the CS map
      FabricRcMgrData->DFXRcmgrOutputBlock.PhysicalDomainInfo[(Index + ActiveCxlCount)].NormalizedCsMap
        = 0xF000 << (i * NORMALIZED_SOCKET_SHIFT);
      ActiveCxlCount++;
    }
  }

  DF_TRACEPOINT(SIL_TRACE_INFO, "BuildCxlInfo Successfull %d\n",ActiveCxlCount);

  return ActiveCxlCount;
}

/**
 * DfXBuildDomainInfo
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
  )
{
  uint32_t                           Ccd;
  uint32_t                           Ccx;
  uint32_t                           Domain;
  uint32_t                           Half;
  uint32_t                           Quad;
  uint32_t                           Socket;
  uint32_t                           NumberOfCpus;
  APOB_SYSTEM_NPS_INFO_TYPE_STRUCT   *NpsInfo;
  uint32_t                           i;
  uint32_t                           ReportedIndex;
  uint32_t                           CxlBase;
  uint32_t                           Nps0CcxMap       = 0x0FFF0FFF;
  uint32_t                           Nps1CcxMap       = 0x0FFF;
  uint32_t                           Nps2CcxMap[]     = {0x0555, 0x0AAA};
  uint32_t                           Nps4CcxMap[]     = {0x0111, 0x0444, 0x0888, 0x0222};

  FabricRcMgrData = (DFX_RCMGR_DATA_BLK *)SilFindStructure(SilId_RcManager,  0);
  if (FabricRcMgrData == NULL) {
    DF_TRACEPOINT(SIL_TRACE_INFO, "Error: DfXBuildDomainInfo failed to find RCMGR data blk\n");
    return SilUnsupported; // Could not find the IP input block
  }
  FabricRcMgrData->DFXRcmgrOutputBlock.DomainInfoValid = false;

  if ((AmdGetApobEntryInstance(APOB_FABRIC, APOB_SYS_NPS_INFO_TYPE, 0, 0,
      (APOB_TYPE_HEADER **)(void*) &NpsInfo)) != SilPass) {
    DF_TRACEPOINT(SIL_TRACE_INFO, "Error: DfXBuildDomainInfo GetApobEntryInstance failed\n");
    return SilUnsupported;
  }

  NumberOfCpus = (uint32_t)DfXGetNumberOfProcessorsPresent();
  CcxAsNuma = FabricRcMgrData->DFXRcmgrOutputBlock.AmdFabricCcxAsNumaDomain;

    // Fill module global CCD data
  BuildCcdInfo ((uint32_t) NumberOfCpus);
  NumberOfPhysicalDomains = 0;
  MaxCcxPerCcd = MAX_CCX_PER_CCD;

  memset((void *) FabricRcMgrData->DFXRcmgrOutputBlock.ReportedDomainCcxMap,
         0x00, sizeof (FabricRcMgrData->DFXRcmgrOutputBlock.ReportedDomainCcxMap));

  switch (NpsInfo->ActualNps) {
    case DF_DRAM_NPS0:
      assert (NumberOfCpus == 2);
      FabricRcMgrData->DFXRcmgrOutputBlock.PhysicalDomainInfo[NumberOfPhysicalDomains].NormalizedCsMap
       = ((1 << DFX_NUM_CS_UMC_BLOCKS) - 1) | (((1 <<DFX_NUM_CS_UMC_BLOCKS) - 1) << NORMALIZED_SOCKET_SHIFT);
      PhysNps = 0;
    if (CcxAsNuma == false) {
      FabricRcMgrData->DFXRcmgrOutputBlock.ReportedDomainCcxMap[NumberOfPhysicalDomains] = Nps0CcxMap;
    }
    NumberOfPhysicalDomains++;
    break;
    case DF_DRAM_NPS1:
      for (Socket = 0; Socket < NumberOfCpus; Socket++) {
        FabricRcMgrData->DFXRcmgrOutputBlock.PhysicalDomainInfo[NumberOfPhysicalDomains].NormalizedCsMap
         = (((1 << DFX_NUM_CS_UMC_BLOCKS) - 1) << ((uint32_t) Socket * NORMALIZED_SOCKET_SHIFT));

        if (CcxAsNuma == false) {
          FabricRcMgrData->DFXRcmgrOutputBlock.ReportedDomainCcxMap[NumberOfPhysicalDomains] =
           Nps1CcxMap << ((uint32_t) Socket * NORMALIZED_SOCKET_SHIFT);
      }
      NumberOfPhysicalDomains++;
    }
    PhysNps = 1;
    break;
    case DF_DRAM_NPS2:
      for (Socket = 0; Socket < NumberOfCpus; Socket++) {
        for (Half = 0; Half < 2; Half++) {
          FabricRcMgrData->DFXRcmgrOutputBlock.PhysicalDomainInfo[NumberOfPhysicalDomains].NormalizedCsMap =
            ((((1 << (DFX_NUM_CS_UMC_BLOCKS / 2)) - 1) << ((uint32_t) Half * (DFX_NUM_CS_UMC_BLOCKS / 2)))
            << ((uint32_t) Socket * NORMALIZED_SOCKET_SHIFT));
          if (CcxAsNuma == false) {
            FabricRcMgrData->DFXRcmgrOutputBlock.ReportedDomainCcxMap[NumberOfPhysicalDomains] =
             Nps2CcxMap[Half] << ((uint32_t) Socket * NORMALIZED_SOCKET_SHIFT);
          }
          NumberOfPhysicalDomains++;
        }
      }
    PhysNps = 2;
    break;
    case DF_DRAM_NPS4:
      for (Socket = 0; Socket < NumberOfCpus; Socket++) {
        for (Quad = 0; Quad < 4; Quad++) {
          FabricRcMgrData->DFXRcmgrOutputBlock.PhysicalDomainInfo[NumberOfPhysicalDomains++].NormalizedCsMap
           = ((((1 << (DFX_NUM_CS_UMC_BLOCKS / 4)) - 1) << ((uint32_t) Quad * (DFX_NUM_CS_UMC_BLOCKS / 4)))
              << ((uint32_t) Socket * NORMALIZED_SOCKET_SHIFT));
          if (CcxAsNuma == false) {
            FabricRcMgrData->DFXRcmgrOutputBlock.ReportedDomainCcxMap[NumberOfPhysicalDomains] =
             Nps4CcxMap[Quad] << ((uint32_t) Socket * NORMALIZED_SOCKET_SHIFT);
          }
          NumberOfPhysicalDomains++;
        }
      }
    PhysNps = 4;
    break;
    default:
      DF_TRACEPOINT(SIL_TRACE_INFO, "Error: DfXBuildDomainInfo ActualNps value: %d\n",NpsInfo->ActualNps);
      assert (false);
      return SilUnsupported;
    break;
  }

  SystemCxlCount = BuildCxlInfo (NumberOfPhysicalDomains);
  NumberOfPhysicalDomains += SystemCxlCount;

  if (CcxAsNuma) {
    NumberOfReportedDomains = 0;
    for (Socket = 0; Socket < NumberOfCpus; Socket++) {
      NumberOfReportedDomains += (FabricRcMgrData->DFXRcmgrOutputBlock.CcdCount[Socket]
                                   * FabricRcMgrData->DFXRcmgrOutputBlock.CcxPerCcd[Socket]);
    }
  }
  else {
    NumberOfReportedDomains = NumberOfPhysicalDomains;
  }

   assert (NumberOfReportedDomains <= MAX_REPORTED_DOMAINS);
  // Build reported domain info
  if (CcxAsNuma) {
    for (Domain = 0; Domain < MAX_PHYSICAL_DOMAINS; Domain++) {
      FabricRcMgrData->DFXRcmgrOutputBlock.PhysicalDomainInfo[Domain].SharingEntityCount = 0;
      FabricRcMgrData->DFXRcmgrOutputBlock.PhysicalDomainInfo[Domain].SharingEntityMap   = 0;
    }

  // Start with non CXL
  ReportedIndex = 0;
  for (Socket = 0; Socket < NumberOfCpus; Socket++) {
    for (Ccd = 0; Ccd < FabricRcMgrData->DFXRcmgrOutputBlock.CcdCount[Socket]; Ccd++) {
      for (Ccx = 0; Ccx < FabricRcMgrData->DFXRcmgrOutputBlock.CcxPerCcd[Socket]; Ccx++) {
          FabricRcMgrData->DFXRcmgrOutputBlock.ReportedDomainInfo[ReportedIndex].Type = NumaDram;
          FabricRcMgrData->DFXRcmgrOutputBlock.ReportedDomainInfo[ReportedIndex].PhysicalDomain
            = GetPhysDomainOfCcd ((uint32_t) Socket, FabricRcMgrData->DFXRcmgrOutputBlock.LogToPhysCcd[Socket][Ccd]);
          FabricRcMgrData->DFXRcmgrOutputBlock.ReportedDomainInfo[ReportedIndex].SocketMap
            = GetSocketMapFromNormalizedCsMap (FabricRcMgrData->DFXRcmgrOutputBlock.PhysicalDomainInfo
            [FabricRcMgrData->DFXRcmgrOutputBlock.ReportedDomainInfo[ReportedIndex].PhysicalDomain].NormalizedCsMap);
          FabricRcMgrData->DFXRcmgrOutputBlock.PhysicalDomainInfo[FabricRcMgrData->DFXRcmgrOutputBlock.
            ReportedDomainInfo[ReportedIndex].PhysicalDomain].SharingEntityCount++;
          FabricRcMgrData->DFXRcmgrOutputBlock.PhysicalDomainInfo[FabricRcMgrData->DFXRcmgrOutputBlock.
            ReportedDomainInfo[ReportedIndex].PhysicalDomain].SharingEntityMap |= (1 << ReportedIndex);
          ReportedIndex++;
      }
    }
  }

    CxlBase = NumberOfPhysicalDomains - SystemCxlCount;
    for (i = 0; i < SystemCxlCount; i++) {
      FabricRcMgrData->DFXRcmgrOutputBlock.ReportedDomainInfo[ReportedIndex].Type = NumaCxl;
      FabricRcMgrData->DFXRcmgrOutputBlock.ReportedDomainInfo[ReportedIndex].SocketMap
        = GetSocketMapFromNormalizedCsMap (FabricRcMgrData->DFXRcmgrOutputBlock.
                    PhysicalDomainInfo[i + CxlBase].NormalizedCsMap);
      FabricRcMgrData->DFXRcmgrOutputBlock.ReportedDomainInfo[ReportedIndex].PhysicalDomain = i + CxlBase;
      FabricRcMgrData->DFXRcmgrOutputBlock.PhysicalDomainInfo[FabricRcMgrData->DFXRcmgrOutputBlock.
        ReportedDomainInfo[ReportedIndex].PhysicalDomain].SharingEntityCount = 1;
      FabricRcMgrData->DFXRcmgrOutputBlock.PhysicalDomainInfo[FabricRcMgrData->DFXRcmgrOutputBlock.
        ReportedDomainInfo[ReportedIndex].PhysicalDomain].SharingEntityMap = (1 << ReportedIndex);
      ReportedIndex++;
    }
    assert (ReportedIndex == NumberOfReportedDomains);
  } else {
    for (Domain = 0; Domain < NumberOfPhysicalDomains; Domain++) {
      FabricRcMgrData->DFXRcmgrOutputBlock.ReportedDomainInfo[Domain].Type
        = ((FabricRcMgrData->DFXRcmgrOutputBlock.PhysicalDomainInfo[Domain].NormalizedCsMap & CS_CMP_SYSTEM_MAP) != 0)
        ? NumaCxl : NumaDram;
      FabricRcMgrData->DFXRcmgrOutputBlock.ReportedDomainInfo[Domain].SocketMap
        = GetSocketMapFromNormalizedCsMap (FabricRcMgrData->DFXRcmgrOutputBlock.
                                           PhysicalDomainInfo[Domain].NormalizedCsMap);
      FabricRcMgrData->DFXRcmgrOutputBlock.ReportedDomainInfo[Domain].PhysicalDomain
        = (uint32_t) Domain;
      FabricRcMgrData->DFXRcmgrOutputBlock.PhysicalDomainInfo[Domain].SharingEntityCount
        = 1;
      FabricRcMgrData->DFXRcmgrOutputBlock.PhysicalDomainInfo[Domain].SharingEntityMap
        = (1 << (uint32_t) Domain);
    }
  }

  FabricRcMgrData->DFXRcmgrOutputBlock.DomainInfoValid = true;
  FabricRcMgrData->DFXRcmgrOutputBlock.NumberOfReportedDomains = NumberOfReportedDomains;

  DF_TRACEPOINT(SIL_TRACE_INFO, "Updated BuildDomainInfo\n");
  return SilPass;
}

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
  )
{
  uint32_t  i;

  FabricRcMgrData = (DFX_RCMGR_DATA_BLK *)SilFindStructure(SilId_RcManager,  0);
  if (FabricRcMgrData == NULL) {
    DF_TRACEPOINT(SIL_TRACE_INFO, "Error :BuildDomainInfo not found RC Memory in DomainXlat\n");
    return SilUnsupported; // Could not find the IP input block
  }
  assert(FabricRcMgrData->DFXRcmgrOutputBlock.DomainInfoValid);

  if (Socket >= DfXGetNumberOfProcessorsPresent ()) {
    DF_TRACEPOINT(SIL_TRACE_INFO, "Error :DomainXlat Invalid Socket number %d\n",Socket);
    return SilInvalidParameter;
  }

  if (Die >= DfGetNumberOfDiesOnSocket ()) {
    DF_TRACEPOINT(SIL_TRACE_INFO, "Error :DomainXlat Invalid Die number %d\n",Socket);
    return SilInvalidParameter;
  }

  if (Ccd >= FabricRcMgrData->DFXRcmgrOutputBlock.CcdCount[Socket]) {
    DF_TRACEPOINT(SIL_TRACE_INFO, "Error :DomainXlat Invalid Ccd number %d\n",Socket);
    return SilInvalidParameter;
  }

  if (Ccx >= FabricRcMgrData->DFXRcmgrOutputBlock.CcxPerCcd[Socket]) {
    DF_TRACEPOINT(SIL_TRACE_INFO, "Error :DomainXlat Invalid Ccx number %d\n",Socket);
    return SilInvalidParameter;
  }

  if (Domain == NULL) {
    DF_TRACEPOINT(SIL_TRACE_INFO, "Error :DomainXlat Domain null pointer %d\n",Domain);
    return SilInvalidParameter;
  }

  // This assert prevents a tainted scalar coverity warning
  assert (FabricRcMgrData->DFXRcmgrOutputBlock.NumberOfReportedDomains < MAX_REPORTED_DOMAINS);
  for (i = 0; i < FabricRcMgrData->DFXRcmgrOutputBlock.NumberOfReportedDomains; i++) {
    if ((FabricRcMgrData->DFXRcmgrOutputBlock.ReportedDomainCcxMap[i] &
        ((1 << (uint32_t) Ccx) << (FabricRcMgrData->DFXRcmgrOutputBlock.LogToPhysCcd[Socket][Ccd]
        * MaxCcxPerCcd)<< ((uint32_t) Socket * NORMALIZED_SOCKET_SHIFT))) != 0) {
      break;
    }
  }

  if (i >= FabricRcMgrData->DFXRcmgrOutputBlock.NumberOfReportedDomains) {
    DF_TRACEPOINT(SIL_TRACE_INFO, "Error :DomainXlat invalid Domain value %d\n",i);
    assert (false);
    return SilInvalidParameter;
  }

  *Domain = i;
  DF_TRACEPOINT(SIL_TRACE_INFO, "DomainXlat Domain Value %d\n",i);
  return SilPass;
}
