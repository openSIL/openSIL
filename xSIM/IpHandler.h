/**
 * @file
 * This file declares the private xSIM functions, variables and
 * macros used to handle the IP classes and Instances.
 */

#pragma once

typedef SIL_STATUS (*FCN_SET_INPUT) (void);
typedef SIL_STATUS (*FCN_IP_INIT) (void);
typedef SIL_STATUS (*SIL_API_INIT) (void);

/* *********************************************************************************************************************
 * Declare common variables here
 */

/** IP Record
 * @details This record is unique for each IP  (device). It provided the details
 *    about the IP that the xSIM infrastructure needs to invoke the IP
 *    support
 */
typedef struct {
  SIL_DATA_BLOCK_ID  IpID;           ///< Identifier of the IP
  uint32_t           BlkRequestSize; ///< Size to request for Input/Output Blocks
  FCN_SET_INPUT      SetInput;       ///< pointer to IP function to establish its Input Block
  FCN_IP_INIT        Initialize;     ///< pointer to IP function to initialize silicon
  SIL_API_INIT       ApiInit;        ///< pointer to the IP function to initialize internal IP API and the Ip-to-Ip
                                     ///< API structure pointer.
} IP_RECORD;

/**
 * This data element identifies the SoC and the IP blocks contained
 * therein. These IP blocks may be shared with other SoCs.
 * The xSim common vars are SoC descriptors used by the openSIL code
 * which provide information about the SoC silicon (e.g. Number of
 * instances of CCX blocks )
 */
typedef struct {
  uint32_t            SocLogicalId;         ///< bitmapped, see SilSocLogicalId.h
  ACTIVE_SOC_DATA     XsimVars;             ///< SoC version of xSim common Vars
  IP_RECORD           IpList[];             ///< Array of IPs contained in this SoC
} SOC_IP_TABLE;
