/* Copyright 2023 Advanced Micro Devices, Inc. All rights reserved. */
// SPDX-License-Identifier: MIT
/**
 * @file  MultiFchInit.c
 * @brief Secondary FCH initialization functions
 *
 */

#include <SilCommon.h>
#include <string.h>
#include "MultiFchInit.h"
#include <DF/Common/SilFabricInfo.h>
#include <DF/Common/FabricRegisterAccCmn.h>
#include <DF/DfIp2Ip.h>
#include <CommonLib/Mmio.h>
#include <CommonLib/SmnAccess.h>
#include <FCH/FchSataIp2Ip.h>
#include <FCH/Common/MultiFch/MultiFchCmn2Rev.h>
#include <FCH/Common/FchCore/FchSata/FchSataReg.h>
#include <FCH/Common/FchCore/FchXhci/FchXhci.h>
#include <FCH/Common/FchCore/FchHwAcpi/FchHwAcpi.h>
#include <FCH/Common/FchCore/FchAb/FchAb.h>
#include <RcMgr/RcMgrIp2Ip.h>
#include <CommonLib/SmnAccess.h>
#include "FchReg.h"

#define DF_X86IOBASE_FUNC 0x00
#define DF_X86IOBASE_OFFSET 0xC0
#define ALIGN_64K 0x0FFFF
#define ACPI_MMIO_BASE 0xFED80000ul
#define PMIO_BASE 0x300 // DWORD
#define FCH_PMIOA_REGD6 0xD6 // AltMmioBase
#define FCH_PMIOA_REGD5 0xD5 // AltMmioEn
#define FCH_PMIOA_REGD3 0xD3 // SD
#define FCH_PMIOA_REGE8 0xE8
#define AOAC_BASE 0x1E00
#define FCH_HS_USB0_NBIF_STRAP0 0x10834600ul
#define FCH_HS_USB1_NBIF_STRAP0 0x10734600ul
#define AMD_VENDOR_ID  0x1022

extern uint32_t Xhci0DevRemovable;
extern uint32_t XhciUsb3PortDisable;
extern uint32_t XhciUsb2PortDisable;
extern uint64_t XhciUsb20OcPinSelect;
extern uint32_t XhciUsb31OcPinSelect;

FCH_MULITI_FCH_RESET_DATA_BLOCK gFchMfResetData;

/**
 * FchSecondaryFchMmioRelocate
 *
 * @brief TBD
 *
 * @param DieBusNum   The bus number of the secondary FCH
 * @param FchDataPtr  An input pointer to the FCH Class IP block data.
 * @return SIL_STATUS
 */
static
SIL_STATUS
FchSecondaryFchMmioRelocate (
  uint32_t            DieBusNum,
  FCHCLASS_INPUT_BLK  *FchDataPtr
  )
{
  SIL_STATUS                  Status;
  uint16_t                    AltMmioBase;
  uint64_t                    Length;
  uint64_t                    FchMmioBase;
  FABRIC_TARGET               MmioTarget;
  uint8_t                     RbNum;
  FABRIC_MMIO_ATTRIBUTE       Attributes;
  RCMGR_IP2IP_API             *RcMgrIp2Ip;
  MULTI_FCH_COMMON_2_REV_XFER_BLOCK *MultiFchC2R;

  Status = SilGetCommon2RevXferTable (SilId_MultiFchClass, (void**) &MultiFchC2R);
  if (Status != SilPass) {
    FCH_TRACEPOINT (SIL_TRACE_ERROR, "FCH common to revision transfer block not found!\n");
    assert (Status == SilPass);
    return Status;
  }

  Status = SilGetIp2IpApi (SilId_RcManager, (void **)(&RcMgrIp2Ip));
  if (Status != SilPass) {
    FCH_TRACEPOINT (SIL_TRACE_ERROR, " MMIO allocator API is not found.\n");
    assert (Status == SilPass);
    return Status;
  }
  Status = MultiFchC2R->GetSecFchRootBridgeNumber (&RbNum);
  if (Status == SilPass) {
    Length = 0x2000;
    MmioTarget.TgtType = TARGET_RB;
    MmioTarget.SocketNum = 1;         //Socket1

    MmioTarget.RbNum = RbNum;
    MmioTarget.PciBusNum = 0;
    Attributes.ReadEnable = 1;
    Attributes.WriteEnable = 1;
    Attributes.NonPosted = 0;
    Attributes.MmioType = NON_PCI_DEVICE_BELOW_4G;

    Status = RcMgrIp2Ip->FabricReserveMmio (&FchMmioBase, &Length, ALIGN_64K, MmioTarget, &Attributes);

    if (Status == SilPass) {
      AltMmioBase = (uint16_t)((FchMmioBase >> 16) & 0xFFFF);
      xUSLSmnReadModifyWrite8 (0,
                              DieBusNum,
                              FCH_SN_SMN_PMIO_BASE + FCH_PMIO_ALTMMIOBASE,
                              0x00,
                              (uint8_t)AltMmioBase
                              );
      xUSLSmnReadModifyWrite8 (0,
                              DieBusNum,
                              FCH_SN_SMN_PMIO_BASE + (FCH_PMIO_ALTMMIOBASE + 1),
                              0x00,
                              (uint8_t)(AltMmioBase >> 8)
                              );
      xUSLSmnReadModifyWrite8 (0,
                              DieBusNum,
                              FCH_SN_SMN_PMIO_BASE + FCH_PMIO_ALTMMIOEN,
                              0xFC,
                              0x1
                              );
      FchDataPtr->FchAcpiMmioBase = (uint32_t)FchMmioBase;
    }
  }
  return Status;
}

/**
 * @brief FchSecondaryFchIORegInit - Secondary Fch IO register Initialization.
 *
 *
 * @param[in] DieBusNum Bus Number for Current Die
 * @param[in] FchData FCHCLASS_INPUT_BLK configuration structure pointer
 * @param[in] FchAb FCH_AB configuration structure pointer
 *
 * @retval None
 *
 */
static
void
FchSecondaryFchIORegInit (
  uint8_t DieBusNum,
  FCHCLASS_INPUT_BLK *FchData,
  FCHAB_INPUT_BLK *FchAbData
  )
{
  // Init AB registers
  FchSecondaryFchInitAB (DieBusNum, FchData, FchAbData);
}

/**
 * @brief FchSecondaryFchDisableSD - Secondary Fch IO register Initialization.
 *
 * @retval None
 *
 */
static
void
FchSecondaryFchDisableSD (
  FCHCLASS_INPUT_BLK  *FchData
)
{
  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  uint32_t FchAcpiMmio;

  FchAcpiMmio = FchData->FchAcpiMmioBase;

  xUSLMemReadModifyWrite8 ((void*) (size_t)(FchAcpiMmio + PMIO_BASE + FCH_PMIOA_REGD3), 0xBF, 0x00);
  xUSLMemReadModifyWrite8 ((void*) (size_t)(FchAcpiMmio + PMIO_BASE + FCH_PMIOA_REGE8), 0xFE, 0x00);
  xUSLMemReadModifyWrite8 ((void*) (size_t)(FchAcpiMmio + AOAC_BASE + 0x72), 0xF7, 0x00);

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
}

/**
 * @brief MultiFchInit - Multi Fch initialization.
 *
 * @retval SIL_STATUS
 *
 */
static
SIL_STATUS
MultiFchInit (
  FCHCLASS_INPUT_BLK  *FchData,
  FCHAB_INPUT_BLK     *FchAb,
  FCHUSB_INPUT_BLK    *FchUsb,
  FCHSATA_INPUT_BLK   *FchSata
  )
{
  uint32_t                          NumberOfProcessors = 0;
  uint32_t                          TotalNumberOfDie = 0;
  uint32_t                          TotalNumberOfRootBridges = 0;
  uint8_t                           FchBusNum;
  DF_IP2IP_API                      *DfIp2IpApi;
  FCHSATA_IP2IP_API                 *FchSataIp2IpApi;
  SIL_STATUS                        Status;
  uint32_t                          IoBaseOrg;
  uint32_t                          IoLimitOrg;
  MULTI_FCH_COMMON_2_REV_XFER_BLOCK *MultiFchC2R;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Status = SilGetCommon2RevXferTable (SilId_MultiFchClass, (void**)&MultiFchC2R);
  if (Status != SilPass) {
    FCH_TRACEPOINT (SIL_TRACE_ERROR, "FCH common to revision transfer block not found!\n");
    assert (Status == SilPass);
    return Status;
  }

  Status = SilGetIp2IpApi (SilId_DfClass, (void*) &DfIp2IpApi);
  if (Status != SilPass) {
    FCH_TRACEPOINT (SIL_TRACE_ERROR, "DF Ip-2-Ip API not found!\n");
    assert (Status == SilPass);
    return Status;
  }

  //Update DATA BLOCK
  memset (&gFchMfResetData, 0, sizeof (FCH_MULITI_FCH_RESET_DATA_BLOCK));
  gFchMfResetData.FchAcpiMmioBase[0] = FchData->FchAcpiMmioBase; // Update index 0 with primary FCH MMIO Base

  DfIp2IpApi->DfGetSystemInfo (&NumberOfProcessors, &TotalNumberOfDie, &TotalNumberOfRootBridges, NULL, NULL);
  FCH_TRACEPOINT(SIL_TRACE_INFO, "NumberOfProcessors=0x%x\n", NumberOfProcessors);
  FCH_TRACEPOINT(SIL_TRACE_INFO, "TotalNumberOfDie=0x%x\n", TotalNumberOfDie);
  FCH_TRACEPOINT(SIL_TRACE_INFO, "TotalNumberOfRootBridges=0x%x\n", TotalNumberOfRootBridges);

  //
  // Check 2nd socket IOD
  //
  if (NumberOfProcessors == 2) {
    Status = MultiFchC2R->GetSecFchBusNumber (&FchBusNum);

    FchSecondaryFchMmioRelocate (FchBusNum, FchData);

    //Update Global Data
    gFchMfResetData.FchAcpiMmioBase[1] = FchData->FchAcpiMmioBase;

    // Enable IO on Die to configure secondary FCH IO regs.
    Status = MultiFchC2R->EnableIoCxxOnDieN (&IoBaseOrg, &IoLimitOrg);

    //Secondary Fch init
    if (FchAb != NULL) {
      FchSecondaryFchIORegInit (FchBusNum, FchData, FchAb);
    }

    // Disable IO on Die to after secondary FCH IO regs config complete.
    Status = MultiFchC2R->DisableIoCxxOnDieN (IoBaseOrg, IoLimitOrg);

    FchSecondaryFchDisableSD (FchData);

    // SATA
    if (FchSata != NULL) {
      Status = SilGetIp2IpApi (SilId_FchSata, (void **)&FchSataIp2IpApi);
      if (Status != SilPass) {
        XPRF_TRACEPOINT (SIL_TRACE_ERROR, "FchSata Ip2Ip Api not found!\n");
        assert (Status == SilPass);
      } else {
        FchSataIp2IpApi->FchInitResetSataProgram ((uint32_t)FchBusNum, FchSata);
      }
    }
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * InitializeMultiFch
 * @brief This is an FCH IP private function, not visible to the Host
 *
 *
 */
SIL_STATUS InitializeMultiFch (void) {
  FCHCLASS_INPUT_BLK  *LclInpFchBlk; //pointer to input blk
  FCHAB_INPUT_BLK     *LclInpFchAbBlk; //pointer to Fch Ab input blk
  FCHUSB_INPUT_BLK    *LclInpFchUsbBlk; //pointer to Fch Usb input blk
  FCHSATA_INPUT_BLK   *LclInpFchSataBlk;
  SIL_STATUS          Status;

  LclInpFchBlk = (FCHCLASS_INPUT_BLK *) xUslFindStructure (SilId_FchClass, 0);
  if (LclInpFchBlk == NULL) {
    // Could not find the IP input block
    FCH_TRACEPOINT (SIL_TRACE_INFO, "openSIL FCH input block not found.\n");
    return SilNotFound;
  } else {
    FCH_TRACEPOINT (SIL_TRACE_INFO,
      "openSIL FCH input block found at: 0x%x \n",
      LclInpFchBlk
      );
  }

  LclInpFchAbBlk = (FCHAB_INPUT_BLK *) xUslFindStructure (SilId_FchAb, 0);
  if (LclInpFchAbBlk == NULL) {
    // Could not find the IP input block
    FCH_TRACEPOINT (SIL_TRACE_INFO, "FCH Ab Input block not found.\n");
  } else {
    FCH_TRACEPOINT (SIL_TRACE_INFO,
      "FCH Ab blk found at: 0x%x \n",
      LclInpFchAbBlk
      );
  }

  LclInpFchUsbBlk = (FCHUSB_INPUT_BLK *) xUslFindStructure (SilId_FchUsb, 0);
  if (LclInpFchUsbBlk == NULL) {
    // Could not find the IP input block
    FCH_TRACEPOINT (SIL_TRACE_INFO, "openSIL FCH Usb input block not found.\n");
  } else {
    FCH_TRACEPOINT (SIL_TRACE_INFO,
      "openSIL FCH Usb input block found at: 0x%x \n",
      LclInpFchUsbBlk
      );
  }

  LclInpFchSataBlk = (FCHSATA_INPUT_BLK *) xUslFindStructure (SilId_FchSata, 0);
  if (LclInpFchSataBlk == NULL) {
    // Could not find the IP input block
    FCH_TRACEPOINT (SIL_TRACE_INFO, "openSIL FCH Sata input block not found.\n");
  } else {
    FCH_TRACEPOINT (SIL_TRACE_INFO,
      "openSIL FCH Sata input block found at: 0x%x \n",
      LclInpFchSataBlk
      );
  }

  Status = MultiFchInit (
    LclInpFchBlk,
    LclInpFchAbBlk,
    LclInpFchUsbBlk,
    LclInpFchSataBlk
    );

  FCH_TRACEPOINT (SIL_TRACE_EXIT, "Exit.\n");
  return Status;

}
