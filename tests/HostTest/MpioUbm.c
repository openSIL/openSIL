/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2026 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file MpioUbm.c
 * @brief Test packed-topology expansion with a mocked mailbox boundary.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Mpio/Common/MpioLib.h>

/* Provider headers may define NDEBUG; fixture checks must always execute. */
#define CHECK(Condition) do { if (!(Condition)) { \
                                fprintf(stderr, "UBM check failed at %d: %s\n", __LINE__, #Condition); \
                                abort (); \
                              } } while (0)

static struct {
  uint64_t Before;
  MPIOCLASS_INPUT_BLK Data;
  uint64_t After;
} Fixture;
#define Input Fixture.Data
#define Ports Input.PcieTopologyData.PortList
#define Complex Input.PcieTopologyData.PlatformData
static DFC_DESCRIPTOR Replies[16];
static unsigned Enumeration, Gets, EnumerationReply, GetReply[16], Checks;
static unsigned StatusRequests, StatusReply;
static unsigned ExpectedHfcs, FailHfc;


uint32_t MpioServiceRequestCommon (PCI_ADDR Address, uint32_t Request, uint32_t *Args, uint32_t Flags)
{
  CHECK(Address.AddressValue == 0 && Flags == 0);
  if (Request == MPIO_GET_STATUS) {
    ++StatusRequests;
    CHECK(Enumeration == 0 && Gets == 0);
    for (unsigned I = 0; I < 6; I++) { CHECK(Args[I] == 0);
    }
    return StatusReply;
  }
  if (Request == BIOS_MPIO_MSG_I2C_ENUMERATE) {
    ++Enumeration;
    CHECK((Args[0] & 15) == 0 && Args[1] == ExpectedHfcs);
    CHECK(Args[0] >= (uintptr_t)Input.UbmHfcDmaBuffer);
    CHECK(Args[0] + ExpectedHfcs * sizeof (MPIO_HFC_PORT) <=
      (uintptr_t)Input.UbmHfcDmaBuffer + sizeof (Input.UbmHfcDmaBuffer)
      );
    CHECK(memcmp((void *)(uintptr_t)Args[0],
      Input.HfcDescriptor.HfcPortList,
      ExpectedHfcs * sizeof (MPIO_HFC_PORT)
      ) == 0
      );
    CHECK(Input.HfcDescriptor.HfcPortList[0].Start_Lane == 32);
    CHECK(Input.HfcDescriptor.HfcPortList[0].Switch[0].Addr == 0x72);
    return EnumerationReply;
  }
  CHECK(Request == BIOS_MPIO_MSG_I2C_DEVICE_GET && Args[0] < ExpectedHfcs && Args[1] < 16);
  unsigned Hfc = Args[0], Index = Args[1];
  ++Gets;
  if (Hfc == FailHfc) {
    return 0xff;
  }
  DFC_DESCRIPTOR Reply = Replies[Index];
  Reply.hfc_idx += Hfc;
  if (Input.HfcDescriptor.HfcPortList[Hfc].NodeType == MPIO_I2C_NODE_TYPE_OCP) {
    CHECK(Args[2] == 0x800000f4 && Index == 0);
    const uint8_t Expected[] = {1, 0x20, 0x81, 48, 11, 0x11, 0, 50, 0x72, 0, 0, 0};
    CHECK(memcmp(&Input.HfcDescriptor.HfcPortList[Hfc], Expected, sizeof (Expected)) == 0);
    memset(&Reply.Device, 0, sizeof (Reply.Device));
    Reply.num_dfcs = 1;
    Reply.lane_start = 0;
    Reply.lane_width = 16;
    memcpy(&Args[1], &Reply, sizeof (Reply));
    return GetReply[Index];
  }
  CHECK(Args[2] == 0);
  Reply.Device.dfcubm.slot += Hfc * 4;
  memcpy(&Args[1], &Reply, sizeof (Reply));
  return GetReply[Index];
}

static void Init (unsigned Sockets)
{
  memset(&Fixture, 0, sizeof (Fixture));
  memset(Replies, 0, sizeof (Replies));
  Fixture.Before = Fixture.After = 0x123456789abcdef0ULL;
  Input.DiscoverUbm = true;
  Complex[0].SocketId = 0;
  Complex[0].PciePortList = Ports;
  Complex[0].Flags = DESCRIPTOR_TERMINATE_LIST;
  Ports[0].EngineData = (MPIO_ENGINE_DATA){.EngineType = MpioPcieEngine, .StartLane = 0, .EndLane = 15};
  Ports[1].Flags = DESCRIPTOR_TERMINATE_LIST;
  Ports[1].EngineData = (MPIO_ENGINE_DATA){.EngineType = MpioUBMHFCEngine, .StartLane = 32, .EndLane = 47};
  Ports[1].Port.SlotNum = 10;
  Ports[1].Port.TargetLinkSpeed = 4;
  Ports[1].Port.NpemCapability = 0xfff;
  Ports[1].PortParams.PhyParam[0] = (MPIO_PORT_PARAM){MPIO_PP_UBM_SWITCH0_ADDR, 0x72};
  Ports[1].PortParams.PhyParam[1] = (MPIO_PORT_PARAM){MPIO_PP_I2C_EXPANDER_ADDRESS, 0x20};
  if (Sockets == 2) {
    CHECK(PROJ_MAX_SOCKETS_SUPPORTED >= 2);
    Complex[0].Flags = 0;
    Complex[1].SocketId = 1;
    Complex[1].Flags = DESCRIPTOR_TERMINATE_LIST;
    Complex[1].PciePortList = &Ports[2];
    Ports[2].Flags = DESCRIPTOR_TERMINATE_LIST;
    Ports[2].EngineData = (MPIO_ENGINE_DATA){.EngineType = MpioPcieEngine, .StartLane = 48, .EndLane = 63};
    Ports[2].Port.SlotNum = 6;
  }
  Enumeration = Gets = 0;
  StatusRequests = 0;
  ExpectedHfcs = 1;
  FailHfc = ~0u;
  StatusReply = BIOSSMC_Result_OK;
  EnumerationReply = BIOSSMC_Result_OK;
  for (unsigned I = 0; I < 16; I++) { GetReply[I] = BIOSSMC_Result_OK;
  }
  for (unsigned I = 0; I < 4; I++) {
    Replies[I].num_dfcs = 4;
    Replies[I].event = BIOS_EVENT_DEVICE_CONNECTED;
    Replies[I].lane_start = 4 * I;
    Replies[I].lane_width = 4;
    Replies[I].Device.dfcubm.type = BIOS_DFC_INFO_TYPE_QUAD_PCI;
    Replies[I].Device.dfcubm.slot = 10 + I;
  }
}

static SIL_STATUS Run (void)
{
  PCI_ADDR Address = {.AddressValue = 0};
  SIL_STATUS Status = MpioDiscoverUbm(&Input, Address);
  CHECK(Fixture.Before == 0x123456789abcdef0ULL && Fixture.After == Fixture.Before);
  ++Checks;
  return Status;
}

static void Rejected (void)
{
  CHECK(Run () == SilPass);
  CHECK(Ports[1].EngineData.EngineType == MpioUnusedEngine);
  CHECK(Ports[0].EngineData.EngineType == MpioPcieEngine);
  CHECK(Ports[1].Flags == DESCRIPTOR_TERMINATE_LIST);
  unsigned Previous = Gets;
  CHECK(Run () == SilPass && Gets == Previous && Enumeration == 1);
}

#if PROJ_MAX_SOCKETS_SUPPORTED > 1
static void MultipleConnectors (bool FailSecond)
{
  Init(2);
  MPIO_PORT_DESCRIPTOR Tail = Ports[2], Hfc = Ports[1];
  Ports[1].Flags = 0;
  Ports[2] = Hfc;
  Ports[2].EngineData.StartLane = 16;
  Ports[2].EngineData.EndLane = 31;
  Ports[3] = Tail;
  Ports[3].Flags = 0;
  Ports[4] = Hfc;
  Ports[4].Flags = 0;
  Ports[5] = Ports[2];
  Complex[1].PciePortList = &Ports[3];
  ExpectedHfcs = 4;
  if (FailSecond) {
    FailHfc = 1;
  }
  CHECK(Run () == SilPass && Enumeration == 1 && StatusRequests == 1);
  unsigned Socket1 = FailSecond ? 6 : 9;
  CHECK(Complex[1].PciePortList == &Ports[Socket1]);
  CHECK(Ports[Socket1].EngineData.StartLane == 48 && Ports[Socket1].Port.SlotNum == 6);
  CHECK(Ports[Socket1 - 1].Flags == DESCRIPTOR_TERMINATE_LIST);
  for (unsigned H = 0; H < 4; H++) {
    unsigned Start = H == 0 ? 1 : H == 1 ? 5 : Socket1 + 1 + (H - 2) * 4;
    if (FailSecond && H == 1) {
      CHECK(Ports[Start].EngineData.EngineType == MpioUnusedEngine);
      continue;
    }
    for (unsigned D = 0; D < 4; D++) {
      MPIO_PORT_DESCRIPTOR *Port = &Ports[Start + D];
      CHECK(Port->EngineData.EngineType == MpioPcieEngine && Port->Port.PortPresent);
      CHECK(Port->EngineData.StartLane == ((H & 1) ? 16 : 32) + D * 4);
      CHECK(Port->EngineData.EndLane == Port->EngineData.StartLane + 3);
      CHECK(Port->PortParams.PhyParam[0].ParamValue == H);
      CHECK(Port->PortParams.PhyParam[1].ParamValue == D);
      CHECK(Port->Port.SlotNum == 10 + H * 4 + D);
    }
  }
  CHECK(Ports[Socket1 + 8].Flags == DESCRIPTOR_TERMINATE_LIST);
  CHECK(Gets == (FailSecond ? 13 : 16));
}
#endif

int main (void)
{
  /* Global interface contract: no switch/expander list means no mailbox call. */
  Init(1);
  Ports[1].PortParams.PhyParam[1].ParamType = 0;
  CHECK(Run () == SilInvalidParameter && Enumeration == 0 && StatusRequests == 0);
  const MPIO_PORT_PARAM OcpParams[] = {
    {MPIO_PP_UBM_SWITCH0_ADDR, 0x72},
    {MPIO_PP_I2C_EXPANDER_ADDRESS, 0x20},
    {MPIO_PP_I2C_EXPANDER_TYPE, 1},
    {MPIO_PP_I2C_CLEAR_ALL_INTS, 1},
    {MPIO_PP_OCP_PRESENT_START, 11},
    {MPIO_PP_OCP_NUM_HOSTS, 1},
    {MPIO_PP_OCP_NUM_SOCKETS, 1},
    {MPIO_PP_OCP_DEF_VALID, 1},
    {MPIO_PP_OCP_DEF_PRSNTB_PRIMARY, 4},
    {MPIO_PP_OCP_DEF_PRSNTB_SECONDARY, 15},
  };
  for (unsigned Bad = 0; Bad < 6; Bad++) {
    Init(PROJ_MAX_SOCKETS_SUPPORTED > 1 ? 2 : 1);
    MPIO_PORT_DESCRIPTOR Tail = Ports[2];
    Ports[2] = Ports[1];
    Ports[1].Flags = 0;
    Ports[1].PortParams.PhyParam[1].ParamType = 0; /* Expander is on OCP path. */
    Ports[2].EngineData.EngineType = MpioOCP3Engine;
    Ports[2].EngineData.StartLane = 48;
    Ports[2].EngineData.EndLane = 63;
    Ports[2].Port.SlotNum = 50;
    memcpy(Ports[2].PortParams.PhyParam, OcpParams, sizeof (OcpParams));
    if (PROJ_MAX_SOCKETS_SUPPORTED > 1) {
      Ports[3] = Tail;
      Complex[1].PciePortList = &Ports[3];
    }
    ExpectedHfcs = 2;
    if (Bad == 1) {
      Ports[2].PortParams.PhyParam[5].ParamValue = 2;
    }
    if (Bad == 2) {
      Ports[2].PortParams.PhyParam[6].ParamValue = 2;
    }
    if (Bad == 3) {
      Ports[2].PortParams.PhyParam[4].ParamValue = 13;
    }
    if (Bad == 4) {
      Ports[2].PortParams.PhyParam[9].ParamValue = 16;
    }
    if (Bad == 5) {
      FailHfc = 1;
    }
    if (Bad > 0 && Bad < 5) {
      CHECK(Run () != SilPass && Enumeration == 0);
      continue;
    }
    CHECK(Run () == SilPass && Enumeration == 1 && Gets == 5);
    CHECK(Ports[5].Flags == DESCRIPTOR_TERMINATE_LIST);
    CHECK(Ports[5].EngineData.EngineType == (Bad ? MpioUnusedEngine : MpioPcieEngine));
    if (!Bad) {
      CHECK(Ports[5].Port.LinkHotplug == PcieHotplugOCP);
      CHECK(Ports[5].Port.SlotNum == 50 && Ports[5].Port.PortPresent);
      CHECK(Ports[5].EngineData.StartLane == 48 && Ports[5].EngineData.EndLane == 63);
      CHECK(Ports[5].PortParams.PhyParam[0].ParamValue == 1);
    }
    if (PROJ_MAX_SOCKETS_SUPPORTED > 1) {
      CHECK(Complex[1].PciePortList == &Ports[6]);
      CHECK(memcmp(&Ports[6], &Tail, sizeof (Tail)) == 0);
    }
  }
#if PROJ_MAX_SOCKETS_SUPPORTED > 1
  MultipleConnectors(false);
  MultipleConnectors(true);
#endif
  Init(1);
  Input.DiscoverUbm = false;
  Input.PcieTopologyData.PlatformData[0].PciePortList = (void *)(uintptr_t)1;
  CHECK(Run () == SilPass && Enumeration == 0); /* EDKII opt-out is untouched. */
  Init(1);
  StatusReply = 0;
  CHECK(Run () == SilPass && StatusRequests == 1 && Enumeration == 0 && Gets == 0);
  CHECK(Ports[1].EngineData.EngineType == MpioUnusedEngine);
  CHECK(Ports[0].EngineData.EngineType == MpioPcieEngine);
  CHECK(Run () == SilPass && StatusRequests == 1);
  Init(1);
  StatusReply = 0xff;
  CHECK(Run () == SilPass && StatusRequests == 1 && Enumeration == 0 && Gets == 0);
  CHECK(Ports[1].EngineData.EngineType == MpioUnusedEngine);
  Init(1);
  CHECK(Run () == SilPass && StatusRequests == 1 && Enumeration == 1 && Gets == 4);


  for (unsigned Sockets = 1; Sockets <= PROJ_MAX_SOCKETS_SUPPORTED && Sockets <= 2; Sockets++) {
    Init(Sockets);
    Replies[1].event = BIOS_EVENT_DEVICE_NOT_PRESENT;
    Replies[1].Device.dfcubm.type = BIOS_DFC_INFO_TYPE_EMPTY;
    Replies[2].Device.dfcubm.type = BIOS_DFC_INFO_TYPE_SATA_SAS;
    CHECK(Run () == SilPass && Gets == 4);
    for (unsigned I = 0; I < 4; I++) {
      CHECK(Ports[I + 1].EngineData.StartLane == 32 + I * 4);
      CHECK(Ports[I + 1].EngineData.EndLane == 35 + I * 4);
      CHECK(Ports[I + 1].Port.PortPresent == (I != 1));
      CHECK(Ports[I + 1].Port.SlotNum == 10 + I);
      CHECK(Ports[I + 1].Port.TargetLinkSpeed == 4);
      CHECK(Ports[I + 1].Port.NpemCapability == 0xfff);
      CHECK(Ports[I + 1].PortParams.PhyParam[0].ParamValue == 0);
      CHECK(Ports[I + 1].PortParams.PhyParam[1].ParamValue == I);
      CHECK(Ports[I + 1].Flags == (I == 3 ? DESCRIPTOR_TERMINATE_LIST : 0));
    }
    CHECK(Ports[3].EngineData.EngineType == MpioSATAEngine);
    if (Sockets == 2) {
      CHECK(Complex[1].PciePortList == &Ports[5]);
      CHECK(Ports[5].EngineData.StartLane == 48 && Ports[5].Port.SlotNum == 6);
      CHECK(Ports[5].Flags == DESCRIPTOR_TERMINATE_LIST);
    }
    CHECK(Run () == SilPass && Enumeration == 1 && Gets == 4);
  }

  /* Empty zero-width placeholders may overlap a populated sibling's lanes. */
  for (unsigned Sockets = 1; Sockets <= PROJ_MAX_SOCKETS_SUPPORTED && Sockets <= 2; Sockets++) {
    Init(Sockets);
    Replies[1].event = BIOS_EVENT_DEVICE_NOT_PRESENT;
    Replies[1].Device.dfcubm.type = BIOS_DFC_INFO_TYPE_EMPTY;
    Replies[1].lane_start = 0;
    Replies[1].lane_width = 0;
    CHECK(Run () == SilPass && Gets == 4);
    CHECK(Ports[1].Port.PortPresent && Ports[1].EngineData.StartLane == 32 && Ports[1].EngineData.EndLane == 35);
    CHECK(!Ports[2].Port.PortPresent && Ports[2].EngineData.StartLane == 33 && Ports[2].EngineData.EndLane == 33);
    CHECK(Ports[3].Port.PortPresent && Ports[4].Port.PortPresent);
    CHECK(Ports[4].Flags == DESCRIPTOR_TERMINATE_LIST);
    if (Sockets == 2) {
      CHECK(Complex[1].PciePortList == &Ports[5] && Ports[5].Port.SlotNum == 6);
    }
  }
  Init(1);
  Replies[1].event = BIOS_EVENT_DEVICE_NOT_PRESENT;
  Replies[1].Device.dfcubm.type = BIOS_DFC_INFO_TYPE_EMPTY;
  Replies[1].lane_start = 15;
  Replies[1].lane_width = 0;
  Rejected ();

  Init(1);
  EnumerationReply = 0;
  Rejected ();
  CHECK(Gets == 0);
  Init(1);
  GetReply[0] = 0;
  Rejected ();
  CHECK(Gets == 1);
  Init(1);
  GetReply[3] = 0xff;
  Rejected (); /* No partial HFC insertion. */
  Init(1);
  Replies[0].num_dfcs = 0;
  Rejected ();
  CHECK(Gets == 1);
  Init(1);
  Replies[0].num_dfcs = 17;
  Rejected ();
  CHECK(Gets == 1);
  Init(1);
  Replies[1].num_dfcs = 3;
  Rejected ();
  Init(1);
  Replies[0].hfc_idx = 1;
  Rejected ();
  Init(1);
  Replies[0].event = BIOS_EVENT_DEVICE_DISCONNECTED;
  Rejected ();
  Init(1);
  Replies[0].lane_width = 0;
  Rejected ();
  Init(1);
  Replies[0].lane_width = 17;
  Rejected ();
  Init(1);
  Replies[0].lane_width = 3;
  Rejected ();
  Init(1);
  Replies[0].lane_start = 15;
  Rejected ();
  Init(1);
  Replies[1].lane_start = 0;
  Rejected ();
  Init(1);
  Replies[0].Device.dfcubm.type = 0;
  Rejected ();
  Init(1);
  Replies[0].Device.dfcubm.type = BIOS_DFC_INFO_TYPE_EMPTY;
  Rejected ();

  Init(1);
  Ports[MAX_PORTS_SUPPORTED - 1] = Ports[1];
  memset(&Ports[1], 0, (MAX_PORTS_SUPPORTED - 2) * sizeof (Ports[0]));
  CHECK(Run () == SilPass && Gets == 4);
  CHECK(Ports[MAX_PORTS_SUPPORTED - 1].EngineData.EngineType == MpioUnusedEngine);
  CHECK(Ports[MAX_PORTS_SUPPORTED - 1].Flags == DESCRIPTOR_TERMINATE_LIST);

  Init(1);
  Ports[0].EngineData.EndLane = 32;
  CHECK(Run () == SilInvalidParameter && Enumeration == 0);
  Init(1);
  Ports[1].EngineData.EndLane = 48;
  CHECK(Run () == SilInvalidParameter && Enumeration == 0);
  Init(1);
  Complex[0].PciePortList = &Ports[1];
  CHECK(Run () == SilInvalidParameter && Enumeration == 0);
  Init(1);
  Ports[1].Flags = 0;
  CHECK(Run () == SilOutOfBounds && Enumeration == 0);
  Init(1);
  Ports[1].PortParams.PhyParam[0].ParamValue = 0x80;
  CHECK(Run () == SilInvalidParameter && Enumeration == 0);
  Init(1);
  for (unsigned I = 0; I < PCIE_PORT_PARAMETER_COUNT; I++) {
    Ports[1].PortParams.PhyParam[I] = (MPIO_PORT_PARAM){MPIO_PP_UBM_SWITCH0_ADDR, 0x72};
  }
  CHECK(Run () == SilInvalidParameter && Enumeration == 0);

#if UINTPTR_MAX > UINT32_MAX
  Init(1);
  MPIOCLASS_INPUT_BLK *High = malloc(sizeof (*High));
  CHECK(High && (uintptr_t)High > UINT32_MAX); /* ASan host heap is above 4GiB. */
  *High = Input;
  High->PcieTopologyData.PlatformData[0].PciePortList = High->PcieTopologyData.PortList;
  PCI_ADDR Address = {.AddressValue = 0};
  CHECK(MpioDiscoverUbm(High, Address) == SilOutOfBounds && Enumeration == 0);
  free(High);
  ++Checks;
#endif
  printf("UBM discovery: %u checks passed; hardware not accessed\n", Checks);
  return 0;
}
