/**
 * @file  NbioTopMem.c
 * @brief OpenSIL NBIO top of memory initialization
 *
 */

/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <xSIM.h>
#include "Nbio.h"
#include "NbioData.h"
#include "NbioPcie.h"
#include "NbioCommon.h"
#include "GnbRegisters.h"
#include <NbioPcieTopologyHelper.h>
#include <MsrReg.h>
#include <CommonLib/CpuLib.h>

/*----------------------------------------------------------------------------------------*/
/**
 * NbioSetTopofMemory
 *
 * Initialize the NBIO top of memory to match the CPU MSRs.
 * Read default registers once. Write back to all instances.
 *
 * @return SIL_STATUS
 * @retval SilPass - everything is OK
 * @retval SilAborted- Failed to allocate MMIO resources or program IOMMU NBIO tables.
 *
 */
SIL_STATUS
NbioSetTopOfMemory (void)
{
  GNB_HANDLE                      *GnbHandle;
  uint64_t                        MsrData;
  uint64_t                        GnbTom2;
  uint64_t                        GnbTom3;
  NB_TOP_OF_DRAM_SLOT1_STRUCT     TOP_OF_DRAM;
  NB_LOWER_TOP_OF_DRAM2_STRUCT    TOM2_LOWER_REG;
  NB_UPPER_TOP_OF_DRAM2_STRUCT    TOM2_UPPER_REG;
  NB_TOP_OF_DRAM3_STRUCT          TOM3_REG;

  NBIO_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  TOM3_REG.Value = 0;
  GnbHandle = GetGnbHandle ();

  // Read TOP_OF_DRAM registers
   TOP_OF_DRAM.Value = xUSLSmnRead (GnbHandle->Address.Address.Segment,
       GnbHandle->Address.Address.Bus,
       SMN_IOHUB0NBIO0_NB_TOP_OF_DRAM_SLOT1_ADDRESS);
   TOP_OF_DRAM.Value = xUSLSmnRead (GnbHandle->Address.Address.Segment,
       GnbHandle->Address.Address.Bus,
       SMN_IOHUB0NBIO0_NB_LOWER_TOP_OF_DRAM2_ADDRESS);
  TOM2_UPPER_REG.Value = xUSLSmnRead (GnbHandle->Address.Address.Segment,
      GnbHandle->Address.Address.Bus,
      SMN_IOHUB0NBIO0_NB_UPPER_TOP_OF_DRAM2_ADDRESS);

  //Read memory size below 4G from MSR C001_001A
  MsrData = xUslRdMsr (MSR_TOM);
  //Write to NB register 0x90
  TOP_OF_DRAM.Field.TOP_OF_DRAM = ((uint32_t)MsrData & 0xFF800000) >> 23;     //Keep bits 31:23

  GnbTom2 = 0;
  GnbTom3 = 0;
  MsrData = xUslRdMsr (MSR_SYS_CFG);
  if ((MsrData & BIT_64(21)) != 0) {
      //If SYS_CFG(MtrrTom2En) then configure GNB TOM2 and TOM3
      //Read memory size above 4G from TOP_MEM2 (MSR C001_001D)
      MsrData = xUslRdMsr (MSR_TOM2);
      if ((MsrData & (uint64_t)0x0000FFFFFFC00000) > ((uint64_t)0x0000010000000000)) {
          // If TOP_MEM2 is above 1TB, enable GNB TOM2 and TOM3
          // Set TOM2 for below 1TB limit
          GnbTom2 = 0x000000FD00000000;
          // TOM3 is INCLUSIVE, so set it to TOM - 1 using bits 51:22
          GnbTom3 = (MsrData - 1) & (uint64_t)0x000FFFFFFFC00000;
      } else {
          // If TOP_MEM2 is below 1TB, set TOM2 using bits 40:23
          GnbTom2 = MsrData & (uint64_t)0x000001FFFF800000;       //Keep bits 40:23
          // If TOP_MEM2 is below 1TB, disable GNB TOM3
        GnbTom3 = 0;
      }
  }
  if (GnbTom2 != 0) {
      // Write memory size[39:32] to indirect register 1A[7:0]
      TOM2_UPPER_REG.Field.UPPER_TOM2 = (uint32_t) ((GnbTom2 >> 32) & 0xFF);
      // Write memory size[31:23] to indirect register 19[31:23] and enable memory through bit 0
      TOM2_LOWER_REG.Field.LOWER_TOM2 = ((uint32_t)GnbTom2 & 0xFF800000) >> 23;
      TOM2_LOWER_REG.Field.ENABLE = 1;
  }
  if (GnbTom3 != 0) {
      // Above 1TB addressing TOM3 if MSR TOM is above 1TB
      TOM3_REG.Field.TOM3_LIMIT = (uint32_t) (GnbTom3 >> 22);
      TOM3_REG.Field.TOM3_ENABLE = 1;
  }

  NBIO_TRACEPOINT (SIL_TRACE_INFO, "lower_tom2: 0x%x\n", TOM2_LOWER_REG.Field.LOWER_TOM2);
  NBIO_TRACEPOINT (SIL_TRACE_INFO, "upper_tom2: 0x%x\n", TOM2_UPPER_REG.Field.UPPER_TOM2);
  NBIO_TRACEPOINT (SIL_TRACE_INFO, "tom3_limit: 0x%x\n", TOM3_REG.Field.TOM3_LIMIT);

  while (GnbHandle != NULL) {
      // Write TOP_OF_DRAM registers to all instances
      xUSLSmnWrite (GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          NBIO_SPACE(GnbHandle, SMN_IOHUB0NBIO0_NB_TOP_OF_DRAM_SLOT1_ADDRESS),
          TOP_OF_DRAM.Value);
      xUSLSmnWrite (GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          NBIO_SPACE(GnbHandle, SMN_IOHUB0NBIO0_NB_LOWER_TOP_OF_DRAM2_ADDRESS),
          TOM2_LOWER_REG.Value);
      xUSLSmnWrite (GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          NBIO_SPACE(GnbHandle, SMN_IOHUB0NBIO0_NB_UPPER_TOP_OF_DRAM2_ADDRESS),
          TOM2_UPPER_REG.Value);
      xUSLSmnWrite (GnbHandle->Address.Address.Segment,
          GnbHandle->Address.Address.Bus,
          NBIO_SPACE(GnbHandle, SMN_IOHUB0NBIO0_NB_TOP_OF_DRAM3_ADDRESS),
          TOM3_REG.Value);
      GnbHandle = GnbGetNextHandle (GnbHandle);
  }
  NBIO_TRACEPOINT (SIL_TRACE_EXIT, "\n");
  return SilPass;
}

