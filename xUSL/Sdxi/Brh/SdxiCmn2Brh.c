/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

/**
 *  @file SdxiCmn2Brh.c
 *  @brief SDXI configuration routines for Breithorn
 */

#include <Nbio/NbioIp2Ip.h>
#include <Nbio/Brh/include/NbifEpFnCfgReg.h>
#include <Nbio/Brh/GnbRegistersBrh.h>
#include <Sdxi/Common/SdxiCmn2Rev.h>

/**************** Common-2-Rev Transfer Table for SDXI BRH ******************/
/*
 *  prototypes for forward references of functions used in the table
 */
void
FinalNbifRegConfigBrh (
  GNB_HANDLE             *GnbHandle,
  PCIe_PLATFORM_CONFIG   *Pcie
  );

/** ---------------------------- Table ---------------------------------
 * @details This is the internal common-2-Rev transfer table for SDXI BRH
 */
SDXI_COMMON_2_REV_XFER_BLOCK SdxiXferBrh = {
  .FinalNbifRegConfig = FinalNbifRegConfigBrh
};

/**--------------------------------------------------------------------
 * SdxiFinalRegisterInitBrh
 *
 * @brief Setup TPH Requester Controller on all PCIe ports
 *
 * @details This is a private IP function not visible to the host.
 *          It initializes the NBIFEPFNCFG register(s) and is
 *          called by NBIO as part of the Pcie topology configuration.
 *
 * @param[in]       GnbHandle         Pointer to the Silicon Descriptor for this node
 *
 * @returns Nothing
 */
static
void
SdxiFinalRegisterInitBrh (
  GNB_HANDLE             *GnbHandle
  )
{
  if (GnbHandle->RBIndex < 4) {
    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, SIL_RESERVED_0857),
      (uint32_t) ~(SIL_RESERVED_0847 |
      SIL_RESERVED_0849),
      (0x1 << SIL_RESERVED_0848) |
      (0x2 << SIL_RESERVED_0850)
      );

    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, SIL_RESERVED_0860),
      (uint32_t) ~(SIL_RESERVED_0847 |
      SIL_RESERVED_0849),
      (0x1 << SIL_RESERVED_0848) |
      (0x2 << SIL_RESERVED_0850)
      );

    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, SIL_RESERVED_0862),
      (uint32_t) ~(SIL_RESERVED_0847 |
      SIL_RESERVED_0849),
      (0x1 << SIL_RESERVED_0848) |
      (0x2 << SIL_RESERVED_0850)
      );

    xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      NBIO_SPACE(GnbHandle, SIL_RESERVED_0863),
      (uint32_t) ~(SIL_RESERVED_0847 |
      SIL_RESERVED_0849),
      (0x1 << SIL_RESERVED_0848) |
      (0x2 << SIL_RESERVED_0850)
      );
  }
}

/**--------------------------------------------------------------------
 * FinalNbifRegConfigBrh
 *
 * @brief     Breithorn specific configuration for SdxiConfig function in Sdxi.c
 *
 * @details   Breithorn specific final configuration for intilizing NBIFEPFNCFG register(s)
 *
 * @param[in] GnbHandle         Pointer to the Silicon Descriptor for this node
 * @param[in] Pcie              Not used
 *
 * @returns   Nothing
 *
 */
void
FinalNbifRegConfigBrh (
  GNB_HANDLE             *GnbHandle,
  PCIe_PLATFORM_CONFIG   *Pcie
  )
{
  UNUSED(Pcie); // Pcie is not used in BRH

  while (GnbHandle != NULL) {
    // Final Configuration. Initialize NBIFEPFNCFG register(s)
    SdxiFinalRegisterInitBrh(GnbHandle);
    GnbHandle = GnbGetNextHandle(GnbHandle);
  }
}
