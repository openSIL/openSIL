/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  IpBlkListF19M70Tp3.c
 *
 * This file contains the list of IP Blocks used for the F19M70 SoC
 * (a.k.a. Phoenix). The build may support multiple families in one socket
 * or multiple families in separate customer boards (1 BIOS, N
 * similar boards)
 */

#include <SilCommon.h>
#include <xSIM.h>
#include "IpHandler.h"
#include <SilSocLogicalId.h>
#include <SMU/Phx/SmuPhx.h>
#include <CCX/Zen4/PHX/CcxPhx.h>
#include <Nbio/Phx/NbioPhx.h>
#include <DF/DfX/PHX/DfPhx.h>
#include <FCH/FchClass-api.h>
#include <FCH/Tacoma/FchTc.h>
#include <FCH/Tacoma/FchCore/FchHwAcpi/FchHwAcpiTc.h>
#include <APOB/PHX/ApobPhx.h>

/**
 * IP Block list
 */
const SOC_IP_TABLE SocIpTblF19M70Tp3 = {
  AMD_FAMILY_19_PHX,     // This is the 'Client' F19M70  a.k.a. Phoenix
                         // xSim Common Var descriptors for F19M70
                         // See: xsim.h:ACTIVE_SOC_DATA
  {.NumCcdsPerDie = 12, },
  // start the list of IPs for this SoC
  {
    //  ID, InputSize, Ptr:FcnSetInput, Ptr:IP Initialize, Ptr:Set Ip Api
    {
      SilId_ApobClass,
      0,
      NULL,
      NULL,
      InitializeApobApiPhx
    },
    {
      SilId_SmuClass,
      0,
      NULL,
      NULL,
      InitializeApiSmuPhx
    },
    {
      SilId_DfClass,
      0,
      NULL,
      NULL,
      InitializeApiDfXPhx
    },
    {
      SilId_NbioClass,
      0,
      NULL,
      NULL,
      InitializeApiNbioPhx
    },
    {
      SilId_CcxClass,
      0,
      NULL,
      NULL,
      InitializeApiZen4Phx
    },
    {
      SilId_FchClass,
      0,
      NULL,
      NULL,
      InitializeFchApiTc
    },
    {
      SilId_FchHwAcpiP,
      0,
      NULL,
      NULL,
      InitializeApiFchHwAcpiTc
    },
    {
      SilId_FchUsb,
      0,
      NULL,
      NULL,
      NULL,
    },
    {SilId_ListEnd}  // End of list marker
  }
};
