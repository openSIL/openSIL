/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file CxlInitBrh.c
 * @brief CXL Brh specific transfer table and I2I API functions definitions
 *
 */

#include <SilCommon.h>
#include <string.h>
#include <CommonLib/SmnAccess.h>
#include <FCH/Common/FchCommon.h>
#include <Mpio/MpioIp2Ip.h>
#include <Mpio/Brh/MpioPcieStrapsBrh.h>
#include <Mpio/Common/MpioInitLib.h>
#include <Nbio/Brh/include/IohcReg.h>
#include <Nbio/Brh/include/NbioBaseReg.h>
#include <Nbio/Brh/include/PcieRcCfgReg.h>
#include <Nbio/Brh/include/GlobalRegB0.h>
#include <Nbio/Brh/GnbRegistersBrh.h>
#include <Cxl/Common/CxlInit.h>
#include <Cxl/CxlClass-api.h>
#include <Cxl/CxlIp2Ip.h>
#include "CxlCmn2Brh.h"
#include "CxlCmn2Rev.h"
#include "CxlBrh.h"
#include "CxlIp2IpBrh.h"
#include "CxlRegBrh.h"

typedef struct {
  uint8_t   field0;
  uint8_t   field1;
} SIL_RESERVED_STRUCT_0006;

static const SIL_RESERVED_STRUCT_0006 CxlLanesConfig1 [] = {
  {0, 15},
  {48, 63},
};

static const SIL_RESERVED_STRUCT_0006 CxlLanesConfig2 [] = {
  {0, 7 },
  {8, 15},
  {48, 55},
  {56, 63},
};

static const SIL_RESERVED_STRUCT_0006 CxlLanesConfig3 [] = {
  {0, 15},
  {16, 31},
  {32, 47},
  {48, 63},
};

static const SIL_RESERVED_STRUCT_0006 CxlLanesConfig4 [] = {
  {0, 7},
  {16, 23},
  {32, 39},
  {48, 55},
};

/**--------------------------------------------------------------------
 * CxlAssignBusBrh
 * @brief Assign bus number for a CXL integrated endpoint
 *
 *  @param      DxRcrb    Pointer to Downstream RCRB
 *  @param      Resources CXL_BUS_LIMITS structure
 *  @param      GnbHandle SILICON_DESCRIPTOR pointer
 *
 *  @return     uint32_t    Bus assigned to endpoint
 *
 **/
static
uint32_t
CxlAssignBusBrh (
  uint32_t               *DsRcrb,
  CXL_BUS_LIMITS         *Resources,
  GNB_HANDLE             *GnbHandle
  )
{
  uint32_t                      WhichBus;
  uint32_t                      Value;
  SIL_RESERVED_UNION_0034    NextLastBus;

  /*
   * Assign a secondary bus in the DS Port
   */
  WhichBus = Resources->BusLimits[GnbHandle->InstanceId];
  Resources->BusLimits[GnbHandle->InstanceId]--;
  CXL_TRACEPOINT(SIL_TRACE_INFO, "RCiEP Bus: %02x\n", WhichBus);

  /*
   * Allocate X (1 should be enough?) bus numbers at the top of the available range
   */
  Value = CxlRegisterRead(DsRcrb, PCICFG_SPACE_PRIMARY_BUS_OFFSET);
  Value &= 0xFF000000;
  Value |= (((WhichBus) << 8) + ((WhichBus) << 16));
  CxlRegisterWrite(DsRcrb, PCICFG_SPACE_PRIMARY_BUS_OFFSET, Value);

  /*
   *  Configure the RCEC with the bus range containing the CXL endpoint(s)
   */
  xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, SIL_RESERVED_0015),
    (uint32_t) ~(RCEC_CFG_HWINIT_WR_LOCK_MASK),
    0
    );

  NextLastBus.Field.field_bits_8_to_15 = WhichBus;                             // secondary bus
  NextLastBus.Field.field_bits_16_to_23 = (uint32_t) GnbHandle->BusNumberLimit; // subordinate bus

  xUSLSmnWrite(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, SIL_RESERVED_0014),
    ((uint32_t) ((uintptr_t)(&NextLastBus)))
    );

  xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    NBIO_SPACE(GnbHandle, SIL_RESERVED_0015),
    (uint32_t) ~(RCEC_CFG_HWINIT_WR_LOCK_MASK),
    (1 << SIL_RESERVED_0016)
    );

  return (WhichBus);
}

/**--------------------------------------------------------------------
 * CxlReportToMpioBrh
 *
 * @brief Generate a list of CXL devices to report to MPIO
 *
 * @details tbd
 *
 * @param  Engine          Pointer to engine config descriptor
 * @param  Buffer          CXL_INFO_LIST pointer
 * @param  Pcie            Pointer to global PCIe configuration
 *
 * @returns Nothing
 * @retval Nothing
 **/
static
void
CxlReportToMpioBrh (
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  CXL_INFO_LIST           *CxlInfoList;
  CXL_DEVICE_INFO         *ThisDevice;
  GNB_HANDLE              *GnbHandle;
  uint32_t                TempBus;
  uint16_t                DvsecCapPtr;
  uint16_t                CxlCapabilityValue;
  uint32_t                WriteValue;
  uint32_t                ReadValue;
  PCI_ADDR                RootPortAddress;
  NBIO_IP2IP_API          *NbioIp2Ip;
  PCIE_DEVICE_TYPE        PcieDeviceType;
  uint32_t                Value32;

  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    CXL_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }
  GnbHandle = (GNB_HANDLE *) (NbioIp2Ip->PcieConfigGetParent(DESCRIPTOR_SILICON, &(Engine->Header)));
  CxlInfoList = (CXL_INFO_LIST *) Buffer;

  if (GnbHandle->SocketId == 0) {
    ThisDevice = &(CxlInfoList->CxlInfoS0[CxlInfoList->CxlPortCountS0]);
    CxlInfoList->CxlPortCountS0++;
    CXL_TRACEPOINT(SIL_TRACE_INFO, "CxlInfoList->CxlPortCountS0: %d \n", CxlInfoList->CxlPortCountS0);
  } else {
    ThisDevice = &(CxlInfoList->CxlInfoS1[CxlInfoList->CxlPortCountS1]);
    CxlInfoList->CxlPortCountS1++;
    CXL_TRACEPOINT(SIL_TRACE_INFO, "CxlInfoList->CxlPortCountS1: %d \n", CxlInfoList->CxlPortCountS1);
  }
  ThisDevice->function = 0;
  ThisDevice->device = 0;
  ThisDevice->bus = 0;
  ThisDevice->iohc_id = GnbHandle->RBIndex;
  ThisDevice->pcie_port = Engine->Type.Cxl.PortId;
  ThisDevice->cxl_type = Engine->Type.Cxl.CxlDeviceType;
  ThisDevice->cxl_ver = Engine->Type.Cxl.CxlVersion;

  if (ThisDevice->cxl_type == 0) {
    TempBus = GnbHandle->Address.Address.Bus + 1; // Assign a temporary bus to get the CXL 2.0 device type
    RootPortAddress.Address.Bus = GnbHandle->Address.Address.Bus;
    RootPortAddress.Address.Segment = GnbHandle->Address.Address.Segment;
    RootPortAddress.Address.Device = 1;
    RootPortAddress.Address.Function = 1;
    RootPortAddress.Address.Register = GnbHandle->Address.Address.Register;

    ReadValue = xUSLPciRead32(RootPortAddress.AddressValue | PCICFG_SPACE_PRIMARY_BUS_OFFSET);
    WriteValue = (TempBus << 16) | (TempBus << 8) | GnbHandle->Address.Address.Bus | GnbHandle->Address.Address.Segment;
    xUSLPciWrite32(RootPortAddress.AddressValue | PCICFG_SPACE_PRIMARY_BUS_OFFSET, WriteValue);
    DvsecCapPtr = CxlGetDvsec((TempBus << 20), DVSEC_VID, DVSEC_VID2, DVSEC_ID);

    if (DvsecCapPtr != 0) {
      CxlCapabilityValue = xUSLPciRead16(MAKE_SBDFO(GnbHandle->Address.Address.Segment,
        TempBus,
        0,
        0,
        (DvsecCapPtr + DVSEC_CXL_CAP_OFFSET)
        )
        );

      switch (CxlCapabilityValue & 0x0005) {
      case 0x0001: Engine->Type.Cxl.CxlDeviceType = 1;
        break;
      case 0x0004: Engine->Type.Cxl.CxlDeviceType = 3;
        break;
      case 0x0005: Engine->Type.Cxl.CxlDeviceType = 2;
        break;
      }

      ThisDevice->cxl_type = Engine->Type.Cxl.CxlDeviceType;
    }

    // Is this a switch?
    RootPortAddress.Address.Bus = TempBus;
    RootPortAddress.Address.Segment = GnbHandle->Address.Address.Segment;
    RootPortAddress.Address.Device = 0;
    RootPortAddress.Address.Function = 0;
    RootPortAddress.Address.Register = 0;

    PcieDeviceType = SilGnbLibGetPcieDeviceType(RootPortAddress, NULL);
    if (PcieDeviceUpstreamPort == PcieDeviceType) {
      Value32 = xUSLPciRead32(RootPortAddress.AddressValue + SIL_RESERVED_0012);
      if ((Value32 & 0xFFFF0000) == SIL_RESERVED_0018) {
        ThisDevice->cxl_type = 3; // Assuming Type 3
      }
    }

    // Restore the config space
    RootPortAddress.Address.Bus = GnbHandle->Address.Address.Bus;
    RootPortAddress.Address.Segment = GnbHandle->Address.Address.Segment;
    RootPortAddress.Address.Device = 1;
    RootPortAddress.Address.Function = 1;
    RootPortAddress.Address.Register = GnbHandle->Address.Address.Register;
    xUSLPciWrite32(RootPortAddress.AddressValue | PCICFG_SPACE_PRIMARY_BUS_OFFSET, ReadValue);
  }

  return;
}

/**--------------------------------------------------------------------
 * CxlAssignResourcesBrh
 *
 * @brief Assign CXL resources
 *
 * @details
 *
 * @param       Engine          Pointer to engine config descriptor
 * @param       Buffer          CXL_BUS_LIMITS pointer
 * @param       Pcie            Pointer to global PCIe configuration
 *
 * @returns Nothing
 * @retval Nothing
 **/
void
CxlAssignResourcesBrh (
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  PCIe_WRAPPER_CONFIG                            *Wrapper;
  GNB_HANDLE                                     *GnbHandle;
  uint32_t                                       Value;
  uint32_t                                       *UsRcrb;
  uint32_t                                       *DsRcrb;
  CXL_BUS_LIMITS                                 *Resources;
  uint32_t                                       WhichBus;
  size_t                                         Status;
  uint16_t                                       DvsecCapPtr;
  uint16_t                                       Value16;
  uint16_t                                       CxlCapabilityValue;
  SIL_RESERVED_UNION_0008                           RcrbBase;
  SIL_RESERVED_UNION_0007                      RcrbBaseHigh;
  uint32_t                                       RcrbBaseLow;
  MPIO_IP2IP_API                                 *MpioApi;
  NBIO_IP2IP_API                                 *NbioIp2Ip;

  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    CXL_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }
  if (SilGetIp2IpApi(SilId_MpioClass, (void **)&MpioApi) != SilPass) {
    CXL_TRACEPOINT(SIL_TRACE_ERROR, " MPIO API is not found.\n");
    return;
  }
  GnbHandle = (GNB_HANDLE *) (NbioIp2Ip->PcieConfigGetParent(DESCRIPTOR_SILICON, &(Engine->Header)));
  Wrapper = (PCIe_WRAPPER_CONFIG *) NbioIp2Ip->PcieConfigGetParent(DESCRIPTOR_ALL_WRAPPERS, &(Engine->Header));

  CXL_TRACEPOINT(SIL_TRACE_INFO,
    " RB %d, Wrapper %d, Port %d\n",
    GnbHandle->RBIndex,
    Wrapper->WrapId,
    Engine->Type.Port.PortId
    );

  Engine->Type.Cxl.ReportToMpioinDxe = true;

  MpioApi->MpioWritePcieStrap(GnbHandle,
    MpioApi->MpioGetPortStrapIndex(MpioApi->MpioGetMembar0SizeStrap (), Engine->Type.Port.PortId),
    0x10,
    Wrapper->WrapId
    );
  MpioApi->MpioWritePcieStrap(GnbHandle, MpioApi->MpioGetCxlModeStrap (), 0xF, Wrapper->WrapId);
  GetMem32BarSize(Engine->Type.Port.Address.AddressValue + 0x10);
  if (Engine->Type.Cxl.CxlVersion == 2) {
    Engine->Type.Cxl.DsRcrb = 0;
    Engine->Type.Cxl.UsRcrb = 0;
    Engine->Type.Cxl.Mmio32Base = 0;
    Engine->Type.Cxl.Mmio32Size = 0;
    Engine->Type.Cxl.Mmio64Base = 0;
    Engine->Type.Cxl.Mmio64Size = 0;
    Engine->Type.Cxl.DsMemBar0 = 0;
    Engine->Type.Cxl.UsMemBar0 = 0;
    Engine->Type.Cxl.CxlDeviceType = 0;
    Engine->Type.Cxl.CxlIndex = GnbHandle->RBIndex;
    CXL_TRACEPOINT(SIL_TRACE_INFO, "CXL 2.0: Setting resources to zero and returning\n");
    return;
  }

  CXL_TRACEPOINT(SIL_TRACE_INFO, "CXL 1.1: Assign Resources\n");
  Resources = (CXL_BUS_LIMITS *) Buffer;

  /*
   * Allocate MMIO space and assign RCRB
   */
  Status = CxlMmioAlloc(GnbHandle, &RcrbBaseLow, &RcrbBaseHigh.Value, 8 * 1024, BELOW_4GIG);
  if (Status != SilPass) {
    return;
  }

  DsRcrb = (uint32_t *) ((uintptr_t)(RcrbBaseLow));
  UsRcrb = (uint32_t *) ((uintptr_t)(RcrbBaseLow + SIL_RESERVED_0019));

  Engine->Type.Cxl.DsRcrb = (uint32_t) ((uintptr_t)DsRcrb);
  Engine->Type.Cxl.UsRcrb = (uint32_t) ((uintptr_t)UsRcrb);

  RcrbBase.Field.field_bits_13_to_31 = (uint32_t) (RcrbBaseLow >> 13);
  RcrbBase.Field.field_bit_0 = 1;

  xUSLSmnWrite(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    PORT_SPACE(GnbHandle,
    Wrapper,
    (Engine->Type.Port.PortId),
    SIL_RESERVED_1612
    ),
    ((uint32_t)((uintptr_t)(&RcrbBaseHigh)))
    );

  xUSLSmnWrite(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    PORT_SPACE(GnbHandle,
    Wrapper,
    (Engine->Type.Port.PortId),
    SIL_RESERVED_1611
    ),
    ((uint32_t)((uintptr_t)(&RcrbBase)))
    );

  /*
   * MEM enable and BME
   */
  CxlRegisterWrite(DsRcrb, PCICFG_SPACE_COMMAND_OFFSET, 0x6);

  /*
   * Assign secondary bus for CXL.io
   */
  WhichBus = CxlAssignBusBrh(DsRcrb, Resources, GnbHandle);

  /*
   * Get PCIe DVSEC for CXL device
   */
  DvsecCapPtr = CxlGetDvsec((WhichBus << 20), DVSEC_VID, DVSEC_VID2, DVSEC_ID);

  if (DvsecCapPtr != 0) {
    /*
     * Read Upstream Port Capability Register (offset 0) (latches address)
     */
    Value = CxlRegisterRead(UsRcrb, 0);

    if (Value != 0xFFFFFFFF) {
      Engine->Type.Cxl.Address.AddressValue = 0;
      Engine->Type.Cxl.Address.Address.Bus = WhichBus;

      Value16 = xUSLPciRead16(MAKE_SBDFO(GnbHandle->Address.Address.Segment,
        WhichBus,
        0,
        0,
        (DvsecCapPtr + DVSEC_CXL_CAP_OFFSET)
        )
        );

      CxlCapabilityValue = Value16 & 0x0005;

      switch (CxlCapabilityValue) {
      case 0x0001: Engine->Type.Cxl.CxlDeviceType = 1;
        break;
      case 0x0004: Engine->Type.Cxl.CxlDeviceType = 3;
        break;
      case 0x0005: Engine->Type.Cxl.CxlDeviceType = 2;
        break;
      }

      /*
       * Allocate and assign BARs for downstream allocations
       */
      AllocateForCxlIo(Engine, MAKE_SBDFO(GnbHandle->Address.Address.Segment, WhichBus, 0, 0, 0));

      /*
       * Assign MEMBAR0 in DS Port
       */
      Status = CxlMmioAlloc(GnbHandle, &RcrbBaseLow, &RcrbBaseHigh.Value, MEM_BAR0_SIZE, ABOVE_4GIG);
      if (Status != SilPass) {
        return;
      }
      CxlRegisterWrite(DsRcrb, PCICFG_SPACE_BAR0_OFFSET, RcrbBaseLow);
      CxlRegisterWrite(DsRcrb, PCICFG_SPACE_BAR0_OFFSET + 4, RcrbBaseHigh.Value);

      CXL_TRACEPOINT(SIL_TRACE_INFO, "Downstream RCRB...\n");
      xUslDumpBuffer((void *) DsRcrb, 0x40, 3);
      CXL_TRACEPOINT(SIL_TRACE_INFO, "\n");
      CXL_TRACEPOINT(SIL_TRACE_INFO, "Upstream RCRB...\n");
      xUslDumpBuffer((void *) UsRcrb, 0x40, 3);
      CXL_TRACEPOINT(SIL_TRACE_INFO, "\n");
    }
  }

  return;
}

/**--------------------------------------------------------------------
 *  @brief Function call to determine if a port has a CXL connection
 *
 *  @param      Engine Pointer to engine descriptor
 *  @param      Buffer Unused buffer pointer
 *  @param      Pcie   Pointer to platform descriptor
 *  @return     VOID
 *
 **/
void
CxlFindPortsBrh (
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  PCIe_WRAPPER_CONFIG                          *Wrapper;
  CXLCLASS_DATA_BLK                            *SilData;
  GNB_HANDLE                                   *GnbHandle;
  SIL_RESERVED_UNION_0032                      LinkStatus;
  SIL_RESERVED_UNION_0013                      PortStatus;
  uint32_t                                     Value;
  MPIO_IP2IP_API                               *MpioApi;
  size_t                                       Status;
  /*
   * Get IP block data
   */
  SilData = (CXLCLASS_DATA_BLK *)xUslFindStructure(SilId_CxlClass, 0);
  assert(SilData != NULL);
  Status = SilGetIp2IpApi(SilId_MpioClass, (void **)&MpioApi);
  if (Status != SilPass) {
    return;
  }

  if (Engine->InitStatus == INIT_STATUS_PCIE_TRAINING_SUCCESS) {
    GnbHandle = (GNB_HANDLE *) PcieConfigGetParentSilicon(Engine);
    Wrapper = PcieConfigGetParentWrapper(Engine);

    CXL_TRACEPOINT(SIL_TRACE_INFO,
      "Enter for RB %d Wrapper %d Port %d\n",
      GnbHandle->RBIndex,
      Wrapper->WrapId,
      Engine->Type.Port.PortId
      );

    if (Engine->Type.Port.CxlControl != 0) {
      MpioApi->MpioSmnPrivateRegisterRead(GnbHandle,
        PORT_SPACE(GnbHandle,
        Wrapper,
        (Engine->Type.Port.PortId),
        SIL_RESERVED_1465
        ),
        &LinkStatus.Value
        );

      // CXL report errors to RCEC
      if ((SilData->CxlInputBlock.ReportErrorsToRcec) == 0) {
        Value = 0;
        //Default after reset (bits 0-8) are set.
        xUSLSmnWrite(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          NBIO_SPACE(GnbHandle, SMN_NB_CXL_CFG1_ADDRESS),
          Value
          );
      }


      // Sync header bypass
      xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
        GnbHandle->Address.Address.Bus,
        PORT_SPACE(GnbHandle,
        Wrapper,
        (Engine->Type.Port.PortId),
        SIL_RESERVED_1615
        ),
        (uint32_t) ~(SIL_RESERVED_1530),
        (SilData->CxlInputBlock.SyncHeaderByPass? 1 : 0) << SIL_RESERVED_1531
        );


      // Cxl.Io ArbWeights
      if (SilData->CxlInputBlock.CxlIoArbWeights != 0) {
        MpioApi->MpioSmnPrivateRegisterRMW(GnbHandle,
          PORT_SPACE(GnbHandle,
          Wrapper,
          (Engine->Type.Port.PortId),
          SIL_RESERVED_1467
          ),
          (uint32_t) ~(SIL_RESERVED_1310),
          (SilData->CxlInputBlock.CxlIoArbWeights << SIL_RESERVED_1311),
          0
          );

        MpioApi->MpioSmnPrivateRegisterRMW(GnbHandle,
          PORT_SPACE(GnbHandle,
          Wrapper,
          (Engine->Type.Port.PortId),
          SIL_RESERVED_1467
          ),
          (uint32_t) ~(SIL_RESERVED_1306),
          (1 << SIL_RESERVED_1307),
          0
          );
      }

      // Cxl.camem ArbWeights
      if (SilData->CxlInputBlock.CxlCaMemArbWeights != 0) {
        MpioApi->MpioSmnPrivateRegisterRMW(GnbHandle,
          PORT_SPACE(GnbHandle,
          Wrapper,
          (Engine->Type.Port.PortId),
          SIL_RESERVED_1467
          ),
          (uint32_t) ~(SIL_RESERVED_1308),
          (SilData->CxlInputBlock.CxlCaMemArbWeights << SIL_RESERVED_1309),
          0
          );

        MpioApi->MpioSmnPrivateRegisterRMW(GnbHandle,
          PORT_SPACE(GnbHandle,
          Wrapper,
          (Engine->Type.Port.PortId),
          SIL_RESERVED_1467
          ),
          (uint32_t) ~(SIL_RESERVED_1306),
          (1 << SIL_RESERVED_1307),
          0
          );
      }

      // CNLI Token Advertisement
      if (SilData->CxlInputBlock.CnliTokenAdvertisement != 0) {
        MpioApi->MpioSmnPrivateRegisterRMW(GnbHandle,
          PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_0510),
          (uint32_t) ~(SIL_RESERVED_0501),
          (SilData->CxlInputBlock.CnliTokenAdvertisement << SIL_RESERVED_0502),
          0
          );

        MpioApi->MpioSmnPrivateRegisterRMW(GnbHandle,
          PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_0510),
          (uint32_t) ~(SIL_RESERVED_0503),
          (1 << SIL_RESERVED_0504),
          0
          );
      }

      // Camem Rx Optimization
      MpioApi->MpioSmnPrivateRegisterRMW(GnbHandle,
        PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1466),
        (uint32_t) ~(SIL_RESERVED_1304),
        (SilData->CxlInputBlock.CxlCamemRxOptimization? 1 : 0 )
          << SIL_RESERVED_1305,
          0
        );

      // Tx Optimization [Direct Out Enable]
      MpioApi->MpioSmnPrivateRegisterRMW(GnbHandle,
        PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_0017),
        (uint32_t) ~(PCIE_LC_ARBMUX_CNTL9_LC_TX_DIRECT_OUT_EN_MASK),
        (SilData->CxlInputBlock.CxlTxOptimizeDirectOutEn? 1 : 0 ) << SIL_RESERVED_0013,
          0
        );

      CXL_TRACEPOINT(SIL_TRACE_INFO,
        "  Alternate Protocol: %08x\n",
        LinkStatus.Field.field_bits_12_to_13
        );

      if (LinkStatus.Field.field_bits_12_to_13 == 3) {
        PcieConfigResetDescriptorFlags(Engine, DESCRIPTOR_PCIE_ENGINE);
        PcieConfigSetDescriptorFlags(Engine, DESCRIPTOR_CXL_ENGINE | DESCRIPTOR_ALLOCATED);
        Engine->InitStatus = INIT_STATUS_PCIE_TRAINING_SUCCESS;

        // Force AER FwFirst if CxlPresent
        if (SilData->CxlInputBlock.AmdCxlProtocolErrorReporting == 2) {
          SilData->CxlInputBlock.AmdPcieAerReportMechanism = SilData->CxlOutputBlock.AmdPcieAerReportMechanism = 2;
        }

        // Disable DPC if CxlPresent
        MpioApi->MpioWritePcieStrap(GnbHandle,
          MpioApi->MpioGetPortStrapIndex(SIL_RESERVED_0423, Engine->Type.Port.PortId),
          0,
          Wrapper->WrapId
          );

        // Mask for poison indication from device on CXL.mem
        xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          CNLI_SPACE(GnbHandle, Engine, SIL_RSVD_ADDR_1F100064),
          ~(CnliCxlCorrErrMask0_Reserved_31_7_MASK),
          (1 << SIL_RESERVED_0010)
          );

        // Mask detection of poison from device
        xUSLSmnReadModifyWrite(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          CNLI_SPACE(GnbHandle, Engine, SMN_CnliCxlUncorrErrMask0_ADDRESS),
          ~(BITS_17_TO_31_MASK_0001),
          (1 << SIL_RESERVED_0011)
          );

        // Check if CXL port is 2.0
        Value = xUSLSmnRead(GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          PORT_SPACE(GnbHandle,
          Wrapper,
          (Engine->Type.Port.PortId),
          SIL_RESERVED_1615
          )
          );
        PortStatus.Value = (uint16_t) (Value >> 16);
        if (PortStatus.Field.field_bit_5) {
          Engine->Type.Cxl.CxlVersion = 2;  // CXL 2.0
          CXL_TRACEPOINT(SIL_TRACE_INFO, "  Found CXL 2.0 ");
        } else {
          Engine->Type.Cxl.CxlVersion = 1;  // CXL 1.1
          CXL_TRACEPOINT(SIL_TRACE_INFO, "  Found CXL 1.1 ");
        }
        CXL_TRACEPOINT(SIL_TRACE_INFO, "on this port %d\n", Engine->Type.Port.PortId);
      }
    }
  }
}

/**--------------------------------------------------------------------
 * CxlDevListGenerateBrh
 *
 * @brief Generate a list of CXL devices for MPIO firmware
 *
 * @details tbd
 *
 * @param  GnbHandle           Pointer to the Silicon Descriptor for this node
 * @param  PortDevMap          Pointer to PortDevMap
 *
 * @returns Nothing
 * @retval Nothing
 */
void
CxlDevListGenerateBrh (
  PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  GNB_HANDLE        *GnbHandle;
  uint32_t          MpioArg[6];
  uint32_t          Response;
  CXL_INFO_LIST     CxlInfo;
  SIL_STATUS        Status;
  MPIO_IP2IP_API    *MpioApi;
  NBIO_IP2IP_API    *NbioIp2Ip;

  CXL_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Status = SilGetIp2IpApi(SilId_MpioClass, (void **)&MpioApi);
  if (Status != SilPass) {
    return;
  }

  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    CXL_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  memset(&CxlInfo, 0x00, sizeof (CxlInfo));
  /*
   * Generate a list of CXL devices for MPIO firmware
   */
  CxlInfo.CxlPortCountS0 = 0;
  CxlInfo.CxlPortCountS1 = 0;

  CXL_TRACEPOINT(SIL_TRACE_INFO, "Assign Resources\n");
  NbioIp2Ip->PcieConfigRunProcForAllEngines(DESCRIPTOR_CXL_ENGINE,
    CxlReportToMpioBrh,
    &CxlInfo,
    Pcie
    );

  GnbHandle = NbioIp2Ip->NbioGetHandle(Pcie);

  CXL_TRACEPOINT(SIL_TRACE_INFO, "%d \n", CxlInfo.CxlPortCountS0);
  if (CxlInfo.CxlPortCountS0 != 0) {
    CXL_TRACEPOINT(SIL_TRACE_INFO, "Sending this data to socket 0\n");
    xUslDumpBuffer((void *) &CxlInfo.CxlInfoS0, (CxlInfo.CxlPortCountS0 << 2), 3);
    CXL_TRACEPOINT(SIL_TRACE_INFO, "\n");
    memset(MpioArg, 0x00, sizeof (MpioArg));
    MpioArg[1] = (uint32_t) ((uintptr_t)&CxlInfo.CxlInfoS0);
    MpioArg[2] = CxlInfo.CxlPortCountS0;
    Response = MpioApi->MpioServiceRequest(NbioIp2Ip->GetHostPciAddress(GnbHandle),
      BIOS_MPIO_MSG_CNLI_INITIALIZE,
      MpioArg,
      0
      );
    CXL_TRACEPOINT(SIL_TRACE_INFO, "  MPIO Response = 0x%x\n", Response);
  }

  while (GnbHandle != NULL) {
    if (GnbHandle->SocketId != 0) {
      break;
    }
    GnbHandle = GnbGetNextHandle(GnbHandle);
  }

  CXL_TRACEPOINT(SIL_TRACE_INFO, "%d \n", CxlInfo.CxlPortCountS1);
  if ((GnbHandle != NULL) && (CxlInfo.CxlPortCountS1 != 0)) {
    CXL_TRACEPOINT(SIL_TRACE_INFO, "Sending this data to socket 1\n");
    xUslDumpBuffer((void *) &CxlInfo.CxlInfoS1, (CxlInfo.CxlPortCountS1 << 2), 3);
    CXL_TRACEPOINT(SIL_TRACE_INFO, "\n");
    memset(MpioArg, 0x00, sizeof (MpioArg));
    MpioArg[1] = (uint32_t) ((uintptr_t)&CxlInfo.CxlInfoS1);
    MpioArg[2] = CxlInfo.CxlPortCountS1;
    Response = MpioApi->MpioServiceRequest(NbioIp2Ip->GetHostPciAddress(GnbHandle),
      BIOS_MPIO_MSG_CNLI_INITIALIZE,
      MpioArg,
      0
      );
    CXL_TRACEPOINT(SIL_TRACE_INFO, "  MPIO Response = 0x%x\n", Response);
  }

  CXL_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**--------------------------------------------------------------------
 * CheckConfig
 *
 * @brief Get CXL early configuration
 *
 * @details Reads the scratch pad to get the CXL early configuration which would have been set in ABL
 *
 * @param  Pcie           Platform Configuration
 *
 * @returns Nothing
 * @retval Nothing
 */
static
uint32_t
CheckConfig (
  PCIe_PLATFORM_CONFIG    *Pcie
  )
{
  GNB_HANDLE        *GnbHandle;
  uint32_t          Value;
  NBIO_IP2IP_API    *NbioIp2Ip;

  CXL_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    CXL_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return SilNotFound;
  }

  GnbHandle = NbioIp2Ip->NbioGetHandle(Pcie);

  Value = xUSLSmnRead(GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    SIL_RSVD_ADDR_13B00074
    );

  CXL_TRACEPOINT(SIL_TRACE_INFO, " Read CXL Config Value%x\n", Value);

  CXL_TRACEPOINT(SIL_TRACE_EXIT, "\n");

  return Value;
}

/**--------------------------------------------------------------------
 * RemoveCxlLinksFromTopologyBrh
 *
 * @brief Remove the CXL links trained early in ABL from the complex
 *
 * @details tbd
 *
 * @param  CurrentComplexConfig           Complex Descriptor
 * @param  Pcie                           Platform Configuration
 *
 * @returns Nothing
 * @retval Nothing
 */
void
RemoveCxlLinksFromTopologyBrh (
  MPIO_COMPLEX_DESCRIPTOR **CurrentComplexConfig,
  PCIe_PLATFORM_CONFIG    *Pcie
  )
{
  MPIO_COMPLEX_DESCRIPTOR   *ComplexDescriptor;
  uint8_t                   PortIndex;
  uint8_t                   Index;
  uint32_t                  Config;
  uint8_t                   NumEntries = 0;
  SIL_RESERVED_STRUCT_0006                  *CxlLanes;
  uint8_t                   Startlane = 0;

  CXL_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if ((CurrentComplexConfig == NULL) || Pcie == NULL) {
    CXL_TRACEPOINT(SIL_TRACE_INFO, "Exit with invalid input values\n");
    return;
  }

  Config = CheckConfig(Pcie);
  if (Config == 0) {
    CXL_TRACEPOINT(SIL_TRACE_INFO, "Invalid CXL Configuration\n");
    return;
  }

  switch (Config) {
  case 1:
    CxlLanes = (SIL_RESERVED_STRUCT_0006 *) CxlLanesConfig1;
    NumEntries = 2;
    break;
  case 2:
    CxlLanes = (SIL_RESERVED_STRUCT_0006 *) CxlLanesConfig2;
    NumEntries = 4;
    break;
  case 3:
    CxlLanes = (SIL_RESERVED_STRUCT_0006 *) CxlLanesConfig3;
    NumEntries = 4;
    break;
  case 4:
    CxlLanes = (SIL_RESERVED_STRUCT_0006 *) CxlLanesConfig4;
    NumEntries = 4;
  }

  ComplexDescriptor = *CurrentComplexConfig;

  while (ComplexDescriptor != NULL) {
    PortIndex = 0;
    while ((ComplexDescriptor->PciePortList[PortIndex].Flags & DESCRIPTOR_TERMINATE_LIST) == 0) {
      for (Index = 0 ; Index < NumEntries; Index++) {
        if (ComplexDescriptor->PciePortList[PortIndex].EngineData.StartLane >
          ComplexDescriptor->PciePortList[PortIndex].EngineData.EndLane) {
          Startlane = ComplexDescriptor->PciePortList[PortIndex].EngineData.EndLane;
        } else {
          Startlane = ComplexDescriptor->PciePortList[PortIndex].EngineData.StartLane;
        }
        if (CxlLanes[Index].field0 == Startlane) {
          ComplexDescriptor->PciePortList[PortIndex].EngineData.EngineType = PcieUnusedEngine;
          break;
        }
      }
      PortIndex++;
    }

    //Check for the last entry terminated by DESCRIPTOR_TERMINATE_LIST flag, from where the above loop terminated.
    if ((ComplexDescriptor->PciePortList[PortIndex].Flags & DESCRIPTOR_TERMINATE_LIST) != 0) {
      for (Index = 0; Index < NumEntries; Index++) {
        if (ComplexDescriptor->PciePortList[PortIndex].EngineData.StartLane >
          ComplexDescriptor->PciePortList[PortIndex].EngineData.EndLane) {
          Startlane = ComplexDescriptor->PciePortList[PortIndex].EngineData.EndLane;
        } else {
          Startlane = ComplexDescriptor->PciePortList[PortIndex].EngineData.StartLane;
        }
        if (CxlLanes[Index].field0 == Startlane) {
          ComplexDescriptor->PciePortList[PortIndex].EngineData.EngineType = PcieUnusedEngine;
          break;
        }
      }
    }
    ComplexDescriptor = PcieConfigGetNextDataDescriptor(ComplexDescriptor);
  }
}

/**
 * Callback to get CXL Link Speed information
 *
 * @param[in]       Engine          Pointer to engine config descriptor
 * @param[in, out]  Buffer          CXL_BUS_LIMITS pointer
 * @param[in]       Pcie            Pointer to global PCIe configuration
 *
 */
static void
FindCxlLinkSpeedCallback (
  PCIe_ENGINE_CONFIG    *Engine,
  void                  *Buffer,
  PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  CXL_LINK_SPEED_INFO      *CxlLinkSpeed;
  PCIe_WRAPPER_CONFIG      *Wrapper;
  GNB_HANDLE               *GnbHandle;
  uint8_t                  LinkSpeed;
  uint32_t                   Value32;

  GnbHandle = (GNB_HANDLE *) PcieConfigGetParentSilicon (Engine);
  if (GnbHandle == NULL) {
    return;
  }

  Wrapper = PcieConfigGetParentWrapper(Engine);
  CxlLinkSpeed = (CXL_LINK_SPEED_INFO  *) Buffer;

  Value32 = xUSLSmnRead (
              GnbHandle->Address.Address.Segment,
              GnbHandle->Address.Address.Bus,
              PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1619)
              );

  LinkSpeed = (uint8_t) (Value32 >> 16 & 0xf);
  CXL_TRACEPOINT (SIL_TRACE_INFO, "CXL Link Status, Segment = 0x%x Bus = 0x%x LinkStatus = 0x%x\n",
                  GnbHandle->Address.Address.Segment, GnbHandle->Address.Address.Bus,
                  LinkSpeed);

  CxlLinkSpeed->CxlPresent = 1;
  if (LinkSpeed == 5) {
    CxlLinkSpeed->CxlSpeedGen5 = 1;
  }

  return;
}

void
GetCxlLinkSpeedBrh (
  PCIe_PLATFORM_CONFIG  *Pcie,
  uint32_t              *CxlMsgBuffer
  )
{
  NBIO_IP2IP_API    *NbioIp2Ip;

  CXL_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    CXL_TRACEPOINT(SIL_TRACE_ERROR, " NBIO API is not found.\n");
    return;
  }

  NbioIp2Ip->PcieConfigRunProcForAllEngines (
      DESCRIPTOR_CXL_ENGINE,
      FindCxlLinkSpeedCallback,
      (void *) CxlMsgBuffer,
      Pcie
      );

  return;
}

/**
 * Trigger warm reset if hot plug slot CXL is not detected in PEI.
 *
 * @param  GnbHandle  Pointer to GnbHandle
 * @param  Engine     Pointer to engine config descriptor
 *
 * @return SIL_STATUS
 * @retval SilPass                  If no action required
 * @retval SilResetRequestWarmImm   To trigger warm reset for early CXL discovery
 */
SIL_STATUS
CxlHotPlugSlotResetBrh (
  GNB_HANDLE            *GnbHandle,
  PCIe_ENGINE_CONFIG    *Engine
  )
{
  uint32_t                                  Value32;
  uint16_t                                  LinkStatus;
  bool                                     LinkTrained;
  uint8_t                                  DelayCount;
  SIL_RESERVED_UNION_0032                  CxlLinkStatus;
  PCIe_WRAPPER_CONFIG                      *Wrapper;
  CXLCLASS_DATA_BLK                        *SilData;
  MPIO_IP2IP_API                           *MpioApi;
  SIL_STATUS                               Status;

  Status = SilPass;
  CXL_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  Status = SilGetIp2IpApi(SilId_MpioClass, (void **)&MpioApi);
  if (Status != SilPass) {
    CXL_TRACEPOINT (SIL_TRACE_INFO, "MPIO APIs not found \n");
    return Status;
  }

  /*
   * Get IP block data
   */
  SilData = (CXLCLASS_DATA_BLK *)SilFindStructure (SilId_CxlClass,  0);
  if (SilData == NULL) {
    // Could not find the IP input block
    Status = SilNotFound;
    CXL_TRACEPOINT (SIL_TRACE_INFO, "CXL IP block not found \n");
    return Status;
  }

  DelayCount = 0;
  LinkTrained = false;

  DelayCount = SilData->CxlInputBlock.CxlHotPlugSlotTimeOut;
  if (DelayCount == 0) {
    // Do not need to check CXL on hot plug slot
    return SilPass;
  }

  if (PcieConfigIsCxlEngine(Engine)) {
    // Link was recognized as CXL at power on
    return SilPass;
  }

  Wrapper = PcieConfigGetParentWrapper(Engine);

  CXL_TRACEPOINT(SIL_TRACE_INFO,
    "Enter for RB %d Wrapper %d Port %d Segment %d Bus 0x%x Device %d Function %d\n",
    GnbHandle->RBIndex,
    Wrapper->WrapId,
    Engine->Type.Port.PortId,
    GnbHandle->Address.Address.Segment,
    GnbHandle->Address.Address.Bus,
    Engine->Type.Port.PortData.DeviceNumber,
    Engine->Type.Port.PortData.FunctionNumber
    );

  CXL_TRACEPOINT(SIL_TRACE_INFO, "Check Link Active ");
  do {
    // Check if link trained
    Value32 = xUSLSmnRead (
      GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      PORT_SPACE(GnbHandle, Wrapper, (Engine->Type.Port.PortId), SIL_RESERVED_1619)
      );

    LinkStatus = (uint16_t) ((Value32 >> 16) & BIT_16(13));
    if (LinkStatus == 0) {
      SilFchStall(1000000);  // 1 sec delay
    } else {
      LinkTrained = true;
      CXL_TRACEPOINT(SIL_TRACE_INFO, "\nLink active, LinkStatus = 0x%x\n", LinkStatus);
      break;
    }
    CXL_TRACEPOINT(SIL_TRACE_RAW, "*");
  } while (--DelayCount);    // default 10 seconds timeout

  if (!LinkTrained) {
    CXL_TRACEPOINT(SIL_TRACE_INFO, "\nError: Link NOT trained!\n");
    return SilPass;
  }

  MpioApi->MpioSmnPrivateRegisterRead(GnbHandle,
    PORT_SPACE(GnbHandle,
    Wrapper,
    (Engine->Type.Port.PortId),
    SIL_RESERVED_1465
    ),
    &CxlLinkStatus.Value
    );

  CXL_TRACEPOINT(SIL_TRACE_INFO, "Alternate Protocol: %08x\n", CxlLinkStatus.Field.field_bits_12_to_13);

  if (CxlLinkStatus.Field.field_bits_12_to_13 == 3) {
    CXL_TRACEPOINT(SIL_TRACE_INFO, "CXL in hot plug Slot: Triggering Warm-Reset for early CXL discovery!\n");

    return SilResetRequestWarmImm;
  }

  return SilPass;
}
