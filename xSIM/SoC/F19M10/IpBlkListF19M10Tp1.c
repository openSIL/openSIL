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
#include <DF/Df.h>
#include <DF/DfX/DfX.h>
#include <RcMgr/DfX/FabricRcInitDfX.h>
#include <RcMgr/FabricResourceManager.h>
#include <SMU/Common/SmuCommon.h>
#include <SMU/V13/SmuV13.h>
#include <CCX/Zen4/Zen4.h>
#include <FCH/Common/Fch.h>
#include <FCH/9004/FchSn.h>
#include <FCH/9004/MultiFch/MultiFchSn.h>
#include <FCH/Common/FchCore/FchHwAcpi/FchHwAcpi.h>
#include <FCH/9004/FchCore/FchHwAcpi/FchHwAcpiSn.h>
#include <FCH/Common/FchCore/FchXhci/FchXhci.h>
#include <FCH/9004/FchCore/FchXhci/FchXhciSn.h>
#include <FCH/Common/FchCore/FchSpi/FchSpi.h>
#include <FCH/Common/FchCore/FchAb/FchAb.h>
#include <FCH/Common/FchCore/FchSata/FchSata.h>
#include <FCH/9004/FchCore/FchSata/FchSataSn.h>
#include <NBIO/IOD/NbioIod.h>
#include <RAS/Genoa/RasGenoa.h>
#include <Mpio/Genoa/Mpio.h>
#include <Sdci/Genoa/Sdci.h>
#include <Cxl/Genoa/Cxl.h>

#if CONFIG_HAVE_XMP_VER_B
#include <XMP/XmpRevB-api.h>      // Example includes RevB prototypes
#endif

/**
 * Declare the IP Block list
 */
const SOC_IP_TABLE SocIpTblF19M10Tp1 = {
  AMD_FAMILY_19_10,     // F19M10  a.k.a. Genoa

  // xSim Common Var descriptors for F19M10.   See: xsim.h:ACTIVE_SOC_DATA
  {.NumCcdsPerDie = 12,
  },
  // start the list of IPs for this SoC
  {
    //  ID, InputSize, Ptr:FcnSetInput, Ptr:IP Initialize, Ptr:Set Ip Api
    {
      SilId_RcManager,
      sizeof(DFX_RCMGR_DATA_BLK),
      DfXRcMgrSetInputBlk,
      InitializeResourceManagerDfXTp1,
      InitializeRcMgrApi
    },
    {
      SilId_DfClass,
      sizeof(DFCLASS_INPUT_BLK),
      DfSetInputBlk,
      InitializeDataFabricTp1,
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
      SilId_NbioClass,
      sizeof(NBIOCLASS_DATA_BLOCK),
      NbioClassSetInputBlk,
      InitializeNbioTp1,
      InitializeApiNbioIod
    },
    {
      SilId_NorthBridgePcie,
      sizeof(NORTH_BRIDGE_PCIE_SIB) + NBIO_PCIE_DATA_LENGTH,
      NULL,
      NULL,
      NULL
    },
    {
      SilId_CcxClass,
      CCX_DATA_SIZE_ZEN4,
      CcxClassSetInputBlk,
      InitializeCcxZen4Tp1,
      InitializeApiZen4
    },
    {
      SilId_FchClass,
      sizeof(FCHCLASS_INPUT_BLK),
      FchClassSetInputBlk,
      NULL,
      InitializeFchApiSn
    },
    {
      SilId_FchHwAcpiP,
      sizeof(FCHHWACPI_INPUT_BLK),
      FchHwAcpiPreliminarySetInputBlk,
      InitializeFchHwAcpiPreliminaryTp1,
      InitializeApiFchHwAcpiSn
    },
    {
      SilId_FchAb,
      sizeof(FCHAB_INPUT_BLK),
      FchAbSetInputBlk,
      InitializeFchAbTp1,
      NULL
    },
    {
      SilId_FchHwAcpi,
      0x0,
      NULL,
      InitializeFchHwAcpiTp1,
      NULL
    },
    {
      SilId_FchUsb,
      sizeof(FCHUSB_INPUT_BLK),
      FchUsbSetInputBlk,
      InitializeFchUsbTp1,
      InitializeApiFchXhciSn
    },
    {
      SilId_FchSpi,
      sizeof(FCHSPI_INPUT_BLK),
      FchSpiSetInputBlk,
      InitializeFchSpiTp1,
      NULL
    },
    {
      SilId_FchSata,
      SATA_CONTROLLER_NUM * sizeof(FCHSATA_INPUT_BLK),
      FchSataSetInputBlk,
      InitializeFchSataSnTp1,
      InitializeApiFchSataSn
    },
    {
      SilId_MultiFchClass, // SilId_MultiFchClass should run AFTER all other FCH IP.
      0,
      NULL,
      InitializeMultiFchSnTp1,
      InitializeMultiFchApiSn
    },
    {
      SilId_RasClass,
      0,
      NULL,
      NULL,
      InitializeApiRasGenoa
    },
    {
      SilId_MpioClass,
      sizeof(MPIOCLASS_INPUT_BLK),
      MpioClassSetInputBlock,
      InitializeMpioTp1,
      SetMpioApi
    },
    {
      SilId_SdciClass,
      sizeof(SDCICLASS_INPUT_BLK),
      SdciClassSetInputBlock,
      InitializeSdciTp1,
      SetSdciApi
    },
    {
      SilId_CxlClass,
      sizeof(CXLCLASS_DATA_BLK),
      CxlClassSetInputBlock,
      InitializeCxlTp1,
      SetCxlApi
    },
#if CONFIG_HAVE_XMP_VER_B
    {
      SilId_XmpClass,
      XMP_INPBLK_SIZE_B,
      XmpSetInputBlockRevBTp1,
      XmpInitializeRevBTp1,
      XmpInitApiRevB
    },
#endif
    {SilId_ListEnd, 0,NULL,NULL,NULL}  // End of list marker
  }
};
