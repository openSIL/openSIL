/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 *  @file PcieRcCfgReg.h
 *  @brief This file contains Pcie rc Register definitions
 */

#pragma once

#define AP_CNTL_AP_NEGOTIATION_GLOBAL_EN_OFFSET  8
#define SIL_RESERVED_1521  0x100

#define SIL_RESERVED_1610  0x1a300538UL

#define SIL_RESERVED_1525  5
#define SIL_RESERVED_1524  0xe0

#define SIL_RESERVED_1614  0x1a300060UL

#define SIL_RESERVED_1523  0
#define SIL_RESERVED_1522  0xf

#define SIL_RESERVED_1613  0x1a300080UL

#define SIL_RESERVED_1527  0
#define SIL_RESERVED_1526  0x1
#define SIL_RESERVED_1533  2
#define SIL_RESERVED_1532  0x4

#define SIL_RESERVED_1531  3
#define SIL_RESERVED_1530  0x8

#define SIL_RESERVED_1529  5
#define SIL_RESERVED_1528  0x20

typedef union {
  struct {
    uint16_t                                      field_bit_0:1;
    uint16_t                                      field_bit_1:1;
    uint16_t                                      field_bit_2:1;
    uint16_t                                      field_bit_3:1;
    uint16_t                                      field_bit_4:1;
    uint16_t                                      field_bit_5:1;
    uint16_t                                      field_bit_6:1;
    uint16_t                                      field_bit_7:1;
    uint16_t                                      field_bit_8:1;
    uint16_t                                      field_bit_9:1;
    uint16_t                                      field_bits_10_to_15:6;
  } Field;
  uint16_t Value;
} SIL_RESERVED_UNION_0012;

#define SIL_RESERVED_1615    0x1a300588UL

typedef union {
  struct {
    uint32_t                                          field_bits_0_to_3:4;
    uint32_t                                          field_bits_4_to_9:6;
    uint32_t                                          field_bits_10_to_11:2;
    uint32_t                                    field_bits_12_to_14:3;
    uint32_t                                     field_bits_15_to_17:3;
    uint32_t                              field_bit_18:1;
    uint32_t                         field_bit_19:1;
    uint32_t                         field_bit_20:1;
    uint32_t                            field_bit_21:1;
    uint32_t                         field_bit_22:1;
    uint32_t                                      field_bit_23:1;
    uint32_t                                         field_bits_24_to_31:8;
  } Field;
  uint32_t Value;
} SIL_RESERVED_UNION_0023;


#define SIL_RESERVED_1616  0x1a300064UL
#define SIL_RESERVED_1619  0x1a300068UL

#define SIL_RESERVED_1535  7
#define SIL_RESERVED_1534  0x380

#define SIL_RESERVED_1617  0x1a300088UL

#define SIL_RESERVED_1537  8
#define SIL_RESERVED_1536  0x700

#define SIL_RESERVED_1618  0x1a300508UL

#define PCIE_CAP_SLOT_IMPLEMENTED_OFFSET  8
#define PCIE_CAP_SLOT_IMPLEMENTED_MASK  0x100

#define SIL_RESERVED_1622  0x1a300058UL
#define SIL_RESERVED_1624  0x1a300388UL

#define SIL_RESERVED_1549  0
#define SIL_RESERVED_1548  0x1

#define SIL_RESERVED_1545  1
#define SIL_RESERVED_1544  0x2

#define SIL_RESERVED_1547  2
#define SIL_RESERVED_1546  0x4

#define SIL_RESERVED_1551  10
#define SIL_RESERVED_1550  0x400

#define SIL_RESERVED_1553  18
#define SIL_RESERVED_1552  0x40000

#define SIL_RESERVED_1625  0x1a300390UL

#define SIL_RESERVED_1577  0
#define SIL_RESERVED_1576  0x1
#define SIL_RESERVED_1591  1
#define SIL_RESERVED_1590  0x2
#define SIL_RESERVED_1587  2
#define SIL_RESERVED_1586  0x4
#define SIL_RESERVED_1579  3
#define SIL_RESERVED_1578  0x8
#define SIL_RESERVED_1593  4
#define SIL_RESERVED_1592  0x10
#define SIL_RESERVED_1585  5
#define SIL_RESERVED_1584  0x20
#define SIL_RESERVED_1583  6
#define SIL_RESERVED_1582  0x40
#define SLOT_CAP_SLOT_PWR_LIMIT_VALUE_OFFSET  7
#define SIL_RESERVED_1594  0x7f80
#define SLOT_CNTL_PWR_INDICATOR_CNTL_OFFSET  8
#define SLOT_CNTL_PWR_INDICATOR_CNTL_MASK  0x300
#define SLOT_CNTL_PWR_CONTROLLER_CNTL_OFFSET  10
#define SLOT_CNTL_PWR_CONTROLLER_CNTL_MASK  0x400
#define SLOT_CAP_SLOT_PWR_LIMIT_SCALE_OFFSET  15
#define SLOT_CAP_SLOT_PWR_LIMIT_SCALE_MASK  0x18000
#define SIL_RESERVED_1581  17
#define SIL_RESERVED_1580  0x20000
#define SIL_RESERVED_1589  18
#define SIL_RESERVED_1588  0x40000
#define SLOT_CAP_PHYSICAL_SLOT_NUM_OFFSET  19
#define SLOT_CAP_PHYSICAL_SLOT_NUM_MASK  0xfff80000

#define SIL_RESERVED_1630  0x1a30006cUL

#define SIL_RESERVED_1543  0
#define SIL_RESERVED_1542  0x3

#define SIL_RESERVED_1541  4
#define SIL_RESERVED_1540  0x10

#define SIL_RESERVED_1539  7
#define SIL_RESERVED_1538  0x80

#define SIL_RESERVED_1623  0x1a300386UL

#define SIL_RESERVED_1575  0
#define SIL_RESERVED_1574  0x1

#define SIL_RESERVED_1629  0x1a30008cUL
#define SIL_RESERVED_1626  0x1a300394UL

#define SIL_RESERVED_1602  3
#define SIL_RESERVED_1601  0x8

#define SIL_RESERVED_1598  5
#define SIL_RESERVED_1597  0x20

#define SIL_RESERVED_1596  12
#define SIL_RESERVED_1595  0x1000

#define SIL_RESERVED_1600  14
#define SIL_RESERVED_1599  0x4000

#define SIL_RESERVED_1631  0x1a300070UL

#define SIL_RESERVED_1603  0

#define SIL_RESERVED_1608  1

#define SIL_RESERVED_1606  2

#define SIL_RESERVED_1607  3

#define SIL_RESERVED_1604  4

#define SIL_RESERVED_1605  8

#define SIL_RESERVED_1567  2
#define SIL_RESERVED_1566  0x4

#define SIL_RESERVED_1555  3
#define SIL_RESERVED_1554  0x8

#define SIL_RESERVED_1565  6
#define SIL_RESERVED_1564  0x40

#define SIL_RESERVED_1563  8
#define SIL_RESERVED_1562  0x100

#define SIL_RESERVED_1627  0x1a3005e4UL
#define SIL_RESERVED_1620  0x1a3004d4UL
#define SIL_RESERVED_1621  0x1a3004d8UL

#define SIL_RESERVED_1569  0
#define SIL_RESERVED_1568  0x1
#define SIL_RESERVED_1557  1
#define SIL_RESERVED_1556  0x2
#define SIL_RESERVED_1559  4
#define SIL_RESERVED_1558  0x30
#define SIL_RESERVED_1561  7
#define SIL_RESERVED_1560  0x80

#define SIL_RESERVED_1573  2
#define SIL_RESERVED_1572  0x4
#define SIL_RESERVED_1571  4
#define SIL_RESERVED_1570  0x10

#define SIL_RESERVED_1628  0x1a3005e8UL


typedef union {
  struct {
    uint16_t                                      field_bit_0:1;
    uint16_t                                         field_bit_1:1;
    uint16_t                                        field_bit_2:1;
    uint16_t                        field_bit_3:1;
    uint16_t                               field_bit_4:1;
    uint16_t                                     field_bit_5:1;
    uint16_t                                    field_bit_6:1;
    uint16_t                                       field_bit_7:1;
    uint16_t          field_bit_8:1;
    uint16_t        field_bit_9:1;
    uint16_t                 field_bit_10:1;
    uint16_t                    field_bit_11:1;
    uint16_t                  field_bit_12:1;
    uint16_t                                     field_bits_13_to_15:3;
  } Field;
  uint16_t Value;
} SIL_RESERVED_UNION_0013;

#define SIL_RESERVED_1609                     0x1a30004cUL

typedef union {
  struct {
    uint32_t                                     field_bit_0:1;
    uint32_t                                       field_bits_1_to_12:12;
    uint32_t                         field_bits_13_to_31:19;
  } Field;
  uint32_t Value;
} SIL_RESERVED_UNION_0008;
#define SIL_RESERVED_1611                    0x1a300640UL

typedef union {
  struct {
    uint32_t                               field_bits_0_to_31:32;
  } Field;
  uint32_t Value;
} SIL_RESERVED_UNION_0007;
#define SIL_RESERVED_1612               0x1a300644UL
