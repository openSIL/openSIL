/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2023 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 *  @file NbioBaseReg.h
 *  @brief This file contains all Nbio Base Register definitions
 */

#pragma once

#define SIL_RESERVED_1293  21
#define SIL_RESERVED_1292  0x200000
#define SIL_RESERVED_1494  0x13b10004UL

#define SIL_RESERVED_1247  0
#define SIL_RESERVED_1246  0x1
#define SIL_RESERVED_1245  6
#define SIL_RESERVED_1244  0x40
#define SIL_RESERVED_1249  12
#define SIL_RESERVED_1248  0x1000
#define SIL_RESERVED_1243  17
#define SIL_RESERVED_1242  0x20000
#define SIL_RESERVED_1496  0x14b0001cUL
#define SIL_RESERVED_1495  0x1470001cUL

#define SIL_RESERVED_1295  16
#define SIL_RESERVED_1294  0x10000
#define SIL_RESERVED_1500  0x4000200UL

#define SIL_RESERVED_1297  1
#define SIL_RESERVED_1296  0xe
#define SIL_RESERVED_1501  0x1a380040UL

#define SIL_RESERVED_1161  17
#define SIL_RESERVED_1160  0x20000
#define SIL_RSVD_ADDR_1A380460  0x1a380460UL

#define SIL_RESERVED_1429  24
#define SIL_RESERVED_1428  0x3000000
#define SIL_RSVD_ADDR_1A3804E0  0x1a3804e0UL

#define SIL_RESERVED_1405  6
#define SIL_RESERVED_1404  0x40
#define SIL_RESERVED_1403  14
#define SIL_RESERVED_1402  0xc000
#define SIL_RESERVED_1503  0x1a380100UL

#define SIL_RESERVED_1435  23
#define SIL_RESERVED_1434  0x800000
#define SIL_RESERVED_1433  24
#define SIL_RESERVED_1432  0x1000000
#define SIL_RSVD_ADDR_1A3801B0  0x1a3801b0UL

#define SIL_RESERVED_1441  7
#define SIL_RESERVED_1440  0x1f80
#define SIL_RESERVED_1439  13
#define SIL_RESERVED_1438  0xfe000
#define SIL_RESERVED_1504  0x1a3804d8UL

#define SIL_RESERVED_1443  19
#define SIL_RESERVED_1442  0xf80000
#define SIL_RESERVED_1505  0x1a3804dcUL

#define SIL_RESERVED_1445  2
#define SIL_RESERVED_1444  0x4
#define SIL_RESERVED_1413  0
#define SIL_RESERVED_1412  0x1
#define SIL_RESERVED_1419  16
#define SIL_RESERVED_1418  0x10000
#define SIL_RESERVED_1506  0x1a3804d4UL

#define SIL_RESERVED_1415  2
#define SIL_RESERVED_1414  0x4
#define SIL_RESERVED_1417  7
#define SIL_RESERVED_1416  0x80
#define SIL_RESERVED_1407  18
#define SIL_RESERVED_1406  0x40000
#define SIL_RESERVED_1487  0x1a340368UL

#define SIL_RESERVED_1449  5
#define SIL_RESERVED_1448  0x20
#define SIL_RESERVED_1411  0
#define SIL_RESERVED_1410  0x1
#define SIL_RESERVED_1409  1
#define SIL_RESERVED_1408  0x2
#define SIL_RESERVED_1447  6
#define SIL_RESERVED_1446  0x40
#define SIL_RESERVED_1455  13
#define SIL_RESERVED_1454  0x2000
#define SIL_RESERVED_1457  14
#define SIL_RESERVED_1456  0x4000
#define SIL_RESERVED_1451  18
#define SIL_RESERVED_1450  0x40000
#define SIL_RESERVED_1453  20
#define SIL_RESERVED_1452  0x100000
#define SIL_RESERVED_1507  0x1a3802c0UL

#define SIL_RESERVED_1459  24
#define SIL_RESERVED_1458  0x1000000
#define SIL_RESERVED_1461  25
#define SIL_RESERVED_1460  0x2000000
#define SIL_RESERVED_1508  0x1a380620UL

#define SIL_RESERVED_1488  0x1a340348UL

#define SIL_RESERVED_1421  15
#define SIL_RESERVED_1420  0x8000
#define SIL_RESERVED_1489  0x1a34000cUL

#define SIL_RESERVED_1423  2
#define SIL_RESERVED_1422  0x4
#define SIL_RESERVED_1425  3
#define SIL_RESERVED_1424  0x8
#define SIL_RESERVED_1427  4
#define SIL_RESERVED_1426  0x10
#define SIL_RESERVED_1490  0x1a340040UL

#define SIL_RESERVED_1431  12
#define SIL_RESERVED_1430  0x1000
#define SIL_RESERVED_1491  0x1a340300UL

typedef union {
  struct {
    uint32_t                            field_bit_0:1;
    uint32_t                            field_bit_1:1;
    uint32_t                            field_bit_2:1;
    uint32_t                            field_bit_3:1;
    uint32_t                            field_bits_4_to_5:2;
    uint32_t                            field_bits_6_to_7:2;
    uint32_t                            field_bits_8_to_9:2;
    uint32_t                            field_bits_10_to_11:2;
    uint32_t                            field_bits_12_to_13:2;
    uint32_t                            field_bits_14_to_15:2;
    uint32_t                            field_bits_16_to_23:8;
    uint32_t                            field_bit_24:1;
    uint32_t                            field_bit_25:1;
    uint32_t                            field_bit_26:1;
    uint32_t                            field_bit_27:1;
    uint32_t                            field_bit_28:1;
    uint32_t                            field_bit_29:1;
    uint32_t                            field_bit_30:1;
    uint32_t                            field_bit_31:1;
  } Field;
  uint32_t Value;
} SIL_RESERVED_UNION_0032;
#define SIL_RESERVED_1465    0x1a340424UL

#define SIL_RESERVED_1335  23
#define SIL_RESERVED_1334  0x800000
#define SIL_RESERVED_1472  0x1a340280UL

#define SIL_RESERVED_1315  8
#define SIL_RESERVED_1314  0x100
#define SIL_RESERVED_1317  13
#define SIL_RESERVED_1316  0x2000
#define SIL_RESERVED_1313  14
#define SIL_RESERVED_1312  0xc000
#define SIL_RESERVED_1468  0x1a3402c4UL

#define SIL_RESERVED_1319  10
#define SIL_RESERVED_1318  0x400
#define SIL_RESERVED_1325  11
#define SIL_RESERVED_1324  0x800
#define SIL_RESERVED_1321  12
#define SIL_RESERVED_1320  0x1000
#define SIL_RESERVED_1323  14
#define SIL_RESERVED_1322  0x4000
#define SIL_RESERVED_1469  0x1a3402d4UL

#define SIL_RESERVED_1327  4
#define SIL_RESERVED_1326  0x10
#define SIL_RESERVED_1470  0x1a3402d8UL

#define SIL_RESERVED_1333  12
#define SIL_RESERVED_1328  20
#define SIL_RESERVED_1330  21
#define SIL_RESERVED_1329  0x600000
#define SIL_RESERVED_1332  23
#define SIL_RESERVED_1331  0x1800000
#define SIL_RESERVED_1471  0x1a3402ecUL

#define SIL_RESERVED_1337  0
#define SIL_RESERVED_1336  0x1
#define SIL_RESERVED_1339  2
#define SIL_RESERVED_1338  0xc
#define SIL_RESERVED_1345  6
#define SIL_RESERVED_1344  0x40
#define SIL_RESERVED_1341  10
#define SIL_RESERVED_1340  0x400
#define SIL_RESERVED_1343  11
#define SIL_RESERVED_1342  0x7800
#define SIL_RESERVED_1473  0x1a340394UL

#define SIL_RESERVED_1347  0
#define SIL_RESERVED_1346  0x1
#define SIL_RESERVED_1349  2
#define SIL_RESERVED_1348  0xc
#define SIL_RESERVED_1355  6
#define SIL_RESERVED_1354  0x40
#define SIL_RESERVED_1351  10
#define SIL_RESERVED_1350  0x400
#define SIL_RESERVED_1353  11
#define SIL_RESERVED_1352  0x7800
#define SIL_RESERVED_1474  0x1a340400UL

#define SIL_RESERVED_1357  0
#define SIL_RESERVED_1356  0x1
#define SIL_RESERVED_1359  2
#define SIL_RESERVED_1358  0xc
#define SIL_RESERVED_1365  6
#define SIL_RESERVED_1364  0x40
#define SIL_RESERVED_1361  10
#define SIL_RESERVED_1360  0x400
#define SIL_RESERVED_1363  11
#define SIL_RESERVED_1362  0x7800
#define SIL_RESERVED_1475  0x1a340390UL

#define SIL_RESERVED_1373  16
#define SIL_RESERVED_1372  0x70000
#define SIL_RESERVED_1375  20
#define SIL_RESERVED_1374  0x700000
#define SIL_RESERVED_1479  0x1a340318UL

#define SIL_RESERVED_1383  10
#define SIL_RESERVED_1382  0x400

#define SIL_RESERVED_1381  17
#define SIL_RESERVED_1380  0x20000

#define SIL_RESERVED_1377  19
#define SIL_RESERVED_1376  0x80000

#define SIL_RESERVED_1379  21
#define SIL_RESERVED_1378  0x600000

#define SIL_RESERVED_1385  30
#define SIL_RESERVED_1384  0x40000000

#define SIL_RESERVED_1480  0x1a340288UL

#define SIL_RESERVED_1394  0x1
#define SIL_RESERVED_1395  0x2
#define SIL_RESERVED_1396  0x4
#define SIL_RESERVED_1397  0x8

#define SIL_RESERVED_1483  0x1a340290UL

#define SIL_RESERVED_1393  12
#define SIL_RESERVED_1392  0x1000

#define SIL_RESERVED_1482  0x1a340414UL

typedef union {
  struct {
    uint32_t                field_bits_0_to_5:6;
    uint32_t                field_bits_6_to_7:2;
    uint32_t                field_bits_8_to_13:6;
    uint32_t                field_bits_14_to_15:2;
    uint32_t                field_bits_16_to_21:6;
    uint32_t                field_bits_22_to_23:2;
    uint32_t                field_bits_24_to_29:6;
    uint32_t                field_bits_30_to_31:2;
  } Field;
  uint32_t Value;
} SIL_RESERVED_UNION_0033;

#define SIL_RESERVED_1484    0x1a340294UL

#define SIL_RESERVED_1463  15
#define SIL_RESERVED_1462  0x8000
#define SIL_RESERVED_1492  0x1a340600UL
#define SIL_RESERVED_1251  13
#define SIL_RESERVED_1250  0x2000
#define SIL_RESERVED_1464  0x1a30006aUL
#define SIL_RESERVED_1399  6
#define SIL_RESERVED_1398  0x40
#define SIL_RESERVED_1485  0x1a340284UL

typedef union {
  struct {
    uint32_t                            field_bits_0_to_2:3;
    uint32_t                            field_bits_3_to_7:5;
    uint32_t                            field_bits_8_to_15:8;
    uint32_t                            field_bits_16_to_18:3;
    uint32_t                            field_bits_19_to_23:5;
    uint32_t                            field_bits_24_to_31:8;
  } Field;
  uint32_t Value;
} SIL_RESERVED_UNION_0036;

#define SIL_RESERVED_1493    0x1a340084UL

#define SIL_RESERVED_1203  0
#define SIL_RESERVED_1202  0x1
#define SIL_RESERVED_1205  1
#define SIL_RESERVED_1204  0x2
#define SIL_RESERVED_1207  2
#define SIL_RESERVED_1206  0x4
#define SIL_RESERVED_1209  3
#define SIL_RESERVED_1208  0x8
#define SIL_RESERVED_1211  4
#define SIL_RESERVED_1210  0x10
#define SIL_RESERVED_1213  5
#define SIL_RESERVED_1212  0x20
#define SIL_RESERVED_1215  6
#define SIL_RESERVED_1214  0x40
#define SIL_RESERVED_1217  7
#define SIL_RESERVED_1216  0x80
#define SIL_RESERVED_1219  8
#define SIL_RESERVED_1218  0x100
#define SIL_RESERVED_1221  9
#define SIL_RESERVED_1220  0x200
#define SIL_RESERVED_1223  10
#define SIL_RESERVED_1222  0x400
#define SIL_RESERVED_1225  11
#define SIL_RESERVED_1224  0x800
#define SIL_RESERVED_1227  12
#define SIL_RESERVED_1226  0x1000
#define SIL_RESERVED_1229  13
#define SIL_RESERVED_1228  0x2000
#define SIL_RESERVED_1231  14
#define SIL_RESERVED_1230  0x4000
#define SIL_RESERVED_1233  15
#define SIL_RESERVED_1232  0x8000
#define SIL_RESERVED_1235  16
#define SIL_RESERVED_1234  0x10000
#define SIL_RESERVED_1237  17
#define SIL_RESERVED_1236  0x20000
#define SIL_RESERVED_1239  18
#define SIL_RESERVED_1238  0x40000
#define SIL_RESERVED_1241  19
#define SIL_RESERVED_1240  0x80000

#define SIL_RESERVED_1498  0x141e8f0UL
#define SIL_RESERVED_1497  0x141e190UL

#define SIL_RESERVED_1163 0
#define SIL_RESERVED_1162 0x1

#define SIL_RESERVED_1165 1
#define SIL_RESERVED_1164 0x2

#define SIL_RESERVED_1167 2
#define SIL_RESERVED_1166 0x4

#define SIL_RESERVED_1169 3
#define SIL_RESERVED_1168 0x8

#define SIL_RESERVED_1171 4
#define SIL_RESERVED_1170 0x10

#define SIL_RESERVED_1173 5
#define SIL_RESERVED_1172 0x20

#define SIL_RESERVED_1175 6
#define SIL_RESERVED_1174 0x40

#define SIL_RESERVED_1177 7
#define SIL_RESERVED_1176 0x80

#define SIL_RESERVED_1179 8
#define SIL_RESERVED_1178 0x100

#define SIL_RESERVED_1181 9
#define SIL_RESERVED_1180 0x200

#define SIL_RESERVED_1183 10
#define SIL_RESERVED_1182 0x400

#define SIL_RESERVED_1185 11
#define SIL_RESERVED_1184 0x800

#define SIL_RESERVED_1187 12
#define SIL_RESERVED_1186 0x1000

#define SIL_RESERVED_1189 13
#define SIL_RESERVED_1188 0x2000

#define SIL_RESERVED_1191 14
#define SIL_RESERVED_1190 0x4000

#define SIL_RESERVED_1193 15
#define SIL_RESERVED_1192 0x8000

#define SIL_RESERVED_1195 16
#define SIL_RESERVED_1194 0x10000

#define SIL_RESERVED_1197 17
#define SIL_RESERVED_1196 0x20000

#define SIL_RESERVED_1199 18
#define SIL_RESERVED_1198 0x40000

#define SIL_RESERVED_1201 19
#define SIL_RESERVED_1200 0x80000

#define SIL_RESERVED_1253  0
#define SIL_RESERVED_1252  0x1
#define SIL_RESERVED_1255  1
#define SIL_RESERVED_1254  0x2
#define SIL_RESERVED_1257  2
#define SIL_RESERVED_1256  0x4
#define SIL_RESERVED_1259  3
#define SIL_RESERVED_1258  0x8
#define SIL_RESERVED_1261  4
#define SIL_RESERVED_1260  0x10
#define SIL_RESERVED_1263  5
#define SIL_RESERVED_1262  0x20
#define SIL_RESERVED_1265  6
#define SIL_RESERVED_1264  0x40
#define SIL_RESERVED_1267  7
#define SIL_RESERVED_1266  0x80
#define SIL_RESERVED_1269  8
#define SIL_RESERVED_1268  0x100
#define SIL_RESERVED_1271  9
#define SIL_RESERVED_1270  0x200
#define SIL_RESERVED_1273  10
#define SIL_RESERVED_1272  0x400
#define SIL_RESERVED_1275  11
#define SIL_RESERVED_1274  0x800
#define SIL_RESERVED_1277  12
#define SIL_RESERVED_1276  0x1000
#define SIL_RESERVED_1279  13
#define SIL_RESERVED_1278  0x2000
#define SIL_RESERVED_1281  14
#define SIL_RESERVED_1280  0x4000
#define SIL_RESERVED_1283  15
#define SIL_RESERVED_1282  0x8000
#define SIL_RESERVED_1285  16
#define SIL_RESERVED_1284  0x10000
#define SIL_RESERVED_1287  17
#define SIL_RESERVED_1286  0x20000
#define SIL_RESERVED_1289  18
#define SIL_RESERVED_1288  0x40000
#define SIL_RESERVED_1291  19
#define SIL_RESERVED_1290  0x80000

#define SIL_RESERVED_1499  0x1013a8f0UL

#define SIL_RESERVED_1305  30
#define SIL_RESERVED_1304  0x40000000
#define SIL_RESERVED_1466  0x1a340450UL

#define SIL_RESERVED_1311  16
#define SIL_RESERVED_1310  0xf0000
#define SIL_RESERVED_1309  20
#define SIL_RESERVED_1308  0xf00000
#define SIL_RESERVED_1307  24
#define SIL_RESERVED_1306  0x1000000
#define SIL_RESERVED_1467  0x1a340440UL

#define SIL_RESERVED_1301  0
#define SIL_RESERVED_1300  0xff
#define SIL_RESERVED_1299  8
#define SIL_RESERVED_1298  0xff00
#define SIL_RESERVED_1502  0x1a38004cUL

#define SIL_RESERVED_1391  0
#define SIL_RESERVED_1390  0x3ff
#define SIL_RESERVED_1387  10
#define SIL_RESERVED_1386  0xffc00
#define SIL_RESERVED_1389  20
#define SIL_RESERVED_1388  0x3ff00000
#define SIL_RESERVED_1481  0x1a340404UL

#define SIL_RESERVED_1371  0
#define SIL_RESERVED_1370  0x1fffff
#define SIL_RESERVED_1478  0x1a3404d4UL

#define SIL_RESERVED_1367  0
#define SIL_RESERVED_1366  0x1fffff
#define SIL_RESERVED_1476  0x1a3404d8UL

#define SIL_RESERVED_1369  0
#define SIL_RESERVED_1368  0x1fffff
#define SIL_RESERVED_1477  0x1a3404dcUL

#define SIL_RESERVED_1486  0x1a34036cUL
#define SIL_RSVD_ADDR_13F00078  0x13f00078UL

#define SIL_RESERVED_1303  13
#define SIL_RESERVED_1302  0x2000
#define SIL_RSVD_ADDR_1A380008  0x1a380008UL

#define SIL_RESERVED_1401  22
#define SIL_RESERVED_1400  0x400000
#define SIL_RSVD_ADDR_1A380714  0x1a380714UL

#define SIL_RESERVED_1437  4
#define SIL_RESERVED_1436  0x10
#define SIL_RSVD_ADDR_1A380064  0x1a380064UL
