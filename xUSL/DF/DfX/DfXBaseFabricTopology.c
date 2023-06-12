/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  DfXBaseFabricTopology.c
 * @brief DfX specific implementations of BaseFabricTopology functions
 */

#include <DF/Df.h>
#include <DF/Common/SilBaseFabricTopologyLib.h>
#include <DF/Common/BaseFabricTopologyCmn.h>
#include <DF/Common/DfCmn2Rev.h>
#include <DF/DfX/DfXBaseFabricTopology.h>
#include "DfXBaseFabricTopology.h"
#include "DfXFabricRegisterAcc.h"

/**
 * DfXGetNumberOfSystemDies
 *
 * @brief TBD
 *
 * @return uint32_t TBD
 *
 */
uint32_t
DfXGetNumberOfSystemDies (void)
{
  uint32_t                   Value32;

  Value32 = DfXFabricRegisterAccRead(0, 0, 0x4, 0x184, FABRIC_REG_ACC_BC);

  return Value32 & 0xFF;
}

/**
 * DfXGetNumberOfSystemRootBridges
 *
 * @brief TBD
 *
 * @return uint32_t TBD
 *
 */
uint32_t
DfXGetNumberOfSystemRootBridges (void)
{
  uint32_t                    RbCount;
  uint32_t                    SocketCount;
  uint32_t                    Socket;
  DF_COMMON_2_REV_XFER_BLOCK  *DfXfer;
  SIL_STATUS                   Status;

  Status = SilGetCommon2RevXferTable(SilId_DfClass, (void **) &DfXfer);
  assert(Status == SilPass);

  SocketCount = DfXfer->DfGetNumberOfProcessorsPresent ();
  RbCount = 0;
  for (Socket = 0; Socket < SocketCount; Socket++) {
    RbCount += DfXGetNumberOfRootBridgesOnSocket(Socket);
  }

  return RbCount;
}

/**
 * DfXGetNumberOfRootBridgesOnSocket
 *
 * @brief TBD
 *
 * @param Socket TBD
 *
 * @return uint32_t TBD
 *
 */
uint32_t
DfXGetNumberOfRootBridgesOnSocket (
  uint32_t Socket
  )
{
  uint32_t                     RbCount;
  uint32_t                     DieCount;
  uint32_t                     Die;
  DF_COMMON_2_REV_XFER_BLOCK  *DfXfer;
  SIL_STATUS                   Status;

  Status = SilGetCommon2RevXferTable(SilId_DfClass, (void **) &DfXfer);
  assert(Status == SilPass);

  DieCount = DfGetNumberOfDiesOnSocket(Socket);
  RbCount = 0;
  for (Die = 0; Die < DieCount; Die++) {
    RbCount += DfXfer->DfGetNumberOfRootBridgesOnDie(Socket, Die);
  }

  return RbCount;
}

/**
 * DfXGetNumberOfRootBridgesOnDie
 *
 * @brief Get the number of root bridges on the given socket die
 *
 * @param Socket     The Socket
 * @param Die        The Die
 *
 * @return uint32_t  The number of root bridges
 *
 */
uint32_t
DfXGetNumberOfRootBridgesOnDie (
  uint32_t Socket,
  uint32_t Die
  )
{
  uint32_t                   RbCount;
  uint32_t                   Value32;
  DF_COMMON_2_REV_XFER_BLOCK *DfXfer;
  SIL_STATUS                   Status;

  Status = SilGetCommon2RevXferTable(SilId_DfClass, (void **) &DfXfer);
  assert(Status == SilPass);


  Value32 = DfXFabricRegisterAccRead(Socket, Die, 0x4, 0x184, FABRIC_REG_ACC_BC);
  RbCount = ((Value32 >> 8) & 0xFF) / (uint32_t) DfXfer->DfGetNumberOfProcessorsPresent ();
  RbCount = RbCount / (uint32_t) DfGetNumberOfDiesOnSocket(Socket);

  return RbCount;
}
