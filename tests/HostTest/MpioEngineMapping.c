/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2026 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file MpioEngineMapping.c
 * @brief Exercise real ASK-to-engine lookup with mocked NBIO traversal.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Nbio/NbioIp2Ip.h>
#include <Mpio/Common/MpioLibLocal.h>

#define CHECK(Condition) do { if (!(Condition)) { \
                                fprintf(stderr, "MPIO mapping check failed at %d: %s\n", __LINE__, #Condition); \
                                abort (); \
                              } } while (0)

static GNB_HANDLE Handle;
static PCIe_WRAPPER_CONFIG Wrapper;
static PCIe_ENGINE_CONFIG Engines[4];
static unsigned Traversals, Checks;

static void
RunEngines (
  uint32_t Flags,
  PCIe_RUN_ON_ENGINE_CALLBACK2 Callback,
  void *Buffer,
  PCIe_WRAPPER_CONFIG *Selected
  )
{
  CHECK(Flags == DESCRIPTOR_PCIE_ENGINE && Selected == &Wrapper);
  for (unsigned Index = 0; Index < 4; Index++) {
    Callback(&Engines[Index], Buffer, Selected);
  }
}

static void
RunWrappers (
  uint32_t Flags,
  PCIe_RUN_ON_WRAPPER_CALLBACK2 Callback,
  void *Buffer,
  GNB_HANDLE *Selected
  )
{
  CHECK(Flags == DESCRIPTOR_ALL_WRAPPERS && Selected == &Handle);
  ++Traversals;
  Callback(&Wrapper, Buffer, Selected);
}

static NBIO_IP2IP_API Nbio = {
  .PcieConfigRunProcForAllEnginesInWrapper = RunEngines,
  .PcieConfigRunProcForAllWrappersInNbio = RunWrappers,
};

SIL_STATUS
SilGetIp2IpApi (
  SIL_DATA_BLOCK_ID Id,
  void **Api
  )
{
  CHECK(Id == SilId_NbioClass);
  *Api = &Nbio;
  return SilPass;
}

int
main (
  void
  )
{
  FW_ASK_STRUCT Ask;
  memset(&Ask, 0, sizeof (Ask));
  Handle.Header.DescriptorFlags = DESCRIPTOR_TERMINATE_TOPOLOGY;
  Wrapper.StartDxioLane = 32;
  Wrapper.EndDxioLane = 47;
  for (unsigned Index = 0; Index < 4; Index++) {
    Engines[Index].Type.Port.PortId = Index;
  }
  Ask.desc.ctrlType = ASK_TYPE_PCIe;
  Ask.desc.startLaneIndex = 44;

  CHECK(MpioFindEngineForAsk(NULL, &Ask) == NULL);
  CHECK(MpioFindEngineForAsk(&Handle, NULL) == NULL);
  CHECK(Traversals == 0);
  Checks += 2;

  /* No allocation and bifurcation failures return a default port number.
   * It must not resolve to a real engine, even when that number is port 0. */
  for (unsigned State = LINK_NOT_ALLOCATED; State <= LINK_TIMEOUT; State++) {
    for (unsigned Port = 0; Port < 4; Port++) {
      Ask.status.state = State;
      Ask.status.port = Port;
      Traversals = 0;
      PCIe_ENGINE_CONFIG *Found = MpioFindEngineForAsk(&Handle, &Ask);
      if (State == LINK_NOT_ALLOCATED || State == LINK_BIFURCATION_FAILURE) {
        CHECK(Found == NULL && Traversals == 0);
      } else {
        /* Allocated port 0 and failed-training ports still have identities. */
        CHECK(Found == &Engines[Port] && Traversals == 1);
      }
      ++Checks;
    }
  }
  Ask.status.state = LINK_TRAINED;
  Ask.status.port = 0xff;
  CHECK(MpioFindEngineForAsk(&Handle, &Ask) == NULL);
  ++Checks;
  Ask.status.port = 0;
  Ask.desc.startLaneIndex = 48;
  CHECK(MpioFindEngineForAsk(&Handle, &Ask) == NULL);
  ++Checks;
  printf("MPIO engine mapping: %u checks passed\n", Checks);
  return 0;
}
