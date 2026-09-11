/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2026 Advanced Micro Devices, Inc. All rights reserved.
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
#include <FCH/Common/FchCore/FchSata/FchSata.h>
#include <FCH/Common/FchCore/FchSata/FchSataBlk.h>
#include <FCH/Kunlun/FchCore/FchSata/FchSataKl.h>
#include <Nbio/Brh/NbioBrh.h>
// CxlBrh.h refers to MPIO_COMPLEX_DESCRIPTOR, so Mpio has to come first.
#include <Mpio/Brh/MpioBrh.h>
#include <Cxl/Brh/CxlBrh.h>
#include <RcMgr/DfX/BRH/RcMgrBrh.h>
#include <MEM/Brh/MemBrh.h>
#include <FCH/Kunlun/FchCore/FchAb/FchAbKl.h>
#include <FCH/Kunlun/FchCore/FchXhci/FchXhciKl.h>
#include <FCH/Kunlun/FchCore/FchHwAcpi/FchHwAcpiKl.h>
#include <FCH/Kunlun/FchCore/FchIsa/FchIsaKl.h>
#include <FCH/Kunlun/MultiFch/MultiFchKl.h>
#include <FCH/Kunlun/FchKl.h>

SOC_IP_TABLE SocIpTblF1AM00Tp3 = {
  AMD_FAMILY_1A_BRH,
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
    //
    // The Ip-2-Ip table is rebuilt for TP3. Register every API required by a
    // pre-OS consumer before that consumer executes.
    //
    // Registrations only -- Initialize stays NULL, since these IPs are brought
    // up at TP1 and must not be re-run here.
    //
    //
    // Register the Kunlun FCH TP3 handlers. FchSata appears below with its TP3
    // initialization callback.
    //
    {
      SilId_FchClass,
      0,
      NULL,
      NULL,
      InitializeFchApiKl
    },
    {
      SilId_FchHwAcpiP,
      0,
      NULL,
      InitializeFchHwAcpiPreliminaryKlTp3,
      InitializeApiFchHwAcpiKl
    },
    {
      SilId_FchAb,
      0,
      NULL,
      InitializeFchAbKlTp3,
      InitializeApiFchAbKl
    },
    {
      SilId_FchHwAcpi,
      0,
      NULL,
      InitializeFchHwAcpiKlTp3,
      NULL
    },
    {
      SilId_FchIsa,
      0,
      NULL,
      InitializeFchIsaKlTp3,
      InitializeApiFchIsaKl
    },
    {
      SilId_FchUsb,
      0,
      NULL,
      InitializeFchUsbKlTp3,
      InitializeApiFchUsbKl
    },
    {
      SilId_MultiFchClass,
      0,
      NULL,
      InitializeMultiFchKlTp3,
      InitializeMultiFchApiKl
    },
    {
      SilId_NbioClass,
      0,
      NULL,
      NULL,
      InitializeApiNbioBrh
    },
    {
      SilId_MpioClass,
      0,
      NULL,
      NULL,
      SetMpioApiBrh
    },
    {
      SilId_RcManager,
      0,
      NULL,
      NULL,
      InitializeRcMgrApiBrh
    },
    {
      SilId_MemClass,
      0,
      NULL,
      NULL,
      SetMemApiBrh
    },
    {
      SilId_CxlClass,
      0,
      NULL,
      NULL,
      SetCxlApiBrh
    },
    {
      SilId_SmuClass,
      0,
      NULL,
      NULL,
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
      SilId_FchSata,
      0,
      NULL,
      InitializeFchSataKlTp3,
      InitializeApiFchSataKl
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
