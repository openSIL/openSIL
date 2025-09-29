## SPDX-License-Identifier: MIT
## Copyright (C) 2024 - 2025 Advanced Micro Devices, Inc. All rights reserved.

# This makefile is really a script to collect windows and linux scripts
# under a single front-end so that things can be run the same way on both.
# The build is done with meson

# NMAKE SPECIFIC CODE BLOCK \
!MESSAGE This Makefile was written for GNU make, and will not work with nmake. # \
!MESSAGE Install it directly from https://gnuwin32.sourceforge.net/packages/make.htm # \
!MESSAGE install with chocolatey https://chocolatey.org/ - 'chocolatey install make' # \
!MESSAGE or install as a part of cygwin: https://www.cygwin.com/ # \
!ERROR "Exiting." # \
# NMAKE BLOCK DONE

ifneq ($(OS),Windows_NT)
# Linux, maybe BSDs?
SHELL := /bin/sh
/ := /
export SIL_HOME := $(PWD)
MESONSCRIPT := $(SIL_HOME)$(/)util$(/)meson.sh
ECHO := echo "
OHCE := "
RMDIR := rm -rf
CP := cp
QUIET_ERRORS := >/dev/null 2>&1
BUILD_DIR_32 := build_L32
BUILD_DIR_64 := build_L64
DOXYAPISCRIPT := .$(/)GenApiSpec.sh
DOXYCODESCRIPT := .$(/)GenCodeSpec.sh
else
# Windows
SHELL=cmd.exe
export SIL_HOME := $(shell cd)
/ := $(shell echo \)
ifeq ($(/),)
/ := $(shell echo \\)
endif
MESONSCRIPT := $(SIL_HOME)$(/)util$(/)meson.cmd
SILENVSCRIPT := $(SIL_HOME)$(/)util$(/)
ECHO := echo *
OHCE :=
RMDIR := rd /s /q
CP := copy
QUIET_ERRORS := >nul 2>&1 || true
BUILD_DIR_32 := build_w32
BUILD_DIR_64 := build_w64
DOXYAPISCRIPT := GenApiSpec.bat
DOXYCODESCRIPT := GenCodeSpec.bat
endif

# By default, hide all of the extra build info so errors are easy to see
ifneq ($(V),1)
.SILENT:
else
MESON_BLD_OPT += --verbose
SHOW_CFG := 1
# Figure out if we're doing a 32-bit or 64-bit build
ifeq ($(filter %32,$(MAKECMDGOALS)),)
BUILD_TYPE := 64
else
BUILD_TYPE := 32
endif
endif

export KCONFIG_CONFIG ?= Mayan_SilCfg
export KCONFIG_AUTOHEADER := $(KCONFIG_CONFIG).h
export KCONFIG_DIR := $(SIL_HOME)$(/)util$(/)kconfig$(/)lib$(/)
MESON_CFG_OPT += -DPlatKcfg=$(KCONFIG_CONFIG)

# We don't want any sort of real default target, so put help first
help:
	@$(ECHO) ================  Kconfig targets  ================$(OHCE)
	@$(ECHO) menuconfig      - TUI-based Kconfig menu to update options$(OHCE)
	@$(ECHO) guiconfig       - GUI-based Kconfig menu to update options$(OHCE)
	@$(ECHO) olddefconfig    - Sets all non-specified config options to default$(OHCE)
	@$(ECHO) savedefconfig   - Save current config as ./defconfig -- minimal config$(OHCE)
	@$(ECHO) defconfig       - New config with default from ARCH supplied defconfig$(OHCE)
	@$(ECHO) allnoconfig     - New config where all options are answered with no$(OHCE)
	@$(ECHO) allyesconfig    - New config where all options are accepted with yes$(OHCE)
	@$(ECHO) alldefconfig    - New config with all symbols set to default$(OHCE)
	@$(ECHO) ================  Build Targets  ================$(OHCE)
	@$(ECHO) set32 32        - Create 32-bit build environment$(OHCE)
	@$(ECHO) set64 64        - Create 64-bit build environment$(OHCE)
	@$(ECHO) build32 build64 - Build 32 or 64-bit opensil$(OHCE)
	@$(ECHO) clean           - Remove 32 and 64 bit build directories$(OHCE)
	@$(ECHO) ================  General Targets  ================$(OHCE)
	@$(ECHO) gitconfig       - Install git hooks$(OHCE)
	@$(ECHO) ================  Environment Variables  ================$(OHCE)
	@$(ECHO) KCONFIG_CONFIG  - Set to one of the files in the configs directory $(OHCE)
	@$(ECHO)                   defaults to $(KCONFIG_CONFIG).$(OHCE)
	@$(ECHO) OPT             - can be "wipe" or "clean".$(OHCE)
	@$(ECHO)                   wipe - Used with the build environment creation step to force recreation.$(OHCE)
	@$(ECHO)                   clean - Used with build step to start a new build instead of incremental.$(OHCE)
	@$(ECHO) V=1             - show makefile steps as they're run.$(OHCE)
	@$(ECHO) CFG_OPT         - Add additional meson configuration options.$(OHCE)
	@$(ECHO)                   '-Dunit_test=true' enables target for testing.$(OHCE)
	@$(ECHO) BLD_OPT         - Add additional meson build options.$(OHCE)

# KCONFIG
#########
KCONFIG_TARGETS := menuconfig guiconfig olddefconfig savedefconfig defconfig \
                   allnoconfig allyesconfig alldefconfig
.PHONY := $(KCONFIG_TARGETS)
$(KCONFIG_TARGETS):
	python $(KCONFIG_DIR)$@.py Kconfig

# Set up build folders
######################
ENV_TARGETS := W32 w32 L32 l32 Set32 set32 32 W64 w64 L64 l64 Set64 set64 64
.PHONY := $(ENV_TARGETS)
$(ENV_TARGETS):
	$(MESONSCRIPT) $@ "$(OPT)" $(CFG_OPT) $(MESON_CFG_OPT)

# Run the build - Calls into meson build scripts
###############
BUILD_TARGETS :=  Build32 build32 Build64 build64
.PHONY := $(BUILD_TARGETS)
$(BUILD_TARGETS):
ifneq ($(SHOW_CFG),)
	$(MESONSCRIPT) Show$(BUILD_TYPE)
endif
	$(MESONSCRIPT) $@ "" $(BLD_OPT) $(MESON_BLD_OPT)

DOXYGEN_TARGETS := docs$(/)API-html docs$(/)Code-html
doxygen: $(DOXYGEN_TARGETS)

docs$(/)API-html:
	cd Documentation$(/)ApiSpec && $(DOXYAPISCRIPT)
	@$(ECHO) Done. $(OHCE)

docs$(/)Code-html:
	cd Documentation$(/)CodeSpec && $(DOXYCODESCRIPT)
	@$(ECHO) Done. $(OHCE)

# Clean the source code
#######################
.PHONY := uncrustify
uncrustify:
	$(foreach reformat_file, $(shell git ls-files -- '*.c' '*.h'), uncrustify -l c -c .$(/)util$(/)uncrustify$(/)uncrustify.cfg --replace --no-backup $(reformat_file);)

# Destroy the build artifacts
#############################
.PHONY := clean
clean:
	-@$(RMDIR) $(SIL_HOME)$(/)$(BUILD_DIR_32) $(QUIET_ERRORS)
	-@$(RMDIR) $(SIL_HOME)$(/)$(BUILD_DIR_64) $(QUIET_ERRORS)
	@$(ECHO) System cleaned.$(OHCE)

.PHONY := doxyclean
doxyclean:
	-@$(RMDIR) $(DOXYGEN_TARGETS)
	-@$(RMDIR) Documentation$(/)ApiSpec$(/)build
	-@$(RMDIR) Documentation$(/)CodeSpec$(/)build

.PHONY := gitconfig
gitconfig:
	$(CP) util$(/)scripts$(/)pre-commit.sh .git$(/)hooks$(/)pre-commit
