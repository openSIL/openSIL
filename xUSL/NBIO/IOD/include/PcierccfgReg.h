/**
 *  @file PcierccfgReg.h
 *  @brief This file contains Pcie rc Register definitions
 */

/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#define SIL_RESERVED_499    0x1a30004cUL

#define AP_CNTL_AP_NEGOTIATION_GLOBAL_EN_OFFSET      8
#define SIL_RESERVED_52     0x100

#define SIL_RESERVED_500    0x1a300538UL

/***********************************************************
* Register Name : DEVICE_CAP2
* Register Description :
* The Device Capabilities 2 register identifies device specific capabilities.
* Visibility : 0x2
************************************************************/
typedef union {
  struct {
    uint32_t                            CPL_TIMEOUT_RANGE_SUPPORTED:4;
    uint32_t                            CPL_TIMEOUT_DIS_SUPPORTED:1;
    uint32_t                            ARI_FORWARDING_SUPPORTED:1;
    uint32_t                            ATOMICOP_ROUTING_SUPPORTED:1;
    uint32_t                            ATOMICOP_32CMPLT_SUPPORTED:1;
    uint32_t                            ATOMICOP_64CMPLT_SUPPORTED:1;
    uint32_t                            CAS128_CMPLT_SUPPORTED:1;
    uint32_t                            NO_RO_ENABLED_P2P_PASSING:1;
    uint32_t                            LTR_SUPPORTED:1;
    uint32_t                            TPH_CPLR_SUPPORTED:2;
    uint32_t                            LN_SYSTEM_CLS:2;
    uint32_t                            TEN_BIT_TAG_COMPLETER_SUPPORTED:1;
    uint32_t                            TEN_BIT_TAG_REQUESTER_SUPPORTED:1;
    uint32_t                            OBFF_SUPPORTED:2;
    uint32_t                            EXTENDED_FMT_FIELD_SUPPORTED:1;
    uint32_t                            END_END_TLP_PREFIX_SUPPORTED:1;
    uint32_t                            MAX_END_END_TLP_PREFIXES:2;
    uint32_t                            EMER_POWER_REDUCTION_SUPPORTED:2;
    uint32_t                            EMER_POWER_REDUCTION_INIT_REQ:1;
    uint32_t                            Reserved_30_27:4;
    uint32_t                            FRS_SUPPORTED:1;
  } Field;
  uint32_t Value;
} DEVICE_CAP2_PCIERCCFG_STRUCT;

#define SIL_RESERVED_501          0x1a30007cUL
#define SIL_RESERVED_55           0
#define SIL_RESERVED_54           0xf
#define SIL_RESERVED_502          0x1a300080UL
#define SIL_RESERVED_57           0
#define SIL_RESERVED_56           0x1
#define SIL_RESERVED_61           2
#define SIL_RESERVED_60           0x4
#define SIL_RESERVED_59           3
#define SIL_RESERVED_58           0x8
#define SIL_RESERVED_503          0x1a300588UL
#define SIL_RESERVED_504          0x1a300064UL
#define SIL_RESERVED3_1462        7
#define SIL_RESERVED3_1484        0x380
#define SIL_RESERVED_506          0x1a300088UL
#define SIL_RESERVED3_1485        0
#define SIL_RESERVED3_1486        0x1
#define SIL_RESERVED3_1487        8
#define SIL_RESERVED3_1488        0x700
#define SIL_RESERVED_505          0x1a300508UL

/***********************************************************
* Register Name : PCIE_CAP
* Register Description :  * The PCI Express Capabilities register identifies PCI Express device type and associated
//capabilities.
* Visibility : 0x2
************************************************************/
// Bitfield Description : (Downstream Ports only) Indicates if the Link associated with this Port is connected to a
//slot. //For Upstream Ports, this bit is undefined.
#define PCIE_CAP_SLOT_IMPLEMENTED_OFFSET          8
#define PCIE_CAP_SLOT_IMPLEMENTED_MASK            0x100
#define SIL_RESERVED_507                          0x1a300058UL
#define SIL_RESERVED_508                          0x1a300388UL
#define SLOT_CAP_SLOT_PWR_LIMIT_VALUE_OFFSET      7
#define SIL_RESERVED_490                          0x7f80
#define SLOT_CAP_SLOT_PWR_LIMIT_SCALE_OFFSET      15
#define SLOT_CAP_SLOT_PWR_LIMIT_SCALE_MASK        0x18000
// Bitfield Description : Indicates the physical slot number attached to this Port.
#define SLOT_CAP_PHYSICAL_SLOT_NUM_OFFSET      19
#define SLOT_CAP_PHYSICAL_SLOT_NUM_MASK        0xfff80000
#define SIL_RESERVED_535                       0x1a30006cUL
