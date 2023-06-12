/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  CoreTopologyService.c
 * @brief CoreTopology service definition.
 *
 */

#include <SilCommon.h>
#include <ProjSocConst.h>
#include <DF/DfIp2Ip.h>
#include <APOB/ApobIp2Ip.h>
#include <APOB/Common/ApobCmn.h>

#include "CoreTopologyService.h"

/**
 * @details From CoreTopologyServicesV3BrhPei.c
 *
 *  CcxCoreTopolgyServicesGetSocketCountOnSystemV3 --> GetSocketCountOnSystemMax
 *  CcxCoreTopolgyServicesGetDieCountOnSocketV3 --> GetDieCountOnSocketMax
 *  CcxCoreTopolgyServicesGetCcdCountOnDieV3 --> GetCcdCountOnDie
 *  CcxCoreTopolgyServicesGetComplexCountOnCcdV3 --> GetComplexCountOnCcd
 *  CcxCoreTopolgyServicesGetCoreCountOnComplexV3 --> GetCoreCountOnComplex
 *  CcxCoreTopolgyServicesGetThreadCountOnCoreV3 --> GetThreadCountOnCore
 */

/**
 * GetSocketCountOnSystemMax
 *
 * @brief Return the maximum sockets supported in the system
 *
 * @return uint32_t The MAX number of sockets supported.
 */
static
uint32_t
GetSocketCountOnSystemMax (void)
{
  DF_IP2IP_API        *DfIp2IpApi;
  SIL_STATUS          Status;
  uint32_t            NumberOfProcessors;
  APOB_IP2IP_API      *ApobIp2IpApi;
  APOB_SOC_DIE_INFO   SocMaxDieInfo;

  Status = SilGetIp2IpApi(SilId_ApobClass, (void **) &ApobIp2IpApi);
  if ((Status != SilPass) || (ApobIp2IpApi == NULL)) {
    assert(Status == SilPass);
    return 0;
  }

  ApobIp2IpApi->ApobGetMaxDieInfo(&SocMaxDieInfo);

  Status = SilGetIp2IpApi(SilId_DfClass, (void **) &DfIp2IpApi);
  if (Status != SilPass) {
    XUSL_TRACEPOINT(SIL_TRACE_ERROR, "DF Ip-2-Ip API not found!\n");
    assert(Status == SilPass);
    return 0;
  }
  Status = DfIp2IpApi->DfGetSystemInfo(&NumberOfProcessors, NULL, NULL, NULL, NULL);
  if (Status != SilPass) {
    NumberOfProcessors = SocMaxDieInfo.MaxSocSocketsSupportedValue;
  }
  return NumberOfProcessors;
}

/**
 * GetDieCountOnSocketMax
 *
 * @brief  Get die count max per socket.
 *
 * @return uint32_t The maximum dies per socket for the system.
 *
 **/
static
uint32_t
GetDieCountOnSocketMax (
  uint32_t            Socket
  )
{
  DF_IP2IP_API        *DfIp2IpApi;
  SIL_STATUS          Status;
  uint32_t            NumberOfDies;
  APOB_IP2IP_API      *ApobIp2IpApi;
  APOB_SOC_DIE_INFO   SocMaxDieInfo;

  Status = SilGetIp2IpApi(SilId_ApobClass, (void **) &ApobIp2IpApi);
  if ((Status != SilPass) || (ApobIp2IpApi == NULL)) {
    assert(Status == SilPass);
    return 0;
  }

  ApobIp2IpApi->ApobGetMaxDieInfo(&SocMaxDieInfo);

  Status = SilGetIp2IpApi(SilId_DfClass, (void **) &DfIp2IpApi);
  if (Status != SilPass) {
    XUSL_TRACEPOINT(SIL_TRACE_ERROR, "DF Ip-2-Ip API not found!\n");
    assert(Status == SilPass);
    return 0;
  }
  Status = DfIp2IpApi->DfGetProcessorInfo(Socket, &NumberOfDies, NULL);
  if (Status != SilPass) {
    NumberOfDies = SocMaxDieInfo.MaxSocDiesPerSocketValue;
  }
  return NumberOfDies;
}

/**
 * GetCcdCountOnDie
 *
 * @brief   Get ccd count for given die.
 *
 * @param[in]  ApobCcdLogToPhysMap  Pointer to the APOB CCD logical to physical map structure
 * @param[in]  Socket               Zero-based socket number to check.
 * @param[in]  Die                  The target die's identifier within Socket.
 * @param[out] NumberOfCcds         Pointer to the number of enabled CCDs on the given socket / die.
 *
 * @retval SilPass               The core topology information was successfully retrieved.
 * @retval SilInvalidParameter   Socket or Die is greater than max supported.
 * @retval SilInvalidParameter   NumberOfCcds points to NULL.
 * @retval SilInvalidParameter   ApobCcdLogToPhysMap points to NULL.
 *
 **/
SIL_STATUS
GetCcdCountOnDie (
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT  *ApobCcdLogToPhysMap,
  uint32_t                                      Socket,
  uint32_t                                      Die,
  uint32_t                                      *NumberOfCcds
  )
{
  uint32_t                                      i;
  uint32_t                                      MaxCcdsPerIoDie;
  APOB_IP2IP_API                                *ApobIp2IpApi;
  APOB_SOC_DIE_INFO                             SocMaxDieInfo;
  SIL_STATUS                                    Status;

  if ((NumberOfCcds == NULL) || (Socket >= PROJ_MAX_SOCKETS_SUPPORTED) || (Die >= PROJ_CCX_MAX_DIES_PER_SOCKET) ||
    (ApobCcdLogToPhysMap == NULL)) {
    return SilInvalidParameter;
  }

  Status = SilGetIp2IpApi(SilId_ApobClass, (void **)&ApobIp2IpApi);
  if ((Status != SilPass) || (ApobIp2IpApi == NULL)) {
    assert(Status == SilPass);
    return Status;
  }
  ApobIp2IpApi->ApobGetMaxDieInfo(&SocMaxDieInfo);

  MaxCcdsPerIoDie = SocMaxDieInfo.MaxSocCcdsPerDieValue;

  for (i = 0; i < MaxCcdsPerIoDie; i++) {
    if (ApobCcdLogToPhysMap->CcdMap[i].PhysCcdNumber == CCX_NOT_PRESENT) {
      break;
    }
  }

  *NumberOfCcds = i;

  return SilPass;
}


/**
 * GetComplexCountOnCcd
 *
 * @brief   Get complex count for given ccd.
 *
 * @param[in]  ApobCcdLogToPhysMap  Pointer to the APOB CCD logical to physical map structure
 * @param[in]  Socket               Zero-based socket number to check.
 * @param[in]  Die                  The target die's identifier within Socket.
 * @param[in]  Ccd                  The target CCD's identifier within die
 * @param[out] NumberOfComplexes    Pointer to the number of enabled complexes on
 *                                  the given socket / die.
 *
 * @retval SilPass               The core topology information was successfully retrieved.
 * @retval SilInvalidParameter   Socket or Die is greater than max supported.
 * @retval SilInvalidParameter   Ccd is non-existent.
 * @retval SilInvalidParameter   NumberOfComplexes pointers to NULL.
 * @retval SilInvalidParameter   ApobCcdLogToPhysMap points to NULL.
 *
 **/
SIL_STATUS
GetComplexCountOnCcd (
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT *ApobCcdLogToPhysMap,
  uint32_t                                     Socket,
  uint32_t                                     Die,
  uint32_t                                     Ccd,
  uint32_t                                     *NumberOfComplexes
  )
{
  uint32_t                                     i;
  SIL_STATUS                                   Status;
  APOB_IP2IP_API                               *ApobIp2IpApi;
  APOB_SOC_DIE_INFO                            SocMaxDieInfo;

  Status = SilGetIp2IpApi(SilId_ApobClass, (void **) &ApobIp2IpApi);
  if ((Status != SilPass) || (ApobIp2IpApi == NULL)) {
    assert(Status == SilPass);
    return Status;
  }
  ApobIp2IpApi->ApobGetMaxDieInfo(&SocMaxDieInfo);

  if ((NumberOfComplexes == NULL) || (Socket >= PROJ_MAX_SOCKETS_SUPPORTED) || (Die >= PROJ_CCX_MAX_DIES_PER_SOCKET) ||
    (Ccd >= PROJ_MAX_CCD_DIES_PER_SOCKET / PROJ_MAX_IO_DIES_PER_SOCKET) || (ApobCcdLogToPhysMap == NULL)) {
    return SilInvalidParameter;
  }

  if (ApobCcdLogToPhysMap->CcdMap[Ccd].PhysCcdNumber != CCX_NOT_PRESENT) {
    for (i = 0; i < SocMaxDieInfo.MaxSocCcxPerCcdValue; i++) {
      if (ApobCcdLogToPhysMap->CcdMap[Ccd].ComplexMap[i].PhysComplexNumber == CCX_NOT_PRESENT) {
        break;
      }
    }
    *NumberOfComplexes = i;
    Status = SilPass;
  } else {
    Status = SilInvalidParameter;
  }

  return Status;
}

/**
 * GetCoreCountOnComplex
 *
 * @brief   Get core count for given complex.
 *
 * @param[in]  ApobCcdLogToPhysMap  Pointer to the APOB CCD logical to physical map structure
 * @param[in]  Socket               Zero-based socket number to check.
 * @param[in]  Die                  The target die's identifier within Socket.
 * @param[in]  Ccd                  The target CCD's identifier within die
 * @param[in]  Complex              The target Complex's identifier within ccd
 * @param[out] NumberOfCcds         Pointer to the number of enabled CCDs on the given socket / die.
 *
 * @retval SilPass               The core topology information was successfully retrieved.
 * @retval SilInvalidParameter   Socket, Die, Ccd, or Complex is greater than max supported.
 * @retval SilInvalidParameter   Ccd or Complex is non-existent.
 * @retval SilInvalidParameter   NumberOfCores points to NULL.
 * @retval SilInvalidParameter   ApobCcdLogToPhysMap points to NULL.
 *
 **/
SIL_STATUS
GetCoreCountOnComplex (
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT *ApobCcdLogToPhysMap,
  uint32_t                                     Socket,
  uint32_t                                     Die,
  uint32_t                                     Ccd,
  uint32_t                                     Complex,
  uint32_t                                     *NumberOfCores
  )
{
  uint32_t                                     i;
  SIL_STATUS                                   Status;
  APOB_IP2IP_API                               *ApobIp2IpApi;
  APOB_SOC_DIE_INFO                            SocMaxDieInfo;

  Status = SilGetIp2IpApi(SilId_ApobClass, (void **) &ApobIp2IpApi);
  if ((Status != SilPass) || (ApobIp2IpApi == NULL)) {
    assert(Status == SilPass);
    return Status;
  }
  ApobIp2IpApi->ApobGetMaxDieInfo(&SocMaxDieInfo);

  if ((NumberOfCores == NULL) || (Socket >= PROJ_MAX_SOCKETS_SUPPORTED) || (Die >= PROJ_CCX_MAX_DIES_PER_SOCKET) ||
    (Ccd >= PROJ_MAX_CCD_DIES_PER_SOCKET / PROJ_MAX_IO_DIES_PER_SOCKET) || (Complex >= PROJ_MAX_COMPLEXES_PER_CCD) ||
    (ApobCcdLogToPhysMap == NULL)) {
    assert(false);
    return SilInvalidParameter;
  }

  Status = SilPass;
  if ((ApobCcdLogToPhysMap->CcdMap[Ccd].PhysCcdNumber == CCX_NOT_PRESENT) ||
    (ApobCcdLogToPhysMap->CcdMap[Ccd].ComplexMap[Complex].PhysComplexNumber == CCX_NOT_PRESENT)) {
    assert(ApobCcdLogToPhysMap->CcdMap[Ccd].PhysCcdNumber != CCX_NOT_PRESENT);
    assert(ApobCcdLogToPhysMap->CcdMap[Ccd].ComplexMap[Complex].PhysComplexNumber != CCX_NOT_PRESENT);
    Status = SilInvalidParameter;
  }

  if (Status != SilInvalidParameter) {
    for (i = 0; i < SocMaxDieInfo.MaxSocCoresPerComplexValue; i++) {
      if (ApobCcdLogToPhysMap->CcdMap[Ccd].ComplexMap[Complex].CoreInfo[i].PhysCoreNumber == CCX_NOT_PRESENT) {
        break;
      }
    }
    *NumberOfCores = i;
    Status = SilPass;
  }

  return Status;
}
/**
 * GetThreadCountOnCore
 *
 * @brief   Get thread count for given core.
 *
 * @param[in]  ApobCcdLogToPhysMap  Pointer to the APOB CCD logical to physical map structure
 * @param[in]  Socket               Zero-based socket number to check.
 * @param[in]  Die                  The target die's identifier within socket.
 * @param[in]  Ccd                  The target CCD's identifier within die
 * @param[in]  Complex              The target Complex's identifier within ccd
 * @param[in]  Core                 The target Core's identifier within core
 * @param[out] NumberOfThreads      Pointer to the number of enabled threads per core on the given socket / die.
 *
 * @retval SilPass               The core topology information was successfully retrieved.
 * @retval SilInvalidParameter   Socket, Die, Ccd, Complex, or Core is greater than max supported.
 * @retval SilInvalidParameter   Ccd, Complex, or Core is non-existent.
 * @retval SilInvalidParameter   NumberOfThreads points to NULL.
 * @retval SilInvalidParameter   ApobCcdLogToPhysMap points to NULL.
 *
 **/
SIL_STATUS
GetThreadCountOnCore (
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT *ApobCcdLogToPhysMap,
  uint32_t                                     Socket,
  uint32_t                                     Die,
  uint32_t                                     Ccd,
  uint32_t                                     Complex,
  uint32_t                                     Core,
  uint32_t                                     *NumberOfThreads
  )
{
  uint32_t                                     i;
  SIL_STATUS                                   Status;
  APOB_IP2IP_API                               *ApobIp2IpApi;
  APOB_SOC_DIE_INFO                            SocMaxDieInfo;

  Status = SilGetIp2IpApi(SilId_ApobClass, (void **) &ApobIp2IpApi);
  if ((Status != SilPass) || (ApobIp2IpApi == NULL)) {
    assert(Status == SilPass);
    return Status;
  }
  ApobIp2IpApi->ApobGetMaxDieInfo(&SocMaxDieInfo);

  if ((NumberOfThreads == NULL) || (Socket >= PROJ_MAX_SOCKETS_SUPPORTED) || (Die >= PROJ_CCX_MAX_DIES_PER_SOCKET) ||
    (Ccd >= PROJ_MAX_CCD_DIES_PER_SOCKET / PROJ_MAX_IO_DIES_PER_SOCKET) || (Complex >= PROJ_MAX_COMPLEXES_PER_CCD) ||
    (Core >= PROJ_MAX_CCX_CORES_PER_COMPLEX) || (ApobCcdLogToPhysMap == NULL)) {
    return SilInvalidParameter;
  }

  Status = SilPass;
  if ((ApobCcdLogToPhysMap->CcdMap[Ccd].PhysCcdNumber == CCX_NOT_PRESENT) ||
    (ApobCcdLogToPhysMap->CcdMap[Ccd].ComplexMap[Complex].PhysComplexNumber == CCX_NOT_PRESENT) ||
    (ApobCcdLogToPhysMap->CcdMap[Ccd].ComplexMap[Complex].CoreInfo[Core].PhysCoreNumber == CCX_NOT_PRESENT)) {
    Status = SilInvalidParameter;
  }

  if (Status != SilInvalidParameter) {
    for (i = 0; i < SocMaxDieInfo.MaxSocThreadPerCore; i++) {
      if (!ApobCcdLogToPhysMap->CcdMap[Ccd].ComplexMap[Complex].CoreInfo[Core].IsThreadEnabled[i]) {
        break;
      }
    }

    *NumberOfThreads = i;
    Status = SilPass;
  }

  return Status;
}

/**
 * LogicalToPhysicalLocation
 *
 * @brief Convert logical Socket/Die/CCD/Complex/Core number to physical number.
 *
 * @param[in]     ApobCcdLogToPhysMap  Pointer to the APOB CCD logical to physical map structure
 * @param[in,out] Socket               On input, caller provides logical socket number,
 *                                     On output, it is physical socket number.
 * @param[in,out] Die                  On input, caller provides logical die number,
 *                                     On output, it is physical die number.
 * @param[in,out] Ccd                  On input, caller provides logical CCD number,
 *                                     On output, it is physical CCD number.
 * @param[in,out] Complex              On input, caller provides logical complex number,
 *                                     On output, it is physical complex number.
 * @param[in,out] Core                 On input, caller provides logical core number,
 *                                     On output, it is physical core number.
 *
 * @retval SilPass                     The conversion is successfully completed.
 * @retval SilUnsupported              Do not support this function.
 * @retval SilInvalidParameter         Socket is non-existent.
 * @retval SilInvalidParameter         Die is not valid.
 * @retval SilInvalidParameter         Ccd is not valid.
 * @retval SilInvalidParameter         Complex is not valid.
 * @retval SilInvalidParameter         Core is not valid.
 *
 */
SIL_STATUS
LogicalToPhysicalLocation (
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT *ApobCcdLogToPhysMap,
  uint32_t                                     *Socket,
  uint32_t                                     *Die,
  uint32_t                                     *Ccd,
  uint32_t                                     *Complex,
  uint32_t                                     *Core
  )
{
  uint32_t                                     i;
  uint32_t                                     ParameterAvailableMask;
  uint32_t                                     LowestBit;
  uint32_t                                     HighestBit;
  APOB_IP2IP_API                               *ApobIp2IpApi;
  APOB_SOC_DIE_INFO                            SocMaxDieInfo;
  SIL_STATUS                                   Status;

  Status = SilGetIp2IpApi(SilId_ApobClass, (void **) &ApobIp2IpApi);
  if ((Status != SilPass) || (ApobIp2IpApi == NULL)) {
    assert(Status == SilPass);
    return Status;
  }
  ApobIp2IpApi->ApobGetMaxDieInfo(&SocMaxDieInfo);
  /*
   * Use bit mask to check input parameter is legal
   * The bit should be continuous set from highest set bit.
   * And lower bits could be not set.
   */
  ParameterAvailableMask = 0;
  if (Core != 0) {
    ParameterAvailableMask |= 0x1;
  }
  if (Complex != 0) {
    ParameterAvailableMask |= 0x2;
  }
  if (Ccd != 0) {
    ParameterAvailableMask |= 0x4;
  }
  if (Die != 0) {
    ParameterAvailableMask |= 0x8;
  }
  if (Socket != 0) {
    ParameterAvailableMask |= 0x10;
  }

  /*
   * Find lowest level
   */
  for (i = 0, LowestBit = sizeof (ParameterAvailableMask) * 8; i < sizeof (ParameterAvailableMask) * 8; i++) {
    if ((ParameterAvailableMask & (0x01 << i)) != 0) {
      LowestBit = i;
      break;
    }
  }
  if (LowestBit == sizeof (ParameterAvailableMask) * 8) {
    return SilInvalidParameter;
  }
  /*
   * Find highest level
   */
  for (i = sizeof (ParameterAvailableMask) * 8 - 1, HighestBit = sizeof (ParameterAvailableMask) * 8;
    i >= LowestBit; i--) {
    if ((ParameterAvailableMask & (0x01 << i)) != 0) {
      HighestBit = i;
      break;
    }
  }
  if (HighestBit < 4) { // At least provide Socket
    return SilInvalidParameter;
  }

  /*
   * Check caller need all level from lowest to highest
   */
  for (i = LowestBit; i < HighestBit; i++) {
    if ((ParameterAvailableMask & (0x01 << i)) == 0) {
      return SilInvalidParameter;
    }
  }

  /*
   * Currently we need Socket and Die to locate corresponding APOB
   */
  if (Die == NULL) {
    return SilPass; // to Socket level only
  } else {
    if (*Die >= SocMaxDieInfo.MaxSocDiesPerSocketValue) {
      return SilInvalidParameter;
    }
  }
  if (ApobCcdLogToPhysMap == NULL) {
    return SilDeviceError;
  }

  if (Ccd != NULL) {
    if (*Ccd >= SocMaxDieInfo.MaxSocCcdsPerDieValue) {
      return SilInvalidParameter;
    }
    if (ApobCcdLogToPhysMap->CcdMap[*Ccd].PhysCcdNumber == CCX_NOT_PRESENT) {
      return SilInvalidParameter;
    }
    if (Complex != NULL) {
      if (*Complex >= SocMaxDieInfo.MaxSocCcxPerCcdValue) {
        return SilInvalidParameter;
      }
      if (ApobCcdLogToPhysMap->CcdMap[*Ccd].ComplexMap[*Complex].PhysComplexNumber == CCX_NOT_PRESENT) {
        return SilInvalidParameter;
      }
      if (Core != NULL) {
        if (*Core >= SocMaxDieInfo.MaxSocCoresPerComplexValue) {
          return SilInvalidParameter;
        }
        if (ApobCcdLogToPhysMap->CcdMap[*Ccd].ComplexMap[*Complex].CoreInfo[*Core].PhysCoreNumber == CCX_NOT_PRESENT) {
          return SilInvalidParameter;
        }
        *Core = (uint32_t)ApobCcdLogToPhysMap->CcdMap[*Ccd].ComplexMap[*Complex].CoreInfo[*Core].PhysCoreNumber;
      }
      *Complex = (uint32_t)ApobCcdLogToPhysMap->CcdMap[*Ccd].ComplexMap[*Complex].PhysComplexNumber;
    }
    *Ccd = (uint32_t)ApobCcdLogToPhysMap->CcdMap[*Ccd].PhysCcdNumber;
  }

  return SilPass;
}

/**
 * GetCoreTopologyOnDieMax
 *
 * @brief   This service retrieves information about the layout of the cores on the given die.
 *
 * @details Will return maximum number of Ccd/Complex/Core/Thread.
 *
 * @param[in]  Socket               Zero-based socket number to check.
 * @param[in]  Die                  The target die's identifier within Socket.
 * @param[out] MaxNumberOfCcds      Pointer to the maximum number of enabled CCDs on the given socket / die.
 * @param[out] MaxNumberOfComplexes Pointer to the maximum number of enabled complexes on the given socket / die.
 * @param[out] MaxNumberOfCores     Pointer to the maximum number of enabled cores per complex on the given
 *                                  socket / die.
 * @param[out] MaxNumberOfThreads   Pointer to the maximum number of enabled threads per core on the given socket / die.
 *
 * @retval SilPass                The core topology information was successfully retrieved.
 * @retval SilInvalidParameter    All output parameter pointers are not valid.
 * @retval SilNotFound            APOB CCD Logical to Physical Map not found.
 *
 **/
SIL_STATUS
GetCoreTopologyOnDieMax (
  uint32_t                                     Socket,
  uint32_t                                     Die,
  uint32_t                                     *MaxNumberOfCcds,
  uint32_t                                     *MaxNumberOfComplexes,
  uint32_t                                     *MaxNumberOfCores,
  uint32_t                                     *MaxNumberOfThreads
  )
{
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT ApobCcdLogToPhysMap;
  SIL_STATUS                                   Status;
  uint32_t                                     CcdLoop;
  uint32_t                                     ComplexLoop;
  uint32_t                                     CoreLoop;
  uint32_t                                     CcdCount;
  uint32_t                                     ComplexCount;
  uint32_t                                     CoreCount;
  uint32_t                                     ThreadCount;
  APOB_IP2IP_API                               *ApobIp2IpApi;

  if ((MaxNumberOfCcds == NULL) ||
    (MaxNumberOfComplexes == NULL) ||
    (MaxNumberOfCores == NULL) ||
    (MaxNumberOfThreads == NULL)) {
    return SilInvalidParameter;
  }

  Status = SilGetIp2IpApi(SilId_ApobClass, (void **) &ApobIp2IpApi);
  if ((Status != SilPass) || (ApobIp2IpApi == NULL)) {
    assert(Status == SilPass);
    return Status;
  }

  Status = ApobIp2IpApi->ApobGetCcdLogToPhysMap(Socket, Die, &ApobCcdLogToPhysMap);

  if (Status == SilPass) {
    *MaxNumberOfCcds = 1;
    *MaxNumberOfComplexes = 1;
    *MaxNumberOfCores = 1;
    *MaxNumberOfThreads = 1;
    Status = GetCcdCountOnDie(&ApobCcdLogToPhysMap, Socket, Die, &CcdCount);
    if (Status != SilPass) {
      CcdCount = 0;
    }
    if (CcdCount > *MaxNumberOfCcds) {
      *MaxNumberOfCcds = CcdCount;
    }
    for (CcdLoop = 0; CcdLoop < CcdCount; CcdLoop++) {
      Status = GetComplexCountOnCcd(&ApobCcdLogToPhysMap, Socket, Die, CcdLoop, &ComplexCount);
      if (Status != SilPass) {
        ComplexCount = 0;
      }
      if (ComplexCount > *MaxNumberOfComplexes) {
        *MaxNumberOfComplexes = ComplexCount;
      }
      for (ComplexLoop = 0; ComplexLoop < ComplexCount; ComplexLoop++) {
        Status = GetCoreCountOnComplex(&ApobCcdLogToPhysMap, Socket, Die, CcdLoop, ComplexLoop, &CoreCount);
        if (Status != SilPass) {
          CoreCount = 0;
        }
        if (CoreCount > *MaxNumberOfCores) {
          *MaxNumberOfCores = CoreCount;
        }
        //
        // Thread count is either 1 or 2. Check if there is 2 here.
        //
        if (*MaxNumberOfThreads == 1) {
          for (CoreLoop = 0; CoreLoop < CoreCount; CoreLoop++) {
            Status = GetThreadCountOnCore(&ApobCcdLogToPhysMap,
              Socket,
              Die,
              CcdLoop,
              ComplexLoop,
              CoreLoop,
              &ThreadCount
              );
            if ((Status == SilPass) && (ThreadCount > 1)) {
              *MaxNumberOfThreads = ThreadCount;
            }
          }
        }
      }
    }
    Status = SilPass;
  }

  return Status;
}

/**
 * @details From PeiCoreTopologyV3Lib.c
 *
 * CoreTopologyV3IterateSocket
 * CoreTopologyV3IterateDie
 * CoreTopologyV3IterateCcd
 * CoreTopologyV3IterateComplex
 * CoreTopologyV3IterateCore
 * CoreTopologyV3IterateThread
 */

/**
 * CoreTopologyV3IterateSocket
 *
 * @brief   Do iteration by Socket
 *
 * @details On input, it is last number of each level.
 *          On output, it is next number of each level.
 *          For initial input, assign each level to value CORE_TOPOLOGY_V3_ITERATION_START.
 *
 * @param[in,out] Socket Zero-based Socket number.
 *
 * @return CORE_TOPOLOGY_ITERATION_RESULT
 *
 **/
CORE_TOPOLOGY_ITERATION_RESULT
CoreTopologyV3IterateSocket (
  uint32_t    *Socket
  )
{
  uint32_t    NumOfSocket;

  if (Socket == NULL) {
    return CORE_TOPOLOGY_V3_NO_NEXT_ITEM;
  }

  if (*Socket == CORE_TOPOLOGY_V3_ITERATION_START) {
    *Socket = 0;
    return CORE_TOPOLOGY_V3_NEXT_SOCKET;
  } else {
    NumOfSocket = GetSocketCountOnSystemMax ();
    if ((*Socket + 1) == NumOfSocket) {
      return CORE_TOPOLOGY_V3_NO_NEXT_ITEM;
    } else if ((*Socket + 1) > NumOfSocket) {
      return CORE_TOPOLOGY_V3_NO_NEXT_ITEM;
    }
    (*Socket)++;
    return CORE_TOPOLOGY_V3_NEXT_SOCKET;
  }
}

/**
 * CoreTopologyV3IterateDie
 *
 * @brief   Do iteration by Die
 *
 * @details On input, it is last number of each level.
 *          On output, it is next number of each level.
 *          For initial input, assign each level to value CORE_TOPOLOGY_V3_ITERATION_START.
 *
 * @param[in,out]   Socket  Zero-based Socket number.
 * @param[in,out]   Die     Zero-based Die number.
 *
 * @return CORE_TOPOLOGY_ITERATION_RESULT
 *
 **/
CORE_TOPOLOGY_ITERATION_RESULT
CoreTopologyV3IterateDie (
  uint32_t                        *Socket,
  uint32_t                        *Die
  )
{
  uint32_t                        NumOfDie;
  CORE_TOPOLOGY_ITERATION_RESULT  IterationResult;

  if (Socket == NULL || Die == NULL) {
    return CORE_TOPOLOGY_V3_NO_NEXT_ITEM;
  }

  if ((*Socket == CORE_TOPOLOGY_V3_ITERATION_START) ||
    (*Die == CORE_TOPOLOGY_V3_ITERATION_START)) {
    *Socket = 0;
    *Die = 0;
    IterationResult = CORE_TOPOLOGY_V3_NEXT_SOCKET;
  } else {
    NumOfDie = GetDieCountOnSocketMax(*Socket);

    if ((*Die + 1) == NumOfDie) {
      // Loop next socket
      IterationResult = CoreTopologyV3IterateSocket(Socket);
      if (IterationResult < CORE_TOPOLOGY_V3_NEXT_DIE) {
        *Die = 0;
      }
    } else if ((*Die + 1) > NumOfDie) {
      IterationResult = CORE_TOPOLOGY_V3_NO_NEXT_ITEM;
    } else {
      (*Die)++;
      IterationResult = CORE_TOPOLOGY_V3_NEXT_DIE;
    }
  }
  return IterationResult;
}

/**
 * CoreTopologyV3IterateCcd
 *
 * @brief   Do iteration by CCD
 *
 * @details On input, it is last number of each level.
 *          On output, it is next number of each level.
 *          For initial input, assign each level to value CORE_TOPOLOGY_V3_ITERATION_START.
 *
 * @param[in]     ApobCcdLogToPhysMap   Pointer to the APOB CCD logical to physical map structure
 * @param[in,out] Socket                Zero-based Socket number.
 * @param[in,out] Die                   Zero-based Die number.
 * @param[in,out] Ccd                   Zero-based Ccd number.
 *
 * @return CORE_TOPOLOGY_ITERATION_RESULT
 *
 **/
CORE_TOPOLOGY_ITERATION_RESULT
CoreTopologyV3IterateCcd (
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT  *ApobCcdLogToPhysMap,
  uint32_t                                      *Socket,
  uint32_t                                      *Die,
  uint32_t                                      *Ccd
  )
{
  uint32_t                                      NumOfCcd;
  CORE_TOPOLOGY_ITERATION_RESULT                IterationResult;
  SIL_STATUS                                    Status;

  if (Socket == NULL || Die == NULL || Ccd == NULL) {
    return CORE_TOPOLOGY_V3_NO_NEXT_ITEM;
  }

  if ((*Socket == CORE_TOPOLOGY_V3_ITERATION_START) ||
    (*Die == CORE_TOPOLOGY_V3_ITERATION_START) ||
    (*Ccd == CORE_TOPOLOGY_V3_ITERATION_START)) {
    *Socket = 0;
    *Die = 0;
    *Ccd = 0;
    IterationResult = CORE_TOPOLOGY_V3_NEXT_SOCKET;
  } else {
    Status = GetCcdCountOnDie(ApobCcdLogToPhysMap, *Socket, *Die, &NumOfCcd);
    if (Status == SilPass) {
      if ((*Ccd + 1) == NumOfCcd) {
        // Loop next die
        IterationResult = CoreTopologyV3IterateDie(Socket, Die);
        if (IterationResult < CORE_TOPOLOGY_V3_NEXT_CCD) {
          *Ccd = 0;
        }
      } else if ((*Ccd + 1) > NumOfCcd) {
        IterationResult = CORE_TOPOLOGY_V3_NO_NEXT_ITEM;
      } else {
        (*Ccd)++;
        IterationResult = CORE_TOPOLOGY_V3_NEXT_CCD;
      }
    } else {
      IterationResult = CORE_TOPOLOGY_V3_NO_NEXT_ITEM;
    }
  }
  return IterationResult;
}

/**
 * CoreTopologyV3IterateComplex
 *
 * @brief   Do iteration by Complex.
 *
 * @details On input, it is last number of each level.
 *          On output, it is next number of each level.
 *          For initial input, assign each level to value CORE_TOPOLOGY_V3_ITERATION_START.
 *
 * @param[in]     ApobCcdLogToPhysMap   Pointer to the APOB CCD logical to physical map structure
 * @param[in,out] Socket                Zero-based Socket number.
 * @param[in,out] Die                   Zero-based Die number.
 * @param[in,out] Ccd                   Zero-based Ccd number.
 * @param[in,out] Complex               Zero-based Complex number.
 *
 * @return CORE_TOPOLOGY_ITERATION_RESULT
 *
 **/
CORE_TOPOLOGY_ITERATION_RESULT
CoreTopologyV3IterateComplex (
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT  *ApobCcdLogToPhysMap,
  uint32_t                                      *Socket,
  uint32_t                                      *Die,
  uint32_t                                      *Ccd,
  uint32_t                                      *Complex
  )
{
  uint32_t                          NumOfComplex;
  SIL_STATUS                        Status;
  CORE_TOPOLOGY_ITERATION_RESULT    IterationResult;

  if ((Socket == NULL) || (Die == NULL) || (Ccd == NULL) || (Complex == NULL) || (ApobCcdLogToPhysMap == NULL)) {
    return CORE_TOPOLOGY_V3_NO_NEXT_ITEM;
  }

  if ((*Socket == CORE_TOPOLOGY_V3_ITERATION_START) ||
    (*Die == CORE_TOPOLOGY_V3_ITERATION_START) ||
    (*Ccd == CORE_TOPOLOGY_V3_ITERATION_START) ||
    (*Complex == CORE_TOPOLOGY_V3_ITERATION_START)) {
    *Socket = 0;
    *Die = 0;
    *Ccd = 0;
    *Complex = 0;
    IterationResult = CORE_TOPOLOGY_V3_NEXT_SOCKET;
  } else {
    Status = GetComplexCountOnCcd(ApobCcdLogToPhysMap, *Socket, *Die, *Ccd, &NumOfComplex);
    if (Status == SilPass) {
      if ((*Complex + 1) == NumOfComplex) {
        // Loop next Ccd
        IterationResult = CoreTopologyV3IterateCcd(ApobCcdLogToPhysMap, Socket, Die, Ccd);
        if (IterationResult < CORE_TOPOLOGY_V3_NEXT_COMPLEX) {
          *Complex = 0;
        }
      } else if ((*Complex + 1) > NumOfComplex) {
        IterationResult = CORE_TOPOLOGY_V3_NO_NEXT_ITEM;
      } else {
        (*Complex)++;
        IterationResult = CORE_TOPOLOGY_V3_NEXT_COMPLEX;
      }
    } else {
      IterationResult = CORE_TOPOLOGY_V3_NO_NEXT_ITEM;
    }
  }
  return IterationResult;
}

/**
 * CoreTopologyV3IterateCore
 *
 * @brief   Do iteration by Core.
 *
 * @details On input, it is last number of each level.
 *          On output, it is next number of each level.
 *          For initial input, assign each level to value CORE_TOPOLOGY_V3_ITERATION_START.
 *
 * @param[in]     ApobCcdLogToPhysMap   Pointer to the APOB CCD logical to physical map structure
 * @param[in,out] Socket                Zero-based Socket number.
 * @param[in,out] Die                   Zero-based Die number.
 * @param[in,out] Ccd                   Zero-based Ccd number.
 * @param[in,out] Complex               Zero-based Complex number.
 * @param[in,out] Core                  Zero-based Core number.
 *
 * @return CORE_TOPOLOGY_ITERATION_RESULT
 *
 **/
CORE_TOPOLOGY_ITERATION_RESULT
CoreTopologyV3IterateCore (
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT  *ApobCcdLogToPhysMap,
  uint32_t                                      *Socket,
  uint32_t                                      *Die,
  uint32_t                                      *Ccd,
  uint32_t                                      *Complex,
  uint32_t                                      *Core
  )
{
  uint32_t                                      NumOfCore;
  SIL_STATUS                                    Status;
  CORE_TOPOLOGY_ITERATION_RESULT                IterationResult;

  if ((Socket == NULL) || (Die == NULL) || (Ccd == NULL) || (Complex == NULL) || (Core == NULL) ||
    (ApobCcdLogToPhysMap == NULL)) {
    return CORE_TOPOLOGY_V3_NO_NEXT_ITEM;
  }

  if ((*Socket == CORE_TOPOLOGY_V3_ITERATION_START) ||
    (*Die == CORE_TOPOLOGY_V3_ITERATION_START) ||
    (*Ccd == CORE_TOPOLOGY_V3_ITERATION_START) ||
    (*Complex == CORE_TOPOLOGY_V3_ITERATION_START) ||
    (*Core == CORE_TOPOLOGY_V3_ITERATION_START)) {
    *Socket = 0;
    *Die = 0;
    *Ccd = 0;
    *Complex = 0;
    *Core = 0;
    IterationResult = CORE_TOPOLOGY_V3_NEXT_SOCKET;
  } else {
    Status = GetCoreCountOnComplex(ApobCcdLogToPhysMap, *Socket, *Die, *Ccd, *Complex, &NumOfCore);
    if (Status == SilPass) {
      if ((*Core + 1) == NumOfCore) {
        // Loop next Complex
        IterationResult = CoreTopologyV3IterateComplex(ApobCcdLogToPhysMap, Socket, Die, Ccd, Complex);
        if (IterationResult < CORE_TOPOLOGY_V3_NEXT_CORE) {
          *Core = 0;
        }
      } else if ((*Core + 1) > NumOfCore) {
        IterationResult = CORE_TOPOLOGY_V3_NO_NEXT_ITEM;
      } else {
        (*Core)++;
        IterationResult = CORE_TOPOLOGY_V3_NEXT_CORE;
      }
    } else {
      IterationResult = CORE_TOPOLOGY_V3_NO_NEXT_ITEM;
    }
  }
  return IterationResult;
}

/**
 * CoreTopologyV3IterateThread
 *
 * @brief   Do iteration by thread.
 *
 * @details On input, it is last number of each level.
 *          On output, it is next number of each level.
 *          For initial input, assign each level to value CORE_TOPOLOGY_V3_ITERATION_START.
 *
 * @param[in,out] Socket                      Zero-based Socket number.
 * @param[in,out] Die                         Zero-based Die number.
 * @param[in,out] Ccd                         Zero-based Ccd number.
 * @param[in,out] Complex                     Zero-based Complex number.
 * @param[in,out] Core                        Zero-based Core number.
 * @param[in,out] Thread                      Zero-based Thread number.
 *
 * @return CORE_TOPOLOGY_ITERATION_RESULT
 *
 **/
CORE_TOPOLOGY_ITERATION_RESULT
CoreTopologyV3IterateThread (
  APOB_CCD_LOGICAL_TO_PHYSICAL_MAP_TYPE_STRUCT  *ApobCcdLogToPhysMap,
  uint32_t                                      *Socket,
  uint32_t                                      *Die,
  uint32_t                                      *Ccd,
  uint32_t                                      *Complex,
  uint32_t                                      *Core,
  uint32_t                                      *Thread
  )
{
  uint32_t                                      NumOfThread;
  CORE_TOPOLOGY_ITERATION_RESULT                IterationResult;
  SIL_STATUS                                    Status;

  if ((Socket == NULL) || (Die == NULL) || (Ccd == NULL) || (Complex == NULL) || (Core == NULL) || (Thread == NULL) ||
    (ApobCcdLogToPhysMap == NULL)) {
    return CORE_TOPOLOGY_V3_NO_NEXT_ITEM;
  }

  if ((*Socket == CORE_TOPOLOGY_V3_ITERATION_START) ||
    (*Die == CORE_TOPOLOGY_V3_ITERATION_START) ||
    (*Ccd == CORE_TOPOLOGY_V3_ITERATION_START) ||
    (*Complex == CORE_TOPOLOGY_V3_ITERATION_START) ||
    (*Core == CORE_TOPOLOGY_V3_ITERATION_START) ||
    (*Thread == CORE_TOPOLOGY_V3_ITERATION_START)) {
    *Socket = 0;
    *Die = 0;
    *Ccd = 0;
    *Complex = 0;
    *Core = 0;
    *Thread = 0;
    IterationResult = CORE_TOPOLOGY_V3_NEXT_SOCKET;
  } else {
    Status = GetThreadCountOnCore(ApobCcdLogToPhysMap, *Socket, *Die, *Ccd, *Complex, *Core, &NumOfThread);
    if (Status == SilPass) {
      if ((*Thread + 1) == NumOfThread) {
        // Loop next Core
        IterationResult = CoreTopologyV3IterateCore(ApobCcdLogToPhysMap, Socket, Die, Ccd, Complex, Core);
        if (IterationResult < CORE_TOPOLOGY_V3_NEXT_THREAD) {
          *Thread = 0;
        }
      } else if ((*Thread + 1) > NumOfThread) {
        IterationResult = CORE_TOPOLOGY_V3_NO_NEXT_ITEM;
      } else {
        (*Thread)++;
        IterationResult = CORE_TOPOLOGY_V3_NEXT_THREAD;
      }
    } else {
      IterationResult = CORE_TOPOLOGY_V3_NO_NEXT_ITEM;
    }
  }
  return IterationResult;
}
