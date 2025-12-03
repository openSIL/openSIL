/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  xPrfCcx.c
 * @brief Define xPrf Functions
 *        These Function returns Silicon Data of the OpenSIL Modules.
 *        These Silicon Data will send to HOST through service provided by OpenSIL.
 */

#include <SilCommon.h>
#include "xPRF.h"
#include <string.h>
#include <CommonLib/CpuLib.h>
#include <Pstates.h>
#include <CoreTopologyService.h>
#include <APOB/Common/ApobCmn.h>
#include <CCX/Common/CcxApic.h>
#include <CCX/CcxIp2Ip.h>
#include "xPrfCcx.h"
#include <CCX/CcxClass-api.h>
#include <CCX/Common/Ccx.h>
#include <CcxCmn2Rev.h>
#include <CcxIp2Ip.h>
#include <DF/DfIp2Ip.h>
#include <APOB/ApobIp2Ip.h>
#include <SMU/Common/SmuCommon.h>
#include <SMU/Common/SmuCmn2Rev.h>
#include <Nbio/NbioIp2Ip.h>


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

  ThreadsPerCore = xUslGetThreadsPerCore ();
  XPRF_TRACEPOINT(SIL_TRACE_INFO, "OpenSIL XPrf ThreadsPerCore Value %d\n", ThreadsPerCore);

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
SIL_STATUS
xPrfGetPStatePower (
  uint32_t  PstateSts,
  uint32_t *PowerInmW
  )
{
  uint32_t Frequency;
  uint32_t VoltageInuV;
  bool     PstateStatus;
  CCX_IP2IP_API *CcxIp2IpApi;

  if (SilGetIp2IpApi(SilId_CcxClass, (void **)&CcxIp2IpApi) != SilPass) {
    XPRF_TRACEPOINT(SIL_TRACE_INFO, "OpenSIL XPrf unable to get CCX I2I API. %d\n");
    return SilNotFound;
  }

  if (CcxIp2IpApi->GetPstateInfo((PSTATE)PstateSts,
    &Frequency,
    &VoltageInuV,
    PowerInmW,
    &PstateStatus
    ) == SilPass) {
    XPRF_TRACEPOINT(SIL_TRACE_INFO, "OpenSIL XPrf PStatePower Value %d\n", *PowerInmW);
  } else {
    XPRF_TRACEPOINT(SIL_TRACE_INFO, "OpenSIL XPrf unable to set PstatePower info.\n");
    return SilAborted;
  }

  return SilPass;
}

/**
 * TotalNumberApicId
 *
 * @brief   This Function is responsible to provide Total Number Of APIC ID's
 *
 * @return  uint32_t Total Number of Apic ID
 */
static uint32_t
TotalNumberApicId (void) {

  uint32_t NumberOfSockets;
  uint32_t NumberOfDies;
  uint32_t TotalApicId;
  uint32_t SocketLoop;
  uint32_t DieLoop;
  uint32_t NumberOfCcds;
  uint32_t NumberOfComplexes;
  uint32_t NumberOfCores;
  uint32_t NumberOfThreads;
  DF_IP2IP_API *DfIp2IpApi;
  SIL_STATUS Status;

  Status = SilGetIp2IpApi(SilId_DfClass, (void **)&DfIp2IpApi);
  assert(Status == SilPass);

  NumberOfSockets = 0;
  NumberOfDies = 0;
  TotalApicId = 0;
  SocketLoop = 0;
  DieLoop = 0;
  NumberOfCcds = 0;
  NumberOfComplexes = 0;
  NumberOfCores = 0;
  NumberOfThreads = 0;
  // Get Number of Socket information of the overall system.
  DfIp2IpApi->DfGetSystemInfo(&NumberOfSockets, NULL, NULL, NULL, NULL);
  //Get Socket and Number of dies information about the processor installed in the socket.
  DfIp2IpApi->DfGetProcessorInfo(SocketLoop, &NumberOfDies, NULL);
  // Get information about the layout of the cores on the given die.
  GetCoreTopologyOnDieMax(SocketLoop,
    DieLoop,
    &NumberOfCcds,
    &NumberOfComplexes,
    &NumberOfCores,
    &NumberOfThreads
    );
  TotalApicId = (NumberOfSockets * NumberOfDies * NumberOfCcds *
    NumberOfComplexes * NumberOfCores * NumberOfThreads);

  XPRF_TRACEPOINT(SIL_TRACE_INFO, "TotalNumberApicId %d\n", TotalApicId);
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
  uint8_t               *SratApic,
  uint32_t              SratApicSize,
  uint8_t               ApicModeValue,
  uint32_t              *SratTableLength
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
  SIL_SRAT_APIC         *ApicEntry;
  SIL_SRAT_x2APIC       *X2ApicEntry;
  CCXCLASS_DATA_BLK     *CcxData;
  SIL_STATUS            Status;
  CCX_IP2IP_API         *CcxIp2Ip;
  DF_IP2IP_API          *DfIp2IpApi;
  APOB_IP2IP_API        *ApobIp2IpApi;
  uint32_t              NumberOfApicIds;
  uint32_t              OrderedLogicalCcd[PROJ_MAX_CCD_DIES_PER_SOCKET];
  uint32_t              CcdNum;
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT  ApobCcdLogToPhysMap;
  APOB_SOC_DIE_INFO                             SocMaxDieInfo;

  if (SilGetIp2IpApi(SilId_CcxClass, (void **)(&CcxIp2Ip)) != SilPass) {
    return SilNotFound;
  }
  if (SilGetIp2IpApi(SilId_DfClass, (void **) &DfIp2IpApi) != SilPass) {
    return SilNotFound;
  }

  if (SilGetIp2IpApi(SilId_ApobClass, (void **) &ApobIp2IpApi) != SilPass) {
    return SilNotFound;
  }

  ApobIp2IpApi->ApobGetMaxDieInfo(&SocMaxDieInfo);

  if ((ApicModeValue == xApicMode) &&
    (((sizeof (SIL_SRAT_APIC)) * (TotalNumberApicId ())) <= SratApicSize)) {
    XPRF_TRACEPOINT(SIL_TRACE_INFO, "SratApicEntry buffer from host match.\n");
  } else {
    if ((ApicModeValue == x2ApicMode) &&
      (((sizeof (SIL_SRAT_x2APIC)) * (TotalNumberApicId ())) <= SratApicSize)) {
      XPRF_TRACEPOINT(SIL_TRACE_INFO, "SratX2ApicEntry buffer from host match.\n");
    } else {
      XPRF_TRACEPOINT(SIL_TRACE_INFO, "SratApicEntry buffer from host does not match.\n");
      return SilOutOfBounds;
    }
  }

  CcxData = (CCXCLASS_DATA_BLK *)xUslFindStructure(SilId_CcxClass, 0);
  if (CcxData == NULL) {
    XPRF_TRACEPOINT(SIL_TRACE_INFO, "SratApicEntry CcxData buffer not found.\n");
    return SilNotFound; // Could not find the IP input block
  }

  *SratTableLength = 0;
  NumberOfApicIds = 0;
  ApicId = 0;
  // Get Number of Socket information of the overall system.
  if (DfIp2IpApi->DfGetSystemInfo(&NumberOfSockets, NULL, NULL, NULL, NULL) == SilPass) {
    for (SocketLoop = 0; SocketLoop < NumberOfSockets; SocketLoop++) {
      if (DfIp2IpApi->DfGetProcessorInfo(SocketLoop, &NumberOfDies, NULL) == SilPass) {
        for (DieLoop = 0; DieLoop < NumberOfDies; DieLoop++) {
          ApobIp2IpApi->ApobGetCcdLogToPhysMap(SocketLoop, DieLoop, &ApobCcdLogToPhysMap);
          if (GetCcdCountOnDie(&ApobCcdLogToPhysMap, SocketLoop, DieLoop, &NumberOfCcds) == SilPass) {
            XPRF_TRACEPOINT(SIL_TRACE_INFO, "Socket %d Die %d has %d CCD(s)\n", SocketLoop, DieLoop, NumberOfCcds);
            assert(NumberOfCcds <= SocMaxDieInfo.MaxSocCcdsPerDieValue);
            Status = CcxIp2Ip->ReOrderLogicalCcdWithNumaDomainOrder(SocketLoop,
              DieLoop,
              NumberOfCcds,
              OrderedLogicalCcd
              );
            if (Status != SilPass) {
              continue;
            }
            for (CcdLoop = 0; CcdLoop < NumberOfCcds; CcdLoop++) {
              CcdNum = OrderedLogicalCcd [CcdLoop];
              Status = GetComplexCountOnCcd(&ApobCcdLogToPhysMap, SocketLoop, DieLoop, CcdNum, &NumberOfComplexes);
              if (Status != SilPass) {
                NumberOfComplexes = 0;
              }
              for (ComplexLoop = 0; ComplexLoop < NumberOfComplexes; ComplexLoop++) {
                if (DfIp2IpApi->DfDomainXlat(SocketLoop, DieLoop, CcdNum, ComplexLoop, &Domain) == SilPass) {
                  Status = GetCoreCountOnComplex(&ApobCcdLogToPhysMap,
                    SocketLoop,
                    DieLoop,
                    CcdNum,
                    ComplexLoop,
                    &NumberOfCores
                    );
                  if (Status != SilPass) {
                    NumberOfCores = 0;
                  }
                  XPRF_TRACEPOINT(SIL_TRACE_INFO,
                    "Socket %d Die %d CCD %d CCX %d has %d Core(s)\n",
                    SocketLoop,
                    DieLoop,
                    CcdNum,
                    ComplexLoop,
                    NumberOfCores
                    );
                  for (CoreLoop = 0; CoreLoop < NumberOfCores; CoreLoop++) {
                    Status = GetThreadCountOnCore(&ApobCcdLogToPhysMap,
                      SocketLoop,
                      DieLoop,
                      CcdNum,
                      ComplexLoop,
                      CoreLoop,
                      &NumberOfThreads
                      );
                    if (Status != SilPass) {
                      NumberOfThreads = 0;
                    }
                    XPRF_TRACEPOINT(SIL_TRACE_INFO,
                      "Socket %d Die %d CCD %d CCX %d Core %d has %d Thread(s)\n",
                      SocketLoop,
                      DieLoop,
                      CcdNum,
                      ComplexLoop,
                      CoreLoop,
                      NumberOfThreads
                      );
                    for (ThreadsLoop = 0; ThreadsLoop < NumberOfThreads; ThreadsLoop++) {
                      // Calculate the APIC Id based on the provided inputs and update into
                      // PState structure
                      ApicId = CcxIp2Ip->CalcLocalApic(SocketLoop,
                        DieLoop,
                        CcdNum,
                        ComplexLoop,
                        CoreLoop,
                        ThreadsLoop
                        );
                      if ((ApicModeValue == x2ApicMode) || ((ApicModeValue == ApicCompatibilityMode)
                        && (ApicId >= SIL_XAPIC_ID_MAX))) {
                        X2ApicEntry = (SIL_SRAT_x2APIC *) SratApic;
                        SratApic += sizeof (SIL_SRAT_x2APIC);
                        *SratTableLength += sizeof (SIL_SRAT_x2APIC);
                        X2ApicEntry->Type = SIL_SRAT_LOCAL_X2_APIC_TYPE;
                        X2ApicEntry->Length = sizeof (SIL_SRAT_x2APIC);
                        X2ApicEntry->ProximityDomain = Domain;
                        X2ApicEntry->x2ApicId = ApicId;
                        X2ApicEntry->Flags.Enabled = 1;
                        X2ApicEntry->ClockDomain = 0;
                        NumberOfApicIds++;
                      } else if (ApicId < SIL_XAPIC_ID_MAX) {
                        ApicEntry = (SIL_SRAT_APIC *) SratApic;
                        SratApic += sizeof (SIL_SRAT_APIC);
                        *SratTableLength += sizeof (SIL_SRAT_APIC);
                        ApicEntry->Type = SIL_SRAT_LOCAL_APIC_TYPE;
                        ApicEntry->Length = sizeof (SIL_SRAT_APIC);
                        ApicEntry->ProximityDomain_7_0 = (uint8_t) Domain;
                        ApicEntry->ProximityDomain_31_8[0] = (uint8_t) (Domain >> 8);
                        ApicEntry->ProximityDomain_31_8[1] = (uint8_t) (Domain >> 16);
                        ApicEntry->ProximityDomain_31_8[2] = (uint8_t) (Domain >> 24);
                        ApicEntry->ApicId = (uint8_t) ApicId;
                        ApicEntry->Flags.Enabled = 1;
                        ApicEntry->LocalSapicEid = 0;
                        ApicEntry->ClockDomain = 0;
                        NumberOfApicIds++;
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  return SilPass;
}

/**
 * xPrfPStateGatherData
 *
 * @brief   This xPrf Service Provide the  PState information.
 *
 * @details This function will collect all PState information from the MSRs and fill up the
 *          Output Buffer.
 *          PState Information are  : Physical socket number
 *                                     Logical core number in this socket
 *                           Pstate enable status
 *                           Core Frequency in MHz
 *                           Power in MilliWatts
 *                           Software P-state number
 *
 * @param   PStateData      Input buffer for the PState System Info structure.  The Host is responsible
 *                          for ensuring the buffer size is sufficient to contain
 *                          SIL_PSTATE_SYS_INFO structure.
 *                          On output, the buffer is populated with SIL_PSTATE_SYS_INFO.
 * @param   PStateDataSize  The size of the PStateDataSize input buffer from the Host. This
 *                          is used by openSIL to ensure the input buffer size is
 *                          sufficient to contain SIL_PSTATE_SYS_INFO .
 *
 * @return  SIL_STATUS
 *
 * @retval  SilPass         The PState System Info was successfully populated.
 * @retval  SilOutOfBounds  The PState System Info input buffer size was not sufficient.
 * @retval  SilNotFound     IP transfer table was not found
 * @retval  SilDeviceError  Not able to get system info
 */
SIL_STATUS
xPrfPStateGatherData (
  SIL_PSTATE_SYS_INFO    *PStateData,
  uint32_t               PStateDataSize
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
  uint32_t               MaxSwState;
  uint32_t               NumberOfBoostPstate;
  uint32_t               Frequency;
  uint32_t               VoltageInuV;
  uint32_t               PowerInmW;
  uint32_t               NumberOfSockets;
  uint32_t               NumberOfDies;
  uint32_t               SocketLoop;
  uint32_t               DieLoop;
  SIL_PSTATE_SOCKET_INFO *PstateSocketInfo;
  SIL_PSTATE_VALUES      *PstateStructure;
  bool                   PstateStatus;
  CCXCLASS_DATA_BLK      *CcxData;
  SIL_STATUS             Status;
  uint32_t               PstateTransLatency;
  CCX_IP2IP_API          *CcxIp2Ip;
  DF_IP2IP_API           *DfIp2IpApi;
  APOB_IP2IP_API         *ApobIp2IpApi;
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT  ApobCcdLogToPhysMap;

  if (SilGetIp2IpApi(SilId_CcxClass, (void **)(&CcxIp2Ip)) != SilPass) {
    return SilNotFound;
  }
  if (SilGetIp2IpApi(SilId_DfClass, (void **) &DfIp2IpApi) != SilPass) {
    return SilNotFound;
  }
  if (SilGetIp2IpApi(SilId_ApobClass, (void **) &ApobIp2IpApi) != SilPass) {
    return SilNotFound;
  }

  if ((sizeof (SIL_PSTATE_SYS_INFO)) <= PStateDataSize) {
    XPRF_TRACEPOINT(SIL_TRACE_INFO, "PStateData buffer from host match.\n");
  } else {
    XPRF_TRACEPOINT(SIL_TRACE_INFO, "PStateData buffer from host does not match.\n");
    return SilOutOfBounds;
  }

  CcxData = (CCXCLASS_DATA_BLK *)xUslFindStructure(SilId_CcxClass, 0);
  if (CcxData == NULL) {
    XPRF_TRACEPOINT(SIL_TRACE_INFO, "PStateGatherData CcxData buffer not found.\n");
    return SilNotFound; // Could not find the IP input block
  }

  // Get maximum number of Pstates in NumberOfPstate
  GetPstateNumber(&NumberOfBoostPstate, &MaxSwState);
  MaxSwState = MaxSwState - NumberOfBoostPstate;

  // Get Number of Socket information of the overall system.
  Status = DfIp2IpApi->DfGetSystemInfo(&NumberOfSockets, NULL, NULL, NULL, NULL);
  if (Status != SilPass) {
    assert(Status == SilPass);
    return SilDeviceError;
  }

  // Calculate the Table Size
  TableSize = (uint32_t) (sizeof (SIL_PSTATE_SYS_INFO) + (((MaxSwState * sizeof (SIL_PSTATE_VALUES))
    + sizeof (SIL_PSTATE_SOCKET_INFO)) * NumberOfSockets));
  // update PState Structure
  PStateData->TotalSocketInSystem = (uint8_t) NumberOfSockets;
  PStateData->SizeOfBytes = TableSize;
  PstateSocketInfo = (SIL_PSTATE_SOCKET_INFO *)PStateData->PStateSocketStruct;

  for (SocketLoop = 0; SocketLoop < NumberOfSockets; SocketLoop++) {
    // Calculate number of logical cores
    LocalApicIdLoop = 0;
    //Get Socket and Number of dies information about the processor installed in the socket.
    Status = DfIp2IpApi->DfGetProcessorInfo(SocketLoop, &NumberOfDies, NULL);
    if (Status != SilPass) {
      NumberOfDies = 0;
      XPRF_TRACEPOINT(SIL_TRACE_ERROR, "Failed to get NumberOfDies\n");
    }
    for (DieLoop = 0; DieLoop < NumberOfDies; DieLoop++) {
      // Get CcdLogToPhysMap for this Socket/Die
      ApobIp2IpApi->ApobGetCcdLogToPhysMap(SocketLoop, DieLoop, &ApobCcdLogToPhysMap);
      Status = GetCcdCountOnDie(&ApobCcdLogToPhysMap, SocketLoop, DieLoop, &NumberOfCcds);
      if (Status != SilPass) {
        NumberOfCcds = 0;
        XPRF_TRACEPOINT(SIL_TRACE_ERROR, "Failed to get NumberOfCcds\n");
      }
      for (CcdLoop = 0; CcdLoop < NumberOfCcds; CcdLoop++) {
        Status = GetComplexCountOnCcd(&ApobCcdLogToPhysMap, SocketLoop, DieLoop, CcdLoop, &NumberOfComplexes);
        if (Status != SilPass) {
          NumberOfComplexes = 0;
          XPRF_TRACEPOINT(SIL_TRACE_ERROR, "Failed to get NumberOfComplexes\n");
        }
        for (ComplexLoop = 0; ComplexLoop < NumberOfComplexes; ComplexLoop++) {
          Status = GetCoreCountOnComplex(&ApobCcdLogToPhysMap,
            SocketLoop,
            DieLoop,
            CcdLoop,
            ComplexLoop,
            &NumberOfCores
            );
          if (Status != SilPass) {
            NumberOfCores = 0;
            XPRF_TRACEPOINT(SIL_TRACE_ERROR, "Failed to get NumberOfCores\n");
          }
          for (CoreLoop = 0; CoreLoop < NumberOfCores; CoreLoop++) {
            Status = GetThreadCountOnCore(&ApobCcdLogToPhysMap,
              SocketLoop,
              DieLoop,
              CcdLoop,
              ComplexLoop,
              CoreLoop,
              &NumberOfThreads
              );
            if (Status != SilPass) {
              NumberOfThreads = 0;
              XPRF_TRACEPOINT(SIL_TRACE_ERROR, "Failed to get NumberOfThreads\n");
            }
            for (ThreadLoop = 0; ThreadLoop < NumberOfThreads; ThreadLoop++) {
              if (LocalApicIdLoop < SIL_ARRAY_SIZE(PstateSocketInfo->LocalApicId)) {
                // Calculate the APIC Id based on the provided inputs and update into PState structure
                PstateSocketInfo->LocalApicId[LocalApicIdLoop] = CcxIp2Ip->CalcLocalApic(SocketLoop,
                  DieLoop,
                  CcdLoop,
                  ComplexLoop,
                  CoreLoop,
                  ThreadLoop
                  );
              } else {
                XPRF_TRACEPOINT(SIL_TRACE_ERROR, "LocalApicIdLoop out of bounds\n");
                assert(LocalApicIdLoop >= SIL_ARRAY_SIZE(PstateSocketInfo->LocalApicId));
              }
              LocalApicIdLoop++;
            }
          }
        }
      }
    }
    PstateSocketInfo->SocketNumber = (uint8_t) SocketLoop;
    PstateSocketInfo->TotalLogicalCores = (uint16_t) LocalApicIdLoop;
    PstateSocketInfo->CreateAcpiTables = true;
    PstateSocketInfo->PStateMaxValue = (uint8_t) MaxSwState;

    // Get transition latency
    PstateTransLatency = 0;
    GetPstateTransLatency(&PstateTransLatency);
    PstateSocketInfo->TransitionLatency = PstateTransLatency;

    // Update IsPsdDependent depend on the AmdPstatePolicy PCD Value
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
      assert(false);
      break;
    }

    PstateStructure = PstateSocketInfo->PStateStruct;
    TotalEnabledPStates = 0;

    for (PstateLoop = 0; PstateLoop <= MaxSwState; PstateLoop++) {
      memset(PstateStructure, 0, sizeof (SIL_PSTATE_VALUES));
      // Get the Pstate information including Freq, Voltage, Power.
      Status = CcxIp2Ip->GetPstateInfo((Pstate0 + PstateLoop),
        &Frequency,
        &VoltageInuV,
        &PowerInmW,
        &PstateStatus
        );
      if (Status == SilPass) {
        // Update Pstate enable status,Core Frequency in MHz, Power in MilliWatts
        // Software P-state number
        PstateStructure->CoreFreq = Frequency;
        PstateStructure->Power = PowerInmW;
        PstateStructure->SwPstateNumber = PstateLoop;
        PstateStructure->PStateEnable = PstateStatus;
        PstateStructure++;
        TotalEnabledPStates++;
      }
    } // for (PstateLoop = 0; PstateLoop < MaxState; PstateLoop++)

    // Do not create ACPI Tables if there is one or less than one PState is enabled
    if (TotalEnabledPStates <= 1) {
      PstateSocketInfo->CreateAcpiTables = false;
    }
    PstateSocketInfo = (SIL_PSTATE_SOCKET_INFO *) ((uint8_t *) PstateSocketInfo +
      sizeof (SIL_PSTATE_SOCKET_INFO) + sizeof (SIL_PSTATE_VALUES) * MaxSwState);
  }

  return SilPass;
}

/**
 * xPrfGetCratHsaProcInfo
 *
 * @brief   This xPrf Service Provide the CRAT information about the HSA.
 *
 * @details This function will collect CRAT information about the HSA and fill up the
 *          Output Buffer.
 *          CRAT Information are  : proximity node
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
  uint8_t                      *CratHsaProcData,
  uint32_t                     CratHsaProcDataSize
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
  uint32_t                     CcdLoop;
  uint32_t                     ComplexLoop;
  uint32_t                     CoreLoop;
  uint32_t                     Domain;
  uint32_t                     PreDomain;
  SIL_CRAT_HSA_PROC_INFO       *CratHsaEntry;
  CCX_IP2IP_API                *CcxIp2Ip;
  DF_IP2IP_API                 *DfIp2IpApi;
  APOB_IP2IP_API               *ApobIp2IpApi;
  SIL_STATUS                   Status;
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT  ApobCcdLogToPhysMap;

  if (SilGetIp2IpApi(SilId_CcxClass, (void **)(&CcxIp2Ip)) != SilPass) {
    return SilNotFound;
  }
  if (SilGetIp2IpApi(SilId_DfClass, (void **) &DfIp2IpApi) != SilPass) {
    return SilNotFound;
  }
  if (SilGetIp2IpApi(SilId_ApobClass, (void **) &ApobIp2IpApi) != SilPass) {
    return SilNotFound;
  }

  NumberOfSockets = 0;
  NumberOfDies = 0;
  SocketLoop = 0;
  PreDomain = 0;
  CratHsaEntry = NULL;

  // Get Number of Socket information of the overall system.
  DfIp2IpApi->DfGetSystemInfo(&NumberOfSockets, &NumberOfDies, NULL, NULL, NULL);

  if ((((sizeof (SIL_CRAT_HSA_PROC_INFO)) * (NumberOfDies))
    <= CratHsaProcDataSize)) {
    XPRF_TRACEPOINT(SIL_TRACE_INFO, "GetCratHsaProcInfo buffer from host match.\n");
  } else {
    XPRF_TRACEPOINT(SIL_TRACE_INFO, "GetCratHsaProcInfo buffer from host does not match.\n");
    return SilOutOfBounds;
  }

  for (SocketLoop = 0; SocketLoop < NumberOfSockets; SocketLoop++) {
    //Get Socket and Number of dies information about the processor installed in the socket.
    if (DfIp2IpApi->DfGetProcessorInfo( SocketLoop, &NumberOfDies, NULL) == SilPass) {
      for (DieLoop = 0; DieLoop < NumberOfDies; DieLoop++) {
        if (DfIp2IpApi->DfDomainXlat(SocketLoop, DieLoop, 0, 0, &Domain) == SilPass) {
          if ((Domain != PreDomain) || (Domain == 0)) {
            CratHsaEntry = (SIL_CRAT_HSA_PROC_INFO *) CratHsaProcData;
            CratHsaProcData += sizeof (SIL_CRAT_HSA_PROC_INFO);
            CratHsaEntry->ProximityNode = Domain;
            // Calculate the APIC Id based on the provided inputs and update into
            // PState structure
            CratHsaEntry->ProcessorIdLow = CcxIp2Ip->CalcLocalApic(SocketLoop,
              DieLoop,
              0,
              0,
              0,
              0
              );
            CratHsaEntry->NumCPUCores = 0;
            PreDomain = Domain;
          }
          //
          // Get thread count by CCD/Complex/Core loop
          //
          ApobIp2IpApi->ApobGetCcdLogToPhysMap(SocketLoop, DieLoop, &ApobCcdLogToPhysMap);
          Status = GetCcdCountOnDie(&ApobCcdLogToPhysMap, SocketLoop, DieLoop, &NumberOfCcds);
          assert(Status == SilPass);
          if (Status != SilPass) {
            NumberOfCcds = 0;
          }
          for (CcdLoop = 0; CcdLoop < NumberOfCcds; CcdLoop++) {
            Status = GetComplexCountOnCcd( &ApobCcdLogToPhysMap, SocketLoop, DieLoop, CcdLoop, &NumberOfComplexes);
            assert(Status == SilPass);
            if (Status != SilPass) {
              NumberOfComplexes = 0;
            }
            for (ComplexLoop = 0; ComplexLoop < NumberOfComplexes; ComplexLoop++) {
              Status = GetCoreCountOnComplex(&ApobCcdLogToPhysMap,
                SocketLoop,
                DieLoop,
                CcdLoop,
                ComplexLoop,
                &NumberOfCores
                );
              assert(Status == SilPass);
              if (Status != SilPass) {
                NumberOfCores = 0;
              }
              for (CoreLoop = 0; CoreLoop < NumberOfCores; CoreLoop++) {
                Status = GetThreadCountOnCore(&ApobCcdLogToPhysMap,
                  SocketLoop,
                  DieLoop,
                  CcdLoop,
                  ComplexLoop,
                  CoreLoop,
                  &NumberOfThreads
                  );
                assert(Status == SilPass);
                if (Status != SilPass) {
                  NumberOfThreads = 0;
                }
                CratHsaEntry->NumCPUCores += (uint16_t) (NumberOfThreads & 0xFFFF);
              }
            }
          }
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
 *          Output Buffer.
 *          CRAT  Cache Information are  : Low value of a logical processor
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
  uint8_t            *CratCacheEntry,
  uint32_t           CratCacheEntrySize
  )
{
  uint32_t           SocketLoop;
  uint32_t           DieLoop;
  uint32_t           CcdLoop;
  uint32_t           ComplexLoop;
  uint32_t           CoreLoop;
  uint32_t           ThreadsLoop;
  uint32_t           NumberOfSockets;
  uint32_t           NumberOfDies;
  uint32_t           NumberOfCcds;
  uint32_t           NumberOfComplexes;
  uint32_t           NumberOfCores;
  uint32_t           NumberOfThreads;
  SIL_CRAT_CACHE     *CratCache;
  SIL_STATUS         Status;
  CCX_IP2IP_API      *CcxIp2Ip;
  DF_IP2IP_API       *DfIp2IpApi;
  APOB_IP2IP_API     *ApobIp2IpApi;
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT  ApobCcdLogToPhysMap;

  if (SilGetIp2IpApi(SilId_CcxClass, (void **)(&CcxIp2Ip)) != SilPass) {
    return SilNotFound;
  }
  if (SilGetIp2IpApi(SilId_DfClass, (void **) &DfIp2IpApi) != SilPass) {
    return SilNotFound;
  }
  if (SilGetIp2IpApi(SilId_ApobClass, (void **) &ApobIp2IpApi) != SilPass) {
    return SilNotFound;
  }

  SocketLoop = 0;
  DieLoop = 0;
  CcdLoop = 0;
  ComplexLoop = 0;
  CoreLoop = 0;
  ThreadsLoop = 0;
  NumberOfSockets = 0;
  NumberOfDies = 0;
  NumberOfCcds = 0;
  NumberOfComplexes = 0;
  NumberOfCores = 0;
  NumberOfThreads = 0;

  if (((sizeof (SIL_CRAT_CACHE)) * (TotalNumberApicId ())) <= CratCacheEntrySize) {
    XPRF_TRACEPOINT(SIL_TRACE_INFO, "xPrfCratCacheEntry buffer from host match.\n");
  } else {
    XPRF_TRACEPOINT(SIL_TRACE_INFO, "xPrfCratCacheEntry buffer from host does not match.\n");
    return SilOutOfBounds;
  }

  // Get Number of Socket information of the overall system.
  DfIp2IpApi->DfGetSystemInfo(&NumberOfSockets, NULL, NULL, NULL, NULL);
  for (SocketLoop = 0; SocketLoop < NumberOfSockets; SocketLoop++) {
    //Get Socket and Number of dies information about the processor installed in the socket.
    if (DfIp2IpApi->DfGetProcessorInfo(SocketLoop, &NumberOfDies, NULL) == SilPass) {
      for (DieLoop = 0; DieLoop < NumberOfDies; DieLoop++) {
        ApobIp2IpApi->ApobGetCcdLogToPhysMap(SocketLoop, DieLoop, &ApobCcdLogToPhysMap);
        Status = GetCcdCountOnDie(&ApobCcdLogToPhysMap, SocketLoop, DieLoop, &NumberOfCcds);
        if (Status != SilPass) {
          NumberOfCcds = 0;
        }
        for (CcdLoop = 0; CcdLoop < NumberOfCcds; CcdLoop++) {
          Status = GetComplexCountOnCcd(&ApobCcdLogToPhysMap,
            SocketLoop,
            DieLoop,
            CcdLoop,
            &NumberOfComplexes
            );
          if (Status != SilPass) {
            NumberOfComplexes = 0;
          }
          for (ComplexLoop = 0; ComplexLoop < NumberOfComplexes; ComplexLoop++) {
            Status = GetCoreCountOnComplex(&ApobCcdLogToPhysMap,
              SocketLoop,
              DieLoop,
              CcdLoop,
              ComplexLoop,
              &NumberOfCores
              );
            if (Status != SilPass) {
              NumberOfCores = 0;
            }
            for (CoreLoop = 0; CoreLoop < NumberOfCores; CoreLoop++) {
              Status = GetThreadCountOnCore(&ApobCcdLogToPhysMap,
                SocketLoop,
                DieLoop,
                CcdLoop,
                ComplexLoop,
                CoreLoop,
                &NumberOfThreads
                );
              if (Status != SilPass) {
                NumberOfThreads = 0;
              }
              for (ThreadsLoop = 0; ThreadsLoop < NumberOfThreads; ThreadsLoop++) {
                CratCache = (SIL_CRAT_CACHE *) CratCacheEntry;
                // Calculate the APIC Id based on the provided inputs
                CratCache->ProcessorIdLow = CcxIp2Ip->CalcLocalApic(SocketLoop,
                  DieLoop,
                  CcdLoop,
                  ComplexLoop,
                  CoreLoop,
                  ThreadsLoop
                  );
                CratCache->CacheLatency = 1;
                CratCache->LinesPerTag = 1;
              }
            }
          }
        }
      }
    }
  }

  return SilPass;
}

/// SMU message ID for getting CPPC minimum frequency
#define SMU_MSG_GET_CPPC_MIN_FREQUENCY  0x40
/// SMU message ID for getting CPPC nominal frequency
#define SMU_MSG_GET_CPPC_NOM_FREQUENCY  0x3A

/**
 * xPrfGetCppcMinFrequency
 *
 * @brief   This function is responsible for reading the minimal CPU frequency
 *          from SMU for ACPI CPPC.
 *
 * @param[out] MinFrequency  Pointer to store the minimal CPU frequency in MHz.
 *
 * @retval SilPass             The minimal frequency was successfully retrieved.
 * @retval SilInvalidParameter The MinFrequency pointer is NULL.
 * @retval SilNotFound         The SMU or NBIO API was not found.
 * @retval SilDeviceError      The SMU request failed.
 */
SIL_STATUS
xPrfGetCppcMinFrequency (
  uint32_t *MinFrequency
  )
{
  uint32_t                     SmuArg[6];
  SMC_RESULT                   Result;
  GNB_HANDLE                   *GnbHandle;
  SMU_COMMON_2_REV_XFER_BLOCK  *SmuXfer;
  NBIO_IP2IP_API               *NbioIp2Ip;

  if (MinFrequency == NULL) {
    return SilInvalidParameter;
  }

  if (SilGetCommon2RevXferTable(SilId_SmuClass, (void **)(&SmuXfer)) != SilPass) {
    return SilNotFound;
  }

  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    return SilNotFound;
  }

  GnbHandle = NbioIp2Ip->GetGnbHandle();

  SmuServiceInitArgumentsCommon(SmuArg);

  Result = SmuXfer->SmuServiceRequest(GnbHandle->Address,
             SMU_MSG_GET_CPPC_MIN_FREQUENCY,
             SmuArg,
             0
             );

  if (Result != SMC_Result_OK) {
    return SilDeviceError;
  }

  *MinFrequency = SmuArg[0];

  return SilPass;
}

/**
 * xPrfGetCppcNomFrequency
 *
 * @brief   This function is responsible for reading the nominal CPU frequency
 *          from SMU for ACPI CPPC.
 *
 * @param[out] NomFrequency  Pointer to store the nominal CPU frequency in MHz.
 *
 * @retval SilPass             The nominal frequency was successfully retrieved.
 * @retval SilInvalidParameter The NomFrequency pointer is NULL.
 * @retval SilNotFound         The SMU or NBIO API was not found.
 * @retval SilDeviceError      The SMU request failed.
 */
SIL_STATUS
xPrfGetCppcNomFrequency (
  uint32_t *NomFrequency
  )
{
  uint32_t                     SmuArg[6];
  SMC_RESULT                   Result;
  GNB_HANDLE                   *GnbHandle;
  SMU_COMMON_2_REV_XFER_BLOCK  *SmuXfer;
  NBIO_IP2IP_API               *NbioIp2Ip;

  if (NomFrequency == NULL) {
    return SilInvalidParameter;
  }

  if (SilGetCommon2RevXferTable(SilId_SmuClass, (void **)(&SmuXfer)) != SilPass) {
    return SilNotFound;
  }

  if (SilGetIp2IpApi(SilId_NbioClass, (void **)(&NbioIp2Ip)) != SilPass) {
    return SilNotFound;
  }

  GnbHandle = NbioIp2Ip->GetGnbHandle();

  SmuServiceInitArgumentsCommon(SmuArg);

  Result = SmuXfer->SmuServiceRequest(GnbHandle->Address,
             SMU_MSG_GET_CPPC_NOM_FREQUENCY,
             SmuArg,
             0
             );

  if (Result != SMC_Result_OK) {
    return SilDeviceError;
  }

  *NomFrequency = SmuArg[0];

  return SilPass;
}
