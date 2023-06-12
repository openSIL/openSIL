/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

/**
 * @file  Sil-api.h
 * @brief
 *  This file is used to declare the Host-API functions, variables,
 *  common definitions, constants and macros available to the Host and
 *  openSIL modules.
 *
 *
 */
/**
 * @cond API_Doc
 *
 *  @defgroup MODULES_IP         IP Modules Interfacing
 *
 * @endcond
 *
 *
 * @cond FullCode_Doc
 * @defgroup Host_group Hostgroup: top level page for openSIL.
 *
 * @endcond
 */

#pragma once
#include <stddef.h>

/** HOST_DEBUG_SERVICE
 *  @brief This is the description for the Host provided debug output function.
 */
  typedef void (*HOST_DEBUG_SERVICE) (
  size_t      MsgLevel,
  const char  *SilPrefix,
  const char  *Message,
  const char  *Function,
  size_t      Line,
  ...
  );

/** @brief Return values
 *
 * @details The openSIL routines will never 'hang'. They will always return with a
 * code or value indicating the state of the outcome of the operation. For
 * functions returning status, these are the defined status codes for
 * all openSIL code.
 * Starting from value 0xF0, this enum defines reset request types from openSIL IPs.
 */
typedef enum {
  SilPass = 0,                    ///< Success, function finished without error.
  SilUnsupportedHardware,         ///< Fail, SoC does not support the function.
  SilUnsupported,                 ///< Fail, the request is not supported.
  SilInvalidParameter,            ///< Fail, the function was called with an
                                  ///<   invalid parameter.
  SilAborted,                     ///< Fail, the processing incurred an error
                                  ///<   causing the processing to be aborted.
                                  ///<   Host should take action.
  SilOutOfResources,              ///< Fail, assignment of memory block failed,
                                  ///<   due to depletion of available resources
  SilNotFound,                    ///< Fail, The element was not found. Recheck
                                  ///<   the ID unique identifier used.
  SilOutOfBounds,                 ///< Fail, there is an out of bounds condition
  SilDeviceError,                 ///< Fail, device being initialized reported
                                  ///<   an error. Check if the IP output block
                                  ///<   has more information.

  SilResetRequestColdImm = 0xF0,  ///< The following values indicate a special
                                  ///<   condition requiring the Host to perform
                                  ///<   a system reset
                                  ///< requesting Host do immediate cold reset
  SilResetRequestColdDef,         ///< requesting Host do deferred cold reset
  SilResetRequestWarmImm,         ///< requesting Host do immediate warm reset
  SilResetRequestWarmDef,         ///< requesting Host do deferred warm reset
} SIL_STATUS;

/* *********************************************************************************************************************
 * Declare common variables here
 */



