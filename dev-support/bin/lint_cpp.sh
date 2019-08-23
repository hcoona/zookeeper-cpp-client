#!/usr/bin/env bash

set -Eeuxo pipefail
SCRIPT_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
BD3_ROOT="$(realpath ${SCRIPT_ROOT}/../../)"
BAZELISK_PY=${BD3_ROOT}/bazelisk.py

python ${BAZELISK_PY} build @cpplint//:cpplint
${BD3_ROOT}/bazel-bin/external/cpplint/cpplint \
  $(find . \
  -path ./output -prune -o \
  -path ./third_party -prune -o \
  -path ./third_party_src -prune -o \
  -path ./snappy -prune -o \
  -path ./storage/leveldb -prune -o \
  -path ./web_server -prune -o \
  \( -name '*.cc' -o -name '*.h' \) -print)
