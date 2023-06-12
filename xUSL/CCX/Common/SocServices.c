/**
 * @file SocServices.c
 * @brief Soc specific services interface
 *
 */

/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

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

/**
 * SocIsOneCcdModeSupported
 * @brief This service returns whether or not the SoC supports disabling all but one CCD.
 *
 *
 * @retval true                               Downing to 1 CCD is supported
 * @retval false                              Downing to 1 CCD is not supported
 *
 **/
bool SocIsOneCcdModeSupported (void) {
  return false;
}

/**
 * SocIsS3Supported
 * @brief Returns whether or not the SoC supports the S3 sleep state.
 *
 * @retval true       S3 is supported by the SoC
 * @retval false      S3 is not supported by the SoC
 */
bool SocIsS3Supported (void) {
  return false;
}

/**
 * SocIsGameModeSupported
 * @brief Returns whether or not the SoC supports game mode.
 *
 * @retval true
 * @retval false
 */
bool SocIsGameModeSupported (void) {
  return false;
}
