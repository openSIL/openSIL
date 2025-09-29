/* SPDX-License-Identifier: MIT */
/**
 * Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 */
/**
 * @file SocServices.c
 * @brief Soc specific services interface
 *
 */


#include <SilCommon.h>
#include <CCX/Common/SocServices.h>
#include <CCX/Common/Ccx.h>

/**
 * SocGetSoftDowncoreAddress
 * @brief This service retrieves the SMN address for CCD-N's (physical or virtual) soft downcore
 * register.
 *
 * @param[in] CcdIndex         CCD index
 *
 * @return                     The SMN address for CCD-N's soft downcore register.
 *
 **/
uint32_t
SocGetSoftDowncoreAddress (
  uint32_t CcdIndex
  )
{
  if (CcdIndex < 8) {
    return 0x30081020 + (CcdIndex << 25);
  } else {
    return 0x4A081020 + ((CcdIndex - 8) << 25);
  }
}
