/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved. */

#ifndef MESON_COVERITY_H
#define MESON_COVERITY_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#ifndef COVERITY_GCC_VERSION_MIN
#define COVERITY_GCC_VERSION_MIN(x, y) 0
#define NOTSUPPORTED__COVERITY_GCC_VERSION_MIN__
#endif /* COVERITY_GCC_VERSION_MIN */

#ifdef __x86_64__
#if COVERITY_GCC_VERSION_AT_MIN(7, 0)
#if 0
typedef float             _Float128 __attribute__ ((__mode__(__TF__)));
typedef __complex__ float __cfloat128 __attribute__ ((__mode__(__TC__)));
typedef _Complex float    __cfloat128 __attribute__ ((__mode__(__TC__)));
#else
#include <unistd.h>
#define __cplusplus 201103L
#include <bits/floatn.h>
#undef __cplusplus
#endif
#endif
#endif

#ifdef NOTSUPPORTED__COVERITY_GCC_VERSION_MIN__
#undef NOTSUPPORTED__COVERITY_GCC_VERSION_MIN
#undef COVERITY_GCC_VERSION_MIN
#endif

#endif /* !MESON_COVERITY_H */
// vim:fenc=utf-8:tw=75
