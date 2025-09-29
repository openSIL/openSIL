/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  MemTablePhx.h
 * @brief Memory Tables in MEM
 *
 */

#pragma once

#include <MEM/Common/MemTableTypes.h>
#include <MEM/MemClass-api.h>

const HOST_TO_APCB_CHANNEL_XLAT gDummyChannelXlatTable[] = {
  // Requested   Translated
  { 0xFF, 0xFF}
};

// Conversion Macro from internal index to Interface DDRVoltage in Millivolts
#define SIL_CONVERT_ENCODED_TO_VDDIO_MILLIVOLTS(EncodedVal, TechType) ( \
          (TechType == SilDdr5Technology) ? \
          ((EncodedVal == VOLT1_1_ENCODED_VAL) ? VOLT1_1_MILLIVOLTS : \
          0) : \
          0)

// Mapping of SPD capacity values to sizes,
const SPD_CAPACITY_TABLE SpdCapacityTable[] = {
  // SPD Value, Size
  0x0000,     // 0000,      Reserved
  0x0000,     // 0001,      Reserved
  0x400,      // 0010,      1 GB
  0x800,      // 0011,      2 GB
  0x1000,     // 0100,      4 GB
  0x2000,     // 0101,      8 GB
  0x4000,     // 0110,      16 GB
  0x8000,     // 0111,      32 GB
  0x3000,     // 1000,      12 GB
  0x6000,     // 1001,      24 GB
  0xc00,      // 1010,      3 GB
  0x1800,     // 1011,      6 GB
  0x4800      // 1100,      18 GB
};

/**
 * @brief A table of type _MEM_TCK_ENTRY with multiple entries for memory clock freq vs ticks ps
 */
const MEM_TCK_ENTRY MemFreqToTckTable[] = {
  {1000, 1000},
  {1050, 952},
  {1067, 938},
  {1200, 833},
  {1333, 750},
  {1367, 732},
  {1400, 714},
  {1433, 698},
  {1467, 682},
  {1500, 667},
  {1533, 652},
  {1567, 638},
  {1600, 625},
  {1633, 612},
  {1667, 599},
  {1700, 588},
  {1733, 577},
  {1767, 565},
  {1800, 555},
  {1833, 545},
  {1867, 535},
  {1900, 526},
  {1933, 517},
  {1967, 508},
  {2000, 500},
  {2033, 491},
  {2067, 483},
  {2100, 476},
  {2133, 468},
  {2167, 461},
  {2200, 454},
  {2233, 447},
  {2267, 441},
  {2300, 434},
  {2333, 428},
  {2367, 422},
  {2400, 416},
  {2433, 411},
  {2467, 405},
  {2500, 400},
  {2550, 392},
  {2600, 384},
  {2650, 377},
  {2700, 370},
  {2750, 363},
  {2800, 357},
  {2850, 350},
  {2900, 344},
  {2950, 338},
  {3000, 333},
  {3050, 327},
  {3100, 322},
  {3150, 317},
  {3200, 312},
  {3250, 307},
  {3300, 303},
  {3350, 298},
  {3400, 294},
  {3450, 289},
  {3500, 285},
  {3550, 281},
  {3600, 277},
  {3650, 273},
  {3700, 270},
  {3750, 266},
  {3800, 263},
  {3850, 259},
  {3900, 256},
  {3950, 253},
  {4000, 250},
  {4050, 246},
  {4100, 243},
  {4150, 240},
  {4200, 238},
  {4250, 235},
  {4300, 232},
  {4350, 229},
  {4400, 227}
};
