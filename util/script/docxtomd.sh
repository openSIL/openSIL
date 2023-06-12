#!/bin/bash

declare -r SELF="$(basename $0)"
declare -r SELF_DIR="$(dirname $(readlink -f $0))"

declare -r PANDOC="$(which pandoc)"

function usage() {

    cat <<EOT

This script facilitates the translation of a docx document to Markdown.
Command 'pandoc' must be on PATH

Usage: ${SELF} [ -h | --help ] file

  -h|--help: prints this message
  file: docx document
EOT

    return 0
}

function check() {

    if [ -z ${PANDOC} ] ; then
        echo "${SELF}: ${FUNCNAME}: error: pandoc executable not on path. Is it installed?"
        return 1
    fi

    if [ -z ${DOC} ] ; then
        echo "${SELF}: ${FUNCNAME}: error: missing argument"
        usage
        return 1
    fi
}

while [ $# -gt 0 ] ; do

    case ${1} in
      -h|--help)
          usage
          exit 0
          ;;
      *)
          DOC="${1}"
           ;;
    esac
    shift
done

if ! check ; then
    exit 1
fi

if [ ! -f "${DOC}" ] ; then
    echo "${SELF}: error: ${DOC}: no such file..."
    exit 2
fi

MEDIA_DIR="$(echo ${DOC} | sed -e 's/\.docx//')"
MD_DOC="${MEDIA_DIR}.md"

if ! pandoc -t gfm -f docx -s --extract-media="${MEDIA_DIR}" "${DOC}" -o "${MD_DOC}" ; then
    exit 3
fi

echo "${SELF}: info: done. See ${MD_DOC} and ${MEDIA_DIR}/"
