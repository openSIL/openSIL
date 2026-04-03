/* SPDX-License-Identifier: MIT */
/* Copyright (C) 2021 - 2025 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  SmuPhx.h
 * @brief OpenSIL SMU PHX specific initialization
 *
 */

#pragma once

#include <SMU/SmuClass-api.h>

#define FEATURE_CCLK_DPM_BIT                 0
#define FEATURE_FAN_CONTROLLER_BIT           1
#define FEATURE_DATA_CALCULATION_BIT         2
#define FEATURE_PPT_BIT                      3
#define FEATURE_TDC_BIT                      4
#define FEATURE_THERMAL_BIT                  5
#define FEATURE_FIT_BIT                      6
#define FEATURE_EDC_BIT                      7
#define FEATURE_PLL_POWER_DOWN_BIT           8
#define FEATURE_VDDOFF_BIT                   9
#define FEATURE_VCN_DPM_BIT                 10
#define FEATURE_CSTATE_BOOST_BIT            11
#define FEATURE_FCLK_DPM_BIT                12
#define FEATURE_SOCCLK_DPM_BIT              13
#define FEATURE_MP0CLK_DPM_BIT              14
#define FEATURE_LCLK_DPM_BIT                15
#define FEATURE_SHUBCLK_DPM_BIT             16
#define FEATURE_DCFCLK_DPM_BIT              17
#define FEATURE_ISP_DPM_BIT                 18
#define FEATURE_IPU_DPM_BIT                 19
#define FEATURE_GFX_DPM_BIT                 20
#define FEATURE_DS_GFXCLK_BIT               21
#define FEATURE_DS_SOCCLK_BIT               22
#define FEATURE_DS_LCLK_BIT                 23
#define FEATURE_DS_DCFCLK_BIT               24
#define FEATURE_DS_SHUBCLK_BIT              25
#define FEATURE_GFX_TEMP_VMIN_BIT           26
#define FEATURE_ZSTATES_BIT                 27
#define FEATURE_WHISPER_MODE_BIT            28
#define FEATURE_DS_FCLK_BIT                 29
#define FEATURE_DS_SMNCLK_BIT               30
#define FEATURE_DS_MP1CLK_BIT               31
#define FEATURE_DS_MP0CLK_BIT               32
#define FEATURE_SMU_LOW_POWER_BIT           33
#define FEATURE_FUSE_PG_BIT                 34
#define FEATURE_GFX_DEM_BIT                 35
#define FEATURE_PSI_BIT                     36
#define FEATURE_PROCHOT_BIT                 37
#define FEATURE_CPUOFF_BIT                  38
#define FEATURE_STAPM_BIT                   39
#define FEATURE_S0I3_BIT                    40
#define FEATURE_DF_LIGHT_CSTATE             41   // shift the order or DFCstate annd DF light Cstate
#define FEATURE_PERF_LIMIT_BIT              42
#define FEATURE_CORE_DLDO_BIT               43
#define FEATURE_DVO_BIT                     44
#define FEATURE_DS_VCN_BIT                  45
#define FEATURE_CPPC_BIT                    46
#define FEATURE_CPPC_PREFERRED_CORES_BIT    47
#define FEATURE_DF_CSTATES_BIT              48
#define FEATURE_FASTBYPASS_CLDO_BIT         49
#define FEATURE_ATHUB_PG_BIT                50
#define FEATURE_VDDOFF_ECO_BIT              51
#define FEATURE_ZSTATES_ECO_BIT             52
#define FEATURE_CC6_BIT                     53
#define FEATURE_DS_UMCCLK_BIT               54
#define FEATURE_DS_ISPCLK_BIT               55
#define FEATURE_DS_HSPCLK_BIT               56
#define FEATURE_MPCCX_WHISPER_MODE_BIT      57
#define FEATURE_DS_IPUCLK_BIT               58
#define FEATURE_MSMU_LOW_POWER_BIT          59
#define NUM_FEATURES                        60


#define FEATURE_CCLK_DPM_MASK            (1 << FEATURE_CCLK_DPM_BIT            ) // 0
#define FEATURE_FAN_CONTROLLER_MASK      (1 << FEATURE_FAN_CONTROLLER_BIT      ) // 1
#define FEATURE_DATA_CALCULATION_MASK    (1 << FEATURE_DATA_CALCULATION_BIT    ) // 2
#define FEATURE_PPT_MASK                 (1 << FEATURE_PPT_BIT                 ) // 3
#define FEATURE_TDC_MASK                 (1 << FEATURE_TDC_BIT                 ) // 4
#define FEATURE_THERMAL_MASK             (1 << FEATURE_THERMAL_BIT             ) // 5
#define FEATURE_FIT_MASK                 (1 << FEATURE_FIT_BIT                 ) // 6
#define FEATURE_EDC_MASK                 (1 << FEATURE_EDC_BIT                 ) // 7
#define FEATURE_PLL_POWER_DOWN_MASK      (1 << FEATURE_PLL_POWER_DOWN_BIT      ) // 8
#define FEATURE_VDDOFF_MASK              (1 << FEATURE_VDDOFF_BIT              ) // 9
#define FEATURE_VCN_DPM_MASK             (1 << FEATURE_VCN_DPM_BIT             ) // 10
#define FEATURE_CSTATE_BOOST_MASK        (1 << FEATURE_CSTATE_BOOST_BIT        ) // 11
#define FEATURE_FCLK_DPM_MASK            (1 << FEATURE_FCLK_DPM_BIT            ) // 12
#define FEATURE_SOCCLK_DPM_MASK          (1 << FEATURE_SOCCLK_DPM_BIT          ) // 13
#define FEATURE_MP0CLK_DPM_MASK          (1 << FEATURE_MP0CLK_DPM_BIT          ) // 14
#define FEATURE_LCLK_DPM_MASK            (1 << FEATURE_LCLK_DPM_BIT            ) // 15
#define FEATURE_SHUBCLK_DPM_MASK         (1 << FEATURE_SHUBCLK_DPM_BIT         ) // 16
#define FEATURE_DCFCLK_DPM_MASK          (1 << FEATURE_DCFCLK_DPM_BIT          ) // 17
#define FEATURE_ISP_DPM_MASK             (1 << FEATURE_ISP_DPM_BIT             ) // 18
#define FEATURE_IPU_DPM_MASK             (1 << FEATURE_IPU_DPM_BIT             ) // 19
#define FEATURE_GFX_DPM_MASK             (1 << FEATURE_GFX_DPM_BIT             ) // 20
#define FEATURE_DS_GFXCLK_MASK           (1 << FEATURE_DS_GFXCLK_BIT           ) // 21
#define FEATURE_DS_SOCCLK_MASK           (1 << FEATURE_DS_SOCCLK_BIT           ) // 22
#define FEATURE_DS_LCLK_MASK             (1 << FEATURE_DS_LCLK_BIT             ) // 23
#define FEATURE_DS_DCFCLK_MASK           (1 << FEATURE_DS_DCFCLK_BIT           ) // 24
#define FEATURE_DS_SHUBCLK_MASK          (1 << FEATURE_DS_SHUBCLK_BIT          ) // 25
#define FEATURE_GFX_TEMP_VMIN_MASK       (1 << FEATURE_GFX_TEMP_VMIN_BIT       ) // 26
#define FEATURE_ZSTATES_MASK             (1 << FEATURE_ZSTATES_BIT             ) // 27
#define FEATURE_WHISPER_MODE_MASK        (1 << FEATURE_WHISPER_MODE_BIT        ) // 28
#define FEATURE_DS_FCLK_MASK             (1 << FEATURE_DS_FCLK_BIT             ) // 29
#define FEATURE_DS_SMNCLK_MASK           (1 << FEATURE_DS_SMNCLK_BIT           ) // 30
#define FEATURE_DS_MP1CLK_MASK           (1 << FEATURE_DS_MP1CLK_BIT           ) // 31

#define FEATURE_DS_MP0CLK_MASK           (1 << (FEATURE_DS_MP0CLK_BIT           - 32)) // 32
#define FEATURE_SMU_LOW_POWER_MASK       (1 << (FEATURE_SMU_LOW_POWER_BIT       - 32)) // 33
#define FEATURE_FUSE_PG_MASK             (1 << (FEATURE_FUSE_PG_BIT             - 32)) // 34
#define FEATURE_GFX_DEM_MASK             (1 << (FEATURE_GFX_DEM_BIT             - 32)) // 35
#define FEATURE_PSI_MASK                 (1 << (FEATURE_PSI_BIT                 - 32)) // 36
#define FEATURE_PROCHOT_MASK             (1 << (FEATURE_PROCHOT_BIT             - 32)) // 37
#define FEATURE_CPUOFF_MASK              (1 << (FEATURE_CPUOFF_BIT              - 32)) // 38
#define FEATURE_STAPM_MASK               (1 << (FEATURE_STAPM_BIT               - 32)) // 39
#define FEATURE_S0I3_MASK                (1 << (FEATURE_S0I3_BIT                - 32)) // 40
#define FEATURE_DF_LIGHT_CSTATE_MASK     (1 << (FEATURE_DF_LIGHT_CSTATE         - 32)) // 41
#define FEATURE_PERF_LIMIT_MASK          (1 << (FEATURE_PERF_LIMIT_BIT          - 32)) // 42
#define FEATURE_CORE_DLDO_MASK           (1 << (FEATURE_CORE_DLDO_BIT           - 32)) // 43
#define FEATURE_DVO_MASK                 (1 << (FEATURE_DVO_BIT                 - 32)) // 44
#define FEATURE_DS_VCN_MASK              (1 << (FEATURE_DS_VCN_BIT              - 32)) // 45
#define FEATURE_CPPC_MASK                (1 << (FEATURE_CPPC_BIT                - 32)) // 46
#define FEATURE_CPPC_PREFERRED_CORES_MASK (1 << (FEATURE_CPPC_PREFERRED_CORES_BIT    - 32)) // 47
#define FEATURE_DF_CSTATES_MASK          (1 << (FEATURE_DF_CSTATES_BIT          - 32)) // 48
#define FEATURE_FASTBYPASS_CLDO_MASK     (1 << (FEATURE_FASTBYPASS_CLDO_BIT     - 32)) // 49
#define FEATURE_ATHUB_PG_MASK            (1 << (FEATURE_ATHUB_PG_BIT            - 32)) // 50
#define FEATURE_VDDOFF_ECO_MASK          (1 << (FEATURE_VDDOFF_ECO_BIT          - 32)) // 51
#define FEATURE_ZSTATES_ECO_MASK         (1 << (FEATURE_ZSTATES_ECO_BIT         - 32)) // 52
#define FEATURE_CC6_MASK                 (1 << (FEATURE_CC6_BIT                 - 32)) // 53
#define FEATURE_DS_UMCCLK_MASK           (1 << (FEATURE_DS_UMCCLK_BIT           - 32)) // 54
#define FEATURE_DS_ISPCLK_MASK           (1 << (FEATURE_DS_ISPCLK_BIT           - 32)) // 55
#define FEATURE_DS_HSPCLK_MASK           (1 << (FEATURE_DS_HSPCLK_BIT           - 32)) // 56
#define FEATURE_MPCCX_WHISPER_MODE_MASK  (1 << (FEATURE_MPCCX_WHISPER_MODE_BIT  - 32)) // 57
#define FEATURE_DS_IPUCLK_MASK           (1 << (FEATURE_DS_IPUCLK_BIT           - 32)) // 58
#define FEATURE_MSMU_LOW_POWER_MASK      (1 << (FEATURE_MSMU_LOW_POWER_BIT      - 32)) // 59


#define FP8_SMU_FEATURES_ENABLE_DEFAULT ( \
  (1 << (FEATURE_CCLK_DPM_BIT            %32)) | \
  (1 << (FEATURE_FAN_CONTROLLER_BIT      %32)) | \
  (1 << (FEATURE_DATA_CALCULATION_BIT    %32)) | \
  (1 << (FEATURE_PPT_BIT                 %32)) | \
  (1 << (FEATURE_TDC_BIT                 %32)) | \
  (1 << (FEATURE_THERMAL_BIT             %32)) | \
  (1 << (FEATURE_FIT_BIT                 %32)) | \
  (1 << (FEATURE_EDC_BIT                 %32)) | \
  (1 << (FEATURE_PLL_POWER_DOWN_BIT      %32)) | \
  (1 << (FEATURE_VDDOFF_BIT              %32)) | \
  (1 << (FEATURE_VCN_DPM_BIT             %32)) | \
  (1 << (FEATURE_CSTATE_BOOST_BIT        %32)) | \
  (1 << (FEATURE_FCLK_DPM_BIT            %32)) | \
  (1 << (FEATURE_SOCCLK_DPM_BIT          %32)) | \
  (1 << (FEATURE_MP0CLK_DPM_BIT          %32)) | \
  (1 << (FEATURE_LCLK_DPM_BIT            %32)) | \
  (1 << (FEATURE_SHUBCLK_DPM_BIT         %32)) | \
  (1 << (FEATURE_DCFCLK_DPM_BIT          %32)) | \
  (0 << (FEATURE_ISP_DPM_BIT             %32)) | \
  (1 << (FEATURE_IPU_DPM_BIT             %32)) | \
  (0 << (FEATURE_GFX_DPM_BIT             %32)) | \
  (1 << (FEATURE_DS_GFXCLK_BIT           %32)) | \
  (1 << (FEATURE_DS_SOCCLK_BIT           %32)) | \
  (1 << (FEATURE_DS_LCLK_BIT             %32)) | \
  (1 << (FEATURE_DS_DCFCLK_BIT           %32)) | \
  (1 << (FEATURE_DS_SHUBCLK_BIT          %32)) | \
  (0 << (FEATURE_GFX_TEMP_VMIN_BIT       %32)) | \
  (1 << (FEATURE_ZSTATES_BIT             %32)) | \
  (1 << (FEATURE_WHISPER_MODE_BIT        %32)) | \
  (1 << (FEATURE_DS_FCLK_BIT             %32)) | \
  (1 << (FEATURE_DS_SMNCLK_BIT           %32)) | \
  (1 << (FEATURE_DS_MP1CLK_BIT           %32)) \
)

#define FP8_SMU_FEATURES_ENABLE_EXT_DEFAULT ( \
  (1 << (FEATURE_DS_MP0CLK_BIT           %32)) | \
  (0 << (FEATURE_SMU_LOW_POWER_BIT       %32)) | \
  (0 << (FEATURE_FUSE_PG_BIT             %32)) | \
  (0 << (FEATURE_GFX_DEM_BIT             %32)) | \
  (1 << (FEATURE_PSI_BIT                 %32)) | \
  (1 << (FEATURE_PROCHOT_BIT             %32)) | \
  (1 << (FEATURE_CPUOFF_BIT              %32)) | \
  (1 << (FEATURE_STAPM_BIT               %32)) | \
  (0 << (FEATURE_S0I3_BIT                %32)) | \
  (1 << (FEATURE_DF_LIGHT_CSTATE         %32)) | \
  (0 << (FEATURE_PERF_LIMIT_BIT          %32)) | \
  (0 << (FEATURE_CORE_DLDO_BIT           %32)) | \
  (1 << (FEATURE_DVO_BIT                 %32)) | \
  (1 << (FEATURE_DS_VCN_BIT              %32)) | \
  (1 << (FEATURE_CPPC_BIT                %32)) | \
  (1 << (FEATURE_CPPC_PREFERRED_CORES_BIT  %32)) | \
  (1 << (FEATURE_DF_CSTATES_BIT          %32)) | \
  (0 << (FEATURE_FASTBYPASS_CLDO_BIT     %32)) | \
  (1 << (FEATURE_ATHUB_PG_BIT            %32)) | \
  (0 << (FEATURE_VDDOFF_ECO_BIT          %32)) | \
  (0 << (FEATURE_ZSTATES_ECO_BIT         %32)) | \
  (1 << (FEATURE_CC6_BIT                 %32)) | \
  (0 << (FEATURE_DS_UMCCLK_BIT           %32)) | \
  (1 << (FEATURE_DS_ISPCLK_BIT           %32)) | \
  (1 << (FEATURE_DS_HSPCLK_BIT           %32)) | \
  (1 << (FEATURE_MPCCX_WHISPER_MODE_BIT  %32)) | \
  (1 << (FEATURE_DS_IPUCLK_BIT           %32)) | \
  (0 << (FEATURE_MSMU_LOW_POWER_BIT      %32)) \
)

#define FP8_SMU_FEATURES_ENABLE_64_DEFAULT  0

#define PHX1AM5_SMU_FEATURES_ENABLE_DEFAULT ( \
  (1 << (FEATURE_CCLK_DPM_BIT            %32)) | \
  (1 << (FEATURE_FAN_CONTROLLER_BIT      %32)) | \
  (1 << (FEATURE_DATA_CALCULATION_BIT    %32)) | \
  (1 << (FEATURE_PPT_BIT                 %32)) | \
  (1 << (FEATURE_TDC_BIT                 %32)) | \
  (1 << (FEATURE_THERMAL_BIT             %32)) | \
  (1 << (FEATURE_FIT_BIT                 %32)) | \
  (1 << (FEATURE_EDC_BIT                 %32)) | \
  (1 << (FEATURE_PLL_POWER_DOWN_BIT      %32)) | \
  (1 << (FEATURE_VDDOFF_BIT              %32)) | \
  (1 << (FEATURE_VCN_DPM_BIT             %32)) | \
  (1 << (FEATURE_CSTATE_BOOST_BIT        %32)) | \
  (1 << (FEATURE_FCLK_DPM_BIT            %32)) | \
  (1 << (FEATURE_SOCCLK_DPM_BIT          %32)) | \
  (1 << (FEATURE_MP0CLK_DPM_BIT          %32)) | \
  (1 << (FEATURE_LCLK_DPM_BIT            %32)) | \
  (1 << (FEATURE_SHUBCLK_DPM_BIT         %32)) | \
  (1 << (FEATURE_DCFCLK_DPM_BIT          %32)) | \
  (0 << (FEATURE_ISP_DPM_BIT             %32)) | \
  (1 << (FEATURE_IPU_DPM_BIT             %32)) | \
  (1 << (FEATURE_GFX_DPM_BIT             %32)) | \
  (1 << (FEATURE_DS_GFXCLK_BIT           %32)) | \
  (1 << (FEATURE_DS_SOCCLK_BIT           %32)) | \
  (1 << (FEATURE_DS_LCLK_BIT             %32)) | \
  (1 << (FEATURE_DS_DCFCLK_BIT           %32)) | \
  (1 << (FEATURE_DS_SHUBCLK_BIT          %32)) | \
  (0 << (FEATURE_GFX_TEMP_VMIN_BIT       %32)) | \
  (0 << (FEATURE_ZSTATES_BIT             %32)) | \
  (0 << (FEATURE_WHISPER_MODE_BIT        %32)) | \
  (1 << (FEATURE_DS_FCLK_BIT             %32)) | \
  (1 << (FEATURE_DS_SMNCLK_BIT           %32)) | \
  (1 << (FEATURE_DS_MP1CLK_BIT           %32)) \
)

#define PHX1AM5_SMU_FEATURES_ENABLE_EXT_DEFAULT ( \
  (1 << (FEATURE_DS_MP0CLK_BIT           %32)) | \
  (0 << (FEATURE_SMU_LOW_POWER_BIT       %32)) | \
  (0 << (FEATURE_FUSE_PG_BIT             %32)) | \
  (0 << (FEATURE_GFX_DEM_BIT             %32)) | \
  (1 << (FEATURE_PSI_BIT                 %32)) | \
  (1 << (FEATURE_PROCHOT_BIT             %32)) | \
  (1 << (FEATURE_CPUOFF_BIT              %32)) | \
  (1 << (FEATURE_STAPM_BIT               %32)) | \
  (1 << (FEATURE_S0I3_BIT                %32)) | \
  (1 << (FEATURE_DF_LIGHT_CSTATE         %32)) | \
  (0 << (FEATURE_PERF_LIMIT_BIT          %32)) | \
  (0 << (FEATURE_CORE_DLDO_BIT           %32)) | \
  (1 << (FEATURE_DVO_BIT                 %32)) | \
  (1 << (FEATURE_DS_VCN_BIT              %32)) | \
  (1 << (FEATURE_CPPC_BIT                %32)) | \
  (1 << (FEATURE_CPPC_PREFERRED_CORES_BIT  %32)) | \
  (1 << (FEATURE_DF_CSTATES_BIT          %32)) | \
  (0 << (FEATURE_FASTBYPASS_CLDO_BIT     %32)) | \
  (1 << (FEATURE_ATHUB_PG_BIT            %32)) | \
  (0 << (FEATURE_VDDOFF_ECO_BIT          %32)) | \
  (0 << (FEATURE_ZSTATES_ECO_BIT         %32)) | \
  (1 << (FEATURE_CC6_BIT                 %32)) | \
  (0 << (FEATURE_DS_UMCCLK_BIT           %32)) | \
  (0 << (FEATURE_DS_ISPCLK_BIT           %32)) | \
  (1 << (FEATURE_DS_HSPCLK_BIT           %32)) | \
  (0 << (FEATURE_MPCCX_WHISPER_MODE_BIT  %32)) | \
  (0 << (FEATURE_DS_IPUCLK_BIT           %32)) | \
  (0 << (FEATURE_MSMU_LOW_POWER_BIT      %32)) \
)

#define PHX1AM5_SMU_FEATURES_ENABLE_64_DEFAULT  0

//------------PHX2 PMM Feature -------------------
#define PHX2_SMU_FEATURES_ENABLE_DEFAULT ( \
  (1 << (FEATURE_CCLK_DPM_BIT            %32)) | \
  (1 << (FEATURE_FAN_CONTROLLER_BIT      %32)) | \
  (1 << (FEATURE_DATA_CALCULATION_BIT    %32)) | \
  (1 << (FEATURE_PPT_BIT                 %32)) | \
  (1 << (FEATURE_TDC_BIT                 %32)) | \
  (1 << (FEATURE_THERMAL_BIT             %32)) | \
  (1 << (FEATURE_FIT_BIT                 %32)) | \
  (1 << (FEATURE_EDC_BIT                 %32)) | \
  (1 << (FEATURE_PLL_POWER_DOWN_BIT      %32)) | \
  (1 << (FEATURE_VDDOFF_BIT              %32)) | \
  (1 << (FEATURE_VCN_DPM_BIT             %32)) | \
  (1 << (FEATURE_CSTATE_BOOST_BIT        %32)) | \
  (1 << (FEATURE_FCLK_DPM_BIT            %32)) | \
  (1 << (FEATURE_SOCCLK_DPM_BIT          %32)) | \
  (1 << (FEATURE_MP0CLK_DPM_BIT          %32)) | \
  (1 << (FEATURE_LCLK_DPM_BIT            %32)) | \
  (1 << (FEATURE_SHUBCLK_DPM_BIT         %32)) | \
  (1 << (FEATURE_DCFCLK_DPM_BIT          %32)) | \
  (0 << (FEATURE_ISP_DPM_BIT             %32)) | \
  (0 << (FEATURE_IPU_DPM_BIT             %32)) | \
  (0 << (FEATURE_GFX_DPM_BIT             %32)) | \
  (0 << (FEATURE_DS_GFXCLK_BIT           %32)) | \
  (1 << (FEATURE_DS_SOCCLK_BIT           %32)) | \
  (1 << (FEATURE_DS_LCLK_BIT             %32)) | \
  (1 << (FEATURE_DS_DCFCLK_BIT           %32)) | \
  (1 << (FEATURE_DS_SHUBCLK_BIT          %32)) | \
  (0 << (FEATURE_GFX_TEMP_VMIN_BIT       %32)) | \
  (0 << (FEATURE_ZSTATES_BIT             %32)) | \
  (1 << (FEATURE_WHISPER_MODE_BIT        %32)) | \
  (1 << (FEATURE_DS_FCLK_BIT             %32)) | \
  (1 << (FEATURE_DS_SMNCLK_BIT           %32)) | \
  (1 << (FEATURE_DS_MP1CLK_BIT           %32)) \
)

#define PHX2_SMU_FEATURES_ENABLE_EXT_DEFAULT ( \
  (1 << (FEATURE_DS_MP0CLK_BIT           %32)) | \
  (0 << (FEATURE_SMU_LOW_POWER_BIT       %32)) | \
  (0 << (FEATURE_FUSE_PG_BIT             %32)) | \
  (0 << (FEATURE_GFX_DEM_BIT             %32)) | \
  (1 << (FEATURE_PSI_BIT                 %32)) | \
  (1 << (FEATURE_PROCHOT_BIT             %32)) | \
  (1 << (FEATURE_CPUOFF_BIT              %32)) | \
  (1 << (FEATURE_STAPM_BIT               %32)) | \
  (1 << (FEATURE_S0I3_BIT                %32)) | \
  (1 << (FEATURE_DF_LIGHT_CSTATE         %32)) | \
  (0 << (FEATURE_PERF_LIMIT_BIT          %32)) | \
  (1 << (FEATURE_CORE_DLDO_BIT           %32)) | \
  (1 << (FEATURE_DVO_BIT                 %32)) | \
  (1 << (FEATURE_DS_VCN_BIT              %32)) | \
  (1 << (FEATURE_CPPC_BIT                %32)) | \
  (1 << (FEATURE_CPPC_PREFERRED_CORES_BIT  %32)) | \
  (1 << (FEATURE_DF_CSTATES_BIT          %32)) | \
  (0 << (FEATURE_FASTBYPASS_CLDO_BIT     %32)) | \
  (1 << (FEATURE_ATHUB_PG_BIT            %32)) | \
  (0 << (FEATURE_VDDOFF_ECO_BIT          %32)) | \
  (0 << (FEATURE_ZSTATES_ECO_BIT         %32)) | \
  (1 << (FEATURE_CC6_BIT                 %32)) | \
  (0 << (FEATURE_DS_UMCCLK_BIT           %32)) | \
  (0 << (FEATURE_DS_ISPCLK_BIT           %32)) | \
  (1 << (FEATURE_DS_HSPCLK_BIT           %32)) | \
  (0 << (FEATURE_MPCCX_WHISPER_MODE_BIT  %32)) | \
  (0 << (FEATURE_DS_IPUCLK_BIT           %32)) | \
  (0 << (FEATURE_MSMU_LOW_POWER_BIT      %32)) \
)

#define PHX2_SMU_FEATURES_ENABLE_64_DEFAULT  0

#define PHX2AM5_SMU_FEATURES_ENABLE_DEFAULT ( \
  (1 << (FEATURE_CCLK_DPM_BIT            %32)) | \
  (1 << (FEATURE_FAN_CONTROLLER_BIT      %32)) | \
  (1 << (FEATURE_DATA_CALCULATION_BIT    %32)) | \
  (1 << (FEATURE_PPT_BIT                 %32)) | \
  (1 << (FEATURE_TDC_BIT                 %32)) | \
  (1 << (FEATURE_THERMAL_BIT             %32)) | \
  (1 << (FEATURE_FIT_BIT                 %32)) | \
  (1 << (FEATURE_EDC_BIT                 %32)) | \
  (1 << (FEATURE_PLL_POWER_DOWN_BIT      %32)) | \
  (1 << (FEATURE_VDDOFF_BIT              %32)) | \
  (1 << (FEATURE_VCN_DPM_BIT             %32)) | \
  (1 << (FEATURE_CSTATE_BOOST_BIT        %32)) | \
  (0 << (FEATURE_FCLK_DPM_BIT            %32)) | \
  (1 << (FEATURE_SOCCLK_DPM_BIT          %32)) | \
  (1 << (FEATURE_MP0CLK_DPM_BIT          %32)) | \
  (1 << (FEATURE_LCLK_DPM_BIT            %32)) | \
  (1 << (FEATURE_SHUBCLK_DPM_BIT         %32)) | \
  (1 << (FEATURE_DCFCLK_DPM_BIT          %32)) | \
  (0 << (FEATURE_ISP_DPM_BIT             %32)) | \
  (0 << (FEATURE_IPU_DPM_BIT             %32)) | \
  (1 << (FEATURE_GFX_DPM_BIT             %32)) | \
  (1 << (FEATURE_DS_GFXCLK_BIT           %32)) | \
  (1 << (FEATURE_DS_SOCCLK_BIT           %32)) | \
  (1 << (FEATURE_DS_LCLK_BIT             %32)) | \
  (1 << (FEATURE_DS_DCFCLK_BIT           %32)) | \
  (1 << (FEATURE_DS_SHUBCLK_BIT          %32)) | \
  (0 << (FEATURE_GFX_TEMP_VMIN_BIT       %32)) | \
  (0 << (FEATURE_ZSTATES_BIT             %32)) | \
  (0 << (FEATURE_WHISPER_MODE_BIT        %32)) | \
  (1 << (FEATURE_DS_FCLK_BIT             %32)) | \
  (1 << (FEATURE_DS_SMNCLK_BIT           %32)) | \
  (1 << (FEATURE_DS_MP1CLK_BIT           %32)) \
)

#define PHX2AM5_SMU_FEATURES_ENABLE_EXT_DEFAULT ( \
  (1 << (FEATURE_DS_MP0CLK_BIT           %32)) | \
  (0 << (FEATURE_SMU_LOW_POWER_BIT       %32)) | \
  (0 << (FEATURE_FUSE_PG_BIT             %32)) | \
  (0 << (FEATURE_GFX_DEM_BIT             %32)) | \
  (1 << (FEATURE_PSI_BIT                 %32)) | \
  (1 << (FEATURE_PROCHOT_BIT             %32)) | \
  (1 << (FEATURE_CPUOFF_BIT              %32)) | \
  (1 << (FEATURE_STAPM_BIT               %32)) | \
  (0 << (FEATURE_S0I3_BIT                %32)) | \
  (1 << (FEATURE_DF_LIGHT_CSTATE         %32)) | \
  (0 << (FEATURE_PERF_LIMIT_BIT          %32)) | \
  (1 << (FEATURE_CORE_DLDO_BIT           %32)) | \
  (1 << (FEATURE_DVO_BIT                 %32)) | \
  (1 << (FEATURE_DS_VCN_BIT              %32)) | \
  (1 << (FEATURE_CPPC_BIT                %32)) | \
  (1 << (FEATURE_CPPC_PREFERRED_CORES_BIT  %32)) | \
  (1 << (FEATURE_DF_CSTATES_BIT          %32)) | \
  (0 << (FEATURE_FASTBYPASS_CLDO_BIT     %32)) | \
  (1 << (FEATURE_ATHUB_PG_BIT            %32)) | \
  (0 << (FEATURE_VDDOFF_ECO_BIT          %32)) | \
  (0 << (FEATURE_ZSTATES_ECO_BIT         %32)) | \
  (1 << (FEATURE_CC6_BIT                 %32)) | \
  (0 << (FEATURE_DS_UMCCLK_BIT           %32)) | \
  (0 << (FEATURE_DS_ISPCLK_BIT           %32)) | \
  (1 << (FEATURE_DS_HSPCLK_BIT           %32)) | \
  (0 << (FEATURE_MPCCX_WHISPER_MODE_BIT  %32)) | \
  (0 << (FEATURE_DS_IPUCLK_BIT           %32)) | \
  (0 << (FEATURE_MSMU_LOW_POWER_BIT      %32)) \
)

#define PHX2AM5_SMU_FEATURES_ENABLE_64_DEFAULT  0

typedef struct {
  uint8_t ForceFanPwmEn;         ///< [1 means use the ForceFanPwm value below]
  uint8_t ForceFanPwm;           ///< [% 0-100]
  uint8_t FanTable_Override;     ///< [1 means use the full set of data specified below; 0 means use default fan table]
  uint8_t FanTable_Hysteresis;   ///<
  uint8_t FanTable_TempLow;      ///<
  uint8_t FanTable_TempMed;      ///<
  uint8_t FanTable_TempHigh;     ///<
  uint8_t FanTable_TempCritical; ///<
  uint8_t FanTable_PwmLow;       ///<
  uint8_t FanTable_PwmMed;       ///<
  uint8_t FanTable_PwmHigh;      ///<
  uint8_t FanTable_PwmFreq;      ///< [0  = 25kHz; 1 = 100Hz]
  uint8_t FanTable_Polarity;     ///< [0 = negative; 1 = positive]
} FAN_POLICY;

typedef struct {
  uint32_t         TdpTjMax;
  FAN_POLICY       FanPolicy;
} FAN_POLICY_TABLE;

typedef struct {
  uint32_t Version;

  //PCD infrastructure limits
  uint32_t SUSTAINED_POWER_LIMIT;           //Power [mWatts] (STAPM)
  uint32_t FAST_PPT_LIMIT;                  //Power [mWatts] (fPPT)
  uint32_t SLOW_PPT_LIMIT;                  //Power [mWatts] (sPPT)
  uint32_t SLOW_PPT_TIME_CONSTANT;          //Time constant (seconds)
  uint8_t  SYSTEM_TEMPERATURE_TRACKING;     //Tskin feedback from EC; 1 = enabled; 0 = disabled
  uint8_t  STAPM_BOOST_OVERRIDE;            //1 = use value in STAPM_BOOST; 0 = use SMU FW default
  uint8_t  STAPM_BOOST;                     //1 = enabled; 0 = disabled
  uint8_t  AA_MODE_EN;                      //A+A power/thermal sharing algorithm; 1 = enable; 0 = disable
  uint32_t STAPM_TIME_CONSTANT;             //Time constant (seconds)
  uint32_t THERMAL_LIMIT;                   //TjMax ['C] (THM)
  uint32_t VRM_VDD_CURRENT_LIMIT;           //Current [mA] (VDDCR_VDD TDC)
  uint32_t VRM_VDD_MAXIMUM_CURRENT_LIMIT;   //Current [mA] (VDDCR_VDD EDC)
  uint32_t VRM_SOC_CURRENT_LIMIT;           //Current [mA] (VDDCR_SOC TDC)
  uint32_t VRM_SOC_MAXIMUM_CURRENT_LIMIT;   //Current [mA] (VDDCR_SOC EDC)
  uint32_t VRM_SR_CURRENT_LIMIT;            //Current [mA] (VDDCR_SR  TDC)
  uint32_t VRM_SR_MAXIMUM_CURRENT_LIMIT;    //Current [mA] (VDDCR_SR  EDC)
  uint32_t PSI3_VDD_CURRENT_LIMIT;          //Current [mA] (VDDCR_VDD PSI3)
  uint32_t PROCHOT_L_DEASSERTION_RAMP_TIME; //Time [ms] (PROCHOT)
  uint32_t P3T_LIMIT;                       //Power [Watts] (P3T - Peak Package Power)
  uint8_t  SYSTEM_CONFIG;                   //0 = use fused default; >0 = user specified value
  uint8_t  SW_SCI_GPE_ID;                   //GPE Event ID [0-31] used by MP1 to trigger software SCI (needs to be edge configured)
  uint8_t  FCH_IOMUX_19;                    //1 or 2 = save/restore for s0i3
  uint8_t  FCH_IOMUX_20;                    //1 or 2 = save/restore for s0i3

  //STT parameters/co-efficients
  uint8_t  STT_PCB_SENSOR_COUNT;           //2 = APU only; 3 = APU+dGPU; all other values are invalid
  uint8_t  STT_spare;
  uint16_t STT_MIN_POWER_LIMIT;            //Unsigned Q8.8 encoding, Minimum APU power limit for STT control
  int16_t  STT_M1;                         //Signed Q6.10 encoding
  int16_t  STT_M2;                         //Signed Q6.10 encoding
  int16_t  STT_M3;                         //Signed Q6.10 encoding
  int16_t  STT_M4;                         //Signed Q6.10 encoding
  int16_t  STT_M5;                         //Signed Q6.10 encoding
  int16_t  STT_M6;                         //Signed Q6.10 encoding
  int16_t  STT_C_APU;                      //Signed Q8.8 encoding
  int16_t  STT_C_HS2;                      //Signed Q8.8 encoding
  uint16_t STT_ALPHA_APU;                  //Unsigned Q0.16 encoding
  uint16_t STT_ALPHA_HS2;                  //Unsigned Q0.16 encoding
  uint16_t STT_SKIN_TEMPERATURE_LIMIT_APU; //Unsigned Q8.8 encoding
  uint16_t STT_SKIN_TEMPERATURE_LIMIT_HS2; //Unsigned Q8.8 encoding
  uint16_t STT_ERROR_COEFF;                //Unsigned Q0.16 encoding
  uint16_t STT_ERROR_RATE_COEFF;           //Unsigned Q0.16 encoding

  //PCD/CBS fan table
  uint8_t FanTable_Override;     //[1 means use the full set of data specified below; 0 means use default fan table]
  uint8_t FanTable_Hysteresis;
  uint8_t FanTable_TempLow;
  uint8_t FanTable_TempMed;
  uint8_t FanTable_TempHigh;
  uint8_t FanTable_TempCritical;
  uint8_t FanTable_PwmLow;
  uint8_t FanTable_PwmMed;
  uint8_t FanTable_PwmHigh;
  uint8_t FanTable_PwmFreq;      //[0 = 25kHz; 1 = 100Hz]
  uint8_t FanTable_Polarity;     //[0 = negative; 1 = positive]
  uint8_t FanTable_spare;

  //CBS debug options
  int16_t  PerCoreDldoPsmMargin[8];     //[PSM count] [signed] for each core
  int16_t  CoreDldoPsmMargin;           //[PSM count] [signed]
  int16_t  GfxDldoPsmMargin;            //[PSM count] [signed]
  int16_t  VddcrVddVoltageMargin;       //[mV] [signed]
  int16_t  VddcrSocVoltageMargin;       //[mV] [signed]
  int16_t  VddcrSrVoltageMargin;        //[mV] [signed]
  uint16_t ForceVddcrVddVoltage;        //[mV; 0 means no force]
  uint16_t ForceVddcrSocVoltage;        //[mV; 0 means no force]
  uint16_t ForceVddcrSrVoltage;         //[mV; 0 means no force]

  uint8_t  C1eEnabled;                  //[0 = disable C1e; 1 = enable C1e]
  uint8_t  CCX_VdciAsync;               //[0 = predictive mode; 1 = async mode]
  uint8_t  DisableMpCcxRootCg;          //[0 = Enable 1= MPCCX root clock wont be gated during vddoff
  uint8_t  ForceFanPwmEn;               //[1 means use the ForceFanPwm value below]
  uint8_t  ForceFanPwm;                 //[% 0-100]
  uint8_t  DfsBypassOff;                //PMFW-995
  uint8_t  CoreCountControlEn;          //[0 = Disable 1 = Enable CoreCountControl feature]
  uint8_t  HTFmaxTemperature;           //High temperature Fmax --- temperature ['C]
  uint16_t HTFmaxFrequency;             //High temperature Fmax --- frequency [MHz]
  uint16_t ForceCclkFrequency;          //Force the CCLK frequency to the specified frequency [MHz]
  uint16_t CclkFmaxOverride;            //Specify the CCLK Fmax override [MHz]
  uint16_t CclkFminOverride;            //Specify the CCLK Fmin override [MHz]
  uint16_t ForceGfxclkFrequency;        //Force the GFXCLK frequency to the specified frequency [MHz]
  uint16_t GfxclkFmaxOverride;          //Specify the GFXCLK Fmax override [MHz]
  uint8_t  PostCodeLevel;               //Postcode Verbosity: 0 = Disable, 1 = LEVEL1, 2 = LEVEL2, 3 = LEVEL3 (max = 3)
  uint8_t  StbVerbosityLevel;           //STB Verbosity: 0 = Disable, 1 = LEVEL1, 2 = LEVEL2, 3 = LEVEL3 (max = 3)
  uint8_t  EnableQos;                   //[0 = disable QoS; 1 = Enable QoS]
  uint8_t  LP2eEnable;
  uint8_t  DisableZ8;
  uint8_t  spare1;
  uint8_t  DisableZ10;
  uint8_t  ZstatesMaskEnable;
  uint32_t ZscIpIdleMask;
  uint32_t ZscWakeEventMask0;
  uint32_t ZscWakeEventMask1;
  uint32_t ZscIpFenceAckMask;

  //PCD/CBS Telemetry setup
  uint32_t TelemetryVddcrVddSlope;  //[uA]
  int32_t  TelemetryVddcrVddOffset;
  uint32_t TelemetryVddcrSocSlope;  //[uA]
  int32_t  TelemetryVddcrSocOffset;
  uint32_t TelemetryVddcrSrSlope;   //[uA]
  int32_t  TelemetryVddcrSrOffset;

  //PRECISE AND DIRECT OVERCLOCKING CONFIG
  uint8_t  OC_DISABLE; //1=disable overclocking; 0=allow overclocking
  uint8_t  OC_MINVID;  //[VID] SVI3 VID encoding
  uint16_t OC_FREQMAX; //MHz

  //DF CSTATE CONFIG
  uint8_t DfCstateConfigOverride;
  uint8_t DfCstateMmStutterOptEn;
  uint8_t DfCstateDceElptModeEn;
  uint8_t DfCstatePwrGateEn;
  uint8_t DfCstateFclkPwrDnEn;
  uint8_t DfCstateSelfRefrEn;
  uint8_t DfCstateUclkPwrDnEn;
  uint8_t spare2;

  //DF Light CSTATE CONFIG
  uint8_t DfLightCstateConfigOverride;
  uint8_t CcmHystDly;
  uint8_t Ccmhystscale;
  uint8_t Ccmmonitorsuccdly;
  uint8_t Ccmmonitorsuccscale;
  uint8_t Ccmmonitorsatlvl;
  uint8_t Ccmmonitorsuccthr;
  uint8_t Ccmmonitorfaildec;
  uint8_t Ccmmonitorburstfilter;
  uint8_t Ccmmonitorburstscale;
  uint8_t spare3;

  uint8_t CC1Dis; //[0 = CC1 default; 1 = CC1 disabled]

  //CPPC Constraints
  uint8_t CppcConstraintsEnabled;
  uint8_t spare4[3];
  uint8_t PerfLimit_MaxRange;
  uint8_t PerfLimit_MinRange;
  uint8_t EPP_MaxRange;
  uint8_t EPP_MinRange;

  //Misc
  uint32_t System;
  uint32_t MinS0i3SleepTimeInMs;
  uint32_t SystemRtcTime;

  //GFX OC limits
  uint8_t  OC_GFX_DISABLE;  //1=disable overclocking; 0=allow overclocking
  uint8_t  OC_GFX_MINVID;   //[PMFW-14874][VID] Setting a maximum voltage limitation with SVI3 VID encoding
  uint16_t OC_GFX_FREQMAX;  //MHz

  //GFXCLK/GFXOFF related parameters
  uint8_t  GceaLinkMgrIdleThreshold;        //Set by SMU FW during enablment of SOC_ULV. Controls delay for GFX SDP port disconnection during idle events
  uint8_t  GfxClkSource;                    // 0 = PLL, 1 = DFLL
  uint8_t  spare5;
  uint8_t  ApccEnable;                      //1 = Enable APCC, should be mutually exclusive to GFX EDC controller

  uint16_t GfxVinit;                        // Voltage required to support Finit during GFX power up/GFXOFF exit (in mV)
  uint16_t GfxclkInitFreq;                  // in Mhz. Options are 800, 600, 400, 300, 200, 100
  uint16_t GfxclkGfxoffEntryFreq;           // in Mhz
  uint16_t GfxclkIdleFreq;                  // in MHz
  uint16_t GfxclkSlewRate;                  // for PLL babystepping
  uint16_t GfxclkFminOverride;              // Specify the GFXCLK Fmin override [MHz]

  uint16_t GfxMinVoltage;                   //Minimum GFX voltage, in mV
  uint16_t GfxMaxVoltage;                   //Maximum GFX voltage, in mV

  uint8_t  GfxDldoBypass;
  uint8_t  GfxDynamicRmEn;
  //GFX temperature dependent Vmin feature
  uint8_t  VDDGFX_TVminOverrideEn;
  uint8_t  spare6;
  uint16_t VDDGFX_TVmin;          // Celcius
  uint16_t VDDGFX_Vmin_HiTemp;    // mV
  uint16_t VDDGFX_Vmin_LoTemp;    // mV
  uint16_t VDDGFX_TVminHystersis; // Celcius

  // Enable automated phase shedding and diode emulation (PSI7)
  uint8_t  VddcrVddPsi7En;
  uint8_t  VddcrSocPsi7En;
  uint8_t  VddcrSrPsi7En;
  uint8_t  PsiDecayConditionVddRail;
  uint8_t  PsiDecayConditionSocRail;
  uint8_t  PsiDecayConditionSrRail;
  uint8_t  PsiDecayConditionOverride; // Override bit to PSI Decay Condition
  uint8_t  spare7;

  uint8_t  VddmCldoFastStartupEn;
  uint8_t  VddmCldoOvDetEn;
  uint16_t PllSscSpreadPercentage; // PLL Spread down percentage, Q8 format. e.g. 192 gives 0.75%

  uint8_t  FclkDpmOptimizationEn;     //[0: disable; 1: enable]
  uint8_t  DisplayIdleOptimizationEn; //[0: disable; 1: enable]
  uint16_t VddpRegDdrRateThreshold;   // DDR RATE thresold for VDDP CLDO regulation

  uint16_t LclkMaxFreq;            // in MHz
  uint16_t LclkMinFreq;            // in MHz

  uint16_t VDD_MISC_Voltage;       // Voltage in mV (used in AOD menu)
  uint8_t  SbTsiSlaveAddrOverride; // This field must be set, so PMFW override the SB-TSI Slave address
  uint8_t  SbTsiSlaveAddrSelect;   // SB-TSI Slave Address Select: 0 = 98h, 1 = 9Ah, 2 = 9Ch, 3 = 9Eh, 4 = 90h, 5 = 92h, 6 = 94h, 7 = 96h

  // Slew rate control for VDDCR_VDD rail
  uint8_t  SlewRateOverrideVdd; // Set this field to indicate the Slew Rate must change
  uint8_t  SlewRateUpIndexVdd;  // 0 = 2.5 mV/us, 1 = 10 mV/us, 2 = 20 mV/us, 3 = 40 mV/us.
  uint8_t  SlewRateDownCntlVdd; // 0: Negative equal to Positive Slew Rate, 1: Negative equal to 1/4 Positive Slew Rate
  uint8_t  spare8;

  // Slew rate control for VDDCR_SOC and VDDCR_SR rails
  uint8_t  SlewRateOverrideSoc; // Set this field to indicate the Slew Rate must change
  uint8_t  SlewRateUpIndexSoc;  // 0 = 2.5 mV/us, 1 = 10 mV/us, 2 = 20 mV/us, 3 = 40 mV/us.
  uint8_t  SlewRateDownCntlSoc; // 0: Negative equal to Positive Slew Rate, 1: Negative equal to 1/4 Positive Slew Rate
  uint8_t  spare9;

  // PSM GuardBand
  int8_t   PsmGuardband_INTERNAL[5][4]; //5 frequency points * 4 temperature points
  int8_t   PsmGuardband_EXTERNAL[5][4]; //5 frequency points * 4 temperature points

  // Z10 Slew Rate control
  uint8_t  Z10SlewRateOverride; // Set this field to indicate the Zstates Slew Rate must change
  uint8_t  Z10UpSlewRateVdd;    // 4 bits [3:0]: Up Slew Rate = [3:0] * 2.5 + 2.5 mV/us
  uint8_t  Z10UpSlewRateSoc;    // 4 bits [3:0]: Up Slew Rate = [3:0] * 2.5 + 2.5 mV/us
  uint8_t  Z10UpSlewRateSr;     // 4 bits [3:0]: Up Slew Rate = [3:0] * 2.5 + 2.5 mV/us
  uint8_t  Z10DownSlewRateVdd;  // 0: Negative equal to Positive Slew Rate, 1: Negative equal to 1/4 Positive Slew Rate
  uint8_t  Z10DownSlewRateSoc;  // 0: Negative equal to Positive Slew Rate, 1: Negative equal to 1/4 Positive Slew Rate
  uint8_t  Z10DownSlewRateSr;   // 0: Negative equal to Positive Slew Rate, 1: Negative equal to 1/4 Positive Slew Rate
  uint8_t  Z8Gate;              // 0: Z8 Gate Disable, 1: Z8 Gate Enable

  // Core DLDO regulation override for non-secure or unlocked parts
  uint8_t  CoreDldoRegulation[8]; // Per core DLDO regulation control (each index is for the corresponding physical core): 0 = bypass, 1 = regulation

  uint8_t  HeteroCoreManagement;  // 0: Disable; 1: Enable Static; 2: Enable Dynamic
  uint8_t  VRHOTEn;// 0 = VR_HOT throttle disabled, 1 = VR_HOT throttle enabled
  uint8_t  SB1;
  uint8_t  SB3;

  uint32_t reserved;

  //SPARE FOR FUTURE USE
  uint32_t spare12[32];
} SMU_PP_TABLE;

typedef union _ZSC_WAKE_EVENT_MASK_0_STRUCT {
  struct {
    uint32_t  WAKE_EVENT_MASK_TIMER:1;       // bit0
    uint32_t  WAKE_EVENT_MASK_S2NS_TIMER:1;  // bit1
    uint32_t  rsvd_0:2;                      // bit[3:2]
    uint32_t  WAKE_EVENT_MASK_FCH:1;         // bit4
    uint32_t  WAKE_EVENT_MASK_ACP:1;         // bit5
    uint32_t  WAKE_EVENT_MASK_MP2:1;         // bit6
    uint32_t  WAKE_EVENT_MASK_DBREQ:1;       // bit7
    uint32_t  WAKE_EVENT_MASK_DCN_S:1;       // bit8
    uint32_t  WAKE_EVENT_MASK_DCN_NS:1;      // bit9
    uint32_t  rsvd_1:6;                      // bit[15:10]
    uint32_t  WAKE_EVENT_MASK_DF:1;          // bit16
    uint32_t  rsvd_2:15;                     // bit[31:17]
  } Field;
  uint32_t Value;
} ZSC_WAKE_EVENT_MASK_0_STRUCT;

SIL_STATUS
InitializeApiSmuPhx (
  SIL_CONTEXT  *SilContext
  );

SIL_STATUS
SmuSetInputBlkPhx (
  SIL_CONTEXT  *SilContext
  );
