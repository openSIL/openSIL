/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file SocList.c
 * This file contains the master list of SoCs supported by the platform
 * as set by the config tool. Each entry in the table points to an
 * openSIL SoC IP block list that is (contained in xSIM).
 */

#include <xSIM.h>
#include "IpHandler.h"

/**
 * Extern SoC IP Tables
 *
 */
extern const SOC_IP_TABLE SocIpTblF19M70Tp1;
extern const SOC_IP_TABLE SocIpTblF19M70Tp2;
extern const SOC_IP_TABLE SocIpTblF19M70Tp3;

/**
 * Configure SoC List
 *
 */

// TP1 SoC IP Tables
const SOC_IP_TABLE *mPlatformSocsTp1[] = {

  &SocIpTblF19M70Tp1,
  NULL  // End of list marker
};

// TP2 SoC IP Tables
const SOC_IP_TABLE *mPlatformSocsTp2[] = {
  &SocIpTblF19M70Tp2,
  NULL  // End of list marker
};

// TP3 SoC IP Tables
const SOC_IP_TABLE *mPlatformSocsTp3[] = {
  &SocIpTblF19M70Tp3,
  NULL  // End of list marker
};
