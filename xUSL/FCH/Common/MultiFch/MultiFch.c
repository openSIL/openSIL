/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MultiFch.c
 * @brief Secondary FCH initialization functions
 *
 */

#include <SilCommon.h>
#include <xSIM.h>
#include "MultiFch.h"
#include <DF/DfIp2Ip.h>
#include <FCH/Common/Fch.h>
#include <FCH/Common/FchCommon.h>
#include <FCH/Common/FchReg.h>
#include <RcMgr/RcMgrIp2Ip.h>
#include <FCH/FchIp2Ip.h>
#include <CommonLib/SmnAccess.h>
#include <FCH/FchMultiFch-api.h>
#include "MultiFchCmn2Rev.h"
#include <string.h>
#include <FabricRegisterAccCmn.h>

/**
 * FchMultiFchGetAcpiMmioBaseAddress
 * @brief Returns the ACPI MMIO base address of the FCH root bridge on the input socket/die
 *
 * @param[in]  SocketNum        Socket number
 * @param[in]  DieNum           Die number on the Socket
 * @param[out] AcpiMmioBaseAddr 64-bit pointer to return ACPI MMIO base address
 *
 * @retval SilPass
 * @retval SilNotFound
 *
 */
SIL_STATUS
FchMultiFchGetAcpiMmioBaseAddress (
  uint32_t SocketNum,
  uint32_t DieNum,
  uint64_t *AcpiMmioBaseAddr
  )
{
  FCHMULTIFCH_OUTPUT_BLK  *OutputBlock;
  OutputBlock = (FCHMULTIFCH_OUTPUT_BLK *) xUslFindStructure(SilId_MultiFchClass,
    SIL_RESERVED_0358
    );
  if (OutputBlock == NULL) {
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "MultiFch output block not found!\n");
    assert(OutputBlock != NULL);
    return SilNotFound;
  }
  *AcpiMmioBaseAddr = OutputBlock->FchAcpiMmioBase[DieNum * SIL_RESERVED_0356 + SocketNum];
  return SilPass;
};

/**
 * FchMultiFchGetFchIohcBusNumber
 * @brief Returns IOHC bus number of the FCH root bridge on the input socket/die
 *
 * @param[in]  SocketNum        Socket number
 * @param[in]  DieNum           Die number on the Socket
 * @param[out] IohcBaseAddr     32-bit pointer to return FCH IOHC bus number
 *
 * @retval SilPass
 * @retval SilNotFound
 *
 */
SIL_STATUS
FchMultiFchGetFchIohcBusNumber (
  uint32_t SocketNum,
  uint32_t DieNum,
  uint32_t *IohcBaseAddr
  )
{
  FCHMULTIFCH_OUTPUT_BLK  *OutputBlock;
  OutputBlock = (FCHMULTIFCH_OUTPUT_BLK *) xUslFindStructure(SilId_MultiFchClass,
    SIL_RESERVED_0358
    );
  if (OutputBlock == NULL) {
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "MultiFch output block not found!\n");
    assert(OutputBlock != NULL);
    return SilNotFound;
  }
  *IohcBaseAddr = OutputBlock->FchIohcBusNumber[DieNum * SIL_RESERVED_0356 + SocketNum];
  return SilPass;
};

/**
 * FchMultiFchHasFchModule
 * @brief Indicates whether the FCH root bridge on the input socket/die is populated or not
 *
 * @param[in] SocketNum        Socket number
 * @param[in] DieNum           Die number on the Socket
 * @param[out] HasFchModule     8-bit pointer to flag indicating whether FCH module is populated on the IOD or not
 *
 * @retval SilPass
 * @retval SilNotFound
 *
 */
SIL_STATUS
FchMultiFchHasFchModule (
  uint32_t SocketNum,
  uint32_t DieNum,
  uint8_t  *HasFchModule
  )
{
  FCHMULTIFCH_OUTPUT_BLK  *OutputBlock;
  OutputBlock = (FCHMULTIFCH_OUTPUT_BLK *) xUslFindStructure(SilId_MultiFchClass,
    SIL_RESERVED_0358
    );
  if (OutputBlock == NULL) {
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "MultiFch output block not found!\n");
    assert(OutputBlock != NULL);
    return SilNotFound;
  }
  *HasFchModule = OutputBlock->HasFchModule[DieNum * SIL_RESERVED_0356 + SocketNum];
  return SilPass;
};

/**
 * FchMultiFchGetFchRbIndexOnDieCmn
 * @brief Returns the FCH root bridge index on the IOD
 *
 * @param[out] RbIndex  32-bit pointer to return FCH root bridge index on the IOD
 *
 * @return SIL_STATUS
 *
 */
SIL_STATUS
FchMultiFchGetFchRbIndexOnDieCmn (
  uint32_t *RbIndex
  )
{

  SIL_STATUS    Status;
  DF_IP2IP_API  *DfIp2IpApi;
  bool          HasFchDevice;
  uint32_t      NumberOfRootBridges;
  uint32_t      Index;

  Status = SilGetIp2IpApi(SilId_DfClass, (void **)&DfIp2IpApi);
  if (Status != SilPass) {
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "DF Ip-2-Ip API not found!\n");
    assert(Status == SilPass);
    return Status;
  }

  Status = DfIp2IpApi->DfGetDieInfo(0, 0, &NumberOfRootBridges, NULL, NULL);
  if (Status != SilPass) {
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "DfGetDieInfo failed (Status=0x%x).\n", Status);
    assert(Status == SilPass);
    return Status;
  }

  for (Index = 0; Index < NumberOfRootBridges; Index++) {
    Status = DfIp2IpApi->DfGetRootBridgeInfo(0, 0, Index, NULL, NULL, NULL, NULL, &HasFchDevice, NULL);
    if (Status != SilPass) {
      FCH_TRACEPOINT(SIL_TRACE_ERROR, "DfGetRootBridgeInfo failed (Status=0x%x).\n", Status);
      assert(Status == SilPass);
      return Status;
    }
    if (HasFchDevice) {
      *RbIndex = Index;
      return SilPass;
    }
  }

  return SilNotFound;
}

/**
 * FchMultiFchReadIoBaseAddress
 * @brief Reads X86IOBASEADDRESS and X86IOLIMITADDRESS registers values
 *
 * @param[in]    SocketNum   Socket Number
 * @param[in]    DieNum      Die Number
 * @param[out]   IoBaseVal   32-bit pointer to return current IO base value
 * @param[out]   IoLimitVal  32-bit pointer to return current IO Limit value
 *
 * @return SIL_STATUS
 */
SIL_STATUS
FchMultiFchReadIoBaseAddress (
  uint32_t SocketNum,
  uint32_t DieNum,
  uint32_t *IoBaseVal,
  uint32_t *IoLimitVal
  )
{
  SIL_STATUS   Status;
  DF_IP2IP_API *DfIp2IpApi;

  Status = SilGetIp2IpApi(SilId_DfClass, (void **)&DfIp2IpApi);
  if (Status != SilPass) {
    assert(Status == SilPass);
    return Status;
  }

  *IoBaseVal = DfIp2IpApi->DfFabricRegisterAccRead(SocketNum,
    DieNum,
    SIL_RESERVED_0351,
    DF_X86IOBASEADDRESS,
    FABRIC_REG_ACC_BC
    );

  *IoLimitVal = DfIp2IpApi->DfFabricRegisterAccRead(SocketNum,
    DieNum,
    SIL_RESERVED_0352,
    DF_X86IOLIMITADDRESS,
    FABRIC_REG_ACC_BC
    );

  return SilPass;
}

/**
 * FchMultiFchWriteIoBaseAddress
 * @brief Updates X86IOBASEADDRESS and X86IOLIMITADDRESS registers
 *
 * @param[in]   SocketNum   Socket number
 * @param[in]   DieNum      Die Number
 * @param[in]   IoBaseVal   Target IO base value
 * @param[in]   IoLimitVal  Target IO limit value
 *
 * @return SIL_STATUS
 */
SIL_STATUS
FchMultiFchWriteIoBaseAddress (
  uint32_t SocketNum,
  uint32_t DieNum,
  uint32_t IoBaseVal,
  uint32_t IoLimitVal
  )
{
  SIL_STATUS         Status;
  DF_IP2IP_API       *DfIp2IpApi;

  Status = SilGetIp2IpApi(SilId_DfClass, (void **)&DfIp2IpApi);
  if (Status != SilPass) {
    assert(Status == SilPass);
    return Status;
  }

  DfIp2IpApi->DfFabricRegisterAccWrite(SocketNum,
    DieNum,
    SIL_RESERVED_0351,
    DF_X86IOBASEADDRESS,
    FABRIC_REG_ACC_BC,
    IoBaseVal
    );

  DfIp2IpApi->DfFabricRegisterAccWrite(SocketNum,
    DieNum,
    SIL_RESERVED_0352,
    DF_X86IOLIMITADDRESS,
    FABRIC_REG_ACC_BC,
    IoLimitVal
    );

  return SilPass;
}

/**
 * FchMultiFchAllocateAcpiMmio
 * @brief Allocates ACPI MMIO region for the request root bridge on the socket
 *
 * @param[in]   IohcBusNum    Iohc bus number of the target root bridge on the socket
 * @param[in]   SocketNum     Socket number
 * @param[in]   RbNum         Root bridge number on the socket
 * @param[out]  MmioBaseAddr  64-bit pointer to the allocated ACPI MMIO starting address
 *
 * @return SIL_STATUS
 *
 */
static
SIL_STATUS
FchMultiFchAllocateAcpiMmio (
  uint32_t IohcBusNum,
  uint32_t SocketNum,
  uint32_t RbNum,
  uint64_t *MmioBaseAddr
  )
{
  FABRIC_TARGET             MmioTarget;
  FABRIC_MMIO_ATTRIBUTE     Attributes;
  RCMGR_IP2IP_API           *RcMgrIp2Ip;
  SIL_STATUS                Status;
  uint64_t                  Length;
  uint32_t                  Address32;
  uint32_t                  Data32;

  Status = SilGetIp2IpApi(SilId_RcManager, (void **)(&RcMgrIp2Ip));
  if (Status != SilPass) {
    FCH_TRACEPOINT(SIL_TRACE_ERROR, " RcMgrIp2Ip API not found.\n");
    assert(Status == SilPass);
    return Status;
  }

  Length = 0x2000;
  MmioTarget.TgtType = TARGET_RB;
  MmioTarget.SocketNum = (uint16_t)SocketNum;
  MmioTarget.RbNum = (uint16_t)RbNum;
  MmioTarget.PciSegNum = 0;
  MmioTarget.PciBusNum = 0;
  Attributes.ReadEnable = 1;
  Attributes.WriteEnable = 1;
  Attributes.NonPosted = 0;
  Attributes.MmioType = NON_PCI_DEVICE_BELOW_4G;

  Status = RcMgrIp2Ip->FabricReserveMmio(MmioBaseAddr,
    &Length,
    ALIGN_64K,
    MmioTarget,
    &Attributes
    );

  if (Status != SilPass) {
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "FabricReserveMmio failed (Status=0x%x).\n", Status);
    assert(Status == SilPass);
    return Status;
  }

  Address32 = *((uint32_t *)MmioBaseAddr) & 0xFFFF0000;
  Data32 = Address32 | 0x0100;
  xUSLSmnWrite(0, IohcBusNum, SIL_RSVD_ADDR_02D01000 + PMIO_BASE + FCH_PM_PWRRSTDEBSEL, Data32);

  return SilPass;
}

/**
 * FchMultiFchDispatchSecondaryInits
 * @brief Dispatches all the sub-modules secondary init functions
 *
 * @return SIL_STATUS
 *
 */
SIL_STATUS
FchMultiFchDispatchSecondaryInits (
  void
  )
{
  uint32_t               DieNum;
  uint32_t               SocketNum;
  uint32_t               RbNum;
  uint64_t               AcpiMmioBaseAddr;
  uint32_t               IohcBusNumber;
  uint8_t                HasFchModule;
  SIL_STATUS             Status;
  FCHMULTIFCH_OUTPUT_BLK *MultiFchOutputBlock;
  SIL_RESERVED_STRUCT_0026   *MultiFchC2R;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  Status = SilGetCommon2RevXferTable(SilId_MultiFchClass, (void **)&MultiFchC2R);
  if (Status != SilPass) {
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "MultiFchC2R transfer call not found!\n");
    assert(Status == SilPass);
    return Status;
  }

  MultiFchOutputBlock = (FCHMULTIFCH_OUTPUT_BLK *) xUslFindStructure(SilId_MultiFchClass,
    SIL_RESERVED_0358
    );
  if (MultiFchOutputBlock == NULL) {
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "MultiFchOutputBlock not found!\n");
    assert(MultiFchOutputBlock != NULL);
    return SilNotFound;
  }

  if ((MultiFchOutputBlock->NumberOfSockets == 0x1) && (MultiFchOutputBlock->NumberOfDiePerSocket == 0x01)) {
    FCH_TRACEPOINT(SIL_TRACE_INFO, "Platform-specific multi FCH initialization skipped.");
    return SilPass;
  }

  Status = MultiFchC2R->field1(&RbNum);
  if (Status != SilPass) {
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "FchMultiFchGetFchRbIndexOnDie failed (Status=0x%x).\n", Status);
    assert(Status == SilPass);
    return Status;
  }

  for ( SocketNum = 0; SocketNum < MultiFchOutputBlock->NumberOfSockets; SocketNum++ ) {
    for ( DieNum = 0; DieNum < MultiFchOutputBlock->NumberOfDiePerSocket; DieNum++ ) {
      if ((SocketNum == 0) && (DieNum == 0)) {
        // This case should have already taken care of. We can safely skip.
        continue;
      }

      Status = FchMultiFchGetAcpiMmioBaseAddress(SocketNum, DieNum, &AcpiMmioBaseAddr);
      if (Status != SilPass) {
        FCH_TRACEPOINT(SIL_TRACE_ERROR, "FchMultiFchGetAcpiMmioBaseAddress failed (Status=%x).\n", Status);
        assert(Status == SilPass);
        return Status;
      }

      Status = FchMultiFchGetFchIohcBusNumber(SocketNum, DieNum, &IohcBusNumber);
      if (Status != SilPass) {
        FCH_TRACEPOINT(SIL_TRACE_ERROR, "FchMultiFchGetFchIohcBusNumber failed (Status=%x).\n", Status);
        assert(Status == SilPass);
        return Status;
      }

      Status = FchMultiFchHasFchModule(SocketNum, DieNum, &HasFchModule);
      if (Status != SilPass) {
        FCH_TRACEPOINT(SIL_TRACE_ERROR, "FchMultiFchHasFchModule failed (Status=%x).\n", Status);
        assert(Status == SilPass);
        return Status;
      }

      // Secondary ABs Init
      FCH_TRACEPOINT(SIL_TRACE_INFO,
        "Dispatching MultiFchInits for Socket %d, Die %d, Rb %d,\n",
        SocketNum,
        DieNum,
        RbNum
        );
      FCH_TRACEPOINT(SIL_TRACE_INFO,
        "With AcpiMmioBaseAddr of 0x%x, IohcBusNumber of 0x%x, and HasFchModule=%d.\n",
        (uint32_t)AcpiMmioBaseAddr,
        IohcBusNumber,
        HasFchModule
        );

      FCH_TRACEPOINT(SIL_TRACE_INFO, "FchMultiFchInitAb...\n");
      Status = MultiFchC2R->field2(SocketNum,
        DieNum,
        RbNum,
        AcpiMmioBaseAddr,
        IohcBusNumber,
        HasFchModule
        );
      if (Status != SilPass) {
        FCH_TRACEPOINT(SIL_TRACE_ERROR, "FchMultiFchInitAb failed (Status=%x).\n", Status);
        assert(Status == SilPass);
        return Status;
      }

      // Secondary SDs Init
      FCH_TRACEPOINT(SIL_TRACE_INFO, "FchMultiFchInitSd...\n");
      Status = MultiFchC2R->field4(SocketNum,
        DieNum,
        RbNum,
        AcpiMmioBaseAddr,
        IohcBusNumber,
        HasFchModule
        );
      if (Status != SilPass) {
        FCH_TRACEPOINT(SIL_TRACE_ERROR, "FchMultiFchInitSd failed (Status=%x).\n", Status);
        assert(Status == SilPass);
        return Status;
      }

      // Secondary USBs Init
      FCH_TRACEPOINT(SIL_TRACE_INFO, "FchMultiFchInitUsb...\n");
      Status = MultiFchC2R->field3(SocketNum,
        DieNum,
        RbNum,
        AcpiMmioBaseAddr,
        IohcBusNumber,
        HasFchModule
        );
      if (Status != SilPass) {
        FCH_TRACEPOINT(SIL_TRACE_ERROR, "FchMultiFchInitUsb failed (Status=%x).\n", Status);
        assert(Status == SilPass);
        return Status;
      }

      // Secondary SATAs Init
      FCH_TRACEPOINT(SIL_TRACE_INFO, "FchMultiFchInitSata...\n");
      Status = MultiFchC2R->field5(SocketNum,
        DieNum,
        RbNum,
        AcpiMmioBaseAddr,
        IohcBusNumber,
        HasFchModule
        );
      if (Status != SilPass) {
        FCH_TRACEPOINT(SIL_TRACE_ERROR, "FchMultiFchInitSata failed (Status=%x).\n", Status);
        assert(Status == SilPass);
        return Status;
      }
    }
  }

  return SilPass;
}

/**
 * FchMultiFchInitAcpiMmioAllocation
 * @brief Initiates the ACPI MMIO allocation for all FCH modules
 * on non-primary socket/die
 *
 * @return SIL_STATUS
 *
 */
SIL_STATUS
FchMultiFchInitAcpiMmioAllocation (
  void
  )
{
  uint32_t                          NumberOfSockets;
  uint32_t                          TotalNumberOfDie;
  uint32_t                          TotalNumberOfRootBridges;
  uint32_t                          NumberOfDiePerSocket;
  uint32_t                          NumberOfRbPerDie;
  uint32_t                          SocketNum;
  uint32_t                          DieNum;
  uint32_t                          RbNum;
  uint32_t                          FchRbIndexOnDie;
  uint64_t                          MmioBaseAddr;
  uint32_t                          IohcBusNumber;
  DF_IP2IP_API                      *DfIp2IpApi;
  SIL_STATUS                        Status;
  FCHMULTIFCH_OUTPUT_BLK            *MultiFchOutputBlock;
  SIL_RESERVED_STRUCT_0026              *MultiFchC2R;
  uint8_t                           HasFchModule;
  bool                              HasFchModuleBool;

  FCH_TRACEPOINT(SIL_TRACE_ENTRY, "\n");

  MultiFchOutputBlock = (FCHMULTIFCH_OUTPUT_BLK *) xUslFindStructure(SilId_MultiFchClass,
    SIL_RESERVED_0358
    );

  if (MultiFchOutputBlock == NULL) {
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "MultiFchOutputBlock not found!\n");
    assert(MultiFchOutputBlock != NULL);
    return SilNotFound;
  }

  Status = SilGetCommon2RevXferTable(SilId_MultiFchClass, (void **)&MultiFchC2R);
  if (Status != SilPass) {
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "MultiFchC2R transfer call not found!\n");
    assert(Status == SilPass);
    return Status;
  }

  Status = SilGetIp2IpApi(SilId_DfClass, (void *) &DfIp2IpApi);
  if (Status != SilPass) {
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "DF Ip-2-Ip API not found!\n");
    assert(Status == SilPass);
    return Status;
  }

  Status = DfIp2IpApi->DfGetSystemInfo(&NumberOfSockets,
    &TotalNumberOfDie,
    &TotalNumberOfRootBridges,
    NULL,
    NULL
    );

  if (Status != SilPass) {
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "Ip-2-Ip DfGetSystemInfo failed (Status=%x).\n", Status);
    assert(Status == SilPass);
    return Status;
  }

  FCH_TRACEPOINT(SIL_TRACE_INFO, "NumberOfProcessors=0x%x\n", NumberOfSockets);
  FCH_TRACEPOINT(SIL_TRACE_INFO, "TotalNumberOfDie=0x%x\n", TotalNumberOfDie);
  FCH_TRACEPOINT(SIL_TRACE_INFO, "TotalNumberOfRootBridges=0x%x\n", TotalNumberOfRootBridges);

  NumberOfDiePerSocket = TotalNumberOfDie / NumberOfSockets;
  NumberOfRbPerDie = TotalNumberOfRootBridges / TotalNumberOfDie;
  Status = MultiFchC2R->field1(&FchRbIndexOnDie);
  if (Status != SilPass) {
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "FchMultiFchGetFchRbIndexOnDie failed (Status=0x%x).\n", Status);
    assert(Status == SilPass);
    return Status;
  }
  MultiFchOutputBlock->NumberOfSockets = NumberOfSockets;
  MultiFchOutputBlock->NumberOfDiePerSocket = NumberOfDiePerSocket;
  MultiFchOutputBlock->NumberOfRbPerDie = NumberOfRbPerDie;

  // Assumption: Each die has exactly one FCH module (whether powered or not doesn't matter)
  MultiFchOutputBlock->ArraySize = TotalNumberOfDie;

  FCH_TRACEPOINT(SIL_TRACE_INFO,
    "NumberOfDiePerSocket: %d, NumberOfRbPerDie: %d, FchRbIndexOnDie: %d.\n",
    NumberOfDiePerSocket,
    NumberOfRbPerDie,
    FchRbIndexOnDie
    );

  // Mmio allocation for non-IOD0 on socket0
  for ( SocketNum = 0; SocketNum < NumberOfSockets; SocketNum++ ) {
    for ( DieNum = 0; DieNum < NumberOfDiePerSocket; DieNum++ ) {
      if ((SocketNum == 0) && (DieNum == 0)) {
        // Special case
        MultiFchOutputBlock->FchAcpiMmioBase[0] = FCH_ACPI_MMIO_BASE;
        MultiFchOutputBlock->FchIohcBusNumber[0] = 0x00;
        MultiFchOutputBlock->HasFchModule[0] = 0x1;
        continue;
      }
      MmioBaseAddr = 0;
      Status = DfIp2IpApi->DfGetRootBridgeInfo(SocketNum,
        DieNum,
        FchRbIndexOnDie,
        NULL,
        &IohcBusNumber,
        NULL,
        NULL,
        &HasFchModuleBool,
        NULL
        );
      if (Status != SilPass) {
        FCH_TRACEPOINT(SIL_TRACE_ERROR, "DfGetRootBridgeInfo failed (Status=0x%x).\n", Status);
        assert(Status == SilPass);
        return Status;
      }
      HasFchModule = HasFchModuleBool ? 0x01 : 0x00;
      FCH_TRACEPOINT(SIL_TRACE_INFO,
        "Allocating Mmio address for SocketNum: %d, DieNum: %d, RbNum: %d, IohcBusNumber: 0x%x, HasFchModule: 0x%x.\n",
        SocketNum,
        DieNum,
        FchRbIndexOnDie,
        IohcBusNumber,
        HasFchModule
        );
      RbNum = DieNum * NumberOfRbPerDie + FchRbIndexOnDie;
      Status = FchMultiFchAllocateAcpiMmio(IohcBusNumber, SocketNum, RbNum, &MmioBaseAddr);
      if (Status != SilPass) {
        FCH_TRACEPOINT(SIL_TRACE_ERROR, "FchMultiFchAllocateAcpiMmio failed (Status=0x%x).\n", Status);
        assert(Status == SilPass);
        return Status;
      }
      FCH_TRACEPOINT(SIL_TRACE_INFO, "Allocated MmioBaseAddr is 0x%x.\n", (uint32_t)MmioBaseAddr);
      // Update Fch Output block data
      MultiFchOutputBlock->HasFchModule[DieNum * SIL_RESERVED_0356 + SocketNum] = HasFchModule;
      MultiFchOutputBlock->FchAcpiMmioBase[DieNum * SIL_RESERVED_0356 + SocketNum] = MmioBaseAddr;
      MultiFchOutputBlock->FchIohcBusNumber[DieNum * SIL_RESERVED_0356 + SocketNum] = IohcBusNumber;
    }
  }

  FCH_TRACEPOINT(SIL_TRACE_EXIT, "\n");
  return SilPass;
}

/**
 * FchMultiFchSetInputBlk
 * @brief Initializes FCH MultiFCH Input/Output data block
 *
 * @retval SilNotFound
 * @retval SilPass
 *
 */
SIL_STATUS
FchMultiFchSetInputBlk (
  void
  )
{
  FCHMULTIFCH_OUTPUT_BLK  *MultiFchOutput;

  MultiFchOutput = (FCHMULTIFCH_OUTPUT_BLK *)SilCreateInfoBlock(SilId_MultiFchClass,
    sizeof (FCHMULTIFCH_OUTPUT_BLK),
    SIL_RESERVED_0358,
    SIL_RESERVED_0353,
    SIL_RESERVED_0357
    );

  if (MultiFchOutput == NULL) {
    FCH_TRACEPOINT(SIL_TRACE_ERROR, "MultiFch output block not found.\n");
    return SilNotFound;
  }

  FCH_TRACEPOINT(SIL_TRACE_INFO, "SIL MultiFchClass Output Blk at: 0x%x \n", MultiFchOutput);
  memset((void *)MultiFchOutput, 0, sizeof (FCHMULTIFCH_OUTPUT_BLK));

  return SilPass;
}
