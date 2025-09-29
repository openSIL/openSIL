@echo off
rem ** SetSilEnv.cmd
rem *    Establish the environment control variables for the openSIL
rem *    build tools  (meson & Kconfig)
rem *
rem *

set SIL_HOME=%cd%
rem
rem  Create the build directory and project config for Meson builder
rem      default is Windows host environment building for 32bit execution.
rem      All host-build units are assumed to be 64bit (Linux or Windows).
call util\meson.cmd W32

rem *
rem * Set the Kconfig environment controls for openSIL
rem *   These settings are defaults for use with CI testing
rem *
set PYTHONPATH=%SIL_HOME%\util\kconfig\lib
set KCONFIG_CONFIG=SilCfg-AllYes
set KCONFIG_AUTOHEADER=%KCONFIG_CONFIG%.h
rem *
echo **** Commands for Kconfig:
echo ** Use the util\Kcfg.cmd program to help with openSIL Kconfig
echo *
