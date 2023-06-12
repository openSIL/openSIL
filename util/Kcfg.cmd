@echo off
goto START
:HELP
echo ** Kcfg.cmd
echo *    Provide a command set to simplify the Kconfig operations for the
echo *      openSIL platform configuration process.
echo *
echo *   Expectations:
echo *   - Run this tool from the openSIL root directory
echo *   - A new platform config file can only be created using Menu
echo *   - A base config file must already exist for the Update and
echo *       AllYes operations
echo *
echo * prototype:
echo *    Kcfg  {Command}  {PlatName}
echo *
echo *      Command -  action to perform. Use one of the following:
echo *        'menu'    - run the interactive menu system to select
echo *                    options for a specific platform.
echo *                    This produces the [platName] file
echo *        'update'  - After creating a [PlatName] file with 'menu',
echo *                    create an updated platform configuration file for use
echo *                    in the code base (this creates the [platName].h file)
echo *
echo *      PlatName - This is the name used to identify the configuration.
echo *               This name will be used as the filename for the output
echo *               config file. Some commands will also use this name to
echo *               load previous configuration selections.
echo *               PlatName may be the full filename as seen in the configs dir
echo *               or may be the base name. This script will check for the
echo *               various suffixes used.
echo *               e.g.  "Onyx_SilCfg" or "Onyx"  may be used.
echo *
echo *      Both parameters are required.
echo ********************************************************************
rem  * Return values:
rem  *   0 - OK
rem  *   1 - Invalid parameter or wrong start dir
rem  *   2 - PlatName error
rem  ********************************************************************
:EXIT1
exit /b 1

:START
rem *
rem * Validate the parameters
rem *
if /i  "%2" equ "" (
    echo "*** ERROR: PlatName is a required parameter"
    goto HELP
)
rem *
rem * Set the Kconfig environment controls for openSIL
rem *
rem * Verify that CWD is at the openSIL root
if "%SIL_HOME%"=="" (
    if exist "util/Kcfg.cmd" (
        set SIL_HOME=%cd%
    ) else (
        echo ** ERROR: Must be started from the openSIL root directory"
        goto EXIT1
    )
)
set PYTHONPATH=%SIL_HOME%\util\kconfig\lib
rem * Save present Env Var values
set orig_KCONFIG_CONFIG=%KCONFIG_CONFIG%
set orig_KCONFIG_AUTOHEADER=%KCONFIG_AUTOHEADER%
rem * Set Env Vars for this platform
rem *
rem * set the Kconfig name and pull previous defs or create a default set.
rem * Kconfig must execute from the top dir (tree root); so, copy/create the
rem *   files temporarily in the root folder, then move them back to /configs
rem
rem * First validate the filename parameter,
rem *   check if it was the full name or just the base
if exist "configs\%2" (
    rem * param was the full filename, proceed...
    set KCONFIG_CONFIG=%2
    set KCONFIG_AUTOHEADER=%2.h
    copy configs\%2* .\
) else (
    rem * check if param was just the base name, add "_SilCfg" to the end
    if exist "configs\%2_SilCfg" (
        rem * Param was base name, set full name and proceed...
        set KCONFIG_CONFIG=%2_SilCfg
        set KCONFIG_AUTOHEADER=%2_SilCfg.h
        copy configs\%2_SilCfg* .\
    ) else (
        rem * If cmd=AllYes, then check for base + "-AllYes"
        if /i  "%1" equ "AllYes" (
            set KCONFIG_CONFIG=%2-AllYes
            set KCONFIG_AUTOHEADER=%2-AllYes.h
            if exist "configs\%2-AllYes" (
                rem * Param was base name, set full name and proceed...
                copy configs\%2-AllYes* .\
            ) else (
                rem * No AllYes file, make one with defaults
                python %PYTHONPATH%\alldefconfig.py
            )
        ) else (
            rem * no file was found. Create a defaults version IFF menu cmd
            if /i  "%1" equ "menu" (
                rem  Only for the 'menu' command should we create the defaults, else error
                echo *** WARNING: "%2" does not exist in \configs\*. Using defaults.
                set KCONFIG_CONFIG=%2_SilCfg
                set KCONFIG_AUTOHEADER=%2_SilCfg.h
                 python %PYTHONPATH%\alldefconfig.py
            ) else (
                rem Don't allow backdoor creation of a config file, force use of Menu
                echo *** ERROR: "%2" is not valid. You MUST use the Menu command
                echo           to create new platform config files.
                exit /b 2
            )
        )
    )
)
rem *******************************************************************
rem * Set Kconfig Env Vars to place a copyright into the output files
rem * Copyright lines must have a ending newline. Use this CMD trick:
rem * How it works?
rem * The caret is an escape character, it escapes the next character and
rem *  itself is removed. But if the next character is a linefeed the linefeed
rem *  is also removed and only the next character is effectivly escaped
rem *  (even if this is also an linefeed).
rem *  Therefore, two empty lines are required.
set LF=^


rem *  !!The above two blank lines are Required!! Do not delete them !
rem  Set Env Vars for this platform
set copyrightDate=2022-%date:~-4%
set KCONFIG_CONFIG_HEADER=# Copyright %copyrightDate% Advanced Micro Devices, Inc.^
 All rights reserved.^%LF%%LF%
set KCONFIG_AUTOHEADER_HEADER=/* Copyright %copyrightDate% Advanced Micro Devices, Inc.^
 All rights reserved.*/^%LF%%LF%
rem *
rem *******************************************************************
rem *
rem * Process the command...
rem *
if /i "%1"=="menu" (
    python %PYTHONPATH%\menuconfig.py Kconfig
    python %PYTHONPATH%\genconfig.py
    echo ** MenuConfig ^& GenConfig :
    echo ** Input from %KCONFIG_CONFIG%
    echo ** Output to: %KCONFIG_CONFIG%,    %KCONFIG_CONFIG%.old ^
    and %KCONFIG_AUTOHEADER%
    goto FINISH
)

if /i "%1"=="update" (
    python %PYTHONPATH%\genconfig.py
    echo ** GenConfig: Input from %KCONFIG_CONFIG%;  output to %KCONFIG_AUTOHEADER%
    goto FINISH
)

:FINISH
rem * put platform config data in the \configs directory
rem * Note Cmd:copy & move may update the timestamp and cause a full rebuild.
move /Y %KCONFIG_CONFIG%*     .\configs\

:CLEANUP
set KCONFIG_CONFIG=%orig_KCONFIG_CONFIG%
set KCONFIG_AUTOHEADER=%orig_KCONFIG_AUTOHEADER%

set orig_KCONFIG_CONFIG=
set orig_KCONFIG_AUTOHEADER=
set KCONFIG_CONFIG_HEADER=
set KCONFIG_AUTOHEADER_HEADER=
set LF=

:Exit
exit /b 0
