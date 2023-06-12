/**
 * @file  xPrfRas.c
 * @brief Platform Reference Firmware - exposes Platform specific features for
 *        RAS
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include <CoreTopologyService.h>
#include <ApobCmn.h>
#include <RAS/Common/RasClass-api.h>
#include <RAS/RasIp2Ip.h>
#include <RAS/RasDefs.h>
#include <MsrReg.h>
#include <DF/DfIp2Ip.h>

#include "xPRF-api.h"

/*
 * xPrfCollectDimmMap
 *
 * @brief Populate the DimmMap buffer input from the Host.
 *
 * @param   DimmMap     Input buffer for the Dimm Map.  The Host is responsible
 *                      for ensuring the buffer size is sufficient to contain
 *                      SIL_ADDR_DATA (defined in RasClass-api.h).
 *                      On output, the buffer is populated with SIL_ADDR_DATA.
 *
 * @return  SIL_STATUS
 *
 * @retval  SilPass         The Dimm Map was successfully populated.
 * @retval  SilOutOfBounds  The Dimm Map input buffer size was not sufficient.
 */
SIL_STATUS
xPrfCollectDimmMap (
  SIL_ADDR_DATA *DimmMap
)
{
  SIL_STATUS    Status;
  uint32_t      NumberOfInstalledProcessors;
  uint32_t      NumberOfRootBridgesOnDie;
  uint32_t      TotalNumberOfDie;
  uint32_t      BusNumberBase;
  uint32_t      SocketId;
  uint32_t      DieId;
  uint32_t      ChannelId;
  uint32_t      SmuFuse;
  SIL_ADDR_DATA *AddrData;
  RAS_IP2IP_API *RasApi;
  DF_IP2IP_API  *DfApi;
  uint8_t       UmcPerDie;

  Status = SilGetIp2IpApi (SilId_RasClass, (void **)&RasApi);
  if (Status != SilPass) {
    XPRF_TRACEPOINT (SIL_TRACE_ERROR, "RAS API not found!\n");
    return Status;
  }
  Status = SilGetIp2IpApi (SilId_DfClass, (void **)&DfApi);
  if (Status != SilPass) {
    XPRF_TRACEPOINT (SIL_TRACE_ERROR, "DF API not found!\n");
    return Status;
  }

  UmcPerDie = RasApi->GetUmcPerDie ();

  AddrData = DimmMap;

  NumberOfInstalledProcessors = 0;
  TotalNumberOfDie = 0;
  NumberOfRootBridgesOnDie = 0;
  BusNumberBase = 0;

  DfApi->DfGetSystemInfo (&NumberOfInstalledProcessors, NULL, NULL, NULL, NULL);

  for (SocketId = 0; SocketId < NumberOfInstalledProcessors; SocketId++) {
    DfApi->DfGetProcessorInfo ((uint32_t)SocketId, &TotalNumberOfDie, NULL);
    for (DieId = 0; DieId < TotalNumberOfDie; DieId++) {
      DfApi->DfGetRootBridgeInfo (
        SocketId,
        DieId,
        NumberOfRootBridgesOnDie,
        NULL,
        &BusNumberBase,
        NULL,
        NULL,
        NULL,
        NULL
        );

      //Get UMC Harvest FUSE
      SmuFuse = RasApi->GetUmcHarvestFuse (BusNumberBase);

      for (ChannelId = 0; ChannelId < UmcPerDie; ChannelId++) {
        if (0 == (SmuFuse & ((uint32_t) 1 << ChannelId))) {
          XPRF_TRACEPOINT (
            SIL_TRACE_INFO,
            "\nsocket %x die %x channel %x pci bus = %x\n",
            SocketId,
            DieId,
            ChannelId,
            BusNumberBase
            );
        }
      }
    }
  }

  return SilPass;
}

/**
 * xPrfGetMaxCpus
 *
 * @brief   Returns the maximum possible number of CPUs.  The maximum here
 *          represents the absolute maximum supported by any SoC in openSIL.
 *          This value is used to allocate memory for the Cpu map.
 *
 *          When the Host allocate memory for the RasCpuMap buffer, this
 *          function should be used to determine the number of SIL_CPU_INFO
 *          structures that need to be allocated.
 *
 * @return  The maximum number of cpus including logical CPUs (threads).
 */
uint32_t
xPrfGetMaxCpus (void)
{
  return (uint32_t)(CCX_MAX_SOCKETS * CCX_MAX_DIES_PER_SOCKET *
    MAX_CCDS_PER_DIE * MAX_COMPLEXES_PER_CCD * CCX_MAX_CORES_PER_COMPLEX *
    CCX_MAX_THREADS_PER_CORE);
}

/*
 * xPrfCollectCpuMap
 *
 * @brief This function is responsible for building the CPU map used by the host
 *        RAS driver.
 *
 * @param   RasCpuMap   On input, the pointer to the CPU map structure.  The structure contains a SIL_CPU_INFO for
 *                      every processor (thread) in the system.  It is the responsibility of the host to allocate
 *                      sufficient memory for this structure.
 *                      On output, the buffer is filled with instances of
 *                      SIL_CPU_INFO (defined in RasClass-api.h) for each CPU.
 * @param   CpuMapSize  The size of the buffer RasCpuMap.
 * @param   TotalCpus   Return the total number of CPUs.
 *
 * @details This function has dependencies on the following openSIL
 *          Services/IPs:
 *
 *          Df->DfGetSystemInfo()
 *          Df->DfGetCoreTopologyOnDie()
 *
 * @retval  SilPass             CPU map created successfully
 * @retval  SilInvalidParameter GetProcessorInfo failed
 * @retval  SilInvalidParameter GetCoreTopologyOnDie failed
 * @retval  SilNotFound         Info for current ccd, complex, or core not found
 * @retval  SilOutOfBounds      The input buffer is not sufficient for all CPUs
 */
SIL_STATUS
xPrfCollectCpuMap (
  SIL_CPU_INFO *RasCpuMap,
  uint32_t     CpuMapSize,
  uint32_t     *TotalCpus
  )
{
  SIL_STATUS    Status;
  uint32_t      SocketCount;
  uint32_t      Index;
  uint32_t      NumberOfDies;
  uint32_t      NumberOfCcds;
  uint8_t       SocketLoop;
  uint8_t       DieLoop;
  uint8_t       CcdLoop;
  uint8_t       ComplexLoop;
  uint8_t       CoreLoop;
  uint32_t      NumberOfComplexes;
  uint32_t      NumberOfCores;
  uint32_t      NumberOfThreads;
  SIL_CPU_INFO  *SilCpuMap;
  uint32_t      CpuInfoSize;
  uint32_t      MapSizeRemaining;
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT  ApobCcdLogToPhysMap;
  DF_IP2IP_API* DfIp2IpApi;

  XPRF_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

  Status = SilGetIp2IpApi (SilId_DfClass, (void **)&DfIp2IpApi);
  if (Status != SilPass) {
    XPRF_TRACEPOINT (SIL_TRACE_ERROR, "DF API not found!\n");
    return Status;
  }

  SilCpuMap = RasCpuMap;
  CpuInfoSize = sizeof (SIL_CPU_INFO);
  // MapSizeRemaining is use to track the available space in RasCpuMap
  MapSizeRemaining = CpuMapSize;

  DfIp2IpApi->DfGetSystemInfo (&SocketCount, NULL, NULL, NULL, NULL);

  Index = 0;
  NumberOfCcds = 0;
  for (SocketLoop = 0; SocketLoop < SocketCount; SocketLoop++) {
    Status = DfIp2IpApi->DfGetProcessorInfo (SocketLoop, &NumberOfDies, NULL);
    if (Status != SilPass) {
      XPRF_TRACEPOINT (SIL_TRACE_ERROR, "GetProcessorInfo failed!\n");
      return Status; // SilInvalidParameter
    }

    XPRF_TRACEPOINT (
      SIL_TRACE_INFO,
      "SocketId: %d, NumberOfComplexes: %d\n",
      SocketLoop,
      NumberOfDies
      );

    for (DieLoop = 0; DieLoop < NumberOfDies; DieLoop++) {
      Status = GetCoreTopologyOnDie (
        SocketLoop,
        DieLoop,
        &NumberOfCcds,
        &NumberOfComplexes,
        &NumberOfCores,
        &NumberOfThreads
        );
      if (Status != SilPass) {
        XPRF_TRACEPOINT (SIL_TRACE_ERROR, "GetCoreTopologyOnDie failed!\n");
        return Status; // SilInvalidParameter
      }

      XPRF_TRACEPOINT (
        SIL_TRACE_INFO,
        "  NumberOfCcd: %d, NumberOfComplexes: %d\n",
        NumberOfCcds,
        NumberOfComplexes
        );

      ApobGetCcdLogToPhysMap (SocketLoop, DieLoop, &ApobCcdLogToPhysMap);

      for (CcdLoop = 0; CcdLoop < NumberOfCcds; CcdLoop++) {
        if (ApobCcdLogToPhysMap.CcdMap[CcdLoop].PhysCcdNumber ==
          CCX_NOT_PRESENT) {
          continue;
        }
        for (ComplexLoop = 0; ComplexLoop < NumberOfComplexes; ComplexLoop++) {
          if (ApobCcdLogToPhysMap.CcdMap[CcdLoop].ComplexMap[ComplexLoop].
            PhysComplexNumber == CCX_NOT_PRESENT) {
            continue;
          }
          for (CoreLoop = 0; CoreLoop < NumberOfCores; CoreLoop++) {
            if (ApobCcdLogToPhysMap.CcdMap[CcdLoop].ComplexMap[ComplexLoop].
              CoreInfo[CoreLoop].PhysCoreNumber == CCX_NOT_PRESENT) {
              continue;
            }

            // Core X Thread 0
            if (MapSizeRemaining < CpuInfoSize) {
              XPRF_TRACEPOINT (
                SIL_TRACE_ERROR,
                "Cpu Map buffer from Host is too small.\n"
                );
              assert (CpuMapSize >= CpuInfoSize);
              return SilOutOfBounds;
            }
            SilCpuMap[Index].ProcessorNumber = Index;    //CPU Logic Number
            SilCpuMap[Index].SocketId = SocketLoop & 0xFF;
            SilCpuMap[Index].DieId = ApobCcdLogToPhysMap.CcdMap[CcdLoop].PhysCcdNumber;
            SilCpuMap[Index].CcxId = ApobCcdLogToPhysMap.CcdMap[CcdLoop].ComplexMap[ComplexLoop].PhysComplexNumber;
            SilCpuMap[Index].CoreId =
              ApobCcdLogToPhysMap.CcdMap[CcdLoop].ComplexMap[ComplexLoop].CoreInfo[CoreLoop].PhysCoreNumber;
            SilCpuMap[Index].ThreadID = 0;
            MapSizeRemaining -= CpuInfoSize;
            Index++;

            // Core X Thread 1
            if (NumberOfThreads > 1) {
              if (MapSizeRemaining < CpuInfoSize) {
                XPRF_TRACEPOINT (
                  SIL_TRACE_ERROR,
                  "Cpu Map buffer from Host is too small.\n"
                  );
                assert (CpuMapSize >= CpuInfoSize);
                return SilOutOfBounds;
              }
              SilCpuMap[Index].ProcessorNumber = Index; //CPU Logic Number
              SilCpuMap[Index].SocketId = SocketLoop & 0xFF;
              SilCpuMap[Index].DieId = ApobCcdLogToPhysMap.CcdMap[CcdLoop].PhysCcdNumber;
              SilCpuMap[Index].CcxId = ApobCcdLogToPhysMap.CcdMap[CcdLoop].ComplexMap[ComplexLoop].PhysComplexNumber;
              SilCpuMap[Index].CoreId =
                ApobCcdLogToPhysMap.CcdMap[CcdLoop].ComplexMap[ComplexLoop].CoreInfo[CoreLoop].PhysCoreNumber;
              SilCpuMap[Index].ThreadID = 1;
              MapSizeRemaining -= CpuInfoSize;
              Index++;
            }
          }
        }
      }
    }
  }

  *TotalCpus = Index;

  XPRF_TRACEPOINT (SIL_TRACE_EXIT, "\n");

  return SilPass;
}

/*
 * xPrfCollectCoreMcaInfo
 *
 * @brief   Save machine check architecture (MCA) IP ID value to CPU MCA Info
 *          buffer
 *
 * @details Read the MCA IP ID for every MCA bank on the currect cpu and save it
 *          into the MCA INFO structure buffer provided by the Host.
 *
 *          It is the responsibility of the Host to ensure the CpuMcaInfo input
 *          buffer is sufficient to contain the SIL_CPU_MCA_INFO (defined in
 *          RasClass-api.h).
 *
 * @note    ***This function is executed on all processors by the Host in a
 *          multi-processor environment.
 *
 * @param   Buffer  An input buffer that points to a struct containing the empty
 *                  Cpu MCA info buffer of the currently executing processor and
 *                  the buffer size.
 */
void
xPrfCollectCoreMcaInfo (
  void *Buffer
)
{
  uint32_t                    BankNum;
  SIL_MCA_IPID_MSR            McaIpid;
  SIL_CPU_MCA_INFO            *CpuMcaInfo;
  SIL_CPU_MCA_INFO_BUFFER     *Input;

  Input = (SIL_CPU_MCA_INFO_BUFFER *)Buffer;
  if (sizeof(SIL_CPU_MCA_INFO) > Input->Size) {
    XPRF_TRACEPOINT (
      SIL_TRACE_ERROR,
      "sizeof(SIL_CPU_MCA_INFO): 0x%x, Input->Size: 0x%x\n",
      sizeof(SIL_CPU_MCA_INFO),
      Input->Size
      );
    // Assert if the input buffer size is too small.
    assert (sizeof(SIL_CPU_MCA_INFO) <= Input->Size);
  }

  CpuMcaInfo = (SIL_CPU_MCA_INFO *)(Input->Buffer);

  for (BankNum = 0; BankNum < XMCA_MAX_BANK_COUNT; BankNum++) {
    McaIpid.Value = xUslRdMsr ((MCA_EXTENSION_BASE +
      ((BankNum * SMCA_REG_PER_BANK) + MCA_IPID_OFFSET)));

    CpuMcaInfo->BankMcaInfo[BankNum].McaIpid.Value = McaIpid.Value;
  }

  return;
}

/*
 * xPrfCollectMcaErrorInfo
 *
 * @brief   Function for performing a search for MCA errors through all banks on
 *          a specific thread.
 *
 * @details It is the responsibility of the Host to ensure the input buffer is
 *          sufficient to contain the SIL_RAS_MCA_ERROR_INFO_V2 (defined in
 *          RasClass-api.h).
 *
 * @note    ***This function is executed on all processors by the Host in a
 *          multi-processor environment.
 *
 * @param Buffer Input pointer to a buffer that will be populated with
 *               SIL_RAS_MCA_ERROR_INFO_V2
 */
void
xPrfCollectMcaErrorInfo (
  void *Buffer
  )
{
  uint32_t                  i;
  uint32_t                  ErrorCount;
  uint8_t                   BankNum;
  SIL_MCA_STATUS_MSR        McaStatusMsr;
  SIL_MCA_IPID_MSR          McaIpidMsr;
  SIL_MCA_DESTAT_MSR        McaDeStatusMsr;
  MCG_CAP_STRUCT            McgCap;
  bool                      DestatExists;
  SIL_RAS_MCA_ERROR_INFO_V2 *RasMcaErrorInfo;

  RasMcaErrorInfo = (SIL_RAS_MCA_ERROR_INFO_V2 *)Buffer;
  McgCap.Value = xUslRdMsr (MSR_MCG_CAP); // MCG_CAP
  BankNum = (uint8_t)McgCap.Field.Count;
  ErrorCount = 0;

  for (i = 0; i < BankNum; i++) {
    McaIpidMsr.Value = xUslRdMsr (
      (MCA_EXTENSION_BASE + ((i * SMCA_REG_PER_BANK) | MCA_IPID_OFFSET))
      );
    if (McaIpidMsr.Field.HardwareID == 0) {
      continue;
    }

    DestatExists = true;
    switch (McaIpidMsr.Field.HardwareID) {
    case MCA_CPU_CORE_ID:
      switch (McaIpidMsr.Field.McaType) {
        case IF_MCA_TYPE:
        case DE_MCA_TYPE:
        case EX_MCA_TYPE:
        case FP_MCA_TYPE:
          DestatExists = false;
          break;
      }
      break;
    case MCA_PARAMETER_BLOCK_ID:
    case MCA_PSP_ID:
    case MCA_SMU_ID:
      DestatExists = false;
      break;
    }

    //Find error log
    McaStatusMsr.Value = xUslRdMsr (
      (MCA_EXTENSION_BASE + ((i * SMCA_REG_PER_BANK) | MCA_STATUS_OFFSET))
      );
    McaDeStatusMsr.Value = 0;
    if (DestatExists) {
      McaDeStatusMsr.Value = xUslRdMsr (
        (MCA_EXTENSION_BASE + ((i * SMCA_REG_PER_BANK) | MCA_DESTAT_OFFSET))
        );
    }

    if (McaStatusMsr.Field.Val || McaDeStatusMsr.Field.Val) {
      //Collect MSR value
      RasMcaErrorInfo->McaBankErrorInfo[ErrorCount].McaBankNumber = i;
      RasMcaErrorInfo->McaBankErrorInfo[ErrorCount].McaStatusMsr =
        McaStatusMsr.Value;
      RasMcaErrorInfo->McaBankErrorInfo[ErrorCount].McaAddrMsr =
        xUslRdMsr (
          (MCA_EXTENSION_BASE + ((i * SMCA_REG_PER_BANK) | MCA_ADDR_OFFSET))
          );
      RasMcaErrorInfo->McaBankErrorInfo[ErrorCount].McaConfigMsr =
        xUslRdMsr (
          (MCA_EXTENSION_BASE + ((i * SMCA_REG_PER_BANK) | MCA_CONFIG_OFFSET))
          );
      RasMcaErrorInfo->McaBankErrorInfo[ErrorCount].McaIpidMsr =
        McaIpidMsr.Value;
      RasMcaErrorInfo->McaBankErrorInfo[ErrorCount].McaSyndMsr =
        xUslRdMsr (
          (MCA_EXTENSION_BASE + ((i * SMCA_REG_PER_BANK) | MCA_SYND_OFFSET))
          );
      RasMcaErrorInfo->McaBankErrorInfo[ErrorCount].McaMisc0Msr =
        xUslRdMsr (
          (MCA_EXTENSION_BASE + ((i * SMCA_REG_PER_BANK) | MCA_MISC0_OFFSET))
          );
      if (McaIpidMsr.Field.HardwareID == MCA_UMC_ID) {
        RasMcaErrorInfo->McaBankErrorInfo[ErrorCount].McaMisc1Msr =
          xUslRdMsr (
            (MCA_EXTENSION_BASE + ((i * SMCA_REG_PER_BANK) | MCA_MISC1_OFFSET))
            );
        RasMcaErrorInfo->McaBankErrorInfo[ErrorCount].McaSynd1Msr =
          xUslRdMsr (
            (MCA_EXTENSION_BASE + ((i * SMCA_REG_PER_BANK) | MCA_SYND1_OFFSET))
            );
        RasMcaErrorInfo->McaBankErrorInfo[ErrorCount].McaSynd2Msr =
          xUslRdMsr (
            (MCA_EXTENSION_BASE + ((i * SMCA_REG_PER_BANK) | MCA_SYND2_OFFSET))
            );
      } else {
        RasMcaErrorInfo->McaBankErrorInfo[ErrorCount].McaMisc1Msr = 0;
        RasMcaErrorInfo->McaBankErrorInfo[ErrorCount].McaSynd1Msr = 0;
        RasMcaErrorInfo->McaBankErrorInfo[ErrorCount].McaSynd2Msr = 0;
      }
      if (DestatExists) {
        RasMcaErrorInfo->McaBankErrorInfo[ErrorCount].McaDeStatMsr =
          xUslRdMsr (
            (MCA_EXTENSION_BASE + ((i * SMCA_REG_PER_BANK) | MCA_DESTAT_OFFSET))
            );
        RasMcaErrorInfo->McaBankErrorInfo[ErrorCount].McaDeAddrMsr =
          xUslRdMsr (
            (MCA_EXTENSION_BASE + ((i * SMCA_REG_PER_BANK) | MCA_DEADDR_OFFSET))
            );
      } else {
        RasMcaErrorInfo->McaBankErrorInfo[ErrorCount].McaDeStatMsr = 0;
        RasMcaErrorInfo->McaBankErrorInfo[ErrorCount].McaDeAddrMsr = 0;
      }
      ErrorCount++;
    }
  }  //for (i = 0; i < BankNum; i++)
  RasMcaErrorInfo->McaBankCount = ErrorCount;
}
