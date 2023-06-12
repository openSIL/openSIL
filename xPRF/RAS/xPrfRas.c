/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  xPrfRas.c
 * @brief Platform Reference Firmware - exposes Platform specific features for
 *        RAS
 */
#include <SilCommon.h>
#include <CoreTopologyService.h>
#include <APOB/Common/ApobCmn.h>
#include <RAS/Common/RasClass-api.h>
#include <RAS/RasIp2Ip.h>
#include <RAS/RasDefs.h>
#include <MsrReg.h>
#include <DF/DfIp2Ip.h>
#include <APOB/ApobIp2Ip.h>


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
  RAS_IP2IP_API *RasApi;

  XPRF_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Status = SilGetIp2IpApi(SilId_RasClass, (void **)&RasApi);
  if (Status != SilPass) {
    XPRF_TRACEPOINT(SIL_TRACE_ERROR, "RAS API not found!\n");
    return Status;
  }

  Status = RasApi->CollectDimmMap(DimmMap);

  return Status;
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
  APOB_IP2IP_API  *ApobIp2IpApi;
  SIL_STATUS      Status;

  Status = SilGetIp2IpApi(SilId_ApobClass, (void **) &ApobIp2IpApi);
  if (Status != SilPass) {
    XPRF_TRACEPOINT(SIL_TRACE_ERROR, "APOB API not found!\n");
    return 0;
  }

  return (uint32_t)(ApobIp2IpApi->ApobGetMaxCpus ());
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
 *          Df->DfGetProcessorInfo()
 *
 * @retval  SilPass             CPU map created successfully
 * @retval  SilInvalidParameter GetProcessorInfo failed
 * @retval  SilInvalidParameter GetCoreTopologyOnDieMax failed
 * @retval  SilNotFound         Info for current ccd, complex, or core not found
 * @retval  SilOutOfBounds      The input buffer is not sufficient for all CPUs
 */
SIL_STATUS
xPrfCollectCpuMap (
  SIL_CPU_INFO                     *RasCpuMap,
  uint32_t                         CpuMapSize,
  uint32_t                         *TotalCpus
  )
{
  SIL_STATUS                       Status;
  uint32_t                         Index;
  uint32_t                         SocketLoop;
  uint32_t                         DieLoop;
  uint32_t                         CcdLoop;
  uint32_t                         ComplexLoop;
  uint32_t                         CoreLoop;
  uint32_t                         CpuInfoSize;
  uint32_t                         MapSizeRemaining;
  DF_IP2IP_API                     *DfIp2IpApi;
  CORE_TOPOLOGY_ITERATION_RESULT   IterationResult;
  uint32_t                         ThreadLoop;
  uint32_t                         PhySocket;
  uint32_t                         PhyDie;
  uint32_t                         PhyCcd;
  uint32_t                         PhyComplex;
  uint32_t                         PhyCore;
  APOB_IP2IP_API                   *ApobIp2IpApi;
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT  ApobCcdLogToPhysMap;

  XPRF_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Status = SilGetIp2IpApi(SilId_DfClass, (void **)&DfIp2IpApi);
  if (Status != SilPass) {
    XPRF_TRACEPOINT(SIL_TRACE_ERROR, "DF API not found!\n");
    return Status;
  }
  if (SilGetIp2IpApi(SilId_ApobClass, (void **) &ApobIp2IpApi) != SilPass) {
    return SilNotFound;
  }

  CpuInfoSize = sizeof (SIL_CPU_INFO);
  // MapSizeRemaining is use to track the available space in RasCpuMap
  MapSizeRemaining = CpuMapSize;

  Index = 0;
  ApobIp2IpApi->ApobGetCcdLogToPhysMap(0, 0, &ApobCcdLogToPhysMap);
  CORE_TOPOLOGY_V3_FOR_EACH_THREAD(&ApobCcdLogToPhysMap,
    IterationResult,
    SocketLoop,
    DieLoop,
    CcdLoop,
    ComplexLoop,
    CoreLoop,
    ThreadLoop
    ) {
    if (Index == xPrfGetMaxCpus ()) {
      XPRF_TRACEPOINT(SIL_TRACE_ERROR, "CPU Index is out of bounds!\n");
      assert(false);
      break;
    }
    PhySocket = SocketLoop;
    PhyDie = DieLoop;
    PhyCcd = CcdLoop;
    PhyComplex = ComplexLoop;
    PhyCore = CoreLoop;
    if (LogicalToPhysicalLocation(&ApobCcdLogToPhysMap, &PhySocket, &PhyDie, &PhyCcd, &PhyComplex, &PhyCore) ==
      SilPass) {
      if (MapSizeRemaining < CpuInfoSize) {
        XPRF_TRACEPOINT(SIL_TRACE_ERROR, "Cpu Map buffer from Host is too small.\n");
        assert(CpuMapSize >= CpuInfoSize);
        return SilOutOfBounds;
      }
      RasCpuMap[Index].ProcessorNumber = Index;    // CPU Logic Number
      RasCpuMap[Index].SocketId = (uint8_t)(PhySocket & 0xFF);
      RasCpuMap[Index].DieId = (uint8_t)(PhyCcd & 0xFF);
      RasCpuMap[Index].CcxId = (uint8_t)(PhyComplex & 0xFF);
      RasCpuMap[Index].CoreId = (uint8_t)(PhyCore & 0xFF);
      RasCpuMap[Index].ThreadID = (uint8_t)(ThreadLoop & 0xFF);
      MapSizeRemaining -= CpuInfoSize;
      Index++;
    }
  }

  *TotalCpus = Index;

  XPRF_TRACEPOINT(SIL_TRACE_EXIT, "\n");

  return SilPass;
}

/*
 * xPrfCollectCoreMcaInfo
 *
 * @brief   Save machine check architecture (MCA) IP ID value to CPU MCA Info
 *          buffer
 *
 * @details Read the MCA IP ID for every MCA bank on the current cpu and save it
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
  if (sizeof (SIL_CPU_MCA_INFO) > Input->Size) {
    XPRF_TRACEPOINT(SIL_TRACE_ERROR,
      "sizeof(SIL_CPU_MCA_INFO): 0x%x, Input->Size: 0x%x\n",
      sizeof (SIL_CPU_MCA_INFO),
      Input->Size
      );
    // Assert if the input buffer size is too small.
    assert(sizeof (SIL_CPU_MCA_INFO) <= Input->Size);
  }

  CpuMcaInfo = (SIL_CPU_MCA_INFO *)(Input->Buffer);

  for (BankNum = 0; BankNum < XMCA_MAX_BANK_COUNT; BankNum++) {
    McaIpid.Value = xUslRdMsr((MCA_EXTENSION_BASE +
      ((BankNum * SMCA_REG_PER_BANK) + MCA_IPID_OFFSET))
      );

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
 * @param   RasMcaErrorInfo   RAS MCA Error info buffer that will be populated by openSIL with MCA error information.
 *
 * @returns SIL_STATUS
 */
SIL_STATUS
xPrfCollectMcaErrorInfo (
  SIL_RAS_MCA_ERROR_INFO_V2 *RasMcaErrorInfo
  )
{
  SIL_STATUS    Status;
  RAS_IP2IP_API *RasApi;

  XPRF_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Status = SilGetIp2IpApi(SilId_RasClass, (void **)&RasApi);
  if (Status != SilPass) {
    XPRF_TRACEPOINT(SIL_TRACE_ERROR, "RAS API not found!\n");
    return Status;
  }

  if (Status == SilPass) {
    RasApi->CollectMcaErrorInfo(RasMcaErrorInfo);
  }

  return Status;
}
