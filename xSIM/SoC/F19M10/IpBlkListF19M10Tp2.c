/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT
/**
 * @file
 *
 * This file contains the list of IP Blocks used for the F19M10 SoC. The
 * build may support multiple families in one socket * or multiple
 * families in separate customer boards (1 Host Firmware, N * similar boards)
 */

#include <SilCommon.h>
#include <xSIM.h>
#include "IpHandler.h"
#include <SilSocLogicalId.h>
#include <SMU/V13/SmuV13.h>
#include <CCX/Zen4/Zen4.h>
#include <FCH/9004/FchSn.h>
#include <FCH/9004/MultiFch/MultiFchSn.h>
#include <FCH/Common/FchCommonCfg.h>
#include <FCH/9004/FchCore/FchHwAcpi/FchHwAcpiSn.h>
#include <FCH/9004/FchCore/FchXhci/FchXhciSn.h>
#include <FCH/9004/FchCore/FchSata/FchSataSn.h>
#include <RAS/Genoa/RasGenoa.h>
#include <DF/DfX/DfX.h>

/**
 * Declare the IP Block list
 */
const SOC_IP_TABLE SocIpTblF19M10Tp2 = {
  AMD_FAMILY_19_10,     // F19M10  a.k.a. Genoa
                        // xSim Common Var descriptors for F19M10
                        // See: xsim.h:ACTIVE_SOC_DATA
  {.NumCcdsPerDie = 12,
  },
  // start the list of IPs for this SoC
  {
    //  ID, InputSize, Ptr:FcnSetInput, Ptr:IP Initialize, Ptr:Set Ip Api
    {
      SilId_DfClass,
      0,
      NULL,
      NULL,
      DfInitApiRev4
    },
    {
      SilId_SmuClass,
      0,
      NULL,
      NULL,
      InitializeSmuApiV13
    },
    {
      SilId_CcxClass,
      0,
      NULL,
      NULL,
      InitializeApiZen4
    },
    {
      SilId_FchClass,
      0,
      NULL,
      NULL,
      InitializeFchApiSn
    },
    {
      SilId_FchHwAcpiP,
      0,
      NULL,
      NULL,
      InitializeApiFchHwAcpiSn
    },
    {
      SilId_FchUsb,
      0,
      NULL,
      NULL,
      InitializeApiFchXhciSn
    },
    {
      SilId_FchSata,
      0,
      NULL,
      NULL,
      InitializeApiFchSataSn
    },
    {
      SilId_MultiFchClass, // SilId_MultiFchClass should run AFTER all other FCH IP blocks
      0,
      NULL,
      NULL,
      InitializeMultiFchApiSn
    },
    {
      SilId_RasClass,
      0,
      NULL,
      NULL,
      InitializeApiRasGenoa
    },
    {SilId_ListEnd}  // End of list marker
  }
};
