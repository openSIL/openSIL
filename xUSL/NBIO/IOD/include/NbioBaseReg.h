/**
 *  @file NbioBaseReg.h
 *  @brief This file contains all Nbio Base Register definitions
 */

/* Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

#define SIL_RESERVED_122          0
#define SIL_RESERVED_121          0xf
#define SIL_RESERVED_124          4
#define SIL_RESERVED_123          0xf0
#define SIL_RESERVED_126          8
#define SIL_RESERVED_125          0xf00
#define SIL_RESERVED_128          12
#define SIL_RESERVED_127          0xf000
#define SIL_RESERVED_130          16
#define SIL_RESERVED_129          0xf0000
#define SIL_RESERVED_132          20
#define SIL_RESERVED_131          0xf00000
#define SIL_RESERVED_134          24
#define SIL_RESERVED_133          0xf000000
#define SIL_RESERVED_136          28
#define SIL_RESERVED_135          0xf0000000
#define SIL_RESERVED_536          0x13b1825cUL
#define SIL_RESERVED_220          21
#define SIL_RESERVED_219          0x200000
#define SIL_RESERVED_739          0x13b10004UL
#define SIL_RESERVED_769          0
#define SIL_RESERVED_768          0x1
#define SIL_RESERVED_747          0x13b3100cUL
#define SIL_RESERVED3_1490        6
#define SIL_RESERVED3_1491        0x40
#define SIL_RESERVED3_1492        12
#define SIL_RESERVED3_1493        0x1000
#define SIL_RESERVED3_1494        17
#define SIL_RESERVED3_1495        0x20000
#define SIL_RESERVED3_1496        0x1530001cUL
#define SIL_RESERVED3_1497        0x1470001cUL
#define SIL_RESERVED3_1498        0x14b0001cUL
#define SIL_RESERVED3_1499        21
#define SIL_RESERVED3_1500        0xe00000
#define SIL_RESERVED3_1501        0x15704330UL
#define SIL_RESERVED2_1084        16
#define SIL_RESERVED3_1502        0x10000
#define SIL_RESERVED3_1503        0x4000200UL
#define SIL_RESERVED_224          29
#define SIL_RESERVED_223          0x20000000
#define SIL_RESERVED_222          31
#define SIL_RESERVED_221          0x80000000
#define SIL_RESERVED_748          0x1a380080UL
#define SIL_RESERVED_226          17
#define SIL_RESERVED_225          0x20000
#define SIL_RESERVED_228          21
#define SIL_RESERVED_227          0x200000
#define SIL_RESERVED_749          0x1a380040UL
#define SIL_RESERVED_412          6
#define SIL_RESERVED_411          0x40
#define SIL_RESERVED_410          14
#define SIL_RESERVED_409          0xc000
#define SIL_RESERVED_751          0x1a380100UL
#define SIL_RESERVED_416          7
#define SIL_RESERVED_415          0x1f80
#define SIL_RESERVED_414          13
#define SIL_RESERVED_413          0xfe000
#define SIL_RESERVED_752          0x1a3804d8UL
#define SIL_RESERVED_418          19
#define SIL_RESERVED_417          0xf80000
#define SIL_RESERVED_753          0x1a3804dcUL
#define SIL_RESERVED_420          2
#define SIL_RESERVED_419          0x4
#define SIL_RESERVED_754          0x1a3804d4UL
#define SIL_RESERVED_424          5
#define SIL_RESERVED_423          0x20
#define SIL_RESERVED_422          6
#define SIL_RESERVED_421          0x40
#define SIL_RESERVED_426          18
#define SIL_RESERVED_425          0x40000
#define SIL_RESERVED_428          20
#define SIL_RESERVED_427          0x100000
#define SIL_RESERVED_755          0x1a3802c0UL
#define SIL_RESERVED_430          24
#define SIL_RESERVED_429          0x1000000
#define SIL_RESERVED_432          25
#define SIL_RESERVED_431          0x2000000
#define SIL_RESERVED_756          0x1a380620UL
#define SIL_RESERVED_434          26
#define SIL_RESERVED_433          0x4000000
#define SIL_RESERVED_757          0x1a380628UL
#define SIL_RESERVED2_1094        13
#define SIL_RESERVED2_1092        0x2000
#define SIL_RESERVED_532          0x1a340008UL
#define SIL_RESERVED_441          15
#define SIL_RESERVED_440          0x8000
#define SIL_RESERVED_533          0x1a34000cUL
#define SIL_RESERVED_443          8
#define SIL_RESERVED_442          0x700
#define SIL_RESERVED_534          0x1a340300UL

typedef union {
  struct {
    uint32_t                            LC_ADVERTISE_MODIFIED_TS_OS_SUPPORT:1;
    uint32_t                            LC_MODIFIED_TS_OS_SUPPORT_RCVD:1;
    uint32_t                            LC_MODIFIED_TS_SENT:1;
    uint32_t                            LC_MODIFIED_TS_RCVD:1;
    uint32_t                            LC_ALTERNATE_PROTOCOL_NEGOTIATION_STATE0:2;
    uint32_t                            LC_ALTERNATE_PROTOCOL_NEGOTIATION_STATE1:2;
    uint32_t                            LC_ALTERNATE_PROTOCOL_NEGOTIATION_STATE2:2;
    uint32_t                            LC_ALTERNATE_PROTOCOL_NEGOTIATION_STATE3:2;
    uint32_t                            LC_ALTERNATE_PROTOCOL_NEGOTIATION_STATUS:2;
    uint32_t                            LC_ALTERNATE_PROTOCOL_RESPONSE_TIME_LIMIT:2;
    uint32_t                            LC_ALTERNATE_PROTOCOL_COUNT:8;
    uint32_t                            LC_MODIFIED_TS_USAGE_MODE_1_SUPPORTED:1;
    uint32_t                            LC_MODIFIED_TS_USAGE_MODE_2_SUPPORTED:1;
    uint32_t                            LC_ALTERNATE_PROTOCOL_SELECTIVE_ENABLE_SUPPORTED:1;
    uint32_t                            LC_ALTERNATE_PROTOCOL_CXL_PCIE_ONLY_NEG_MODE:1;
    uint32_t                            LC_ALTERNATE_PROTOCOL_CHECK_COMMON_CLOCK:1;
    uint32_t                            LC_ALTERNATE_PROTOCOL_CHECK_RTM_CXL_AWARE:1;
    uint32_t                            LC_ALTERNATE_PROTOCOL_ABORT_RCVD_USAGE_MODE_000:1;
    uint32_t                            LC_CXL_COMMON_CLOCK_IN_MODTS2:1;
  } Field;
  uint32_t Value;
} SIL_RESERVED_4;
#define SIL_RESERVED_509          0x1a340424UL
#define SIL_RESERVED_242          23
#define SIL_RESERVED_241          0x800000
#define SIL_RESERVED_512          0x1a340280UL
#define SIL_RESERVED_246          8
#define SIL_RESERVED_245          0x100
#define SIL_RESERVED_244          14
#define SIL_RESERVED_243          0xc000
#define SIL_RESERVED_513          0x1a3402c4UL
#define SIL_RESERVED3_1504        12
#define SIL_RESERVED3_1505        0x1000
#define SIL_RESERVED3_1506        14
#define SIL_RESERVED3_1507        0x4000
#define SIL_RESERVED_514          0x1a3402d4UL
#define SIL_RESERVED_248          4
#define SIL_RESERVED_247          0x10
#define SIL_RESERVED_515          0x1a3402d8UL
#define SIL_RESERVED_250          0
#define SIL_RESERVED_249          0x3
#define SIL_RESERVED_252          2
#define SIL_RESERVED_251          0xc
#define SIL_RESERVED_254          4
#define SIL_RESERVED_253          0x30
#define SIL_RESERVED_260          12
#define SIL_RESERVED_255          20
#define SIL_RESERVED_257          21
#define SIL_RESERVED_256          0x600000
#define SIL_RESERVED_259          23
#define SIL_RESERVED_258          0x1800000
#define SIL_RESERVED_516          0x1a3402ecUL
#define SIL_RESERVED_262          0
#define SIL_RESERVED_261          0x1
#define SIL_RESERVED_264          2
#define SIL_RESERVED_263          0xc
#define SIL_RESERVED_270          6
#define SIL_RESERVED_269          0x40
#define SIL_RESERVED_266          10
#define SIL_RESERVED_265          0x400
#define SIL_RESERVED_268          11
#define SIL_RESERVED_267          0x7800
#define SIL_RESERVED_517          0x1a340394UL
#define SIL_RESERVED_272          0
#define SIL_RESERVED_271          0x1
#define SIL_RESERVED_274          2
#define SIL_RESERVED_273          0xc
#define SIL_RESERVED_280          6
#define SIL_RESERVED_279          0x40
#define SIL_RESERVED_276          10
#define SIL_RESERVED_275          0x400
#define SIL_RESERVED_278          11
#define SIL_RESERVED_277          0x7800
#define SIL_RESERVED_518          0x1a340400UL
#define SIL_RESERVED_282          0
#define SIL_RESERVED_281          0x1
#define SIL_RESERVED_284          2
#define SIL_RESERVED_283          0xc
#define SIL_RESERVED_290          6
#define SIL_RESERVED_289          0x40
#define SIL_RESERVED_286          10
#define SIL_RESERVED_285          0x400
#define SIL_RESERVED_288          11
#define SIL_RESERVED_287          0x7800
#define SIL_RESERVED_519          0x1a340390UL
#define SIL_RESERVED_382          16
#define SIL_RESERVED_381          0x70000
#define SIL_RESERVED_384          20
#define SIL_RESERVED_383          0x700000
#define SIL_RESERVED_523          0x1a340318UL
#define SIL_RESERVED_390          10
#define SIL_RESERVED_389          0x400
#define SIL_RESERVED_386          19
#define SIL_RESERVED_385          0x80000
#define SIL_RESERVED_388          21
#define SIL_RESERVED_387          0x600000
#define SIL_RESERVED_392          30
#define SIL_RESERVED_391          0x40000000
#define SIL_RESERVED_524          0x1a340288UL
#define SIL_RESERVED_403          0x1
#define SIL_RESERVED_404          0x2
#define SIL_RESERVED_405          0x4
#define SIL_RESERVED_406          0x8
#define SIL_RESERVED_527          0x1a340290UL
#define SIL_RESERVED3_1508        12
#define SIL_RESERVED3_1509        0x1000
#define SIL_RESERVED_528          0x1a340414UL
#define SIL_RESERVED_529          0x1a340294UL
#define SIL_RESERVED_408          6
#define SIL_RESERVED_407          0x40
#define SIL_RESERVED_530          0x1a340284UL

typedef union {
  struct {
    uint32_t                            TX_REQUESTER_ID_FUNCTION:3;
    uint32_t                            TX_REQUESTER_ID_DEVICE:5;
    uint32_t                            TX_REQUESTER_ID_BUS:8;
    uint32_t                            TX_SWUS_REQUESTER_ID_FUNCTION:3;
    uint32_t                            TX_SWUS_REQUESTER_ID_DEVICE:5;
    uint32_t                            TX_SWUS_REQUESTER_ID_BUS:8;
  } Field;
  uint32_t Value;
} SIL_RESERVED_5;

#define SIL_RESERVED_531         0x1a340084UL
#define SIL_RESERVED2_964        0x80000
#define SIL_RESERVED2_965        0x100000
#define SIL_RESERVED2_966        0x200000
#define SIL_RESERVED2_967        0x400000
#define SIL_RESERVED2_960        0x800000
#define SIL_RESERVED2_961        0x1000000
#define SIL_RESERVED2_962        0x2000000
#define SIL_RESERVED2_963        0x4000000
#define SIL_RESERVED2_1311       0x5d284UL

typedef union {
  struct {
    uint32_t                            ThreadEn;
  } Field;
  uint32_t Value;
} SIL_RESERVED3_1510;

#define SIL_RESERVED2_1293        0x30081018UL
#define SIL_RESERVED2_1294        0x4a081018UL
#define SIL_RESERVED3_1511        0
#define SIL_RESERVED3_1512        0x1
#define SIL_RESERVED3_1513        1
#define SIL_RESERVED2_969         0x2
#define SIL_RESERVED3_1514        2
#define SIL_RESERVED2_971         0x4
#define SIL_RESERVED3_1515        3
#define SIL_RESERVED2_973         0x8
#define SIL_RESERVED3_1516        4
#define SIL_RESERVED3_1517        0x10
#define SIL_RESERVED3_1518        5
#define SIL_RESERVED2_976         0x20
#define SIL_RESERVED3_1519        6
#define SIL_RESERVED2_978         0x40
#define SIL_RESERVED2_981         7
#define SIL_RESERVED2_980         0x80
#define SIL_RESERVED2_983         8
#define SIL_RESERVED2_982         0x100
#define SIL_RESERVED2_985         9
#define SIL_RESERVED2_984         0x200
#define SIL_RESERVED3_1520        10
#define SIL_RESERVED2_986         0x400
#define SIL_RESERVED3_1521        11
#define SIL_RESERVED2_987         0x800
#define SIL_RESERVED2_989         12
#define SIL_RESERVED2_988         0x1000
#define SIL_RESERVED2_991         13
#define SIL_RESERVED2_990         0x2000
#define SIL_RESERVED2_993         14
#define SIL_RESERVED2_992         0x4000
#define SIL_RESERVED3_1522        15
#define SIL_RESERVED2_994         0x8000
#define SIL_RESERVED2_996         16
#define SIL_RESERVED2_995         0x10000
#define SIL_RESERVED2_998         17
#define SIL_RESERVED2_997         0x20000
#define SIL_RESERVED2_1000        18
#define SIL_RESERVED2_999         0x40000
#define SIL_RESERVED2_1001        19
#define SIL_RESERVED3_1523        0x80000
#define SIL_RESERVED2_1317        0x141e8f0UL
#define SIL_RESERVED2_1034        0
#define SIL_RESERVED2_1033        0x1
#define SIL_RESERVED2_1036        1
#define SIL_RESERVED2_1035        0x2
#define SIL_RESERVED2_1037        2
#define SIL_RESERVED3_1524        0x4
#define SIL_RESERVED3_1525        3
#define SIL_RESERVED2_1038        0x8
#define SIL_RESERVED2_1039        4
#define SIL_RESERVED3_1526        0x10
#define SIL_RESERVED3_1527        5
#define SIL_RESERVED3_1528        0x20
#define SIL_RESERVED2_1041        6
#define SIL_RESERVED2_1040        0x40
#define SIL_RESERVED2_1043        7
#define SIL_RESERVED2_1042        0x80
#define SIL_RESERVED2_1045        8
#define SIL_RESERVED2_1044        0x100
#define SIL_RESERVED2_1047        9
#define SIL_RESERVED2_1046        0x200
#define SIL_RESERVED2_1048        10
#define SIL_RESERVED3_1529        0x400
#define SIL_RESERVED2_1050        11
#define SIL_RESERVED2_1049        0x800
#define SIL_RESERVED2_1051        12
#define SIL_RESERVED3_1530        0x1000
#define SIL_RESERVED2_1052        13
#define SIL_RESERVED3_1531        0x2000
#define SIL_RESERVED3_1532        14
#define SIL_RESERVED3_1533        0x4000
#define SIL_RESERVED2_1053        15
#define SIL_RESERVED3_1534        0x8000
#define SIL_RESERVED3_1535        16
#define SIL_RESERVED2_1054        0x10000
#define SIL_RESERVED2_1055        17
#define SIL_RESERVED3_1536        0x20000
#define SIL_RESERVED2_1057        18
#define SIL_RESERVED2_1056        0x40000
#define SIL_RESERVED3_1537        19
#define SIL_RESERVED2_1058        0x80000
#define SIL_RESERVED2_1321        0x1013a8f0UL
#define SIL_RESERVED2_1258        0
#define SIL_RESERVED2_1257        0x1
#define SIL_RESERVED3_1538        1
#define SIL_RESERVED2_1259        0x2
#define SIL_RESERVED2_1261        2
#define SIL_RESERVED2_1260        0x4
#define SIL_RESERVED2_1263        3
#define SIL_RESERVED2_1262        0x8
#define SIL_RESERVED2_1265        4
#define SIL_RESERVED2_1264        0x10
#define SIL_RESERVED2_1267        5
#define SIL_RESERVED2_1266        0x20
#define SIL_RESERVED2_1269        6
#define SIL_RESERVED2_1268        0x40
#define SIL_RESERVED2_1270        7
#define SIL_RESERVED3_1539        0x80
#define SIL_RESERVED2_1272        8
#define SIL_RESERVED2_1271        0x100
#define SIL_RESERVED2_1274        9
#define SIL_RESERVED2_1273        0x200
#define SIL_RESERVED2_1276        10
#define SIL_RESERVED2_1275        0x400
#define SIL_RESERVED2_1278        11
#define SIL_RESERVED2_1277        0x800
#define SIL_RESERVED2_1280        12
#define SIL_RESERVED2_1279        0x1000
#define SIL_RESERVED2_1281        13
#define SIL_RESERVED3_1540        0x2000
#define SIL_RESERVED2_1283        14
#define SIL_RESERVED2_1282        0x4000
#define SIL_RESERVED2_1285        15
#define SIL_RESERVED2_1284        0x8000
#define SIL_RESERVED2_1287        16
#define SIL_RESERVED2_1286        0x10000
#define SIL_RESERVED2_1288        17
#define SIL_RESERVED3_1541        0x20000
#define SIL_RESERVED2_1290        18
#define SIL_RESERVED2_1289        0x40000
#define SIL_RESERVED2_1292        19
#define SIL_RESERVED2_1291        0x80000
#define SIL_RESERVED3_1542        0x141e190UL
#define SIL_RESERVED2_1416        0x151e140UL
#define SIL_RESERVED3_1543        30
#define SIL_RESERVED3_1544        0x40000000
#define SIL_RESERVED3_1545        0x1a340450UL
#define SIL_RESERVED_238          16
#define SIL_RESERVED_237          0xf0000
#define SIL_RESERVED_236          20
#define SIL_RESERVED_235          0xf00000
#define SIL_RESERVED_234          24
#define SIL_RESERVED_233          0x1000000
#define SIL_RESERVED_510          0x1a340440UL
#define SIL_RESERVED_439          8
#define SIL_RESERVED_438          0x100
#define SIL_RESERVED_437          10
#define SIL_RESERVED_436          0x7c00
#define SIL_RESERVED_232          0
#define SIL_RESERVED_231          0xff
#define SIL_RESERVED_230          8
#define SIL_RESERVED_229          0xff00
#define SIL_RESERVED_750          0x1a38004cUL
#define SIL_RESERVED_402          0
#define SIL_RESERVED_401          0x3ff
#define SIL_RESERVED_398          10
#define SIL_RESERVED_397          0xffc00
#define SIL_RESERVED_400          20
#define SIL_RESERVED_399          0x3ff00000
#define SIL_RESERVED_526          0x1a340404UL
#define SIL_RESERVED_298          0
#define SIL_RESERVED_297          0x1f
#define SIL_RESERVED_304          5
#define SIL_RESERVED_303          0x1e0
#define SIL_RESERVED_300          10
#define SIL_RESERVED_299          0x7c00
#define SIL_RESERVED_306          15
#define SIL_RESERVED_305          0x78000
#define SIL_RESERVED_302          20
#define SIL_RESERVED_301          0x1f00000
#define SIL_RESERVED_308          25
#define SIL_RESERVED_307          0x1e000000
#define SIL_RESERVED_492          0x1a3404e0UL
#define SIL_RESERVED_310          0
#define SIL_RESERVED_309          0x1f
#define SIL_RESERVED_316          5
#define SIL_RESERVED_315          0x1e0
#define SIL_RESERVED_312          10
#define SIL_RESERVED_311          0x7c00
#define SIL_RESERVED_318          15
#define SIL_RESERVED_317          0x78000
#define SIL_RESERVED_314          20
#define SIL_RESERVED_313          0x1f00000
#define SIL_RESERVED_320          25
#define SIL_RESERVED_319          0x1e000000
#define SIL_RESERVED_493          0x1a3404e4UL
#define SIL_RESERVED_322          0
#define SIL_RESERVED_321          0x1f
#define SIL_RESERVED_328          5
#define SIL_RESERVED_327          0x1e0
#define SIL_RESERVED_324          10
#define SIL_RESERVED_323          0x7c00
#define SIL_RESERVED_330          15
#define SIL_RESERVED_329          0x78000
#define SIL_RESERVED_326          20
#define SIL_RESERVED_325          0x1f00000
#define SIL_RESERVED_332          25
#define SIL_RESERVED_331          0x1e000000
#define SIL_RESERVED_494          0x1a3404e8UL
#define SIL_RESERVED_338          0
#define SIL_RESERVED_337          0x1f
#define SIL_RESERVED_344          5
#define SIL_RESERVED_343          0x1e0
#define SIL_RESERVED_334          10
#define SIL_RESERVED_333          0x7c00
#define SIL_RESERVED_340          15
#define SIL_RESERVED_339          0x78000
#define SIL_RESERVED_336          20
#define SIL_RESERVED_335          0x1f00000
#define SIL_RESERVED_342          25
#define SIL_RESERVED_341          0x1e000000
#define SIL_RESERVED_495          0x1a3404ecUL
#define SIL_RESERVED_346          0
#define SIL_RESERVED_345          0x1f
#define SIL_RESERVED_352          5
#define SIL_RESERVED_351          0x1e0
#define SIL_RESERVED_348          10
#define SIL_RESERVED_347          0x7c00
#define SIL_RESERVED_354          15
#define SIL_RESERVED_353          0x78000
#define SIL_RESERVED_350          20
#define SIL_RESERVED_349          0x1f00000
#define SIL_RESERVED_356          25
#define SIL_RESERVED_355          0x1e000000
#define SIL_RESERVED_496          0x1a3404f0UL
#define SIL_RESERVED_358          0
#define SIL_RESERVED_357          0x1f
#define SIL_RESERVED_364          5
#define SIL_RESERVED_363          0x1e0
#define SIL_RESERVED_360          10
#define SIL_RESERVED_359          0x7c00
#define SIL_RESERVED_366          15
#define SIL_RESERVED_365          0x78000
#define SIL_RESERVED_362          20
#define SIL_RESERVED_361          0x1f00000
#define SIL_RESERVED_368          25
#define SIL_RESERVED_367          0x1e000000
#define SIL_RESERVED_497          0x1a3404f4UL
#define SIL_RESERVED_370          0
#define SIL_RESERVED_369          0x1f
#define SIL_RESERVED_376          5
#define SIL_RESERVED_375          0x1e0
#define SIL_RESERVED_372          10
#define SIL_RESERVED_371          0x7c00
#define SIL_RESERVED_378          15
#define SIL_RESERVED_377          0x78000
#define SIL_RESERVED_374          20
#define SIL_RESERVED_373          0x1f00000
#define SIL_RESERVED_380          25
#define SIL_RESERVED_379          0x1e000000
#define SIL_RESERVED_498          0x1a3404f8UL
#define SIL_RESERVED_296          0
#define SIL_RESERVED_295          0x1fffff
#define SIL_RESERVED_522          0x1a3404d4UL
#define SIL_RESERVED_292          0
#define SIL_RESERVED_291          0x1fffff
#define SIL_RESERVED_520          0x1a3404d8UL
#define SIL_RESERVED_294          0
#define SIL_RESERVED_293          0x1fffff
#define SIL_RESERVED_521          0x1a3404dcUL
