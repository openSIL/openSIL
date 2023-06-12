/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT
/**
 * @file
 * XMP Class IP input defaults
 *
 * This file holds the platform default values for the DF class module.
 *
 *
 *>>> Note to Module owner:
 *    This is a template file. Please replace, or add to, this file
 *    with the code appropriate for your IPClass module.
 */

#include <SilCommon.h>
#include <XmpClass-api.h>


/* Validate the Kcfg settings.
   Depending on the selected options, some default values may not be
   present in the Kcfg-out.h file. Set those to zero (0)
   Normally, if the module is not to be included in the build, then the
   related variables do not matter. However, some CI test tools will
   compile ALL files regardless. For this case, we make sure the variables
   have a value.
 */

#ifndef CONFIG_XMP_FEAT1_VAR
    #define CONFIG_XMP_FEAT1_VAR  0
#endif

#ifndef CONFIG_XMP_FEAT2_VAR
    #define CONFIG_XMP_FEAT2_VAR  0
#endif

#ifndef CONFIG_XMP_VAR_TWO
    #define CONFIG_XMP_VAR_TWO  0
#endif

#ifndef CONFIG_XMP_FEAT_X_EN
    #define CONFIG_XMP_FEAT_X_EN  0
#endif


/* Declare the common defaults instance */
const XMPCLASS_INPUT_BLK   XmpClassDflts = {
  /* This is where you declare all the common input block vars/values
   * you want to share with the Host.
   * This becomes part of the IP API for the Host.
   */
  .MyFeat1Var       = CONFIG_XMP_FEAT1_VAR,
  .MyFeat2Var       = CONFIG_XMP_FEAT2_VAR,
  .MyVarTwo         = CONFIG_XMP_VAR_TWO,
  .MyFeatXEn        = CONFIG_XMP_FEAT_X_EN

};

