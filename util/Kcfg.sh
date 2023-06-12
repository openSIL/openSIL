#!/bin/bash
#
KcfgHelp () {
    echo "*******************************************************************"
    echo "** Kcfg.sh                                                        *"
    echo "*    Provide a command set to simplify the Kconfig operations for *"
    echo "*      the openSIL platform configuration process.                *"
    echo "*                                                                 *"
    echo "*   Expectations:                                                 *"
    echo "*   - Run this tool from the openSIL root directory               *"
    echo "*   - A new platform config file can only be created using Menu   *"
    echo "*   - A base config file must already exist for the Update and    *"
    echo "*       AllYes operations                                         *"
    echo "*                                                                 *"
    echo "* prototype:                                                      *"
    echo "*    Kcfg  {Command}  {PlatName}                                  *"
    echo "*                                                                 *"
    echo "*      Command -  action to perform. Use one of the following:    *"
    echo "*        'menu'    - run the interactive menu system to select    *"
    echo "*                    options for a specific platform.             *"
    echo "*                    This produces the [platName] file            *"
    echo "*        'update'  - After creating a [PlatName] file with 'menu',*"
    echo "*                    create an updated platform configuration     *"
    echo "*                    for use in the code base (this creates       *"
    echo "*                    the [platName].h file)                       *"
    echo "*                                                                 *"
    echo "*      PlatName - This is the name used to identify the config.   *"
    echo "*               This name will be used as the filename for the    *"
    echo "*               output config file. Some commands will also use   *"
    echo "*               this name to load previous config selections.     *"
    echo "*               PlatName may be the full filename as seen in the  *"
    echo "*               configs dir or may be the base name. This script  *"
    echo "*               will check for the various suffixes used.         *"
    echo "*               e.g.  'Onyx_SilCfg' or 'Onyx'  may be used.       *"
    echo "*                                                                 *"
    echo "*      Both parameters are required.                              *"
    echo "*******************************************************************"
    return 1
#  * Return values:
#  *   0 - OK
#  *   1 - Invalid parameter or wrong start dir
#  *   2 - PlatName error
#  ********************************************************************
};

shopt -s nocasematch        # ignore case

PlatName () {
    # Validate the 2nd parameter: PlatName
    # set the Kconfig name and pull previous defs, or create the default set.
    # Kconfig must execute from the top dir (tree root) so copy/create the files
    #   temporarily in the root folder, then move them back to /configs
    echo "** Validating PlatName parameter: ${2}"
    if test -z ${2} ; then
        echo "*** ERROR: PlatName parameter misssing"
        KcfgHelp
        exit 1
    fi
    if test -f "configs/${2}" ; then
        # param was full filename, proceed...
        # Try to avoid altering the timestamp
        export KCONFIG_CONFIG=$2
        export KCONFIG_AUTOHEADER=${KCONFIG_CONFIG}.h
        cp -p --update --verbose  configs/${2}* .
    elif test -f "configs/${2}_SilCfg" ; then
        # param was base name, add '_SilCfg' to the end and proceed...
        export KCONFIG_CONFIG=$2_SilCfg
        export KCONFIG_AUTOHEADER=${KCONFIG_CONFIG}.h
        cp -p --update --verbose  configs/${KCONFIG_CONFIG}* .
    else
        # file was not found, create default IFF cmd=menu
        if [[ ${1} == menu ]] ; then
            # Only for the 'menu' command should we create the defults, else error
            echo "*** WARNING: \"${2}\" does not exist in /configs/*. Using defaults."
            export KCONFIG_CONFIG=$2_SilCfg
            export KCONFIG_AUTOHEADER=${KCONFIG_CONFIG}.h
            python ${PYTHONPATH}/alldefconfig.py
        else
            # Don't allow backdoor creation of a config file, force use of Menu
            echo "*** ERROR: \"${2}\" is not valid. You MUST use the Menu command"
            echo "     to create new platform config files."
            exit 2
        fi
    fi
    echo "** Verfied Plat filenames as: ${KCONFIG_CONFIG}, ${KCONFIG_AUTOHEADER}"
};

####     MAIN Start    ###############
#
# Set the Kconfig environment controls for openSIL
#
# Verify that CWD is at the openSIL root
if [[ ${SIL_HOME} == "" ]]; then
    if test -f "util/Kcfg.sh" ; then
        export SIL_HOME=$(pwd)
    else
        echo "** ERROR: Must be started from the openSIL root directory"
        exit  1
    fi
fi

export PYTHONPATH=${SIL_HOME}/util/kconfig/lib

# Save present Env Var values
orig_KCONFIG_CONFIG=${KCONFIG_CONFIG}
orig_KCONFIG_AUTOHEADER=${KCONFIG_AUTOHEADER}

# Set Env Vars for this platform
copyrightDate='2022-'$(date +'%Y')

# Kconfig will output these vars into the output files....
#    use them to insert the copyright notice
# Note: NewLine must be included in the var, so ending quote is on a new line.
export KCONFIG_CONFIG_HEADER="# Copyright ${copyrightDate} Advanced\
 Micro Devices, Inc. All rights reserved.
"
export KCONFIG_AUTOHEADER_HEADER="/* Copyright ${copyrightDate} Advanced\
 Micro Devices, Inc. All rights reserved.*/
"

#
# Validate the parameters, process the command
#
shopt -s nocasematch        # ignore case

case $1 in
  ( menu )
    PlatName ${1} ${2}
    python ${PYTHONPATH}/menuconfig.py Kconfig
    python ${PYTHONPATH}/genconfig.py
    echo "** MenuConfig & GenConfig :"
    echo "** Input from ${KCONFIG_CONFIG}"
    echo "** Output to: ${KCONFIG_CONFIG}.old and ${KCONFIG_AUTOHEADER}"
    ;;

  ( update )
    PlatName ${1} ${2}
    retcode=$?
    if [ ${retcode} -eq 0 ] ; then
        python ${PYTHONPATH}/genconfig.py
        echo "** GenConfig: Input from ${KCONFIG_CONFIG};"
        echo "              output to ${KCONFIG_AUTOHEADER}"
    fi
    ;;

  ( help )
    KcfgHelp
    exit 0
    ;;

  (*)
    echo "*** ERROR:Invalid command parameter"
    KcfgHelp
    exit 1
    ;;
esac
#
# Now move the resulting files back to the /configs directory
#   and remove any that were not updated (not moved back)
#

echo "** Moving: ${KCONFIG_CONFIG}, ${KCONFIG_AUTOHEADER} back to /configs dir"
mv  --verbose --update  ${KCONFIG_CONFIG}*    ./configs/
rm  --verbose ${KCONFIG_CONFIG}*
