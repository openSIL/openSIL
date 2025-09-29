/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  FabricRcManagerPhx.h
 * @brief Fabric MMIO/IO type definitions
 *
 */
#pragma once

#include <ProjSocConst.h>

#define PHX_MAX_HOST_BRIDGES_PER_DIE      1   ///< Max host bridges per die
#define PHX_MAX_DIES_PER_SOCKET           1   ///< Max number of dies per socket
#define PHX_MAX_HOST_BRIDGES_PER_SOCKET   (PHX_MAX_DIES_PER_SOCKET * PHX_MAX_HOST_BRIDGES_PER_DIE)
