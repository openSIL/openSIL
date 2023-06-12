/**
 * @file  FchPT.h
 * @brief FCH PT Data definition
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#pragma pack (push, 1)

//Promontory param structure
/// PT_USB structure
typedef struct {
  uint8_t         PTXhciGen1;                     ///< PTXhciGen1
  uint8_t         PTXhciGen2;                     ///< PTXhciGen2
  uint8_t         PTAOAC;                         ///< PTAOAC
  uint8_t         PTHW_LPM ;                      ///< PTHW_LPM
  uint8_t         PTDbC;                          ///< PTDbC
  uint8_t         PTXHC_PME;                      ///< PTXHC_PME
  uint8_t         Equalization4;                  ///< Enable/Disable Equalization 4 for PT only
  uint8_t         Redriver;                       ///< Enable/Disable Redriver Setting for PT only
  uint8_t         eg_iosc_cnt;                    ///< Enable/Disable eg_iosc_cnt Setting for LP-PT only
  uint8_t         XhciLockEnable;                 ///< Enable/Disable PT lock
} PT_USB;
/// PT_SATA structure
typedef struct {
  uint8_t         PTSataMode;                     ///< PTSataMode
  uint8_t         PTSataAggresiveDevSlpP0;        ///< PTSataAggresiveDevSlpP0
  uint8_t         PTSataAggresiveDevSlpP1;        ///< PTSataAggresiveDevSlpP1
  uint8_t         PTSataAggrLinkPmCap;            ///< PTSataAggrLinkPmCap
  uint8_t         PTSataPscCap;                   ///< PTSataPscCap
  uint8_t         PTSataSscCap;                   ///< PTSataSscCap
  uint8_t         PTSataMsiCapability;            ///< PTSataPscCap
  uint8_t         PTSataPortMdPort0;              ///< PTSataPortMdPort0
  uint8_t         PTSataPortMdPort1;              ///< PTSataPortMdPort1
  uint8_t         PTSataHotPlug;                  ///< PTSataHotPlug
} PT_SATA;

/// PT_SATAPort structure
typedef struct {
  uint8_t         PTSataPort0Enable;             ///<PTSataPort0 Enable/Disable
  uint8_t         PTSataPort1Enable;             ///<PTSataPort1 Enable/Disable
  uint8_t         PTSataPort2Enable;             ///<PTSataPort2 Enable/Disable
  uint8_t         PTSataPort3Enable;             ///<PTSataPort3 Enable/Disable
  uint8_t         PTSataPort4Enable;             ///<PTSataPort4 Enable/Disable
  uint8_t         PTSataPort5Enable;             ///<PTSataPort5 Enable/Disable
  uint8_t         PTSataPort6Enable;             ///<PTSataPort6 Enable/Disable
  uint8_t         PTSataPort7Enable;             ///<PTSataPort7 Enable/Disable
} PT_SATAPort;


/// PT_PCIE structure
typedef struct {
  uint8_t         PromontoryPCIeEnable;                   ///< PCIeEnable
  uint8_t         PromontoryPCIeASPM;                     ///< PCIeASPM
} PT_PCIE;
/// PT_DIDVID structure
typedef struct {
  uint8_t         GppNumber;                             ///< GppNumber
  uint8_t         PXEFound;                              ///< PXE Bridge Found
  uint8_t         PXEDevNumber;                          ///< PXE Device Number
  uint8_t         XhciLock;                              ///< XHCI lock detect
  uint32_t        XhciID;                                ///< XhciDIDVID
  uint32_t        SataID;                                ///< SataDIDVID
  uint32_t        GpioID;                                ///< GpioDIDVID
  uint64_t        FwVersion;                             ///< FwVersion
} PT_ADDR;

/// PT_USBPort structure
typedef struct {
  uint8_t         PTUsb31P0;                    ///< PTUsb31Port0 Enable/Disable for PROM3/4
  uint8_t         PTUsb31P1;                    ///< PTUsb31Port0 Enable/Disable for PROM3/4
  uint8_t         PTUsb30P0;                    ///< PTUsb30Port0 Enable/Disable for PROM3/4
  uint8_t         PTUsb30P1;                    ///< PTUsb30Port1 Enable/Disable for PROM3/4
  uint8_t         PTUsb30P2;                    ///< PTUsb30Port2 Enable/Disable for PROM3/4
  uint8_t         PTUsb30P3;                    ///< PTUsb30Port3 Enable/Disable for PROM3/4
  uint8_t         PTUsb30P4;                    ///< PTUsb30Port4 Enable/Disable for PROM3/4
  uint8_t         PTUsb30P5;                    ///< PTUsb30Port5 Enable/Disable for PROM3/4
  uint8_t         PTUsb20P0;                    ///< PTUsb20Port0 Enable/Disable for PROM3/4
  uint8_t         PTUsb20P1;                    ///< PTUsb20Port1 Enable/Disable for PROM3/4
  uint8_t         PTUsb20P2;                    ///< PTUsb20Port2 Enable/Disable for PROM3/4
  uint8_t         PTUsb20P3;                    ///< PTUsb20Port3 Enable/Disable for PROM3/4
  uint8_t         PTUsb20P4;                    ///< PTUsb20Port4 Enable/Disable for PROM3/4
  uint8_t         PTUsb20P5;                    ///< PTUsb20Port5 Enable/Disable for PROM3/4
} PT_USBPort;

/// PT_USBPort structure for PRO460
typedef struct {
  bool       PTUsb31P0;                    ///< PTUsb31Port0 Enable/Disable for PRO460
  bool       PTUsb31P1;                    ///< PTUsb31Port1 Enable/Disable for PRO460
  bool       PTUsb30P0;                    ///< PTUsb30Port0 Enable/Disable for PRO460
  bool       PTUsb30P1;                    ///< PTUsb30Port1 Enable/Disable for PRO460
  bool       PTUsb30P2;                    ///< PTUsb30Port2 Enable/Disable for PRO460
  bool       PTUsb30P3;                    ///< PTUsb30Port3 Enable/Disable for PRO460
  bool       PTUsb20P0;                    ///< PTUsb20Port0 Enable/Disable for PRO460
  bool       PTUsb20P1;                    ///< PTUsb20Port1 Enable/Disable for PRO460
  bool       PTUsb20P2;                    ///< PTUsb20Port2 Enable/Disable for PRO460
  bool       PTUsb20P3;                    ///< PTUsb20Port3 Enable/Disable for PRO460
  bool       PTUsb20P4;                    ///< PTUsb20Port4 Enable/Disable for PRO460
  bool       PTUsb20P5;                    ///< PTUsb20Port5 Enable/Disable for PRO460
} PT_USBPortP460;

/// PT_USBPort structure for PROM2
typedef struct {
  uint8_t         PTUsb31P0;                    ///< PTUsb31Port0 Enable/Disable for PROM2
  uint8_t         PTUsb31P1;                    ///< PTUsb31Port0 Enable/Disable for PROM2
  uint8_t         PTUsb30P0;                    ///< PTUsb30Port0 Enable/Disable for PROM2
  uint8_t         PTUsb30P1;                    ///< PTUsb30Port1 Enable/Disable for PROM2
  uint8_t         PTUsb20P0;                    ///< PTUsb20Port0 Enable/Disable for PROM2
  uint8_t         PTUsb20P1;                    ///< PTUsb20Port1 Enable/Disable for PROM2
  uint8_t         PTUsb20P2;                    ///< PTUsb20Port2 Enable/Disable for PROM2
  uint8_t         PTUsb20P3;                    ///< PTUsb20Port3 Enable/Disable for PROM2
  uint8_t         PTUsb20P4;                    ///< PTUsb20Port4 Enable/Disable for PROM2
  uint8_t         PTUsb20P5;                    ///< PTUsb20Port5 Enable/Disable for PROM2
} PT_USBPortPROM2;


/// PT_USBPort structure for PROM1
typedef struct {
  uint8_t         PTUsb31P0;                    ///< PTUsb31Port0 Enable/Disable for PROM1
  uint8_t         PTUsb31P1;                    ///< PTUsb31Port0 Enable/Disable for PROM1
  uint8_t         PTUsb30P0;                    ///< PTUsb30Port0 Enable/Disable for PROM1
  uint8_t         PTUsb20P0;                    ///< PTUsb20Port0 Enable/Disable for PROM1
  uint8_t         PTUsb20P1;                    ///< PTUsb20Port1 Enable/Disable for PROM1
  uint8_t         PTUsb20P2;                    ///< PTUsb20Port2 Enable/Disable for PROM1
  uint8_t         PTUsb20P3;                    ///< PTUsb20Port3 Enable/Disable for PROM1
  uint8_t         PTUsb20P4;                    ///< PTUsb20Port4 Enable/Disable for PROM1
  uint8_t         PTUsb20P5;                    ///< PTUsb20Port5 Enable/Disable for PROM1
} PT_USBPortPROM1;


///PTUSB31TxStructure
typedef struct {
  uint8_t         USB31Gen1Swing;           ///< PT USB31PCS_B1 genI swing
  uint8_t         USB31Gen2Swing;           ///< PT USB31PCS_B1 genII swing
  uint8_t         USB31Gen1PreEmEn;         ///< PT USB31PCS_B1 genI pre-emphasis enable
  uint8_t         USB31Gen2PreEmEn;         ///< PT USB31PCS_B1 genII pre-emphasis enable
  uint8_t         USB31Gen1PreEmLe;         ///< PT USB31PCS_B1 genI pre-emphasis level
  uint8_t         USB31Gen2PreEmLe;         ///< PT USB31PCS_B1 genII pre-emphasis level
  uint8_t         USB31Gen1PreShEn;         ///< PT USB31PCS_B1 genI pre-shoot enable
  uint8_t         USB31Gen2PreShEn;         ///< PT USB31PCS_B1 genII pre-shoot enable
  uint8_t         USB31Gen1PreShLe;         ///< PT USB31PCS_B1 genI pre-shoot level
  uint8_t         USB31Gen2PreShLe;         ///< PT USB31PCS_B1 genII pre-shoot level
} PT_USB31Tx;

///PTUSB30TxStructure
typedef struct {
  uint8_t         USB30Gen1Swing;           ///< PTUSB30PCS_B3 genI swing
  uint8_t         USB30Gen1PreEmEn;         ///< PTUSB30PCS_B3 genI pre-emphasis enable
  uint8_t         USB30Gen1PreEmLe;         ///< PTUSB30PCS_B3 genI pre-emphasis level
} PT_USB30Tx;

///LPPTUSB20TxStructure
typedef struct {
  uint8_t         SlewRateDelay;            ///< LPPT SlewRateDelay
  uint8_t         DrivingCurrent;           ///< LPPT DrivingCurrent
} LPPT_USB20Tx;


///PTUSBTxStructure
typedef struct {
  PT_USB31Tx    USB31Tx[2];               ///< PT /LP-PT USB31Tx setting
  PT_USB30Tx    USB30Tx[6];               ///< PT /LP-PT USB30Tx setting
  uint8_t         USB20B2Tx00;              ///< PT USB2.0 TX driving current, 7: largest By USB_HSDP/N[0]
  uint8_t         USB20B2Tx05;              ///< PT USB2.0 TX driving current, 7: largest By USB_HSDP/N[5]
  uint8_t         USB20B3Tx1113;            ///< PT /LP-PT USB2.0 TX driving current, 7: largest By USB_HSDP/N[13][11]
  uint8_t         USB20B3Tx1012;            ///< PT /LP-PT USB2.0 TX driving current, 7: largest By USB_HSDP/N[12][10]
  uint8_t         USB20B4Tx0206;            ///< PT /LP-PT USB2.0 TX driving current, 7: largest By USB_HSDP/N[2][6]
  uint8_t         USB20B4Tx0307;            ///< PT /LP-PT USB2.0 TX driving current, 7: largest By USB_HSDP/N[3][7]
  uint8_t         USB20B5Tx0408;            ///< PT /LP-PT USB2.0 TX driving current, 7: largest By USB_HSDP/N[4][8]
  uint8_t         USB20B5Tx0109;            ///< PT /LP-PT USB2.0 TX driving current, 7: largest By USB_HSDP/N[1][9]
} PT_USBTx;

///LPPTUSBTxStructure
typedef struct {
  PT_USB31Tx    USB31Tx[2];               ///< PT /LP-PT USB31Tx setting
  PT_USB30Tx    USB30Tx[6];               ///< PT /LP-PT USB30Tx setting
  LPPT_USB20Tx  USB20Tx[8];         ///< LP-PT USB20 Tx setting
} LPPT_USBTx;

///PTSataTxStructure
typedef struct {
  uint8_t        SATAGen1Swing;             ///< genI swing
  uint8_t        SATAGen2Swing;             ///< genII swing
  uint8_t        SATAGen3Swing;             ///< genIII swing
  uint8_t        SATAGen1PreEmEn;           ///< genI pre-emphasis enable
  uint8_t        SATAGen2PreEmEn;           ///< genII pre-emphasis enable
  uint8_t        SATAGen3PreEmEn;           ///< genIII pre-emphasis enable
  uint8_t        SATAGen1PreEmLevel;        ///< genI pre-emphasis level
  uint8_t        SATAGen2PreEmLevel;        ///< genII pre-emphasis level
  uint8_t        SATAGen3PreEmLevel;        ///< genIII pre-emphasis level
} PT_SATATx;

///PT Svid Ssid Structure
typedef struct {
  uint16_t        SVID;             ///< Svid
  uint16_t        SSID;             ///< Ssid
} PT_ID;

///Promontory param structure
typedef struct _FCH_PT {
  PT_USB          PromontoryUSB;             ///<PTUSBStructure
  PT_SATA         PromontorySATA;            ///<PTSATAStructure
  PT_SATAPort     PromontorySATAPort;        ///<PTSATAPortStructure
  PT_PCIE         PromontoryPCIE;            ///<PTPCIEStructure
  PT_ADDR         PromontoryAddr;            ///<PTIDStructure
  PT_USBPort      PromontoryUSBPort;         ///<PTUSBPortStructure
  PT_USBPortP460  PTUSBPortPRO460;           ///<PTUSBPortStructure for PRO460
  PT_USBPortPROM2 PTUSBPortPROM2;            ///<PTUSBPortStructure for PROM2
  PT_USBPortPROM1 PTUSBPortPROM1;            ///<PTUSBPortStructure for PROM1
  PT_USBTx        PTUSBTX;                   ///<PTUSBTX
  LPPT_USBTx      LPPTUSBTX;                 ///<LP-PTUSBTX
  PT_SATATx       PTSATATX[8];               ///<PTSATATX
  uint8_t         Deemphasis[4];             ///<Fine Tune Up stream port lane de-emphasis
  bool            PCIEPorts[8];              ///<Enable/Disable PCIE Switch Downstream Ports
  bool            LPPTGPPClkForceOn[8];      ///<Enable/Disable LP PT GPP Clock Force On
  PT_ID           PTID[8];                   ///<PTID
} FCH_PT;

//-------------------------------------------- Promontory param structure
#pragma pack (pop)
