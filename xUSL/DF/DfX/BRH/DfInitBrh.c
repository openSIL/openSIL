/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  DfInitBrh.c
 * @brief Various functions and data for data fabric initialization on BRH
 */

#include <SilCommon.h>
#include <ProjSocConst.h>
#include <DF/Df.h>
#include <DF/DfX/DfX.h>
#include <DF/Common/DfCmn2Rev.h>
#include <DF/DfX/DfXFabricRegisterAcc.h>
#include <DF/DfX/DfXAcpiDomainInfo.h>
#include "DfInitBrh.h"
#include "BrhAbstractRegAcc.h"
#include "SilFabricRegistersBrh.h"
#include "SilFabricInfoBrh.h"
#include <APOB/ApobIp2Ip.h>
#include <APOB/Common/ApobCmn.h>

const uint32_t BrhDfGlblCtrlInstanceIds[] = {
  SIL_RESERVED_0164,
  SIL_RESERVED_0167,
  SIL_RESERVED_0168,
  SIL_RESERVED_0169,
  SIL_RESERVED_0170,
  SIL_RESERVED_0171,
  SIL_RESERVED_0172,
  SIL_RESERVED_0173,
  SIL_RESERVED_0174,
  SIL_RESERVED_0175,
  SIL_RESERVED_0165,
  SIL_RESERVED_0166,
  SIL_RESERVED_0160,
  SIL_RESERVED_0161,
  SIL_RESERVED_0162,
  SIL_RESERVED_0163,
  SIL_RESERVED_0132,
  SIL_RESERVED_0133,
  SIL_RESERVED_0134,
  SIL_RESERVED_0135,
  SIL_RESERVED_0136,
  SIL_RESERVED_0137,
  SIL_RESERVED_0138,
  SIL_RESERVED_0139,
  SIL_RESERVED_0121,
  SIL_RESERVED_0122,
  SIL_RESERVED_0123,
  SIL_RESERVED_0124,
  SIL_RESERVED_0199,
  SIL_RESERVED_0200,
  SIL_RESERVED_0201,
  SIL_RESERVED_0202,
  SIL_RESERVED_0183,
  SIL_RESERVED_0184,
  SIL_RESERVED_0185,
  SIL_RESERVED_0186,
  SIL_RESERVED_0187,
  SIL_RESERVED_0188,
  SIL_RESERVED_0189,
  SIL_RESERVED_0190,
  SIL_RESERVED_0191,
  SIL_RESERVED_0192,
  SIL_RESERVED_0193,
  SIL_RESERVED_0194,
  SIL_RESERVED_0195,
  SIL_RESERVED_0196,
  SIL_RESERVED_0197,
  SIL_RESERVED_0198,
  SIL_RESERVED_0179,
  SIL_RESERVED_0180,
  SIL_RESERVED_0181,
  SIL_RESERVED_0182,
  SIL_RESERVED_0230,
  SIL_RESERVED_0126,
  SIL_RESERVED_0127,
  SIL_RESERVED_0128,
  SIL_RESERVED_0129,
  SIL_RESERVED_0130,
  SIL_RESERVED_0131,
  SIL_RESERVED_0140,
  SIL_RESERVED_0141,
  SIL_RESERVED_0142,
  SIL_RESERVED_0143,
  SIL_RESERVED_0222,
  SIL_RESERVED_0223,
  SIL_RESERVED_0224,
  SIL_RESERVED_0225,
  SIL_RESERVED_0226,
  SIL_RESERVED_0227,
  SIL_RESERVED_0228,
  SIL_RESERVED_0229,
  SIL_RESERVED_0231,
  SIL_RESERVED_0238,
  SIL_RESERVED_0239,
  SIL_RESERVED_0240,
  SIL_RESERVED_0241,
  SIL_RESERVED_0242,
  SIL_RESERVED_0243,
  SIL_RESERVED_0244,
  SIL_RESERVED_0245,
  SIL_RESERVED_0246,
  SIL_RESERVED_0232,
  SIL_RESERVED_0233,
  SIL_RESERVED_0234,
  SIL_RESERVED_0235,
  SIL_RESERVED_0236,
  SIL_RESERVED_0237,
  SIL_RESERVED_0247,
  SIL_RESERVED_0258,
  SIL_RESERVED_0259,
  SIL_RESERVED_0260,
  SIL_RESERVED_0261,
  SIL_RESERVED_0262,
  SIL_RESERVED_0263,
  SIL_RESERVED_0264,
  SIL_RESERVED_0265,
  SIL_RESERVED_0266,
  SIL_RESERVED_0248,
  SIL_RESERVED_0249,
  SIL_RESERVED_0250,
  SIL_RESERVED_0251,
  SIL_RESERVED_0252,
  SIL_RESERVED_0253,
  SIL_RESERVED_0254,
  SIL_RESERVED_0255,
  SIL_RESERVED_0256,
  SIL_RESERVED_0257,
  0xFFFFFFFF
};

const uint32_t *
BrhGetGlblCtrlInstanceIds (void)
{
  return BrhDfGlblCtrlInstanceIds;
}

void
BrhGetNpsMapData (
  uint32_t *Nps0,
  uint32_t *Nps1,
  uint32_t *Nps2,
  uint32_t *Nps4
  )
{
  if (Nps0 == NULL || Nps1 == NULL || Nps2 == NULL || Nps4 == NULL) {
    DF_TRACEPOINT(SIL_TRACE_ERROR, "NULL passed as argument\n");
    assert(false);
    return;
  } else {
    *Nps0 = 0xFFFFFFFF;
    *Nps1 = 0xFFFF;
    Nps2[0] = 0x5555;
    Nps2[1] = 0xAAAA;
    Nps4[0] = 0x1111;
    Nps4[1] = 0x4444;
    Nps4[2] = 0x8888;
    Nps4[3] = 0x2222;
  }
}


/**
 * BrhGetPhysDomainOfCcd
 *
 * @brief This function returns the physical domain that the given CCD belongs to
 *
 * @param  Socket      Zero based socket number
 * @param  PhysCcd     Physical CCD number
 * @param  DomainInfo  Input Pointer to the domain info block
 *
 * @retval uint32_t    Physical domain number
 */
uint32_t
BrhGetPhysDomainOfCcd (
  uint32_t            Socket,
  uint32_t            PhysCcd,
  uint32_t            NumberOfPhysicalDomains,
  void *DomainInfo
  )
{
  DF_DOMAIN_INFO_BLK *DomainInfoBlk = (DF_DOMAIN_INFO_BLK *) DomainInfo;
  uint32_t            PhysicalDomainOfCcd;
  uint32_t            CcdToQuadrant[] = {0, 3, 1, 2, 0, 3, 1, 2, 0, 3, 1, 2, 0, 3, 1, 2};

  if (PhysCcd >= sizeof (CcdToQuadrant) / sizeof (CcdToQuadrant[0])) {
    assert(false);
    return 0xFF;
  }

  for (PhysicalDomainOfCcd = 0; PhysicalDomainOfCcd < NumberOfPhysicalDomains;
    PhysicalDomainOfCcd++) {
    if ((DomainInfoBlk->PhysicalDomainInfo[PhysicalDomainOfCcd].
      NormalizedCsMap & ((0x111 << CcdToQuadrant[PhysCcd]) << (Socket * NORMALIZED_SOCKET_SHIFT))) != 0) {
      break;
    }
  }

  DF_TRACEPOINT(SIL_TRACE_INFO, "GetPhysDomainOfCcd Value %d, %d\n", PhysicalDomainOfCcd, NumberOfPhysicalDomains);

  assert(PhysicalDomainOfCcd < NumberOfPhysicalDomains);
  return PhysicalDomainOfCcd;
}

/**
 * BrhBuildCcdInfo
 *
 * @brief This function gathers data about CCDs to be fill the DomainInfo block.
 *
 * @param  SocketCount  Number of processors installed
 * @param  DieCount     Number of dies in a socket
 * @param  DomainInfo   Input Pointer to the domain info block
 *
 */
void
BrhBuildCcdInfo (
  uint32_t            SocketCount,
  uint32_t            DieCount,
  void *DomainInfo
  )
{
  uint32_t                i;
  uint32_t                j;
  uint32_t                CcdMap;
  CCD_ENABLE_REGISTER     CcdEnable;
  FABRIC_BLOCK_INSTANCE_INFORMATION3__CSNCSPIEALLM_REGISTER InstanceInfo3;
  DF_DOMAIN_INFO_BLK *DomainInfoBlk = (DF_DOMAIN_INFO_BLK *) DomainInfo;
  APOB_IP2IP_API          *ApobIp2IpApi;
  APOB_SOC_DIE_INFO       SocMaxDieInfo;
  SIL_STATUS              Status;

  Status = SilGetIp2IpApi(SilId_ApobClass, (void **) &ApobIp2IpApi);
  if ((Status != SilPass) || (ApobIp2IpApi == NULL)) {
    assert(Status == SilPass);
    return;
  }
  ApobIp2IpApi->ApobGetMaxDieInfo(&SocMaxDieInfo);

  for (i = 0; i < SocketCount; i++) {
    CcdMap = 0;
    DomainInfoBlk->CcxPerCcd[i] = 1;
    for (j = 0; j < SIL_RESERVED_0209; j++) {
      InstanceInfo3.Value = DfXFabricRegisterAccRead(i,
        0,
        FABRICBLOCKINSTANCEINFORMATION3_CSNCSPIEALLM_FUNC,
        FABRICBLOCKINSTANCEINFORMATION3_CSNCSPIEALLM_REG,
        SIL_RESERVED_0132 + j
        );
      if (InstanceInfo3.Field.BlockFabricID != 0x55) {
        CcdEnable.Value = DfXFabricRegisterAccRead(i, 0, CCDENABLE_FUNC, CCDENABLE_REG, SIL_RESERVED_0132 + j);
        CcdMap |= (CcdEnable.Field.CCDEn & 1) << j;
        if (CcdEnable.Field.SdpWideEn == 0) {
          // CCM is not in the wide mode, the second SDP port is for the second CCD
          CcdMap |= ((CcdEnable.Field.CCDEn >> 1) & 1) << SIL_RESERVED_0209 << j;
        }
      }
    }
    DomainInfoBlk->CcdCount[i] = 0;
    for (j = 0; CcdMap != 0; CcdMap &= ~(1 << j), j++) {
      if ((CcdMap & (1 << j)) != 0) {
        assert(j < PROJ_MAX_CCD_DIES_PER_SOCKET);
        DomainInfoBlk->LogToPhysCcd[i][DomainInfoBlk->CcdCount[i]++] = j;
      }
    }
    assert(DomainInfoBlk->CcdCount[i] <= SocMaxDieInfo.MaxSocCcdPerSocket);
  }

  DF_TRACEPOINT(SIL_TRACE_INFO, "BuildCcdInfo Successful\n");
}

/**
 * FabricSocInitBrh
 *
 * @brief   Makes Brh specific initialization calls
 *
 * @details This function makes DF init calls specific to Brh initialization.
 *          It is called after the DF common initialization is complete.
 *
 * @param   DfInputBlock  Pointer to the data fabric input block
 */
void
FabricSocInitBrh (
  DFCLASS_INPUT_BLK *DfInputBlock
  )
{
  DfXClockGatingInit(DfInputBlock);
  DfXPieRasInit(DfInputBlock);
}
