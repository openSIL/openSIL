/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  DfInitPhx.c
 * @brief Various functions and data for data fabric initialization on PHX
 */


#include <SilCommon.h>
#include <ProjSocConst.h>
#include <DF/Df.h>
#include <DF/Common/DfCmn2Rev.h>
#include <DF/DfX/PHX/DfAcpiDomainInfoPhx.h>
#include "DfPhx.h"
#include "DfSilFabricInfoPhx.h"
#include <DF/DfX/DfX.h>

const uint32_t PhxDfGlblCtrlInstanceIds[] = {
  PHX_CCM0_INSTANCE_ID,
  PHX_CS0_INSTANCE_ID,
  PHX_CS1_INSTANCE_ID,
  PHX_CS2_INSTANCE_ID,
  PHX_CS3_INSTANCE_ID,
  PHX_DCE_INSTANCE_ID,
  PHX_GCM0_INSTANCE_ID,
  PHX_GCM1_INSTANCE_ID,
  PHX_GCM2_INSTANCE_ID,
  PHX_GCM3_INSTANCE_ID,
  PHX_IOM0_INSTANCE_ID,
  PHX_IOS0_INSTANCE_ID,
  PHX_IPU0_INSTANCE_ID,
  PHX_MMHUB_INSTANCE_ID,
  PHX_NCS0_INSTANCE_ID,
  PHX_NCS1_INSTANCE_ID,
  PHX_PIE_INSTANCE_ID,
  PHX_SPF0_INSTANCE_ID,
  PHX_SPF1_INSTANCE_ID,
  PHX_SPF2_INSTANCE_ID,
  PHX_SPF3_INSTANCE_ID,
  PHX_TCDX0_INSTANCE_ID,
  PHX_TCDX1_INSTANCE_ID,
  PHX_TCDX2_INSTANCE_ID,
  PHX_TCDX3_INSTANCE_ID,
  PHX_TCDX4_INSTANCE_ID,
  PHX_TCDX5_INSTANCE_ID,
  0xFFFFFFFF
};

/**
 *  FabricSocInitPhx
 *
 * @brief Makes PHX specific initialization calls
 * @details This function makes DF init calls specific to Phx initialization.
 *          It is called after the DF common initialization is complete.
 *
 * @param   SilContext          A context structure through which host firmware defined data
 *                              can be passed to openSIL. The host firmware is responsible
 *                              for initializing the SIL_CONTEXT structure.
 */
void
FabricSocInitPhx (
  SIL_CONTEXT       *SilContext,
  DFCLASS_INPUT_BLK *DfInputBlock
  )
{
  DfPhxRasInit(SilContext, DfInputBlock);
  DfXClockGatingInit(SilContext, DfInputBlock);
  DfPhxCsInit(SilContext);
}

/**
 * PhxGetGlblCtrlInstanceIds
 *
 * @brief This function returns the Global Control Instance IDs
 *
 * @retval  Global Control Instance IDs
 */
const uint32_t *
PhxGetGlblCtrlInstanceIds (void) {
  return PhxDfGlblCtrlInstanceIds;
}

/**
 * PhxGetNpsMapData
 *
 * @brief This function update the NpsMapData
 *
 * @param  Nps0  Zero based socket number
 * @param  Nps1  Physical CCD number
 * @param  Nps2  Input Pointer to the domain info block
 * @param  Nps4  Input Pointer to the domain info block
 *
 */
void
PhxGetNpsMapData (
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
  }

  *Nps0 = 0x0FFF0FFF;
  *Nps1 = 0x0FFF;
  Nps2[0] = 0x0555;
  Nps2[1] = 0x0AAA;
  Nps4[0] = 0x0111;
  Nps4[1] = 0x0444;
  Nps4[2] = 0x0888;
  Nps4[3] = 0x0222;
}

/**
 * PhxGetPhysDomainOfCcd
 *
 * @brief This function returns the physical domain that the given CCD belongs to
 *
 * @param  Socket      Zero based socket number
 * @param  PhysCcd     Physical CCD number
 * @param  DomainInfo  Input Pointer to the domain info block
 *
 * @retval     uint32_t    Physical domain number
 */
uint32_t
PhxGetPhysDomainOfCcd (
  uint32_t             Socket,
  uint32_t             PhysCcd,
  uint32_t             NumberOfPhysicalDomains,
  void *DomainInfo
  )
{
  DF_PHX_DOMAIN_INFO_BLK *DomainInfoBlk = (DF_PHX_DOMAIN_INFO_BLK *) DomainInfo;
  uint32_t  PhysicalDomainOfCcd;

  for (PhysicalDomainOfCcd = 0; PhysicalDomainOfCcd < NumberOfPhysicalDomains; PhysicalDomainOfCcd++) {
    if ((DomainInfoBlk->PhysicalDomainInfo[PhysicalDomainOfCcd].NormalizedCsMap & ((1 << PhysCcd)
      << (Socket * NORMALIZED_SOCKET_SHIFT))) != 0) {
      break;
    }
  }

  DF_TRACEPOINT(SIL_TRACE_INFO, "GetPhysDomainOfCcd Value %d,%d\n", PhysicalDomainOfCcd, NumberOfPhysicalDomains);

  assert(PhysicalDomainOfCcd < NumberOfPhysicalDomains);
  return PhysicalDomainOfCcd;
}

/**
 * PhxBuildCcdInfo
 *
 * @brief This function gathers data about CCDs to be fill the DomainInfo block.
 *
 * @param  SocketCount  Number of processors installed
 * @param  DieCount     Number of dies in a socket
 * @param  DomainInfo   Input Pointer to the domain info block
 *
 */
void
PhxBuildCcdInfo (
  uint32_t             SocketCount,
  uint32_t             DieCount,
  void                 *DomainInfo
  )
{
  UNUSED(SocketCount);
  UNUSED(DieCount);
  UNUSED(DomainInfo);

  DF_TRACEPOINT(SIL_TRACE_INFO, "PHX does not support Build CCD Info Function\n");
}
