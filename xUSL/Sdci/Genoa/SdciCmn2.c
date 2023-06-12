/**
 *  @file SdciCmn2.c
 *  @brief SDCI configuration routines
 */

/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <Mpio/MpioIp2Ip.h>
#include <NBIO/IOD/include/PcierccfgReg.h>
#include <NBIO/IOD/include/Nbifepf0cfgReg.h>
#include <NBIO/IOD/include/NbifmmReg.h>
#include <NBIO/IOD/GnbRegisters.h>
#include <NBIO/NbioIp2Ip.h>
#include <CommonLib/SmnAccess.h>
#include <Sdci/Common/Sdci.h>
#include <Sdci/Common/SdciCmn2Rev.h>
#include <Mpio/Genoa/MpioPcieStraps.h>

/**************** Common-2-Rev Transfer Table for SDCI Genoa ******************/
/*
 *  prototypes for forward references of functions used in the table
 */
void
SdciFinalRegisterInit (
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  );

void
CmdaPcieRootPortConfigWrapper (
  PCIe_WRAPPER_CONFIG               *Wrapper,
  void                              *Buffer,
  GNB_HANDLE                        *GnbHandle
  );

void
InitMpdmaEngine (
  PCIe_PLATFORM_CONFIG       *Pcie
  );

/** ---------------------------- Table ---------------------------------
 * @details This is the internal common-2-Rev transfer table for SDCI Genoa
 */
SDCI_COMMON_2_REV_XFER_BLOCK SdciXfer = {
  .SdciFinalRegisterInit           = SdciFinalRegisterInit,
  .CmdaConfigPcieRootPortWrapper   = CmdaPcieRootPortConfigWrapper,
  .InitMpdmaEngine                 = InitMpdmaEngine
};

/**--------------------------------------------------------------------
 * SdciFinalRegisterInit
 *
 * @brief Setup TPH Requester Controller on all PCIe ports
 *
 * @details This is a private IP function not visible to the host.
 *          It initializes the NBIFEPFNCFG register(s) and is
 *          called by NBIO as part of the Pcie topology configuration.
 *
 * @param[in]       Engine          Pointer to engine config descriptor
 * @param[in, out]  Buffer          Not used
 * @param[in]       Pcie            Pointer to global PCIe configuration
 *
 * @returns Nothing
 */
void
SdciFinalRegisterInit (
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  GNB_HANDLE            *GnbHandle;
  PCIe_WRAPPER_CONFIG   *Wrapper;
  NBIO_IP2IP_API        *NbioIp2Ip;

  if (SilGetIp2IpApi (SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    SDCI_TRACEPOINT (SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }
  GnbHandle = (GNB_HANDLE *) (NbioIp2Ip->PcieConfigGetParent (DESCRIPTOR_SILICON, &(Engine->Header)));
  Wrapper = (PCIe_WRAPPER_CONFIG *) NbioIp2Ip->PcieConfigGetParent (DESCRIPTOR_ALL_WRAPPERS, &(Engine->Header));

  SDCI_TRACEPOINT (SIL_TRACE_INFO, " Enter for RB %d Wrapper %d Port %d\n",
    GnbHandle->RBIndex,
    Wrapper->WrapId,
    Engine->Type.Port.PortId);

  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                   GnbHandle->Address.Address.Bus,
                   PORT_SPACE(GnbHandle,
                              Wrapper,
                              Engine->Type.Port.PortId,
                              SIL_RESERVED2_1302),
                   (uint32_t) ~(SIL_RESERVED2_1076 |
                               SIL_RESERVED2_1078),
                   (0x1 << SIL_RESERVED2_1077) |
                   (0x2 << SIL_RESERVED2_1079)
                   );
  return;
}

/**--------------------------------------------------------------------
 * CmdaPcieRootPortConfigWrapper
 *
 * @brief Per-Wrapper for CMDA initialization
 *
 * @details tbd
 *
 * @param[in]     Wrapper   Wrapper configuration info
 * @param[in,out] Buffer    Buffer pointer
 * @param[in]     GnbHandle GnbHandle Pointer
 *
 * @returns Nothing
 */
void
CmdaPcieRootPortConfigWrapper (
  PCIe_WRAPPER_CONFIG               *Wrapper,
  void                              *Buffer,
  GNB_HANDLE                        *GnbHandle
  )
{
  DEVICE_CAP2_PCIERCCFG_STRUCT    TphCplrValue;
  MPIO_IP2IP_API                  *MpioApi;
  SIL_STATUS                      Status;

  Status = SilGetIp2IpApi (SilId_MpioClass, (void **)&MpioApi);
  if (Status != SilPass) {
    return;
  }

  MpioApi->MpioWritePcieStrap (GnbHandle, SIL_RESERVED_893, 1, Wrapper->WrapId);

  TphCplrValue.Value = xUSLSmnRead (
    GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, SIL_RESERVED_501));

  SDCI_TRACEPOINT (SIL_TRACE_INFO, "TPH_CPLR_SUPPORTED = 0x%x\n", TphCplrValue.Field.TPH_CPLR_SUPPORTED);

  return;
}

/**--------------------------------------------------------------------
 * InitMpdmaEngine
 *
 * @brief Initialize MPDMA engine
 *
 * @details This function expose TPH Requester Capability for MPDMA,
 *          and set up the TPH modes and ST Table locations. This
 *          function is specific to Genoa and not part of the common code.
 *
 * @param[in] Pcie Platform Config
 *
 * @returns Nothing
 */
void
InitMpdmaEngine (
  PCIe_PLATFORM_CONFIG       *Pcie
  )
{
  GNB_HANDLE                        *GnbHandle;
  uint8_t                             Index;
  NBIO_IP2IP_API                    *NbioIp2Ip;

  SDCI_TRACEPOINT (SIL_TRACE_INFO, "InitMpdmaEngine\n");

  if (SilGetIp2IpApi (SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    SDCI_TRACEPOINT (SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }
  GnbHandle = NbioIp2Ip->NbioGetHandle (Pcie);

  while (GnbHandle != NULL) {
    if ((GnbHandle->RBIndex & 1) == 0) {
      xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                       GnbHandle->Address.Address.Bus,
                       NBIO_SPACE(GnbHandle, SIL_RESERVED2_1337),
                       (uint32_t)~(SIL_RESERVED2_1151 |
                                SIL_RESERVED2_1157 |
                                SIL_RESERVED2_1155),
                       (0x1 << SIL_RESERVED2_1153) |
                       (0x2 << SIL_RESERVED2_1158) |
                       (0x1 << SIL_RESERVED2_1156)
                       );
      xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                       GnbHandle->Address.Address.Bus,
                       NBIO_SPACE(GnbHandle, SIL_RESERVED2_1389),
                       (uint32_t)~(SIL_RESERVED2_1151 |
                                 SIL_RESERVED2_1157 |
                                 SIL_RESERVED2_1155),
                       (0x1 << SIL_RESERVED2_1153) |
                       (0x2 << SIL_RESERVED2_1158) |
                       (0x1 << SIL_RESERVED2_1156)
                       );
      xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                       GnbHandle->Address.Address.Bus,
                       NBIO_SPACE(GnbHandle, SIL_RESERVED2_1432),
                       (uint32_t)~(SIL_RESERVED2_1151 |
                                 SIL_RESERVED2_1157 |
                                 SIL_RESERVED2_1155),
                       (0x1 << SIL_RESERVED2_1153) |
                       (0x2 << SIL_RESERVED2_1158) |
                       (0x1 << SIL_RESERVED2_1156)
                       );

      xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                       GnbHandle->Address.Address.Bus,
                       NBIO_SPACE(GnbHandle, SIL_RESERVED2_1331),
                       (uint32_t) ~(SIL_RESERVED2_1120 |
                                  SIL_RESERVED2_1122),
                       (0x0 << SIL_RESERVED2_1121) |
                       (0x8 << SIL_RESERVED2_1123)
                       );
      xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                       GnbHandle->Address.Address.Bus,
                       NBIO_SPACE(GnbHandle, SIL_RESERVED2_1383),
                       (uint32_t) ~(SIL_RESERVED2_1120 |
                                  SIL_RESERVED2_1122),
                       (0x0 << SIL_RESERVED2_1121) |
                       (0x8 << SIL_RESERVED2_1123)
                       );
      xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                       GnbHandle->Address.Address.Bus,
                       NBIO_SPACE(GnbHandle, SIL_RESERVED2_1428),
                       (uint32_t) ~(SIL_RESERVED2_1120 |
                                  SIL_RESERVED2_1122),
                       (0x0 << SIL_RESERVED2_1121) |
                       (0x8 << SIL_RESERVED2_1123)
                       );

      xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                       GnbHandle->Address.Address.Bus,
                       NBIO_SPACE(GnbHandle, SIL_RESERVED2_1333),
                       (uint32_t) ~SIL_RESERVED2_1136,
                       ((uint32_t)0x1 << SIL_RESERVED2_1137)
                       );
      xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                       GnbHandle->Address.Address.Bus,
                       NBIO_SPACE(GnbHandle, SIL_RESERVED2_1385),
                       (uint32_t) ~SIL_RESERVED2_1136,
                       ((uint32_t)0x1 << SIL_RESERVED2_1137)
                       );
      xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                       GnbHandle->Address.Address.Bus,
                      NBIO_SPACE(GnbHandle, SIL_RESERVED2_1429),
                      (uint32_t) ~SIL_RESERVED2_1136,
                      ((uint32_t)0x1 << SIL_RESERVED2_1137)
                      );

      xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                       GnbHandle->Address.Address.Bus,
                       NBIO_SPACE(GnbHandle, SIL_RESERVED2_1334),
                       (uint32_t) ~(SIL_RESERVED2_1138 |
                                  SIL_RESERVED2_1142 |
                                  SIL_RESERVED2_1140),
                       (0x1 << SIL_RESERVED2_1139) |
                       (0x8 << SIL_RESERVED2_1143) |
                       (0x2 << SIL_RESERVED2_1141)
                       );
      xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                       GnbHandle->Address.Address.Bus,
                       NBIO_SPACE(GnbHandle, SIL_RESERVED2_1386),
                       (uint32_t) ~(SIL_RESERVED2_1138 |
                                  SIL_RESERVED2_1142 |
                                  SIL_RESERVED2_1140),
                       (0x1 << SIL_RESERVED2_1139) |
                       (0x8 << SIL_RESERVED2_1143) |
                       (0x2 << SIL_RESERVED2_1141)
                       );
      xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                       GnbHandle->Address.Address.Bus,
                       NBIO_SPACE(GnbHandle, SIL_RESERVED2_1430),
                       (uint32_t) ~(SIL_RESERVED2_1138 |
                                  SIL_RESERVED2_1142 |
                                  SIL_RESERVED2_1140),
                       (0x1 << SIL_RESERVED2_1139) |
                       (0x8 << SIL_RESERVED2_1143) |
                       (0x2 << SIL_RESERVED2_1141)
                       );

      for(Index = 0; Index  < 4; Index++) {
      SDCI_TRACEPOINT (SIL_TRACE_INFO, "Index = 0x%x, STRAP7_ADDR = 0x%x\n",
      Index, SIL_RESERVED2_1343 + ((Index*2) << 8));

        xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                         GnbHandle->Address.Address.Bus,
                         NBIO_SPACE(GnbHandle, SIL_RESERVED2_1343 + ((Index*2) << 8)),
                         (uint32_t) ~(SIL_RESERVED2_1175 |
                                     SIL_RESERVED2_1173 |
                                     SIL_RESERVED2_1170),
                         (0x2 << SIL_RESERVED2_1176) |
                         (0x1 << SIL_RESERVED2_1174) |
                         (0x1 << SIL_RESERVED2_1171)
                         );
        xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                         GnbHandle->Address.Address.Bus,
                         NBIO_SPACE(GnbHandle, SIL_RESERVED2_1395 + (Index << 8)),
                         (uint32_t) ~(SIL_RESERVED2_1175 |
                                     SIL_RESERVED2_1173 |
                                     SIL_RESERVED2_1170),
                         (0x2 << SIL_RESERVED2_1176) |
                         (0x1 << SIL_RESERVED2_1174) |
                         (0x1 << SIL_RESERVED2_1171)
                         );
        xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                         GnbHandle->Address.Address.Bus,
                         NBIO_SPACE(GnbHandle, SIL_RESERVED2_1436 + (Index << 8)),
                         (uint32_t) ~(SIL_RESERVED2_1175 |
                                     SIL_RESERVED2_1173 |
                                     SIL_RESERVED2_1170),
                         (0x2 << SIL_RESERVED2_1176) |
                         (0x1 << SIL_RESERVED2_1174) |
                         (0x1 << SIL_RESERVED2_1171)
                         );

        xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                         GnbHandle->Address.Address.Bus,
                         NBIO_SPACE(GnbHandle, SIL_RESERVED2_1341 + (Index << 8)),
                         (uint32_t) ~(SIL_RESERVED2_1163 |
                                     SIL_RESERVED2_1165),
                         (0x1 << SIL_RESERVED2_1164) |
                         (0x8 << SIL_RESERVED2_1166)
                         );
        xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                         GnbHandle->Address.Address.Bus,
                         NBIO_SPACE(GnbHandle, SIL_RESERVED2_1393 + (Index << 8)),
                         (uint32_t) ~(SIL_RESERVED2_1163 |
                                     SIL_RESERVED2_1165),
                         (0x1 << SIL_RESERVED2_1164) |
                         (0x8 << SIL_RESERVED2_1166)
                         );
        xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                         GnbHandle->Address.Address.Bus,
                         NBIO_SPACE(GnbHandle, SIL_RESERVED2_1434 + (Index << 8)),
                         (uint32_t) ~(SIL_RESERVED2_1163 |
                                     SIL_RESERVED2_1165),
                         (0x1 << SIL_RESERVED2_1164) |
                         (0x8 << SIL_RESERVED2_1166)
                         );
      }
    }
    GnbHandle = GnbGetNextHandle (GnbHandle);
  }
  return;
}
