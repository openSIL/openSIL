@echo off
rem ** meson.cmd
rem *
rem * SPDX-License-Identifier: MIT
rem * Copyright (C) 2022 - 2025 Advanced Micro Devices, Inc. All rights reserved.

goto TestParams
:MenuHelp
    echo "*******************************************************************"
    echo "** Meson.cmd   {command}  {option}                                *"
    echo " *    Provide a command set to simplify the Meson operations for  *"
    echo " *    the openSIL platform build tools  (meson)                   *"
    echo " *                                                                *"
    echo " *   command - Action and target obj size                         *"
    echo " *        W32 32 or Set32 - establishes the 32bit build dir/env   *"
    echo " *        W64 64 or Set64 - established the 64bit build dir/env   *"
    echo " *        Build32 or Build64 - initiates a build for the 32/64 env*"
    echo " *                                                                *"
    echo " *   option - Option for command                                  *"
    echo " *        'wipe' may be used with the setup command to rebuild    *"
    echo " *                the make system control files                   *"
    echo " *        'clean' may be used with the compile command to clear   *"
    echo " *                previous compilation results.                   *"
    echo "*******************************************************************"
    exit /b 0

:TestParams
rem Start of main processing..

rem Check the file system type and don't try to build on 9p filesystems - it doesn't seem to work.
rem If you can figure out how to fix this, please feel free to remove this check. It's just here
rem to keep others from wasting time the way I did.
set scriptname=%0
set scriptname=%scriptname:\=\\%
for /F "tokens=2 skip=1 USEBACKQ delims==" %%G in (`wmic datafile where Name^="%scriptname%" get FSName /value`) do (if not defined fstype set "fstype=%%G")
if "-%fstype%-" == "-9P-" goto FS_Error

set SIL_HOME=%cd%
set CMD=%1
set OPT=%2

setlocal enabledelayedexpansion
if NOT "%3" == "" (
  set OPT2=#####%*
  set OPT2=!OPT2:#####%1 %2 =!
)

rem Verify that the meson is installed either natively or with python
meson.exe -v >nul 2>&1
if %errorlevel% equ 0 (
    set MESON_CMD=meson.exe
    goto MesonConfigured
)

pip show meson >nul 2>&1
if %errorlevel% neq 0 (
    goto MesonError
) else (
    set MESON_CMD=python -m mesonbuild.mesonmain
    goto MesonConfigured
)

:MesonConfigured
rem Check for cl, msbuild.exe and VSINSTALLDIR
if "-%VSINSTALLDIR%-" == "--" goto VisualStudioError
cl.exe >nul 2>&1
if %errorlevel% neq 0 goto VisualStudioError
msbuild.exe -ver >nul 2>&1
if %errorlevel% neq 0 goto VisualStudioError

rem check for nasm
nasm.exe -h >nul 2>&1
if %errorlevel% neq 0 goto NasmError

rem check that the meson version is greater or equal to 1.1.0
setlocal disabledelayedexpansion
for /F "tokens=* USEBACKQ" %%G in (`%MESON_CMD% --version`) do (
  set MESON_VERSION=%%G
)

for /F "tokens=1-2 delims=." %%G in ("%MESON_VERSION%") do (
  set MESON_MAJOR_VER=%%G
  set MESON_MINOR_VER=%%H
)

if %MESON_MAJOR_VER% gtr 1 (
  endlocal
  goto :meson_version_good
)
if %MESON_MAJOR_VER% equ 1 if %MESON_MINOR_VER% geq 1 (
  endlocal
  goto :meson_version_good
)

echo "Error: Your meson version (%MESON_VERSION%) is below 1.1.0 and needs to be updated."
exit /b 1

:meson_version_good

rem
rem  Create the build directory and project config for Meson builder
rem
    rem the " -Dbackend=vs " parameter is for use with the MSVC ToolChain
    rem  it designates the make system to use is Visual Studio (VS)
if /i "%CMD%" == "W32" goto Set32
if /i "%CMD%" == "32"  goto Set32
if /i "%CMD%" == "set32"  goto Set32
if /i "%CMD%" == "Build32" goto Build32
if /i "%CMD%" == "Show32" goto Show32
goto Check64

rem Set the meson build folder for 32 bits
:Set32
    set MOptions=-Dbackend=vs -Dunit_test=true
    if /i "%OPT%" == "wipe" set MOptions=%MOptions% --wipe
    echo "* %MESON_CMD% setup build_w32 --cross-file %SIL_HOME%\util\meson\x86-i386-msvc %MOptions% %OPT2%"
    %MESON_CMD% setup build_w32 --cross-file %SIL_HOME%\util\meson\x86-i386-msvc %MOptions% %OPT2%
    set SIL_BUILD=%SIL_HOME%\build_w32
    echo "***** The Meson environment for Windows 32b is ready *****"
    goto Done

:Build32
    set MOptions=
    if /i "%OPT%" == "clean" set MOptions=--clean
    echo "* %MESON_CMD% compile -C build_w32 %MOptions% %OPT2%"
    %MESON_CMD% compile -C build_w32 %MOptions% %OPT2%
    goto Done

:Show32
    cd build_w32
    %MESON_CMD% configure --no-pager
    goto Done

:Check64
if /i "%CMD%" == "W64" goto Set64
if /i "%CMD%" == "64"  goto Set64
if /i "%CMD%" == "set64"  goto Set64
if /i "%CMD%" == "Build64" goto Build64
if /i "%CMD%" == "Show64" goto Show64
goto MenuHelp

:Set64
    set MOptions=-Dbackend=vs -Dunit_test=true
    if /i "%OPT%" == "wipe" set MOptions=%MOptions% --wipe
    echo "* %MESON_CMD% setup build_w64 --cross-file %SIL_HOME%\util\meson\x86-amd64-msvc %MOptions% %OPT2%"
    %MESON_CMD% setup build_w64 --cross-file %SIL_HOME%\util\meson\x86-amd64-msvc %MOptions%  %OPT2%
    set SIL_BUILD=%SIL_HOME%\build_w64
    echo "***** The Meson environment for Windows 64b is ready *****"
    goto Done

:Build64
    set MOptions=
    if /i "%OPT%" == "clean" set MOptions=--clean
    echo "* %MESON_CMD% compile -C build_w64 %MOptions%  %OPT2%"
            %MESON_CMD% compile -C build_w64 %MOptions% %OPT2%
    goto Done

:Show64
    cd build_w64
    %MESON_CMD% configure --no-pager
    goto Done

:FS_Error
    echo * Error: Meson does not build correctly when run on Windows under a WSL2-mounted drive.
    echo *        If you're building under windows, move this to a native filesystem.
    exit /b 1

:VisualStudioError
    echo * Error: To build AMD openSIL under windows, you need to have the VSINSTALLDIR environment
    echo *        variable set, and both cl.exe and msbuild.exe in your path.
    exit /b 1

:MesonError:
    echo * Error: Meson is either not installed or not in your path.
    echo *        Please install meson using the python "pip install meson" command
    echo *        or "choco install meson" - https://chocolatey.org/
    exit /b 1

:NasmError
    echo * Error: Nasm is either not installed or not in your path.
    echo *        Please install meson using "choco install nasm" - https://chocolatey.org/
    echo *        or with a windows installer: https://www.nasm.us
    exit /b 1

:Done
