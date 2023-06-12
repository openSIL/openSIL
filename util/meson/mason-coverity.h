/* SPDX-License-Identifier: MIT */
/*
 * Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef MASON_COVERITY_H
#define MASON_COVERITY_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#ifndef COVERITY_GCC_VERSION_MIN
#define COVERITY_GCC_VERSION_MIN(x, y) 0
#define NOTSUPPORTED__COVERITY_GCC_VERSION_MIN__
#endif /* COVERITY_GCC_VERSION_MIN */

#ifdef __x86_64__
#if COVERITY_GCC_VERSION_AT_MIN (7, 0)
#if 0
typedef float             _Float128 __attribute__((__mode__ (__TF__)));
typedef __complex__ float __cfloat128 __attribute__((__mode__ (__TC__)));
typedef _Complex float    __cfloat128 __attribute__((__mode__ (__TC__)));
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

#endif /* !MASON_COVERITY_H */
// vim:fenc=utf-8:tw=75
