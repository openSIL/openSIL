/* Copyright 2008-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#ifndef AMD_DELI_H
#define AMD_DELI_H

#define DXIO_FW_MP1_DELI_MAX_NUM_DXIO_LGS   4
#define DXIO_FW_MP1_DELI_NUM_PROTO_VALS     4

typedef struct
{
  uint8_t                       engType;      // = 1 for PCIe and 3 for sata
  uint8_t                       numDWords;
  uint8_t                       version;
  uint8_t                       reserved1;
} DXIO_FW_MP1_DELI_HEADER;

typedef struct
{
  uint32_t                      KPNP_startPhyLane;
  uint32_t                      KPNP_endPhyLane;
  uint32_t                      KPNP_LANE_REQ_STATUS; // Tx/Rx acks ;
  uint32_t                      KPMX_HWDEBUG;         // Engaged etc.
} DXIO_FW_MP1_DELI_DXIO_LGS;  // Dxio Lane Groups (LGS)

typedef struct
{
  uint8_t                       numLGroups;
  uint8_t                       version;
  uint8_t                       reserved1;
  uint8_t                       reserved2;
  DXIO_FW_MP1_DELI_DXIO_LGS   LGS[DXIO_FW_MP1_DELI_MAX_NUM_DXIO_LGS];
} DXIO_FW_MP1_DELI_DXIO;

typedef struct
{
  uint8_t         version;
  uint8_t         reserved1;
  uint8_t         reserved2;
  uint8_t         reserved3;
  uint32_t        VAL[DXIO_FW_MP1_DELI_NUM_PROTO_VALS]; // Space Holder
} DXIO_FW_MP1_DELI_PORTO_BASE;

typedef struct
{
  uint8_t         version;
  uint8_t         reserved1;
  uint8_t         reserved2;
  uint8_t         reserved3;

  uint32_t        PCIE_link_state;    // from BIFP:PCIE_LC_STATE0

  uint8_t         PCIE_link_speed;    // from BIFPLR:LINK_STATUS
  uint8_t         PCIE_link_width;    // from BIFPLR:LINK_STATUS
  uint8_t         PCIE_link_active;   // from BIFPLR:LINK_STATUS
  uint8_t         reserved4;

  uint32_t        SWRST_CONTROL_6;    // for HOLD_TRAINING bits
  uint32_t        reserved5;
} DXIO_FW_MP1_DELI_PROTO_PCIE;

typedef struct
{
  uint8_t         version;
  uint8_t         reserved1;
  uint8_t         reserved2;
  uint8_t         reserved3;
  uint32_t        SATA_driver_detected;
  uint32_t        SATA_driver_speed;
  uint32_t        SATA_unused1;
  uint32_t        SATA_unused2;
} DXIO_FW_MP1_DELI_PROTO_SATA;

typedef union
{
  DXIO_FW_MP1_DELI_PORTO_BASE     Base;
  DXIO_FW_MP1_DELI_PROTO_PCIE     Pcie;
  DXIO_FW_MP1_DELI_PROTO_SATA     Sata;
} DXIO_FW_MP1_DELI_PROTO_BASE_U;


// the main info struct has 3 parts ... a header, a dxio part and a protocol-specific part
// there is a dxio sub-section for each lane group of an engine. Define the
// maximum number of lane groups per engine
typedef struct
{
  DXIO_FW_MP1_DELI_HEADER           Hdr;
  DXIO_FW_MP1_DELI_DXIO             Dxio;
  DXIO_FW_MP1_DELI_PROTO_BASE_U     Proto;
} DXIO_FW_MP1_DELI_INFO;

#endif
