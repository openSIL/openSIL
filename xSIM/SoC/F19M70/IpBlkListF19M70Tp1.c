/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  IpBlkListF19M70Tp1.c
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
#include <RcMgr/DfX/PHX/RcMgrPhx.h>
#include <DF/DfX/PHX/DfPhx.h>
#include <CCX/Zen4/PHX/CcxPhx.h>
#include <Nbio/Phx/NbioPhx.h>
#include <Mpio/Phx/MpioPhx.h>
#include <GFX/Phx/GfxPhx.h>
#include <FCH/FchClass-api.h>
#include <FCH/Tacoma/FchTc.h>
#include <FCH/Tacoma/FchCore/FchAb/FchAbTc.h>
#include <FCH/Tacoma/FchCore/FchHwAcpi/FchHwAcpiTc.h>
#include <FCH/Tacoma/FchCore/FchUsb/FchUsbTc.h>
#include <FCH/Tacoma/FchCore/FchIsa/FchIsaTc.h>
#include <FCH/Tacoma/FchCore/FchSd/FchSdTc.h>
#include <APOB/PHX/ApobPhx.h>
#include <MEM/Phx/MemPhx.h>
#include <PROM/PromClass-api.h>
#include <PROM/Common/PromInit.h>


/**
 * IP Block list
 * @brief IP Block list
 * @details The host firmware does not have a specific initialization order.
 *          The openSIL firmware however, does have an initialization order
 *          for the IPs, since some IPs are dependent on other IP blocks.
 *          In the table below, the following order is used due to inter-IP
 *          dependencies:
 *          - RC Mgr: Multiple services in resource manager are used by other
 *                    IPs, this IP is initialized at the beginning of
 *                    the IP block list.
 *          - DF:     Multiple services in DF are used by other
 *                    IPs, this IP is initialized after resource manager
 *                    in the IP block list.
 *          - SMU:    CCX, FCH and MPIO use SMU APIs, so SMU needs to be
 *                    initialized before these 3 IPs.
 *          - CCX:    CCX is dependent on SMU and DF APIs, and is initialized
 *                    after those IPs.
 *          - NBIO:   NBIO produces the PCIe topology. MPIO is dependent
 *                    on this topology. NBIO is initialized prior to
 *                    MPIO due to this dependency.
 *          - FCH:    MPIO is dependent on some FCH services, due to which FCH
 *                    needs to be initialized prior to MPIO. The rest of the FCH
 *                    blocks are initialized before MPIO as well, to
 *                    group the initialization of all FCH sub-components.
 *          - MPIO:   MPIO firmware initializes and trains the PCIe links
 *                    (required for CXL). Due to this dependency,
 *                    MPIO is initialized prior to CXL IP block.
 *          - Prom: PROM21 is dependent on MPIO and is initialized after MPIO.
 */
const SOC_IP_TABLE SocIpTblF19M70Tp1 = {
  AMD_FAMILY_19_PHX,     // This is the 'Client' F19M70  a.k.a. Phoenix

  // xSim Common Var descriptors for F19M70.   See: xsim.h:ACTIVE_SOC_DATA
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
      SilId_RcManager,
      RCMGR_DATA_SIZE_PHX,
      RcMgrSetInputBlkPhx,
      InitializeRcMgrPhxTp1,
      InitializeApiRcMgrPhx
    },
    {
      SilId_DfClass,
      DF_DATA_SIZE_PHX,
      DfXSetInputBlkPhx,
      InitializeDfXPhxTp1,
      InitializeApiDfXPhx
    },
    {
      SilId_SmuClass,
      sizeof (SMUCLASS_INPUT_BLK),
      SmuSetInputBlkPhx,
      NULL,
      InitializeApiSmuPhx
    },
    {
      SilId_MemClass,
      0,
      NULL,
      NULL,
      SetMemApiPhx
    },
    {
      SilId_NbioClass,
      NBIO_DATA_SIZE_PHX,
      NbioSetInputBlkPhx,
      InitializeNbioPhxTp1,
      InitializeApiNbioPhx
    },
    {
      SilId_FchClass,
      FCHCLASS_INPBLK_SIZE_TC,
      FchClassSetInputBlkTc,
      NULL,
      InitializeFchApiTc
    },
    {
      SilId_FchHwAcpiP,
      FCHACPI_INPBLK_SIZE_TC,
      FchHwAcpiSetInputBlkTc,
      InitializeFchHwAcpiPreliminaryTcTp1,
      InitializeApiFchHwAcpiTc
    },
    {
      SilId_FchAb,
      FCH_AB_INPBLK_SIZE_TC,
      FchAbSetInputBlkTc,
      InitializeFchAbTcTp1,
      NULL
    },
    {
      SilId_FchHwAcpi,
      0x0,
      NULL,
      InitializeFchHwAcpiTcTp1,
      NULL
    },
    {
      SilId_FchUsb,
      FCHUSB_INPBLK_SIZE_TC,
      FchUsbSetInputBlkTc,
      InitializeFchUsbTcTp1,
      InitializeApiFchUsbTc
    },
    {
      SilId_FchIsa,
      FCHISA_INPBLK_SIZE_TC,
      FchIsaSetInputBlkTc,
      InitializeFchIsaTcTp1,
      InitializeApiFchIsaTc
    },
    {
      SilId_FchSd,
      0,
      NULL,
      InitializeFchSdTcTp1,
      NULL
    },
    {
      SilId_GfxClass,
      GFX_DATA_SIZE_PHX,
      GfxSetInputBlkPhx,
      InitializeGfxPhxTp1,
      NULL
    },
    {
      SilId_MpioClass,
      MPIO_DATA_SIZE_PHX,
      MpioSetInputBlkPhx,
      InitializeMpioPhxTp1,
      InitializeApiMpioPhx
    },
    {
      SilId_PromClass,
      PROMCLASS_DATA_SIZE,
      PromClassSetInputBlock,
      InitializePromTp1,
      NULL,
    },
    {
      SilId_CcxClass,
      CCX_DATA_SIZE_ZEN4_PHX,
      CcxSetInputBlkPhx,
      InitializeCcxZen4PhxTp1,
      InitializeApiZen4Phx
    },
    {SilId_ListEnd, 0, NULL, NULL, NULL}  // End of list marker
  }
};
