@echo off
rem ** SetSilEnv.cmd
rem *    Establish the environment control variable for the openSIL
rem *    buld tools  (meson & Kconfig)
rem *
rem *   %1 param - indicator flag for obj size desired ( 'W32' or 'W64')
rem *

set SIL_HOME=%cd%
rem
rem  Create the build directory and project config for Meson builder
rem
if /i %1 == W32 (
    rem the " -Dbackend=vs " parameter
    meson setup build_w32 --cross-file util/meson/x86-i386-msvc -Dbackend=vs -Dunit_test=true
    set SIL_BUILD=%SIL_HOME%\build_w32
    echo ***** The Meson environment for Win 32b is ready *****
    echo * Use this cmd for building:
    echo *  $   meson compile -C build_w32
    echo *
) else if /i %1 == W64 (
    meson setup build_w64 --cross-file util/meson/x86-amd64-msvc -Dbackend=vs -Dunit_test=true
    set SIL_BUILD=%SIL_HOME%\build_w64
    echo **** The Meson environment for Win 64b is ready ****
    echo * Use this cmd for building:
    echo *   $   meson compile -C build_w64
    echo *
) else (
    echo ****ERROR bad param. Use 'W32' or 'W64'
    exit (1)
)
echo **** Commands for Kconfig:
echo ** Use the Kcfg.cmd program to help with openSIL Kconfig
call util\Kcfg.cmd help

