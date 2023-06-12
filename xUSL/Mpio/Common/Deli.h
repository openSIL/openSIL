/* SPDX-License-Identifier: MIT */
/*
 *****************************************************************************
 *
 * Copyright (C) 2008 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 * AMD is granting you permission to use this software and documentation (if
 * any) (collectively, the "Materials") pursuant to the terms and conditions
 * of the Software License Agreement included with the Materials.  If you do
 * not have a copy of the Software License Agreement, contact your AMD
 * representative for a copy.
 *
 * You agree that you will not reverse engineer or decompile the Materials, in
 * whole or in part, except as allowed by applicable law.
 *
 * WARRANTY DISCLAIMER: THE MATERIALS ARE PROVIDED "AS IS" WITHOUT WARRANTY OF
 * ANY KIND.  AMD DISCLAIMS ALL WARRANTIES, EXPRESS, IMPLIED, OR STATUTORY,
 * INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE, NON-INFRINGEMENT, THAT THE
 * MATERIALS WILL RUN UNINTERRUPTED OR ERROR-FREE OR WARRANTIES ARISING FROM
 * CUSTOM OF TRADE OR COURSE OF USAGE.  THE ENTIRE RISK ASSOCIATED WITH THE
 * USE OF THE MATERIAL IS ASSUMED BY YOU.  Some jurisdictions do not allow the
 * exclusion of implied warranties, so the above exclusion may not apply to
 * You.
 *
 * LIMITATION OF LIABILITY AND INDEMNIFICATION:  AMD AND ITS LICENSORS WILL
 * NOT, UNDER ANY CIRCUMSTANCES BE LIABLE TO YOU FOR ANY PUNITIVE, DIRECT,
 * INCIDENTAL, INDIRECT, SPECIAL OR CONSEQUENTIAL DAMAGES ARISING FROM USE OF
 * THE MATERIALS OR THIS AGREEMENT EVEN IF AMD AND ITS LICENSORS HAVE BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.  In no event shall AMD's total
 * liability to You for all damages, losses, and causes of action (whether in
 * contract, tort (including negligence) or otherwise) exceed the amount of
 * $100 USD.  You agree to defend, indemnify and hold harmless AMD and its
 * licensors, and any of their directors, officers, employees, affiliates or
 * agents from and against any and all loss, damage, liability and other
 * expenses (including reasonable attorneys' fees), resulting from Your use of
 * the Materials or violation of the terms and conditions of this Agreement.
 * U.S. GOVERNMENT RESTRICTED RIGHTS: The Materials are "commercial items", as
 * that term is defined at 48 C.F.R. Section 2.101, consisting of "commercial
 * computer software" and "commercial computer software documentation", as
 * such terms are used in 48 C.F.R. Section 12.212 and 48 C.F.R. Section
 * 227.7202, respectively. Consistent with 48 C.F.R. Section 12.212 or 48
 * C.F.R. Section 227.7202-1 through 227.7202-4, as applicable, the commercial
 * computer software and commercial computer software documentation are being
 * licensed to U.S.  Government end users (a) only as commercial items and (b)
 * with only those rights as are granted to all other end users pursuant to
 * the terms and conditions set forth in this Agreement. Unpublished rights
 * are reserved under the copyright laws of the United States.
 *
 * EXPORT RESTRICTIONS: You shall adhere to all applicable U.S. import/export
 * laws and regulations, as well as the import/export control laws and
 * regulations of other countries as applicable. You further agrees to not
 * export, re-export, or transfer, directly or indirectly, any product,
 * technical data, software or source code received from AMD under this
 * license, or the direct product of such technical data or software to any
 * country for which the United States or any other applicable government
 * requires an export license or other governmental approval without first
 * obtaining such licenses or approvals; or in violation of any applicable
 * laws or regulations of the United States or the country where the technical
 * data or software was obtained.  You acknowledge that the technical data and
 * software received will not, in the absence of authorization from U.S. or
 * local law and regulations as applicable, be used by or exported,
 * re-exported or transferred to: (i) any sanctioned or embargoed country, or
 * to nationals or residents of such countries; (ii) any restricted end-user
 * as identified on any applicable government end-user list; or (iii) any
 * party where the end-use involves nuclear, chemical/biological weapons,
 * rocket systems, or unmanned air vehicles.   For the most current Country
 * Group listings, or for additional information about the EAR or Licensee's
 * obligations under those regulations, please refer to the U.S. Bureau of
 * Industry and Security's website at http://www.bis.doc.gov/.
 *******************************************************************************
 *
 */
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
} DXIO_FW_MP1_DELI_DXIO_LGS;                          // Dxio Lane Groups (LGS)

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
  uint8_t         PCIE_port_num;      // Assigned PCIe port number

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
