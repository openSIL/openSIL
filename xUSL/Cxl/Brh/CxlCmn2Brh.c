/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file CxlCmn2Brh.c
 * @brief Defines C2R Table for CXL Brh
 *
 */

#include <Cxl/Common/CxlCmn2Rev.h>
#include <Cxl/Brh/CxlCmn2Brh.h>

/** ---------------------------- Cmn2Rev Transfer Table ---------------------------------
 * @details This is the internal common-2-Rev transfer table for BRH
 */
CXL_COMMON_2_REV_XFER_BLOCK CxlXferBrh = {
  .CxlAssignResources = CxlAssignResourcesBrh,
  .CxlFindPorts = CxlFindPortsBrh,
  .CxlDevListGenerate = CxlDevListGenerateBrh
};
