/**
 * @file  xPrfCcx.c
 * @brief Define xPrf Functions
 *        These Function retuns Silicon Data of the OpenSIL Modules.
 *        These Silicon Data will send to HOST through service provided by OpenSIL.
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include "xPRF.h"
#include <string.h>
#include <CommonLib/CpuLib.h>
#include <Pstates.h>
#include <CoreTopologyService.h>
#include <ApobCmn.h>
#include <CCX/Common/CcxApic.h>
#include "xPrfCcx.h"
#include <CCX/CcxClass-api.h>
#include <CCX/Common/Ccx.h>
#include <CcxCmn2Rev.h>
#include <CcxIp2Ip.h>
#include <DF/DfIp2Ip.h>
#include <DF/DfX/FabricAcpiDomain/FabricAcpiDomainInfo.h>

/**
 * xPrfGetThreadsPerCore
 *
 * @brief   This Function is responsible to provide the threads per core values.
 *          Threads per core read from the MSR Register CPUID Fn0x8000001E_EBX
 *          xPRF wrapper function call xPrfGetThreadsPerCore to
 *          get the threads per core values
 *
 * @return  uint8_t Threads per Core values
 */
uint8_t
xPrfGetThreadsPerCore (void)
{
  uint8_t ThreadsPerCore;

  ThreadsPerCore = 0;

  ThreadsPerCore = xUslGetThreadsPerCore();
  XPRF_TRACEPOINT (SIL_TRACE_INFO, "OpenSIL XPrf ThreadsPerCore Value %d\n",ThreadsPerCore);

  return ThreadsPerCore;

}

/**
 * xPrfGetPStatePower
 *
 * @brief   This Function is responsible to provide the Power in mW of the specified PState.
 *          GetPstateInfo take input has current PState value and provide the Power in mW.
 *          xPRF wrapper function call xPrfGetPStatePower to
 *          get Power in mW of the specified PState.
 *
 * @return  uint32_t Power in mW of the specified PState
 */
uint32_t
xPrfGetPStatePower(void)
{
  PSTATE PstateSts;
  uint32_t Frequency;
  uint32_t VoltageInuV;
  uint32_t PowerInmW;
  bool     PstateStatus;

  PstateSts = (PSTATE)GetCurrentPstate();
  // Get the Pstate information including Freq, Voltage, Power.
  GetPstateInfo(PstateSts,&Frequency,&VoltageInuV,&PowerInmW,&PstateStatus);
  XPRF_TRACEPOINT (SIL_TRACE_INFO, "OpenSIL XPrf PStatePower Value %d\n",PowerInmW);

  return PowerInmW;
}

/**
 * TotalNumberApicId
 *
 * @brief   This Function is responsible to provide Total Number Of APIC ID's
 *
 * @return  uint32_t Total Number of Apic ID
 */
static uint32_t
TotalNumberApicId(void) {

  uint32_t NumberOfSockets  ;
  uint32_t NumberOfDies     ;
  uint32_t TotalApicId      ;
  uint32_t SocketLoop       ;
  uint32_t DieLoop          ;
  uint32_t NumberOfCcds     ;
  uint32_t NumberOfComplexes;
  uint32_t NumberOfCores    ;
  uint32_t NumberOfThreads  ;
  DF_IP2IP_API* DfIp2IpApi;
  SIL_STATUS Status;

  Status = SilGetIp2IpApi (SilId_DfClass, (void **)&DfIp2IpApi);
  assert (Status == SilPass);

  NumberOfSockets   = 0;
  NumberOfDies      = 0;
  TotalApicId       = 0;
  SocketLoop        = 0;
  DieLoop           = 0;
  NumberOfCcds      = 0;
  NumberOfComplexes = 0;
  NumberOfCores     = 0;
  NumberOfThreads   = 0;
  // Get Number of Socket information of the overall system.
  DfIp2IpApi->DfGetSystemInfo (&NumberOfSockets, NULL, NULL, NULL, NULL);
  //Get Socket and Number of dies information about the processor installed in the socket.
  DfIp2IpApi->DfGetProcessorInfo (SocketLoop, &NumberOfDies, NULL);
  // Get information about the layout of the cores on the given die.
  GetCoreTopologyOnDie (SocketLoop, DieLoop, &NumberOfCcds,
                        &NumberOfComplexes, &NumberOfCores, &NumberOfThreads);
  TotalApicId = (NumberOfSockets*NumberOfDies*NumberOfCcds*
                        NumberOfComplexes*NumberOfCores*NumberOfThreads);

  XPRF_TRACEPOINT (SIL_TRACE_INFO,"TotalNumberApicId %d\n",TotalApicId);
  return (TotalApicId);
}

/**
 * xPrfCreateSratApicEntry
 *
 * @brief create SRAT Local APIC structure
 *
 * @param   SratApic      Input buffer for the SRAT Local APIC structure.  The Host is responsible
 *                        for ensuring the buffer size is sufficient to contain
 *                        SIL_SRAT_APIC or SIL_SRAT_x2APIC structure.
 *                        On output, the buffer is populated with SIL_SRAT_APIC or SIL_SRAT_x2APIC.
 * @param   SratApicSize  The size of the SratApicSize input buffer from the Host. This
 *                        is used by openSIL to ensure the input buffer size is
 *                        sufficient to contain SIL_SRAT_APIC or SIL_SRAT_x2APIC.
 *
 * @param   ApicModeValue APIC operation modes. Options are:
 *                        ApicMode              - Small systems (< 128 cores).
 *                        x2ApicMode            - Large systems.
 *                        ApicCompatibilityMode - Allow the code to choose depending on the quantity
 *                        of cores present. X2 mode is preferred.
 *
 * @param  SratTableLength xPrfCreateSratApicEntry xPrf Service update Total SRAT Table Length into
 *                        "SratTableLength" variable.
 *
 * @return  SIL_STATUS
 *
 * @retval  SilPass         The SRAT APIC Entry was successfully populated.
 * @retval  SilOutOfBounds  The SRAT APIC Entry input buffer size was not sufficient.
 * @retval  SilNotFound     IP transfer table was not found
 */
SIL_STATUS
xPrfCreateSratApicEntry (
  uint8_t   *SratApic,
  uint32_t  SratApicSize,
  uint8_t   ApicModeValue,
  uint32_t  *SratTableLength
  )
{
  uint32_t              ApicId;
  uint32_t              SocketLoop;
  uint32_t              DieLoop;
  uint32_t              CcdLoop;
  uint32_t              ComplexLoop;
  uint32_t              CoreLoop;
  uint32_t              ThreadsLoop;
  uint32_t              NumberOfSockets;
  uint32_t              NumberOfDies;
  uint32_t              NumberOfCcds;
  uint32_t              NumberOfComplexes;
  uint32_t              NumberOfCores;
  uint32_t              NumberOfThreads;
  uint32_t              Domain;
  uint8_t               ApicMode;
  SIL_SRAT_APIC         *ApicEntry;
  SIL_SRAT_x2APIC       *X2ApicEntry;
  CCXCLASS_DATA_BLK     *CcxData;
  uint8_t               PhyCcdNum;
  uint8_t               PhyCcxNum;
  uint8_t               PhyCoreNum;
  uint32_t              ApobInstanceId;
  SIL_STATUS            Status;
  CCX_IP2IP_API         *CcxIp2Ip;
  DF_IP2IP_API          *DfIp2IpApi;
  uint32_t              NumberOfApicIds;

  if (SilGetIp2IpApi (SilId_CcxClass, (void **)(&CcxIp2Ip)) != SilPass) {
    return SilNotFound;
  }
  if (SilGetIp2IpApi (SilId_DfClass, (void**) &DfIp2IpApi) != SilPass) {
    return SilNotFound;
  }

  if ((ApicModeValue == xApicMode) &&
      (((sizeof(SIL_SRAT_APIC)) * (TotalNumberApicId())) <= SratApicSize)) {
       XPRF_TRACEPOINT (SIL_TRACE_INFO,"SratApicEntry buffer from host match.\n");
  } else {
    if ((ApicModeValue == x2ApicMode) &&
        (((sizeof(SIL_SRAT_x2APIC)) * (TotalNumberApicId())) <= SratApicSize)) {
         XPRF_TRACEPOINT (SIL_TRACE_INFO,"SratX2ApicEntry buffer from host match.\n");
    } else {
      XPRF_TRACEPOINT (SIL_TRACE_INFO,"SratApicEntry buffer from host does not match.\n");
      return SilOutOfBounds;
    }
  }

  CcxData = (CCXCLASS_DATA_BLK *)SilFindStructure (SilId_CcxClass,  0);
  if (CcxData == NULL) {
    XPRF_TRACEPOINT (SIL_TRACE_INFO,"SratApicEntry CcxData buffer not found.\n");
    return SilNotFound; // Could not find the IP input block
  }

  // get Apic Mode
  ApicMode = CcxData->CcxInputBlock.AmdApicMode;
  NumberOfApicIds = 0;
  // Get Number of Socket information of the overall system.
  DfIp2IpApi->DfGetSystemInfo (&NumberOfSockets, NULL, NULL, NULL, NULL);
  for (SocketLoop = 0; SocketLoop < NumberOfSockets; SocketLoop++) {
    //Get Socket and Number of dies information about the processor installed in the socket.
    if (DfIp2IpApi->DfGetProcessorInfo (SocketLoop, &NumberOfDies, NULL) == SilPass) {
      for (DieLoop = 0; DieLoop < NumberOfDies; DieLoop++) {
        ApobInstanceId = ((uint32_t) SocketLoop << 8) | (uint32_t) DieLoop;
        // Get information about the layout of the cores on the given die.
        GetCoreTopologyOnDie (SocketLoop, DieLoop, &NumberOfCcds,
                              &NumberOfComplexes, &NumberOfCores, &NumberOfThreads);
          for (CcdLoop = 0; CcdLoop < NumberOfCcds; CcdLoop++) {
            Status = ApobGetPhysCcdNumber (ApobInstanceId, (uint32_t)CcdLoop, &PhyCcdNum);
            if (Status != SilPass || PhyCcdNum == CCX_NOT_PRESENT) {
              continue;
            }
            for (ComplexLoop = 0; ComplexLoop < NumberOfComplexes; ComplexLoop++) {
              Status = ApobGetPhysComplexNumber (ApobInstanceId, (uint32_t) CcdLoop,
                                                 (uint32_t) ComplexLoop, &PhyCcxNum);
              if (Status != SilPass || PhyCcxNum == CCX_NOT_PRESENT) {
                continue;
              }
              if (DfIp2IpApi->DfDomainXlat (SocketLoop, DieLoop, CcdLoop, ComplexLoop, &Domain) ==  SilPass) {
                for (CoreLoop = 0; CoreLoop < NumberOfCores; CoreLoop++) {
                  Status = ApobGetPhysCoreNumber (ApobInstanceId, (uint32_t) CcdLoop,
                                        (uint32_t) ComplexLoop, (uint32_t) CoreLoop, &PhyCoreNum);
                  if (Status != SilPass || PhyCoreNum == CCX_NOT_PRESENT) {
                    continue;
                  }
                  for (ThreadsLoop = 0; ThreadsLoop < NumberOfThreads; ThreadsLoop++) {
                    // Calculate the APIC Id based on the provided inputs and update into
                    // PState structure
                    ApicId = CcxIp2Ip->CalcLocalApic(SocketLoop, DieLoop, CcdLoop,
                                                ComplexLoop, CoreLoop, ThreadsLoop);
                    if ((ApicMode == x2ApicMode) || ((ApicMode == ApicCompatibilityMode)
                         && (ApicId >= SIL_XAPIC_ID_MAX))) {
                      X2ApicEntry                        = (SIL_SRAT_x2APIC *) SratApic;
                      SratApic                          += sizeof (SIL_SRAT_x2APIC);
                      X2ApicEntry->Type                  = SIL_SRAT_LOCAL_X2_APIC_TYPE;
                      X2ApicEntry->Length                = sizeof (SIL_SRAT_x2APIC);
                      X2ApicEntry->ProximityDomain       = Domain;
                      X2ApicEntry->x2ApicId              = ApicId;
                      X2ApicEntry->Flags.Enabled         = 1;
                      X2ApicEntry->ClockDomain           = 0;
                      NumberOfApicIds ++;
                    } else if (ApicId < SIL_XAPIC_ID_MAX) {
                      ApicEntry                          = (SIL_SRAT_APIC *) SratApic;
                      SratApic                          += sizeof (SIL_SRAT_APIC);
                      ApicEntry->Type                    = SIL_SRAT_LOCAL_APIC_TYPE;
                      ApicEntry->Length                  = sizeof (SIL_SRAT_APIC);
                      ApicEntry->ProximityDomain_7_0     = (uint8_t) Domain;
                      ApicEntry->ProximityDomain_31_8[0] = (uint8_t) (Domain >> 8);
                      ApicEntry->ProximityDomain_31_8[1] = (uint8_t) (Domain >> 16);
                      ApicEntry->ProximityDomain_31_8[2] = (uint8_t) (Domain >> 24);
                      ApicEntry->ApicId                  = (uint8_t) ApicId;
                      ApicEntry->Flags.Enabled           = 1;
                      ApicEntry->LocalSapicEid           = 0;
                      ApicEntry->ClockDomain             = 0;
                      NumberOfApicIds ++;
                    }
                  }
                }
              }
            }
          }
      }
    }
  }
  if ((ApicMode == x2ApicMode) || ((ApicMode == ApicCompatibilityMode)
      && (ApicId >= SIL_XAPIC_ID_MAX))) {
      *SratTableLength = (NumberOfApicIds * (sizeof (SIL_SRAT_x2APIC)));
  } else if (ApicId < SIL_XAPIC_ID_MAX) {
      *SratTableLength = (NumberOfApicIds * (sizeof (SIL_SRAT_APIC)));
  }

  return SilPass;
}

/**
 * xPrfPStateGatherData
 *
 * @brief   This xPrf Service Provide the  PState information.
 *
 * @details This function will collect all PState information from the MSRs and fill up the
 *          Ouput Buffer.
 *          PState Informations are  : Physical socket number
 *                                     Logical core number in this socket
 *                           Pstate enable status
 *                           Core Frequency in MHz
 *                           Power in MilliWatts
 *                           Software P-state number
 *
 * @param   PStateData      Input buffer for the PState System Info structure.  The Host is responsible
 *                          for ensuring the buffer size is sufficient to contain
 *                          PSTATE_SYS_INFO  structure.
 *                          On output, the buffer is populated with PSTATE_SYS_INFO.
 * @param   PStateDataSize  The size of the PStateDataSize input buffer from the Host. This
 *                          is used by openSIL to ensure the input buffer size is
 *                          sufficient to contain PSTATE_SYS_INFO .
 *
 * @return  SIL_STATUS
 *
 * @retval  SilPass         The PState System Info was successfully populated.
 * @retval  SilOutOfBounds  The PState System Info input buffer size was not sufficient.
 * @retval  SilNotFound     IP transfer table was not found
 */
SIL_STATUS
xPrfPStateGatherData (
  PSTATE_SYS_INFO      *PStateData,
  uint32_t             PStateDataSize
  )
{
  uint32_t               LocalApicIdLoop;
  uint32_t               TableSize;
  uint32_t               PstateLoop;
  uint32_t               TotalEnabledPStates;
  uint32_t               NumberOfCcds;
  uint32_t               NumberOfComplexes;
  uint32_t               NumberOfCores;
  uint32_t               NumberOfThreads;
  uint32_t               CcdLoop;
  uint32_t               ComplexLoop;
  uint32_t               CoreLoop;
  uint32_t               ThreadLoop;
  uint32_t               LogicalCoreNum;
  uint32_t               MaxSwState;
  uint32_t               NumberOfBoostPstate;
  uint32_t               Frequency;
  uint32_t               VoltageInuV;
  uint32_t               PowerInmW;
  uint32_t               NumberOfSockets;
  uint32_t               NumberOfDies;
  uint32_t               SocketLoop;
  uint32_t               DieLoop;
  PSTATE_SOCKET_INFO     *PstateSocketInfo;
  PSTATE_VALUES          *PstateStructure;
  bool                   PstateStatus;
  CCXCLASS_DATA_BLK      *CcxData;
  uint8_t                PhyCcdNum;
  uint8_t                PhyCcxNum;
  uint8_t                PhyCoreNum;
  uint32_t               ApobInstanceId;
  SIL_STATUS             Status;
  uint32_t               PstateTransLatency;
  CCX_IP2IP_API          *CcxIp2Ip;
  DF_IP2IP_API           *DfIp2IpApi;

  if (SilGetIp2IpApi (SilId_CcxClass, (void **)(&CcxIp2Ip)) != SilPass) {
    return SilNotFound;
  }
  if (SilGetIp2IpApi (SilId_DfClass, (void**) &DfIp2IpApi) != SilPass) {
    return SilNotFound;
  }

  PstateTransLatency  = 0;

  if ((sizeof(PSTATE_SYS_INFO)) <= PStateDataSize) {
    XPRF_TRACEPOINT (SIL_TRACE_INFO,"PStateData buffer from host match.\n");
  } else {
    XPRF_TRACEPOINT (SIL_TRACE_INFO,"PStateData buffer from host does not match.\n");
    return SilOutOfBounds;
  }

  CcxData = (CCXCLASS_DATA_BLK *)SilFindStructure (SilId_CcxClass,0);
  if (CcxData == NULL) {
    XPRF_TRACEPOINT (SIL_TRACE_INFO,"PStateGatherData CcxData buffer not found.\n");
    return SilNotFound; // Could not find the IP input block
  }

  // Get maximum number of Pstates in NumberOfPstate
  GetPstateNumber (&NumberOfBoostPstate,&MaxSwState);
  MaxSwState = MaxSwState - NumberOfBoostPstate;

  // Get Number of Socket information of the overall system.
  DfIp2IpApi->DfGetSystemInfo (&NumberOfSockets, NULL, NULL, NULL, NULL);

  // Calculate the Table Size
  TableSize = (uint32_t) (sizeof (PSTATE_SYS_INFO) + (((MaxSwState*sizeof (PSTATE_VALUES))
                          + sizeof (PSTATE_SOCKET_INFO))* NumberOfSockets));
  // update PState Structure
  PStateData->TotalSocketInSystem = (uint8_t) NumberOfSockets;
  PStateData->SizeOfBytes         = TableSize;
  PstateSocketInfo                = (PSTATE_SOCKET_INFO *)PStateData->PStateSocketStruct;

  for (SocketLoop = 0; SocketLoop < NumberOfSockets; SocketLoop++) {
    // Calculate number of logical cores
    LogicalCoreNum  = 0;
    LocalApicIdLoop = 0;
    //Get Socket and Number of dies information about the processor installed in the socket.
    DfIp2IpApi->DfGetProcessorInfo (SocketLoop, &NumberOfDies, NULL);
    for (DieLoop = 0; DieLoop < NumberOfDies; DieLoop++) {
      ApobInstanceId = (((uint32_t) SocketLoop << 8) | ((uint32_t) DieLoop));
      // Get information about the layout of the cores on the given die.
      GetCoreTopologyOnDie (SocketLoop, DieLoop, &NumberOfCcds, &NumberOfComplexes,
                            &NumberOfCores, &NumberOfThreads);
      // Calculate number of logical cores
      LogicalCoreNum += NumberOfCcds * NumberOfComplexes * NumberOfCores * NumberOfThreads;
      for (CcdLoop = 0; CcdLoop < NumberOfCcds; CcdLoop++) {
        Status = ApobGetPhysCcdNumber (ApobInstanceId, (uint32_t)CcdLoop, &PhyCcdNum);
        if (Status != SilPass || PhyCcdNum == CCX_NOT_PRESENT) {
          continue;
        }
        for (ComplexLoop = 0; ComplexLoop < NumberOfComplexes; ComplexLoop++) {
          Status = ApobGetPhysComplexNumber (ApobInstanceId, (uint32_t) CcdLoop,
                                             (uint32_t) ComplexLoop, &PhyCcxNum);
          if (Status != SilPass || PhyCcxNum == CCX_NOT_PRESENT) {
            continue;
          }
          for (CoreLoop = 0; CoreLoop < NumberOfCores; CoreLoop++) {
            Status = ApobGetPhysCoreNumber (ApobInstanceId, (uint32_t) CcdLoop,
                                 (uint32_t) ComplexLoop, (uint32_t) CoreLoop, &PhyCoreNum);
            if (Status != SilPass || PhyCoreNum == CCX_NOT_PRESENT) {
              continue;
            }
            for (ThreadLoop = 0; ThreadLoop < NumberOfThreads; ThreadLoop++) {
              if (LocalApicIdLoop < ((sizeof(PstateSocketInfo->LocalApicId))/
                                     (sizeof((PstateSocketInfo->LocalApicId)[0])))) {
              // Calculate the APIC Id based on the provided inputs and update into PState structure
                PstateSocketInfo->LocalApicId[LocalApicIdLoop] = CcxIp2Ip->CalcLocalApic(SocketLoop,
                                  DieLoop, CcdLoop, ComplexLoop, CoreLoop, ThreadLoop);
              } else {
                assert (false);
              }
              LocalApicIdLoop++;
            }
          }
        }
      }
    }

  PstateSocketInfo->SocketNumber        = (uint8_t) SocketLoop;
  PstateSocketInfo->TotalLogicalCores   = (uint8_t) LogicalCoreNum;
  PstateSocketInfo->CreateAcpiTables    = true;
  PstateSocketInfo->PStateMaxValue      = (uint8_t) MaxSwState;

  // Get transition latency
  GetPstateTransLatency (&PstateTransLatency);
  PstateSocketInfo->TransitionLatency   = PstateTransLatency;

  switch (CcxData->CcxInputBlock.AmdPstatePolicy) {
    case 0:
      PstateSocketInfo->IsPsdDependent = false;
    break;
    case 1:
      PstateSocketInfo->IsPsdDependent = true;
    break;
    case 2:
      PstateSocketInfo->IsPsdDependent = false;
    break;
    default:
      assert (false);
    break;
  }

  PstateStructure = PstateSocketInfo->PStateStruct;
  TotalEnabledPStates = 0;

  for (PstateLoop = 0; PstateLoop <= MaxSwState; PstateLoop++) {
    memset(PstateStructure,0,sizeof (PSTATE_VALUES));
    // Get the Pstate information including Freq, Voltage, Power.
    if (GetPstateInfo ((Pstate0 + PstateLoop), (uint32_t *)&Frequency,
                       (uint32_t *) &VoltageInuV,(uint32_t *) &PowerInmW, &PstateStatus) == SilPass) {
      // Update Pstate enable status,Core Frequency in MHz, Power in MilliWatts
      // Software P-state number
      PstateStructure->CoreFreq       = (uint32_t) Frequency;
      PstateStructure->Power          = (uint32_t) PowerInmW;
      PstateStructure->SwPstateNumber = PstateLoop;
      PstateStructure->PStateEnable   = PstateStatus;
      PstateStructure++;
      TotalEnabledPStates++;
    }
  }

  // Do not create ACPI Tables if there is one or less than one PState is enabled
  if (TotalEnabledPStates <= 1) {
    PstateSocketInfo->CreateAcpiTables = false;
  }
  PstateSocketInfo = (PSTATE_SOCKET_INFO *) ((uint8_t *) PstateSocketInfo +
                      sizeof (PSTATE_SOCKET_INFO) + sizeof (PSTATE_VALUES) * MaxSwState);
  }

  return SilPass;
}

/**
 * xPrfGetCratHsaProcInfo
 *
 * @brief   This xPrf Service Provide the CRAT information about the HSA.
 *
 * @details This function will collect CRAT information about the HSA and fill up the
 *          Ouput Buffer.
 *          CRAT Informations are  : proximity node
 *                                  logical processor included in this HSA proximity domain
 *                                  count of execution units present in the APU node.
 *
 * @param  CratHsaProcData      Input buffer for the CRAT Info structure about the HSA. The
 *                              Host is responsible for ensuring the buffer size is
 *                              sufficient to contain SIL_CRAT_HSA_PROC_INFO  structure.
 *                              On output, the buffer is populated with SIL_CRAT_HSA_PROC_INFO.
 * @param  CratHsaProcDataSize  The size of the CratHsaProcDataSize input buffer from the Host.
 *                              This is used by openSIL to ensure the input buffer size is
 *                              sufficient to contain SIL_CRAT_HSA_PROC_INFO .
 *
 * @return  SIL_STATUS
 *
 * @retval  SilPass         The PState System Info was successfully populated.
 * @retval  SilOutOfBounds  The PState System Info input buffer size was not sufficient.
 * @retval  SilNotFound     IP transfer table was not found
 */
SIL_STATUS
xPrfGetCratHsaProcInfo (
  uint8_t   *CratHsaProcData,
  uint32_t  CratHsaProcDataSize
  )
{
  uint32_t                     SocketLoop;
  uint32_t                     DieLoop;
  uint32_t                     NumberOfSockets;
  uint32_t                     NumberOfDies;
  uint32_t                     NumberOfCcds;
  uint32_t                     NumberOfComplexes;
  uint32_t                     NumberOfCores;
  uint32_t                     NumberOfThreads;
  uint32_t                     Domain;
  uint32_t                     PreDomain;
  SIL_CRAT_HSA_PROC_INFO       *CratHsaEntry;
  CCX_IP2IP_API                *CcxIp2Ip;
  DF_IP2IP_API                 *DfIp2IpApi;

  if (SilGetIp2IpApi (SilId_CcxClass, (void **)(&CcxIp2Ip)) != SilPass) {
    return SilNotFound;
  }
  if (SilGetIp2IpApi (SilId_DfClass, (void**) &DfIp2IpApi) != SilPass) {
    return SilNotFound;
  }

  NumberOfSockets = 0;
  NumberOfDies    = 0;
  SocketLoop      = 0;
  PreDomain       = 0;
  CratHsaEntry    = NULL;

  // Get Number of Socket information of the overall system.
  DfIp2IpApi->DfGetSystemInfo (&NumberOfSockets, &NumberOfDies, NULL, NULL, NULL);

  if ((((sizeof(SIL_CRAT_HSA_PROC_INFO)) * (NumberOfDies))
                                            <= CratHsaProcDataSize)) {
    XPRF_TRACEPOINT (SIL_TRACE_INFO,"GetCratHsaProcInfo buffer from host match.\n");
  } else {
    XPRF_TRACEPOINT (SIL_TRACE_INFO,"GetCratHsaProcInfo buffer from host does not match.\n");
    return SilOutOfBounds;
  }

  for (SocketLoop = 0; SocketLoop < NumberOfSockets; SocketLoop++) {
    //Get Socket and Number of dies information about the processor installed in the socket.
    if (DfIp2IpApi->DfGetProcessorInfo ( SocketLoop, &NumberOfDies, NULL) == SilPass) {
      for (DieLoop = 0; DieLoop < NumberOfDies; DieLoop++) {
        if (DfIp2IpApi->DfDomainXlat (SocketLoop, DieLoop, 0, 0, &Domain) ==  SilPass) {
          if ((Domain != PreDomain) || (Domain == 0)) {
            CratHsaEntry                   = (SIL_CRAT_HSA_PROC_INFO *) CratHsaProcData;
            CratHsaProcData               += sizeof (SIL_CRAT_HSA_PROC_INFO);
            CratHsaEntry->ProximityNode    = Domain;
            // Calculate the APIC Id based on the provided inputs and update into
            // PState structure
            CratHsaEntry->ProcessorIdLow   = CcxIp2Ip->CalcLocalApic (SocketLoop, DieLoop,
                                                                        0, 0, 0, 0);
            CratHsaEntry->NumCPUCores      = 0;
            PreDomain = Domain;
          }
          // Get information about the layout of the cores on the given die.
          GetCoreTopologyOnDie (SocketLoop, DieLoop, &NumberOfCcds, &NumberOfComplexes,
                                &NumberOfCores, &NumberOfThreads);
          CratHsaEntry->NumCPUCores += (uint16_t) (NumberOfCcds * NumberOfComplexes *
                                                 NumberOfCores * NumberOfThreads);
        }
      }
    }
  }

  return SilPass;
}

/**
 * xPrfCratCacheEntry
 *
 * @brief   This xPrf Service Provide the CRAT Cache information .
 *
 * @details This function will collect CRAT Cache information and fill up the
 *          Ouput Buffer.
 *          CRAT  Cache Informations are  : Low value of a logical processor
 *                                                    which includes this component
 *
 * @param  CratCacheEntry       Input buffer for the CRAT Cache Info structure. The
 *                              Host is responsible for ensuring the buffer size is
 *                              sufficient to contain SIL_CRAT_CACHE  structure.
 *                              On output, the buffer is populated with SIL_CRAT_CACHE.
 * @param  CratCacheEntrySize   The size of the CratCacheEntrySize input buffer from the Host.
 *                              This is used by openSIL to ensure the input buffer size is
 *                              sufficient to contain SIL_CRAT_CACHE .
 *
 * @return  SIL_STATUS
 *
 * @retval  SilPass         The PState System Info was successfully populated.
 * @retval  SilOutOfBounds  The PState System Info input buffer size was not sufficient.
 * @retval  SilNotFound     IP transfer table was not found
 */
SIL_STATUS
xPrfCratCacheEntry (
  uint8_t   *CratCacheEntry,
  uint32_t  CratCacheEntrySize
  )
{
  uint32_t        SocketLoop;
  uint32_t        DieLoop;
  uint32_t        CcdLoop;
  uint32_t        ComplexLoop;
  uint32_t        CoreLoop;
  uint32_t        ThreadsLoop;
  uint32_t        NumberOfSockets;
  uint32_t        NumberOfDies;
  uint32_t        NumberOfCcds;
  uint32_t        NumberOfComplexes;
  uint32_t        NumberOfCores;
  uint32_t        NumberOfThreads;
  SIL_CRAT_CACHE  *CratCache;
  uint8_t         PhyCcdNum;
  uint8_t         PhyCcxNum;
  uint8_t         PhyCoreNum;
  uint32_t        ApobInstanceId;
  SIL_STATUS      Status;
  CCX_IP2IP_API   *CcxIp2Ip;
  DF_IP2IP_API    *DfIp2IpApi;

  if (SilGetIp2IpApi (SilId_CcxClass, (void **)(&CcxIp2Ip)) != SilPass) {
    return SilNotFound;
  }
  if (SilGetIp2IpApi (SilId_DfClass, (void**) &DfIp2IpApi) != SilPass) {
    return SilNotFound;
  }

  SocketLoop        = 0;
  DieLoop           = 0;
  CcdLoop           = 0;
  ComplexLoop       = 0;
  CoreLoop          = 0;
  ThreadsLoop       = 0;
  NumberOfSockets   = 0;
  NumberOfDies      = 0;
  NumberOfCcds      = 0;
  NumberOfComplexes = 0;
  NumberOfCores     = 0;
  NumberOfThreads   = 0;

  if (((sizeof(SIL_CRAT_CACHE)) * (TotalNumberApicId())) <= CratCacheEntrySize) {
    XPRF_TRACEPOINT (SIL_TRACE_INFO,"xPrfCratCacheEntry buffer from host match.\n");
  } else {
    XPRF_TRACEPOINT (SIL_TRACE_INFO,"xPrfCratCacheEntry buffer from host does not match.\n");
    return SilOutOfBounds;
  }

  // Get Number of Socket information of the overall system.
  DfIp2IpApi->DfGetSystemInfo (&NumberOfSockets, NULL, NULL, NULL, NULL);
    for (SocketLoop = 0; SocketLoop < NumberOfSockets; SocketLoop++) {
      //Get Socket and Number of dies information about the processor installed in the socket.
      if (DfIp2IpApi->DfGetProcessorInfo (SocketLoop, &NumberOfDies, NULL) == SilPass) {
        for (DieLoop = 0; DieLoop < NumberOfDies; DieLoop++) {
          ApobInstanceId = ((uint32_t) SocketLoop << 8) | (uint32_t) DieLoop;
          // Get information about the layout of the cores on the given die.
          GetCoreTopologyOnDie (SocketLoop, DieLoop, &NumberOfCcds, &NumberOfComplexes,
                                &NumberOfCores, &NumberOfThreads);
          for (CcdLoop = 0; CcdLoop < NumberOfCcds; CcdLoop++) {
              Status = ApobGetPhysCcdNumber (ApobInstanceId, (uint32_t)CcdLoop, &PhyCcdNum);
              if (Status != SilPass || PhyCcdNum == CCX_NOT_PRESENT) {
                continue;
              }
            for (ComplexLoop = 0; ComplexLoop < NumberOfComplexes; ComplexLoop++) {
              Status = ApobGetPhysComplexNumber (ApobInstanceId, (uint32_t) CcdLoop,
                                                 (uint32_t) ComplexLoop, &PhyCcxNum);
              if (Status != SilPass || PhyCcxNum == CCX_NOT_PRESENT) {
                continue;
              }
              for (CoreLoop = 0; CoreLoop < NumberOfCores; CoreLoop++) {
                Status = ApobGetPhysCoreNumber (ApobInstanceId, (uint32_t) CcdLoop,
                                  (uint32_t) ComplexLoop, (uint32_t) CoreLoop, &PhyCoreNum);
                if (Status != SilPass || PhyCoreNum == CCX_NOT_PRESENT) {
                  continue;
                }
                for (ThreadsLoop = 0; ThreadsLoop < NumberOfThreads; ThreadsLoop++) {
                  CratCache                   = (SIL_CRAT_CACHE *) CratCacheEntry;
                  // Calculate the APIC Id based on the provided inputs
                  CratCache->ProcessorIdLow   = CcxIp2Ip->CalcLocalApic (SocketLoop,DieLoop,
                                                  CcdLoop, ComplexLoop, CoreLoop, ThreadsLoop);
                  CratCache->CacheLatency     = 1;
                  CratCache->LinesPerTag      = 1;
                }
              }
            }
          }
        }
      }
    }

  return SilPass;
}
