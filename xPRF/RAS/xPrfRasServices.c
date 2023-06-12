/**
 * @file  xPrfRasServices.c
 * @brief Platform Reference Firmware - exposes Platform specific features for
 *        RAS that will provide boottime/runtime services.
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#include <SilCommon.h>
#include <xPRF-api.h>
#include <RAS/Common/RasClass-api.h>
#include <RAS/Common/RasReg.h>
#include <RAS/RasIp2Ip.h>
#include <RAS/RasDefs.h>
#include <MsrReg.h>
#include <string.h>
#include <DF/DfIp2Ip.h>

/*
 * xPrfMcaErrorAddrTranslate
 *
 * @brief   Translate Unified Memory Controller (UMC) local address into
 *          specific memory DIMM information and system address.
 *
 * @param   SystemMemoryAddress Pointer to return the calculated system address
 * @param   NormalizedAddress   UMC memory address Information passed in from
 *                              Host.
 * @param   DimmInfo            Point to a buffer to populate translated
 *                              normalized address data. Host is responsible
 *                              for ensuring the buffer size is sufficient to
 *                              contain SIL_DIMM_INFO (defined in
 *                              RasClass-api.h).
 * @param   AddrData            Dimm information map, created by Host call to
 *                              xPrfCollectDimmMap, used in address translation
 *
 * @return  SIL_STATUS
 *
 */
SIL_STATUS
xPrfMcaErrorAddrTranslate (
  uint64_t                *SystemMemoryAddress,
  SIL_NORMALIZED_ADDRESS  *NormalizedAddress,
  SIL_DIMM_INFO           *DimmInfo,
  SIL_ADDR_DATA           *AddrData
  )
{
  SIL_STATUS    Status;
  RAS_IP2IP_API *RasApi;

  Status = SilGetIp2IpApi (SilId_RasClass, (void **)&RasApi);
  if (Status != SilPass) {
    XPRF_TRACEPOINT (SIL_TRACE_ERROR, "RAS API not found!\n");
    return Status;
  }

  // Convert the normalized address to DRAM address in DimmInfo
  RasApi->TranslateNormToDramAddr (
    NormalizedAddress->NormalizedAddr,
    NormalizedAddress->NormalizedSocketId,
    NormalizedAddress->NormalizedDieId,
    NormalizedAddress->NormalizedChannelId,
    0,
    &DimmInfo->ChipSelect,
    &DimmInfo->Bank,
    &DimmInfo->Row,
    &DimmInfo->Column,
    &DimmInfo->Rankmul,
    &DimmInfo->SubChan,
    AddrData
    );

  // Calculate the system address from the normalized address
  *SystemMemoryAddress = RasApi->CalcSysAddr (
      NormalizedAddress->NormalizedAddr,
      NormalizedAddress->NormalizedSocketId,
      NormalizedAddress->NormalizedChannelId
      );

  if (0xffffffffffffffff == *SystemMemoryAddress) {
    // Return with SilInvalidParameter on invalid system address
    return SilInvalidParameter;
  }

  return SilPass;
}

/*
 * xPrfTranslateSysAddrToCS
 *
 * @brief Translate system address into specific memory DIMM information and
 *        normalized address information
 *
 * @param  SystemMemoryAddress  System Address
 * @param  NormalizedAddress    UMC memory address Information
 * @param  DimmInfo             DIMM information
 * @param  AddrData             Dimm information map used in address translation
 *
 * @return SIL_STATUS
 *
 */
SIL_STATUS
xPrfTranslateSysAddrToCS (
  uint64_t                *SystemMemoryAddress,
  SIL_NORMALIZED_ADDRESS  *NormalizedAddress,
  SIL_DIMM_INFO           *DimmInfo,
  SIL_ADDR_DATA           *AddrData
  )
{
  SIL_STATUS    Status;
  RAS_IP2IP_API *RasApi;

  Status = SilGetIp2IpApi (SilId_RasClass, (void **)&RasApi);
  if (Status != SilPass) {
    XPRF_TRACEPOINT (SIL_TRACE_ERROR, "RAS API not found!\n");
    return Status;
  }

  RasApi->CalcNormAddr (*SystemMemoryAddress, NormalizedAddress);

  if (0xffffffffffffffff == NormalizedAddress->NormalizedAddr) {
    // Return with SilInvalidParameter on invalid system address
    return SilInvalidParameter;
  }

  RasApi->TranslateNormToDramAddr (
    NormalizedAddress->NormalizedAddr,
    NormalizedAddress->NormalizedSocketId,
    NormalizedAddress->NormalizedDieId,
    NormalizedAddress->NormalizedChannelId,
    0,
    &DimmInfo->ChipSelect,
    &DimmInfo->Bank,
    &DimmInfo->Row,
    &DimmInfo->Column,
    &DimmInfo->Rankmul,
    &DimmInfo->SubChan,
    AddrData
    );

  return SilPass;
}

/**
 * xPrfMcaIpIdInstanceIdInit
 *
 * @brief Program the Core MCA_IPID MSR Instance ID values for the CPU specified in RasCpuInfo.
 *
 * @param   RasCpuInfo  The CPU info structure for the core to program (RasClass-api.h)
 *
 * @return  SIL_STATUS
 * @retval  SilPass     If the function completed normally
 * @retval  SilNotFound If RAS API was not found in the API list
 */
SIL_STATUS
xPrfMcaIpIdInstanceIdInit (
  SIL_CPU_INFO  *RasCpuInfo
  )
{
  SIL_STATUS            Status;
  RAS_IP2IP_API         *RasApi;

  Status = SilGetIp2IpApi (SilId_RasClass, (void **)&RasApi);
  if (Status != SilPass) {
    XPRF_TRACEPOINT (SIL_TRACE_ERROR, "RAS API not found!\n");
    return Status;
  }

  RasApi->ProgramCoreMcaIpIdInstanceId (RasCpuInfo);

  return SilPass;
}

/*
 * xPrfProgramCoreMcaConfigUmc
 *
 * @brief  This function will enable/disable FRU text in MCA based on the input flag.
 *
 * @details When enabled the FRU text is reported in McaSynd1/McaSynd2 MSRs.
 *
 * @param  EnableFruText  Input flag to enable/disable FRU text in MCA.
 *
 * @return  SIL_STATUS
 * @retval  SilPass     If the function completed normally
 * @retval  SilNotFound If RAS API was not found in the API list
 */
SIL_STATUS
xPrfProgramCoreMcaConfigUmc (
  bool  EnableFruText
)
{
  SIL_STATUS            Status;
  RAS_IP2IP_API         *RasApi;

  Status = SilGetIp2IpApi (SilId_RasClass, (void **)&RasApi);
  if (Status != SilPass) {
    XPRF_TRACEPOINT (SIL_TRACE_ERROR, "RAS API not found!\n");
    return Status;
  }

  // Enable FRU text for UMC MCA
  RasApi->ProgramCoreMcaConfigUmc (EnableFruText);

  return SilPass;
}

/*
 * xPrfSetIpMcaCtlMask
 *
 * @brief Set the MCA_CTL_MASK for a given IP.
 *
 * @param HardwareId      Hardware ID input to match with MCA Bank MCA IP ID.
 * @param McaType         MCA Type input to match with MCA Bank MCA IP ID.
 * @param IpMcaPolicyCfg  Host firmware input containing platform policy information
 *                        that is used to configure MCA Control mask settings. The structure is a union
 *                        of all supported IP RAS policy structure. The host is responsible for populating the
 *                        appropriate IP structure with valid policy information. Currently supported IP structures:
 *
 *                        - SIL_NBIO_RAS_POLICY
 *
 * @returns SIL_STATUS
 * @retval  SilNotFound   If RAS API was not found in the API list
 * @retval  SilPass       Function completed normally
 *
 */
SIL_STATUS
xPrfSetIpMcaCtlMask (
  uint16_t          HardwareId,
  uint16_t          McaType,
  SIL_IP_RAS_POLICY *IpMcaPolicyCfg
  )
{
  SIL_STATUS            Status;
  RAS_IP2IP_API         *RasApi;

  Status = SilGetIp2IpApi (SilId_RasClass, (void **)&RasApi);
  if (Status != SilPass) {
    XPRF_TRACEPOINT (SIL_TRACE_ERROR, "RAS API not found!\n");
    return Status;
  }

  RasApi->SetIpMcaCtlMask (HardwareId,
                           McaType,
                           IpMcaPolicyCfg
                           );

  return SilPass;
}

/*
 * xPrfTranslateSysAddrToDpa
 *
 * @brief   System Address Translate to DIMM Physical Address (DPA)
 *
 * @details Translate system address into specific memory DIMM information and
 *          normalized address information
 *
 * @param SystemMemoryAddress System Address input
 * @param Dpa                 DIMM Physical Address to output
 * @param AddrData            Dimm information map input used in address
 *                            translation
 *
 * @return SIL_STATUS
 *
 * @retval SilNotFound        RAS API was not found in the API list
 * @retval SilPass            Address translated successfully
 */
SIL_STATUS
xPrfTranslateSysAddrToDpa (
  uint64_t      *SystemMemoryAddress,
  uint64_t      *Dpa,
  SIL_ADDR_DATA *AddrData
  )
{
  SIL_NORMALIZED_ADDRESS NormalizedAddress;
  SIL_DIMM_INFO          DimmInfo;
  SIL_STATUS             Status;
  RAS_IP2IP_API          *RasApi;

  Status = SilGetIp2IpApi (SilId_RasClass, (void **)&RasApi);
  if (Status != SilPass) {
    XPRF_TRACEPOINT (SIL_TRACE_ERROR, "RAS API not found!\n");
    return Status;
  }

  memset (&NormalizedAddress, 0, sizeof (SIL_NORMALIZED_ADDRESS));
  memset (&DimmInfo, 0, sizeof (SIL_DIMM_INFO));

  xPrfTranslateSysAddrToCS (
    SystemMemoryAddress,
    &NormalizedAddress,
    &DimmInfo,
    AddrData
    );

  /*redacted*/

  return SilPass;
}

/*
 * xPrfUpdateDimmFruTextToMca
 *
 * @brief   This function provides the service to add Field Replaceable Unit
 *          (FRU) text to UMC MCA bank
 *
 * @details This function has dependencies on the following openSIL
 *          Services/IPs:
 *
 *          Df->DfGetSystemInfo()
 *          Df->DfGetProcessorInfo()
 *          Df->DfGetRootBridgeInfo()
 *
 * @param   DimmFruTextTable Input of the Dimm Fru Text table.  Host is expected
 *                           to provide the table in a format compatible with
 *                           SIL_DIMM_FRUTEXT_TABLE (defined in RasClass-api.h)
 * @return SIL_STATUS
 *
 * @retval SilNotFound         The RAS IP Api was not found
 * @retval SilInvalidParameter An invalid parameter was passed to
 *                             RasApi->UpdateFruTextToUmc
 * @retval SilPass             FRU text successfully updated
 */
SIL_STATUS
xPrfUpdateDimmFruTextToMca (
  SIL_DIMM_FRUTEXT_TABLE   *DimmFruTextTable
  )
{
  uint32_t      Index;
  SIL_STATUS    Status;
  uint32_t      NumberOfInstalledProcessors;
  uint32_t      NumberOfRootBridgesOnDie;
  uint32_t      TotalNumberOfDie;
  uint32_t      BusNumberBase;
  uint32_t      SocketId;
  uint32_t      DieId;
  uint32_t      SmuFuse;
  RAS_IP2IP_API *RasApi;
  DF_IP2IP_API  *DfApi;

  XPRF_TRACEPOINT (SIL_TRACE_ENTRY, "\n");

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

  NumberOfInstalledProcessors = 0;
  TotalNumberOfDie = 0;
  NumberOfRootBridgesOnDie = 0;
  BusNumberBase = 0;

  DfApi->DfGetSystemInfo (&NumberOfInstalledProcessors, NULL, NULL, NULL, NULL);

  for (SocketId = 0; SocketId < NumberOfInstalledProcessors; SocketId++) {
    DfApi->DfGetProcessorInfo (SocketId, &TotalNumberOfDie, NULL);
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
      XPRF_TRACEPOINT (
        SIL_TRACE_INFO,
        "[RAS] SocketId: 0x%x, Die: 0x%x, Bus Base: 0x%x\n",
        SocketId,
        DieId,
        BusNumberBase
        );

      //Get UMC Harvest FUSE
      SmuFuse = RasApi->GetUmcHarvestFuse (BusNumberBase);

      XPRF_TRACEPOINT (
        SIL_TRACE_INFO,
        "[RAS] AmdDimmFruTextTable entries: 0x%x\n",
        DimmFruTextTable->TableEntryNum
        );
      for (Index = 0; Index < DimmFruTextTable->TableEntryNum;
        Index++) {
        if ((DimmFruTextTable->FrutextEntry[Index].Socket == (uint16_t)SocketId)
          && (DimmFruTextTable->FrutextEntry[Index].Die == (uint16_t)DieId)) {
          if (0 == (SmuFuse &
            ((uint32_t) 1 << DimmFruTextTable->FrutextEntry[Index].Channel))) {
            XPRF_TRACEPOINT (
              SIL_TRACE_INFO,
              "[RAS] Set Frutext for Socket: 0x%x, Die: 0x%x, Channel: 0x%x, Module: 0x%x\n",
              DimmFruTextTable->FrutextEntry[Index].Socket,
              DimmFruTextTable->FrutextEntry[Index].Die,
              DimmFruTextTable->FrutextEntry[Index].Channel,
              DimmFruTextTable->FrutextEntry[Index].Module
              );
            Status = RasApi->UpdateFruTextToUmc (
              BusNumberBase,
              &DimmFruTextTable->FrutextEntry[Index]
              );
            if (Status != SilPass) {
              XPRF_TRACEPOINT (
                SIL_TRACE_ERROR,
                "[RAS] Input Parameter invalid!!!\n"
                );
            }
          }
        }
      } //for (Index = 0; Index < DimmFruTextTable->TableEntryNum; Index++) {
    } //for (DieId = 0; DieId < TotalNumberOfDie; DieId++) {
  } //for (SocketId = 0; SocketId < NumberOfInstalledProcessors; SocketId++) {

  return Status;
}

/*
 * xPrfGetLocalSmiStatus
 *
 * @brief   Get the Local Smi Status from the SMM save state area.
 *
 * @details The top 512 bytes (FE00h to FFFFh) of SMRAM memory space are the
 *          default SMM state-save area. When an SMI occurs, the processor saves
 *          its state in the 512-byte SMRAM state-save area during the control
 *          transfer into SMM.
 *
 * @note  ***This function is expected to be executed on a specific processor
 *        by the Host in a multi-processor environment.
 *
 * @return uint32_t      The local SMI status for the processor
 */
uint32_t
xPrfGetLocalSmiStatus (
    void
  )
{
  SMM_SAVE_STATE    *SmmSaveState;
  size_t            SmmBase;
  SMM_BASE_STRUCT   SmmBaseMsr;

  SmmBaseMsr.Value = xUslRdMsr (MSR_SMM_BASE);
  SmmBase = SmmBaseMsr.Field.SmmBase;
  SmmSaveState = (SMM_SAVE_STATE *)(SmmBase + SMM_SAVE_STATE_OFFSET);

  return (SmmSaveState->LocalSmiStatus);
}
