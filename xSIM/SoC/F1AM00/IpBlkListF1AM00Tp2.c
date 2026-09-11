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
// CxlBrh.h refers to MPIO_COMPLEX_DESCRIPTOR, so Mpio has to come first.
#include <Mpio/Brh/MpioBrh.h>
#include <Cxl/Brh/CxlBrh.h>
#include <Nbio/Brh/NbioBrh.h>
#include <RcMgr/DfX/BRH/RcMgrBrh.h>
#include <MEM/Brh/MemBrh.h>
#include <Sdxi/Brh/SdxiBrh.h>
#include <FCH/Common/FchCore/FchSata/FchSata.h>
#include <FCH/Common/FchCore/FchSata/FchSataBlk.h>
#include <FCH/Kunlun/FchCore/FchAb/FchAbKl.h>
#include <FCH/Kunlun/FchCore/FchSata/FchSataKl.h>
#include <FCH/Kunlun/FchCore/FchXhci/FchXhciKl.h>
#include <FCH/Kunlun/FchCore/FchHwAcpi/FchHwAcpiKl.h>
#include <FCH/Kunlun/FchCore/FchIsa/FchIsaKl.h>
#include <FCH/Kunlun/MultiFch/MultiFchKl.h>
#include <FCH/Kunlun/FchKl.h>

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
    //
    // The Ip-2-Ip table is rebuilt for TP2. Register every API required by a
    // TP2 consumer, ordered before its consumers.
    //
    // These are registrations only -- Initialize stays NULL, because the IPs
    // themselves are already brought up at TP1 and must not run again here.
    //
    //
    // Kunlun FCH TP2 handlers reuse input blocks allocated at TP1, so these
    // entries carry no size or SetInput callback.
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
      InitializeFchHwAcpiPreliminaryKlTp2,
      InitializeApiFchHwAcpiKl
    },
    {
      SilId_FchAb,
      0,
      NULL,
      InitializeFchAbKlTp2,
      InitializeApiFchAbKl
    },
    {
      SilId_FchHwAcpi,
      0,
      NULL,
      InitializeFchHwAcpiKlTp2,
      NULL
    },
    {
      SilId_FchIsa,
      0,
      NULL,
      InitializeFchIsaKlTp2,
      InitializeApiFchIsaKl
    },
    {
      SilId_FchUsb,
      0,
      NULL,
      InitializeFchUsbKlTp2,
      InitializeApiFchUsbKl
    },
    {
      SilId_FchSata,
      0,
      NULL,
      InitializeFchSataKlTp2,
      InitializeApiFchSataKl
    },
    {
      SilId_MultiFchClass,
      0,
      NULL,
      InitializeMultiFchKlTp2,
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
      SilId_SdxiClass,
      0,
      NULL,
      NULL,
      SetSdxiApiBrh
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
