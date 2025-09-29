/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  DfAcpiDomainInfoPhx.c
 * @brief OpenSIL Fabric ACPI Domain information for SRAT, CRAT, SLIT, CDIT, and MSCT
 *
 */

#include <SilCommon.h>
#include <ApobCmn.h>
#include <DF/Df.h>
#include <DF/Common/SilBaseFabricTopologyLib.h>
#include <DF/Common/BaseFabricTopologyCmn.h>
#include <DF/Common/DfCmn2Rev.h>
#include <string.h>
#include <DfSilFabricInfoPhx.h>
#include <DF/DfX/PHX/DfSilFabricRegistersPhx.h>
#include <DF/DfX/DfXFabricRegisterAcc.h>
#include "DfPhx.h"
#include <DF/DfX/PHX/DfAcpiDomainInfoPhx.h>


/**
 * GetSocketMapFromNormalizedCsMapPhx
 *
 * @brief This function returns a bitmap of sockets belonging to the CS map
 *
 * @param  NormalizedCsMap     CS map to check
 *
 * @retval     uint32_t            Socket bit map
 */
static
uint32_t
GetSocketMapFromNormalizedCsMapPhx (
  uint32_t  NormalizedCsMap
  )
{
  uint32_t  i;
  uint32_t  SocketMap;

  SocketMap = 0;
  for (i = 0; i < PHX_MAX_SOCKETS; i++) {
    SocketMap |= ((NormalizedCsMap & (SOCKET_CS_MAP << ((uint32_t) i * NORMALIZED_SOCKET_SHIFT)))
      != 0) ? (1 << (uint32_t) i) : 0;
  }

  DF_TRACEPOINT(SIL_TRACE_INFO, "openSIL GetSocketMapFromNormalizedCsMapPhx SocketMap value %x\n", SocketMap);
  return SocketMap;

}

/**
 * DfInitMixedDimmConfigPhx
 *
 * @brief This function gathers data about CCDs to be fill the DomainInfo block.
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param
 *
 */
static
SIL_STATUS
DfInitMixedDimmConfigPhx (
  SIL_CONTEXT  *SilContext
  )
{
  uint32_t                     i;
  uint32_t                     j;
  uint32_t                     DiffIntlvLimitAddr = 0;
  DRAM_BASE_ADDRESS_REGISTER   DramBaseAddr[PHX_NUMBER_OF_DRAM_REGIONS];
  DRAM_LIMIT_ADDRESS_REGISTER  DramLimitAddr[PHX_NUMBER_OF_DRAM_REGIONS];
  SIL_RESERVED_UNION_0009   DramAddressCtl[PHX_NUMBER_OF_DRAM_REGIONS];
  DRAM_ADDRESS_INTLV_REGISTER  DramAddressIntlv[PHX_NUMBER_OF_DRAM_REGIONS];
  DF_PHX_DOMAIN_INFO_BLK       *DomainInfo;
  DFCLASS_INPUT_BLK            *DfIpBlock;

  DfIpBlock = (DFCLASS_INPUT_BLK *) xUslFindStructure(SilContext, SilId_DfClass, DFCLASS_INSTANCE);
  if (DfIpBlock == NULL) {
    DF_TRACEPOINT(SIL_TRACE_ERROR, "failed to find Df input data blk\n");
    return SilUnsupported;
  }

  DomainInfo = (DF_PHX_DOMAIN_INFO_BLK *) xUslFindStructure(SilContext, SilId_DfClass, DF_PHX_DOMAIN_INSTANCE);
  if (DomainInfo == NULL) {
    DF_TRACEPOINT(SIL_TRACE_ERROR, "failed to find Df domain info data blk\n");
    return SilUnsupported;
  }

  DomainInfo->IsMixedDimmConfig = false;
  if (DfIpBlock->AmdFabricMixedDimmConfigNumaDomain == false) {
    // Mixed DIMM config extended NUMA domain feature disabled
    DF_TRACEPOINT(SIL_TRACE_INFO, "Mixed DIMM config extended NUMA domain feature disabled.\n");
    return SilPass;
  }

  for (i = 0; i < PHX_NUMBER_OF_DRAM_REGIONS; i++) {
    DramBaseAddr[i].Value = DfXFabricRegisterAccRead(SilContext,
      0,
      0,
      DRAMBASEADDRESS_0_FUNC,
      (DFXFABx200 + (i * (DFXFABx210 - DFXFABx200))),
      PHX_IOM0_INSTANCE_ID
      );

    DramLimitAddr[i].Value = DfXFabricRegisterAccRead(SilContext,
      0,
      0,
      DRAMLIMITADDRESS_0_FUNC,
      (DFXFABx204 + (i * (DFXFABx214 - DFXFABx204))),
      PHX_IOM0_INSTANCE_ID
      );

    DramAddressCtl[i].Value = DfXFabricRegisterAccRead(SilContext,
      0,
      0,
      DRAMADDRESSCTL0_FUNC,
      (DFXFABx208 + (i * (DFXFABx218 - DFXFABx208))),
      PHX_IOM0_INSTANCE_ID
      );

    DramAddressIntlv[i].Value = DfXFabricRegisterAccRead(SilContext,
      0,
      0,
      DRAMADDRESSINTLV_0_FUNC,
      (DFXFABx20C + (i * (DFXFABx21C - DFXFABx20C))),
      PHX_IOM0_INSTANCE_ID
      );
  }

  for (i = 0; i < (PHX_NUMBER_OF_DRAM_REGIONS - 1); i++) {
    if (DramAddressCtl[i].Field.field_bit_0 == 1) {
      DiffIntlvLimitAddr = DramBaseAddr[i].Field.DramBaseAddr << 12;
      for (j = i + 1; j < PHX_NUMBER_OF_DRAM_REGIONS; j++) {
        if (DramAddressCtl[j].Field.field_bit_0 == 1) {
          if ((uint32_t)(DramBaseAddr[j].Field.DramBaseAddr << 12) > (uint32_t)(DiffIntlvLimitAddr)) {
            DiffIntlvLimitAddr = DramBaseAddr[j].Field.DramBaseAddr << 12;
          }
          if (DramAddressIntlv[i].Field.IntLvNumChan != DramAddressIntlv[j].Field.IntLvNumChan) {
            if (((DramAddressIntlv[i].Field.IntLvNumChan > DramAddressIntlv[j].Field.IntLvNumChan) && \
              (DramLimitAddr[j].Field.DramLimitAddr > DiffIntlvLimitAddr)) || \
              ((DramAddressIntlv[i].Field.IntLvNumChan < DramAddressIntlv[j].Field.IntLvNumChan) && \
              (DramLimitAddr[i].Field.DramLimitAddr > DiffIntlvLimitAddr))) {
              DomainInfo->IsLowIntlvAbove = true;
            }
            DomainInfo->IsMixedDimmConfig = true;
          }
        }
      }
      break;
    }
  }

  DF_TRACEPOINT(SIL_TRACE_INFO, "Mixed DIMM config extended NUMA domain feature enabled\n");
  DF_TRACEPOINT(SIL_TRACE_INFO, "IsMixedDimmConfig is %s\n", DomainInfo->IsMixedDimmConfig ? "true" : "false");

  return SilPass;
}

/**
 * DfBuildCcmInfoPhx
 *
 * @brief This function gathers data about CCMs to be used by the protocol procedures
 *
 * @param  SilContext           A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @param  SocketCount          Number of processors installed
 *
 */
static
SIL_STATUS
DfBuildCcmInfoPhx (
  SIL_CONTEXT       *SilContext,
  uint32_t          SocketCount
  )
{
  uint32_t                 i;
  uint32_t                 j;
  uint32_t                 m;
  uint32_t                 CcmMap;
  DF_PHX_DOMAIN_INFO_BLK   *DomainInfo;

  DomainInfo = (DF_PHX_DOMAIN_INFO_BLK *) xUslFindStructure(SilContext, SilId_DfClass, DF_PHX_DOMAIN_INSTANCE);
  if (DomainInfo == NULL) {
    DF_TRACEPOINT(SIL_TRACE_ERROR, "failed to find Df domain info data blk\n");
    return SilUnsupported;
  }

  for (i = 0; i < SocketCount; i++) {
    DomainInfo->CcxPerCcm[i] = 0x1;

    m = DomainInfo->CcmCount[i] = 0;
    CcmMap = 0x1;

    for (j = 0; CcmMap != 0; CcmMap &= ~(1 << j), j++) {
      if ((CcmMap & (1 << j)) != 0) {
        assert(j < PHX_NUM_CCM_BLOCKS);
        if (m < PHX_NUM_CCM_BLOCKS) {
          DomainInfo->LogToPhysCcm[i][m] = j;
          DomainInfo->CcmCount[i]++;
        }
      }
    }
    assert(DomainInfo->CcmCount[i] <= PHX_NUM_CCM_BLOCKS);
  }

  DF_TRACEPOINT(SIL_TRACE_INFO, "BuildCcmInfo Successful \n");
  return SilPass;
}

/**
 * DfBuildDomainInfoPhx
 *
 * @brief    This function gathers data about the NUMA domains
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 * @retval   SilPass               Successful
 * @retval   SilInvalidParameter   An error was encountered. Data is not valid
 */
SIL_STATUS
DfBuildDomainInfoPhx (
  SIL_CONTEXT  *SilContext
  )
{
  uint32_t                           Ccd;
  uint32_t                           Ccx;
  uint32_t                           Domain;
  uint32_t                           Socket;
  uint32_t                           NumberOfCpus;
  uint32_t                           i;
  uint32_t                           ReportedIndex;
  uint32_t                           NumberOfPhysicalDomains = 0;
  uint32_t                           NumberOfReportedDomains = 0;
  bool                               CcxAsNuma = false;
  uint32_t                           SLinkBase;
  uint32_t                           SetBitCount;
  DF_PHX_DOMAIN_INFO_BLK             *DomainInfo;
  DFCLASS_INPUT_BLK                  *DfIpBlock;
  DF_COMMON_2_REV_XFER_BLOCK *DfXfer;
  SIL_STATUS                         Status;
  uint32_t                           LSbitSet;
  uint32_t                           SocketShift;

  Status = SilGetCommon2RevXferTable(SilContext, SilId_DfClass, (void **) &DfXfer);
  assert(Status == SilPass);

  DfIpBlock = (DFCLASS_INPUT_BLK *) xUslFindStructure(SilContext, SilId_DfClass, DFCLASS_INSTANCE);
  if (DfIpBlock == NULL) {
    DF_TRACEPOINT(SIL_TRACE_ERROR, "failed to find Df input data blk\n");
    return SilUnsupported;
  }

  DomainInfo = (DF_PHX_DOMAIN_INFO_BLK *) xUslFindStructure(SilContext, SilId_DfClass, DF_PHX_DOMAIN_INSTANCE);
  if (DomainInfo == NULL) {
    DF_TRACEPOINT(SIL_TRACE_ERROR, "failed to find Df domain info data blk\n");
    return SilUnsupported;
  }

  DomainInfo->DomainInfoValid = false;

  NumberOfCpus = (uint32_t) DfXfer->DfGetNumberOfProcessorsPresent(SilContext);
  if (NumberOfCpus > PROJ_MAX_SOCKETS_SUPPORTED) {
    DF_TRACEPOINT(SIL_TRACE_INFO, "Error: NumberOfCpus unsupported value\n");
    return SilUnsupported;
  }
  CcxAsNuma = DfIpBlock->AmdFabricCcxAsNumaDomain;

  DfInitMixedDimmConfigPhx(SilContext);

  // Fill module global CCD data
  DfBuildCcmInfoPhx(SilContext, (uint32_t) NumberOfCpus);
  NumberOfPhysicalDomains = 0;

  for (Socket = 0; Socket < NumberOfCpus; Socket++) {
    DomainInfo->PhysicalDomainInfo[NumberOfPhysicalDomains++].NormalizedCsMap
      = (((1 << PHX_NUM_CS_UMC_BLOCKS) - 1) << ((uint32_t) Socket * NORMALIZED_SOCKET_SHIFT));
  }

  DomainInfo->PhysNps = 1;
  DomainInfo->SystemSLinkCount = 0;

  if (CcxAsNuma) {
    NumberOfReportedDomains = DomainInfo->SystemSLinkCount;
    for (Socket = 0; Socket < NumberOfCpus; Socket++) {
      NumberOfReportedDomains += ((DomainInfo->CcmCount[Socket]) * (DomainInfo->CcxPerCcm[Socket]));
    }
  } else {
    NumberOfReportedDomains = NumberOfPhysicalDomains;
  }

  memset((void *) DomainInfo->ReportedDomainCcxMap, 0x00, sizeof (DomainInfo->ReportedDomainCcxMap));

  if (CcxAsNuma) {
    for (Domain = 0; Domain < MAX_PHYSICAL_DOMAINS; Domain++) {
      DomainInfo->PhysicalDomainInfo[Domain].SharingEntityCount = 0;
      DomainInfo->PhysicalDomainInfo[Domain].SharingEntityMap = 0;
    }
    // Start with non S-Link
    ReportedIndex = 0;
    if (DomainInfo->CcmCount[Socket] <= PROJ_MAX_SOCKETS_SUPPORTED) {
      for (Socket = 0; Socket < NumberOfCpus; Socket++) {
        for (Ccd = 0; Ccd < DomainInfo->CcmCount[Socket]; Ccd++) {
          if (DomainInfo->CcxPerCcm[Socket] <= MAX_CCX_PER_CCM) {
            for (Ccx = 0; Ccx < DomainInfo->CcxPerCcm[Socket]; Ccx++) {
              DomainInfo->ReportedDomainInfo[ReportedIndex].Type = NumaDram;
              DomainInfo->ReportedDomainInfo[ReportedIndex].PhysicalDomain =
                DfXfer->DfGetPhysDomainOfCcd((uint32_t) Socket,
                DomainInfo->LogToPhysCcd[Socket][Ccd],
                NumberOfPhysicalDomains,
                DomainInfo
                );
              DomainInfo->ReportedDomainInfo[ReportedIndex].SocketMap = GetSocketMapFromNormalizedCsMapPhx
                  (DomainInfo->PhysicalDomainInfo[DomainInfo->ReportedDomainInfo[ReportedIndex].
                  PhysicalDomain].NormalizedCsMap
                  );
              DomainInfo->PhysicalDomainInfo[DomainInfo->ReportedDomainInfo[ReportedIndex].
                PhysicalDomain].SharingEntityCount++;
              DomainInfo->PhysicalDomainInfo[DomainInfo->ReportedDomainInfo[ReportedIndex].
                PhysicalDomain].SharingEntityMap |= (1 << ReportedIndex);
              DomainInfo->ReportedDomainCcxMap[ReportedIndex] = (1 << (uint32_t) Ccx) <<
                (DomainInfo->LogToPhysCcm[Socket][Ccd] * MAX_CCX_PER_CCM) <<
                ((uint32_t) Socket * NORMALIZED_SOCKET_SHIFT);
              ReportedIndex++;
            }
          }
        }
      }
    }

    SLinkBase = NumberOfPhysicalDomains - DomainInfo->SystemSLinkCount;
    for (i = 0; i < DomainInfo->SystemSLinkCount; i++) {
      DomainInfo->ReportedDomainInfo[ReportedIndex].Type = NumaSLink;
      DomainInfo->ReportedDomainInfo[ReportedIndex].SocketMap =
        GetSocketMapFromNormalizedCsMapPhx(DomainInfo->PhysicalDomainInfo
        [i + SLinkBase].NormalizedCsMap
        );
      DomainInfo->ReportedDomainInfo[ReportedIndex].PhysicalDomain = i + SLinkBase;
      DomainInfo->PhysicalDomainInfo[DomainInfo->ReportedDomainInfo
        [ReportedIndex].PhysicalDomain].SharingEntityCount = 1;
      DomainInfo->PhysicalDomainInfo[DomainInfo->ReportedDomainInfo
        [ReportedIndex].PhysicalDomain].SharingEntityMap = (1 << ReportedIndex);
      ReportedIndex++;
    }
    assert(ReportedIndex == NumberOfReportedDomains);
  } else {
    for (Domain = 0; Domain < NumberOfPhysicalDomains; Domain++) {
#if (CS_CCIX_SYSTEM_MAP == 0)
      DomainInfo->ReportedDomainInfo[Domain].Type = NumaDram;
#else
      DomainInfo->ReportedDomainInfo[Domain].Type =
        ((DomainInfo->PhysicalDomainInfo[Domain].NormalizedCsMap
        & CS_CCIX_SYSTEM_MAP) != 0) ? NumaSLink : NumaDram;
#endif
      DomainInfo->ReportedDomainInfo[Domain].SocketMap =
        GetSocketMapFromNormalizedCsMapPhx(DomainInfo->PhysicalDomainInfo[Domain]
        .NormalizedCsMap
        );
      DomainInfo->ReportedDomainInfo[Domain].PhysicalDomain = (uint32_t) Domain;
      DomainInfo->PhysicalDomainInfo[Domain].SharingEntityCount = 1;
      DomainInfo->PhysicalDomainInfo[Domain].SharingEntityMap = (1 << (uint32_t) Domain);
      if (DomainInfo->ReportedDomainInfo[Domain].Type == NumaDram) {
        SetBitCount = xUslGetSetBitCount(DomainInfo->PhysicalDomainInfo
          [Domain].NormalizedCsMap
          );
        if (SetBitCount == 16) {
          DomainInfo->ReportedDomainCcxMap[Domain] = 0xFFFFFFFF;
        } else {
          LSbitSet = (SetBitCount != 0) ? (uint32_t) xUslLowBitSet32
              (DomainInfo->PhysicalDomainInfo[Domain].NormalizedCsMap) : 0;
          if (LSbitSet >= 16) {
            SocketShift = 16;
            LSbitSet -= 16;
          } else {
            SocketShift = 0;
          }
          DomainInfo->ReportedDomainCcxMap[Domain] = ((1 << (SetBitCount << 1))
            - 1) << (LSbitSet << 1) << SocketShift;
        }
      }
    }
  }

  DomainInfo->DomainInfoValid = true;
  DomainInfo->NumberOfReportedDomains = NumberOfReportedDomains;

  DF_TRACEPOINT(SIL_TRACE_INFO, "openSIL NumberOfReportedDomains %x\n", NumberOfReportedDomains);
  DF_TRACEPOINT(SIL_TRACE_INFO, "Updated BuildDomainInfo\n");

  return SilPass;
}

/**
 * DfDomainXlatPhx
 *
 * @brief This function translates a core's physical location to the appropriate NUMA domain.
 *
 * @param   SilContext            A context structure through which host firmware defined data
 *                                can be passed to openSIL. The host firmware is responsible
 *                                for initializing the SIL_CONTEXT structure.
 * @param   Socket                Zero based socket that the core is attached to
 * @param   Die                   DF die on socket that the core is attached to
 * @param   Ccd                   Logical CCD the core is on
 * @param   Ccx                   Logical core complex
 * @param   Domain                Domain the core belongs to
 *
 * @retval      SilPass               Domain is valid
 * @retval      SilInvalidParameter   No core at location
 */
SIL_STATUS
DfDomainXlatPhx (
  SIL_CONTEXT  *SilContext,
  uint32_t     Socket,
  uint32_t     Die,
  uint32_t     Ccd,
  uint32_t     Ccx,
  uint32_t     *Domain
  )
{
  DF_PHX_DOMAIN_INFO_BLK *DomainInfo;
  uint32_t                           i;
  DF_COMMON_2_REV_XFER_BLOCK *DfXfer;
  SIL_STATUS                         Status;

  Status = SilGetCommon2RevXferTable(SilContext, SilId_DfClass, (void **) &DfXfer);
  DomainInfo = (DF_PHX_DOMAIN_INFO_BLK *)xUslFindStructure(SilContext, SilId_DfClass, DF_PHX_DOMAIN_INSTANCE);

  if (Status != SilPass) {
    DF_TRACEPOINT(SIL_TRACE_ERROR, "Unable to get C2R Table for DF\n");
    assert(Status == SilPass);
    Status = SilOutOfResources;
  } else if (DomainInfo == NULL) {
    DF_TRACEPOINT(SIL_TRACE_INFO, "Error :BuildDomainInfo not found RC Memory in DomainXlat\n");
    assert(DomainInfo != NULL);
    Status = SilUnsupported; // Could not find the IP data block
  } else if (Socket >= DfXfer->DfGetNumberOfProcessorsPresent(SilContext)) {
    DF_TRACEPOINT(SIL_TRACE_INFO, "Error: DomainXlat invalid Socket number %d\n", Socket);
    Status = SilInvalidParameter;
  } else if (Die >= DfGetNumberOfDiesOnSocket(SilContext, Socket)) {
    DF_TRACEPOINT(SIL_TRACE_INFO, "Error: DomainXlat invalid Die number %d\n", Socket);
    Status = SilInvalidParameter;
  } else if (Ccd >= DomainInfo->CcmCount[Socket]) {
    DF_TRACEPOINT(SIL_TRACE_INFO, "Error: DomainXlat invalid Ccd number %d\n", Socket);
    Status = SilInvalidParameter;
  } else if (Ccx >= DomainInfo->CcxPerCcm[Socket]) {
    DF_TRACEPOINT(SIL_TRACE_INFO, "Error: DomainXlat invalid Ccx number %d\n", Socket);
    Status = SilInvalidParameter;
  } else if (Domain == NULL) {
    DF_TRACEPOINT(SIL_TRACE_INFO, "Error: DomainXlat domain null pointer %d\n", Domain);
    Status = SilInvalidParameter;
  } else if (DomainInfo->NumberOfReportedDomains >= MAX_REPORTED_DOMAINS) {
    assert(DomainInfo->NumberOfReportedDomains < MAX_REPORTED_DOMAINS);
    Status = SilOutOfBounds;
  } else {
    for (i = 0; i < DomainInfo->NumberOfReportedDomains; i++) {
      if ((DomainInfo->ReportedDomainCcxMap[i] &
        ((1 << (uint32_t) Ccx) << (DomainInfo->LogToPhysCcm[Socket][Ccd] * MAX_CCX_PER_CCM)
                               << ((uint32_t) Socket * NORMALIZED_SOCKET_SHIFT))) != 0) {
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
      DF_TRACEPOINT(SIL_TRACE_INFO, "DomainXlat Domain Value %d\n", i);
    }
  }

  return Status;
}
