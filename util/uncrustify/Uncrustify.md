# Uncrustify Code Beautifier Tool and Configuration

[TOC]

## Introduction
Uncrustify is a source code beautifier for C, C++, C#, Objective-C, D, Java, Pawn and Vala.  For the purposes of openSIL, it is used to manage the coding style within the openSIL libraries. The configurations will apply formatting as defined in the openSIL Coding Style guide.

## Installation

Supported Version: [Uncrustify 0.79.0](https://github.com/uncrustify/uncrustify/releases/tag/uncrustify-0.79.0)

Linux:

- Uncrustify is available from official repositories via `apt/yum install`
- If the package manager doesn't support 0.79.0, download the source from [GitHub](https://github.com/uncrustify/uncrustify)

Windows:

- Precompiled binaries/source available for download from [GitHub](https://github.com/uncrustify/uncrustify/releases) repository or [Sourceforge](https://sourceforge.net/projects/uncrustify/files/).



## Running Uncrustify Manually

Uncrustify can be run manually on a single file or list of files.

- To run on a single file:

  `uncrustify -c <CFG_FILE> -f <INPUT_FILE> -o <OUTPUT_FILE>`

  Where <INPUT_FILE> is the path and filename input

- To run on a list of files:

  `uncrustify -c <CFG_FILE> -F <INPUT_FILE>`

  Where <INPUT_FILE> is file with one filename (including path) per line. Output files will be appended with `.uncrustify` by default.

- To run on a git repo where the changes are tracked, there's no need to use separate output files. Instead, just replace the existing files:
  `for file in $(git ls-files | grep "\.[ch]$"); do uncrustify -c util/uncrustify/uncrustify.cfg -l c --replace --no-backup "${file}"; done`


## IDE Integration

Uncrustify supports integration with Visual Studio Code by installing the following prerequisites.

### VS Code extension: Uncrustify (Zachary Flower)

After installing the extension, configure the following settings:

| Extension Setting                            | Value                                                   |
| -------------------------------------------- | ------------------------------------------------------- |
| uncrustify.configPath.[Linux,Osx,Windows]    | Path to Uncrustify configuration file (uncrustify.cfg). |
| uncrustify.executablePath.[Linux,Osx,Windows | Path to the Uncrustify executable                       |

### VS Code Extension: Format Files (jbockle)

After installing the extension, configure the following settings:

| Extension Setting               | Value                                                |
| :------------------------------ | ---------------------------------------------------- |
| formatFiles.extensionsToInclude | Comma delimited list of extensions to included (c,h) |
| formatFiles.excludedFolders     | List of folder to ignore when formatting             |

### Running Uncrustify in VS Code

1. Open command palette (Ctrl+Shift+P)
2. Enter "Start Format Files: Workspace" and select.
3. Select the newly added workspace folder.
4. Select "Do it!"

**NOTE**: All updates will be performed and saved in the current directory. It is suggested to copy a folder to a new location and update the formatting there. The formatting output can then be compared to the original code to ensure the formatting is correct.

## Configuration File
The openSIL Uncrustify configuration file is located at `util/uncrustify/uncrustify.cfg`

It represents a subset of the configurations available in [default.cfg](https://github.com/uncrustify/uncrustify/blob/master/documentation/htdocs/default.cfg) for Uncrustify-0.79.0 that align with the openSIL Coding Style guide.

## Script Support
TBD

## Updating the config file

To update the config, generate the fully documented config file with the command:
  ` uncrustify -c util/uncrustify/uncrustify.cfg -o uncrustify.cfg --update-config-with-doc`

This can then be compared against the existing config.

Alternatively, use a tool like the [Uncrustify web configurator](https://github.com/CDanU/uncrustify_config).

# Licensing

SPDX-License-Identifier: MIT
Copyright (C) 2024 - 2025 Advanced Micro Devices, Inc. All rights reserved.

