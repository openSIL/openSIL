/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 */
/**
 * @file
 *
 * This file contains the list of IP Blocks used for the F1AM00
 * SoC (a.k.a. Breithorn). The build may support multiple
 * families in one socket or multiple families in separate
 * customer boards (1 BIOS, N similar boards)
 */

#include <xSIM.h>
#include "IpHandler.h"
#include <SilSocLogicalId.h>
#include <SMU/Brh/SmuBrh.h>
#include <DF/DfX/BRH/DfBrh.h>
#include <RAS/Brh/RasBrh.h>
#include <CCX/Zen5/BRH/CcxBrh.h>
#include <APOB/BRH/ApobBrh.h>

/**
 * Declare the IP Block list
 */
SOC_IP_TABLE SocIpTblF1AM00Tp2 = {
  AMD_FAMILY_1A_BRH,    // This is the 'Breithorn' F1AM00  a.k.a. Turin
                        // xSim Common Var descriptors for F19M10
                        // See: xsim.h:ACTIVE_SOC_DATA
  {.NumCcdsPerDie = 16, },
  // start the list of IPs for this SoC
  {
    {
      SilId_ApobClass,
      0,
      NULL,
      NULL,
      InitializeApobApiBrh
    },
    {
      SilId_SmuClass,
      0,
      NULL,
      InitializeSmuTp2Brh,
      InitializeSmuApiBrh
    },
    {
      SilId_DfClass,
      0,
      NULL,
      NULL,
      DfInitApiBrh
    },
    {
      SilId_CcxClass,
      0,
      NULL,
      NULL,
      InitializeApiZen5Brh
    },
    {
      SilId_RasClass,
      0,
      NULL,
      NULL,
      InitializeApiRasBrh
    },
    //  ID, InputSize, Ptr:FcnSetInput, Ptr:IP Initialize, Ptr:Set Ip Api
    {SilId_ListEnd}  // End of list marker
  }
};
