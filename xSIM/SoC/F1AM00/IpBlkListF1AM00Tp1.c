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
#include <Nbio/Brh/NbioBrh.h>
#include <Mpio/Brh/MpioBrh.h>
#include <SMU/Brh/SmuBrh.h>
#include <RcMgr/DfX/BRH/RcMgrBrh.h>
#include <DF/DfX/BRH/DfBrh.h>
#include <CCX/Zen5/BRH/CcxBrh.h>
#include <Sdxi/Brh/SdxiBrh.h>
#include <Cxl/Brh/CxlBrh.h>
#include <RAS/Brh/RasBrh.h>
#include <MEM/Brh/MemBrh.h>

#include <FCH/Common/FchCore/FchSata/FchSata.h>
#include <FCH/Common/FchCore/FchSata/FchSataBlk.h>
#include <FCH/Kunlun/FchCore/FchAb/FchAbKl.h>
#include <FCH/Kunlun/FchCore/FchSata/FchSataKl.h>
#include <FCH/Kunlun/FchCore/FchXhci/FchXhciKl.h>
#include <FCH/Kunlun/FchCore/FchHwAcpi/FchHwAcpiKl.h>
#include <FCH/Kunlun/FchCore/FchIsa/FchIsaKl.h>
#include <FCH/Kunlun/MultiFch/MultiFchKl.h>
#include <FCH/Kunlun/FchKl.h>

#include <APOB/BRH/ApobBrh.h>
/**
 * Declare the IP Block list
 */
SOC_IP_TABLE SocIpTblF1AM00Tp1 = {
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
    {
      SilId_DfClass,
      DF_DATA_SIZE_BRH,
      DfSetInputBlkBrh,
      InitializeDataFabricTp1Brh,
      DfInitApiBrh
    },
    {
      SilId_RcManager,
      RCMGR_DATA_SIZE_BRH,
      RcMgrSetInputBlkBrh,
      InitializeResourceManagerBrhTp1,
      InitializeRcMgrApiBrh
    },
    {
      SilId_SmuClass,
      sizeof (SMUCLASS_INPUT_BLK),
      SmuSetInputBlkBrh,
      InitializeSmuTp1Brh,
      InitializeSmuApiBrh
    },
    {
      SilId_MemClass,
      0,
      NULL,
      NULL,
      SetMemApiBrh
    },
    {
      SilId_NbioClass,
      NBIO_DATA_SIZE_BRH,
      NbioClassSetInputBlk,
      InitializeNbioBrhTp1,
      InitializeApiNbioBrh
    },
    {
      SilId_CcxClass,
      CCX_DATA_SIZE_ZEN5_BRH,
      CcxClassSetInputBlkBrh,
      InitializeCcxZen5BrhTp1,
      InitializeApiZen5Brh
    },
    {
      SilId_FchClass,
      FCHCLASS_INPBLK_SIZE_KL,
      FchClassSetInputBlkKl,
      NULL,
      InitializeFchApiKl
    },
    {
      SilId_FchHwAcpiP,
      FCHACPI_INPBLK_SIZE_KL,
      FchHwAcpiPreliminarySetInputBlkKl,
      InitializeFchHwAcpiPreliminaryKlTp1,
      InitializeApiFchHwAcpiKl
    },
    {
      SilId_FchAb,
      FCHAB_INPBLK_SIZE_KL,
      FchAbSetInputBlkKl,
      InitializeFchAbKlTp1,
      InitializeApiFchAbKl
    },
    {
      SilId_FchHwAcpi,
      0x0,
      NULL,
      InitializeFchHwAcpiKlTp1,
      NULL
    },
    {
      SilId_FchIsa,
      FCHISA_INPBLK_SIZE_KL,
      FchIsaSetInputBlkKl,
      InitializeFchIsaKlTp1,
      InitializeApiFchIsaKl
    },
    {
      SilId_FchUsb,
      FCHUSB_INPBLK_SIZE_KL,
      FchUsbSetInputBlkKl,
      InitializeFchUsbKlTp1,
      InitializeApiFchUsbKl
    },
    {
      SilId_FchSata,
      SATA_CONTROLLER_NUM * sizeof(FCHSATA_INPUT_BLK),
      FchSataSetInputBlk,
      InitializeFchSataKlTp1,
      InitializeApiFchSataKl
    },
    {
      SilId_MultiFchClass,
      MULTIFCH_DATA_SIZE_KL,
      MultiFchSetInputBlkKl,
      InitializeMultiFchKlTp1,
      InitializeMultiFchApiKl
    },
    {
      SilId_MpioClass,
      sizeof (MPIOCLASS_INPUT_BLK),
      MpioClassSetInputBlock,
      InitializeMpioBrhTp1,
      SetMpioApiBrh
    },
    {
      SilId_SdxiClass,
      sizeof (SDXICLASS_INPUT_BLK),
      SdxiClassSetInputBlock,
      InitializeSdxiBrhTp1,
      SetSdxiApiBrh
    },
    {
      SilId_CxlClass,
      sizeof (CXLCLASS_DATA_BLK),
      CxlClassSetInputBlock,
      InitializeBrhCxlTp1,
      SetCxlApiBrh
    },
    {
      SilId_RasClass,
      RAS_DATA_SIZE_BRH,
      RasSetDataBlkBrh,
      InitializeRasTp1Brh,
      InitializeApiRasBrh
    },
    {SilId_ListEnd}  // End of list marker
  }
};
