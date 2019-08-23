#!/usr/bin/env bash

set -Eeuxo pipefail
SCRIPT_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
BD3_ROOT="$(realpath ${SCRIPT_ROOT}/../../)"
BAZELISK_PY=${BD3_ROOT}/bazelisk.py

if [ -z ${1+x} ]; then
  COMPILATION_MODE=fastbuild
else
  COMPILATION_MODE=$1
fi

python ${BAZELISK_PY} build --compilation_mode=${COMPILATION_MODE} //...
