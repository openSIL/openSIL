/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MpioServerHotplug.h
 * @brief This file contains prototypes for OpenSIL MPIO PCIe Server Hotplug
 *
 */

#pragma once

#include <SilCommon.h>

typedef enum {
  HotplugPresenceDetect,                                      ///< Simple Presence Detect
  HotplugExpressModule,                                       ///< PCIe Express Module
  HotplugEnterpriseSsd,                                       ///< Enterprise SSD
  HotplugExpressModuleB,                                      ///< PCIe Express Module B
  MaxHotplugFormat                                            ///< Not valid value, used to verify input
} MPIO_HOTPLUG_TYPE;

/// PCIe Hotplug Mapping Descriptor
typedef struct  {
  uint32_t  HotPlugFormat        :3;  /**< Hotplug Format
                                       *  @li @b 0 - Simple Presence Detect
                                       *  @li @b 1 - PCIe Express Module
                                       *  @li @b 2 - Enterprise SSD
                                       */
  uint32_t  ResetDescriptorValid :1;  /**< Reset Descriptor Valid
                                       *  @li @b 0 - Reset is not supported from the I2C subsystem
                                       *  @li @b 1 - Reset is supported from the I2C subsystem
                                       */
  uint32_t  PortActive           :1;  /**< This bit is essentially a valid marker indicating whether the hot-plug slot is
                                       *   implemented in the system and whether the information in the descriptor register
                                       *   is valid.
                                       *  @li @b 0 - This descriptor should be ignored
                                       *  @li @b 1 - This descriptor is valid
                                       */
  uint32_t  MasterSlaveAPU       :1;  /**< this bit indicates the APU (Master or Slave die) the PCIe device, that is
                                       *   mapped onto the hot-plug slot, resides. Note, the "Master" APU is the APU
                                       *   that hosts the I2C subsystem. The "Slave" APU communicates with the "Master"
                                       *   APU through a die-to-die communication link (DDCL).
                                       *  @li @b 0 - Connected to Master APU
                                       *  @li @b 1 - Connected to Slave APU
                                       */
  uint32_t  DieID                :1;  ///< Indicates the Die number in the system
  uint32_t  PciePortInstance     :4;  ///< Indicates port number on the core
  uint32_t  PcieTileInstance     :4;  /**< Indicates the Aperture ID assigned to the PCIe tile on the SMN network.
                                       *   This is used by the SMU to route the "SMU Status Transfer" to the correct PCIe tile.
                                       *   This can also be seen as the PCIe "core number", that contains PCIe device that is
                                       *   mapped onto the hot-plug slot.
                                       */
  uint32_t  BridgeOffset         :5;  ///< Logical bridge ID
  uint32_t  Reserved0            :4;  ///< Reserved
  uint32_t  AltSlotNum           :6;  /**< When the descriptor is used for an Enterprise SSD form-factor, and the slot
                                       *   supports two PCIe links, this field serves as a pointer to the PCIE_HOTPLUG_MAPPING<slotNum>
                                       *   descriptor associated with the 'other' PCIe link. The primary descriptor shall point to
                                       *   the secondary descriptor, and the secondary descriptor must point to the primary descriptor.
                                       *   For a slot that only supports a single PCIe link, the primary descriptor must set this field to zero.
                                       */
  uint32_t  SecondaryLink        :1;  /**< When the descriptor is used for an Enterprise SSD form-factor, this bit specifies whether the
                                       *   descriptor is for the primary PCIe device, or the secondary PCIe device associated with the slot.
                                       *  @li @b 0 - This is the primary PCIe device
                                       *  @li @b 1 - This is the secondary PCIe device
                                       */
  uint32_t  Reserved1            :1;  ///< Reserved for future use
} MPIO_PCIE_HOTPLUG_MAPPING;

/// PCIe Hotplug Function Descriptor
typedef struct  {
  uint32_t  I2CGpioBitSelector   :3;  /**< I2C GPIO Bit Select : for a simple presense detect usage model, this field indicates which
                                       *   bit of which IO byte is used for the presence detect signal associated with this descriptor.
                                       *   For an Enterprise SSD form-factor, bits 0 and 1 must be 0 and bit 2 selects which nibble of the
                                       *   IO byte is used for the hot plug signals associated with this descriptor.
                                       */
  uint32_t  I2CGpioByteMapping   :3;  /**< I2C GPIO IO Byte Mapping : indicates which IO byte of the I2C GPIO device is used for the
                                       *   hot-plug signals associated with this descriptor.
                                       */
  uint32_t  I2CGpioDeviceAddress :5;  /**< I2C GPIO Device Address : indicates the I2C address for the I2C GPIO device
                                       *   that is used to host the hot-plug signals associated with this descriptor.
                                       */
  uint32_t  I2CDeviceType        :2;  /**< I2C Device Type : indicates which of three I2C GPIO devices is used to host the hot-plug
                                       *   signals associated with this descriptor.
                                       *  @li @b 00 - PCA9539 address and register map; in support of legacy hot-plug implementations
                                       *  @li @b 01 - PCA9535 address and register map; in support of newer hot-plug implementations
                                       *  @li @b 10 - PCA9506 address and register map; in support of high slot-capacity implementations
                                       *  @li @b 11 - reserved
                                       *   Note, PCA9535 and PCA9506 devices may be intermixed on the I2C interface.
                                       *   However, due to common addressing, a combined total of eight devices may be populated.
                                       */
  uint32_t  I2CBusSegment        :5;  /**< I2C Bus Segment :  in the context of an I2C subsystem constructed with PCA9545 I2C switches,
                                       *   this field specifies the downstream I2C bus segment, on one of two PCA9545 switches, on which
                                       *   the target I2C GPIO device is located. However, for I2C topologies that are not constructed
                                       *   with any PCA9545 devices, one value is reserved to identify that the I2C GPIO device does
                                       *   not reside behind any PCA9454 I2C switch.
                                       *  @li @b 00000 - PCA9545, address=00, downstream I2C bus=00 (I2C address = 0x70)
                                       *  @li @b 00001 - PCA9545, address=00, downstream I2C bus=01 (I2C address = 0x70)
                                       *  @li @b 00010 - PCA9545, address=00, downstream I2C bus=10 (I2C address = 0x70)
                                       *  @li @b 00011 - PCA9545, address=00, downstream I2C bus=11 (I2C address = 0x70)
                                       *  @li @b 00100 - PCA9545, address=01, downstream I2C bus=00 (I2C address = 0x71)
                                       *  @li @b 00101 - PCA9545, address=01, downstream I2C bus=01 (I2C address = 0x71)
                                       *  @li @b 00110 - PCA9545, address=01, downstream I2C bus=10 (I2C address = 0x71)
                                       *  @li @b 00111 - no PCA9545; I2C bus directly connected to APU socket
                                       *  @li @b 01000 : PCA9545, address=01, downstream I2C bus=11 (I2C address = 0x71)
                                       *  @li @b 01001 : PCA9545, address=02, downstream I2C bus=00 (I2C address = 0x72)
                                       *  @li @b 01010 : PCA9545, address=02, downstream I2C bus=01 (I2C address = 0x72)
                                       *  @li @b 01011 : PCA9545, address=02, downstream I2C bus=10 (I2C address = 0x72)
                                       *  @li @b 01100 : PCA9545, address=02, downstream I2C bus=11 (I2C address = 0x72)
                                       *  @li @b 01101 : PCA9545, address=03, downstream I2C bus=00 (I2C address = 0x73)
                                       *  @li @b 01110 : PCA9545, address=03, downstream I2C bus=01 (I2C address = 0x73)
                                       *  @li @b 01111 : PCA9545, address=03, downstream I2C bus=10 (I2C address = 0x73)
                                       *  @li @b 10000 : PCA9545, address=03, downstream I2C bus=11 (I2C address = 0x73)
                                       */
  uint32_t  FunctionMask         :8;  /**< Function Mask : when the descriptor is used for a multi-bit hot-plug functional format,
                                       *   this field can be used to opt-out of specific pin functionality. When an 8bit GPIO group
                                       *   is allocated to the hot-plug function, all eight bits of this field are valid. When a
                                       *   4bit GPIO group is allocated to the hot-plug function, only the lower four bits of this
                                       *   field are valid. When valid, if bitN of this field is set, the sub-element of the hot-plug
                                       *   function that is mapped to the Nth IO bit of the byte/nibble is disabled. Once disabled
                                       *   via this field, the associated pin is made available for other potential purposes such
                                       *   as a RESET or GPIO capability.
                                       */
  uint32_t  I2CBusSegment2       :6;  /**< I2C Bus Segment2 : this defines an upstream PCA9545 or 9546 I2C switch that resides on
                                       *   on the I2C bus before the switch defined in I2CBusSegment.  This switch is not defined
                                       *   in the HOTPLUG_DESCRIPTOR entries, however, in order to maintain backwards compatibility
                                       *   with older programs.  This switch is defined by using the port parameters:
                                       *   PP_LEGACY_SWITCH0_ADDR and PP_LEGACY_SWITCH0_SELECT in the CPM topology tables.
                                       */
} MPIO_PCIE_HOTPLUG_FUNCTION;

/// PCIe Hotplug Reset Descriptor
typedef struct  {
  uint32_t  Reserved0            :3;  ///< Reserved for future use
  uint32_t  I2CGpioByteSelect    :3;  /**< I2C GPIO IO Byte Mapping : indicates which IO byte of the I2C GPIO device is used for the
                                       *   hot-plug signals associated with this descriptor.
                                       */
  uint32_t  I2CGpioDeviceAddress :5;  /**< I2C GPIO Device Mapping : indicates the I2C address for the I2C GPIO device
                                       *   that is used to host the hot-plug signals associated with this descriptor.
                                       */
  uint32_t  I2CDeviceType        :2;  /**< I2C Device Type : indicates which of three I2C GPIO devices is used to host the hot-plug
                                       *   signals associated with this descriptor.
                                       *  @li @b 00 - PCA9539 address and register map; in support of legacy hot-plug implementations
                                       *  @li @b 01 - PCA9535 address and register map; in support of newer hot-plug implementations
                                       *  @li @b 10 - PCA9506 address and register map; in support of high slot-capacity implementations
                                       *  @li @b 11 - reserved
                                       *   Note, PCA9535 and PCA9506 devices may be intermixed on the I2C interface.
                                       *   However, due to common addressing, a combined total of eight devices may be populated.
                                       */
  uint32_t  I2CBusSegment        :5;  /**< I2C Bus Segment :  in the context of an I2C subsystem constructed with PCA9545 I2C switches,
                                       *   this field specifies the downstream I2C bus segment, on one of two PCA9545 switches, on which
                                       *   the target I2C GPIO device is located. However, for I2C topologies that are not constructed
                                       *   with any PCA9545 devices, one value is reserved to identify that the I2C GPIO device does
                                       *   not reside behind any PCA9454 I2C switch.
                                       *  @li @b 000 - PCA9545, address=00, downstream I2C bus=00
                                       *  @li @b 001 - PCA9545, address=00, downstream I2C bus=01
                                       *  @li @b 010 - PCA9545, address=00, downstream I2C bus=10
                                       *  @li @b 011 - PCA9545, address=00, downstream I2C bus=11
                                       *  @li @b 100 - PCA9545, address=01, downstream I2C bus=00
                                       *  @li @b 101 - PCA9545, address=01, downstream I2C bus=01
                                       *  @li @b 110 - PCA9545, address=01, downstream I2C bus=10
                                       *  @li @b 111 - no PCA9545; I2C bus directly connected to APU socket
                                       */
  uint32_t  ResetSelect          :8;  /**< Reset Select : this bit-field is used to specify which pin within the selected I2C IO byte
                                       *   (identified by the other fields in this descriptor) is used for the reset function associated
                                       *   with the <slotNum> PCIe link. Since reset is a singular function, only one bit in this field
                                       *   may be set to one. When the Nth bit of this field is set to 1, the Nth bit of the selected I2C
                                       *   IO byte serves as the platform reset signal for the associated PCIe link.
                                       *   Note, although this descriptor identifies a single bit, this field has been defined in this
                                       *   manner so as to be consistent with both the "FunctionMask" field in the FUNCTION descriptor
                                       *   and the "GPIOSelect" field in the GPIO descriptor.
                                       */
  uint32_t  Reserved1            :6;  ///< Reserved for future use
} MPIO_PCIE_HOTPLUG_RESET;


/// Engine Configuration
typedef struct {
  uint8_t  StartLane;                /**< Start Lane ID (in reversed configuration StartLane > EndLane)
                                      * This value should correspond exactly to the definition the PCIe topology
                                      */
  uint8_t  EndLane;                  /**< End lane ID (in reversed configuration StartLane > EndLane)
                                      * This value should correspond exactly to the definition the PCIe topology
                                      */
  uint8_t  SocketNumber;             ///< Socket Number of this port
  uint8_t  SlotNumber;               ///< Unique identifier for the physical slot number of this port
} MPIO_HOTPLUG_ENGINE_DATA;


typedef struct {
  uint32_t                      Flags;        ///< The hight order bit indicates that this is the last item in the array
  MPIO_HOTPLUG_ENGINE_DATA      Engine;       ///< This structure identifies a specific slot in the platform
  MPIO_PCIE_HOTPLUG_MAPPING     Mapping;      ///< This structure describes logical information about the slot and its connection to the platform
  MPIO_PCIE_HOTPLUG_FUNCTION    Function;     ///< This structure describes the GPIO extender connection to the server hotplug slot.
  MPIO_PCIE_HOTPLUG_RESET       Reset;        /**< This structure describes for the link specifies the exact pin within the hotplug
                                               * I2C subsystem onto which the reset functionality is mapped.
                                               */
} MPIO_HOTPLUG_DESCRIPTOR;

/**
 *  @brief Macro to associate platform's physical PCIe device/slot.
 *  @param mStartLane      Start lane of device
 *  @param mEndLane        Ending lane of device
 *  @param mSocketNumber   Socket number to which slot is connected
 *  @param mSlotNumber     Slot number to identify the location of Physical slot. Valid values 1 to 159
 */
#define  MPIO_HOTPLUG_ENGINE_DATA_INITIALIZER(mStartLane, mEndLane, mSocketNumber, mSlotNumber) {mStartLane, mEndLane, mSocketNumber, mSlotNumber}
/**
 *  @brief Macro to map the slot to a PCIe port
 *  @details Each MAPPING descriptor contains information that identifies how a given hot-plug slot maps onto the APU device capabilities.
             Depending on whether the port is active, the <slotNum> MAPPING descriptor specifies a mapping to a particular PCIe function/port in the socket. Additionally, the
             MAPPING descriptor also identifies the level of hot-plug capability, as well as the applicability of the RESET descriptor. The MAPPING descriptor is always
             valid for an active hot-plug slot.
 *  @param mHotPlugFormat         specifies which hot-plug form-factor the descriptor pertains to
 *  @param mGpioDescriptorValid   Indicates whether the GPIO descriptor is populated
 *  @param mResetDescriptorValid  Indicates if link reset is supported from I2C
 *  @param mPortActive            Indicates if hotplug slot is implemented
 *  @param mMasterSlaveAPU        Is slot connected to Master or Slave APU
 *  @param mDieNumber             Die Number
 *  @param mAlternateSlotNumber   Alternate Slot number
 *  @param mPrimarySecondaryLink  Primary or Secondary link
 */
#define  MPIO_PCIE_HOTPLUG_INITIALIZER_MAPPING(mHotPlugFormat, mDeprecated, mResetDescriptorValid, mPortActive, mMasterSlaveAPU, mDieNumber, \
                                               mAlternateSlotNumber, mPrimarySecondaryLink) \
                                              {mHotPlugFormat, mResetDescriptorValid, mPortActive, mMasterSlaveAPU, mDieNumber, \
                                               0, 0, 0, 0, mAlternateSlotNumber, mPrimarySecondaryLink, 0}
/**
 *  @brief This macro used to define I2c connections to a HotPlug slot.
 *  @details Associated with each MAPPING descriptor, a FUNCTION descriptor is provided that indicates where the associated hot-plug functionality is mapped
 within the I2C subsystem. Through the FUNCTION descriptor, the exact pin (or group of pins) on a specific IO byte of a specific I2C GPIO device.
 In addition, where a group of pins is allocated to the hot-plug functionality (based on the "HotPlugFormat" field of the corresponding MAPPING descriptor),
 a bitwise mask is provided to opt-out of specific functional elements of the specified hot-plug behaviour, freeing the associated pin for other potential uses.
 *  @param mI2CGpioBitSelector    I2C GPIO Bit Select : for a simple presense detect usage model, this field indicates which
                                                            bit of which IO byte is used for the presence detect signal associated with this descriptor.
                                                            For an Enterprise SSD form-factor, bits 0 and 1 must be 0 and bit 2 selects which nibble of the
                                                            IO byte is used for the hot plug signals associated with this descriptor.
 *  @param mI2CGpioByteMapping    I2C GPIO IO Byte Mapping : I2C GPIO IO Byte Mapping : indicates which IO byte of the I2C GPIO device is used for the
                                                                 hot-plug signals associated with this descriptor.
 *  @param mI2CGpioDeviceMapping  I2C GPIO Device Mapping indicates the lower address bits used to complete the I2C 7-bit address
 *  @param mI2CDeviceType         I2C Device Type : indicates which of three I2C GPIO devices is used to host the hot-plug
                                                    signals associated with this descriptor.
                                                    @li @b 00 - PCA9539 address and register map; in support of legacy hot-plug implementations
                                                    @li @b 01 - PCA9535 address and register map; in support of newer hot-plug implementations
                                                    @li @b 10 - PCA9506 address and register map; in support of high slot-capacity implementations
                                                    @li @b 11 - reserved
                                                    Note, PCA9535 and PCA9506 devices may be intermixed on the I2C interface.
                                                    However, due to common addressing, a combined total of eight devices may be populated.
 *  @param mI2CBusSegment         I2C Bus Segment :  in the context of an I2C subsystem constructed with PCA9545 I2C switches,
                                                     this field specifies the downstream I2C bus segment, on one of two PCA9545 switches, on which
                                                     the target I2C GPIO device is located. However, for I2C topologies that are not constructed
                                                     with any PCA9545 devices, one value is reserved to identify that the I2C GPIO device does
                                                     not reside behind any PCA9454 I2C switch.
                                                     @li @b 00000 - PCA9545, address=00, downstream I2C bus=00 (I2C address = 0x70)
                                                     @li @b 00001 - PCA9545, address=00, downstream I2C bus=01 (I2C address = 0x70)
                                                     @li @b 00010 - PCA9545, address=00, downstream I2C bus=10 (I2C address = 0x70)
                                                     @li @b 00011 - PCA9545, address=00, downstream I2C bus=11 (I2C address = 0x70)
                                                     @li @b 00100 - PCA9545, address=01, downstream I2C bus=00 (I2C address = 0x71)
                                                     @li @b 00101 - PCA9545, address=01, downstream I2C bus=01 (I2C address = 0x71)
                                                     @li @b 00110 - PCA9545, address=01, downstream I2C bus=10 (I2C address = 0x71)
                                                     @li @b 00111 - no PCA9545; I2C bus directly connected to APU socket
                                                     @li @b 01000 : PCA9545, address=01, downstream I2C bus=11 (I2C address = 0x71)
                                                     @li @b 01001 : PCA9545, address=02, downstream I2C bus=00 (I2C address = 0x72)
                                                     @li @b 01010 : PCA9545, address=02, downstream I2C bus=01 (I2C address = 0x72)
                                                     @li @b 01011 : PCA9545, address=02, downstream I2C bus=10 (I2C address = 0x72)
                                                     @li @b 01100 : PCA9545, address=02, downstream I2C bus=11 (I2C address = 0x72)
                                                     @li @b 01101 : PCA9545, address=03, downstream I2C bus=00 (I2C address = 0x73)
                                                     @li @b 01110 : PCA9545, address=03, downstream I2C bus=01 (I2C address = 0x73)
                                                     @li @b 01111 : PCA9545, address=03, downstream I2C bus=10 (I2C address = 0x73)
                                                     @li @b 10000 : PCA9545, address=03, downstream I2C bus=11 (I2C address = 0x73)
 *  @param mFunctionMask          Function Mask : when the descriptor is used for a multi-bit hot-plug functional format,
                                                  this field can be used to opt-out of specific pin functionality. When an 8bit GPIO group
                                                  is allocated to the hot-plug function, all eight bits of this field are valid. When a
                                                  4bit GPIO group is allocated to the hot-plug function, only the lower four bits of this
                                                  field are valid. When valid, if bitN of this field is set, the sub-element of the hot-plug
                                                  function that is mapped to the Nth IO bit of the byte/nibble is disabled. Once disabled
                                                  via this field, the associated pin is made available for other potential purposes such
                                                  as a RESET or GPIO capability.
 */
#define  MPIO_PCIE_HOTPLUG_INITIALIZER_FUNCTION(mI2CGpioBitSelector, mI2CGpioByteMapping, mI2CGpioDeviceMapping, mI2CDeviceType, mI2CBusSegment, mFunctionMask) \
                                               {mI2CGpioBitSelector, mI2CGpioByteMapping, mI2CGpioDeviceMapping, mI2CDeviceType, mI2CBusSegment & 0x1F, mFunctionMask, 0}
/**
 *  @brief This macro used to connect a pin used for reset to the HotPlug slot.
 *  @details When PCIE_HOTPLUG_MAPPING<slotNum>.ResetDescriptorVld is set to 0, reset  functionality is not tied to the hot-plug slot and must be furnished in
 some other manner. However, when the ResetDescriptorVld bit-field is set to 1, the RESET descriptor for the <slotNum> link specifies the exact pin
 within the hot-plug I2C subsystem onto which the reset functionality is mapped, enabling control of the reset functionality through a BIOS
 accessible API.
 *  @param mI2CGpioByteMapping    I2C GPIO IO Byte Mapping : I2C GPIO IO Byte Mapping : indicates which IO byte of the I2C GPIO device is used for the
                                                                 hot-plug signals associated with this descriptor.
 *  @param mI2CGpioDeviceMapping  I2C GPIO Device Mapping indicates the lower address bits used to complete the I2C 7-bit address
 *  @param mI2CDeviceType         I2C Device Type : indicates which of three I2C GPIO devices is used to host the hot-plug
                                                    signals associated with this descriptor.
                                                    @li @b 00 - PCA9539 address and register map; in support of legacy hot-plug implementations
                                                    @li @b 01 - PCA9535 address and register map; in support of newer hot-plug implementations
                                                    @li @b 10 - PCA9506 address and register map; in support of high slot-capacity implementations
                                                    @li @b 11 - reserved
                                                    Note, PCA9535 and PCA9506 devices may be intermixed on the I2C interface.
                                                    However, due to common addressing, a combined total of eight devices may be populated.
 *  @param mI2CBusSegment         I2C Bus Segment :  in the context of an I2C subsystem constructed with PCA9545 I2C switches,
                                                         this field specifies the downstream I2C bus segment, on one of two PCA9545 switches, on which
                                                         the target I2C GPIO device is located. However, for I2C topologies that are not constructed
                                                         with any PCA9545 devices, one value is reserved to identify that the I2C GPIO device does
                                                         not reside behind any PCA9454 I2C switch.
                                                         @li @b 00000 - PCA9545, address=00, downstream I2C bus=00 (I2C address = 0x70)
                                                         @li @b 00001 - PCA9545, address=00, downstream I2C bus=01 (I2C address = 0x70)
                                                         @li @b 00010 - PCA9545, address=00, downstream I2C bus=10 (I2C address = 0x70)
                                                         @li @b 00011 - PCA9545, address=00, downstream I2C bus=11 (I2C address = 0x70)
                                                         @li @b 00100 - PCA9545, address=01, downstream I2C bus=00 (I2C address = 0x71)
                                                         @li @b 00101 - PCA9545, address=01, downstream I2C bus=01 (I2C address = 0x71)
                                                         @li @b 00110 - PCA9545, address=01, downstream I2C bus=10 (I2C address = 0x71)
                                                         @li @b 00111 - no PCA9545; I2C bus directly connected to APU socket
                                                         @li @b 01000 : PCA9545, address=01, downstream I2C bus=11 (I2C address = 0x71)
                                                         @li @b 01001 : PCA9545, address=02, downstream I2C bus=00 (I2C address = 0x72)
                                                         @li @b 01010 : PCA9545, address=02, downstream I2C bus=01 (I2C address = 0x72)
                                                         @li @b 01011 : PCA9545, address=02, downstream I2C bus=10 (I2C address = 0x72)
                                                         @li @b 01100 : PCA9545, address=02, downstream I2C bus=11 (I2C address = 0x72)
                                                         @li @b 01101 : PCA9545, address=03, downstream I2C bus=00 (I2C address = 0x73)
                                                         @li @b 01110 : PCA9545, address=03, downstream I2C bus=01 (I2C address = 0x73)
                                                         @li @b 01111 : PCA9545, address=03, downstream I2C bus=10 (I2C address = 0x73)
                                                         @li @b 10000 : PCA9545, address=03, downstream I2C bus=11 (I2C address = 0x73)
 *  @param mResetSelect           This bit-field is used to specify which pin is used for reset function.
 */
#define  MPIO_PCIE_HOTPLUG_INITIALIZER_RESET(mI2CGpioByteMapping, mI2CGpioDeviceMapping, mI2CDeviceType, mI2CBusSegment, mResetSelect) \
                                            {0, mI2CGpioByteMapping, mI2CGpioDeviceMapping, mI2CDeviceType, mI2CBusSegment, mResetSelect, 0}
/**
 *  @brief This macro defines a NULL descriptor in cases where the reset descriptor is not valid
 */
#define  MPIO_PCIE_HOTPLUG_INITIALIZER_NO_RESET() {0, 0, 0, 0, 0, 0, 0}

// bitmasks for GPIO pins based on hotplug format

/**
 *  @brief bitmasks for GPIO pin PCIE_HOTPLUG_EXPRESSMODULE_PRESENT
 */
#define PCIE_HOTPLUG_EXPRESSMODULE_PRESENT_    0x01
/**
 *  @brief bitmasks for GPIO pin PCIE_HOTPLUG_EXPRESSMODULE_PWRFLT
 */
#define PCIE_HOTPLUG_EXPRESSMODULE_PWRFLT_     0x02
/**
 *  @brief bitmasks for GPIO pin PCIE_HOTPLUG_EXPRESSMODULE_PWRFLT
 */
#define PCIE_HOTPLUG_EXPRESSMODULE_ATNSW_      0x04
/**
 *  @brief bitmasks for GPIO pin PCIE_HOTPLUG_EXPRESSMODULE_EMILS
 */
#define PCIE_HOTPLUG_EXPRESSMODULE_EMILS       0x08
/**
 *  @brief bitmasks for GPIO pin PCIE_HOTPLUG_EXPRESSMODULE_PWREN_
 */
#define PCIE_HOTPLUG_EXPRESSMODULE_PWREN_      0x10
/**
 *  @brief bitmasks for GPIO pin PCIE_HOTPLUG_EXPRESSMODULE_ATNLED
 */
#define PCIE_HOTPLUG_EXPRESSMODULE_ATNLED      0x20
/**
 *  @brief bitmasks for GPIO pin PCIE_HOTPLUG_EXPRESSMODULE_PWRLED
 */
#define PCIE_HOTPLUG_EXPRESSMODULE_PWRLED      0x40
/**
 *  @brief bitmasks for GPIO pin PCIE_HOTPLUG_EXPRESSMODULE_EMIL
 */
#define PCIE_HOTPLUG_EXPRESSMODULE_EMIL        0x80

/**
 *  @brief Mode B bitmasks for GPIO pin PCIE_HOTPLUG_EXPRESSMODULE_B_ATNLED
 */
#define PCIE_HOTPLUG_EXPRESSMODULE_B_ATNLED    0x01
/**
 *  @brief Mode B bitmasks for GPIO pin PCIE_HOTPLUG_EXPRESSMODULE_B_PWRLED
 */
#define PCIE_HOTPLUG_EXPRESSMODULE_B_PWRLED    0x02
/**
 *  @brief Mode B bitmasks for GPIO pin PCIE_HOTPLUG_EXPRESSMODULE_B_PWREN_
 */
#define PCIE_HOTPLUG_EXPRESSMODULE_B_PWREN_    0x04
/**
 *  @brief Mode B bitmasks for GPIO pin PCIE_HOTPLUG_EXPRESSMODULE_B_ATNSW_
 */
#define PCIE_HOTPLUG_EXPRESSMODULE_B_ATNSW_    0x08
/**
 *  @brief Mode B bitmasks for GPIO pin PCIE_HOTPLUG_EXPRESSMODULE_B_PRESENT_
 */
#define PCIE_HOTPLUG_EXPRESSMODULE_B_PRESENT_  0x10
/**
 *  @brief Mode B bitmasks for GPIO pin PCIE_HOTPLUG_EXPRESSMODULE_B_PWRFLT_
 */
#define PCIE_HOTPLUG_EXPRESSMODULE_B_PWRFLT_   0x20
/**
 *  @brief Mode B bitmasks for GPIO pin PCIE_HOTPLUG_EXPRESSMODULE_B_EMILS
 */
#define PCIE_HOTPLUG_EXPRESSMODULE_B_EMILS     0x40
/**
 *  @brief Mode B bitmasks for GPIO pin PCIE_HOTPLUG_EXPRESSMODULE_B_EMIL
 */
#define PCIE_HOTPLUG_EXPRESSMODULE_B_EMIL      0x80

/**
 *  @brief bitmasks for GPIO pin PCIE_HOTPLUG_EXPRESS_SSD_PRESENT
 */
#define PCIE_HOTPLUG_EXPRESS_SSD_PRESENT_      0x10
/**
 *  @brief bitmasks for GPIO pin PCIE_HOTPLUG_EXPRESS_SSD_IFDET
 */
#define PCIE_HOTPLUG_EXPRESS_SSD_IFDET_        0x20
/**
 *  @brief bitmasks for GPIO pin PCIE_HOTPLUG_EXPRESS_SSD_DUALEN
 */
#define PCIE_HOTPLUG_EXPRESS_SSD_DUALEN_       0x40
