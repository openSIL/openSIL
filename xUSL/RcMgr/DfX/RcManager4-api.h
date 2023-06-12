/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT
/**
 * @file  RcManager-api.h
 * @brief openSIL-Host Resource Manager IP interface
 *
 * @details The RCmgr API provides some controls for how to distribute the
 *  resources of the PCIe devices. Note: This is NOT the assignment of
 *  resources for the end-point devices ('PCIe device enumeration'). That is
 *  performed by the Host firmware. The RCmgr distributes the SoC avaiable
 *  resources in groups to the root bridges in preparation for that later
 *  device enumeration.
 *
 *  This file provides the structures details for the Host to configure the
 *  base RCmanager operations.
 */
/*
 * Note: This file is the Host API definitions.
 *    Include into this file ONLY those items the Host needs to access
 *    and work with your data structure(s)
 */


/** @cond API_Doc
 *  @ingroup MODULES_IP
 *  @page IP_RCMGR   Resource Manager api
 *
 *   The Resource Manager (RCManager) handles the distribution of the MMIO
 *   and IO resources particularly to the PCI root bridges.
 *
 *   Input parameters reflect platform organization of the PCIe devices
 *   especially when there are multiple root bridges - showing a favor to one
 *   root bridge over another for distributing the resources.
 *
 *   See the 'Files - @ref RcManager-api.h' section of this document for
 *   further details.
 *
 *
 * @endcond
 */
#pragma once

#include <RcMgr/FabricResourceManager.h>
#include <ApobCmn.h>
#include <DF/DfX/FabricAcpiDomain/FabricAcpiDomainInfo.h>

#pragma pack(push, 1)

typedef struct {
  DFX_FABRIC_IO_MANAGER   IoRcMgr;
  DFX_FABRIC_MMIO_MANAGER MmioRcMgr;
  bool      SetRcBasedOnNv;
  uint8_t   SocketNumber;
  uint8_t   RbsPerSocket;
  bool      McptEnable;
  uint64_t  PciExpressBaseAddress;
  uint32_t  BottomMmioReservedForPrimaryRb;
  uint32_t  MmioSizePerRbForNonPciDevice;
  uint64_t  MmioAbove4GLimit;
  uint32_t  Above4GMmioSizePerRbForNonPciDevice;
  uint8_t   BmcSocket;
  uint8_t   EarlyBmcLinkLaneNum;
  bool      ResourceDistributionNv[RCMGR_MAX_SOCKETS * DFX_MAX_RBS_PER_SOCKET];
  DFX_FABRIC_RESOURCE_FOR_EACH_RB ResourceSizeForEachRb;
} DFX_RCMGR_INPUT_BLK;

typedef struct {
  bool                     AmdFabricCcxAsNumaDomain;
  uint32_t                 CcdCount[MAX_SOCKETS_SUPPORTED];
  uint32_t                 CcxPerCcd[MAX_SOCKETS_SUPPORTED];
  uint32_t                 LogToPhysCcd[MAX_SOCKETS_SUPPORTED][DFX_MAX_CCD_PER_SOCKET];
  uint32_t                 ReportedDomainCcxMap[MAX_REPORTED_DOMAINS];
  FABRIC_DOMAIN_INFO2      ReportedDomainInfo[MAX_REPORTED_DOMAINS];
  FABRIC_PHYS_DOMAIN_INFO  PhysicalDomainInfo[MAX_PHYSICAL_DOMAINS];
  bool                     DomainInfoValid;
  uint32_t                 NumberOfReportedDomains;
} DFX_RCMGR_OUTPUT_BLK;

typedef struct {
  DFX_RCMGR_INPUT_BLK  DFXRcmgrInputBlock;
  DFX_RCMGR_OUTPUT_BLK DFXRcmgrOutputBlock;
} DFX_RCMGR_DATA_BLK;

#pragma pack(pop)
