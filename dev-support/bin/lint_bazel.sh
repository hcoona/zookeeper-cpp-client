#!/usr/bin/env bash

set -Eeuxo pipefail
SCRIPT_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
BD3_ROOT="$(realpath ${SCRIPT_ROOT}/../../)"
BAZELISK_PY=${BD3_ROOT}/bazelisk.py

python ${BAZELISK_PY} run //:buildifier_lint
