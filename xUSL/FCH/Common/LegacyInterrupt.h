/**
 * @file LegacyInterrupt.h
 * @brief Legacy Interrupt helper functions header file.
 */
/* Copyright 2021-2023 Advanced Micro Devices, Inc. All rights reserved.    */
// SPDX-License-Identifier: MIT

#pragma once

SIL_STATUS GetNumberPirqs (uint8_t *NumberPirqs);
SIL_STATUS ReadPirq (uint8_t PirqNumber, uint8_t *PirqData);
SIL_STATUS WritePirq (uint8_t PirqNumber, uint8_t PirqData);
void InitializelLegacyInterrupt (void);
