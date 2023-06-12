/**
 * @file  NbioBaseInitGenoa.c
 * @brief This file contains API definition Nbio Base Initialization
 *
 */

/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <xSIM.h>
#include <CommonLib/SmnAccess.h>
#include <NbioSmnTable.h>
#include "GnbRegisters.h"
#include "NbioData.h"
#include "NbioCommon.h"
#include <NBIO/GnbDxio.h>
#include "NbioPcieTopologyHelper.h"
#include <NBIO/IOD/include/GlobalRegB0.h>
#include <NBIO/IOD/include/SyshubmmReg.h>
#include <NbioPcie.h>
#include <SilSocLogicalId.h>

extern const SMN_TABLE       NbioPprInitValues [];
extern const SMN_TABLE       GnbEarlyInitTableCommon [];
extern const SMN_TABLE       GnbnBifInitTable [];
extern const SMN_TABLE       GnbOncePerSocketInitMP [];
extern const SMN_TABLE       GnbSdpMuxInitTableCommon [];
extern NBIOCLASS_DATA  mNbioIpBlockData;
extern NBIO_CONFIG_DATA mNbioConfigDataDflts;

#define FUSE_TYPE_ROOTBRIDGE       1
#define FUSE_TYPE_PCIECONTROLLER   2

void NbifConfigureCommonOptions(GNB_HANDLE *GnbHandle);

/*----------------------------------------------------------------------------------------*/
/**
 * NbioBaseInitGenoa
 *
 * @brief  This function initializes the Nbif ports
 *
 * @retval SilPass              The Nbio base initialization is successful
 * @retval SilInvalidParameter  Error indicating invalid input arguements are passed
 *
 */

static
SIL_STATUS
NbioBaseInitGenoa(
         GNB_HANDLE          *GnbHandle
  )
{
  if (GnbHandle == NULL) {
    NBIO_TRACEPOINT (SIL_TRACE_INFO, "ERROR : Invalid input argument: GnbHandle\n");
    assert (false);
    return SilInvalidParameter;
  }

  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                GnbHandle->Address.Address.Bus,
                NBIO_SPACE(GnbHandle, SIL_RESERVED2_1360),
                (uint32_t) ~(SIL_RESERVED2_1255 |
                          SIL_RESERVED2_1251 |
                          SIL_RESERVED2_1253),
                (GnbHandle->Address.Address.Bus << SIL_RESERVED2_1256) |
                (0x7 << SIL_RESERVED2_1252) |
                (0x2 << SIL_RESERVED2_1254)
                );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                GnbHandle->Address.Address.Bus,
                NBIO_SPACE(GnbHandle, SIL_RESERVED2_1442),
                (uint32_t) ~(SIL_RESERVED2_1237 |
                          SIL_RESERVED2_1233 |
                          SIL_RESERVED2_1235),
                (GnbHandle->Address.Address.Bus << SIL_RESERVED2_1238) |
                (0 << SIL_RESERVED2_1234) |
                (0 << SIL_RESERVED2_1236)
                );

    xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                     GnbHandle->Address.Address.Bus,
                     NBIO_SPACE(GnbHandle, SIL_RESERVED2_1339),
                     (uint32_t) ~(SIL_RESERVED2_1159),
                     (0x0 << SIL_RESERVED2_1161)
                    );
    xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                     GnbHandle->Address.Address.Bus,
                     NBIO_SPACE(GnbHandle, SIL_RESERVED2_1318),
                     (uint32_t) ~(1 << (PCIE_FUNCTION2 + SIL_RESERVED2_1002)),
                     (0x0 << (PCIE_FUNCTION2 + SIL_RESERVED2_1002))
                    );
    xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                     GnbHandle->Address.Address.Bus,
                     NBIO_SPACE(GnbHandle, SIL_RESERVED2_1391),
                     (uint32_t) ~(SIL_RESERVED2_1159),
                     (0x0 << SIL_RESERVED2_1161)
                    );
    xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                     GnbHandle->Address.Address.Bus,
                     NBIO_SPACE(GnbHandle, SIL_RESERVED2_1372),
                     (uint32_t) ~(1 << (PCIE_FUNCTION2 + SIL_RESERVED2_1002)),
                     (0x0 << (PCIE_FUNCTION2 + SIL_RESERVED2_1002))
                    );

  if (mNbioIpBlockData.NbioConfigData->NbifMgcgHysteresis != 0) {
    xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                     GnbHandle->Address.Address.Bus,
                     NBIO_SPACE(GnbHandle, SIL_RESERVED2_1322),
                     (uint32_t) ~(SIL_RESERVED2_1066),
                     (mNbioIpBlockData.NbioConfigData->NbifMgcgHysteresis <<
                       SIL_RESERVED2_1067)
                    );
    xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                     GnbHandle->Address.Address.Bus,
                     NBIO_SPACE(GnbHandle, SIL_RESERVED2_1375),
                     (uint32_t) ~(SIL_RESERVED2_1066),
                     (mNbioIpBlockData.NbioConfigData->NbifMgcgHysteresis <<
                       SIL_RESERVED2_1067)
                    );
  }
  if (mNbioIpBlockData.NbioConfigData->SyshubMgcgHysteresis != 0) {
    xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                     GnbHandle->Address.Address.Bus,
                     NBIO_SPACE(GnbHandle, SIL_RESERVED2_1366),
                     (uint32_t) ~(SIL_RESERVED2_1459),
                     (mNbioIpBlockData.NbioConfigData->SyshubMgcgHysteresis <<
                       SIL_RESERVED2_1460)
                    );
    xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                     GnbHandle->Address.Address.Bus,
                     NBIO_SPACE(GnbHandle, SIL_RESERVED2_1418),
                     (uint32_t) ~(SIL_RESERVED2_1459),
                     (mNbioIpBlockData.NbioConfigData->SyshubMgcgHysteresis <<
                       SIL_RESERVED2_1460)
                    );
    xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                     GnbHandle->Address.Address.Bus,
                     NBIO_SPACE(GnbHandle, SIL_RESERVED2_1324),
                     (uint32_t) ~(SIL_RESERVED2_1082),
                     (mNbioIpBlockData.NbioConfigData->SyshubMgcgHysteresis <<
                       SIL_RESERVED2_1083)
                    );
    xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                     GnbHandle->Address.Address.Bus,
                     NBIO_SPACE(GnbHandle, SIL_RESERVED2_1377),
                     (uint32_t) ~(SIL_RESERVED2_1082),
                     (mNbioIpBlockData.NbioConfigData->SyshubMgcgHysteresis <<
                       SIL_RESERVED2_1083)
                    );
  }
  if (GnbHandle->RBIndex == 2) {
    xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                     GnbHandle->Address.Address.Bus,
                     NBIO_SPACE(GnbHandle, SIL_RESERVED2_1348),
                     (uint32_t) ~(SIL_RESERVED2_1191),
                     (1 << SIL_RESERVED2_1193)
                    );
  }

  return SilPass;
}


/**
 * IsSteppingAx
 *
 * @brief Checks for SOC stepping A0, A1, or AB
 *
 * @retval  true if Genoa AX stepping, false Otherwise
 */
static
bool
IsSteppingAx (void)
{
   SOC_LOGICAL_ID    LogicalId;
   SIL_STATUS        Status;

   Status = GetSocLogicalIdOnCurrentCore(&LogicalId);
    if (Status == SilPass) {
        if (LogicalId.Revision & (AMD_REV_F19_GENOA_AX))
        {
            return true;  //AX
        }
    }
    return false; //B0
}

/*----------------------------------------------------------------------------------------*/
/**
 * NbioBaseConfigurationGenoa
 *
 * @brief  This function pefroms early NBIO initialization.
 *
 * @retval SilPass              The Nbio base configuration is successful
 * @retval SilInvalidParameter  Error indicating invalid input arguements are passed
 *
 */
SIL_STATUS
NbioBaseConfigurationGenoa (void)
{
  uint32_t       Property;
  GNB_HANDLE     *GnbHandle;
  uint32_t       Value;
  NB_BUS_NUM_CNTL_STRUCT          BusNumCntl;

  NBIO_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  GnbHandle = NULL;

  GnbHandle = GetGnbHandle ();

  if (GnbHandle == NULL) {
    NBIO_TRACEPOINT (SIL_TRACE_INFO, "ERROR : Invalid GnbHandle\n");
    assert (false);
    return SilInvalidParameter;
  }

  while (GnbHandle != NULL) {
    // IOHC needs to know the segment it resides on for MCTP message routing.
    // Setting the segment and bus information and LAT_Mode is set to 1
    BusNumCntl.Field.NB_SEGMENT = GnbHandle->Address.Address.Segment;
    BusNumCntl.Field.NB_BUS_NUM = GnbHandle->Address.Address.Bus;
    BusNumCntl.Field.NB_BUS_LAT_Mode = 1;
    xUSLSmnWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SMN_IOHUB0NBIO0_NB_BUS_NUM_CNTL_ADDRESS),
                  BusNumCntl.Value
                  );
    // Copy SB_LOCATION to the L1 and L2 in IOMMU. This only applies to NBIO3, where the FCH is located
    if (GnbHandle->SbPresent == 1) {
      Value = xUSLSmnRead  (GnbHandle->Address.Address.Segment,
                            GnbHandle->Address.Address.Bus,
                            NBIO_SPACE(GnbHandle, SMN_IOHUB0NBIO0_SB_LOCATION_ADDRESS)
                            );
      xUSLSmnWrite (GnbHandle->Address.Address.Segment,
                    GnbHandle->Address.Address.Bus,
                    NBIO_SPACE(GnbHandle, SMN_IOAGR_IOHUB0NBIO0_L1_SB_LOCATION_ADDRESS),
                    Value
                    );
      xUSLSmnWrite (GnbHandle->Address.Address.Segment,
                    GnbHandle->Address.Address.Bus,
                    NBIO_SPACE(GnbHandle, SMN_IOHUB0NBIO0_L2_SB_LOCATION_ADDRESS),
                    Value
                    );
    } else {
      Value = 0;
      xUSLSmnWrite (GnbHandle->Address.Address.Segment,
                    GnbHandle->Address.Address.Bus,
                    NBIO_SPACE(GnbHandle, SMN_IOHUB0NBIO0_SB_LOCATION_ADDRESS),
                    Value
                    );
    }
    GnbHandle = GnbGetNextHandle (GnbHandle);
  }

  //Need to declare them over mNbioIpBlockData
  if (mNbioIpBlockData.NbioConfigData->NbioGlobalCgOverride == 0) {
    mNbioIpBlockData.NbioConfigData->CfgSstunlClkGating = false;
    mNbioIpBlockData.NbioConfigData->IOHCClkGatingSupport = false;
    mNbioIpBlockData.NbioConfigData->IOHCPgEnable = false;
    mNbioIpBlockData.NbioConfigData->CfgNbifMgcgClkGating = false;
    mNbioIpBlockData.NbioConfigData->CfgSyshubMgcgClkGating = false;
    mNbioIpBlockData.NbioConfigData->NbifMgcgHysteresis = 0;
    mNbioIpBlockData.NbioConfigData->SyshubMgcgHysteresis = 0;
  }

  GnbHandle = GetGnbHandle ();
  if (GnbHandle == NULL) {
    NBIO_TRACEPOINT (SIL_TRACE_INFO, "ERROR : Invalid GnbHandle\n");
    assert (false);
    return SilInvalidParameter;
  }

  while (GnbHandle != NULL) {
    if ((GnbHandle->RBIndex & 1) != 0) {
      GnbHandle->Header.DescriptorFlags |= SILICON_NOT_PRESENT_NBIF0;
      GnbHandle->Header.DescriptorFlags |= SILICON_NOT_PRESENT_NBIF2;
    } else {
      GnbHandle->Header.DescriptorFlags |= SILICON_NOT_PRESENT_NBIF1;
    }
    if ((GnbHandle->RBIndex & 0x3) != 0) {
      GnbHandle->Header.DescriptorFlags |= SILICON_NOT_PRESENT_SDPMUX;
    }
    if ((GnbHandle->RBIndex & 0x2) != 0) {
      GnbHandle->Header.DescriptorFlags |= SILICON_NOT_PRESENT_SST;
    }
    NBIO_TRACEPOINT (SIL_TRACE_INFO,
                   "Descriptor flags for RB %d = 0x%X \n",
                   GnbHandle->RBIndex,
                   GnbHandle->Header.DescriptorFlags
                   );

    GnbHandle = GnbGetNextHandle (GnbHandle);
  }

  GnbHandle = GetGnbHandle ();
  if (GnbHandle == NULL) {
    NBIO_TRACEPOINT (SIL_TRACE_INFO, "ERROR : Invalid GnbHandle\n");
    assert (false);
    return SilInvalidParameter;
  }

  while (GnbHandle != NULL) {
    ///
    /// Blast table(s) specific to socket 0 (first socket)
    ///
    Property = PROPERTY_DEFAULT_DEVICE_PRESENCE;
    NBIO_TRACEPOINT (SIL_TRACE_INFO, "Default properties for RB %d = 0x%X", GnbHandle->RBIndex, Property);
    Property &= ~(GnbHandle->Header.DescriptorFlags << 16);
    NBIO_TRACEPOINT (SIL_TRACE_INFO, "Properties for RB %d = 0x%X", GnbHandle->RBIndex, Property);

    if (Property & PROPERTY_PRESENT_IOHC) {
      if (mNbioIpBlockData.NbioConfigData->IOHCClkGatingSupport) {
        Property |= PROPERTY_IOHC_CLKGATING_ENABLED;
      } else {
        Property |= PROPERTY_IOHC_CLKGATING_DISABLED;
      }
      if (mNbioIpBlockData.NbioConfigData->CfgSstunlClkGating) {
        Property |= PROPERTY_SST_CLKGATING_ENABLED;
      } else {
        Property |= PROPERTY_SST_CLKGATING_DISABLED;
      }
      if (mNbioIpBlockData.NbioConfigData->IOHCPgEnable) {
        Property |= PROPERTY_IOHC_CLKGATING_ENABLED;
      } else {
        Property |= PROPERTY_IOHC_CLKGATING_DISABLED;
      }
      if (mNbioIpBlockData.NbioConfigData->CfgNbifMgcgClkGating) {
        Property |= PROPERTY_NBIF_MGCG_CLKGATING_ENABLED;
      } else {
        Property |= PROPERTY_NBIF_MGCG_CLKGATING_DISABLED;
      }
      if (Property & PROPERTY_PRESENT_SYSHUB) {
        if (mNbioIpBlockData.NbioConfigData->CfgSyshubMgcgClkGating) {
          Property |= PROPERTY_SYSHUB_MGCG_CLKGATING_ENABLED;
        } else {
          Property |= PROPERTY_SYSHUB_MGCG_CLKGATING_DISABLED;
        }
      }
      if (0 != mNbioIpBlockData.NbioConfigData->TPHCompleterEnable) {
        Property |= PROPERTY_TPH_COMPLETER_ENABLED;
      }
    }

    if ((GnbHandle->RBIndex & 0x3) == 0) {
      ///
      /// Blast table(s) specific to socket 1 (second socket)
      ///
      ProgramNbioSmnTable (GnbHandle, GnbOncePerSocketInitMP, NBIO_SPACE (GnbHandle, 0), Property);
    }
    ///
    /// Blast tables that should be applied to each GnbHandle
    ///
    ProgramNbioSmnTable (GnbHandle, NbioPprInitValues, NBIO_SPACE (GnbHandle, 0), Property);
    ///
    /// Blast SdpMux tables that should be applied to each Socket
    ///
    ProgramNbioSmnTable (GnbHandle, GnbSdpMuxInitTableCommon, NBIO_SPACE (GnbHandle, 0),Property);
    ProgramNbioSmnTable (GnbHandle, GnbEarlyInitTableCommon, NBIO_SPACE (GnbHandle, 0), Property);

    if ((GnbHandle->RBIndex & 1) == 0) {
      ProgramNbioSmnTable (GnbHandle, GnbnBifInitTable, NBIO_SPACE (GnbHandle, 0), Property);
      NbioBaseInitGenoa (GnbHandle);
    }
    NbifConfigureCommonOptions (GnbHandle);
    /// IOHC client4 VC routing table setting based on type of version
    if(IsSteppingAx() == true) {
      xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED_536),
                  (uint32_t) ~(SIL_RESERVED_129),
                  (0x0 << SIL_RESERVED_130));
      xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED_536),
                  (uint32_t) ~(SIL_RESERVED_131),
                  (0x0 << SIL_RESERVED_132));
    }
    else //B0
    {
      xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED_536),
                  (uint32_t) ~(SIL_RESERVED_129),
                  (0x2 << SIL_RESERVED_130));
      xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED_536),
                  (uint32_t) ~(SIL_RESERVED_131),
                  (0x2 << SIL_RESERVED_132));
    }

    ///
    /// Bar allocation for SMN non pci devices
    /// Note that PSPP bar allocation is slightly different and is
    /// done elsewhere
    ///
    if (mNbioIpBlockData.NbioConfigData->IohcNonPCIBarInitSmu) {
      NonPciBarInit(
        GnbHandle,
        NBIO_SPACE(GnbHandle, SMN_IOHUB0NBIO0_SMU_BASE_ADDR_LO_ADDRESS),
        NBIO_SPACE(GnbHandle, SMN_IOHUB0NBIO0_SMU_BASE_ADDR_HI_ADDRESS),
        NONPCI_BARSIZE_1MB,
        true,
        true,
        false
        );
    }
    if (mNbioIpBlockData.NbioConfigData->IohcNonPCIBarInitDbg) {
      NonPciBarInit(
        GnbHandle,
        NBIO_SPACE(GnbHandle, SMN_IOHUB0NBIO0_DBG_BASE_ADDR_LO_ADDRESS),
        NBIO_SPACE(GnbHandle, SMN_IOHUB0NBIO0_DBG_BASE_ADDR_HI_ADDRESS),
        NONPCI_BARSIZE_512KB,
        true,
        true,
        false
        );
    }
    if (mNbioIpBlockData.NbioConfigData->IohcNonPCIBarInitFastReg) {
      NonPciBarInit(
        GnbHandle,
        NBIO_SPACE(GnbHandle, SMN_IOHUB0NBIO0_FASTREG_BASE_ADDR_LO_ADDRESS),
        NBIO_SPACE(GnbHandle, SMN_IOHUB0NBIO0_FASTREG_BASE_ADDR_HI_ADDRESS),
        NONPCI_BARSIZE_1MB,
        true,
        true,
        false
        );
    }
    if (mNbioIpBlockData.NbioConfigData->IohcNonPCIBarInitFastRegCtl) {
      NonPciBarInit(
        GnbHandle,
        NBIO_SPACE(GnbHandle, SMN_IOHUB0NBIO0_FASTREGCNTL_BASE_ADDR_LO_ADDRESS),
        NBIO_SPACE(GnbHandle, SMN_IOHUB0NBIO0_FASTREGCNTL_BASE_ADDR_HI_ADDRESS),
        NONPCI_BARSIZE_4KB,
        true,
        true,
        false
        );
    }

    GnbHandle = GnbGetNextHandle (GnbHandle);
  }

  NBIO_TRACEPOINT (SIL_TRACE_EXIT, "\n");

  return SilPass;
}


/**
 * PcieEnumerateAndHarvestWrappers
 *
 * @brief  Function to remove harvested PCIe cores from SOC Complex Descriptor
 *
 * @param  Pcie  Pointer to SOC Complex Descriptor
  *
 */
void
PcieEnumerateAndHarvestWrappers (PCIe_PLATFORM_CONFIG *Pcie)
{
  PCIe_SILICON_CONFIG   *SiliconList;
  PCIe_WRAPPER_CONFIG   *PcieCoreList;
  PCIe_COMPLEX_CONFIG   *ComplexList;
  uint32_t              FuseData;
  uint32_t              *FuseMaskList;
  uint8_t               Index;
  uint32_t              FuseMaskPcieCoreAx [] = {
                            SIL_RESERVED2_964,
                            SIL_RESERVED2_960,
                            SIL_RESERVED2_965,
                            SIL_RESERVED2_961,
                            SIL_RESERVED2_966,
                            SIL_RESERVED2_962,
                            SIL_RESERVED2_967,
                            SIL_RESERVED2_963
                        };

  uint32_t              FuseMaskPcieCoreBx [] = {
                            SIL_RESERVED2_956,
                            SIL_RESERVED2_952,
                            SIL_RESERVED2_957,
                            SIL_RESERVED2_953,
                            SIL_RESERVED2_958,
                            SIL_RESERVED2_954,
                            SIL_RESERVED2_959,
                            SIL_RESERVED2_955
                        };

  ComplexList = (PCIe_COMPLEX_CONFIG *) PcieConfigGetChild (DESCRIPTOR_COMPLEX, &Pcie->Header);
  NBIO_TRACEPOINT (SIL_TRACE_ENTRY, "\n");


    while (ComplexList != NULL) {
        SiliconList = (PCIe_SILICON_CONFIG *) PcieConfigGetChild (DESCRIPTOR_SILICON, &(ComplexList->Header));
        while (SiliconList != NULL) {
            PcieCoreList = (PCIe_WRAPPER_CONFIG *) PcieConfigGetChild (DESCRIPTOR_ALL_WRAPPERS, &(SiliconList->Header));
            while (PcieCoreList != NULL) {
                Index = (SiliconList->RBIndex << 1) + PcieCoreList->WrapId;

				if (IsSteppingAx()) {
          FuseData = xUSLSmnRead (SiliconList->Address.Address.Segment,
                                  SiliconList->Address.Address.Bus,
                                  SIL_RESERVED2_1311
                                  );
					FuseMaskList = FuseMaskPcieCoreAx;
				} else {
          FuseData = xUSLSmnRead (SiliconList->Address.Address.Segment,
                                  SiliconList->Address.Address.Bus,
                                  SIL_RESERVED2_1310
                                  );
					FuseMaskList = FuseMaskPcieCoreBx;
				}

                if (FuseData & FuseMaskList[Index]) {
                    PcieComplexHidePcieCore(SiliconList, PcieCoreList->WrapId);
                }
				/* coverity[ptr_arith] */
                PcieCoreList = PcieLibGetNextDescriptor (PcieCoreList);
			}
			/* coverity[ptr_arith] */
			SiliconList = PcieLibGetNextDescriptor (SiliconList);
		}
		/* coverity[ptr_arith] */
		if ((ComplexList->Header.DescriptorFlags & DESCRIPTOR_TERMINATE_TOPOLOGY) == 0) {
		/* coverity[ptr_arith] */
		ComplexList++;
		} else {
		/* coverity[ptr_arith] */
		ComplexList = NULL;
		}
	}
  NBIO_TRACEPOINT (SIL_TRACE_EXIT, "\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * NbifConfigureCommonOptions
 *
 * @brief  This function enables some straps to expose some PCIe capability for MPDMA-TF
 *
 * @param  GnbHandle            Pointer to Iohc config data
 */
void
NbifConfigureCommonOptions(
 GNB_HANDLE          *GnbHandle
  )
{
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1358),
                  (uint32_t) ~(SIL_RESERVED2_1237 |
                            SIL_RESERVED2_1233 |
                            SIL_RESERVED2_1235),
                  (GnbHandle->Address.Address.Bus << SIL_RESERVED2_1238) |
                  (0x7 << SIL_RESERVED2_1234) |
                  (0x1 << SIL_RESERVED2_1236)
                  );

  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1332),
                  (uint32_t) ~(SIL_RESERVED2_1134),
                  (((mNbioIpBlockData.NbioConfigData->CfgSriovEnDev0F1) ? 1 : 0) << SIL_RESERVED2_1135)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1328),
                  (uint32_t) ~(SIL_RESERVED2_1110),
                  (((mNbioIpBlockData.NbioConfigData->CfgAriEnDev0F1) ? 1 : 0) << SIL_RESERVED2_1111)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1332),
                  (uint32_t) ~(SIL_RESERVED2_1126),
                  (((mNbioIpBlockData.NbioConfigData->CfgAerEnDev0F1) ? 1 : 0) << SIL_RESERVED2_1127)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1332),
                  (uint32_t) ~(SIL_RESERVED2_1124),
                  (((mNbioIpBlockData.NbioConfigData->CfgAcsEnDev0F1) ? 1 : 0) << SIL_RESERVED2_1125)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1332),
                  (uint32_t) ~(SIL_RESERVED2_1128),
                  (((mNbioIpBlockData.NbioConfigData->CfgAtsEnDev0F1) ? 1 : 0) << SIL_RESERVED2_1129)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1332),
                  (uint32_t) ~(SIL_RESERVED2_1132),
                  (((mNbioIpBlockData.NbioConfigData->CfgPasidEnDev0F1) ? 1 : 0) << SIL_RESERVED2_1133)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1335),
                  (uint32_t) ~(SIL_RESERVED2_1144),
                  (((mNbioIpBlockData.NbioConfigData->CfgPwrEnDev0F1) ? 1 : 0) << SIL_RESERVED2_1145)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1335),
                  (uint32_t) ~(SIL_RESERVED2_1146),
                  (((mNbioIpBlockData.NbioConfigData->CfgRtrEnDev0F1) ? 1 : 0) << SIL_RESERVED2_1147)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1332),
                  (uint32_t) ~(SIL_RESERVED2_1130),
                  (((mNbioIpBlockData.NbioConfigData->CfgPriEnDev0F1) ? 1 : 0) << SIL_RESERVED2_1131)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1384),
                  (uint32_t) ~(SIL_RESERVED2_1134),
                  (((mNbioIpBlockData.NbioConfigData->CfgSriovEnDev0F1) ? 1 : 0) << SIL_RESERVED2_1135)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1381),
                  (uint32_t) ~(SIL_RESERVED2_1110),
                  (((mNbioIpBlockData.NbioConfigData->CfgAriEnDev0F1) ? 1 : 0) << SIL_RESERVED2_1111)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1384),
                  (uint32_t) ~(SIL_RESERVED2_1126),
                  (((mNbioIpBlockData.NbioConfigData->CfgAerEnDev0F1) ? 1 : 0) << SIL_RESERVED2_1127)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1384),
                  (uint32_t) ~(SIL_RESERVED2_1124),
                  (((mNbioIpBlockData.NbioConfigData->CfgAcsEnDev0F1) ? 1 : 0) << SIL_RESERVED2_1125)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1384),
                  (uint32_t) ~(SIL_RESERVED2_1128),
                  (((mNbioIpBlockData.NbioConfigData->CfgAtsEnDev0F1) ? 1 : 0) << SIL_RESERVED2_1129)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1384),
                  (uint32_t) ~(SIL_RESERVED2_1132),
                  (((mNbioIpBlockData.NbioConfigData->CfgPasidEnDev0F1) ? 1 : 0) << SIL_RESERVED2_1133)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1387),
                  (uint32_t) ~(SIL_RESERVED2_1144),
                  (((mNbioIpBlockData.NbioConfigData->CfgPwrEnDev0F1) ? 1 : 0) << SIL_RESERVED2_1145)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1387),
                  (uint32_t) ~(SIL_RESERVED2_1146),
                  (((mNbioIpBlockData.NbioConfigData->CfgRtrEnDev0F1) ? 1 : 0) << SIL_RESERVED2_1147)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1384),
                  (uint32_t) ~(SIL_RESERVED2_1130),
                  (((mNbioIpBlockData.NbioConfigData->CfgPriEnDev0F1) ? 1 : 0) << SIL_RESERVED2_1131)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1300),
                  (uint32_t) ~(SIL_RESERVED2_1029),
                  (((mNbioIpBlockData.NbioConfigData->AtcEnable) ? 1 : 0) << SIL_RESERVED2_1030)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1305),
                  (uint32_t) ~(SIL_RESERVED2_1029),
                  (((mNbioIpBlockData.NbioConfigData->AtcEnable) ? 1 : 0) << SIL_RESERVED2_1030)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1353),
                  (uint32_t) ~(SIL_RESERVED2_1203),
                  (((mNbioIpBlockData.NbioConfigData->AcsEnRccDev0) ? 1 : 0) << SIL_RESERVED2_1204)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1353),
                  (uint32_t) ~(SIL_RESERVED2_1205),
                  (((mNbioIpBlockData.NbioConfigData->AerEnRccDev0) ? 1 : 0) << SIL_RESERVED2_1206)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1403),
                  (uint32_t) ~(SIL_RESERVED2_1203),
                  (((mNbioIpBlockData.NbioConfigData->AcsEnRccDev0) ? 1 : 0) << SIL_RESERVED2_1204)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1403),
                  (uint32_t) ~(SIL_RESERVED2_1205),
                  (((mNbioIpBlockData.NbioConfigData->AerEnRccDev0) ? 1 : 0) << SIL_RESERVED2_1206)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1356),
                  (uint32_t) ~(SIL_RESERVED2_1224),
                  (((mNbioIpBlockData.NbioConfigData->AcsSourceValStrap5) ? 1 : 0) << SIL_RESERVED2_1225)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1356),
                  (uint32_t) ~(SIL_RESERVED2_1226),
                  (((mNbioIpBlockData.NbioConfigData->AcsTranslationalBlockingStrap5) ? 1 : 0) << SIL_RESERVED2_1227)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1356),
                  (uint32_t) ~(SIL_RESERVED2_1222),
                  (((mNbioIpBlockData.NbioConfigData->AcsP2pReqStrap5) ? 1 : 0) << SIL_RESERVED2_1223)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1356),
                  (uint32_t) ~(SIL_RESERVED2_1218),
                  (((mNbioIpBlockData.NbioConfigData->AcsP2pCompStrap5) ? 1 : 0) << SIL_RESERVED2_1219)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1356),
                  (uint32_t) ~(SIL_RESERVED2_1228),
                  (((mNbioIpBlockData.NbioConfigData->AcsUpstreamFwdStrap5) ? 1 : 0) << SIL_RESERVED2_1229)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1356),
                  (uint32_t) ~(SIL_RESERVED2_1220),
                  (((mNbioIpBlockData.NbioConfigData->AcsP2PEgressStrap5) ? 1 : 0) << SIL_RESERVED2_1221)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1356),
                  (uint32_t) ~(SIL_RESERVED2_1216),
                  (((mNbioIpBlockData.NbioConfigData->AcsDirectTranslatedStrap5) ? 1 : 0) << SIL_RESERVED2_1217)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1356),
                  (uint32_t) ~(SIL_RESERVED2_1230),
                  (((mNbioIpBlockData.NbioConfigData->AcsSsidEnStrap5) ? 1 : 0) << SIL_RESERVED2_1231)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1406),
                  (uint32_t) ~(SIL_RESERVED2_1224),
                  (((mNbioIpBlockData.NbioConfigData->AcsSourceValStrap5) ? 1 : 0) << SIL_RESERVED2_1225)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1406),
                  (uint32_t) ~(SIL_RESERVED2_1226),
                  (((mNbioIpBlockData.NbioConfigData->AcsTranslationalBlockingStrap5) ? 1 : 0) << SIL_RESERVED2_1227)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1406),
                  (uint32_t) ~(SIL_RESERVED2_1222),
                  (((mNbioIpBlockData.NbioConfigData->AcsP2pReqStrap5) ? 1 : 0) << SIL_RESERVED2_1223)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1406),
                  (uint32_t) ~(SIL_RESERVED2_1218),
                  (((mNbioIpBlockData.NbioConfigData->AcsP2pCompStrap5) ? 1 : 0) << SIL_RESERVED2_1219)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1406),
                  (uint32_t) ~(SIL_RESERVED2_1228),
                  (((mNbioIpBlockData.NbioConfigData->AcsUpstreamFwdStrap5) ? 1 : 0) << SIL_RESERVED2_1229)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1406),
                  (uint32_t) ~(SIL_RESERVED2_1220),
                  (((mNbioIpBlockData.NbioConfigData->AcsP2PEgressStrap5) ? 1 : 0) << SIL_RESERVED2_1221)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1406),
                  (uint32_t) ~(SIL_RESERVED2_1216),
                  (((mNbioIpBlockData.NbioConfigData->AcsDirectTranslatedStrap5) ? 1 : 0) << SIL_RESERVED2_1217)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1406),
                  (uint32_t) ~(SIL_RESERVED2_1230),
                  (((mNbioIpBlockData.NbioConfigData->AcsSsidEnStrap5) ? 1 : 0) << SIL_RESERVED2_1231)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle,  SIL_RESERVED2_1326),
                  (uint32_t) ~(SIL_RESERVED2_1096),
                  (((mNbioIpBlockData.NbioConfigData->DlfEnStrap1) ? 1 : 0) << SIL_RESERVED2_1097)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle,  SIL_RESERVED2_1326),
                  (uint32_t) ~(SIL_RESERVED2_1100),
                  (((mNbioIpBlockData.NbioConfigData->Phy16gtStrap1) ? 1 : 0) << SIL_RESERVED2_1101)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle,  SIL_RESERVED2_1326),
                  (uint32_t) ~(SIL_RESERVED2_1098),
                  (((mNbioIpBlockData.NbioConfigData->MarginEnStrap1) ? 1 : 0) << SIL_RESERVED2_1099)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle,  SIL_RESERVED2_1379),
                  (uint32_t) ~(SIL_RESERVED2_1096),
                  (((mNbioIpBlockData.NbioConfigData->DlfEnStrap1) ? 1 : 0) << SIL_RESERVED2_1097)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle,  SIL_RESERVED2_1379),
                  (uint32_t) ~(SIL_RESERVED2_1100),
                  (((mNbioIpBlockData.NbioConfigData->Phy16gtStrap1) ? 1 : 0) << SIL_RESERVED2_1101)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle,  SIL_RESERVED2_1379),
                  (uint32_t) ~(SIL_RESERVED2_1098),
                  (((mNbioIpBlockData.NbioConfigData->MarginEnStrap1) ? 1 : 0) << SIL_RESERVED2_1099)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle,  SIL_RESERVED2_1424),
                  (uint32_t) ~(SIL_RESERVED2_1096),
                  (((mNbioIpBlockData.NbioConfigData->DlfEnStrap1) ? 1 : 0) << SIL_RESERVED2_1097)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle,  SIL_RESERVED2_1424),
                  (uint32_t) ~(SIL_RESERVED2_1100),
                  (((mNbioIpBlockData.NbioConfigData->Phy16gtStrap1) ? 1 : 0) << SIL_RESERVED2_1101)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle,  SIL_RESERVED2_1424),
                  (uint32_t) ~(SIL_RESERVED2_1098),
                  (((mNbioIpBlockData.NbioConfigData->MarginEnStrap1) ? 1 : 0) << SIL_RESERVED2_1099)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1301),
                  (uint32_t) ~(SIL_RESERVED2_1070),
                  (((mNbioIpBlockData.NbioConfigData->PriEnPageReq) ? 1 : 0) << SIL_RESERVED2_1071)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1306),
                  (uint32_t) ~(SIL_RESERVED2_1070),
                  (((mNbioIpBlockData.NbioConfigData->PriEnPageReq) ? 1 : 0) << SIL_RESERVED2_1071)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1301),
                  (uint32_t) ~(SIL_RESERVED2_1072),
                  (((mNbioIpBlockData.NbioConfigData->PriResetPageReq) ? 1 : 0) << SIL_RESERVED2_1073)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1306),
                  (uint32_t) ~(SIL_RESERVED2_1072),
                  (((mNbioIpBlockData.NbioConfigData->PriResetPageReq) ? 1 : 0) << SIL_RESERVED2_1073)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1299),
                  (uint32_t) ~(SIL_RESERVED2_1017),
                  (((mNbioIpBlockData.NbioConfigData->AcsSourceVal) ? 1 : 0) << SIL_RESERVED2_1019)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1299),
                  (uint32_t) ~(SIL_RESERVED2_1021),
                  (((mNbioIpBlockData.NbioConfigData->AcsTranslationalBlocking) ? 1 : 0) << SIL_RESERVED2_1023)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1299),
                  (uint32_t) ~(SIL_RESERVED2_1013),
                  (((mNbioIpBlockData.NbioConfigData->AcsP2pReq) ? 1 : 0) << SIL_RESERVED2_1015)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1299),
                  (uint32_t) ~(SIL_RESERVED2_1005),
                  (((mNbioIpBlockData.NbioConfigData->AcsP2pComp) ? 1 : 0) << SIL_RESERVED2_1007)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle,SIL_RESERVED2_1299),
                  (uint32_t) ~(SIL_RESERVED2_1025),
                  (((mNbioIpBlockData.NbioConfigData->AcsUpstreamFwd) ? 1 : 0) << SIL_RESERVED2_1027)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1299),
                  (uint32_t) ~(SIL_RESERVED2_1009),
                  (((mNbioIpBlockData.NbioConfigData->AcsP2PEgress) ? 1 : 0) << SIL_RESERVED2_1011)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1304),
                  (uint32_t) ~(SIL_RESERVED2_1017),
                  (((mNbioIpBlockData.NbioConfigData->AcsSourceVal) ? 1 : 0) << SIL_RESERVED2_1019)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1304),
                  (uint32_t) ~(SIL_RESERVED2_1021),
                  (((mNbioIpBlockData.NbioConfigData->AcsTranslationalBlocking) ? 1 : 0) << SIL_RESERVED2_1023)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1304),
                  (uint32_t) ~(SIL_RESERVED2_1013),
                  (((mNbioIpBlockData.NbioConfigData->AcsP2pReq) ? 1 : 0) << SIL_RESERVED2_1015)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1304),
                  (uint32_t) ~(SIL_RESERVED2_1005),
                  (((mNbioIpBlockData.NbioConfigData->AcsP2pComp) ? 1 : 0) << SIL_RESERVED2_1007)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle,SIL_RESERVED2_1304),
                  (uint32_t) ~(SIL_RESERVED2_1025),
                  (((mNbioIpBlockData.NbioConfigData->AcsUpstreamFwd) ? 1 : 0) << SIL_RESERVED2_1027)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1304),
                  (uint32_t) ~(SIL_RESERVED2_1009),
                  (((mNbioIpBlockData.NbioConfigData->AcsP2PEgress) ? 1 : 0) << SIL_RESERVED2_1011)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1309),
                  (uint32_t) ~(SIL_RESERVED2_1017),
                  (((mNbioIpBlockData.NbioConfigData->AcsSourceValStrap5) ? 1 : 0) << SIL_RESERVED2_1019)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1309),
                  (uint32_t) ~(SIL_RESERVED2_1021),
                  (((mNbioIpBlockData.NbioConfigData->AcsTranslationalBlocking) ? 1 : 0) << SIL_RESERVED2_1023)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1309),
                  (uint32_t) ~(SIL_RESERVED2_1015),
                  (((mNbioIpBlockData.NbioConfigData->AcsP2pReq) ? 1 : 0) << SIL_RESERVED2_1013)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1309),
                  (uint32_t) ~(SIL_RESERVED2_1005),
                  (((mNbioIpBlockData.NbioConfigData->AcsP2pComp) ? 1 : 0) << SIL_RESERVED2_1007)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1309),
                  (uint32_t) ~(SIL_RESERVED2_1025),
                  (((mNbioIpBlockData.NbioConfigData->AcsUpstreamFwd) ? 1 : 0) << SIL_RESERVED2_1027)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1309),
                  (uint32_t) ~(SIL_RESERVED2_1009),
                  (((mNbioIpBlockData.NbioConfigData->AcsP2PEgress) ? 1 : 0) << SIL_RESERVED2_1011)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1354),
                  (uint32_t) ~(SIL_RESERVED2_1207),
                  (((mNbioIpBlockData.NbioConfigData->RccDev0E2EPrefix) ? 1 : 0) << SIL_RESERVED2_1208)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1404),
                  (uint32_t) ~(SIL_RESERVED2_1207),
                  (((mNbioIpBlockData.NbioConfigData->RccDev0E2EPrefix) ? 1 : 0) << SIL_RESERVED2_1208)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1354),
                  (uint32_t) ~(SIL_RESERVED2_1209),
                  (((mNbioIpBlockData.NbioConfigData->RccDev0ExtendedFmtSupported) ? 1 : 0) << SIL_RESERVED2_1210)
                  );
  xUSLSmnReadModifyWrite (GnbHandle->Address.Address.Segment,
                  GnbHandle->Address.Address.Bus,
                  NBIO_SPACE(GnbHandle, SIL_RESERVED2_1404),
                  (uint32_t) ~(SIL_RESERVED2_1209),
                  (((mNbioIpBlockData.NbioConfigData->RccDev0ExtendedFmtSupported) ? 1 : 0) << SIL_RESERVED2_1210)
                  );
  return;
}
