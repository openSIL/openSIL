/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

/**
 *  @file CxlCmn2.c
 *  @brief This file contains the Genoa specific code used
 *         by the common CXL code, in the Cxl/Common folder, to implement
 *         functions that differ for each silicon revision. The common
 *         code uses the Cmn-2-Rev transfer table to access the routines.
 *         This file declares the Genoa transfer table and defines the Genoa
 *         specific functions.
 */

#include <SilCommon.h>
#include <string.h>
#include <Mpio/MpioIp2Ip.h>
#include <Mpio/Common/MpioStructs.h>
#include <NBIO/NbioIp2Ip.h>
#include <NBIO/IOD/include/IohcReg.h>
#include <NBIO/IOD/include/NbioBaseReg.h>
#include <NBIO/IOD/GnbRegisters.h>
#include <RcMgr/RcMgrIp2Ip.h>
#include <Cxl/Common/CxlInit.h>
#include <Cxl/CxlClass-api.h>
#include "CxlCmn2Rev.h"


#define CXLCLASS_MAJOR_REV   0
#define CXLCLASS_MINOR_REV   1
#define CXLCLASS_INSTANCE    0

/// CXL Message List - This enumerator defines the messages supported by the CXL firmware
typedef enum {
  MPIO_MSG_CXL_INITIALIZE = 0x0D,   ///< 0x0D - Initialize the list of CXL devices for MPIO firmware
} CXL_MESSAGES;

/**************** Common-2-Rev Transfer Table for SDCI Genoa ******************/
/*
 *  prototypes for forward references of functions used in the table
 */
void
CxlAssignResources (
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  );

void
CxlFindPorts (
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  );

void
CxlDevListGenerate (
  PCIe_PLATFORM_CONFIG  *Pcie,
  CXL_INFO_LIST     *CxlInfo
  );

/** ---------------------------- Table ---------------------------------
 * @details This is the internal common-2-Rev transfer table for SDCI Genoa
 */
CXL_COMMON_2_REV_XFER_BLOCK CxlXfer = {
  .CxlAssignResources    = CxlAssignResources,
  .CxlFindPorts          = CxlFindPorts,
  .CxlDevListGenerate    = CxlDevListGenerate
};

/**--------------------------------------------------------------------
 * CxlAssignBus
 *
 * @brief Assign bus number for a CXL integrated endpoint
 *
 * @details tbd
 *
 * @param [in] DxRcrb    Pointer to Downstream RCRB
 * @param [in] Resources CXL_BUS_LIMITS structure
 * @param [in] GnbHandle SILICON_DESCRIPTOR pointer
 *
 * @returns Which bus is assigned
 * @retval 32-bit bus value
 */
static
uint32_t
CxlAssignBus (
  uint32_t               *DsRcrb,
  CXL_BUS_LIMITS       *Resources,
  GNB_HANDLE           *GnbHandle
)
{
  uint32_t          WhichBus;
  uint32_t          Value;
  SIL_RESERVED_2    NextLastBus;



  /*
   * Assign a secondary bus in the DS Port
   */
  WhichBus = Resources->BusLimits[GnbHandle->InstanceId];
  Resources->BusLimits[GnbHandle->InstanceId]--;


  /*
   * Allocate X (1 should be enough?) bus numbers at the top of the available range
   */
  Value = CxlRegisterRead (DsRcrb, PCICFG_SPACE_PRIMARY_BUS_OFFSET);
  Value &= 0xFF000000;
  Value |= (((WhichBus) << 8) + ((WhichBus) << 16));
  CxlRegisterWrite (DsRcrb, PCICFG_SPACE_PRIMARY_BUS_OFFSET, Value);

  xUSLSmnReadModifyWrite (
    GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, SIL_RESERVED_16),
    (uint32_t) ~(RCEC_CFG_HWINIT_WR_LOCK_MASK),
    0
    );

  NextLastBus.Field.NEXT_BUS = WhichBus;  // secondary bus
  NextLastBus.Field.LAST_BUS = (uint32_t) GnbHandle->BusNumberLimit; // subordinate bus

  xUSLSmnWrite (
    GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, SIL_RESERVED_15),
    ((uint32_t) ((uintptr_t)(&NextLastBus)))
    );

  xUSLSmnReadModifyWrite (
    GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, SIL_RESERVED_16),
    (uint32_t) ~(RCEC_CFG_HWINIT_WR_LOCK_MASK),
    (1 << SIL_RESERVED_11)
    );

  return (WhichBus);
}

/**--------------------------------------------------------------------
 * CxlReportToMpio
 *
 * @brief Generate a list of CXL devices to report to MPIO
 *
 * @details tbd
 *
 * @param[in]       Engine          Pointer to engine config descriptor
 * @param[in, out]  Buffer          CXL_INFO_LIST pointer
 * @param[in]       Pcie            Pointer to global PCIe configuration
 *
 * @returns Nothing
 * @retval Nothing
 */
static
void
CxlReportToMpio (
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  CXL_INFO_LIST         *CxlInfoList;
  CXL_DEVICE_INFO       *ThisDevice;
  GNB_HANDLE            *GnbHandle;
  NBIO_IP2IP_API        *NbioIp2Ip;

  if (SilGetIp2IpApi (SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {

    return;
  }
  GnbHandle = (GNB_HANDLE *) (NbioIp2Ip->PcieConfigGetParent (DESCRIPTOR_SILICON, &(Engine->Header)));
  CxlInfoList = (CXL_INFO_LIST *) Buffer;

  if (GnbHandle->SocketId == 0) {
    ThisDevice = &(CxlInfoList->CxlInfoS0[CxlInfoList->CxlPortCountS0]);
    CxlInfoList->CxlPortCountS0++;
  } else {
    ThisDevice = &(CxlInfoList->CxlInfoS1[CxlInfoList->CxlPortCountS1]);
    CxlInfoList->CxlPortCountS1++;
  }
  ThisDevice->function = Engine->Type.Cxl.Address.Address.Function;
  ThisDevice->device = Engine->Type.Cxl.Address.Address.Device;
  ThisDevice->bus = Engine->Type.Cxl.Address.Address.Bus;
  ThisDevice->iohc_id = GnbHandle->RBIndex;
  ThisDevice->pcie_port = Engine->Type.Cxl.PortId;
  ThisDevice->cxl_type = Engine->Type.Cxl.CxlDeviceType;
  ThisDevice->unused = 0;
  return;
}

/**--------------------------------------------------------------------
 * CxlAssignResources
 *
 * @brief Assign CXL resources
 *
 * @details tbd
 *
 * @param[in]       Engine          Pointer to engine config descriptor
 * @param[in, out]  Buffer          CXL_BUS_LIMITS pointer
 * @param[in]       Pcie            Pointer to global PCIe configuration
 *
 * @returns Nothing
 * @retval Nothing
 */
void
CxlAssignResources (
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  PCIe_WRAPPER_CONFIG                          *Wrapper;
  GNB_HANDLE                                   *GnbHandle;
  uint32_t                                     Value;
  NB_PCIE0_PORTA_CXL_RCRB_BASE_ADDR_LO_STRUCT  BarLow;
  NB_PCIE0_PORTA_CXL_RCRB_BASE_ADDR_HI_STRUCT  BarHigh;
  uint32_t                                     *UsRcrb;
  uint32_t                                     *DsRcrb;
  CXL_BUS_LIMITS                               *Resources;
  uint32_t                                     WhichBus;
  size_t                                       Status;
  uint16_t                                     DvsecCapPtr;
  uint16_t                                     DvsecRegLocPtr;
  uint16_t                                     Value16;
  bool                                         Cxl2p0Mapping = false;
  MPIO_IP2IP_API                               *MpioApi;
  NBIO_IP2IP_API                               *NbioIp2Ip;



  if (SilGetIp2IpApi (SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {

    return;
  }
  GnbHandle = (GNB_HANDLE *) (NbioIp2Ip->PcieConfigGetParent (DESCRIPTOR_SILICON, &(Engine->Header)));
  Wrapper = (PCIe_WRAPPER_CONFIG *) NbioIp2Ip->PcieConfigGetParent (DESCRIPTOR_ALL_WRAPPERS, &(Engine->Header));

  if (SilGetIp2IpApi (SilId_MpioClass, (void **)&MpioApi) != SilPass) {
    return;
  }

  CXL_TRACEPOINT (SIL_TRACE_INFO, " RB %d, Wrapper %d Port %d\n",
    GnbHandle->RBIndex,
    Wrapper->WrapId,
    Engine->Type.Port.PortId
    );

  Resources = (CXL_BUS_LIMITS *) Buffer;

  Status = CxlMmioAlloc (GnbHandle, &BarLow.Value, &BarHigh.Value, 8 * 1024, BELOW_4GIG);
  if (Status != SilPass) {
    return;
  }

  DsRcrb = (uint32_t *) ((uintptr_t)(BarLow.Value));
  UsRcrb = (uint32_t *) ((uintptr_t)(BarLow.Value + SIL_RESERVED_14));

  Engine->Type.Cxl.DsRcrb = (uint32_t) ((uintptr_t)DsRcrb);
  Engine->Type.Cxl.UsRcrb = (uint32_t) ((uintptr_t)UsRcrb);

  xUSLSmnWrite (
    GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE (GnbHandle,
      (SMN_IOHUB0NBIO0_NB_PCIE0_PORTA_CXL_RCRB_BASE_ADDR_HI_ADDRESS) + (Engine->Type.Port.PortId * 8)),
    ((uint32_t)((uintptr_t)(&BarHigh)))
    );

  BarLow.Field.PCIE0_PORTA_CXL_RCRB_ENABLE = 1;
  xUSLSmnWrite (
    GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE (GnbHandle,
      (SMN_IOHUB0NBIO0_NB_PCIE0_PORTA_CXL_RCRB_BASE_ADDR_LO_ADDRESS) + (Engine->Type.Port.PortId * 8)),
    ((uint32_t)((uintptr_t)(&BarLow)))
    );

  /*
   * MEM enable and BME
   */
  CxlRegisterWrite (DsRcrb, PCICFG_SPACE_COMMAND_OFFSET, 0x6);

  /*
   * Assign secondary bus for CXL.io
   */
  WhichBus = CxlAssignBus (DsRcrb, Resources, GnbHandle);

  /*
   * Get PCIe DVSEC for CXL device
   */
  DvsecCapPtr = CxlGetDvsec ((WhichBus << 20), DVSEC_VID, DVSEC_VID2, DVSEC_ID);

  if(DvsecCapPtr != 0) {
    /*
	 * Read Upstream Port Capability Register (offset 0) (latches address)
	 */
    Value = CxlRegisterRead (UsRcrb, 0);

    /*
	 * If the data is 0xFFFFFFFF, check if this is a 2.0 device
	 */
    if (Value == 0xFFFFFFFF) {
      /*
	   * Get DVSEC Locator
	   */
      DvsecRegLocPtr = CxlGetDvsec((WhichBus << 20), DVSEC_VID2, DVSEC_VID2, DVSEC_ID_REG_LOCATOR);
      if(DvsecRegLocPtr != 0) {
        Cxl2p0Mapping = true;
        Engine->Type.Cxl.UsRcrb = 0;

      }
    }

    if ((Value != 0xFFFFFFFF) || Cxl2p0Mapping) {
      Engine->Type.Cxl.Address.AddressValue = 0;
      Engine->Type.Cxl.Address.Address.Bus = WhichBus;

      xUSLPciRead (MAKE_SBDFO (GnbHandle->Address.Address.Segment, WhichBus, 0, 0, (
        DvsecCapPtr + DVSEC_CXL_CAP_OFFSET)),
        AccessWidth16,
        &Value16
        );
      /*
	   * Type 2 not supproted
	   */
      assert ((Value16 & 0x5) != 0x5);
      if ((Value16 & 0x1) != 0) {
        Engine->Type.Cxl.CxlDeviceType = 1;
      }
      if ((Value16 & 0x4) != 0) {
        Engine->Type.Cxl.CxlDeviceType = 3;
      }

      /*
	   * Allocate and assign BARs for downstream allocations
	   */
      AllocateForCxlIo (Engine, MAKE_SBDFO (GnbHandle->Address.Address.Segment, WhichBus, 0, 0, 0));

      /*
	   * Assign MEMBAR0 in DS Port
	   */
      Status = CxlMmioAlloc (GnbHandle, &BarLow.Value, &BarHigh.Value, MEM_BAR0_SIZE, ABOVE_4GIG);
      if (Status != SilPass) {
        return;
      }
      CxlRegisterWrite (DsRcrb, PCICFG_SPACE_BAR0_OFFSET, BarLow.Value);
      CxlRegisterWrite (DsRcrb, PCICFG_SPACE_BAR0_OFFSET + 4, BarHigh.Value);
    }
  }


  return;
}

/**--------------------------------------------------------------------
 * CxlFindPorts
 *
 * @brief Determine if a port has a CXL connection
 *
 * @details tbd
 *
 * @param [in] Engine Pointer to engine descriptor
 * @param [in] Buffer Unused buffer pointer
 * @param [in] Pcie   Pointer to platform descriptor
 *
 * @returns Nothing
 * @retval Nothing
 */
void
CxlFindPorts (
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  )
{

  CXLCLASS_DATA_BLK                            *SilData;
  PCIe_WRAPPER_CONFIG                          *Wrapper;
  GNB_HANDLE                                   *GnbHandle;
  SIL_RESERVED_4                               LinkStatus;
  uint32_t                                     Value;
  SIL_STATUS                                   Status;
  SOC_LOGICAL_ID                               LogicalId;
  MPIO_IP2IP_API                               *MpioApi;
  NBIO_IP2IP_API                               *NbioIp2Ip;



  Status = GetCoreLogicalIdOnCurrentCore ((CORE_LOGICAL_ID*)&LogicalId);
  /*
   * Get IP block data
   */
  SilData = (CXLCLASS_DATA_BLK *)SilFindStructure(SilId_CxlClass,  0);
  assert (SilData != NULL);

  Status = SilGetIp2IpApi (SilId_MpioClass, (void **)&MpioApi);
  if (Status != SilPass) {
    return;
  }
  if (SilGetIp2IpApi (SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {

    return;
  }

  if (Engine->InitStatus == INIT_STATUS_PCIE_TRAINING_SUCCESS) {
    GnbHandle = (GNB_HANDLE *) (NbioIp2Ip->PcieConfigGetParent (DESCRIPTOR_SILICON, &(Engine->Header)));
    Wrapper = (PCIe_WRAPPER_CONFIG *) NbioIp2Ip->PcieConfigGetParent (DESCRIPTOR_ALL_WRAPPERS, &(Engine->Header));

    CXL_TRACEPOINT (SIL_TRACE_INFO, " Enter for RB %d Wrapper %d Port %d\n",
      GnbHandle->RBIndex,
      Wrapper->WrapId,
      Engine->Type.Port.PortId
      );

    LinkStatus.Value = xUSLSmnRead (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE (GnbHandle,
                  Wrapper,
                  (Engine->Type.Port.PortId),
                  SIL_RESERVED_509)
      );

    if (Engine->Type.Port.CxlControl != 0) {
      /*
	   * CXL report errors to RCEC
	   */
      if(!SilData->CxlInputBlock.ReportErrorsToRcec)
      {
          Value = 0;
          /*
		   * Default after reset (bits 0-8) are set.
		   */
          xUSLSmnWrite (
            GnbHandle->Address.Address.Segment,
            GnbHandle->Address.Address.Bus,
            NBIO_SPACE(GnbHandle, SMN_NB_CXL_CFG1_ADDRESS),
            ((uint32_t)((uintptr_t)(&Value))));
      }

      /*
	   * Cxl.Io ArbWeights
	   */
      if(SilData->CxlInputBlock.CxlIoArbWeights != 0)
      {
          xUSLSmnReadModifyWrite (
            GnbHandle->Address.Address.Segment,
            GnbHandle->Address.Address.Bus,
            PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
            SIL_RESERVED_510),
            (uint32_t) ~(SIL_RESERVED_237),
            (SilData->CxlInputBlock.CxlIoArbWeights << SIL_RESERVED_238)
            );

          xUSLSmnReadModifyWrite (
            GnbHandle->Address.Address.Segment,
            GnbHandle->Address.Address.Bus,
            PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
            SIL_RESERVED_510),
            (uint32_t) ~(SIL_RESERVED_233),
            (1 << SIL_RESERVED_234)
            );
      }

      /*
	   * Cxl.camem ArbWeights
	   */
      if(SilData->CxlInputBlock.CxlCaMemArbWeights != 0)
      {
          xUSLSmnReadModifyWrite (
            GnbHandle->Address.Address.Segment,
            GnbHandle->Address.Address.Bus,
            PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
            SIL_RESERVED_510),
            (uint32_t) ~(SIL_RESERVED_235),
            (SilData->CxlInputBlock.CxlCaMemArbWeights << SIL_RESERVED_236)
            );

          xUSLSmnReadModifyWrite (
            GnbHandle->Address.Address.Segment,
            GnbHandle->Address.Address.Bus,
            PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
            SIL_RESERVED_510),
            (uint32_t) ~(SIL_RESERVED_233),
            (1 << SIL_RESERVED_234)
            );
      }

      /*
	   * CNLI Token Advertisement
	   */
      if(SilData->CxlInputBlock.CnliTokenAdvertisement != 0)
      {
          xUSLSmnReadModifyWrite (
            GnbHandle->Address.Address.Segment,
            GnbHandle->Address.Address.Bus,
            PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
            SIL_RESERVED_510),
            (uint32_t) ~(SIL_RESERVED_436),
            (SilData->CxlInputBlock.CnliTokenAdvertisement << SIL_RESERVED_437)
            );

          xUSLSmnReadModifyWrite (
            GnbHandle->Address.Address.Segment,
            GnbHandle->Address.Address.Bus,
            PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
            SIL_RESERVED_510),
            (uint32_t) ~(SIL_RESERVED_438),
            (1 << SIL_RESERVED_439)
            );
      }

#ifdef ENABLE_CFL_FIND_PORTS_CALLBACK_1
      Status = PcieGetLogicalId(&LogicalId);
#else
    Status = SilUnsupported;
#endif //ENABLE_CFL_FIND_PORTS_CALLBACK_1

      if (Status != SilPass) {
        /*
		 * B0 or above
		 */
        if(!((LogicalId.Revision & (AMD_REV_F19_GENOA_AX)) && (LogicalId.Family & AMD_FAMILY_GENOA))) {
          /*
		   * Camem Rx Optimization
		   */
          xUSLSmnReadModifyWrite (
            GnbHandle->Address.Address.Segment,
            GnbHandle->Address.Address.Bus,
            PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
            SIL_RESERVED_511),
            (uint32_t) ~(SIL_RESERVED_239),
            (((SilData->CxlInputBlock.CxlCamemRxOptimization)? 1 : 0 ) <<
            SIL_RESERVED_240)
            );
          xUSLSmnReadModifyWrite (
            GnbHandle->Address.Address.Segment,
            GnbHandle->Address.Address.Bus,
            PORT_SPACE (GnbHandle, Wrapper, (Engine->Type.Port.PortId),
            SIL_RESERVED_19),
            (uint32_t) ~(PCIE_LC_ARBMUX_CNTL9_LC_TX_DIRECT_OUT_EN_MASK),
            (((SilData->CxlInputBlock.CxlTxOptimizeDirectOutEn)? 1 : 0 ) <<
            SIL_RESERVED_22)
            );
        }
      }
    }

    CXL_TRACEPOINT (
      SIL_TRACE_INFO,
      "  Alternate Protocol: %08x\n",
      LinkStatus.Field.LC_ALTERNATE_PROTOCOL_NEGOTIATION_STATUS
      );

    if (LinkStatus.Field.LC_ALTERNATE_PROTOCOL_NEGOTIATION_STATUS == 3) {

      PcieConfigResetDescriptorFlags (Engine, DESCRIPTOR_PCIE_ENGINE);
      PcieConfigSetDescriptorFlags (Engine, DESCRIPTOR_CXL_ENGINE | DESCRIPTOR_ALLOCATED);
      Engine->InitStatus = INIT_STATUS_PCIE_TRAINING_SUCCESS;
      Engine->Type.Port.PortData.LinkHotplug = PcieHotplugDisabled;

      xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        CNLI_SPACE (GnbHandle, Engine, SIL_RESERVED_20),
        ~CnliCxlCorrErrStat0_Reserved_31_7_MASK,
        (1 << SIL_RESERVED_18) |
        (1 << SIL_RESERVED_12)
        );
      xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        CNLI_SPACE (GnbHandle, Engine, SIL_RESERVED_10),
        ~CnliCxlCorrErrMask0_Reserved_31_7_MASK,
        (1 << SIL_RESERVED_13)
        );
      xUSLSmnReadModifyWrite (
        GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        CNLI_SPACE (GnbHandle, Engine, SMN_CnliCxlUncorrErrMask0_ADDRESS),
        ~CnliCxlUncorrErrMask0_Reserved_31_12_MASK,
        (1 << SIL_RESERVED_17)
        );
      if (SilData->CxlInputBlock.AmdCxlProtocolErrorReporting == 2) {
        SilData->CxlInputBlock.AmdPcieAerReportMechanism = SilData->CxlOutputBlock.AmdPcieAerReportMechanism = 2;
      }

      MpioApi->MpioWritePcieStrap (
        GnbHandle,
        MpioApi->MpioGetPortStrapIndex(MpioApi->MpioGetDpcCapabilityStrap(), Engine->Type.Port.PortId),
        0,
        Wrapper->WrapId
      );
    }

  }


}

/**--------------------------------------------------------------------
 * CxlDevListGenerate
 *
 * @brief Generate a list of CXL devices for MPIO firmware
 *
 * @details tbd
 *
 * @param[in]  GnbHandle           Pointer to the Silicon Descriptor for this node
 * @param[in]  PortDevMap          Pointer to PortDevMap
 *
 * @returns Nothing
 * @retval Nothing
 */
void
CxlDevListGenerate (
  PCIe_PLATFORM_CONFIG  *Pcie,
  CXL_INFO_LIST     *CxlInfo
  )
{
  GNB_HANDLE        *GnbHandle;
  uint32_t          MpioArg[6];
  uint32_t          Response;
  SIL_STATUS        Status;
  MPIO_IP2IP_API    *MpioApi;
  NBIO_IP2IP_API    *NbioIp2Ip;



  Status = SilGetIp2IpApi (SilId_MpioClass, (void **)&MpioApi);
  if (Status != SilPass) {
    return;
  }

  if (SilGetIp2IpApi (SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {

    return;
  }

  /*
   * Generate a list of CXL devices for MPIO firmware
  */
  CxlInfo->CxlPortCountS0 = 0;
  CxlInfo->CxlPortCountS1 = 0;


  NbioIp2Ip->PcieConfigRunProcForAllEngines(
               DESCRIPTOR_CXL_ENGINE,
               CxlReportToMpio,
               &CxlInfo,
               Pcie
               );

  GnbHandle = NbioIp2Ip->NbioGetHandle (Pcie);
  if (CxlInfo->CxlPortCountS0 != 0) {

    memset (MpioArg, 0x00, sizeof(MpioArg));
    MpioArg[1] = (uint32_t) ((uintptr_t)&CxlInfo->CxlInfoS0);
    MpioArg[2] = CxlInfo->CxlPortCountS0;
    Response = MpioApi->MpioServiceRequest (NbioIp2Ip->GetHostPciAddress (GnbHandle),
                                            MPIO_MSG_CXL_INITIALIZE,
                                            MpioArg,
                                            0);

  }

  while (GnbHandle != NULL) {
    if (GnbHandle->SocketId != 0) {
      break;
    }
    GnbHandle = GnbGetNextHandle (GnbHandle);
  }

  if ((GnbHandle != NULL) && (CxlInfo->CxlPortCountS1 != 0)) {

    memset (MpioArg, 0x00, sizeof(MpioArg));
    MpioArg[1] = (uint32_t) ((uintptr_t)&CxlInfo->CxlInfoS1);
    MpioArg[2] = CxlInfo->CxlPortCountS1;
    Response = MpioApi->MpioServiceRequest (NbioIp2Ip->GetHostPciAddress (GnbHandle),
                                            MPIO_MSG_CXL_INITIALIZE,
                                            MpioArg,
                                            0);

  }


}
